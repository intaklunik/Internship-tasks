import Qt.labs.folderlistmodel 1.0
import QtQuick 2.2
import QtQuick.Controls 1.5
import QtQuick.Controls 2.5
import QtQml 2.12


ApplicationWindow {
    id: root
    visible: true
    width: 800
    height: 480
    title: qsTr("Image Viewer")

    ModesMenu{
        id: modes
        anchors.top: root.top
        height: root.height / 10
        anchors.horizontalCenter: parent.horizontalCenter
    }
    property string filepath : "/"

    FileSystemTree {
        id: fsTree
        anchors.top: modes.bottom
        anchors.left: root.left
        width: parent.width / 3
        height: root.height - modes.height
    }

    function createImageWindow(fileurl) {
        var component = Qt.createComponent("ImageWindow.qml")
        var window = component.createObject(root, {"path" : fileurl})
        window.show()
    }

    Rectangle{
        id: rect_view
        anchors.top: modes.bottom
        anchors.left: fsTree.right
        anchors.leftMargin: 20
        height: parent.height - modes.height
        width: parent.width - fsTree.width - 20
        states: [
            State {
                name: "stateList"
                PropertyChanges { target: list; visible: true }
            },
            State {
                name: "stateGrid"
                PropertyChanges { target: grid; visible: true }
            },
            State {
                name: "statePathView"
                PropertyChanges { target: pathView; visible: true; focus: true }
            }
        ]
        state: "stateList"

        FolderListModel {
            id: imgModel
            folder:"file://" + filepath
            showDirs: false
            nameFilters: ["*.gif", "*.png", "*.jpg", "*.jpeg", "*.bmp", "*.svg"]
        }

        ListView{
            id: list
            anchors.fill: parent
            visible: false
            model: imgModel
            ScrollBar.vertical: ScrollBar { active: true }
            delegate:
                Component {
                    Item {
                        width: parent.width
                        height: 40
                        MouseArea {
                            anchors.fill: parent
                            onClicked: { createImageWindow(fileURL) }
                        }
                        Row{
                            spacing: 20
                            Image {
                                id: img_list
                                asynchronous: true
                                width: 30
                                height: 30
                                fillMode: Image.PreserveAspectCrop
                                source: fileURL
                            }
                            Text {
                                anchors.verticalCenter: img_list.verticalCenter
                                wrapMode: Text.NoWrap
                                elide: Text.ElideRight
                                text: fileName
                            }
                        }
                    }
                }
        }

        GridView{
            id: grid
            anchors.fill: parent
            anchors.horizontalCenter: parent.horizontalCenter
            visible: false
            model: imgModel
            delegate: Component {
                    Item {
                        width: parent.width
                        height: 40
                        MouseArea {
                            anchors.fill: parent
                            onClicked: { createImageWindow(fileURL) }
                        }
                        Column{
                            Image {
                                id: img_grid
                                anchors.horizontalCenter: parent.horizontalCenter
                                asynchronous: true
                                width: 45
                                height: 45
                                fillMode: Image.PreserveAspectCrop
                                source: fileURL
                            }
                            Text{
                                width: img_grid.width
                                anchors.horizontalCenter: parent.horizontalCenter
                                elide: Text.ElideRight
                                maximumLineCount: 3
                                wrapMode: Text.Wrap
                                text: fileName
                            }

                        }
                    }
                }

            ScrollBar.vertical: ScrollBar {
                                    active: true
                                }
        }

        PathView {
            id: pathView
            visible: false
            anchors.fill: parent
            model: imgModel
            pathItemCount: 5
            delegate: Component {
                Item {
                    width: parent.width
                    height: 40
                    opacity: PathView.isCurrentItem ? 1 : 0.5
                    MouseArea {
                        anchors.fill: parent
                        onClicked: { createImageWindow(fileURL) }
                    }
                    Column{
                        id: col_pathView
                        Image {
                            id: img
                            anchors.horizontalCenter: parent.horizontalCenter
                            asynchronous: true
                            width: 50
                            height: 50
                            fillMode: Image.PreserveAspectCrop
                            source: fileURL
                        }
                        Text{
                            width: img.width
                            elide: Text.ElideRight
                            maximumLineCount: 2
                            wrapMode: Text.Wrap
                            text: fileName
                        }
                    }
                }
            }
            path: Path {
                startX: fsTree.width + rect_view.width / 10
                startY: rect_view.height / 2
                PathArc {
                    x: fsTree.width + rect_view.width * 7 / 10
                    y: rect_view.height / 2 + 100
                    radiusX: rect_view.width / 3
                    radiusY: rect_view.height / 3
                    useLargeArc: true
                }
            }
            focus: true
            Keys.onLeftPressed: decrementCurrentIndex()
            Keys.onRightPressed: incrementCurrentIndex()
            MouseArea{
               id:mouse
               anchors.fill: parent
               onWheel: {
                   if( wheel.angleDelta.y > 0 )
                       pathView.incrementCurrentIndex()
                   else
                       pathView.decrementCurrentIndex()
                }
               onClicked: { pathView.incrementCurrentIndex() }
            }
        }
    }
}
