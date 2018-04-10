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
       // anchors.verticalCenter: background.verticalCenter
    }

    Image {
        id: logo
        anchors.horizontalCenter: successText.horizontalCenter
        anchors.verticalCenter: background.verticalCenter
        //anchors.top: back.bottom
        //anchors.topMargin: 20
        source: "images/logo_250dpi_dnai.png"
    }

    Rectangle {
        id: buttonLaunch
        anchors.top: logo.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: logo.horizontalCenter
        radius: 7
        height: 50
        width: logo.width// + 30
        color: "#D8C9FF"
        /*SequentialAnimation on color
        {
            loops: Animation.Infinite

            ColorAnimation
            {
                from: "#D8C9FF"
                to: "#303030"
                duration: 2500
            }
            ColorAnimation
            {
                from: "#303030"
                to: "#D8C9FF"
                duration: 0
            }
        }*/

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
