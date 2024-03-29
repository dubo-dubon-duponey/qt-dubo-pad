/*
 * Copyright (c) 2019, Dubo Dubon Duponey <dubodubonduponey+github@pm.me>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef DUBOPAD_HELPERS_H
#define DUBOPAD_HELPERS_H

#include <QObject>
#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
#include <QtWebEngine>
#endif
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QFileInfo>
#include <QDir>
#include <QWebChannel>

class Helpers: public QObject
{
  Q_OBJECT

public:
    Helpers(QObject * parent = nullptr):QObject(parent){}

    // Make it crash
    Q_INVOKABLE static void nhehehehe(){
        QString * dumb = nullptr;
        QByteArray a = dumb->toLocal8Bit();
    }

    // Get data from plist
    static QString ReadFromAppInfo(const QString key);

    // Simple webengine init
    static QWebChannel * SetupWebView()
    {
        QFileInfo jsFileInfo(QDir::currentPath() + QString::fromLatin1("/qwebchannel.js"));

        if (!jsFileInfo.exists())
            QFile::copy(QString::fromLatin1(":/qtwebchannel/qwebchannel.js"), jsFileInfo.absoluteFilePath());

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
    QtWebEngine::initialize();
#endif
        QWebEngineView * view = new QWebEngineView();

        QWebChannel * channel = new QWebChannel(view->page());
        view->page()->setWebChannel(channel);

        view->load(QUrl(QString::fromLatin1("qrc:/demo.html")));
        view->show();

        return channel;
    }

};

#endif
