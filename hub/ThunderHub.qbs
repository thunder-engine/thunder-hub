import qbs

Project {
    id: ThunderHub
    property stringList srcFiles: [
        "**/*.qml",
        "**/*.ui",
        "**/*.cpp",
        "**/*.h",
        "res/icon.rc",
        "res/hub.qrc",
        "res/app-Info.plist"
    ]

    property stringList incPaths: [
        "../thirdparty/7zip/src/7zip"
    ]

    QtGuiApplication {
        name: ThunderHub.PRODUCT_NAME
        files: [
            "res/versions.json",
        ].concat(ThunderHub.srcFiles)

        Depends { name: "cpp" }
        Depends { name: "bundle" }
        Depends { name: "Qt"; submodules: ["core", "gui", "widgets", "multimedia", "quickwidgets"]; }
        property bool isBundle: qbs.targetOS.contains("darwin") && bundle.isBundle
        bundle.infoPlist: ({
            "NSHumanReadableCopyright": "(C) 2007-" + ThunderHub.COPYRIGHT_YEAR + " by " + ThunderHub.COPYRIGHT_AUTHOR
        })
        bundle.identifierPrefix: "com.thunderengine.hub"

        consoleApplication: false

        cpp.defines: {
            var result  = ThunderHub.defines
            result.push("NEXT_SHARED")
            return result
        }

        cpp.includePaths: ThunderHub.incPaths
        property string prefix: qbs.targetOS.contains("windows") ? "lib" : ""
        cpp.cxxLanguageVersion: "c++14"

        Properties {
            condition: qbs.targetOS.contains("windows")
        }

        Properties {
            condition: qbs.targetOS.contains("linux")
            cpp.rpaths: "$ORIGIN/../lib"
        }

        Properties {
            condition: qbs.targetOS.contains("darwin")
            cpp.sonamePrefix: "@rpath"
            cpp.rpaths: "@executable_path/../Frameworks/"
            cpp.weakFrameworks: ["OpenGL"]
        }

        Group {
            name: "Install " + ThunderHub.PRODUCT_NAME
            qbs.install: true
            qbs.installDir: ThunderHub.BIN_PATH
            qbs.installPrefix: ThunderHub.PREFIX

            fileTagsFilter: isBundle ? ["bundle.content"] : ["application"]
            qbs.installSourceBase: product.buildDirectory
        }

        Group {
            name: "Icon"
            qbs.install: qbs.targetOS.contains("darwin")
            files: [
                "res/icons/thunder.icns"
            ]
            qbs.installDir: ThunderHub.BIN_PATH + "/" + ThunderHub.bundle + "../Resources"
            qbs.installPrefix: ThunderHub.PREFIX
        }
    }
}
