#pragma once

#include <QtGui>
#include <QtWidgets>
#include <string>
#include <QMainWindow>
#include <QMessageBox>

#include "ui_ImageProcessing.h"
#include "AssistFunction.h"
#include "OpencvConfig.h"
#include "MainProcess.h"
#include "surfacegraph.h"
#include "customchart.h"

class ImageProcessing : public QMainWindow
{
	Q_OBJECT

public:
	ImageProcessing(QWidget *parent = Q_NULLPTR);
	void CreateMenuToolbar();
	void CreateDockPanes();
	void InitDockImgShow();
	void InitDockImgList();
	void InitDockImg3D();
	void InitDockImg2D();
	void DrawFrame();
public:
	QDockWidget* m_dockImgShow;
	QDockWidget* m_dockImgList;
	QDockWidget* m_dockGodView;
	QDockWidget* m_dockLog;
	QDockWidget* m_dockAreaInfo;
	QDockWidget* m_dockImg3D;
	QDockWidget* m_dockImg2D;

	QLabel* m_labelImgShow;
	QTableWidget * m_listImgList;
	QImage* m_pImgShow;

	QScrollArea* m_ImgShowScrollArea;
public:
	MainProcess m_MainProcess;
private slots:
	void on_refreshFrame(const Mat);

protected:
	bool eventFilter(QObject* obj,QEvent* event);
private:
	Ui::ImageProcessingClass ui;
private slots:
	void on_openFile();

	void on_imglistLClicked(int row, int col);

	void on_firstFrame();
	void on_backwardFrame();
	void on_playCtrl();
	void on_forwardFrame();
	void on_lastFrame();

	void on_imgshowFullSpread();
	void on_imgshowImgAutoSpread();
	void on_imgshowWndAutoSpread();
	void on_imgshowZoomOut();
	void on_imgshowZoomIn();
	void on_imgshowNoScale();

	void on_medianFiltering();
	void on_gaussianBlur();

	void on_linearTransformation();
	void on_platformHistogram();
};
