import QtQuick 2.13
import QtQuick.Controls 2.4

Item {
    anchors.fill: parent

    Sdk {
        id: sdk
        height: 150
        anchors.left: parent.left
        anchors.right: parent.right

        onInstallSdk: {
            installPopup.visible = true
            installPopup.version = version
            sdkModel.setVersion(installPopup.version)
        }
    }

    Projects {
        id: projects
        anchors.top: sdk.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }

    Rectangle {
        id: installPopup

        visible: false

        anchors.fill: parent

        color: theme.hoverPanel

        property string version: ""
        property var components: []

        function hidePopup() {
            visible = false
        }

        Rectangle {
            id: popupHeader

            height: 50
            radius: theme.frameRadius

            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 100

            color: theme.blue

            Text {
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: 20
                anchors.topMargin: 10

                text: qsTr("Install Thunder SDK")

                font.pixelSize: theme.h2
                color: theme.textColor
            }
        }

        Rectangle {
            y: 540
            height: 50
            radius: theme.frameRadius

            anchors.left: popupHeader.left
            anchors.right: popupHeader.right

            Button {
                id: cancelButton

                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: 20

                width: 100
                height: 40

                text: qsTr("CANCEL")

                contentItem: Text {
                    text: parent.text
                    font.pixelSize: theme.h3
                    color: theme.greyDark
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                onClicked: installPopup.hidePopup()

                Shortcut {
                    sequence: StandardKey.Cancel
                    onActivated: installPopup.hidePopup()
                }
            }

            Button {
                id: installButton

                anchors.right: parent.right
                anchors.top: parent.top
                anchors.rightMargin: 20

                width: 100
                height: 40

                text: qsTr("INSTALL")

                contentItem: Text {
                    text: parent.text
                    font.pixelSize: theme.h3
                    color: theme.greyDark
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    installPopup.hidePopup()
                    sdkModel.installSdk(installPopup.version, installPopup.components);
                }
            }
        }

        Rectangle {
            height: 400

            anchors.top: popupHeader.bottom
            anchors.left: popupHeader.left
            anchors.right: popupHeader.right
            anchors.topMargin: -theme.frameRadius

            ListView {
                id: modules

                anchors.fill: parent
                anchors.margins: 20

                model: sdkModel

                delegate: Rectangle {
                    height: 32
                    width: modules.width

                    Text {
                        text: name
                        font.pixelSize: theme.h3
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    CheckBox  {
                        enabled: !optionality
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter

                        indicator.width: 20
                        indicator.height: 20

                        onCheckStateChanged: {
                            if(checked) {
                                installPopup.components.push(url)
                            } else {
                                installPopup.components.splice(url, 1)
                            }
                        }
                    }
                }
            }
        }
    }
}
