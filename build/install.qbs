import qbs
import qbs.FileInfo
import qbs.Environment

Product {
    id: install
    name: "_install"

    Depends { name: "cpp" }
    Depends {
        name: "Qt.core"
    }

    property string suffix: {
        if(qbs.targetOS.contains("windows")) {
            return ".dll"
        } else if(qbs.targetOS.contains("darwin")) {
            return ".dylib"
        }
        return ".so"
    }

    property var pluginFiles: {
        var files = []
        if(qbs.targetOS.contains("windows")) {
            if(qbs.debugInformation) {
                files.push("**/*d.dll")
            } else {
                files.push("**/*.dll")
            }
        } else if(qbs.targetOS.contains("linux")) {
            files.push("**/*.so")
        } else {
            files.push("*")
        }
        return files
    }

    property var pluginExcludeFiles: {
        var files = ["*.pdb"];
        if (!(qbs.targetOS.contains("windows") && qbs.debugInformation)) {
            files.push("**/*d.dll");
        }
        return files;
    }

    Group {
        name: "Qt DLLs"
        prefix: {
            if (qbs.targetOS.contains("windows")) {
                return Qt.core.binPath + "/"
            } else {
                return Qt.core.libPath + "/"
            }
        }

        files: {
            var list = [];
            var libs = ["Core", "Gui", "Network", "OpenGL", "QmlCore", "Qml", "QmlMeta", "QmlModels", "QmlNetwork", "QmlWorkerScript",
                        "QmlXmlListModel", "QuickControls2Basic", "QuickControls2BasicStyleImpl", "QuickControls2", "QuickControls2Impl",
                        "Quick", "QuickDialogs2", "QuickDialogs2QuickImpl", "QuickDialogs2Utils", "QuickLayouts", "QuickTemplates2",
                        "QuickWidgets", "Svg", "Xml", "Widgets"]
            if (!Qt.core.frameworkBuild) {
                var libPrefix = (qbs.targetOS.contains("linux") ? "lib" : "") + "Qt" + Qt.core.versionMajor
                var libPostfix = ((qbs.targetOS.contains("windows") && qbs.debugInformation) ? "d": "") + cpp.dynamicLibrarySuffix
                if(qbs.targetOS.contains("linux")) {
                    list.push("libicudata.so.73", "libicudata.so.73.2")
                    list.push("libicui18n.so.73", "libicui18n.so.73.2")
                    list.push("libicuuc.so.73", "libicuuc.so.73.2")

                    libs.push("DBus")
                    libs.push("XcbQpa")

                    for(var it in libs) {
                        list.push(libPrefix + libs[it] + libPostfix + "." + Qt.core.versionMajor + "." + Qt.core.versionMinor + "." + Qt.core.versionPatch)
                        list.push(libPrefix + libs[it] + libPostfix + "." + Qt.core.versionMajor)
                    }
                } else {
                    for(var it in libs) {
                        list.push(libPrefix + libs[it] + libPostfix)
                    }
                }
            } else {
                libs.push("DBus")
                libs.push("XcbQpa")

                var libPrefix = "**/Qt"
                var libPostfix = ".framework/**"
                for(var it in libs) {
                    list.push(libPrefix + libs[it] + libPostfix)
                }
            }
            return list
        }
        qbs.install: true
        qbs.installDir: {
            if(qbs.targetOS.contains("darwin")) {
                return install.BIN_PATH + "/" + install.bundle + "/Frameworks/"
            } else if(qbs.targetOS.contains("windows")) {
                return install.BIN_PATH + "/" + install.bundle
            }
            return install.LIB_PATH
        }
        qbs.installPrefix: install.PREFIX

        excludeFiles: [
            "**/Headers",
            "**/Headers/**",
            "**/*.prl",
            "**/*_debug"
        ]
        qbs.installSourceBase: prefix
    }

    Group {
        name: "Qt Image Format Plugins"
        prefix: FileInfo.joinPaths(Qt.core.pluginPath, "/imageformats/")
        files: pluginFiles
        excludeFiles: pluginExcludeFiles
        qbs.install: true
        qbs.installDir: install.QTPLUGINS_PATH + "/imageformats"
        qbs.installPrefix: install.PREFIX
    }

    Group {
        name: "Qt Platform Plugins"
        prefix: FileInfo.joinPaths(Qt.core.pluginPath, "/platforms/")
        files: pluginFiles
        excludeFiles: pluginExcludeFiles
        qbs.install: true
        qbs.installDir: install.QTPLUGINS_PATH + "/platforms"
        qbs.installPrefix: install.PREFIX
    }

    Group {
        name: "Qt XCB Integrations Plugins"
        condition: qbs.targetOS.contains("linux")
        prefix: FileInfo.joinPaths(Qt.core.pluginPath, "/xcbglintegrations/")
        files: pluginFiles
        excludeFiles: pluginExcludeFiles
        qbs.install: true
        qbs.installDir: install.QTPLUGINS_PATH + "/xcbglintegrations"
        qbs.installPrefix: install.PREFIX
    }

    Group {
        name: "QML Plugins"
        prefix: FileInfo.joinPaths(Qt.core.pluginPath, "/../qml/")

        files: {
            var files = ["QtQuick/**/*.qml", "QtQml/XmlListModel/**/*.qml",
                         "QtQuick/**/plugins.qmltypes", "QtQml/XmlListModel/**/plugins.qmltypes",
                         "QtQuick/**/qmldir", "QtQml/XmlListModel/**/qmldir"]
            if(qbs.targetOS.contains("windows")) {
                if(qbs.debugInformation) {
                    files.push("QtQuick/**/*d.dll")
                    files.push("QtQml/XmlListModel/**/*d.dll")
                } else {
                    files.push("QtQuick/**/*.dll")
                    files.push("QtQml/XmlListModel/**/*.dll")
                }
            } else if(qbs.targetOS.contains("linux")) {
                files.push("QtQuick/**/*.so")
                files.push("QtQml/XmlListModel/**/*.so")
            } else {
                files.push("QtQml/WorkerScript/**/*.dylib")
                files.push("QtQml/WorkerScript/**/plugins.qmltypes")
                files.push("QtQml/WorkerScript/**/qmldir")

                files.push("QtQuick/**/*.dylib")
                files.push("QtQml/XmlListModel/**/*.dylib")
            }
            return files
        }

        excludeFiles: pluginExcludeFiles
        qbs.install: true
        qbs.installDir: install.QML_PATH
        qbs.installPrefix: install.PREFIX
        qbs.installSourceBase: prefix
    }
}
