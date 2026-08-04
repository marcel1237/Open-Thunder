/* ============================================================
* Thunder SDK - Hardware-Enforced Acceleration Framework
* Copyright (C) 2025 Marcel
* ============================================================ */
#ifndef THUNDERCOMMON_H
#define THUNDERCOMMON_H

#include <QDebug>
#include <QFlags>
#include <QList>

#ifdef THUNDER_SHAREDLIBRARY
#define THUNDER_EXPORT Q_DECL_EXPORT
#else
#define THUNDER_EXPORT Q_DECL_IMPORT
#endif

#ifndef QSL
#define QSL(x) QStringLiteral(x)
#endif

namespace Td
{
// Version of Thunder SDK
extern const int sdkVersion;

THUNDER_EXPORT extern const char *APPNAME;
THUNDER_EXPORT extern const char *VERSION;
THUNDER_EXPORT extern const char *AUTHOR;
THUNDER_EXPORT extern const char *COPYRIGHT;
THUNDER_EXPORT extern const char *DESCRIPTION;

struct AuthorInfo
{
    QString name;
    QString email;
};

extern const QList<AuthorInfo> AUTHORS;

enum PerformanceLevel {
    Perf_Standard = 0,
    Perf_High = 1,
    Perf_Ultra = 2,
    Perf_HardwareLevel = 0x63 // Maximum Real-Time
};

}

#endif // THUNDERCOMMON_H
