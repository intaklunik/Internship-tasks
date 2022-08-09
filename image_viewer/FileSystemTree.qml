import QtQuick.Controls 1.5

TreeView {
    model: fileSystemModel
    rootIndex: rootPathIndex
    TableViewColumn {
        title: "Folder name"
        role: "fileName"
        resizable: true
    }
    onClicked: {
        var url = fileSystemModel.data(index, "DisplayRole")
        filepath = url
    }

}
