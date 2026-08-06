#ifndef THUNDER_ADBLOCKMANAGER_H
#define THUNDER_ADBLOCKMANAGER_H

#include <QReadWriteLock>
#include <QUrl>
#include <memory>
#include <vector>
#include "adblockrule.h"

class THUNDER_EXPORT AdBlockManager {
public:
    static AdBlockManager& instance();

    bool loadFromFile(const QString& path, QString* error = nullptr);
    bool loadRules(const QStringList& filters);
    bool shouldBlock(const QUrl& url) const;
    void clear();
    qsizetype ruleCount() const;

private:
    AdBlockManager() = default;
    mutable QReadWriteLock m_lock;
    std::vector<std::unique_ptr<AdBlockRule>> m_rules;
};

#endif
