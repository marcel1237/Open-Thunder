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
#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QProgressBar>
#include "thundercommon.h"
#include "thunder_ui_constants.h"

class THUNDER_EXPORT BrowserWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit BrowserWindow(QWidget* parent = nullptr);
    ~BrowserWindow();

private Q_SLOTS:
    void loadUrl();
    void updateProgress(int progress);
    void updateTitle();

private:
    void setupUi();
    void applyHexStyle();

    QWebEngineView* m_view;
    QLineEdit* m_addressBar;
    QPushButton* m_backButton;
    QPushButton* m_forwardButton;
    QPushButton* m_reloadButton;
    QProgressBar* m_progressBar;
    QWidget* m_toolbar;
};

#endif // BROWSERWINDOW_H
