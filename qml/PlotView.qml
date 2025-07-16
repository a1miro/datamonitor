import QtQuick 2.15
import QtQuick.Controls 2.15
import DataMonitor 1.0

Rectangle {
    id: root
    color: "#ffffff"
    border.color: "#bdc3c7"
    border.width: 2

    property var dataGenerator: null
    property real timeWindow: 5.0
    property int dataPointCount: 0

    function updateData() {
        if (dataGenerator) {
            var newData = dataGenerator.getData()
            plotRenderer.data = newData
            dataPointCount = newData.length
        }
    }

    // Background grid
    Canvas {
        id: gridCanvas
        anchors.fill: parent
        anchors.margins: 40

        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)

            ctx.strokeStyle = "#ecf0f1"
            ctx.lineWidth = 1

            // Vertical grid lines (time)
            var timeSteps = 10
            for (var i = 0; i <= timeSteps; i++) {
                var x = (i / timeSteps) * width
                ctx.beginPath()
                ctx.moveTo(x, 0)
                ctx.lineTo(x, height)
                ctx.stroke()
            }

            // Horizontal grid lines (amplitude)
            var amplitudeSteps = 10
            for (var j = 0; j <= amplitudeSteps; j++) {
                var y = (j / amplitudeSteps) * height
                ctx.beginPath()
                ctx.moveTo(0, y)
                ctx.lineTo(width, y)
                ctx.stroke()
            }
        }

        onWidthChanged: requestPaint()
        onHeightChanged: requestPaint()
    }

    // Axis labels
    Column {
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.verticalCenter: parent.verticalCenter
        spacing: 30 // Fixed spacing instead of dynamic

        Repeater {
            model: 6
            Text {
                // Fix: dataBounds.y is the minimum, dataBounds.y + dataBounds.height is the maximum
                text: (plotRenderer.dataBounds.y +
                       (plotRenderer.dataBounds.height * (5 - index) / 5)).toFixed(1)
                font.pixelSize: 10
                color: "#7f8c8d"
            }
        }
    }

    Row {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 50 // Fixed spacing instead of dynamic

        Repeater {
            model: 6
            Text {
                text: (plotRenderer.dataBounds.left +
                       (plotRenderer.dataBounds.width * index / 5)).toFixed(1) + "s"
                font.pixelSize: 10
                color: "#7f8c8d"
            }
        }
    }

    // Main plot renderer
    PlotRenderer {
        id: plotRenderer
        anchors.fill: parent
        anchors.margins: 40
        antialiasing: true

        lineColor: "#e74c3c"
        lineWidth: 2.0
        autoScale: true
        timeWindow: root.timeWindow

        // Update data bounds when timeWindow changes
        onTimeWindowChanged: {
            // Trigger auto-scaling recalculation
            if (autoScale && dataGenerator) {
                updateData()
            }
        }
    }

    // Title
    Text {
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        text: qsTr("Real-Time Sine Wave Data")
        font.pixelSize: 16
        font.bold: true
        color: "#2c3e50"
    }

    // Y-axis label
    Text {
        anchors.left: parent.left
        anchors.leftMargin: 15
        anchors.verticalCenter: parent.verticalCenter
        text: "Amplitude"
        font.pixelSize: 12
        color: "#7f8c8d"
        rotation: -90
        transformOrigin: Item.Center
    }

    // X-axis label
    Text {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 15
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Time (seconds)"
        font.pixelSize: 12
        color: "#7f8c8d"
    }
}
