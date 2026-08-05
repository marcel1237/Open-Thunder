/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 * This code is public for visibility but use is governed by the TSAL v1.0.
 * Unauthorized commercial use or redistribution is strictly prohibited.
 */
#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#include <QApplication>
#include <QList>
#include "thundercommon.h"

class BrowserWindow;

class THUNDER_EXPORT MainApplication : public QApplication
{
    Q_OBJECT
public:
    explicit MainApplication(int &argc, char** argv);
    ~MainApplication();

    bool isClosing() const { return m_isClosing; }
    static MainApplication* instance();

private:
    bool m_isClosing = false;
    QList<BrowserWindow*> m_windows;
};

#endif // MAINAPPLICATION_H
