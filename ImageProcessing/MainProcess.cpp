
#include"MainProcess.h"
#include <QDebug>

MainProcess::MainProcess(QObject* parent) :QThread(parent)
{
	stopped = false;
	m_iFrameIndex = 0;
	qRegisterMetaType<Mat >("Mat");
}

void MainProcess::startThread()
{
	start();
	stopped = false;
}
void MainProcess::stopThread()
{
	quit();
	stopped = true;
}

void MainProcess::run()
{
	while (!stopped)
	{
		qDebug() << "Running..." ;
		msleep(20);
		CaptureFrame();
		ProcessFrame();
	}
}

void MainProcess::CaptureFrame()
{
	switch (m_SourceType)
	{
	case IMG_SOURCE_IMGS:
		m_CurrentFrame = imread(m_ImgNameList[m_iFrameIndex].toStdString(),-1);

		m_iFrameIndex++;
		if (m_iFrameIndex == m_ImgNameList.size())
		{
			m_iFrameIndex = 0;
		}
		break;
	case IMG_SOURCE_VIDEO:
		break;
	case IMG_SOURCE_RTMP:
		break;
	default:
		break;
	}
}
void MainProcess::ProcessFrame()
{
	emit to_refreshFrame(m_CurrentFrame);
	emit to_showImage3D(m_CurrentFrame);
	emit to_showImage2D(m_CurrentFrame);
}

