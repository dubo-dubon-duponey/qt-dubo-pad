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
    explicit Config(QObject * parent):QObject(parent){
        Infos[QString::fromLatin1("product")] = QString::fromUtf8("My DuboPad App");
        Infos[QString::fromLatin1("version")] = QString::fromLatin1("0.0.1");
        Infos[QString::fromLatin1("platform")] = QSysInfo::productType();
        Infos[QString::fromLatin1("platform-version")] = QSysInfo::productVersion();
        Infos[QString::fromLatin1("cpu-build")] = QSysInfo::buildCpuArchitecture();
        Infos[QString::fromLatin1("cpu-runtime")] = QSysInfo::currentCpuArchitecture();
        // XXX nothing here, and not super useful (better done in javascript)
//        Infos[QString::fromLatin1("machine-uuid")] = QString::fromLatin1(QSysInfo::machineUniqueId());
//        Infos[QString::fromLatin1("boot-uuid")] = QString::fromLatin1(QSysInfo::bootUniqueId());
    }

    // Sugar, actually stored in Infos
    Q_PROPERTY(QString Product                  READ product WRITE setProduct   NOTIFY updated)
    Q_PROPERTY(QString Version                  READ version WRITE setVersion   NOTIFY updated)

    // Main data object
    Q_PROPERTY(QVariant Infos                   READ getInfos   WRITE setInfos  NOTIFY updated)

    // Configuration
    Q_PROPERTY(QString Server                   MEMBER Server                   NOTIFY updated)
    Q_PROPERTY(bool    EnableSystemCrashReport  MEMBER EnableSystemCrashReport  NOTIFY updated)
    Q_PROPERTY(bool    AutoUpload               MEMBER AutoUpload               NOTIFY updated)
    Q_PROPERTY(bool    RateLimit                MEMBER RateLimit                NOTIFY updated)

    // These cannot be changed from javascript by design
    Q_PROPERTY(QString CrashDirectory           READ getCrashDirectory          CONSTANT)
    Q_PROPERTY(QString HandlerPath              READ getHandlerPath             CONSTANT)

    QString Server = QString::fromUtf8("http://localhost");

    QString CrashDirectory = QString::fromLatin1("");
    QString HandlerPath = QString::fromLatin1("");

    QMap<QString, QString> Infos = QMap<QString, QString>();

    bool EnableSystemCrashReport = false;
    bool AutoUpload = true;
    bool RateLimit = false;

signals:
    void updated();

private:

    QString getCrashDirectory()     const {return CrashDirectory;}
    QString getHandlerPath()        const {return HandlerPath;}

    QString product(){
        QString key = QString::fromLatin1("product");
        if (Infos.find(key) == Infos.end())
            return QString::fromLatin1("");
        return Infos[key];
    }

    void setProduct(QString product){
        Infos[QString::fromLatin1("product")] = product;
        emit updated();
    }

    QString version(){
        QString key = QString::fromLatin1("version");
        if (Infos.find(key) == Infos.end())
            return QString::fromLatin1("");
        return Infos[key];
    }

    void setVersion(QString version){
        Infos[QString::fromLatin1("version")] = version;
        emit updated();
    }

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
        emit updated();
    }
};

}

#endif // DUBOPAD_CONFIG_H
