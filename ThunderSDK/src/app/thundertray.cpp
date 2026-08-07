/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#include "thundertray.h"
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QProcess>
#include <QApplication>
#include <iostream>

ThunderTray::ThunderTray(QMainWindow* mainWindow, QObject* parent)
    : QObject(parent), m_mainWindow(mainWindow), m_memMonitor(nullptr)
{
    m_trayMenu = new QMenu();

    // New: Command Center Toggle
    QAction* cmdAction = m_trayMenu->addAction("📦 Command Center");
    QAction* pagingAction = m_trayMenu->addAction("🧠 Report de Paginação");
    QAction* reportAction = m_trayMenu->addAction("📋 Show Hardware Report");
    m_trayMenu->addSeparator();
    QAction* quitAction = m_trayMenu->addAction("❌ Terminate Thunder Matrix");

    connect(cmdAction, &QAction::triggered, this, &ThunderTray::toggleCommandCenter);
    connect(pagingAction, &QAction::triggered, this, &ThunderTray::openPagingReport);
    connect(reportAction, &QAction::triggered, this, &ThunderTray::showHardwareReport);
    connect(quitAction, &QAction::triggered, this, &ThunderTray::terminateAllInstances);

    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setContextMenu(m_trayMenu);
    m_trayIcon->setIcon(createTrayIcon());
    m_trayIcon->setToolTip("Thunder Multiversal Matrix: ACTIVE");

    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &ThunderTray::onTrayIconActivated);
}

QIcon ThunderTray::createTrayIcon()
{
    QPixmap pixmap(64, 64);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    path.moveTo(35, 5);
    path.lineTo(15, 35);
    path.lineTo(30, 35);
    path.lineTo(25, 60);
    path.lineTo(50, 25);
    path.lineTo(35, 25);
    path.closeSubpath();

    painter.setBrush(QColor(255, 242, 0));
    painter.setPen(QPen(Qt::black, 2));
    painter.drawPath(path);
    painter.end();

    return QIcon(pixmap);
}

void ThunderTray::show() { m_trayIcon->show(); }

void ThunderTray::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) {
        toggleCommandCenter();
    }
}

void ThunderTray::toggleCommandCenter()
{
    if (m_mainWindow) {
        if (m_mainWindow->isVisible()) {
            m_mainWindow->hide();
        } else {
            m_mainWindow->show();
            m_mainWindow->raise();
            m_mainWindow->activateWindow();
        }
    }
}

void ThunderTray::showHardwareReport()
{
    QProcess::startDetached("/home/marcel1237/Thunder/Thunder Linux/build/Thunder_Ultimate_Benchmark", QStringList());
}

void ThunderTray::terminateAllInstances()
{
    std::cout << "[⚡] Global Termination Signal Sent to Matrix..." << std::endl;
    QProcess::execute("pkill", QStringList() << "-9" << "-f" << "Thunder");
    qApp->quit();
}

void ThunderTray::openPagingReport()
{
    QProcess::startDetached("/home/marcel1237/Thunder/Thunder Linux/build/Thunder_Paging_Report", QStringList());
    if (!m_memMonitor) {
        m_memMonitor = new ThunderMemoryMonitor();
        m_memMonitor->setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    }
    m_memMonitor->show();
    m_memMonitor->raise();
}
