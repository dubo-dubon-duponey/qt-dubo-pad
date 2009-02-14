#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QTimer>
#include <QWidget>

#include <libdubopad/client.h>
#include <libdubopad/root.h>

// Define helper for plist info retrieval
QString ReadFromAppInfo(const QString key);

// Helper that crashes the application
void crashit(){
    // Crash it baby
    QString * dumb;
    QByteArray a = dumb->toLocal8Bit();
}

// helper that dumps version info about the library
void info()
{
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

// Main...
int main(int argc, char *argv[])
{
    // Get your app going
    QApplication app(argc, argv);

    // Just print out some info about the library
    info();

    // Get the path for crashpad_handler
    QDir * d = new QDir(QCoreApplication::applicationDirPath());
    d->cdUp();
    d->cd("Helpers");

    // Get the app data path, create a Dubopad subdir to store the crah database
    QDir * e = new QDir(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0]);
    e->mkdir("Dubopad");
    e->cd("Dubopad");

    // Create the client
    DuboPad::Client * c = new DuboPad::Client();

    // Configure now
    // Set the paths
    c->config->HandlerPath = d->filePath("crashpad_handler");
    c->config->CrashDirectory = e->path();

    // As an example, get the rest of the configuration from the Info.plist
    c->config->Product = ReadFromAppInfo("CFBundleName");
    c->config->Version = ReadFromAppInfo("CFBundleShortVersionString");
    c->config->Platform = c->config->PLATFORM_MAC();

    QString sys = ReadFromAppInfo("DuboPadEnableSystemCrashReport");
    if (sys == "YES"){
        c->config->EnableSystemCrashReport = true;
    }

    QString rl = ReadFromAppInfo("DuboPadRateLimit");
    if (rl == "YES"){
        c->config->RateLimit = true;
    }

    QString up = ReadFromAppInfo("DuboPadAutoUpload");
    if (up == "NO"){
        c->config->AutoUpload = false;
    }

    c->config->Server = ReadFromAppInfo("DuboPadServer");

    // Any additional info you want to pass along
    c->config->Infos["random_foobar"] = "some random_foobar";

    qDebug() << "Using crashpad handler located in:" << c->config->HandlerPath;
    qDebug() << "Storing crashes in:" << c->config->CrashDirectory;
    qDebug() << "Reported product name, version, platform:" << c->config->Product << c->config->Version << c->config->Platform;
    qDebug() << "Enable system crash reporter as well?" << c->config->EnableSystemCrashReport;
    qDebug() << "Automatically upload crashes?" << c->config->AutoUpload;
    qDebug() << "Rate limit uploads?" << c->config->RateLimit;
    qDebug() << "Using server:" << c->config->Server;


    // Display something useless...
    QWidget * r = new QWidget;
    r->show();

    // Start the crash reporter
    c->Start();

    // Shoot the app in 10... 9...
    QTimer::singleShot(10000, crashit);

    int a;
    a = app.exec();
    return a;
}
