/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 * This code is public for visibility but use is governed by the TSAL v1.0.
 * Unauthorized commercial use or redistribution is strictly prohibited.
 */
#include "mainapplication.h"
#include "browserwindow.h"
#include "thundercommon.h"
#include "kernel_bridge.h"
#include <QStandardPaths>
#include <QDir>

MainApplication::MainApplication(int &argc, char** argv)
    : QApplication(argc, argv)
{
    setApplicationName(QString::fromLatin1(Td::APPNAME));
    setApplicationVersion(QString::fromLatin1(Td::VERSION));

    // Redirect ALL storage to Hardware RAM-Drive
    QString ramPath = Td::Kernel::initRamStorage();
    if (!ramPath.isEmpty()) {
        qputenv("XDG_CACHE_HOME", ramPath.toLocal8Bit());
        qputenv("XDG_CONFIG_HOME", ramPath.toLocal8Bit());
        qputenv("XDG_DATA_HOME", ramPath.toLocal8Bit());

        // Force Qt to use our Hex-RAM path for everything
        QDir().mkpath(ramPath);
    }

    BrowserWindow* window = new BrowserWindow();
    window->show();
    m_windows.append(window);
}

MainApplication::~MainApplication()
{
}

MainApplication* MainApplication::instance()
{
    return qobject_cast<MainApplication*>(QCoreApplication::instance());
}
