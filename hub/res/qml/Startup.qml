import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Window

import "qrc:/qml/."
import "qrc:/qml/components/."

Window {
    id: rect
    color: theme.background
    width: 1280
    height: 720
    visible: true

    Theme {
        id: theme
    }

    Rectangle {
        id: leftPanel
        width: 200
        color: theme.itemColor
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.left: parent.left

        Item {
            id: rectangle
            height: 40
            anchors.rightMargin: 10
            anchors.leftMargin: 10
            anchors.topMargin: 10
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top

            Image {
                id: image
                x: 0
                y: 0
                width: 40
                height: 40
                source: "qrc:/icons/thunderlight.svg"
            }

            Text {
                id: label

                anchors.left: image.right
                anchors.bottom: image.bottom
                anchors.leftMargin: 10

                text: "Hub"

                font.bold: true
                font.pixelSize: theme.h1
                verticalAlignment: Text.AlignBottom
                color: theme.textColor
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: Qt.openUrlExternally("http://thunderengine.org")
            }
        }

        ButtonGroup {
            buttons: column.children
        }

        Column {
            id: column
            anchors.top: parent.top
            anchors.topMargin: 100
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.rightMargin: 10
            anchors.leftMargin: 10

            SideButton {
                checked: true
                text: qsTr("Projects")
                source: "Projects.qml"
            }

            SideButton {
                text: qsTr("Installs")
                source: "Installs.qml"
            }
/*
            SideButton {
                text: qsTr("Learn")
                source: "Learn.qml"
            }
*/
            SideButton {
                text: qsTr("Blog")
                source: "Blog.qml"
            }

            SideButton {
                text: qsTr("Settings")
                source: "Settings.qml"
            }
        }

        Rectangle {
            id: versionDlg

            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 10
            height: 150
            radius: theme.frameRadius
            color: theme.accentColor
            visible: false

            property string version: ""

            Column {
                anchors.fill: parent
                anchors.margins: 10
                anchors.topMargin: 30
                spacing: 10

                Text {
                    anchors.left: parent.left
                    anchors.right: parent.right

                    text: qsTr("Hub update available")

                    font.bold: true
                    font.pixelSize: theme.textSize
                    color: theme.textColor
                }

                Text {
                    anchors.left: parent.left
                    anchors.right: parent.right

                    text: qsTr("Hub %1 now available for download").arg(versionDlg.version)
                    wrapMode: Text.WordWrap
                    font.pixelSize: theme.textSize
                    color: theme.textColor
                }

                ToolButton {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    text: qsTr("Download")

                    background: Rectangle {
                        anchors.fill: parent
                        radius: theme.frameRadius
                        border.color: theme.frameBorder
                        color: parent.hovered ? theme.itemColorHover : theme.itemColor
                    }

                    contentItem: Text {
                        text: parent.text
                        color: theme.textColor
                        font.pixelSize: theme.textSize
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }

                    onClicked: Qt.openUrlExternally("https://github.com/thunder-engine/thunder-hub/releases/tag/" + versionDlg.version)
                }
            }

            Connections {
                target: updateManager
                function onUpdateAvailable(data) {
                    versionDlg.version = data
                    versionDlg.visible = true
                }
            }
        }
    }

    Item {
        anchors.left: leftPanel.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.width - leftPanel.width

        Loader {
            id: pageLoader
            anchors.fill: parent
            source: "Projects.qml"
        }
    }
}
