import QtQuick 2.0
import QtQuick.Controls 1.4 as C
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 2.3
import UpdaterController 1.0

Item {
    signal pressedCancelButton()

    function start() {
        Controller.start()
    }

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

    Text {
        id: title
        text: qsTr("Switch to version " + Controller.version)
        font.family: sfLight.name
        color: "white"
        font.pointSize: 18
        anchors.top: logo.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: logo.horizontalCenter
    }

    Rectangle {
        id: backgroundDownload
        anchors.top: title.bottom
        anchors.left: background.left
        anchors.right: background.right
        anchors.bottom: background.bottom
        anchors.margins: 20
        anchors.topMargin: 10
        anchors.bottomMargin: 0
        color: "transparent"

    C.ProgressBar {
        id: scrollBar
        value: 0.5
        anchors.top: backgroundDownload.top
        anchors.topMargin: 10
        anchors.left: backgroundDownload.left
        anchors.leftMargin: 40
        anchors.right: backgroundDownload.right
        anchors.rightMargin: 40
        style: ProgressBarStyle {
                background: Rectangle {
                    radius: 2
                    color: "#35334A"
                    border.color: "#35334A"
                    border.width: 1
                    implicitWidth: 200
                    implicitHeight: 24
                }
                progress: Rectangle {
                    color: "#D8C9FF"
                    border.color: "#35334A"
                }
            }
    }



   /* ScrollView {
        anchors.top: scrollBar.bottom
        anchors.bottom: backgroundDownload.bottom
        anchors.left: scrollBar.left
        anchors.right: scrollBar.right
    //    anchors.topMargin: 15
        anchors.margins: 15
        background: Rectangle {
            color: "#DDDDDD"
            radius: 5
        }
        clip: true

        ListView {
            model: 20
            delegate: ItemDelegate {
                id: itemDelegate
                highlighted: false
                text: "Item " + index
                font.family: sfLight.name
                font.pointSize: 12
                height: itemDelegate.implicitHeight - 15
                MouseArea {
                    anchors.fill: parent
                }
            }
        }
    }*/

    property int rectWidth: 80
    property int rectHeight: 35

    Rectangle {
        id: itemCancel
        width: backgroundDownload.rectWidth
        height: backgroundDownload.rectHeight
        color: "#CCCCCC"
        radius: 7
        border.width: 1
        border.color: "#35334A"
        anchors.top: scrollBar.bottom
     //   anchors.left: backgroundDownload.left
        anchors.topMargin: 20
        anchors.horizontalCenter: backgroundDownload.horizontalCenter

        function enter() {
             itemCancel.color = "#AAAAAA"
        }

        function exit() {
             itemCancel.color = "#CCCCCC"
        }

        Text {
             anchors.centerIn: parent
             text: "Cancel"
             color: "black"
             font.pointSize: 14
             font.bold: false
             font.family: sfLight.name
        }
        MouseArea {
            id: cancelMouse
            hoverEnabled: true
            anchors.fill: parent
            onClicked: {
                pressedCancelButton()
             }
            onEntered: {
                cancelMouse.cursorShape = Qt.PointingHandCursor
                itemCancel.enter()
            }

            onExited: {
                cancelMouse.cursorShape = Qt.ArrowCursor
                itemCancel.exit()
            }
        }
    }

    }


}
