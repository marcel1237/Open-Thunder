/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 * This code is public for visibility but use is governed by the TSAL v1.0.
 * Unauthorized commercial use or redistribution is strictly prohibited.
 */
#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QWebEngineView>
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
