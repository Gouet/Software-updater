import QtQuick 2.6
import QtQuick.Controls 2.3
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import UpdaterController 1.0

Item {
    id: homePage
    property int commonMargin: 10
    property var window

    signal update()

    Image {
        id: background
        anchors.fill: parent
        source: "images/background.png"
    }

    Image {
        id: logo
        anchors.horizontalCenter: background.horizontalCenter
        anchors.top: background.top
        anchors.topMargin: 35
        source: "images/logo_300dpi_dnai.png"
    }

    Rectangle {
        id: buttonLaunch
        anchors.top: logo.bottom
        anchors.topMargin: homePage.commonMargin + 40
        anchors.horizontalCenter: logo.horizontalCenter
        radius: 7
        height: 50
        width: logo.width// + 30
        color: "#D8C9FF"

        Text {
            property string hoverColor: "#FFFFFF"
            property string notHoverColor: "#35334A"

            id: buttonLaunchText
            text: qsTr("Update")
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
                console.log("CLIC")
                popup.open()
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
        anchors.topMargin: commonMargin
        anchors.horizontalCenter: buttonLaunch.horizontalCenter
        text: qsTr("Version ") + Controller.prevVersion + " ~> " + Controller.version
    }

    UpdaterPopUp {
        id: popup
        window: homePage.window
        title: qsTr("To update, DNAI need to restart.")
        desc: qsTr("Restart now ?")

        onPressedNoButton: {
            console.log("PRESSED NO")
            close()
        }

        onPressedYesButton: {
            console.log("PRESSED YES")

            close()

            homePage.update()
        }
    }
}
