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
#include "browserwindow.h"
#include "thunder_url_interceptor.h"
#include <QWebEngineSettings>
#include <QWebEngineProfile>
#include <QShortcut>
#include <QPalette>
#include <QApplication>

BrowserWindow::BrowserWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setupUi();
    applyHexStyle();

    // Low-latency UI Flags
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    // Application-wide performance attributes
    QApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    // Install Hex-Accelerated Network Interceptor
    QWebEngineProfile *profile = m_view->page()->profile();
    profile->setUrlRequestInterceptor(new Td::Network::ThunderUrlInterceptor(this));

    // Performance Optimizations for "Fastest Browser"
    QWebEngineSettings *settings = m_view->settings();
    settings->setAttribute(QWebEngineSettings::AutoLoadImages, true);
    settings->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    settings->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    settings->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled, false); // Disabled for instant response
    settings->setAttribute(QWebEngineSettings::ErrorPageEnabled, false);

    // Enable hardware acceleration
    settings->setAttribute(QWebEngineSettings::Accelerated2dCanvasEnabled, true);
    settings->setAttribute(QWebEngineSettings::WebGLEnabled, true);

    // Low-level Chromium optimizations via Environment (Kernel/Process level)
    // --enable-native-gpu-memory-buffers: Enables DMA-BUF for zero-copy hardware path
    // --enable-gpu-rasterization: Bypasses CPU for all drawing
    // --use-gl=egl: Direct hardware interface to EGL (Linux native)
    // --canvas-msaa-sample-count=4: Multi-sample anti-aliasing for text
    // --enable-font-antialiasing: HW accelerated font smoothing
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS",
            "--disable-gpu-vsync "
            "--enable-threaded-compositing "
            "--enable-zero-copy "
            "--ignore-gpu-blocklist "
            "--enable-native-gpu-memory-buffers "
            "--enable-gpu-rasterization "
            "--enable-oop-rasterization "
            "--use-gl=egl "
            "--canvas-msaa-sample-count=4 "
            "--enable-font-antialiasing "
            "--enable-subpixel-font-scaling "
            "--enable-features=CanvasOopRasterization,GpuRasterization,SkiaRenderer,ZstdContentEncoding,BrotliContentEncoding,Vulkan,VulkanFromANGLE,DefaultAngleVulkan,RawDraw "
            "--enable-hardware-cursors "
            "--enable-low-delay-input "
            "--use-vulkan "
            "--use-angle=vulkan "
            "--enable-native-gpu-memory-buffers "
            "--enable-gpu-memory-buffer-video-frames "
            "--use-vulkan=native "
            "--disable-vulkan-fallback-to-gl-for-testing "
            "--vulkan-heap-memory-limit=0 "
            "--enable-skia-graphite "
            "--enable-features=SkiaGraphite,VulkanBindless,VulkanFromANGLE,CanvasOopRasterization,GpuRasterization,SkiaRenderer,ZstdContentEncoding,BrotliContentEncoding,Vulkan,DefaultAngleVulkan,RawDraw,VulkanPipelineCache,PersistentShaderCache,VulkanImagelessFramebuffer,VulkanMemoryModel,VaapiVideoDecoder,VaapiIgnoreDriverChecks,AcceleratedVideoDecodeLinuxZeroCopyGL,VulkanSharedImage,VulkanDescriptorIndexing,VulkanQueuePriority "
            "--enable-zero-copy "
            "--enable-native-gpu-memory-buffers "
            "--use-gl=egl "
            "--enable-gpu-memory-buffer-video-frames "
            "--disable-vulkan-surface-intermediate-buffer "
            "--force-vulkan-full-screen-surface "
            "--disable-vulkan-fallback-to-gl-for-testing "
            "--gpu-program-cache-size-kb=1048576 "
            "--disable-gpu-watchdog");

    // [TH-11] Mesa Zero-Error & [TH-13] Pre-Compiled Shaders
    qputenv("MESA_NO_ERROR", "1");
    qputenv("MESA_VK_WSI_PRESENT_MODE", "mailbox");
    qputenv("RADV_PERFTEST", "nggc,sam,nogttspill,extra_queues"); // AMD Parallel Queues
    qputenv("ANV_ENABLE_PIPELINE_CACHE", "1"); // Intel Pipeline Cache

    // Pre-Compiled Hardware Shader Cache (Warming)
    qputenv("QTWEBENGINE_DISABLE_GPU_WATCHDOG", "1");
    qputenv("MESA_GLSL_CACHE_DISABLE", "0");
    qputenv("MESA_GLSL_CACHE_MAX_SIZE", "1G");

    // VA-API Hardware Acceleration for Linux Kernel
    qputenv("QT_VIDEO_ALLOW_HW_ACCEL", "1");
    qputenv("LIBVA_DRIVER_NAME", "iHD");

    connect(m_addressBar, &QLineEdit::returnPressed, this, &BrowserWindow::loadUrl);
    connect(m_view, &QWebEngineView::loadProgress, this, &BrowserWindow::updateProgress);
    connect(m_view, &QWebEngineView::titleChanged, this, &BrowserWindow::updateTitle);

    m_view->load(QUrl(QStringLiteral("https://www.google.com")));
}

