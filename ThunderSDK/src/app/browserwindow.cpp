/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * Licensed under the terms of the Multi-License Agreement (13 licenses).
 * See LICENSE.md in the project root for full license details.
 */
#include "browserwindow.h"
#include "thunder_url_interceptor.h"
#include "thundercommon.h"
#include "thunder_ui_constants.h"
#include <QWebEngineProfile>
#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QWebEnginePage>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QShortcut>
#include <QPalette>
#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QTabBar>
#include <QComboBox>
#include <QIcon>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>

BrowserWindow::BrowserWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setupUi();
    applyModernStyle();

    // Low-latency UI Flags
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    // Initial Tab / Restore Session
    restoreSession();

    // Environment-level hardware forcing
    qputenv("MESA_SHADER_CACHE_DISABLE", "0");
    qputenv("MESA_SHADER_CACHE_MAX_SIZE", "1G");

    connect(m_addressBar, &QLineEdit::returnPressed, this, &BrowserWindow::loadUrl);
    connect(m_searchBar, &QLineEdit::returnPressed, this, &BrowserWindow::executeSearch);
    connect(m_tabs, &QTabWidget::currentChanged, this, &BrowserWindow::currentTabChanged);
    connect(m_tabs, &QTabWidget::tabCloseRequested, this, &BrowserWindow::closeTab);

    // Shortcuts
    new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_T), this, SLOT(addNewTab()));
    new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_W), this, [this](){ closeTab(m_tabs->currentIndex()); });
}

QWebEngineView* BrowserWindow::currentView() const
{
    return qobject_cast<QWebEngineView*>(m_tabs->currentWidget());
}

void BrowserWindow::addNewTab(const QUrl &url)
{
    QWebEngineView* view = new QWebEngineView(this);
    view->page()->setBackgroundColor(QColor::fromRgba(Td::UI::ColorBackground));

    // Configure Persistent Profile for Auth/Logins (Google, Microsoft, Yahoo, etc.)
    QString profilePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/ThunderProfile";
    QDir().mkpath(profilePath);

    QWebEngineProfile* profile = new QWebEngineProfile("ThunderDefault", view);
    profile->setPersistentStoragePath(profilePath);
    profile->setPersistentCookiesPolicy(QWebEngineProfile::ForcePersistentCookies);
    profile->setHttpCacheType(QWebEngineProfile::DiskHttpCache);
    profile->setHttpCacheMaximumSize(1024 * 1024 * 512); // 512MB Cache

    // Set page to use the persistent profile
    QWebEnginePage* page = new QWebEnginePage(profile, view);
    view->setPage(page);

    // Install Hex-Accelerated Network Interceptor per profile/view
    profile->setUrlRequestInterceptor(new Td::Network::ThunderUrlInterceptor(this));

    // Performance Settings
    QWebEngineSettings *settings = view->settings();
    settings->setAttribute(QWebEngineSettings::AutoLoadImages, true);
    settings->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    settings->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    settings->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled, false);
    settings->setAttribute(QWebEngineSettings::Accelerated2dCanvasEnabled, true);
    settings->setAttribute(QWebEngineSettings::WebGLEnabled, true);

    int index = m_tabs->addTab(view, "New Coordinate");
    m_tabs->setCurrentIndex(index);

    connect(view, &QWebEngineView::loadProgress, this, &BrowserWindow::updateProgress);
    connect(view, &QWebEngineView::titleChanged, this, &BrowserWindow::updateTitle);
    connect(view, &QWebEngineView::urlChanged, this, &BrowserWindow::updateTitle);

    if (!url.isEmpty()) {
        view->load(url);
    }
}

void BrowserWindow::closeTab(int index)
{
    if (m_tabs->count() > 1) {
        QWidget* widget = m_tabs->widget(index);
        m_tabs->removeTab(index);
        delete widget;
    } else {
        close();
    }
}

void BrowserWindow::currentTabChanged(int index)
{
    Q_UNUSED(index);
    updateTitle();
}

