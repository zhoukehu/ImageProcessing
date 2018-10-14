#ifndef CUSTOMCHART_H
#define CUSTOMCHART_H

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QLegendMarker>
#include <QtCharts/QLineSeries>
#include <QtCharts/QXYLegendMarker>
#include <QtCharts/QValueAxis>

#include "AssistFunction.h"

using namespace QT_CHARTS_NAMESPACE;

class CustomChart : public QObject
{
    Q_OBJECT
public:
    explicit CustomChart(QChart *chart);
    ~CustomChart();

public Q_SLOTS:
	void on_showImage2D(const Mat image);
	void handleMarkerClicked();
	void changeTheme(int theme);

private:
    QChart *chart;
	QLineSeries *seriesCol;
	QLineSeries *seriesRow;
};

#endif // SURFACEGRAPH_H
