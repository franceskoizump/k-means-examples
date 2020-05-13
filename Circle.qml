import QtQuick 2.3

Rectangle {
    property var xx: 100
    property var yy: 100
    property var rr: 50


    Rectangle {
        x: xx - rr
        y: yy - rr
        width: rr*2
        height: rr*2
        color: "transparent"
        border.color: "black"
        border.width: 1
        radius: rr
    }
}
