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
#include "browserwindow.h"
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
