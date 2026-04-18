import QtQuick
import QtQuick.Window

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("HyperArt QML Prototype")

    Rectangle {
        anchors.fill: parent
        color: "#202020"

        Text {
            anchors.centerIn: parent
            text: "Welcome to HyperArt QML!\n(Core Library Linked Successfully)"
            color: "#ffffff"
            font.pixelSize: 24
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
