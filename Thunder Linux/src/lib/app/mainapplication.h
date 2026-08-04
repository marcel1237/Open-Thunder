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
#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#include <QApplication>
#include <QList>
#include "thundercommon.h"

class BrowserWindow;

class THUNDER_EXPORT MainApplication : public QApplication
{
    Q_OBJECT
public:
    explicit MainApplication(int &argc, char** argv);
    ~MainApplication();

    bool isClosing() const { return m_isClosing; }
    static MainApplication* instance();

private:
    bool m_isClosing = false;
    QList<BrowserWindow*> m_windows;
};

#endif // MAINAPPLICATION_H