void BrowserWindow::setupUi()
{
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setObjectName("centralWidget");
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // --- Modern Toolbar ---
    m_toolbar = new QWidget(this);
    m_toolbar->setObjectName("mainToolbar");
    m_toolbar->setFixedHeight(Td::UI::ToolbarHeight);
    QHBoxLayout* toolbarLayout = new QHBoxLayout(m_toolbar);
    toolbarLayout->setContentsMargins(Td::UI::PaddingMedium, 0, Td::UI::PaddingMedium, 0);
    toolbarLayout->setSpacing(Td::UI::PaddingSmall);

    m_backButton = new QPushButton("←", m_toolbar);
    m_forwardButton = new QPushButton("→", m_toolbar);
    m_reloadButton = new QPushButton("⟳", m_toolbar);
    m_addTabButton = new QPushButton("+", m_toolbar);

    m_backButton->setFixedSize(Td::UI::ButtonSize, Td::UI::ButtonSize);
    m_forwardButton->setFixedSize(Td::UI::ButtonSize, Td::UI::ButtonSize);
    m_reloadButton->setFixedSize(Td::UI::ButtonSize, Td::UI::ButtonSize);
    m_addTabButton->setFixedSize(Td::UI::ButtonSize, Td::UI::ButtonSize);

    m_addressBar = new QLineEdit(m_toolbar);
    m_addressBar->setPlaceholderText("Enter coordinate (URL) or search query...");
    m_addressBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    m_searchEngineSelector = new QComboBox(m_toolbar);
    m_searchEngineSelector->addItem(QIcon(":/icons/icons/duckduckgo.ico"), "DuckDuckGo");
    m_searchEngineSelector->addItem(QIcon(":/icons/icons/brave.ico"), "Brave");
    m_searchEngineSelector->addItem(QIcon(":/icons/icons/google.ico"), "Google");
    m_searchEngineSelector->addItem(QIcon(":/icons/icons/bing.ico"), "Bing");
    m_searchEngineSelector->addItem(QIcon(":/icons/icons/yahoo.ico"), "Yahoo");
    m_searchEngineSelector->addItem(QIcon(":/icons/icons/wikipedia.ico"), "Wikipedia");
    m_searchEngineSelector->setFixedWidth(160);
    m_searchEngineSelector->setIconSize(QSize(16, 16));

    m_searchBar = new QLineEdit(m_toolbar);
    m_searchBar->setPlaceholderText("Search...");
    m_searchBar->setFixedWidth(200);

    toolbarLayout->addWidget(m_backButton);
    toolbarLayout->addWidget(m_forwardButton);
    toolbarLayout->addWidget(m_reloadButton);
    toolbarLayout->addWidget(m_addTabButton);
    toolbarLayout->addWidget(m_addressBar);
    toolbarLayout->addWidget(m_searchEngineSelector);
    toolbarLayout->addWidget(m_searchBar);

    connect(m_addTabButton, &QPushButton::clicked, this, [this](){ addNewTab(); });
    connect(m_backButton, &QPushButton::clicked, this, [this](){ if(currentView()) currentView()->back(); });
    connect(m_forwardButton, &QPushButton::clicked, this, [this](){ if(currentView()) currentView()->forward(); });
    connect(m_reloadButton, &QPushButton::clicked, this, [this](){ if(currentView()) currentView()->reload(); });

    // --- Progress Bar ---
    m_progressBar = new QProgressBar(this);
    m_progressBar->setFixedHeight(3);
    m_progressBar->setTextVisible(false);
    m_progressBar->setMaximum(100);
    m_progressBar->setStyleSheet("QProgressBar { background: transparent; border: none; } QProgressBar::chunk { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #00f2ff, stop:1 #ff00ea); }");

    // --- Tab Widget ---
    m_tabs = new QTabWidget(this);
    m_tabs->setTabsClosable(true);
    m_tabs->setMovable(true);
    m_tabs->setObjectName("mainTabs");

    layout->addWidget(m_toolbar);
    layout->addWidget(m_progressBar);
    layout->addWidget(m_tabs);

    setCentralWidget(centralWidget);
    resize(1280, 800);
}

void BrowserWindow::applyModernStyle()
{
    // Global Palette
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor::fromRgba(Td::UI::ColorBackground));
    pal.setColor(QPalette::WindowText, QColor::fromRgba(Td::UI::ColorText));
    pal.setColor(QPalette::Base, QColor::fromRgba(Td::UI::ColorBackground));
    pal.setColor(QPalette::Text, QColor::fromRgba(Td::UI::ColorText));
    setPalette(pal);

    // CSS Styling for UI Elements
    QString style = QString(
        "QWidget#centralWidget { background-color: #%1; }"
        "QWidget#mainToolbar { background-color: #%2; border-bottom: 1px solid #%3; }"
        "QLineEdit { background-color: #1a1f26; color: white; border: 1px solid #30363d; border-radius: %4px; padding: 0 15px; font-size: 14px; height: %5px; }"
        "QLineEdit:focus { border: 1px solid #00f2ff; background-color: #000000; }"
        "QComboBox { background-color: #1a1f26; color: white; border: 1px solid #30363d; border-radius: %4px; padding-left: 10px; font-size: 13px; height: %5px; }"
        "QComboBox::drop-down { border: none; }"
        "QComboBox QAbstractItemView { background-color: #0d1117; color: white; selection-background-color: #1f242c; }"
        "QPushButton { background-color: transparent; color: #c9d1d9; border: none; border-radius: %4px; font-size: 20px; font-weight: bold; }"
        "QPushButton:hover { background-color: #1f242c; color: #00f2ff; }"
        "QPushButton:pressed { background-color: #0d1117; }"
        "QTabWidget::pane { border: none; }"
        "QTabBar::tab { background: #161b22; color: #8b949e; padding: 10px 20px; border-top-left-radius: 8px; border-top-right-radius: 8px; margin-right: 2px; font-weight: bold; }"
        "QTabBar::tab:selected { background: #1f242c; color: #00f2ff; border-bottom: 2px solid #00f2ff; }"
        "QTabBar::tab:hover { background: #1f242c; color: white; }"
        "QTabBar::close-button { image: url(none); subcontrol-position: right; }"
    )
    .arg(Td::UI::ColorBackground & 0xFFFFFF, 6, 16, QChar('0'))
    .arg(Td::UI::ColorToolbar & 0xFFFFFF, 6, 16, QChar('0'))
    .arg(Td::UI::ColorBorder & 0xFFFFFF, 6, 16, QChar('0'))
    .arg(Td::UI::BorderRadius)
    .arg(Td::UI::AddressBarHeight);

    setStyleSheet(style);
}

