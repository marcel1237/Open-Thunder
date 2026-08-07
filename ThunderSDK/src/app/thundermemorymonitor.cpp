/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#include "thundermemorymonitor.h"
#include <QHeaderView>
#include <QProcess>
#include <QTextEdit>
#include <QFontDatabase>
#include <QRegularExpression>

ThunderMemoryMonitor::ThunderMemoryMonitor(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("Thunder Matrix - Multiversal Paging Monitor");
    resize(1200, 800);

    setStyleSheet("background-color: #02040a; color: #e6edf3; font-family: 'Segoe UI', sans-serif;");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    m_totalLabel = new QLabel("⚡ OMNICORE SILICON MONITOR ACTIVE", this);
    m_totalLabel->setStyleSheet("font-size: 26px; font-weight: 900; color: #00f2ff; letter-spacing: 2px; margin: 10px;");
    mainLayout->addWidget(m_totalLabel);

    QHBoxLayout* splitLayout = new QHBoxLayout();
    splitLayout->setSpacing(20);

    // 1. Interactive Table (Left)
    m_table = new QTableWidget(0, 3, this);
    m_table->setHorizontalHeaderLabels({"RESOURCE NAME", "PID", "MEMORY (MB)"});
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->verticalHeader()->setVisible(false);
    m_table->setStyleSheet(
        "QTableWidget { border: 1px solid #30363d; background-color: #0d1117; border-radius: 8px; }"
        "QHeaderView::section { background-color: #161b22; color: #00f2ff; border: 1px solid #30363d; font-weight: bold; }"
    );
    splitLayout->addWidget(m_table, 2);

    // 2. Official Terminal View (Right)
    QTextEdit* terminalView = new QTextEdit(this);
    terminalView->setObjectName("terminalView");
    terminalView->setReadOnly(true);
    terminalView->setLineWrapMode(QTextEdit::NoWrap);
    terminalView->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    terminalView->setStyleSheet(
        "QTextEdit#terminalView { "
        " background-color: #000000; "
        " border: 2px solid #00f2ff; "
        " border-radius: 8px; "
        " padding: 20px; "
        " color: #00ff41; "
        " font-size: 13px; "
        "}"
    );
    splitLayout->addWidget(terminalView, 3);

    mainLayout->addLayout(splitLayout);

    m_refreshTimer = new QTimer(this);
    connect(m_refreshTimer, &QTimer::timeout, this, &ThunderMemoryMonitor::refreshStats);
    m_refreshTimer->start(3000);

    refreshStats();
}

void ThunderMemoryMonitor::refreshStats()
{
    // Execute the actual shell report tool
    QProcess* reportProc = new QProcess(this);
    QString reportPath = "/home/marcel1237/Thunder/Thunder Linux/build/Thunder_Paging_Report";

    connect(reportProc, &QProcess::finished, [this, reportProc](int exitCode) {
        if (exitCode == 0) {
            QString rawOutput = reportProc->readAllStandardOutput();
            QTextEdit* terminal = findChild<QTextEdit*>("terminalView");
            if (terminal) {
                // Remove ANSI color codes
                QString cleanOutput = rawOutput;
                cleanOutput.remove(QRegularExpression("\x1b\\[[0-9;]*m"));
                terminal->setPlainText(cleanOutput);
            }
        }
        reportProc->deleteLater();
    });

    reportProc->start(reportPath);

    // Also update the table independently for interactivity
    auto stats = getSystemMemoryUsage();
    m_table->setRowCount(0);
    for (const auto& info : stats) {
        int row = m_table->rowCount();
        m_table->insertRow(row);
        m_table->setItem(row, 0, new QTableWidgetItem(info.name));
        m_table->setItem(row, 1, new QTableWidgetItem(QString::number(info.pid)));
        QTableWidgetItem* memItem = new QTableWidgetItem(QString::number(info.memMB, 'f', 2) + " MB");
        memItem->setForeground(QColor(0, 255, 65));
        m_table->setItem(row, 2, memItem);
    }
}

QList<ThunderMemoryMonitor::ProcessInfo> ThunderMemoryMonitor::getSystemMemoryUsage()
{
    QList<ProcessInfo> result;
    QProcess ps;
    ps.start("ps", QStringList() << "-eo" << "comm,pid,rss" << "--sort=-rss" << "--no-headers");
    ps.waitForFinished();
    QString output = ps.readAllStandardOutput();
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    int count = 0;
    for (const QString& line : lines) {
        if (++count > 30) break;
        QStringList parts = line.split(' ', Qt::SkipEmptyParts);
        if (parts.size() >= 3) {
            ProcessInfo info;
            info.name = parts[0];
            info.pid = parts[1].toLong();
            info.memMB = parts[2].toDouble() / 1024.0;
            result.append(info);
        }
    }
    return result;
}
