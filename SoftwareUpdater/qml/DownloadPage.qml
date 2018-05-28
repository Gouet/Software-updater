import QtQuick 2.0
import QtQuick.Controls 1.4 as C
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 2.3
import UpdaterController 1.0

Item {
    id: downloadPage
    signal pressedCancelButton()
    signal downdloadFailed();
    signal downloadSuccess();
    property var window

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
        source: Qt.platform.os == "windows" ? "images/logo_500dpi_dnai.png" : "images/logo_300dpi_dnai.png"
    }

    Text {
        id: title
        text: qsTr("Switch to version " + Controller.version)
        font.family: sfLight.name
        color: "white"
        font.pointSize:  Qt.platform.os == "windows" ? 14 : 18
        anchors.top: logo.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: logo.horizontalCenter
    }

    UpdaterPopUp {
        id: popupError
        window: downloadPage.window
        title: "Failed to update"
        desc: "An error occured.\nThe update has failed."
        showNoButton: false
        yesStringButton: "Ok"
        closePolicy: Popup.NoAutoClose

        onPressedYesButton: {
            console.log("STOP")
            downdloadFailed()
            //pressedCancelButton()
            popupError.close()
        }
    }

    Connections {
        target: Controller

        onInternetFailed: {
            popupError.open()
            console.log('FAILED')
        }

        onFilesMovedSuccess: {
            console.log('SUCCESS')
            downloadPage.downloadSuccess()
        }

        onFilesMovedFailed: {
            console.log('FAILED')
            popupError.open()
        }
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

        property int avancement: 0

        id: scrollBar
        value: Controller.avancement
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
                    implicitHeight: Qt.platform.os == "windows" ? 30 : 24
                }
                progress: Rectangle {
                    color: "#D8C9FF"
                    border.color: "#35334A"
                }
            }
    }


    property int rectWidth: Qt.platform.os == "windows" ? 160 : 80
    property int rectHeight: Qt.platform.os == "windows" ? 70 : 35

    Rectangle {
        id: itemCancel
        width: backgroundDownload.rectWidth
        height: backgroundDownload.rectHeight
        color: "#CCCCCC"
        radius: 7
        border.width: 1
        border.color: "#35334A"
        anchors.top: scrollBar.bottom
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
             font.pointSize: Qt.platform.os == "windows" ? 10 : 14
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