void BrowserWindow::loadUrl()
{
    if (!currentView()) return;
    QString input = m_addressBar->text().trimmed();
    if (input.isEmpty()) return;

    // Logic to distinguish between URL and Search Query
    bool isUrl = input.contains('.') && !input.contains(' ');
    if (input.startsWith("http://") || input.startsWith("https://") || input.startsWith("file://")) {
        isUrl = true;
    }

    if (isUrl) {
        if (!input.contains("://")) {
            input = "https://" + input;
        }
        currentView()->load(QUrl(input));
    } else {
        // Redirect to search engine
        QString engine = m_searchEngineSelector->currentText();
        QString searchUrl;
        if (engine == "Brave") searchUrl = "https://search.brave.com/search?q=%1";
        else if (engine == "Google") searchUrl = "https://www.google.com/search?q=%1";
        else if (engine == "Bing") searchUrl = "https://www.bing.com/search?q=%1";
        else if (engine == "Yahoo") searchUrl = "https://search.yahoo.com/search?p=%1";
        else if (engine == "Wikipedia") searchUrl = "https://en.wikipedia.org/wiki/Special:Search?search=%1";
        else searchUrl = "https://duckduckgo.com/?q=%1";

        currentView()->load(QUrl(searchUrl.arg(input)));
    }
}

void BrowserWindow::executeSearch()
{
    if (!currentView()) return;
    QString query = m_searchBar->text();
    if (query.isEmpty()) return;

    QString engine = m_searchEngineSelector->currentText();
    QString searchUrl;

    if (engine == "Brave") {
        searchUrl = "https://search.brave.com/search?q=%1";
    } else if (engine == "Google") {
        searchUrl = "https://www.google.com/search?q=%1";
    } else if (engine == "Bing") {
        searchUrl = "https://www.bing.com/search?q=%1";
    } else if (engine == "Yahoo") {
        searchUrl = "https://search.yahoo.com/search?p=%1";
    } else if (engine == "Wikipedia") {
        searchUrl = "https://en.wikipedia.org/wiki/Special:Search?search=%1";
    } else {
        searchUrl = "https://duckduckgo.com/?q=%1";
    }

    currentView()->load(QUrl(searchUrl.arg(query)));
}

void BrowserWindow::updateProgress(int progress)
{
    m_progressBar->setValue(progress);
    if (progress >= 100) m_progressBar->hide();
    else m_progressBar->show();
}

void BrowserWindow::updateTitle()
{
    if (!currentView()) return;
    QString title = currentView()->title();
    if (title.isEmpty()) title = "Thunder Node";

    int index = m_tabs->currentIndex();
    m_tabs->setTabText(index, title);

    setWindowTitle(QString("Thunder - %1").arg(title));
    m_addressBar->setText(currentView()->url().toString());
}

void BrowserWindow::saveSession()
{
    QSettings settings("Thunder", "Session");
    QStringList urls;
    for (int i = 0; i < m_tabs->count(); ++i) {
        QWebEngineView* view = qobject_cast<QWebEngineView*>(m_tabs->widget(i));
        if (view) {
            urls.append(view->url().toString());
        }
    }
    settings.setValue("urls", urls);
    settings.setValue("currentIndex", m_tabs->currentIndex());
}

void BrowserWindow::restoreSession()
{
    QSettings settings("Thunder", "Session");
    QStringList urls = settings.value("urls").toStringList();
    int currentIndex = settings.value("currentIndex", 0).toInt();

    if (urls.isEmpty()) {
        addNewTab(QUrl(QStringLiteral("https://duckduckgo.com")));
    } else {
        for (const QString& urlStr : urls) {
            addNewTab(QUrl(urlStr));
        }
        if (currentIndex < m_tabs->count()) {
            m_tabs->setCurrentIndex(currentIndex);
        }
    }
}

void BrowserWindow::closeEvent(QCloseEvent *event)
{
    saveSession();
    QMainWindow::closeEvent(event);
}

BrowserWindow::~BrowserWindow()
{
}
