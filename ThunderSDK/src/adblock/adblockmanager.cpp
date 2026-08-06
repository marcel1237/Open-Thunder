#include "adblockmanager.h"
#include <QFile>
#include <QTextStream>

AdBlockManager& AdBlockManager::instance() {
    static AdBlockManager manager;
    return manager;
}

bool AdBlockManager::loadFromFile(const QString& path, QString* error) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        if (error) *error = file.errorString();
        return false;
    }
    QStringList filters;
    QTextStream stream(&file);
    while (!stream.atEnd()) filters.append(stream.readLine());
    return loadRules(filters);
}

bool AdBlockManager::loadRules(const QStringList& filters) {
    std::vector<std::unique_ptr<AdBlockRule>> parsed;
    parsed.reserve(static_cast<size_t>(filters.size()));
    for (const QString& filter : filters) {
        auto rule = std::make_unique<AdBlockRule>(filter);
        if (rule->isValid()) parsed.push_back(std::move(rule));
    }
    QWriteLocker locker(&m_lock);
    m_rules = std::move(parsed);
    return true;
}

bool AdBlockManager::shouldBlock(const QUrl& url) const {
    QReadLocker locker(&m_lock);
    bool blocked = false;
    for (const auto& rule : m_rules) {
        if (!rule->matches(url)) continue;
        if (rule->isException()) return false;
        blocked = true;
    }
    return blocked;
}

void AdBlockManager::clear() {
    QWriteLocker locker(&m_lock);
    m_rules.clear();
}

qsizetype AdBlockManager::ruleCount() const {
    QReadLocker locker(&m_lock);
    return static_cast<qsizetype>(m_rules.size());
}
