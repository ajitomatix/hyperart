import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import HyperArt 1.0

ApplicationWindow {
    id: window
    width: 1024
    height: 768
    visible: true
    title: qsTr("Hyper Art") + (appController.documentTitle ? " - " + appController.documentTitle : "")
    
    AppController {
        id: appController
        onLoadFailed: function(message) {
            statusBarText.text = qsTr("Error: ") + message;
        }
        onDiagramChanged: {
            if (hasDiagram) {
                statusBarText.text = qsTr("File loaded successfully.");
            }
        }
    }

    Action {
        id: openAction
        text: qsTr("&Open...")
        icon.name: "document-open"
        shortcut: "Ctrl+O"
        onTriggered: fileDialog.open()
    }
    Action {
        id: saveAsAction
        text: qsTr("&Save As...")
        icon.name: "document-save-as"
        shortcut: "Ctrl+S"
        enabled: appController.hasDiagram
        onTriggered: saveFileDialog.open()
    }
    Action {
        id: playPauseAction
        text: animTimer.running ? qsTr("&Pause") : qsTr("&Play")
        icon.name: animTimer.running ? "media-playback-pause" : "media-playback-start"
        enabled: appController.hasDiagram && appController.totalAnimationSteps > 0
        shortcut: "Space"
        onTriggered: {
            if (animTimer.running) {
                animTimer.stop()
            } else {
                if (appController.animationStep < 0 || appController.animationStep >= appController.totalAnimationSteps) {
                    appController.animationStep = 0
                }
                animTimer.start()
            }
        }
    }
    Action {
        id: stopAction
        text: qsTr("St&op")
        icon.name: "media-playback-stop"
        enabled: appController.hasDiagram && appController.animationStep >= 0
        shortcut: "Escape"
        onTriggered: {
            animTimer.stop()
            appController.animationStep = -1
        }
    }
    Action {
        id: nextStepAction
        text: qsTr("Next Frame")
        icon.name: "media-seek-forward"
        enabled: appController.hasDiagram && appController.animationStep < appController.totalAnimationSteps
        shortcut: "Right"
        onTriggered: appController.nextAnimationStep()
    }
    Action {
        id: prevStepAction
        text: qsTr("Previous Frame")
        icon.name: "media-seek-backward"
        enabled: appController.hasDiagram && appController.animationStep > 0
        shortcut: "Left"
        onTriggered: appController.prevAnimationStep()
    }
    Action {
        id: toggleLayersAction
        text: qsTr("Toggle &Layers")
        icon.name: "view-list-details"
        shortcut: "Ctrl+L"
        onTriggered: layerDrawer.open()
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem { action: openAction }
            MenuItem { action: saveAsAction }
        }
        Menu {
            title: qsTr("&View")
            MenuItem { action: toggleLayersAction }
        }
        Menu {
            title: qsTr("&Animation")
            MenuItem { action: playPauseAction }
            MenuItem { action: stopAction }
            MenuSeparator {}
            MenuItem { action: prevStepAction }
            MenuItem { action: nextStepAction }
        }
    }

    FileDialog {
        id: fileDialog
        title: qsTr("Open a HyperArt Design")
        nameFilters: ["HyperArt Designs (*.had)"]
        onAccepted: {
            console.log("QML requested to open: " + selectedFile)
            appController.openDesign(selectedFile)
        }
    }

    FileDialog {
        id: saveFileDialog
        title: qsTr("Save Design As")
        fileMode: FileDialog.SaveFile
        nameFilters: ["PNG Image (*.png)", "JPEG Image (*.jpg)"]
        onAccepted: {
            console.log("QML requested to save: " + selectedFile)
            canvas.saveAs(selectedFile)
        }
    }

    Timer {
        id: animTimer
        interval: 16 // roughly 60fps
        repeat: true
        running: false
        onTriggered: {
            if (appController.animationStep < appController.totalAnimationSteps) {
                appController.animationStep += 1;
            } else {
                running = false; // stop timer
                appController.animationStep = -1; // seamlessly revert to layer mode
            }
        }
    }

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            
            ToolButton { action: prevStepAction; display: AbstractButton.IconOnly; ToolTip.text: qsTr("Previous Frame"); ToolTip.visible: hovered }
            ToolButton { action: playPauseAction; display: AbstractButton.IconOnly; ToolTip.text: animTimer.running ? qsTr("Pause") : qsTr("Play"); ToolTip.visible: hovered }
            ToolButton { action: stopAction; display: AbstractButton.IconOnly; ToolTip.text: qsTr("Stop"); ToolTip.visible: hovered }
            ToolButton { action: nextStepAction; display: AbstractButton.IconOnly; ToolTip.text: qsTr("Next Frame"); ToolTip.visible: hovered }

            Item { Layout.fillWidth: true }
            
            ToolButton { action: toggleLayersAction; display: AbstractButton.TextBesideIcon; ToolTip.text: qsTr("Toggle Layers"); ToolTip.visible: hovered }
        }
    }

    Drawer {
        id: layerDrawer
        width: 200
        height: window.height
        edge: Qt.RightEdge

        background: Rectangle {
            color: window.palette.window
        }

        Column {
            anchors.fill: parent
            anchors.margins: 15
            spacing: 10
            
            Label {
                text: "Pattern Layers"
                font.bold: true
                color: window.palette.windowText
            }
            
            Repeater {
                model: appController.hasDiagram ? appController.numLayers : 0
                delegate: CheckBox {
                    text: qsTr("Layer ") + index
                    checked: appController.isLayerVisible(index)
                    contentItem: Text {
                        text: parent.text
                        color: window.palette.windowText
                        leftPadding: parent.indicator.width + parent.spacing
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: {
                        appController.toggleLayer(index, checked)
                    }
                }
            }
        }
    }

    // The central rendering view powered by C++ QQuickPaintedItem (QPainter via FBO)
    Rectangle {
        anchors.fill: parent
        color: canvas.backgroundColor

        HyperArtCanvas {
            id: canvas
            anchors.fill: parent
            anchors.margins: 20
            controller: appController
            backgroundColor: window.palette.base
        }
    }
    
    footer: ToolBar {
        Label {
            id: statusBarText
            text: qsTr("Ready")
            padding: 5
        }
    }
}
