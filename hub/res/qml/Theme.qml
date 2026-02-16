import QtQuick
import QtQuick.Controls

Item {
    property string textColor: "#ffffff"

    property string panel: "#60000000"
    property string hoverPanel: "#30000000"

    property string grey: "#606060"
    property string greyLight: "#8d8d8d"
    property string greyDark: "#363636"
    property string greyHover: "#404040"

    property string blue: "#0277bd"
    property string blueLight: "#58a5f0"
    property string blueDark: "#004c8c"
    property string blueHover: "#0288d1"

    property string green: "#2e7d32"
    property string greenLight: "#60ad5e"
    property string greenDark: "#005005"
    property string greenHover: "#388e3c"

    property string red: "#c62828"
    property string redLight: "#ff5f52"
    property string redDark: "#8e0000"
    property string redHover: "#d32f2f"

    property int frameRadius: 4

    property int textSize: 14
    property int h1: 28
    property int h2: 22
    property int h3: 18

    property var colors: [Qt.rgba(1,0,0), Qt.rgba(0,1,0), Qt.rgba(0,0,1), Qt.rgba(1,1,0), Qt.rgba(1,0,1), Qt.rgba(0,1,1)]
}
