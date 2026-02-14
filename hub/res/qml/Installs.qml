import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "components"

Item {
    id: sdkPannel

    signal installSdk(string version)

    Item {
        id: pannel
        height: 90
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        Text {
            id: label

            anchors.left: parent.left
            anchors.bottom: pannel.bottom
            anchors.leftMargin: 20

            text: qsTr("Engine Versions")

            font.bold: true
            font.pixelSize: theme.h1
            color: theme.textColor
        }

        ToolButton {
            id: addSdk
            height: 32
            width: 120

            anchors.right: parent.right
            anchors.bottom: pannel.bottom
            anchors.rightMargin: 20

            text: qsTr("Install Editor")

            background: Rectangle {
                anchors.fill: parent
                radius: theme.frameRadius
                color: parent.hovered ? theme.blueLight : theme.blue
            }

            contentItem: Text {
                text: addSdk.text
                color: theme.textColor
                font.pixelSize: theme.textSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            onClicked: contextMenu.open()

            Menu {
                id: contextMenu

                x: 0
                y: 70
                width: 150

                background: Rectangle {
                    color: theme.greyDark
                    radius: theme.frameRadius
                    border.color: theme.greyLight
                }

                property var versionsList: installModel ? installModel.sdkVersions() : []

                MenuSeparator {
                    height: theme.frameRadius
                    visible: false
                }

                Repeater {
                    model: contextMenu.versionsList
                    delegate: StyledItem {
                        text: modelData
                        onTriggered: sdkPannel.showInstallPopup(modelData)
                    }
                }

                MenuSeparator {
                    height: theme.frameRadius
                    visible: false
                }

                Connections {
                    target: installModel

                    function onSdkListChanged() {
                        if(sdkModel) {
                            contextMenu.versionsList = installModel.sdkVersions();
                        }
                    }
                }
            }
        }

        ToolButton {
            id: locateProject
            height: 32
            width: 120

            anchors.right: addSdk.left
            anchors.bottom: pannel.bottom
            anchors.rightMargin: 20

            text: qsTr("Locate Installed")

            background: Rectangle {
                anchors.fill: parent
                radius: theme.frameRadius
                color: parent.hovered ? theme.hoverPanel : theme.panel
            }

            contentItem: Text {
                text: locateProject.text
                color: theme.textColor
                font.pixelSize: theme.textSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            onClicked: installModel.locateSdk()
        }
    }

    Rectangle {
        anchors.top: pannel.bottom
        anchors.left: pannel.left
        anchors.right: pannel.right
        anchors.topMargin: 10

        height: 1
        color: theme.blue
    }

    ListView {
        id: sdkView

        model: installModel

        anchors.margins: 20
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: pannel.bottom
        anchors.bottom: parent.bottom

        spacing: 2
        clip: true

        delegate: Item {
            width: ListView.view ? ListView.view.width : 0
            height: 70

            RowLayout {
                anchors.fill: parent

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    height: 70
                    clip: true

                    color: theme.hoverPanel
                    radius: theme.frameRadius

                    Row {
                        anchors.leftMargin: 20
                        anchors.topMargin: 5
                        spacing: 20
                        anchors.fill: parent

                        Image {
                            id: logo
                            width: 60
                            height: 60
                            source: "qrc:/icons/thunderlight.svg"
                        }

                        Column {
                            Text {
                                text: qsTr("Thunder Engine ") + version
                                color: theme.textColor
                                font.bold: true
                                font.pixelSize: theme.h3
                                verticalAlignment: Text.AlignVCenter
                            }

                            Text {
                                text: path
                                color: theme.textColor
                                font.pixelSize: theme.textSize
                                verticalAlignment: Text.AlignVCenter
                            }

                            Text {
                                text: "Status: " + status
                                visible: progress > -1

                                verticalAlignment: Text.AlignBottom
                                horizontalAlignment: Text.AlignLeft

                                color: theme.textColor
                                font.pixelSize: theme.textSize
                            }
                        }
                    }
/*
                    Rectangle {
                        id: progressBar

                        visible: true//progress > -1

                        anchors.top: parent.bottom
                        anchors.left: logo.right
                        anchors.leftMargin: 10

                        height: theme.textSize
                        width: progress
                        radius: theme.frameRadius / 2

                        color: theme.blue
                    }
*/
                }

                ToolButton {
                    id: toolButton

                    icon.source: "qrc:/icons/handle.png"
                    icon.color: theme.textColor

                    Layout.fillHeight: true
                    width: 20

                    background: Rectangle {
                        anchors.fill: parent
                        radius: theme.frameRadius
                        color: parent.hovered ? theme.hoverPanel : "transparent"
                    }

                    onClicked: itemContextMenu.open()

                    Menu {
                        id: itemContextMenu

                        x: 0
                        y: 70
                        width: 150

                        background: Rectangle {
                            color: theme.greyDark
                            radius: theme.frameRadius
                            border.color: theme.greyLight
                        }

                        property var versionsList: installModel ? installModel.sdkVersions() : []

                        MenuSeparator {
                            height: theme.frameRadius
                            visible: false
                        }

                        StyledItem {
                            text: qsTr("Manage")
                            onTriggered: sdkPannel.showInstallPopup(version)
                        }

                        StyledItem {
                            text: qsTr("Show in Explorer")
                            onTriggered: installModel.explorer(version)
                        }

                        StyledItem {
                            text: qsTr("Uninstall")
                            onTriggered: installModel.uninstallSdk(version)
                        }

                        MenuSeparator {
                            height: theme.frameRadius
                            visible: false
                        }
                    }
                }
            }
        }
    }

    Components {
        id: installPopup
        visible: false
    }

    function showInstallPopup(version) {
        sdkModel.setVersion(version)
        installPopup.version = version
        installPopup.visible = true
        installPopup.modules = installModel.installedModules(version);
    }
}
