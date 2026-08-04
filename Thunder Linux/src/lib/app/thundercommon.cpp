/* ============================================================
* Thunder SDK - Hardware-Enforced Acceleration Framework
* Copyright (C) 2025 Marcel
* ============================================================ */
#include "thundercommon.h"

namespace Td
{
const int sdkVersion = 0x0001;

THUNDER_EXPORT const char *APPNAME = "Thunder SDK";
THUNDER_EXPORT const char *VERSION = "1.0.0-Core";
THUNDER_EXPORT const char *AUTHOR = "Marcel";
THUNDER_EXPORT const char *COPYRIGHT = "2025";
THUNDER_EXPORT const char *DESCRIPTION = "Hardware-Enforced High-Performance Computing Framework";

const QList<AuthorInfo> AUTHORS = {
    {QSL("Marcel"), QSL("marcel@thunder-sdk.org")},
};

}
