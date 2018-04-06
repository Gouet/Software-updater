import QtQuick 2.6
import QtQuick.Controls 2.3
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3

Item {
    property var window
    property string title: ""
    property string desc: ""
    signal pressedNoButton
    signal pressedYesButton

    function open() {
        popup.open()
    }

    function close() {
        popup.close()
    }

    Popup {
           id: popup
           x: window.width / 2 - popup.width / 2
           y: window.height / 2 - popup.height / 2
           width: 250
           height: 140
           modal: true
           focus: true
           closePolicy: Popup.CloseOnPressOutside
           padding: 10


           background: popupbackground

           Rectangle {
                id: popupbackground
                width: popup.width
                height: popup.height
                x: 0// - popup.padding
                y: 0// - popup.padding
                border.color: "#35334A"
                border.width: 1
                radius: 15
                color: "white"
          }

           Text {
               id: popupTitle
               horizontalAlignment: Text.AlignHCenter
               width: popup.width - popup.padding * 2
               font.pointSize: 17
               font.family: sfLight.name
               wrapMode: Text.WordWrap
               anchors.top: popupbackground.top
               anchors.topMargin: 30
               font.bold: false
               color: "#35334A"
               text: title
           }

           Text {
               id: popupDesc
               horizontalAlignment: Text.AlignHCenter
               width: popup.width - popup.padding * 2
               font.pointSize: 14
               font.family: sfLight.name
               anchors.top: popupTitle.bottom
               anchors.topMargin: 10
               wrapMode: Text.WordWrap
               text: desc
           }

           Row {
               id: rowLayout
               anchors.horizontalCenter: popupDesc.horizontalCenter
               anchors.top: popupDesc.bottom
               anchors.topMargin: 15

             //  anchors.bottom: popupbackground.bottom
              // anchors.bottomMargin: 5
               spacing: 30

               property int rectWidth: 80
               property int rectHeight: 35

               Rectangle {
                   id: itemNo
                   width: rowLayout.rectWidth
                   height: rowLayout.rectHeight
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
                           pressedNoButton()
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
                   width: rowLayout.rectWidth
                   height: rowLayout.rectHeight
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
                           pressedYesButton()
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
