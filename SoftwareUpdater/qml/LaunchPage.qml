import QtQuick 2.0
import QtGraphicalEffects 1.0
import UpdaterController 1.0

Item {
    id: launchPage

    property var window
    property int commonMargin: 10

    Image {
        id: background
        anchors.fill: parent
        source: "images/background.png"
    }

    Text {
        id: successText
        text: qsTr("Successful update !")
        font.family: sfBold.name
        color: "#D8C9FF"
        font.pointSize: 40
        font.bold: false
        anchors.horizontalCenter: background.horizontalCenter
        anchors.top: background.top
        anchors.topMargin: 20
    }

    Image {
        id: logo
        anchors.horizontalCenter: successText.horizontalCenter
        anchors.verticalCenter: background.verticalCenter
        source: Qt.platform.os == "windows" ? "images/logo_300dpi_dnai.png" : "images/logo_250dpi_dnai.png"
    }

    Rectangle {
        id: buttonLaunch
        anchors.top: logo.bottom
        anchors.topMargin: Qt.platform.os == "windows" ? 40 : 20
        anchors.horizontalCenter: logo.horizontalCenter
        radius: 7
        height: Qt.platform.os == "windows" ? 85 : 50
        width: Qt.platform.os == "windows" ? 55 + buttonLaunchText.width : logo.width
        color: "#D8C9FF"

        Text {
            property string hoverColor: "#FFFFFF"
            property string notHoverColor: "#35334A"

            id: buttonLaunchText
            text: qsTr("Launch DNAI")
            font.family: sfBold.name
            color: notHoverColor
            font.pointSize: 24
            font.bold: false
            anchors.horizontalCenter: buttonLaunch.horizontalCenter
            anchors.verticalCenter: buttonLaunch.verticalCenter

            function enter() {
                buttonLaunchText.color = hoverColor
            }

            function exit() {
               buttonLaunchText.color = notHoverColor
            }
        }

        MouseArea {
            id: buttonLaunchMouse
            anchors.fill: parent
            hoverEnabled: true
            onClicked: {
                Controller.launchApplication()
                Controller.quit()
            }
            onEntered: {
                buttonLaunchMouse.cursorShape = Qt.PointingHandCursor
                buttonLaunchText.enter()
            }
            onExited: {
                buttonLaunchMouse.cursorShape = Qt.ArrowCursor
                buttonLaunchText.exit()
            }
        }
    }
}
