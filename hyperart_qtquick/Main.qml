import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import hyperart.main

ApplicationWindow {
    id: appRoot
    visible: false
    width: 0
    height: 0

    Action {
        id: newWindowAction
        text: qsTr("New Window")
        shortcut: "Ctrl+N"
        onTriggered: WindowManager.openWindow(null)
    }

    Action {
        id: openAction
        text: qsTr("Open...")
        shortcut: "Ctrl+O"
        onTriggered: globalFileDialog.open()
    }

    Action {
        id: quitAction
        text: qsTr("Quit")
        shortcut: "Ctrl+Q"
        onTriggered: Qt.quit()
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem { action: newWindowAction }
            MenuItem { action: openAction }
            MenuItem { action: quitAction }
        }
    }

    FileDialog {
        id: globalFileDialog
        title: qsTr("Open a HyperArt Design")
        nameFilters: ["HyperArt Designs (*.had)"]
        onAccepted: {
            WindowManager.openWindow(selectedFile)
        }
    }

    Component.onCompleted: {
        WindowManager.openWindow(null);
    }
}
