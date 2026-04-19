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
    title: qsTr("Hyper Art")
    
    // We hold a single instance of AppController tied to our window lifecycle
    AppController {
        id: appController
        onLoadFailed: function(message) {
            statusBarText.text = qsTr("Error: ") + message;
        }
        onDiagramChanged: {
            statusBarText.text = qsTr("File loaded successfully.");
        }
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("&Open...")
                onTriggered: fileDialog.open()
            }
            MenuItem {
                text: qsTr("&Save As...")
                onTriggered: saveFileDialog.open()
            }
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
            
            ToolButton {
                text: qsTr(animTimer.running ? "Pause" : "Play")
                enabled: appController.hasDiagram && appController.totalAnimationSteps > 0
                onClicked: {
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
            
            ToolButton {
                text: qsTr("Stop")
                enabled: appController.hasDiagram && appController.animationStep >= 0
                onClicked: {
                    animTimer.stop()
                    appController.animationStep = -1
                }
            }

            Item { Layout.fillWidth: true }
            ToolButton {
                text: qsTr("Layers")
                onClicked: layerDrawer.open()
            }
        }
    }

    Drawer {
        id: layerDrawer
        width: 200
        height: window.height
        edge: Qt.RightEdge

        background: Rectangle {
            color: "#2a2a2a"
        }

        Column {
            anchors.fill: parent
            anchors.margins: 15
            spacing: 10
            
            Label {
                text: "Pattern Layers"
                font.bold: true
                color: "white"
            }
            
            Repeater {
                model: appController.hasDiagram ? appController.numLayers : 0
                delegate: CheckBox {
                    text: qsTr("Layer ") + index
                    checked: appController.isLayerVisible(index)
                    contentItem: Text {
                        text: parent.text
                        color: "white"
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
        color: "#1e1e1e" // sleek dark mode aesthetic

        HyperArtCanvas {
            id: canvas
            anchors.fill: parent
            anchors.margins: 20
            controller: appController
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
