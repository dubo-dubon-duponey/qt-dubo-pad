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

int Client::start()
{
    // Get the path to the local database and handler
    base::FilePath db(config->CrashDirectory.toStdString());
    base::FilePath handler(config->HandlerPath.toStdString());

    // Create the annotations object
    std::map<std::string, std::string> annotations;
    // Copy over user informations
    QMap<QString, QString>::const_iterator i = config->Infos.constBegin();
    while (i != config->Infos.constEnd()) {
        const std::string key = i.key().toStdString();
        const std::string value = i.value().toStdString();
        annotations[const_cast<char*>(key.c_str())] = value;
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
