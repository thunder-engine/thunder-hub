import QtQuick
import QtQuick.Controls

MenuItem {
    contentItem: Text {
        text: parent.text
        color: theme.textColor
        font.pixelSize: 14
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        anchors.fill: parent

        anchors.leftMargin: 1
        anchors.rightMargin: 1

        color: parent.highlighted ? theme.greyHover : "transparent"
    }
}
