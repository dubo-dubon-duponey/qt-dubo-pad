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

#ifndef DUBOPAD_CONFIG_H
#define DUBOPAD_CONFIG_H

#include "global.h"
#include <QObject>
#include <QString>
#include <QMap>
#include <QVariant>

namespace DuboPad {

class LIBDUBOPADSHARED_EXPORT Config : public QObject
{
  Q_OBJECT

public:
    explicit Config(QObject * parent):QObject(parent){}

    Q_PROPERTY(QString Product  MEMBER Product  NOTIFY productChanged)
    Q_PROPERTY(QString Version  MEMBER Version  NOTIFY versionChanged)
    Q_PROPERTY(QString Platform MEMBER Platform NOTIFY platformChanged)
    Q_PROPERTY(QString Server   MEMBER Server   NOTIFY serverChanged)

    Q_PROPERTY(bool    EnableSystemCrashReport  MEMBER EnableSystemCrashReport  NOTIFY enableSystemCrashReportChanged)
    Q_PROPERTY(bool    AutoUpload               MEMBER AutoUpload               NOTIFY autoUploadChanged)
    Q_PROPERTY(bool    RateLimit                MEMBER RateLimit                NOTIFY rateLimitChanged)

    Q_PROPERTY(QString CrashDirectory   READ getCrashDirectory  CONSTANT)
    Q_PROPERTY(QString HandlerPath      READ getHandlerPath     CONSTANT)

    Q_PROPERTY(QVariant Infos   READ getInfos   WRITE setInfos  NOTIFY infosChanged)

    Q_PROPERTY(QString PLATFORM_MAC READ PLATFORM_MAC CONSTANT)
    Q_PROPERTY(QString PLATFORM_NUX READ PLATFORM_NUX CONSTANT)
    Q_PROPERTY(QString PLATFORM_WIN READ PLATFORM_WIN CONSTANT)

    QString Product = "MySuperApp";
    QString Version = "0.0.1";
    QString Platform = "OSX";
    QString Server = "http://localhost";
    bool EnableSystemCrashReport = false;
    bool AutoUpload = true;
    bool RateLimit = false;

    QString CrashDirectory = "";
    QString HandlerPath = "";
    QString getCrashDirectory()     const {return CrashDirectory;}
    QString getHandlerPath()        const {return HandlerPath;}

    QString PLATFORM_MAC()          const {return QString::fromLatin1("macOS");}
    QString PLATFORM_WIN()          const {return QString::fromLatin1("Windows");}
    QString PLATFORM_NUX()          const {return QString::fromLatin1("Linux");}

    QMap<QString, QString> Infos = QMap<QString, QString>();


signals:
    void productChanged();
    void versionChanged();
    void platformChanged();
    void serverChanged();
    void enableSystemCrashReportChanged();
    void autoUploadChanged();
    void rateLimitChanged();
    void infosChanged();

private:
    QVariant getInfos() const
    {
        QMap<QString, QVariant> list;
        QMap<QString, QString>::const_iterator i = Infos.constBegin();
        while (i != Infos.constEnd()) {
            list[i.key()] = QVariant(i.value());
            ++i;
        }

        return QVariant(list);
    }

    void setInfos(const QVariant & list)
    {
        Infos.clear();
        QMap<QString, QVariant> object = list.toMap();
        QMap<QString, QVariant>::const_iterator i = object.constBegin();
        while (i != object.constEnd()) {
            Infos[i.key()] = i.value().toString();
            ++i;
        }
    }
};

}

#endif // DUBOPAD_CONFIG_H
