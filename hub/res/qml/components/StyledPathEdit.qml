import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

ToolButton {
    id: pathEdit

    width: 600
    height: 28
    clip: true

    property var onDirectorySelected: function(path) {
        pathEdit.text = path
    }

    background: Rectangle {
        color: parent.hovered ? theme.hoverPanel : theme.panel
        radius: theme.frameRadius
        border.color: theme.greyLight
    }

    contentItem: Row {
        spacing: 6

        Image {
            source: "qrc:/icons/folder.png"
            sourceSize: Qt.size(16, 16)
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            text: pathEdit.text
            color: theme.textColor
            font.pixelSize: theme.textSize
            verticalAlignment: Text.AlignVCenter
        }
    }

    FolderDialog {
        id: folderDialog
        title: qsTr("Please choose a folder")

        onAccepted: {
            let urlObj = new URL(folderDialog.selectedFolder)
            let path = urlObj.pathname

            if(Qt.platform.os === "windows" && path.startsWith("/")) {
                path = path.substring(1)
            }


            pathEdit.onDirectorySelected(path)
        }
    }

    onClicked: {
        folderDialog.currentFolder = "file:///" + pathEdit.text
        folderDialog.open()
    }
}
