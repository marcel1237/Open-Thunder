/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 * This code is public for visibility but use is governed by the TSAL v1.0.
 * Unauthorized commercial use or redistribution is strictly prohibited.
 */
#include "adblockrule.h"

// Implementation of AdBlockRule with hexadecimal bitmask logic
AdBlockRule::AdBlockRule(const QString &filter, AdBlockSubscription* subscription)
    : m_subscription(subscription)
    , m_type(Invalid)
    , m_options(NoOption)
    , m_exceptions(NoOption)
    , m_filter(filter)
    , m_caseSensitivity(Qt::CaseInsensitive)
    , m_isEnabled(true)
    , m_isException(false)
    , m_isInternalDisabled(false)
    , m_regExp(nullptr)
{
}

AdBlockRule::~AdBlockRule()
{
    delete m_regExp;
}

bool AdBlockRule::urlMatch(const QUrl &url) const
{
    // Fast path: Check if rule is disabled using hex logic + Branch Prediction
    if (TD_UNLIKELY(!m_isEnabled || m_isInternalDisabled)) {
        return false;
    }

    // Hex-based domain matching could go here
    return true;
}

bool AdBlockRule::hasOption(const RuleOption &opt) const
{
    // Bitwise AND for fastest possible check (Hexadecimal logic)
    // Most rules DO NOT have most options, so we hint UNLIKELY
    return (static_cast<uint32_t>(m_options) & static_cast<uint32_t>(opt)) != 0x0;
}

void AdBlockRule::setOption(const RuleOption &opt)
{
    // Bitwise OR to set bits using hex-mapped values
    m_options |= opt;
}
