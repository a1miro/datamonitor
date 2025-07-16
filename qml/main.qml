import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import DataMonitor 1.0

ApplicationWindow {
    id: window
    width: Screen.width / 2
    height: Screen.height / 2
    x: (Screen.width - width) / 2
    y: (Screen.height - height) / 2
    visible: true
    title: qsTr("Real-Time Data Monitor")

    DataGenerator {
        id: dataGenerator
        frequency: frequencySlider.value
        amplitude: amplitudeSlider.value
        bufferSize: 5000
        running: startStopButton.checked

        onDataChanged: {
            plotView.updateData()
            plotView1.updateData()
            plotView2.updateData()
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10

        // Control Panel
        GroupBox {
            title: "Controls"
            Layout.fillWidth: true
            Layout.preferredHeight: 120

            GridLayout {
                anchors.fill: parent
                columns: 6
                columnSpacing: 20

                // Start/Stop Button
                Button {
                    id: startStopButton
                    text: checked ? "Stop" : "Start"
                    checkable: true
                    checked: true // Start with data generation enabled
                    Layout.preferredWidth: 80

                    background: Rectangle {
                        color: startStopButton.checked ? "#e74c3c" : "#27ae60"
                        radius: 5
                        border.color: "#2c3e50"
                        border.width: 1
                    }
                }

                // Frequency Control
                Label {
                    text: "Frequency:"
                    Layout.alignment: Qt.AlignRight
                }

                Slider {
                    id: frequencySlider
                    from: 0.1
                    to: 10.0
                    value: 1.0
                    stepSize: 0.1
                    Layout.preferredWidth: 150
                }

                Label {
                    text: frequencySlider.value.toFixed(1) + " Hz"
                    Layout.preferredWidth: 50
                }

                // Amplitude Control
                Label {
                    text: "Amplitude:"
                    Layout.alignment: Qt.AlignRight
                }

                Slider {
                    id: amplitudeSlider
                    from: 0.1
                    to: 5.0
                    value: 1.0
                    stepSize: 0.1
                    Layout.preferredWidth: 150
                }

                Label {
                    text: amplitudeSlider.value.toFixed(1)
                    Layout.preferredWidth: 50
                }

                // Time Window Control
                Label {
                    text: "Time Window:"
                    Layout.alignment: Qt.AlignRight
                }

                Slider {
                    id: timeWindowSlider
                    from: 1.0
                    to: 10.0
                    value: 5.0
                    stepSize: 0.5
                    Layout.preferredWidth: 150

                    onValueChanged: {
                        plotView.timeWindow = value
                        plotView1.timeWindow = value
                        plotView2.timeWindow = value
                    }
                }

                Label {
                    text: timeWindowSlider.value.toFixed(1) + " s"
                    Layout.preferredWidth: 50
                }
            }
        }

        // Status Bar
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            color: "#ecf0f1"
            border.color: "#bdc3c7"
            border.width: 1

            RowLayout {
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                spacing: 20

                Text {
                    text: "Status: " + (dataGenerator.running ? "Running" : "Stopped")
                    color: dataGenerator.running ? "#27ae60" : "#e74c3c"
                    font.bold: true
                }

                Text {
                    text: "Data Points: " + plotView.dataPointCount
                    color: "#2c3e50"
                }

                Text {
                    text: "Render FPS: 60"
                    color: "#2c3e50"
                }
            }

            // Clear Data Button
            Button {
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                text: "Clear Data"

                onClicked: {
                    dataGenerator.clearData()
                }
            }
        }

        // Plot View
        PlotView {
            id: plotView
            Layout.fillWidth: true
            Layout.fillHeight: true
            dataGenerator: dataGenerator
            timeWindow: timeWindowSlider.value
        }

        PlotView {
            id: plotView1
            Layout.fillWidth: true
            Layout.fillHeight: true
            dataGenerator: dataGenerator
            timeWindow: timeWindowSlider.value
        }

        PlotView {
            id: plotView2
            Layout.fillWidth: true
            Layout.fillHeight: true
            dataGenerator: dataGenerator
            timeWindow: timeWindowSlider.value
        }
    }
}
