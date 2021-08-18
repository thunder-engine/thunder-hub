import QtQuick 2.0
import QtQuick.Controls 2.3

Item {
    id: sdkPannel

    signal installSdk(string version)

    Item {
        id: pannel

        height: 50

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        Text {
            id: label

            anchors.left: parent.left
            anchors.bottom: pannel.bottom
            anchors.leftMargin: 20

            text: qsTr("Engine Versions")
            font.bold: false
            font.pixelSize: theme.h1

            color: theme.textColor
        }

        ToolButton {
            id: addSdk

            height: 24
            width: 24

            anchors.left: label.right
            anchors.bottom: pannel.bottom
            anchors.leftMargin: 10

            hoverEnabled: true
            text: qsTr("+")

            background: Rectangle {
                anchors.fill: parent
                radius: theme.frameRadius
                color: parent.hovered ? theme.blueLight : theme.blue
            }
            contentItem: Text {
                text: parent.text
                font.bold: false
                font.pixelSize: theme.h3

                color: theme.textColor

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            onClicked: contextMenu.open()

            Menu {
                id: contextMenu

                x: 0
                y: 30
                width: 150
                MenuItem {
                    text: qsTr("Locate Installed")
                    onTriggered: installModel.locateSdk()
                }

                Repeater {
                    model: sdkModel.sdkVersions()
                    MenuItem {
                        text: "Version: " + modelData
                        onTriggered: sdkPannel.installSdk(modelData)
                    }
                }
            }
        }
    }

    GridView {
        id: sdkView

        cellHeight: 78
        cellWidth: 200

        anchors.margins: 20
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: pannel.bottom
        anchors.bottom: parent.bottom

        model: installModel

        delegate: Rectangle {
            x: 5
            width: 190
            height: 68
            color: (progress === -1) ? theme.panel : theme.hoverPanel
            radius: theme.frameRadius

            Image {
                id: icon

                width: 48
                height: 48

                anchors.left: parent.left
                anchors.top: parent.top
                anchors.margins: 10

                source: "qrc:/icons/thunderlight.svg"
            }
            Text {
                text: version

                anchors.top: icon.top
                anchors.left: icon.right
                anchors.right: parent.right

                verticalAlignment: Text.AlignBottom
                horizontalAlignment: Text.AlignHCenter

                color: theme.textColor
                font.bold: false
                font.pixelSize: theme.h3
            }  

            Rectangle {
                id: progressBar

                visible: progress > -1

                anchors.bottom: icon.bottom
                anchors.left: icon.right
                anchors.leftMargin: 10

                height: theme.textSize
                width: progress
                radius: theme.frameRadius / 2

                color: theme.blue
            }

            Text {
                text: status

                anchors.bottom: progressBar.top
                anchors.left: icon.right
                anchors.right: parent.right
                anchors.leftMargin: 10

                verticalAlignment: Text.AlignBottom
                horizontalAlignment: Text.AlignLeft

                color: theme.textColor
                font.bold: false
                font.pixelSize: theme.textSize
            }

            Button {
                id: toolButton

                text: "⋮"
                anchors.right: parent.right
                anchors.top: parent.top

                width: 30
                height: 30

                background: Rectangle {
                    color: "transparent"
                }
                contentItem: Text {
                    text: parent.text
                    font.bold: true
                    font.pixelSize: theme.h1
                    color: theme.textColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                onClicked: itemContextMenu.open()

                Menu {
                    id: itemContextMenu

                    x: -toolButton.x + 20
                    y: 20
                    width: 150

                    MenuItem {
                        text: qsTr("Uninstall")
                        onTriggered: installModel.uninstallSdk(path)
                    }
                }
            }
        }
    }
}
