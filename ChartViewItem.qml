import QtQuick 2.15
import QtCharts 2.5
import QtQuick.Controls 2.15
import QtQml 2.15

ChartView {
    id: chartView
    visible: true
    enabled: true
    title: qsTr("Word frequency")
    legend.alignment: Qt.AlignBottom
    antialiasing: true
    theme: ChartView.ChartThemeBlueCerulean

    Connections {
        target: readFileEngine
        function onBarDataChanged(data) {
            mySeries.clear()
            let max = 0
            for (const word in data) {
                var barset = mySeries.append(word, [ data[word] ]);
                barset.labelFont.pixelSize = 9
                max = max < data[word] ? data[word] : max
            }
            mySeries.max = Math.pow(10, max.toString().length-1) * (Number(max.toString()[0])+1)
        }
    }

    BarSeries {
        id: mySeries
        barWidth: 1
        labelsVisible: true
        labelsAngle: 90

        property int max: 100

        axisX: ValueAxis {
            min: -0.5
            max: 0.5
            tickCount: 10
            labelsVisible: false
        }

        axisY: ValueAxis {
            min: 0
            tickCount: 6
            labelsFont.pixelSize: 9
            Binding on max {
                value: mySeries.max
            }
        }
    }
}

