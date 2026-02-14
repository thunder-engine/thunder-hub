import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

Rectangle {
    id: installPopup

    anchors.fill: parent

    color: theme.hoverPanel

    property string version: ""
    property var modules: []

    function hidePopup() {
        visible = false
    }

    Rectangle {
        width: 800
        height: 400

        anchors.centerIn: parent

        radius: theme.frameRadius
        color: theme.greyDark
        border.color: theme.greyLight


        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20

            spacing: 10

            Item {
                id: title
                Layout.fillWidth: true
                height: 40

                Text {
                    text: installPopup.version + qsTr(" Install Thunder SDK")

                    font.bold: true
                    font.pixelSize: theme.h2
                    color: theme.textColor
                    verticalAlignment: Text.AlignVCenter
                }

                ToolButton {
                    width: 30
                    height: 30

                    anchors.right: parent.right

                    icon.source: "qrc:/icons/close.png"
                    icon.color: theme.textColor

                    background: Rectangle {
                        anchors.fill: parent
                        radius: theme.frameRadius
                        color: parent.hovered ? theme.greyHover : theme.grey
                    }

                    Shortcut {
                        sequence: StandardKey.Cancel
                        onActivated: installPopup.hidePopup()
                    }

                    onClicked: installPopup.hidePopup()
                }
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: theme.greyLight
            }

            ListView {
                id: modules

                Layout.fillWidth: true
                Layout.fillHeight: true

                model: sdkModel
                clip: true

                delegate: Item {
                    id: modelRow

                    height: 32
                    width: modules.width

                    Text {
                        text: name
                        font.pixelSize: theme.textSize
                        color: theme.textColor
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    RowLayout {
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter

                        Text {
                            text: size
                            font.pixelSize: theme.textSize
                            color: theme.textColor
                        }

                        CheckBox {
                            id: control

                            enabled: optional
                            checked: installed

                            onCheckStateChanged: {
                                if(checked) {
                                    installPopup.modules.push(platform)
                                } else {
                                    var index = installPopup.modules.indexOf(platform);
                                    if (index !== -1) {
                                        installPopup.modules.splice(index, 1)
                                    }
                                }
                            }
                        }
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: theme.greyLight
            }

            RowLayout {
                anchors.rightMargin: 20

                ToolButton {
                    id: installButton

                    width: 200
                    height: 40

                    text: qsTr("Install")

                    background: Rectangle {
                        anchors.fill: parent
                        radius: theme.frameRadius
                        color: parent.hovered ? theme.blueHover : theme.blue
                    }

                    contentItem: Text {
                        text: parent.text
                        font.pixelSize: theme.h3
                        color: theme.textColor
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: {
                        installPopup.hidePopup()
                        installModel.updateSdk(installPopup.version, installPopup.modules);
                    }
                }

                ToolButton {
                    id: cancelButton

                    width: 200
                    height: 40

                    text: qsTr("Cancel")

                    background: Rectangle {
                        anchors.fill: parent
                        radius: theme.frameRadius
                        color: parent.hovered ? theme.greyHover : theme.grey
                    }

                    contentItem: Text {
                        text: parent.text
                        font.pixelSize: theme.h3
                        color: theme.textColor
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: installPopup.hidePopup()
                }
            }
        }
    }
}

