/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#include <QApplication>
#include "sdkwindow.h"
#include "thundertray.h"
#include "thundersdk.h"

int main(int argc, char *argv[])
{
    // Initialize Thunder Matrix Core
    Td::initializeHardwareAcceleration();

    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false); // Keep app running when window is hidden

    // Create the Command Center Window (BUT DO NOT SHOW IT)
    SDKWindow* window = new SDKWindow();

    // Initialize Tray and link it to the window
    ThunderTray tray(window);
    tray.show();

    return app.exec();
}
