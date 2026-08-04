/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 * This code is public for visibility but use is governed by the TSAL v1.0.
 * Unauthorized commercial use or redistribution is strictly prohibited.
 */
#include <QMutex>
#include <QUrl>
#include <QWebEngineUrlRequestInfo>

#include "thundercommon.h"

#define ADBLOCK_EASYLIST_URL QSL("https://easylist-downloads.adblockplus.org/easylist.txt")
#define ADBLOCK_NOCOINLIST_URL QSL("https://raw.githubusercontent.com/hoshsadiq/adblock-nocoin-list/master/nocoin.txt")

class AdBlockRule;
class AdBlockDialog;
class AdBlockMatcher;
class AdBlockNeworkRequest;
class AdBlockCustomList;
class AdBlockSubscription;
class AdBlockUrlInterceptor;

struct AdBlockedRequest
{
    QUrl requestUrl;
    QUrl firstPartyUrl;
    QByteArray requestMethod;
    QWebEngineUrlRequestInfo::ResourceType resourceType;
    QWebEngineUrlRequestInfo::NavigationType navigationType;
    QString rule;
};
Q_DECLARE_METATYPE(AdBlockedRequest)

class THUNDER_EXPORT AdBlockManager : public QObject
{
    Q_OBJECT

public:
    AdBlockManager(QObject* parent = nullptr);
    ~AdBlockManager();

    void load();
    void save();

    bool isEnabled() const;
    bool canRunOnScheme(const QString &scheme) const;
    bool canBeBlocked(const QUrl &url) const;

    QString elementHidingRules(const QUrl &url) const;
    QString elementHidingRulesForDomain(const QUrl &url) const;

    QString elementRemoveRulesForDomain(const QUrl &url) const;

    AdBlockSubscription* subscriptionByName(const QString &name) const;
    QList<AdBlockSubscription*> subscriptions() const;

    bool block(AdBlockNeworkRequest &request, QString &ruleFilter, QString &ruleSubscription, QUrl &rewriteUrl);

    QVector<AdBlockedRequest> blockedRequestsForUrl(const QUrl &url) const;
    void clearBlockedRequestsForUrl(const QUrl &url);

    QStringList disabledRules() const;
    void addDisabledRule(const QString &filter);
    void removeDisabledRule(const QString &filter);

    bool addSubscriptionFromUrl(const QUrl &url);

    AdBlockSubscription* addSubscription(const QString &title, const QString &url);
    bool removeSubscription(AdBlockSubscription* subscription);

    AdBlockCustomList* customList() const;

    static AdBlockManager* instance();

Q_SIGNALS:
    void enabledChanged(bool enabled);
    void blockedRequestsChanged(const QUrl &url);

public Q_SLOTS:
    void setEnabled(bool enabled);
    void showRule();

    void updateMatcher();
    void updateAllSubscriptions();

    void requestBlocked(const AdBlockedRequest &request);

    AdBlockDialog *showDialog(QWidget *parent = nullptr);

private:
    bool m_loaded;
    bool m_enabled;

    QList<AdBlockSubscription*> m_subscriptions;
    AdBlockMatcher* m_matcher;
    QStringList m_disabledRules;

    AdBlockUrlInterceptor *m_interceptor;
    QPointer<AdBlockDialog> m_adBlockDialog;
    QMutex m_mutex;
    QHash<QUrl, QVector<AdBlockedRequest>> m_blockedRequests;
};

#endif // ADBLOCKMANAGER_H
