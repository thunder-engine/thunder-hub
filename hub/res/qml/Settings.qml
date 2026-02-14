import QtQuick
import QtQuick.Controls

import "components"

Item {
    Item {
        id: pannel
        height: 90
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        Text {
            anchors.left: parent.left
            anchors.bottom: pannel.bottom
            anchors.leftMargin: 20

            text: qsTr("Settings")

            font.bold: true
            font.pixelSize: theme.h1
            color: theme.textColor
        }

        Rectangle {
            anchors.top: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: 10

            height: 1
            color: theme.blue
        }
    }

    Column {
        spacing: 20
        anchors.margins: 20

        anchors.top: pannel.bottom
        anchors.left: pannel.left
        anchors.right: pannel.right

        Column {
            spacing: 5

            Text {
                font.bold: true
                font.pixelSize: theme.h3
                color: theme.textColor
                text: qsTr("Projects location")
            }

            Text {
                font.pixelSize: theme.textSize
                color: theme.textColor
                text: qsTr("The new projects will be created in this location by default.")
            }

            StyledPathEdit {
                text: settingsManager.projectsDir

                onDirectorySelected: function(path) {
                    settingsManager.projectsDir = path
                }
            }
        }

        Column {
            spacing: 5

            Text {
                font.bold: true
                font.pixelSize: theme.h3
                color: theme.textColor
                text: qsTr("SDK location")
            }

            Text {
                font.pixelSize: theme.textSize
                color: theme.textColor
                text: qsTr("All SDKs will be installed to this location.")
            }

            StyledPathEdit {
                text: settingsManager.sdkDir

                onDirectorySelected: function(path) {
                    settingsManager.sdkDir = path
                }
            }
        }

        Column {
            spacing: 5

            Text {
                font.bold: true
                font.pixelSize: theme.h3
                color: theme.textColor
                text: qsTr("Temp location")
            }

            Text {
                font.pixelSize: theme.textSize
                color: theme.textColor
                text: qsTr("This location will be used as the temporary location for downloaded artifacts before extraction.")
            }

            StyledPathEdit {
                text: settingsManager.tempDir

                onDirectorySelected: function(path) {
                    settingsManager.tempDir = path
                }
            }
        }
    }
}
