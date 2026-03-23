import QtQuick

Rectangle {
    id: rect
    width: 300
    height: 500
    color: theme.itemColor
    radius: theme.frameRadius
    clip: true

    property variant blogData: undefined

    Image {
        id: thumbnail
        width: rect.width
        height: rect.height / 2
        fillMode: Image.PreserveAspectCrop
        source: (typeof(blogData) !== "undefined") ? blogData.icon : ""
    }
    Rectangle {
        id: separator
        anchors.top: thumbnail.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        height: 1
        color: theme.accentColor
    }
    Column {
        anchors.left: parent.left
        anchors.top: separator.bottom
        padding: spacing
        spacing: 20
        Text {
            width: rect.width - parent.spacing * 2
            font.pixelSize: theme.h2
            color: theme.textColor
            text: (typeof(blogData) !== "undefined") ? blogData.title : ""
            wrapMode: Text.WordWrap
        }
        Text {
            width: rect.width - parent.spacing * 2
            font.pixelSize: theme.textSize
            color: theme.textColor
            text: (typeof(blogData) !== "undefined") ? blogData.brief : ""
            wrapMode: Text.WordWrap
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: parent.color = theme.itemColorHover
        onExited: parent.color = theme.itemColor
        onClicked: {
            if(typeof(blogData) !== "undefined") {
                Qt.openUrlExternally(blogData.link)
            }
        }
    }
}
