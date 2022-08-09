import QtQuick 2.3
import QtQuick.Controls 1.2

ApplicationWindow {

    property string path

    id: root
    width: 500
    height: 500
    title: qsTr(path)

    Rectangle{
        width: root.width
        height: root.height
    Image {
        anchors.centerIn: parent
        width: 400
        height: 400
        fillMode: Image.PreserveAspectCrop
        source:  path
    }
    }

}
