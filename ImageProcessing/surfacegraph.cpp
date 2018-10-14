/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Data Visualization module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "surfacegraph.h"

#include <QtDataVisualization/QValue3DAxis>
#include <QtDataVisualization/Q3DTheme>
#include <QtGui/QImage>
#include <QtCore/qmath.h>

using namespace QtDataVisualization;

SurfaceGraph::SurfaceGraph(Q3DSurface *surface)
    : m_graph(surface)
{
    m_graph->setAxisX(new QValue3DAxis);
    m_graph->setAxisY(new QValue3DAxis);
    m_graph->setAxisZ(new QValue3DAxis);

    m_sqrtSinProxy = new QSurfaceDataProxy();
    m_sqrtSinSeries = new QSurface3DSeries(m_sqrtSinProxy);

	m_sqrtSinSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
	m_sqrtSinSeries->setFlatShadingEnabled(true);

	m_graph->axisX()->setLabelFormat("%.0f");
	m_graph->axisY()->setLabelFormat("%.0f");
	m_graph->axisZ()->setLabelFormat("%.0f");
	m_graph->axisX()->setRange(0, 100);
	m_graph->axisY()->setRange(0, 255);
	m_graph->axisZ()->setRange(0, 100);
	m_graph->axisX()->setLabelAutoRotation(30);
	m_graph->axisY()->setLabelAutoRotation(90);
	m_graph->axisZ()->setLabelAutoRotation(30);

	m_graph->addSeries(m_sqrtSinSeries);
}

SurfaceGraph::~SurfaceGraph()
{
    delete m_graph;
}

void SurfaceGraph::fillMatProxy(const Mat img)
{
	int top = 0;
	int left = 0;
	int width = 100;
	int height = 100;
	QSurfaceDataArray *dataArray = new QSurfaceDataArray;
	dataArray->reserve(height);

	Mat temp = img(Rect(top, left, width, height)).clone();
	if (temp.channels() == 3)
	{
		cvtColor(temp, temp, COLOR_RGB2GRAY);
	}

	for (int i = 0; i < height; i++) 
	{
		Mat& row = temp.row(i);
		uchar* data = row.ptr();
		QSurfaceDataRow *newRow = new QSurfaceDataRow(width);
		// Keep values within range bounds, since just adding step can cause minor drift due
		// to the rounding errors.
		for (int  j = 0; j < width; j++)
		{
			(*newRow)[j].setPosition(QVector3D(j, data[j], i));
		}
		
		*dataArray << newRow;
	}
	m_sqrtSinProxy->resetArray(dataArray);
}

void SurfaceGraph::on_showImage3D(const Mat frame)
{
	fillMatProxy(frame);
}

void SurfaceGraph::changeTheme(int theme)
{
    m_graph->activeTheme()->setType(Q3DTheme::Theme(theme));
}

void SurfaceGraph::setBlackToYellowGradient()
{
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::black);
    gr.setColorAt(0.33, Qt::blue);
    gr.setColorAt(0.67, Qt::red);
    gr.setColorAt(1.0, Qt::yellow);

    m_graph->seriesList().at(0)->setBaseGradient(gr);
    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}

void SurfaceGraph::setGreenToRedGradient()
{
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::darkGreen);
    gr.setColorAt(0.5, Qt::yellow);
    gr.setColorAt(0.8, Qt::red);
    gr.setColorAt(1.0, Qt::darkRed);

    m_graph->seriesList().at(0)->setBaseGradient(gr);
    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}

