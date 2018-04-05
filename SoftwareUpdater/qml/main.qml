import QtQuick 2.6
import QtQuick.Controls 2.3
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3

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
        anchors.topMargin: window.commonMargin
        anchors.horizontalCenter: buttonLaunch.horizontalCenter
        text: qsTr("Version 0.1.13 ~> 0.1.14")
    }

    Popup {
           id: popup
           x: window.width / 2 - popup.width / 2
           y: window.height / 2 - popup.height / 2
           width: 300
           height: 200
           modal: true
           focus: true
           closePolicy: Popup.CloseOnPressOutside
           padding: 10

           Rectangle {
               id: popupbackground
               //anchors.fill: popup
               width: popup.width
               height: popup.height
               x: 0 - popup.padding
               y: 0 - popup.padding
               border.color: "#35334A"
               border.width: 5
               color: "white"
           }

           Text {
               id: popupTitle
               horizontalAlignment: Text.AlignHCenter
               width: popup.width - popup.padding * 2
               font.pointSize: 20
               font.family: sfBold.name
               anchors.topMargin: 20
               font.bold: true
               color: "#35334A"
               text: qsTr("Warning !")
           }

           Text {
               id: popupDesc
               horizontalAlignment: Text.AlignHCenter
               width: popup.width - popup.padding * 2
               font.pointSize: 14
               font.family: sfLight.name
               anchors.top: popupTitle.bottom
               anchors.topMargin: 20
               wrapMode: Text.WordWrap
               text: qsTr("You have to close the DNAI application before starting update.\n\nDo you want to close the application ?")
           }

           Row {
               id: rowLayout
               anchors.horizontalCenter: popupbackground.horizontalCenter
               anchors.bottom: popupbackground.bottom
               anchors.bottomMargin: 15
               spacing: 20

               Rectangle {
                   id: itemNo
                   width: 100
                   height: 50
                   color: "#CCCCCC"
                   radius: 7
                   border.width: 1
                   border.color: "#35334A"

                   function enter() {
                        itemNo.color = "#AAAAAA"
                   }

                   function exit() {
                        itemNo.color = "#CCCCCC"
                   }

                   Text {
                        anchors.centerIn: parent
                        text: "No"
                        color: "black"
                        font.pointSize: 14
                        font.bold: false
                        font.family: sfLight.name
                   }
                   MouseArea {
                       id: noMouse
                       hoverEnabled: true
                       anchors.fill: parent
                       onClicked: {
                        }
                       onEntered: {
                           noMouse.cursorShape = Qt.PointingHandCursor
                           itemNo.enter()
                       }

                       onExited: {
                           noMouse.cursorShape = Qt.ArrowCursor
                           itemNo.exit()
                       }
                   }
               }
               Rectangle {
                   id: itemYes
                   width: 100
                   height: 50
                   color: "#D8C9FF"
                   radius: 7
                   border.width: 1
                   border.color: "#35334A"


                   Text {
                       id: yesTextPopUp
                        anchors.centerIn: parent
                        text: "Yes"
                        font.pointSize: 16
                        font.bold: true
                        font.family: sfBold.name
                        color: yesTextPopUp.notHoverColor
                        property string hoverColor: "#FFFFFF"
                        property string notHoverColor: "#35334A"

                        function enter() {
                             yesTextPopUp.color = yesTextPopUp.hoverColor
                        }

                        function exit() {
                             yesTextPopUp.color = yesTextPopUp.notHoverColor
                        }

                   }
                   MouseArea {
                       id: yesMouse
                       hoverEnabled: true
                       anchors.fill: parent
                       onClicked: {
                        }
                       onEntered: {
                           yesMouse.cursorShape = Qt.PointingHandCursor
                           yesTextPopUp.enter()
                       }

                       onExited: {
                           yesMouse.cursorShape = Qt.ArrowCursor
                           yesTextPopUp.exit()
                       }
                   }
               }
           }
       }

}
