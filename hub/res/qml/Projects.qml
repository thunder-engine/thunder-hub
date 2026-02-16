import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "components"

Item {
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

            text: qsTr("Projects")

            font.bold: true
            font.pixelSize: theme.h1
            color: theme.textColor
        }

        ToolButton {
            id: newProject
            height: 32
            width: 120

            anchors.right: parent.right
            anchors.bottom: pannel.bottom
            anchors.rightMargin: 20

            text: qsTr("New project")

            display: AbstractButton.TextBesideIcon

            background: Rectangle {
                anchors.fill: parent
                radius: theme.frameRadius
                color: parent.hovered ? theme.blueLight : theme.blue
            }

            contentItem: Row {
                spacing: 6

                Image {
                    source: "qrc:/icons/plus.png"
                    sourceSize: Qt.size(16, 16)
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    text: newProject.text
                    color: theme.textColor
                    font.pixelSize: theme.textSize
                    verticalAlignment: Text.AlignVCenter
                }
            }

            onClicked: projectsManager.newProject()
        }

        ToolButton {
            id: locateProject
            height: 32
            width: 120

            anchors.right: newProject.left
            anchors.bottom: pannel.bottom
            anchors.rightMargin: 20

            text: qsTr("Locate Existing")

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

            onClicked: projectsManager.importProject()
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
        id: projectsView

        model: projectsManager

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

                    radius: theme.frameRadius
                    color: "transparent"

                    Row {
                        anchors.leftMargin: 20
                        anchors.topMargin: 5
                        spacing: 20
                        anchors.fill: parent

                        Image {
                            width: 60
                            height: 60
                            source: icon
                        }

                        Column {
                            Text {
                                text: name
                                color: theme.textColor
                                font.bold: true
                                font.pixelSize: theme.h3
                                verticalAlignment: Text.AlignVCenter
                            }

                            Text {
                                text: path
                                color: theme.textColor
                                font.bold: false
                                font.pixelSize: theme.textSize
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: projectsManager.openProject(path)
                        onEntered: parent.color = theme.hoverPanel
                        onExited: parent.color = "transparent"
                    }
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

                        MenuSeparator {
                            height: theme.frameRadius
                            visible: false
                        }

                        StyledItem {
                            text: qsTr("Remove from list")
                            onTriggered: projectsManager.removeProject(path)
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
}
