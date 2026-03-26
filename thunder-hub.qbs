import qbs
import qbs.Process
import qbs.TextFile

Project {
    id: thunder

    property string COMPANY_NAME: "ThunderEngine"
    property string PRODUCT_NAME: "ThunderHub"
    property string PRODUCT_VERSION: "2026.2"
    property string EDITOR_NAME: "WorldEditor"

    property string bundle: {
        if(qbs.targetOS.contains("darwin")) {
            return  "/" + PRODUCT_NAME + ".app/Contents"
        }
        return "";
    }

    Probe {
        id: probe
        property string YEAR
        configure: {
            YEAR = new Date().getFullYear().toString()
        }
    }

    property string COPYRIGHT_YEAR: probe.YEAR

    property string RESOURCE_ROOT: "res"

    property string PREFIX: ""
    property string PLATFORM_PATH: "hub"
    property string BIN_PATH: PLATFORM_PATH + bundle
    property string QTPLUGINS_PATH: BIN_PATH + "/plugins"
    property string QML_PATH: BIN_PATH + "/qml"

    property stringList defines: {
        var result  = [
            "COMPANY_NAME=\"" + COMPANY_NAME + "\"",
            "PRODUCT_NAME=\"" + PRODUCT_NAME + "\"",
            "PRODUCT_VERSION=\"" + PRODUCT_VERSION + "\"",
            "EDITOR_NAME=\"" + EDITOR_NAME + "\"",
            "COPYRIGHT_YEAR=" + COPYRIGHT_YEAR
        ];
        return result;
    }

    references: [
        "hub/ThunderHub.qbs",
        "build/install.qbs",
        "thirdparty/thirdparty.qbs"
    ]
}

