import QtQuick

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

            text: qsTr("Learn")

            font.bold: true
            font.pixelSize: theme.h1
            color: theme.textColor
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
}
