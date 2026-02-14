import qbs
import qbs.Process
import qbs.TextFile

Project {
    id: thunder

    property string COMPANY_NAME: "FrostSpear"
    property string PRODUCT_NAME: "Hub"
    property string EDITOR_NAME: "WorldEditor"

    property string PLATFORM: {
        var arch = qbs.architecture;
        if(qbs.targetOS.contains("darwin") || qbs.targetOS[0] === "linux") {
            arch = "x86_64"
        }
        return qbs.targetOS[0] + "/" + arch;
    }

     property string bundle: {
        if(qbs.targetOS.contains("darwin")) {
            return PRODUCT_NAME + ".app/Contents/MacOS/"
        }
        return "";
    }

    Probe {
        id: probe
        property string REVISION
        property string YEAR
        configure: {
            YEAR = new Date().getFullYear().toString()
            REVISION = "develop"
            var p = new Process()
            p.setWorkingDirectory(thunder.sourceDirectory)
            if (p.exec("git", ["rev-parse", "HEAD"]) === 0) {
                REVISION = p.readStdOut().trim()
            } else {
                console.error(p.readStdErr())
            }
        }
    }

    property string COPYRIGHT_YEAR: probe.YEAR

    property string RESOURCE_ROOT: "res"

    property string PREFIX: ""
    property string LAUNCHER_PATH: "launcher"
    property string PLATFORM_PATH: PLATFORM
    property string BIN_PATH: PLATFORM_PATH + "/bin"
    property string LIB_PATH: (qbs.targetOS[0] === "linux") ? PLATFORM_PATH + "/lib" : BIN_PATH
    property string STATIC_PATH: PLATFORM_PATH + "/static"
    property string PLUGINS_PATH: BIN_PATH + "/plugins"

    property stringList defines: {
        var result  = [
            "COMPANY_NAME=\"" + COMPANY_NAME + "\"",
            "PRODUCT_NAME=\"" + PRODUCT_NAME + "\"",
            "EDITOR_NAME=\"" + EDITOR_NAME + "\"",
            "SDK_VERSION=\"" + probe.SDK_VERSION + "\"",
            "COPYRIGHT_YEAR=" + COPYRIGHT_YEAR,
            "REVISION=\"" + probe.REVISION + "\"",
            "LEGAL=\"" + probe.LEGAL + "\""

        ];
        return result;
    }

    references: [
        "hub/ThunderHub.qbs",
        "build/install.qbs",
        "thirdparty/thirdparty.qbs"
    ]
}

