import QtQuick 2.0
import QtQuick.Controls 2.3

Item {
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
            text: qsTr("Projects")
            font.bold: false
            font.pixelSize: theme.h1
            color: theme.textColor
        }

        Button {
            id: newProject
            height: 24
            width: 24
            anchors.left: label.right
            anchors.bottom: pannel.bottom
            anchors.leftMargin: 10
            hoverEnabled: true
            text: "+"

            background: Rectangle {
                anchors.fill: parent
                radius: theme.frameRadius
                color: parent.hovered ? theme.blueHover : theme.blue
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
                    text: qsTr("New Project")
                    onTriggered: projectsModel.newProject()
                }
                MenuItem {
                    text: qsTr("Locate Existing")
                    onTriggered: projectsModel.importProject()
                }
            }
        }
    }

    GridView {
        id: projectsView

        anchors.margins: 20
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: pannel.bottom
        anchors.bottom: parent.bottom

        cellHeight: 180
        cellWidth: 200

        model: projectsModel
        clip: true

        delegate: Rectangle {
            x: 5
            width: 190
            height: 170
            color: theme.panel
            radius: theme.frameRadius

            Column {
                spacing: 5
                anchors.fill: parent
                Image {
                    width: 128
                    height: 128
                    source: icon
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Rectangle {
                    width: parent.width
                    height: 1
                    color: theme.blue
                }
                Text {
                    text: name
                    anchors.horizontalCenter: parent.horizontalCenter
                    verticalAlignment: Text.AlignBottom
                    color: theme.textColor
                    font.bold: false
                    font.pixelSize: theme.h3
                }
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onClicked: projectsModel.openProject(path)
                onEntered: parent.color = theme.hoverPanel
                onExited: parent.color = theme.panel
            }

            ToolButton {
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
                        text: qsTr("Remove from list")
                        onTriggered: projectsModel.removeProject(path)
                    }
                }
            }
        }
    }
}
