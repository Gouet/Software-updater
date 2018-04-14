import QtQuick 2.6
import QtQuick.Controls 2.3
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3

Item {
    id: popupManager
    property var window
    property string title: ""
    property string desc: ""

    property real closePolicy: Popup.CloseOnPressOutside

    property bool showNoButton: true
    property bool showYesButton: true

    property string yesStringButton: "Yes"
    property string noStringButton: "No"

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
           width: Qt.platform.os == "windows" ? 500 : 250
           height: Qt.platform.os == "windows" ? 280 : 140
           modal: true
           focus: true
           closePolicy: popupManager.closePolicy //Popup.CloseOnPressOutside
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
               parent: popupbackground
               horizontalAlignment: Text.AlignHCenter
               width: popup.width - popup.padding * 2
               font.pointSize: Qt.platform.os == "windows" ? 15 : 17
               font.family: sfLight.name
               wrapMode: Text.WordWrap
               anchors.top: popupbackground.top
               anchors.topMargin: Qt.platform.os == "windows" ? 20 : 15
               font.bold: false
               color: "#35334A"
               text: title
           }

           Text {
               id: popupDesc
               parent: popupbackground
               horizontalAlignment: Text.AlignHCenter
               width: popup.width - popup.padding * 2
               font.pointSize: Qt.platform.os == "windows" ? 12 : 14
               font.family: sfLight.name
               anchors.top: popupTitle.bottom
               anchors.topMargin: 10
               wrapMode: Text.WordWrap
               text: desc
           }

           Item {
               parent: popupbackground
               anchors.bottom: popupbackground.bottom
               anchors.topMargin: -10
               anchors.top: popupDesc.bottom
               anchors.left: popupbackground.left
               anchors.right: popupbackground.right


           Row {
               parent: parent
               id: rowLayout
               anchors.centerIn: parent
               //anchors.horizontalCenter: parent.horizontalCenter
              // anchors.top: popupDesc.bottom
              // anchors.margins: 15

               spacing: 30

               property int rectWidth: Qt.platform.os == "windows" ? 160 : 80
               property int rectHeight: Qt.platform.os == "windows" ? 70 : 35

               Rectangle {
                   id: itemNo
                   width: rowLayout.rectWidth
                   height: rowLayout.rectHeight
                   color: "#CCCCCC"
                   radius: 7
                   border.width: 1
                   border.color: "#35334A"
                   visible: popupManager.showNoButton

                   function enter() {
                        itemNo.color = "#AAAAAA"
                   }

                   function exit() {
                        itemNo.color = "#CCCCCC"
                   }

                   Text {
                        anchors.centerIn: parent
                        text: popupManager.noStringButton
                        color: "black"
                        font.pointSize: Qt.platform.os == "windows" ? 12 : 14
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
                   visible: popupManager.showYesButton


                   Text {
                       id: yesTextPopUp
                        anchors.centerIn: parent
                        text: popupManager.yesStringButton
                        font.pointSize: Qt.platform.os == "windows" ? 14 : 16
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
}
