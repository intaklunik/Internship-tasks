import QtQuick 2.2
import QtQuick.Controls 2.12

Row{
    ListModel {
        id: modeModel
        ListElement {
            idButton: "butt_List"
            name: "List"
            type: "stateList"
        }
        ListElement {
            idButton: "butt_Grid"
            name: "Grid"
            type: "stateGrid"
        }
        ListElement {
            idButton: "butt_PathView"
            name: "PathView"
            type: "statePathView"
        }
    }

    Repeater {
        model: modeModel
        Button {
            id: idButton
            flat: true
            text: name
            onClicked: { rect_view.state = type }
            background: Rectangle {
                       implicitWidth: 100
                       color: idButton.down ? "#ecf2f9" : "white"
                       border.color: "#ecf2f9"
                       border.width: 2
                   }
        }
    }
}
