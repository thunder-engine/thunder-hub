import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Window

import "qrc:/qml/."

Window {
    id: rect
    color: theme.grey
    width: 1280
    height: 720
    visible: true

    Theme {
        id: theme
    }

    Rectangle {
        id: leftPanel
        width: 200
        color: theme.panel
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

            RadioButton {
                checked: true
                text: qsTr("Projects")
                indicator: Rectangle {
                    width: 4
                    height: parent.height
                    visible: parent.checked
                    color: theme.blue
                }
                contentItem: Text {
                    text: parent.text
                    font.pixelSize: theme.h2
                    color: parent.checked ? theme.textColor : theme.greyLight
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 10
                }
                onClicked: pageLoader.source = "Projects.qml"
            }

            RadioButton {
                text: qsTr("Installs")
                indicator: Rectangle {
                    width: 4
                    height: parent.height
                    visible: parent.checked
                    color: theme.blue
                }
                contentItem: Text {
                    text: parent.text
                    font.pixelSize: theme.h2
                    color: parent.checked ? theme.textColor : theme.greyLight
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 10
                }
                onClicked: pageLoader.source = "Installs.qml"
            }
/*
            RadioButton {
                text: qsTr("Learn")
                indicator: Rectangle {
                    width: 4
                    height: parent.height
                    visible: parent.checked
                    color: theme.blue
                }
                contentItem: Text {
                    text: parent.text
                    font.pixelSize: theme.h2
                    color: parent.checked ? theme.textColor : theme.greyLight
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 10
                }
                onClicked: pageLoader.source = "Learn.qml"
            }

            RadioButton {
                text: qsTr("Blog")
                indicator: Rectangle {
                    width: 4
                    height: parent.height
                    visible: parent.checked
                    color: theme.blue
                }
                contentItem: Text {
                    text: parent.text
                    font.pixelSize: theme.h2
                    color: parent.checked ? theme.textColor : theme.greyLight
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 10
                }
                onClicked: pageLoader.source = "Blog.qml"
            }
*/
            RadioButton {
                text: qsTr("Settings")
                indicator: Rectangle {
                    width: 4
                    height: parent.height
                    visible: parent.checked
                    color: theme.blue
                }
                contentItem: Text {
                    text: parent.text
                    font.pixelSize: theme.h2
                    color: parent.checked ? theme.textColor : theme.greyLight
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 10
                }
                onClicked: pageLoader.source = "Settings.qml"
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
