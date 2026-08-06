#ifndef THUNDER_ADBLOCKRULE_H
#define THUNDER_ADBLOCKRULE_H

#include <QRegularExpression>
#include <QString>
#include <QUrl>
#include "app/thundercommon.h"

class THUNDER_EXPORT AdBlockRule {
public:
    explicit AdBlockRule(QString filter = {});

    const QString& filter() const { return m_filter; }
    bool isValid() const { return m_valid; }
    bool isException() const { return m_exception; }
    bool matches(const QUrl& url) const;

private:
    QString m_filter;
    QRegularExpression m_expression;
    bool m_valid = false;
    bool m_exception = false;
};

#endif
