/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#ifndef THUNDERCOMMON_H
#define THUNDERCOMMON_H

#include <QDebug>
#include <QFlags>
#include <QList>
#include <cstdint>

#ifdef THUNDER_SHAREDLIBRARY
#define THUNDER_EXPORT Q_DECL_EXPORT
#else
#define THUNDER_EXPORT Q_DECL_IMPORT
#endif

#ifndef QSL
#define QSL(x) QStringLiteral(x)
#endif

#if defined(__GNUC__) || defined(__clang__)
#define TD_LIKELY(x)   __builtin_expect(!!(x), 1)
#define TD_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define TD_LIKELY(x)   (x)
#define TD_UNLIKELY(x) (x)
#endif

namespace Td {
extern const int sdkVersion;
THUNDER_EXPORT extern const char *APPNAME;
THUNDER_EXPORT extern const char *VERSION;
THUNDER_EXPORT extern const char *DESCRIPTION;

struct AuthorInfo {
    QString name;
    QString email;
};
extern const QList<AuthorInfo> AUTHORS;

enum PerformanceLevel {
    Perf_Standard = 0,
    Perf_High = 1,
    Perf_Ultra = 2,
    Perf_HardwareLevel = 0x63
};
}

#endif // THUNDERCOMMON_H
