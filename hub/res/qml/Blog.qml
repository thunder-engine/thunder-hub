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
        color: theme.blue
    }

    Item {
        id: projectsView

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: pannel.bottom
        anchors.bottom: parent.bottom

        ColumnLayout {
            id: view
            anchors.fill: parent
            anchors.margins: spacing
            spacing: 30

            Rectangle {
                id: tile00
                height: 300
                color: theme.panel
                Layout.fillWidth: true

                property variant headerData: undefined

                Row {
                    spacing: view.spacing
                    anchors.fill: parent

                    Image {
                        anchors.bottom: parent.bottom
                        anchors.top: parent.top
                        width: parent.width / 2
                        fillMode: Image.PreserveAspectCrop
                        clip: true
                        source: (typeof(tile00.headerData) !== "undefined") ? tile00.headerData.thumbnail : ""
                    }
                    Column {
                        anchors.top: parent.top
                        anchors.topMargin: 30
                        spacing: 20
                        width: parent.width / 2
                        anchors.margins: spacing
                        Text {
                            width: parent.width - parent.spacing * 2
                            font.pixelSize: 24
                            color: theme.textColor
                            text: (typeof(tile00.headerData) !== "undefined") ? tile00.headerData.title : ""
                            wrapMode: Text.WordWrap
                        }
                        Text {
                            width: parent.width - parent.spacing * 2
                            font.pixelSize: 16
                            color: theme.textColor
                            text: (typeof(tile00.headerData) !== "undefined") ? tile00.headerData.summary : ""
                            wrapMode: Text.WordWrap
                        }
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: parent.color = theme.hoverPanel
                    onExited: parent.color = theme.panel
                    onClicked: {
                        if(typeof(headerData) !== "undefined") {
                            Qt.openUrlExternally(blogData.link)
                        }
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: view.spacing

                BlogTile {
                    id: tile11
                    Layout.fillWidth: true
                }
                BlogTile {
                    id: tile12
                    Layout.fillWidth: true
                }
                BlogTile {
                    id: tile13
                    Layout.fillWidth: true
                }
                BlogTile {
                    id: tile14
                    Layout.fillWidth: true
                }
            }

            Item {
                Layout.fillHeight: true
            }
        }

        XmlListModel {
            source: feedManager.blogFeed
            query: "/feed/entry"

            XmlListModelRole { name: "title"; elementName: "title/string()" }
            XmlListModelRole { name: "summary"; elementName: "fn:replace(summary/string(), '\&lt;a href=.*\/a\&gt;', '')" }
            XmlListModelRole { name: "thumbnail"; elementName: "media:thumbnail/@url/string()" }
            XmlListModelRole { name: "link"; elementName: "link/@href/string()" }

            onStatusChanged: {
                if(status == XmlListModel.Ready) {
                    tile00.headerData = get(0)

                    tile11.blogData = get(1)
                    tile12.blogData = get(2)
                    tile13.blogData = get(3)
                    tile14.blogData = get(4)
                }
            }
        }
    }
}
