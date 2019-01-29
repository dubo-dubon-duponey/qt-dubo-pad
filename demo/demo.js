window.onload = function(){
    return new QWebChannel(qt.webChannelTransport, function(channel){
        // Purely for debug
        window.XXX = channel.objects;

        // Display library information
        var disp = channel.objects.Root;
        var iNode = document.getElementById("info");
        var list = ["NAME", "VENDOR", "VERSION", "REVISION", "CHANGESET", "BUILD", "LINK", "QT", "PLUGIN_NAME", "PLUGIN_VERSION", "PLUGIN_REVISION"];

        list.forEach(function(i){
            var line = document.createElement("div");
            line.appendChild(document.createTextNode(i + ": " + disp[i]));
            iNode.appendChild(line)
        });

        // Display handler configuration
        disp = channel.objects.Dubo.config;
        iNode = document.getElementById("config");
        list = ["HandlerPath", "CrashDirectory", "Product", "Version", "EnableSystemCrashReport", "AutoUpload", "RateLimit", "Server"];

        list.forEach(function(i){
            var line = document.createElement("div");
            line.appendChild(document.createTextNode(i + ": " + disp[i]));
            iNode.appendChild(line)
        });

        // A little something from javascript
        var inf = disp.Infos;
        inf.heyYa = "This is javascript speaking to you";
        inf.something = "Different";
        // This QVariant object is not live... need to reassign.
        disp.Infos = inf;

        // Wire-up the start & crash buttons
        var crasher = document.getElementById("crash");
            crasher.removeAttribute("disabled");
        crasher.onclick = function(/*e*/){
            channel.objects.Crasher.nhehehehe();
        };

        var starter = document.getElementById("start");
        starter.removeAttribute("disabled");
        starter.onclick = function(/*e*/){
            channel.objects.Dubo.start();
        };
    });
};
