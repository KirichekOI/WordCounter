import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import QtCharts 2.5
import QtQml 2.15

Window {
    id: window
    visible: true
    width: 640
    height: 480
    minimumWidth: 640
    minimumHeight: 480
    title: qsTr("Word counter")

    FileDialog{
        id: openFileDialog
        title: "Open file"
        folder: shortcuts.home
        selectExisting: true
        nameFilters: ["All files (*)"]
        onAccepted: {
            textFilePath.text = fileUrl
            btnStartParsing.enabled = true
            readFileEngine.filePath = fileUrl
        }
        onRejected: {
            textFilePath.text = "Canceled"
            btnStartParsing.enabled = false
        }
    }

    Button {
        id: btnOpenFile
        x: 521
        width: 150
        text: qsTr("Open file")
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 20
        onClicked: openFileDialog.open();
    }

    Button {
        id: btnStartParsing
        enabled: false
        text: qsTr("Start parsing")
        anchors.right: btnOpenFile.right
        anchors.rightMargin: 0
        anchors.left: btnOpenFile.left
        anchors.leftMargin: 0
        anchors.top: btnOpenFile.bottom
        anchors.topMargin: 10
        onClicked: {
            readFileEngine.startParsing()
            btnOpenFile.enabled = false
            btnStartParsing.enabled = false
        }
    }

    ProgressBar {
        id: progressBar
        y: 450
        height: 20
        anchors.right: textFilePath.right
        anchors.rightMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 20
        value: 0.0
    }

    TextField {
        id :textFilePath
        height: 40
        anchors.right: btnOpenFile.left
        anchors.rightMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 20
        readOnly: true
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }

    ChartViewItem {
        id: chartView
        visible: true
        enabled: true
        anchors.top: textFilePath.bottom
        anchors.topMargin: 10
        anchors.right: textFilePath.right
        anchors.rightMargin: 0
        anchors.bottom: progressBar.top
        anchors.bottomMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 20
    }

    Connections {
        target: readFileEngine
        function onProgressChanged(val) {
            progressBar.value = val
        }
        function onFinished() {
            btnOpenFile.enabled = true
            btnStartParsing.enabled = true
        }
    }
}


