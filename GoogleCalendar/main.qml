import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.VirtualKeyboard 2.2
import QtQuick.Controls 1.6
import Calendar 1.0

Window {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    Calendar{
    id: calendar
    }

    InputPanel {
        id: inputPanel
        z: 99
        x: 0
        y: window.height
        width: window.width

        states: State {
            name: "visible"
            when: inputPanel.active
            PropertyChanges {
                target: inputPanel
                y: window.height - inputPanel.height
            }
        }
        transitions: Transition {
            from: ""
            to: "visible"
            reversible: true
            ParallelAnimation {
                NumberAnimation {
                    properties: "y"
                    duration: 250
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }

    Button {
        id: button_Login
        x: 69
        y: 52
        text: qsTr("Login")
        onClicked: {
        calendar.Login();
        }
    }

    Button {
        id: button_GetEvents
        x: 192
        y: 52
        text: qsTr("Get Events")
    }

    Text {
        id: text_Content
        x: 7
        y: 114
        width: 622
        height: 346
        text: qsTr("")
        font.pixelSize: 13
    }
}
