import QtQuick 2.0

Item {
    width: 4
    height: 4
    property alias color: rec.color
    Rectangle {
        id: rec
        color: 'black'
        anchors.fill: parent
    }
}
