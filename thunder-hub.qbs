import qbs
import qbs.Process
import qbs.TextFile

Project {
    id: thunder

    property string COMPANY_NAME: "FrostSpear"
    property string PRODUCT_NAME: "Hub"
    property string PRODUCT_VERSION: "2026.1"
    property string EDITOR_NAME: "WorldEditor"

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
    property string PLATFORM_PATH: bundle + "hub"
    property string BIN_PATH: PLATFORM_PATH
    property string QTPLUGINS_PATH: BIN_PATH + "/plugins"
    property string QML_PATH: BIN_PATH + "/qml"

    property stringList defines: {
        var result  = [
            "COMPANY_NAME=\"" + COMPANY_NAME + "\"",
            "PRODUCT_NAME=\"" + PRODUCT_NAME + "\"",
            "PRODUCT_VERSION=\"" + PRODUCT_VERSION + "\"",
            "EDITOR_NAME=\"" + EDITOR_NAME + "\"",
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

