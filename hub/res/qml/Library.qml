import QtQuick 2.0

Item {
    anchors.fill: parent

    Sdk {
        id: sdk
        height: 150
        anchors.left: parent.left
        anchors.right: parent.right
    }

    Projects {
        id: projects
        anchors.top: sdk.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }
}
