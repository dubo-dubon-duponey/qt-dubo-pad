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

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QTimer>
#include <QWidget>

#include <libdubopad/client.h>
#include <libdubopad/root.h>

#include <QWebChannel>

#include "helpers.h"

void OutputLibraryInfo(){
    DuboPad::Root * root = new DuboPad::Root();
    qDebug() << root->property("NAME");
    qDebug() << root->property("VENDOR");
    qDebug() << root->property("VERSION");
    qDebug() << root->property("REVISION");
    qDebug() << root->property("CHANGESET");
    qDebug() << root->property("BUILD");
    qDebug() << root->property("LINK");
    qDebug() << root->property("QT");
    qDebug() << root->property("PLUGIN_NAME");
    qDebug() << root->property("PLUGIN_VERSION");
    qDebug() << root->property("PLUGIN_REVISION");

}

void OutputConfiguration(DuboPad::Client * pad){
    qDebug() << "Using crashpad handler located in:" << pad->config->HandlerPath;
    qDebug() << "Storing crashes in:" << pad->config->CrashDirectory;
    qDebug() << "Reported product name, version, platform:" << pad->config->Product << pad->config->Version << pad->config->Platform;
    qDebug() << "Enable system crash reporter as well?" << pad->config->EnableSystemCrashReport;
    qDebug() << "Automatically upload crashes?" << pad->config->AutoUpload;
    qDebug() << "Rate limit uploads?" << pad->config->RateLimit;
    qDebug() << "Using server:" << pad->config->Server;
}

void Configure(DuboPad::Client * pad){
    // Get the path for crashpad_handler - if you put it somewhere else, change below
    QDir * d = new QDir(QCoreApplication::applicationDirPath());
    d->cdUp();
    d->cd("Helpers");

    pad->config->HandlerPath = d->filePath("crashpad_handler");

    // Get the app data path, create a Dubopad subdir to store the crah database
    QDir * e = new QDir(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0]);
    e->mkdir("Dubopad");
    e->cd("Dubopad");

    pad->config->CrashDirectory = e->path();

    // As an example, get the rest of the configuration from the Info.plist (mac only), or set it to whatever you want
    pad->config->Product = Helpers::ReadFromAppInfo("CFBundleName");
    pad->config->Version = Helpers::ReadFromAppInfo("CFBundleShortVersionString");
    pad->config->Platform = pad->config->PLATFORM_MAC();

    QString sys = Helpers::ReadFromAppInfo("DuboPadEnableSystemCrashReport");
    if (sys == "YES"){
        pad->config->EnableSystemCrashReport = true;
    }

    QString rl = Helpers::ReadFromAppInfo("DuboPadRateLimit");
    if (rl == "YES"){
        pad->config->RateLimit = true;
    }

    QString up = Helpers::ReadFromAppInfo("DuboPadAutoUpload");
    if (up == "NO"){
        pad->config->AutoUpload = false;
    }

    pad->config->Server = Helpers::ReadFromAppInfo("DuboPadServer");

    // Any additional info you want to pass along
    pad->config->Infos["random_foobar"] = "This is purely from QT side";
}

/**
 * Example implementation in a QT app
 */
int mainNoJavascript(int argc, char *argv[])
{
    // Get your app going
    QApplication app(argc, argv);

    // Create the client
    DuboPad::Client * pad = new DuboPad::Client();

    // Basic configuration
    Configure(pad);

    // Show something
    QWidget * r = new QWidget;
    r->show();

    // Just spit out library info
    OutputLibraryInfo();
    // Spit out current configuration
    OutputConfiguration(pad);

    // Start the crash handler
    pad->Start();

    // Shoot the app in 10... 9...
    QTimer::singleShot(10000, Helpers::nhehehehe);

    return app.exec();
}


/**
 * Exemple implementation delegating start to the WebView so that javascript can add more information
 * WARNING: use at your own risk...
 */
int mainJavascript(int argc, char *argv[])
{
    // Get your app going
    QApplication app(argc, argv);

    // Create the client
    DuboPad::Client * pad = new DuboPad::Client();

    // Basic configuration
    Configure(pad);

    // Display the webview
    QWebChannel * chan = Helpers::SetupWebView();

    // Attach objects to the javascript context
    DuboPad::Root * root = new DuboPad::Root();
    Helpers * crasher = new Helpers(root);

    chan->registerObject("Root", root);
    chan->registerObject("Dubo", pad);
    chan->registerObject("Crasher", crasher);

    return app.exec();
}

int main(int argc, char *argv[]){
    // Delegated to javascript
    return mainJavascript(argc, argv);
    // Purely c++
    // return mainNoJavascript(argc, argv);
}
