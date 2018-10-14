#pragma once
#include <QThread>
#include <QMetaType>
#include "ProcessDataDefine.h"
#include "AssistFunction.h"

class MainProcess:public QThread
{
	Q_OBJECT
public:
	explicit MainProcess(QObject* parent = 0);
	void CaptureFrame();
	void ProcessFrame();
public slots:
	void startThread();
	void stopThread();

private:
	void run();
	volatile bool stopped;

signals:
	void to_refreshFrame(const Mat);
	void to_showImage3D(const Mat);
	void to_showImage2D(const Mat);

public:
	QStringList m_ImgNameList;
	IMG_SOURCE_TYPE m_SourceType;
	Mat m_CurrentFrame;
	uint m_iFrameIndex;
};

