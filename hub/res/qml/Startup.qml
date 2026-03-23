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
