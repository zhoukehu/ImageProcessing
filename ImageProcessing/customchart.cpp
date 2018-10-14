#include "customchart.h"
#include <QtCore/QDebug>

CustomChart::CustomChart(QChart *ch)
    : chart(ch)
{
	chart->legend()->setVisible(true);
	chart->legend()->setAlignment(Qt::AlignRight);

	QValueAxis* axisX = new QValueAxis();
	QValueAxis* axisY = new QValueAxis();

	axisX->setRange(0, 100);
	axisY->setRange(0,255);
	axisX->setLabelFormat("%.0f");
	axisY->setLabelFormat("%.0f");

	chart->setAxisX(axisX);
	chart->setAxisY(axisY);

	chart->setTheme(QChart::ChartThemeLight);
}

CustomChart::~CustomChart()
{
    delete chart;
}

void CustomChart::on_showImage2D(const Mat img)
{
	chart->removeAllSeries();
	seriesCol = new QLineSeries();
	seriesCol->setName(QString("X"));
	seriesRow = new QLineSeries();
	seriesRow->setName(QString("Y"));

	int top = 0;
	int left = 0;
	int width = 100;
	int height = 100;

	Mat temp = img(Rect(top, left, width, height)).clone();
	if (temp.channels() == 3)
	{
		cvtColor(temp, temp, COLOR_RGB2GRAY);
	}

	Mat centerCol = temp.col(50);
	Mat centerRow = temp.row(50);

	uchar* data = centerCol.ptr();
	for (int i = 0; i < centerCol.rows; i++)
	{
		seriesCol->append(i, data[i]);
	}

	data = centerRow.ptr();
	for (int i = 0; i < centerRow.cols; i++)
	{
		seriesRow->append(i, data[i]);
	}

	chart->addSeries(seriesCol);
	chart->addSeries(seriesRow);

	const auto markers = chart->legend()->markers();
	for (QLegendMarker *marker : markers) {
		// Disconnect possible existing connection to avoid multiple connections
		QObject::disconnect(marker, &QLegendMarker::clicked, this, &CustomChart::handleMarkerClicked);
		QObject::connect(marker, &QLegendMarker::clicked, this, &CustomChart::handleMarkerClicked);
	}
}

void CustomChart::handleMarkerClicked()
{
	qDebug() << "Unknown marker type";
	QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender());
	Q_ASSERT(marker);

	switch (marker->type())
	{
	case QLegendMarker::LegendMarkerTypeXY:
	{
		// Toggle visibility of series
		marker->series()->setVisible(!marker->series()->isVisible());

		// Turn legend marker back to visible, since hiding series also hides the marker
		// and we don't want it to happen now.
		marker->setVisible(true);

		// Dim the marker, if series is not visible
		qreal alpha = 1.0;

		if (!marker->series()->isVisible())
			alpha = 0.5;

		QColor color;
		QBrush brush = marker->labelBrush();
		color = brush.color();
		color.setAlphaF(alpha);
		brush.setColor(color);
		marker->setLabelBrush(brush);

		brush = marker->brush();
		color = brush.color();
		color.setAlphaF(alpha);
		brush.setColor(color);
		marker->setBrush(brush);

		QPen pen = marker->pen();
		color = pen.color();
		color.setAlphaF(alpha);
		pen.setColor(color);
		marker->setPen(pen);

		break;
	}
	default:
	{
		qDebug() << "Unknown marker type";
		break;
	}
	}
}

void CustomChart::changeTheme(int theme)
{
	chart->setTheme(QChart::ChartTheme(theme));
}

