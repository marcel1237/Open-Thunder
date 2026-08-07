/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#ifndef THUNDERTRAY_H
#define THUNDERTRAY_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QObject>
#include <QMainWindow>
#include "thundermemorymonitor.h"

class ThunderTray : public QObject
{
    Q_OBJECT
public:
    explicit ThunderTray(QMainWindow* mainWindow, QObject* parent = nullptr);
    void show();

private slots:
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void toggleCommandCenter();
    void showHardwareReport();
    void openPagingReport();
    void terminateAllInstances();

private:
    QIcon createTrayIcon();
    QSystemTrayIcon* m_trayIcon;
    QMenu* m_trayMenu;
    QMainWindow* m_mainWindow;
    ThunderMemoryMonitor* m_memMonitor;
};

#endif // THUNDERTRAY_H
