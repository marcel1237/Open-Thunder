/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#ifndef SDKWINDOW_H
#define SDKWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QProgressBar>
#include "thundertray.h"
#include "thundermemorymonitor.h"

class SDKWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit SDKWindow(QWidget* parent = nullptr);
    ~SDKWindow();

private slots:
    void runBenchmark();
    void toggleNitroCore();
    void openPagingReport();
    void performHandshake();

private:
    void setupUi();
    void applyTheme();

    QTextEdit* m_logArea;
    QProgressBar* m_powerLevel;
    QLabel* m_statusLabel;
    ThunderMemoryMonitor* m_memMonitor;
};

#endif // SDKWINDOW_H
