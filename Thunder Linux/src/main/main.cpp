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
#include "mainapplication.h"
#include "kernel_bridge.h"

#include <iostream>
#include <cstdlib>

#ifndef Q_OS_WIN
void msgHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // ... (rest of msgHandler remains same or removed for speed)
}
#endif

int main(int argc, char* argv[])
{
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
    qInstallMessageHandler(&msgHandler);
#endif

    MainApplication app(argc, argv);


    if (app.isClosing())
        return 0;

    return app.exec();
}
