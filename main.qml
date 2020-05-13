import QtQuick 2.6
import QtQuick.Controls 2.0
import io.qt.examples.backend 1.0

ApplicationWindow {
    id: root
    width: 300
    height: 480
    visible: true
    property var points: []
    property var centers: []
    property var circles: [];
    property bool isAddingPoint: true;

    BackEnd {
        id: backend
    }

    function createCenter(x, y) {
        centers.push(
                    Qt.createQmlObject(
                        'import QtQuick 2.0; Point {x:' + x + '; y:' + y + ';color:\'yellow\'}',
                        root
                        )
                    );
    }
    function createPoint(x, y) {
        points.push(
                    Qt.createQmlObject(
                        'import QtQuick 2.0; Point {x:' + x + '; y:' + y + '}',
                        root
                        )
                    );
    }
    function createCluster(x, y, R) {
        createCenter(x, y);
        circles.push(
                    Qt.createQmlObject(
                        'import QtQuick 2.0; Circle {xx:' + x + '; yy:' + y + ';rr: ' + R + ' ' + '}',
                        root
                        )
                    );
    }

    MouseArea {
        anchors.fill: parent
        id: mouseArea
        onClicked: {
            if (isAddingPoint) {
                backend.addPoint(mouseX, mouseY);
                createPoint(mouseX, mouseY);
            } else {
                backend.addCenter(mouseX, mouseY);
                createCenter(mouseX, mouseY);
            }
        }


    }

    function clearCenters() {
        backend.clearCenters();
        centers.forEach(el => el.destroy());
        centers = [];
        circles.forEach(el => el.destroy());
        circles = [];
    }

    Button {
        id: point_ar_center
        x: 149
        width: 145
        height: 29
        onClicked: {
            isAddingPoint = !isAddingPoint;
        }
        text: isAddingPoint?"Точка":"Центр"
        anchors.top: parent.top
        anchors.topMargin: 11
        anchors.right: parent.right
        anchors.rightMargin: 6
    }
    Button {
        id: clear_all_points
        x: 149
        width: 145
        height: 31
        onClicked: {
            backend.clearPoints();
            points.forEach(el => el.destroy());
            points = [];
        }
        text: 'Очистить все точки'
        anchors.top: parent.top
        anchors.topMargin: 46
        anchors.right: parent.right
        anchors.rightMargin: 6
    }
    Button {
        id: clear_all_centers
        x: 149
        width: 145
        height: 31
        onClicked: clearCenters()
        text: 'Убрать все центры'
        anchors.top: parent.top
        anchors.topMargin: 83
        anchors.right: parent.right
        anchors.rightMargin: 6
    }

    function reDraw() {
        var x = 1;
        var y = 1;
        var len = 0;
        while (true) {
            len = backend.getLengthCenter();
            x = backend.getNewCenterX();
            y = backend.getNewCenterY();
            backend.inc();
            if (x === 0 && y === 0) break;
            createCluster(x, y, len);
        }
    }

    Button {
        id: solve_button
        x: 149
        width: 145
        height: 31
        onClicked: {
            backend.solve();
            centers.forEach(el => el.destroy());
            centers = [];
            circles.forEach(el => el.destroy());
            circles = [];
            reDraw();
        }

        text: 'Решить'
        anchors.top: parent.top
        anchors.topMargin: 120
        anchors.right: parent.right
        anchors.rightMargin: 6
    }
    Button {
        id: solve_button_median
        x: 149
        width: 145
        height: 31
        onClicked: {
            backend.solve_k_med();
            centers.forEach(el => el.destroy());
            centers = [];
            circles.forEach(el => el.destroy());
            circles = [];
            reDraw();
        }

        text: 'Решить k-медиан'
        anchors.top: parent.top
        anchors.topMargin: 157
        anchors.right: parent.right
        anchors.rightMargin: 6
    }
    Button {
        id: solve_button_C_median
        x: 149
        width: 145
        height: 31
        onClicked: {
            backend.solve_C_means();
            centers.forEach(el => el.destroy());
            centers = [];
            circles.forEach(el => el.destroy());
            circles = [];
            reDraw();
        }

        text: 'Решить C-медиан'
        anchors.top: parent.top
        anchors.topMargin: 194
        anchors.right: parent.right
        anchors.rightMargin: 6
    }
    TextField {
        id: textEdit
        x: 153
        y: 231
        width: 141
        height: 31
        text: qsTr("1.0")
        anchors.right: parent.right
        anchors.rightMargin: 6
        font.pixelSize: 12
        onTextChanged: {
            var a = parseFloat(text);
            if (!isNaN(a)) {
                backend.setDisp(a)
            } else {
                text = "1.0"
                backend.setDisp(a)
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:2}D{i:3;anchors_y:11}D{i:4;anchors_y:46}
}
##^##*/
