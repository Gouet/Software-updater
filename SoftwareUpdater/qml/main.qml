import QtQuick 2.6
import QtQuick.Window 2.2

Item {
    id: window
    visible: true
    width: 800
    height: 450
    property int commonMargin: 10

    FontLoader { id: sfLightItalic; source: "fonts/SF UI Text Light Italic.otf" }
    FontLoader { id: sfLight; source: "fonts/SF UI Text Light.otf" }
    FontLoader { id: sfBold; source: "fonts/SF UI Text Bold.otf" }

    Image {
        id: background
        anchors.fill: parent
        source: "images/background.png"
    }

    Image {
        id: logo
        x: window.width / 2 - logo.width / 2
        y: 35
        source: "images/logo_300dpi_dnai.png"
    }



    Rectangle {
        id: buttonLaunch
        anchors.top: logo.bottom
        anchors.topMargin: window.commonMargin + 40
        anchors.horizontalCenter: logo.horizontalCenter
        radius: 7
        height: 50
        width: logo.width// + 30
        color: "#D8C9FF"

        Text {
            property string hoverColor: "#FFFFFF"
            property string notHoverColor: "#35334A"

            id: buttonLaunchText
            text: qsTr("Update !")
            font.family: sfBold.name
            color: notHoverColor
            font.pointSize: 24
            font.bold: true
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
                console.log("CLIC")
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

    Text {
        id: version
        color: "white"
        font.pointSize: 12
        font.family: sfLightItalic.name
        font.italic: true
        anchors.top: buttonLaunch.bottom
        anchors.topMargin: window.commonMargin
        anchors.horizontalCenter: buttonLaunch.horizontalCenter
        text: qsTr("Version 0.1.13 ~> 0.1.14")
    }

}
