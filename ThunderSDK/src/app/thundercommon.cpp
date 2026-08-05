/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 * This code is public for visibility but use is governed by the TSAL v1.0.
 * Unauthorized commercial use or redistribution is strictly prohibited.
 */
#include "thundercommon.h"

namespace Td {

const int sdkVersion = 0x0001;

THUNDER_EXPORT const char *APPNAME = "Thunder SDK";
THUNDER_EXPORT const char *VERSION = "1.0.0-Core";
THUNDER_EXPORT const char *DESCRIPTION = "Hardware-Enforced High-Performance Computing Framework";

const QList<AuthorInfo> AUTHORS = {
    {QSL("Marcel"), QSL("marcel@thunder-sdk.org")},
};

} // namespace Td