void BrowserWindow::setupUi()
{
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Toolbar (Minimalist)
    m_toolbar = new QWidget(this);
    m_toolbar->setFixedHeight(Td::UI::ToolbarHeight);
    QHBoxLayout* toolbarLayout = new QHBoxLayout(m_toolbar);
    toolbarLayout->setContentsMargins(Td::UI::PaddingMedium, 0, Td::UI::PaddingMedium, 0);
    toolbarLayout->setSpacing(Td::UI::PaddingSmall);

    m_backButton = new QPushButton(QStringLiteral("<"), m_toolbar);
    m_forwardButton = new QPushButton(QStringLiteral(">"), m_toolbar);
    m_reloadButton = new QPushButton(QStringLiteral("R"), m_toolbar);
    m_addressBar = new QLineEdit(m_toolbar);

    m_backButton->setFixedSize(Td::UI::ButtonSize, Td::UI::ButtonSize);
    m_forwardButton->setFixedSize(Td::UI::ButtonSize, Td::UI::ButtonSize);
    m_reloadButton->setFixedSize(Td::UI::ButtonSize, Td::UI::ButtonSize);
    m_addressBar->setFixedHeight(Td::UI::AddressBarHeight);

    toolbarLayout->addWidget(m_backButton);
    toolbarLayout->addWidget(m_forwardButton);
    toolbarLayout->addWidget(m_reloadButton);
    toolbarLayout->addWidget(m_addressBar);

    // Progress Bar (Hex height 0x2)
    m_progressBar = new QProgressBar(this);
    m_progressBar->setFixedHeight(0x2);
    m_progressBar->setTextVisible(false);
    m_progressBar->setMaximum(100);

    // WebView
    m_view = new QWebEngineView(this);
    m_view->page()->setBackgroundColor(QColor::fromRgba(Td::UI::ColorBackground));

    layout->addWidget(m_toolbar);
    layout->addWidget(m_progressBar);
    layout->addWidget(m_view);

    setCentralWidget(centralWidget);
    resize(1280, 720);
}

void BrowserWindow::applyHexStyle()
{
    // Use Hex Palette for lightning fast UI rendering (bypassing heavy CSS engine)
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor::fromRgba(Td::UI::ColorBackground));
    pal.setColor(QPalette::WindowText, QColor::fromRgba(Td::UI::ColorText));
    pal.setColor(QPalette::Base, QColor::fromRgba(Td::UI::ColorToolbar));
    pal.setColor(QPalette::Text, QColor::fromRgba(Td::UI::ColorText));
    pal.setColor(QPalette::Button, QColor::fromRgba(Td::UI::ColorToolbar));
    pal.setColor(QPalette::Highlight, QColor::fromRgba(Td::UI::ColorAccent));
    setPalette(pal);

    // Minimal CSS for borders and padding, hex-driven
    m_addressBar->setStyleSheet(QString("QLineEdit { background-color: #%1; color: #%2; border: 1px solid #%3; padding-left: 5px; }")
        .arg(Td::UI::ColorBackground & 0xFFFFFF, 6, 16, QChar('0'))
        .arg(Td::UI::ColorText & 0xFFFFFF, 6, 16, QChar('0'))
        .arg(Td::UI::ColorBorder & 0xFFFFFF, 6, 16, QChar('0')));

    m_toolbar->setAutoFillBackground(true);
    m_progressBar->setStyleSheet(QString("QProgressBar::chunk { background-color: #%1; } QProgressBar { border: none; background: transparent; }")
        .arg(Td::UI::ColorAccent & 0xFFFFFF, 6, 16, QChar('0')));
}

void BrowserWindow::loadUrl()
{
    QString url = m_addressBar->text();
    if (!url.startsWith("http")) {
        url = "https://" + url;
    }
    m_view->load(QUrl(url));
}

void BrowserWindow::updateProgress(int progress)
{
    m_progressBar->setValue(progress);
    if (progress >= 100) m_progressBar->hide();
    else m_progressBar->show();
}

void BrowserWindow::updateTitle()
{
    setWindowTitle(QString("Thunder - %1").arg(m_view->title()));
    m_addressBar->setText(m_view->url().toString());
}

BrowserWindow::~BrowserWindow()
{
}
