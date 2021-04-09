import QtQuick 2.0

Rectangle {
    id: rect
    height: 500
    color: theme.panel

    property variant blogData: undefined

    Image {
        id: thumbnail
        width: rect.width
        height: rect.height / 2
        fillMode: Image.PreserveAspectCrop
        clip: true
        source: (typeof(blogData) !== "undefined") ? blogData.thumbnail : ""
    }
    Column {
        anchors.left: parent.left
        anchors.top: thumbnail.bottom
        anchors.margins: spacing
        spacing: 20
        Text {
            anchors.leftMargin: 20
            width: rect.width - 20
            font.pixelSize: 24
            color: theme.textColor
            text: (typeof(blogData) !== "undefined") ? blogData.title : ""
            wrapMode: Text.WordWrap
        }
        Text {
            anchors.leftMargin: 20
            width: rect.width - 20
            font.pixelSize: 16
            color: theme.textColor
            text: (typeof(blogData) !== "undefined") ? blogData.summary : ""
            wrapMode: Text.WordWrap
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: parent.color = theme.hoverPanel
        onExited: parent.color = theme.panel
        onClicked: {
            if(typeof(blogData) !== "undefined") {
                Qt.openUrlExternally(blogData.link)
            }
        }
    }
}
