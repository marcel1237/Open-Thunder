/* ============================================================
* Thunder - Qt web browser
* Copyright (C) 2025 Marcel
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* ============================================================ */
#include "adblockrule.h"
#include "thunder_branch_optimizer.h"

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
