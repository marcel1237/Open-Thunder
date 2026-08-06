/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
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
#include <QTabWidget>
#include <QComboBox>
#include <QCloseEvent>
#include "thundercommon.h"

class THUNDER_EXPORT BrowserWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit BrowserWindow(QWidget* parent = nullptr);
    ~BrowserWindow();

    QWebEngineView* currentView() const;

protected:
    void closeEvent(QCloseEvent *event) override;

private Q_SLOTS:
    void loadUrl();
    void executeSearch();
    void updateProgress(int progress);
    void updateTitle();
    void addNewTab(const QUrl &url = QUrl());
    void closeTab(int index);
    void currentTabChanged(int index);

private:
    void setupUi();
    void applyModernStyle();
    void saveSession();
    void restoreSession();

    QTabWidget* m_tabs;
    QLineEdit* m_addressBar;
    QLineEdit* m_searchBar;
    QComboBox* m_searchEngineSelector;
    QPushButton* m_backButton;
    QPushButton* m_forwardButton;
    QPushButton* m_reloadButton;
    QPushButton* m_addTabButton;
    QProgressBar* m_progressBar;
    QWidget* m_toolbar;
};

#endif // BROWSERWINDOW_H
