import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQml.XmlListModel

import "components"

Item {
    id: element

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

            text: qsTr("Blog")

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
        color: theme.accentColor
    }

    GridView {
        model: feedManager

        anchors.margins: 20
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: pannel.bottom
        anchors.bottom: parent.bottom

        clip: true

        cellWidth: 310
        cellHeight: 510

        delegate: BlogTile {
            id: tile
            Layout.fillWidth: true
            blogData: model
        }
    }
}
