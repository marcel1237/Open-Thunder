#include "adblockrule.h"

AdBlockRule::AdBlockRule(QString filter)
    : m_filter(std::move(filter)) {
    m_filter = m_filter.trimmed();
    if (m_filter.isEmpty() || m_filter.startsWith('!') ||
        (m_filter.startsWith('[') && m_filter.endsWith(']')) ||
        m_filter.contains("##")) return;

    QString pattern = m_filter;
    if (pattern.startsWith("@@")) {
        m_exception = true;
        pattern.remove(0, 2);
    }

    if (pattern.startsWith("||")) {
        pattern.remove(0, 2);
        if (pattern.endsWith('^')) pattern.chop(1);
        const QString host = QRegularExpression::escape(pattern);
        pattern = QStringLiteral(R"(^https?://([^/]*\.)?%1(?=[:/]|$))").arg(host);
    } else {
        const bool startAnchored = pattern.startsWith('|');
        const bool endAnchored = pattern.endsWith('|');
        if (startAnchored) pattern.remove(0, 1);
        if (endAnchored && !pattern.isEmpty()) pattern.chop(1);
        pattern = QRegularExpression::escape(pattern);
        pattern.replace(QStringLiteral("\\*"), QStringLiteral(".*"));
        pattern.replace(QStringLiteral("\\^"), QStringLiteral(R"((?:[^\w.%_-]|$))"));
        if (startAnchored) pattern.prepend('^');
        if (endAnchored) pattern.append('$');
    }

    m_expression = QRegularExpression(pattern, QRegularExpression::CaseInsensitiveOption);
    m_valid = m_expression.isValid() && !pattern.isEmpty();
}

bool AdBlockRule::matches(const QUrl& url) const {
    return m_valid && url.isValid() && m_expression.match(url.toString(QUrl::FullyEncoded)).hasMatch();
}
