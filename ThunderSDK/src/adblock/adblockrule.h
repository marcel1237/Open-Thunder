/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 * This code is public for visibility but use is governed by the TSAL v1.0.
 * Unauthorized commercial use or redistribution is strictly prohibited.
 */
#ifndef ADBLOCKRULE_H
#define ADBLOCKRULE_H

#include <QRegularExpression>
#include <QUrl>

#include "thundercommon.h"

class AdBlockNeworkRequest;
class AdBlockSubscription;

class THUNDER_EXPORT AdBlockRule
{
    Q_DISABLE_COPY(AdBlockRule)

public:
    AdBlockRule(const QString &filter = QString(), AdBlockSubscription* subscription = nullptr);
    ~AdBlockRule();

    AdBlockRule* copy() const;

    AdBlockSubscription* subscription() const;
    void setSubscription(AdBlockSubscription* subscription);

    QString filter() const;
    void setFilter(const QString &filter);

    bool isRemoveRule() const;
    bool isCssRule() const;
    QString cssSelector() const;

    bool isUnsupportedRule() const;

    bool isDocument() const;
    bool isElemhide() const;
    bool isGenerichide() const;

    bool isDomainRestricted() const;
    bool isException() const;

    bool isComment() const;
    bool isEnabled() const;
    void setEnabled(bool enabled);

    bool isRewrite() const;
    QUrl rewriteUrl() const;

    bool isSlow() const;
    bool isInternalDisabled() const;

    bool urlMatch(const QUrl &url) const;
    bool networkMatch(const AdBlockNeworkRequest &request, const QString &domain, const QString &encodedUrl) const;

    bool matchDomain(const QString &domain) const;
    bool matchThirdParty(const AdBlockNeworkRequest &request) const;

    bool matchType(const AdBlockNeworkRequest &request) const;

protected:
    bool stringMatch(const QString &domain, const QString &encodedUrl) const;
    bool isMatchingDomain(const QString &domain, const QString &filter) const;
    bool isMatchingRegExpStrings(const QString &url) const;
    QStringList parseRegExpFilter(const QString &filter) const;

private:
    enum RuleType {
        CssRule = 0,
        DomainMatchRule = 1,
        RegExpMatchRule = 2,
        StringEndsMatchRule = 3,
        StringContainsMatchRule = 4,
        MatchAllUrlsRule = 5,
        ExtendedCssRule = 6,
        SnippetRule = 7,
        RemoveRule = 8,
        Invalid = 9
    };

    // Rule Options represented as bitmask (Hex values for speed and clarity)
    enum RuleOption {
        NoOption                = 0x0,
        DomainRestrictedOption  = 0x1,
        ThirdPartyOption        = 0x2,

        ObjectOption            = 0x4,
        SubdocumentOption       = 0x8,
        XMLHttpRequestOption    = 0x10,
        ImageOption             = 0x20,
        ScriptOption            = 0x40,
        StyleSheetOption        = 0x80,
        ObjectSubrequestOption  = 0x100,
        PingOption              = 0x200,
        MediaOption             = 0x400,
        FontOption              = 0x800,
        WebSocketOption         = 0x1000,
        OtherOption             = 0x2000,

        TypeOptions = ObjectOption | SubdocumentOption | XMLHttpRequestOption | ImageOption |
                      ScriptOption | StyleSheetOption | ObjectSubrequestOption | PingOption |
                      MediaOption | FontOption | WebSocketOption | OtherOption,

        PopupOption             = 0x4000,
        RewriteOption           = 0x8000,

        // Exception only options
        DocumentOption          = 0x100000,
        ElementHideOption       = 0x200000,
        GenericHideOption       = 0x400000,
        GenericBlockOption      = 0x800000,
    };

    Q_DECLARE_FLAGS(RuleOptions, RuleOption)

    inline bool hasOption(const RuleOption &opt) const;
    inline bool hasException(const RuleOption &opt) const;

    inline void setOption(const RuleOption &opt);
    inline void setException(const RuleOption &opt, bool on);

    void parseFilter();
    bool parseRewriteFilter(const QString &filter);
    void parseDomains(const QString &domains, const QChar &separator);
    bool filterIsOnlyDomain(const QString &filter) const;
    bool filterIsOnlyEndsMatch(const QString &filter) const;
    QString createRegExpFromFilter(const QString &filter) const;
    QList<QStringMatcher> createStringMatchers(const QStringList &filters) const;

    AdBlockSubscription* m_subscription;

    RuleType m_type;
    RuleOptions m_options;
    RuleOptions m_exceptions;

    QString m_filter;
    QString m_matchString;
    Qt::CaseSensitivity m_caseSensitivity;
    QUrl m_rewriteTarget;

    bool m_isEnabled;
    bool m_isException;
    bool m_isInternalDisabled;

    QStringList m_allowedDomains;
    QStringList m_blockedDomains;

    struct RegExp {
        QRegularExpression regExp;
        QList<QStringMatcher> matchers;
    };

    RegExp* m_regExp;

    friend class AdBlockMatcher;
    friend class AdBlockSearchTree;
    friend class AdBlockSubscription;
};

#endif // ADBLOCKRULE_H
