/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#ifndef THUNDERMEMORYMONITOR_H
#define THUNDERMEMORYMONITOR_H

#include <QWidget>
#include <QTableWidget>
#include <QTimer>
#include <QVBoxLayout>
#include <QLabel>

class ThunderMemoryMonitor : public QWidget
{
    Q_OBJECT
public:
    explicit ThunderMemoryMonitor(QWidget* parent = nullptr);

public slots:
    void refreshStats();

private:
    QTableWidget* m_table;
    QTimer* m_refreshTimer;
    QLabel* m_totalLabel;

    struct ProcessInfo {
        QString name;
        long pid;
        double memMB;
    };

    QList<ProcessInfo> getSystemMemoryUsage();
};

#endif // THUNDERMEMORYMONITOR_H
