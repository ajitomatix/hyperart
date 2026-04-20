pragma Singleton
import QtQuick

QtObject {
    id: windowManager

    property int cascadeStep: 0
    property var lastWindow: null

    function openWindow(fileUrl) {
        var component = Qt.createComponent("DesignWindow.qml");
        
        if (component.status === Component.Error) {
            console.error("Error loading DesignWindow.qml:", component.errorString());
            return;
        }

        var properties = {};
        
        // If we have an existing window, calculate the offset (cascading)
        // Otherwise, let the OS decide the position for the first window
        if (cascadeStep > 0 && lastWindow) {
            properties.x = lastWindow.x + 24;
            properties.y = lastWindow.y + 24;
        }

        var win = component.createObject(null, properties);
        
        if (win) {
            win.show();
            if (fileUrl) {
                win.appController.openDesign(fileUrl);
            }
            
            lastWindow = win;
            cascadeStep++;
            
            // Basic wrap-around logic for the cascading effect
            if (cascadeStep > 10) {
                cascadeStep = 1; // start from 1 again, or reset base
            }
        } else {
            console.error("Failed to create DesignWindow object");
        }
    }
}
