import QtQuick
import QtQuick.Controls.Basic

RadioButton {
    property string source: ""

    indicator: Rectangle {
        width: 4
        height: parent.height
        visible: parent.checked
        color: theme.accentColor
    }
    contentItem: Text {
        text: parent.text
        font.pixelSize: theme.h2
        color: parent.checked ? theme.textColor : theme.textColorDiabled
        verticalAlignment: Text.AlignVCenter
        leftPadding: 10
    }
    onClicked: pageLoader.source = source
}
