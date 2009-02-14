#include "client/crashpad_client.h"
#include "client/crashpad_info.h"
#include "client/settings.h"
#include "base/files/file_path.h"
#include "client/crash_report_database.h"

#include "libdubopad/client.h"

namespace DuboPad{

Client::Client(QObject * parent): QObject(parent)
{
    config = new Config(this);
}

int Client::Start()
{
    // Get the path to the local database and handler
    base::FilePath db(config->CrashDirectory.toStdString());
    base::FilePath handler(config->HandlerPath.toStdString());

    // Create the annotations object
    std::map<std::string, std::string> annotations;
    // Copy over prod, ver, plat
    annotations["product"] = config->Product.toStdString();
    annotations["version"]  = config->Version.toStdString();
    annotations["platform"] = config->Platform.toStdString();
    // Copy over user informations
    QMap<QString, QString>::const_iterator i = config->Infos.constBegin();
    while (i != config->Infos.constEnd()) {
        char* c = const_cast<char*>((i.key()).toStdString().c_str());
        annotations[c] = i.value().toStdString();
        ++i;
    }

    // Disable system crash reporter if asked
    if (! config->EnableSystemCrashReport){
        crashpad::CrashpadInfo* crashpad_info = crashpad::CrashpadInfo::GetCrashpadInfo();
        crashpad_info->set_system_crash_reporter_forwarding(crashpad::TriState::kDisabled);
    }

    crashpad::CrashpadClient client;
    std::vector<std::string> arguments;

    if (! config->RateLimit){
        arguments.push_back("--no-rate-limit");
    }

    // Enable uploads
    if (config->AutoUpload){
        std::unique_ptr<crashpad::CrashReportDatabase> database = crashpad::CrashReportDatabase::Initialize(db);
        database->GetSettings()->SetUploadsEnabled(true);
    }

    bool rc = client.StartHandler(handler,
                             db,
                             db,
                             config->Server.toStdString(),
                             annotations,
                             arguments,
                             true,
                             true);

    // client.UseSystemDefaultHandler();
    // XXX Winblows (and linux?) might need this - plus call IPC handler for subprocess?
    // rc = client.WaitForHandlerStart(INFINITE);

    return rc;
}

}
