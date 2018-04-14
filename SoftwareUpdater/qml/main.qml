import QtQuick 2.6
import QtQuick.Controls 2.3
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import UpdaterController 1.0

Item {
    id: window
    visible: true
    width: Qt.platform.os == "windows" ? 1200 : 800
    height: Qt.platform.os == "windows" ? 750 : 450

    FontLoader { id: sfLightItalic; source: "fonts/SF UI Text Light Italic.otf" }
    FontLoader { id: sfLight; source: "fonts/SF UI Text Light.otf" }
    FontLoader { id: sfBold; source: "fonts/SF UI Text Bold.otf" }

    HomePage {
        anchors.fill: window
        id: homePage
        visible: true
        window: window
        width: window.width
        height: window.height

        onUpdate: {
            homePage.visible = false
            downloadPage.visible = true
            downloadPage.start()
        }
    }

    DownloadPage {
        id: downloadPage
        anchors.fill: window
        visible: false
        window: window

        onDowndloadFailed: {
            homePage.visible = true
            downloadPage.visible = false
        }

        onPressedCancelButton: {
            homePage.visible = true
            downloadPage.visible = false
            Controller.cancel()
        }

        onDownloadSuccess: {
            launchPage.visible = true
            downloadPage.visible = false
        }
    }

    LaunchPage {
        id: launchPage
        window: window
        anchors.fill: window
        visible: false
    }

}
