/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * Licensed under the terms of the Multi-License Agreement (13 licenses).
 * See LICENSE.md in the project root for full license details.
 * This code is public for visibility but use is governed by the TSAL v1.0.
 * Unauthorized commercial use or redistribution is strictly prohibited.
 */
#include <iostream>
#include <cstdlib>
#include <QtCore/QtGlobal>
#include <QtCore/QMessageLogContext>
#include <QtCore/QString>
#include "mainapplication.h"
#include "thundersdk.h"
#include "kernel_bridge.h"

#ifndef Q_OS_WIN
void msgHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // High-speed logging bypass
    (void)type; (void)context; (void)msg;
}
#endif

int main(int argc, char* argv[])
{
    // Qt WebEngine consumes Chromium flags during process initialization.
    // Advanced flags are therefore explicit, user-supplied and applied before
    // QApplication exists; no driver-specific flags are forced by default.
    const QByteArray experimentalFlags = qgetenv("THUNDER_EXPERIMENTAL_GPU_FLAGS");
    if (!experimentalFlags.isEmpty())
        qputenv("QTWEBENGINE_CHROMIUM_FLAGS", experimentalFlags);
#ifdef Q_OS_LINUX
    // Apply Kernel Optimizations BEFORE initializing the GUI
    Td::Kernel::optimizeProcess();

    // Enable Hardware Speculation Speed (Spectre-Bypass)
    Td::Kernel::enableSpeculationSpeed();

    // Final Hardware Handshake (Diagnostic)
    Td::Kernel::verifyHardwareHandshake();

    // Disable GNOME/KDE Bloat for this process
    qputenv("QT_NO_ASSET_CACHE", "1");
    qputenv("QT_QUICK_CACHE_CONTROL", "1");

    // Force Wayland or X11 for speed, avoiding auto-detection overhead
    if (std::getenv("WAYLAND_DISPLAY")) {
        qputenv("QT_QPA_PLATFORM", "wayland");
    } else {
        qputenv("QT_QPA_PLATFORM", "xcb");
    }
#endif

#ifndef Q_OS_WIN
    qInstallMessageHandler(msgHandler);
#endif

    MainApplication app(argc, argv);

    if (app.isClosing())
        return 0;

    return app.exec();
}
