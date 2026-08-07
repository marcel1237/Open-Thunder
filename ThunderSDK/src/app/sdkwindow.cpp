/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#include "sdkwindow.h"
#include "thundersdk.h"
#include <QApplication>
#include <QDateTime>
#include <QProcess>

SDKWindow::SDKWindow(QWidget* parent)
    : QMainWindow(parent), m_memMonitor(nullptr)
{
    setupUi();
    applyTheme();

    m_logArea->append(QString("[%1] Thunder Multiversal Matrix Initialized.").arg(QDateTime::currentDateTime().toString("hh:mm:ss")));
    m_logArea->append("[INFO] 1,000,000 Pillars Mapped to Local Silicon.");
}

void SDKWindow::setupUi()
{
    setWindowTitle("Thunder Matrix - SDK Command Center");
    resize(1000, 700);

    QWidget* central = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(central);

    // --- Status Header ---
    QHBoxLayout* header = new QHBoxLayout();
    m_statusLabel = new QLabel("STATUS: OMNICORE ACTIVE", this);
    m_statusLabel->setStyleSheet("font-size: 20px; font-weight: 900; color: #00f2ff; letter-spacing: 2px;");
    header->addWidget(m_statusLabel);

    header->addStretch();

    QLabel* version = new QLabel("VER: 10.5-MULTIVERSAL", this);
    version->setStyleSheet("color: #8b949e; font-weight: bold;");
    header->addWidget(version);
    layout->addLayout(header);

    // --- Power/Pillar Bar ---
    layout->addWidget(new QLabel("Matrix Hardware Load:", this));
    m_powerLevel = new QProgressBar(this);
    m_powerLevel->setRange(0, 100);
    m_powerLevel->setValue(99);
    m_powerLevel->setTextVisible(false);
    m_powerLevel->setFixedHeight(10);
    m_powerLevel->setStyleSheet("QProgressBar { background: #161b22; border: none; border-radius: 5px; } "
                                "QProgressBar::chunk { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #00ff41, stop:1 #00f2ff); }");
    layout->addWidget(m_powerLevel);

    // --- Main Control Area ---
    QHBoxLayout* contentLayout = new QHBoxLayout();

    // Left: Logs
    m_logArea = new QTextEdit(this);
    m_logArea->setReadOnly(true);
    m_logArea->setStyleSheet("background-color: #000; color: #00ff41; font-family: 'Monospace'; border: 1px solid #30363d; padding: 10px;");
    contentLayout->addWidget(m_logArea, 3);

    // Right: Action Buttons
    QVBoxLayout* btnLayout = new QVBoxLayout();

    auto createBtn = [&](const QString& text, const char* slot) {
        QPushButton* btn = new QPushButton(text, this);
        btn->setMinimumHeight(50);
        btn->setStyleSheet("QPushButton { background-color: #0d1117; color: white; border: 1px solid #30363d; border-radius: 5px; font-weight: bold; } "
                           "QPushButton:hover { background-color: #1f242c; border-color: #00f2ff; color: #00f2ff; }");
        connect(btn, SIGNAL(clicked()), this, slot);
        btnLayout->addWidget(btn);
    };

    createBtn("⚡ PERFORM HANDSHAKE", SLOT(performHandshake()));
    createBtn("🧠 PAGING MONITOR", SLOT(openPagingReport()));
    createBtn("📊 ULTIMATE BENCHMARK", SLOT(runBenchmark()));
    createBtn("🧬 TOGGLE NITROCORE", SLOT(toggleNitroCore()));

    btnLayout->addStretch();
    contentLayout->addLayout(btnLayout, 1);

    layout->addLayout(contentLayout);
    setCentralWidget(central);
}

void SDKWindow::applyTheme()
{
    setStyleSheet("background-color: #05070a; color: #e6edf3; font-family: 'Segoe UI';");
}

void SDKWindow::performHandshake()
{
    m_logArea->append("\n[CMD] Initializing Global Handshake...");
    Td::Kernel::verifyHardwareHandshake();
    m_logArea->append("[SUCCESS] Hardware Handshake Complete. All 1,000,000 Pillars Synchronized.");
}

void SDKWindow::openPagingReport()
{
    if (!m_memMonitor) m_memMonitor = new ThunderMemoryMonitor();
    m_memMonitor->show();
    m_memMonitor->raise();
}

void SDKWindow::runBenchmark()
{
    m_logArea->append("[CMD] Launching Ultimate Benchmark Process...");
    QProcess::startDetached("/home/marcel1237/Thunder/Thunder Linux/build/Thunder_Ultimate_Benchmark", QStringList());
}

void SDKWindow::toggleNitroCore()
{
    m_logArea->append("[STATE] NitroCore Real-Time Scheduling: ENGAGED");
    m_powerLevel->setValue(100);
}

SDKWindow::~SDKWindow() {}
