#include "ImageProcessing.h"
#pragma execution_character_set("utf-8")

ImageProcessing::ImageProcessing(QWidget *parent)
	: QMainWindow(parent)
{
	m_dockImgShow = NULL;
	m_dockImgList = NULL;
	m_dockGodView = NULL;
	m_dockLog = NULL;
	m_dockAreaInfo = NULL;
	m_dockImg3D = NULL;
	m_dockImg2D = NULL;

	m_ImgShowScrollArea = NULL;

	m_labelImgShow = NULL;
	m_listImgList = NULL;
	m_pImgShow = NULL;

	ui.setupUi(this);

	setWindowTitle("图像处理工具 V1.0");
	setWindowIcon(QIcon(":Icons/Image/icon/ImageProcessing.png"));

	CreateMenuToolbar();

	int width = 960;
	int height = 540;
	resize(QSize(width, height));
	centralWidget();

	CreateDockPanes();
	InitDockImgShow();
	InitDockImgList();
	InitDockImg3D();
	InitDockImg2D();

	connect(&m_MainProcess,SIGNAL(to_refreshFrame(const Mat)),this,SLOT(on_refreshFrame(const Mat)), Qt::BlockingQueuedConnection);
}

void ImageProcessing::CreateMenuToolbar()
{
	//文件菜单
	QMenu *fileMenu = menuBar()->addMenu(tr("文件"));
	QAction *openFile = new QAction(QIcon(":/MenuImages/Image/menu/FileOpen.png"), tr("打开文件(&O)"), this); 

	openFile->setShortcut(QKeySequence("Ctrl+O"));
	openFile->setStatusTip(tr("打开文件"));

	connect(openFile, SIGNAL(triggered()), this, SLOT(on_openFile()));

	fileMenu->addAction(openFile);

	//播放菜单
	QMenu *playMenu = menuBar()->addMenu(tr("播放"));
	QAction *firstFrame = new QAction(QIcon(":/MenuImages/Image/menu/First.png"), tr("首帧"), this);
	QAction *backwardFrame = new QAction(QIcon(":/MenuImages/Image/menu/Backward.png"), tr("后退"), this);
	QAction *playCtrl = new QAction(QIcon(":/MenuImages/Image/menu/Play.png"), tr("播放"), this);
	QAction *forwardFrame = new QAction(QIcon(":/MenuImages/Image/menu/Forward.png"), tr("前进"), this);
	QAction *lastFrame = new QAction(QIcon(":/MenuImages/Image/menu/Last.png"), tr("末帧"), this);

	playCtrl->setShortcut(QKeySequence(Qt::Key_Space));

	firstFrame->setStatusTip(tr("跳至首帧"));
	backwardFrame->setStatusTip(tr("后退一帧"));
	playCtrl->setStatusTip(tr("播放/停止"));
	forwardFrame->setStatusTip(tr("前进一帧"));
	lastFrame->setStatusTip(tr("跳至末帧"));

	connect(firstFrame, SIGNAL(triggered()), this, SLOT(on_firstFrame()));
	connect(backwardFrame, SIGNAL(triggered()), this, SLOT(on_backwardFrame()));
	connect(playCtrl, SIGNAL(triggered()), this, SLOT(on_playCtrl()));
	connect(forwardFrame, SIGNAL(triggered()), this, SLOT(on_forwardFrame()));
	connect(lastFrame, SIGNAL(triggered()), this, SLOT(on_lastFrame()));

	playMenu->addAction(firstFrame);
	playMenu->addAction(backwardFrame);
	playMenu->addAction(playCtrl);
	playMenu->addAction(forwardFrame);
	playMenu->addAction(lastFrame);

	//显示菜单
	QMenu *imgshowMenu = menuBar()->addMenu(tr("显示"));
	QAction *imgshowFullSpread = new QAction(QIcon(":/MenuImages/Image/menu/FullSpread.png"), tr("铺满窗口"), this);
	QAction *imgshowImgAutoSpread = new QAction(QIcon(":/MenuImages/Image/menu/ImgAutoSpread.png"), tr("适应窗口"), this);
	QAction *imgshowWndAutoSpread = new QAction(QIcon(":/MenuImages/Image/menu/WndAutoSpread.png"), tr("适应图像"), this);
	QAction *imgshowZoomIn = new QAction(QIcon(":/MenuImages/Image/menu/ZoomIn.png"), tr("放大"), this);
	QAction *imgshowZoomOut = new QAction(QIcon(":/MenuImages/Image/menu/ZoomOut.png"), tr("缩小"), this);
	QAction *imgshowNoScale = new QAction(QIcon(":/MenuImages/Image/menu/NoScale.png"), tr("1:1"), this);

	imgshowFullSpread->setCheckable(true);
	imgshowImgAutoSpread->setCheckable(true);
	imgshowWndAutoSpread->setCheckable(true);

	imgshowFullSpread->setStatusTip(tr("铺满窗口"));
	imgshowImgAutoSpread->setStatusTip(tr("图像适应窗口"));
	imgshowWndAutoSpread->setStatusTip(tr("窗口适应图像"));
	imgshowZoomIn->setStatusTip(tr("放大图像"));
	imgshowNoScale->setStatusTip(tr("原图大小"));
	imgshowZoomOut->setStatusTip(tr("缩小图像"));

	connect(imgshowFullSpread, SIGNAL(triggered()), this, SLOT(on_imgshowFullSpread()));
	connect(imgshowImgAutoSpread, SIGNAL(triggered()), this, SLOT(on_imgshowImgAutoSpread()));
	connect(imgshowWndAutoSpread, SIGNAL(triggered()), this, SLOT(on_imgshowWndAutoSpread()));
	connect(imgshowZoomIn, SIGNAL(triggered()), this, SLOT(on_imgshowZoomIn()));
	connect(imgshowNoScale, SIGNAL(triggered()), this, SLOT(on_imgshowNoScale()));
	connect(imgshowZoomOut, SIGNAL(triggered()), this, SLOT(on_imgshowZoomOut()));

	imgshowMenu->addAction(imgshowFullSpread);
	imgshowMenu->addAction(imgshowImgAutoSpread);
	imgshowMenu->addAction(imgshowWndAutoSpread);
	imgshowMenu->addAction(imgshowZoomIn);
	imgshowMenu->addAction(imgshowNoScale);
	imgshowMenu->addAction(imgshowZoomOut);

	//预处理菜单
	QMenu *preproMenu = menuBar()->addMenu(tr("预处理"));
	QAction *medianFiltering = new QAction(QIcon(":/MenuImages/Image/menu/MedianFiltering.png"), tr("中值滤波(&M)"), this);
	QAction *gaussianBlur = new QAction(QIcon(":/MenuImages/Image/menu/GaussianBlur.png"), tr("高斯模糊(&G)"), this);

	medianFiltering->setCheckable(true);
	gaussianBlur->setCheckable(true);

	connect(medianFiltering, SIGNAL(triggered()), this, SLOT(on_medianFiltering()));
	connect(gaussianBlur, SIGNAL(triggered()), this, SLOT(on_gaussianBlur()));

	preproMenu->addAction(medianFiltering);
	preproMenu->addAction(gaussianBlur);

	//增强菜单
	QMenu *enhanceMenu = menuBar()->addMenu(tr("图像增强"));
	QAction *linearTransformation = new QAction(QIcon(":/MenuImages/Image/menu/LinearTransformation.png"), tr("线性变换(&L)"), this);
	QAction *platformHistogram = new QAction(QIcon(":/MenuImages/Image/menu/PlatformHistogram.png"), tr("平台直方图(&P)"), this);

	linearTransformation->setCheckable(true);
	platformHistogram->setCheckable(true);

	connect(linearTransformation, SIGNAL(triggered()), this, SLOT(on_linearTransformation()));
	connect(platformHistogram, SIGNAL(triggered()), this, SLOT(on_platformHistogram()));

	enhanceMenu->addAction(linearTransformation);
	enhanceMenu->addAction(platformHistogram);


	//窗口自带的工具栏
	ui.mainToolBar->setWindowTitle(tr("主工具栏"));
	ui.mainToolBar->addAction(openFile);
	ui.mainToolBar->addSeparator();
	ui.mainToolBar->addAction(firstFrame);
	ui.mainToolBar->addAction(backwardFrame);
	ui.mainToolBar->addAction(playCtrl);
	ui.mainToolBar->addAction(forwardFrame);
	ui.mainToolBar->addAction(lastFrame);
	

	//图像处理工具栏
	QToolBar *imgproToolBar = new QToolBar();
	imgproToolBar->setWindowTitle(tr("图像处理工具栏"));
	imgproToolBar->setMovable(true); 
	addToolBar(imgproToolBar);   
	
	QToolButton *preproToolBtn = new QToolButton(this);
	preproToolBtn->setText(tr("预处理"));
	preproToolBtn->setIcon(QIcon(":/MenuImages/Image/menu/PreProcess.png"));
	preproToolBtn->setMenu(preproMenu);
	preproToolBtn->setPopupMode(QToolButton::MenuButtonPopup);
	imgproToolBar->addWidget(preproToolBtn);

	QToolButton *enhanceToolBtn = new QToolButton(this);
	enhanceToolBtn->setText(tr("图像增强"));
	enhanceToolBtn->setIcon(QIcon(":/MenuImages/Image/menu/Enhance.png"));
	enhanceToolBtn->setMenu(enhanceMenu);
	enhanceToolBtn->setPopupMode(QToolButton::MenuButtonPopup);
	imgproToolBar->addWidget(enhanceToolBtn);

	
	imgproToolBar->addSeparator();
	imgproToolBar->addAction(imgshowZoomOut);
	imgproToolBar->addAction(imgshowNoScale);
	imgproToolBar->addAction(imgshowZoomIn);

}
void ImageProcessing::on_openFile()
{
	QString full_path = QFileDialog::getOpenFileName(this, tr("选择文件"), ".", tr("Images(*.jpg *.png *.bmp)")); 
	QFileInfo file_info=QFileInfo(full_path);
	if (!full_path.isEmpty())
	{
		QString file_path = file_info.absolutePath();
		QString file_name = file_info.fileName();
		QString file_suffix = file_info.suffix();

		//图像序列
		if (file_suffix.compare("png",Qt::CaseInsensitive) == 0 ||
			file_suffix.compare("jpg", Qt::CaseInsensitive) == 0 ||
			file_suffix.compare("bmp", Qt::CaseInsensitive) == 0 ||
			file_suffix.compare("jpeg", Qt::CaseInsensitive) == 0)
		{
			TraversalDirs(file_path, file_suffix, m_MainProcess.m_ImgNameList);
			if (m_MainProcess.m_ImgNameList.size() > 0)
			{
				//更新列表
				m_listImgList->clearContents();
				m_listImgList->setRowCount(m_MainProcess.m_ImgNameList.size());
				for (int i = 0; i < m_MainProcess.m_ImgNameList.size(); i++)
				{
					QString str;
					str.sprintf("%d", i);
					m_listImgList->setItem(i, 0, new QTableWidgetItem(str));
				}
				m_pImgShow->load(m_MainProcess.m_ImgNameList[0]);
				m_MainProcess.m_SourceType = IMG_SOURCE_IMGS;
				DrawFrame();
			}
		}
		
	}
}

void ImageProcessing::on_firstFrame()
{
	QMessageBox::information(this, tr("提示"), tr("First"), QMessageBox::Ok);
}
void ImageProcessing::on_backwardFrame()
{
	QMessageBox::information(this, tr("提示"), tr("Back"), QMessageBox::Ok);
}
void ImageProcessing::on_playCtrl()
{
	QAction* actioon = static_cast<QAction*>(sender());
	if (m_MainProcess.isRunning())
	{
		m_MainProcess.stopThread();
		actioon->setIcon(QIcon(":/MenuImages/Image/menu/Play.png"));
	}
	else
	{
		m_MainProcess.startThread();
		actioon->setIcon(QIcon(":/MenuImages/Image/menu/Stop.png"));
	}
}
void ImageProcessing::on_forwardFrame()
{
	QMessageBox::information(this, tr("提示"), tr("Forward"), QMessageBox::Ok);
}
void ImageProcessing::on_lastFrame()
{
	QMessageBox::information(this, tr("提示"), tr("Last"), QMessageBox::Ok);
}

void ImageProcessing::on_medianFiltering()
{
	QMessageBox::information(this, tr("提示"), tr("Median"), QMessageBox::Ok);
}
void ImageProcessing::on_gaussianBlur()
{
	QMessageBox::information(this, tr("提示"), tr("OpenFile"), QMessageBox::Ok);
}

void ImageProcessing::on_imgshowFullSpread()
{
	QImage Img = m_labelImgShow->pixmap()->toImage().scaled(m_ImgShowScrollArea->width() - 2, m_ImgShowScrollArea->height() - 2, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	m_pImgShow = new QImage(Img);
	DrawFrame();
}
void ImageProcessing::on_imgshowImgAutoSpread()
{
	QImage Img;
	double ImgRatio = 1.0 * m_labelImgShow->pixmap()->toImage().width() / m_labelImgShow->pixmap()->toImage().height();
	double WinRatio = 1.0 * (m_ImgShowScrollArea->width() - 2) / (m_ImgShowScrollArea->height() - 2); 
	if (ImgRatio > WinRatio)       
	{
		Img = m_labelImgShow->pixmap()->toImage().scaled((m_ImgShowScrollArea->width() - 2), (m_ImgShowScrollArea->width() - 2) / ImgRatio, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	}
	else                           
	{
		Img = m_labelImgShow->pixmap()->toImage().scaled((m_ImgShowScrollArea->height() - 2) * ImgRatio, (m_ImgShowScrollArea->height() - 2), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	}
	m_pImgShow = new QImage(Img);
	DrawFrame();
}
void ImageProcessing::on_imgshowWndAutoSpread()
{
	TIP("WndAuto");
}
void ImageProcessing::on_imgshowZoomOut()
{
	TIP("ZoomOut");
}
void ImageProcessing::on_imgshowZoomIn()
{
	TIP("ZoomIn");
}
void ImageProcessing::on_imgshowNoScale()
{
	TIP("NoScale");
}


void ImageProcessing::on_linearTransformation()
{
	TIP("Linear");
}
void ImageProcessing::on_platformHistogram()
{
	TIP("Plat");
}
void ImageProcessing::CreateDockPanes()
{
	QWidget* p = takeCentralWidget();   //删除中央窗体
	if (p) 	delete p;
	setDockNestingEnabled(true);        //允许嵌套dock

	m_dockImgShow = new QDockWidget(tr("图像"), this);
	m_dockImgList = new QDockWidget(tr("列表"), this);
	m_dockGodView = new QDockWidget(tr("地图"), this);
	m_dockLog = new QDockWidget(tr("日志"),this);
	m_dockAreaInfo = new QDockWidget(tr("区域"), this);
	m_dockImg3D = new QDockWidget(tr("3D"), this);
	m_dockImg2D = new QDockWidget(tr("2D"), this);

	m_dockImgShow->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetClosable);
	m_dockImgList->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetClosable);
	m_dockGodView->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetClosable);
	m_dockLog->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetClosable);
	m_dockAreaInfo->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetClosable);
	m_dockImg3D->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetClosable);
	m_dockImg2D->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetClosable);


	//m_dockGodView->setFixedWidth(400);
	//m_dockLog->setFixedWidth(400);
	//m_dockAreaInfo->setFixedWidth(400);
	//m_dockImg2D->setFixedWidth(400);
	//m_dockImg3D->setFixedWidth(400);
	//m_dockImgList->setFixedWidth(200);

	m_dockImgShow->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	m_dockImgList->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	m_dockGodView->setAllowedAreas(Qt::AllDockWidgetAreas);
	m_dockAreaInfo->setAllowedAreas(Qt::AllDockWidgetAreas);
	m_dockImg3D->setAllowedAreas(Qt::AllDockWidgetAreas);
	m_dockImg2D->setAllowedAreas(Qt::AllDockWidgetAreas);

	setCentralWidget(m_dockImgShow);
	addDockWidget(Qt::LeftDockWidgetArea, m_dockImgList);
	addDockWidget(Qt::RightDockWidgetArea, m_dockGodView);
	addDockWidget(Qt::RightDockWidgetArea, m_dockLog);
	addDockWidget(Qt::RightDockWidgetArea, m_dockAreaInfo);
	addDockWidget(Qt::RightDockWidgetArea, m_dockImg3D);
	addDockWidget(Qt::RightDockWidgetArea, m_dockImg2D);

	splitDockWidget(m_dockImgList, m_dockImgShow, Qt::Horizontal);  
	splitDockWidget(m_dockGodView, m_dockAreaInfo, Qt::Vertical);    

	tabifyDockWidget(m_dockAreaInfo, m_dockImg3D);
	tabifyDockWidget(m_dockImg3D, m_dockImg2D);
	tabifyDockWidget(m_dockGodView, m_dockLog);

	m_dockAreaInfo->raise();
	m_dockGodView->raise();

}
void ImageProcessing::InitDockImgShow()        //初始化图像显示窗口
{
	m_labelImgShow = new QLabel(m_dockImgShow);
	m_labelImgShow->setScaledContents(true);  
	m_labelImgShow->installEventFilter(this);

	int width = 500;
	int height = 500;

	Mat temp(width, height, CV_8UC3);
	temp(Rect(0, 0,248, 248)).setTo(Scalar(8,163,238));
	temp(Rect(0, 252, 248, 248)).setTo(Scalar(241, 80, 31));
	temp(Rect(252, 0, 248, 248)).setTo(Scalar(255, 184, 2));
	temp(Rect(252,252, 248, 248)).setTo(Scalar(126, 185, 2));
	cvtColor(temp, temp, COLOR_RGB2BGR);

	QImage qimg=Mat2QImage(temp);

	m_pImgShow = new QImage(qimg);
//	m_pImgShow->fill(qRgb(0, 0, 0));
	DrawFrame();

	m_ImgShowScrollArea = new QScrollArea(this);
	m_ImgShowScrollArea->setBackgroundRole(QPalette::Dark);
	m_ImgShowScrollArea->setAlignment(Qt::AlignCenter);
	m_ImgShowScrollArea->setWidget(m_labelImgShow);
	m_dockImgShow->setWidget(m_ImgShowScrollArea);
}

void ImageProcessing::InitDockImgList()    //初始化图像列表窗口
{
	m_listImgList = new QTableWidget(m_dockImgList);
	m_listImgList->setColumnCount(1);
	m_listImgList->verticalHeader()->setVisible(false);
	m_listImgList->installEventFilter(this);

	m_listImgList->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_listImgList->setSelectionMode(QAbstractItemView::SingleSelection);
	m_listImgList->setEditTriggers(QAbstractItemView::NoEditTriggers);

	QTableWidgetItem *item0 = new QTableWidgetItem(tr("#"));
	item0->setTextAlignment(Qt::AlignLeft);
	m_listImgList->setHorizontalHeaderItem(0, item0);

	m_listImgList->setColumnWidth(0,400);

	connect(m_listImgList, SIGNAL(cellClicked(int,int)), this, SLOT(on_imglistLClicked(int,int)));

	m_dockImgList->setWidget(m_listImgList);
}

void ImageProcessing::InitDockImg3D()
{
	Q3DSurface *graph = new Q3DSurface();
	QWidget *container = QWidget::createWindowContainer(graph);

	if (!graph->hasContext()) {
		QMessageBox msgBox;
		msgBox.setText("Couldn't initialize the OpenGL context.");
		msgBox.exec();
		return;
	}

	QSize screenSize = graph->screen()->size();
//	container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.6));
//	container->setMaximumSize(screenSize);
	container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	container->setFocusPolicy(Qt::StrongFocus);

	QWidget *widget = new QWidget;
	QHBoxLayout *hLayout = new QHBoxLayout(widget);
	QVBoxLayout *vLayout = new QVBoxLayout();
	hLayout->addWidget(container, 1);
	hLayout->addLayout(vLayout);
	vLayout->setAlignment(Qt::AlignTop);

	widget->setWindowTitle(QStringLiteral("Surface"));

	QGroupBox *selectionGroupBox = new QGroupBox(QStringLiteral("Selection Mode"));

	QRadioButton *modeNoneRB = new QRadioButton(widget);
	modeNoneRB->setText(QStringLiteral("No selection"));
	modeNoneRB->setChecked(false);

	QRadioButton *modeItemRB = new QRadioButton(widget);
	modeItemRB->setText(QStringLiteral("Item"));
	modeItemRB->setChecked(false);

	QRadioButton *modeSliceRowRB = new QRadioButton(widget);
	modeSliceRowRB->setText(QStringLiteral("Row Slice"));
	modeSliceRowRB->setChecked(false);

	QRadioButton *modeSliceColumnRB = new QRadioButton(widget);
	modeSliceColumnRB->setText(QStringLiteral("Column Slice"));
	modeSliceColumnRB->setChecked(false);

	QVBoxLayout *selectionVBox = new QVBoxLayout;
	selectionVBox->addWidget(modeNoneRB);
	selectionVBox->addWidget(modeItemRB);
	selectionVBox->addWidget(modeSliceRowRB);
	selectionVBox->addWidget(modeSliceColumnRB);
	selectionGroupBox->setLayout(selectionVBox);

	QComboBox *themeList = new QComboBox(widget);
	themeList->addItem(QStringLiteral("Qt"));
	themeList->addItem(QStringLiteral("Primary Colors"));
	themeList->addItem(QStringLiteral("Digia"));
	themeList->addItem(QStringLiteral("Stone Moss"));
	themeList->addItem(QStringLiteral("Army Blue"));
	themeList->addItem(QStringLiteral("Retro"));
	themeList->addItem(QStringLiteral("Ebony"));
	themeList->addItem(QStringLiteral("Isabelle"));

	QGroupBox *colorGroupBox = new QGroupBox(QStringLiteral("Custom gradient"));

	QLinearGradient grBtoY(0, 0, 1, 100);
	grBtoY.setColorAt(1.0, Qt::black);
	grBtoY.setColorAt(0.67, Qt::blue);
	grBtoY.setColorAt(0.33, Qt::red);
	grBtoY.setColorAt(0.0, Qt::yellow);
	QPixmap pm(24, 100);
	QPainter pmp(&pm);
	pmp.setBrush(QBrush(grBtoY));
	pmp.setPen(Qt::NoPen);
	pmp.drawRect(0, 0, 24, 100);
	QPushButton *gradientBtoYPB = new QPushButton(widget);
	gradientBtoYPB->setIcon(QIcon(pm));
	gradientBtoYPB->setIconSize(QSize(24, 100));

	QLinearGradient grGtoR(0, 0, 1, 100);
	grGtoR.setColorAt(1.0, Qt::darkGreen);
	grGtoR.setColorAt(0.5, Qt::yellow);
	grGtoR.setColorAt(0.2, Qt::red);
	grGtoR.setColorAt(0.0, Qt::darkRed);
	pmp.setBrush(QBrush(grGtoR));
	pmp.drawRect(0, 0, 24, 100);
	QPushButton *gradientGtoRPB = new QPushButton(widget);
	gradientGtoRPB->setIcon(QIcon(pm));
	gradientGtoRPB->setIconSize(QSize(24, 100));

	QHBoxLayout *colorHBox = new QHBoxLayout;
	colorHBox->addWidget(gradientBtoYPB);
	colorHBox->addWidget(gradientGtoRPB);
	colorGroupBox->setLayout(colorHBox);

	vLayout->addWidget(selectionGroupBox);

	vLayout->addWidget(new QLabel(QStringLiteral("Theme")));
	vLayout->addWidget(themeList);
	vLayout->addWidget(colorGroupBox);

//	widget->show();
	m_dockImg3D->setWidget(widget);

	SurfaceGraph* modifier= new SurfaceGraph(graph);

	connect(&m_MainProcess, SIGNAL(to_showImage3D(const Mat)),modifier,SLOT(on_showImage3D(const Mat)),Qt::BlockingQueuedConnection);

	QObject::connect(modeNoneRB, &QRadioButton::toggled,
		modifier, &SurfaceGraph::toggleModeNone);
	QObject::connect(modeItemRB, &QRadioButton::toggled,
		modifier, &SurfaceGraph::toggleModeItem);
	QObject::connect(modeSliceRowRB, &QRadioButton::toggled,
		modifier, &SurfaceGraph::toggleModeSliceRow);
	QObject::connect(modeSliceColumnRB, &QRadioButton::toggled,
		modifier, &SurfaceGraph::toggleModeSliceColumn);
	QObject::connect(themeList, SIGNAL(currentIndexChanged(int)),
		modifier, SLOT(changeTheme(int)));
	QObject::connect(gradientBtoYPB, &QPushButton::pressed,
		modifier, &SurfaceGraph::setBlackToYellowGradient);
	QObject::connect(gradientGtoRPB, &QPushButton::pressed,
		modifier, &SurfaceGraph::setGreenToRedGradient);

	modeItemRB->setChecked(true);
	themeList->setCurrentIndex(2);
}

void ImageProcessing::InitDockImg2D()
{
	QChart* chart = new QChart();
	QChartView *chartView = new QChartView(chart);
	chartView->setRenderHint(QPainter::Antialiasing);

	QWidget *widget = new QWidget;
	QHBoxLayout *hLayout = new QHBoxLayout(widget);
	QVBoxLayout *vLayout = new QVBoxLayout();
	hLayout->addWidget(chartView, 1);
	hLayout->addLayout(vLayout);
	vLayout->setAlignment(Qt::AlignTop);

	QComboBox *themeList = new QComboBox(widget);
	themeList->addItem(QStringLiteral("Light"));
	themeList->addItem(QStringLiteral("BlueCerulean"));
	themeList->addItem(QStringLiteral("Dark"));
	themeList->addItem(QStringLiteral("BrownSand"));
	themeList->addItem(QStringLiteral("BlueNcs"));
	themeList->addItem(QStringLiteral("HighContrast"));
	themeList->addItem(QStringLiteral("BlueIcy"));
	themeList->addItem(QStringLiteral("Qt"));

	vLayout->addWidget(new QLabel(QStringLiteral("Theme")));
	vLayout->addWidget(themeList);

	m_dockImg2D->setWidget(widget);
	CustomChart* modifier = new CustomChart(chart);

	connect(&m_MainProcess, SIGNAL(to_showImage2D(const Mat)), modifier, SLOT(on_showImage2D(const Mat)), Qt::BlockingQueuedConnection);
	QObject::connect(themeList, SIGNAL(currentIndexChanged(int)),modifier, SLOT(changeTheme(int)));

	themeList->setCurrentIndex(0);
}


void ImageProcessing::DrawFrame()
{
	if (!m_pImgShow)
	{
		return;
	}

	m_labelImgShow->setPixmap(QPixmap::fromImage(*m_pImgShow));
	m_labelImgShow->resize(m_pImgShow->width(), m_pImgShow->height());
}

void ImageProcessing::on_imglistLClicked(int row,int col)
{
	m_pImgShow->load(m_MainProcess.m_ImgNameList[row]);
	DrawFrame();
}

bool ImageProcessing::eventFilter(QObject* obj, QEvent* event)
{
	if (obj == m_listImgList)
	{
		if (event->type() == QEvent::KeyPress)
		{
			QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
			if (keyEvent->key() == Qt::Key_Down)
			{
				//TIP("D");
			}
		}
		if (event->type() == QEvent::KeyRelease)
		{
			QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
			if (keyEvent->key() == Qt::Key_Down || keyEvent->key() == Qt::Key_Up)
			{
				QList<QTableWidgetItem*>items = m_listImgList->selectedItems();
				int count = items.count();
				if (count > 0)
				{
					int row = m_listImgList->row(items.at(0));
					m_pImgShow->load(m_MainProcess.m_ImgNameList[row]);
					DrawFrame();
				}
			}

		}
	}

	if (obj == m_labelImgShow)
	{
		switch (event->type())
		{
		case QEvent::MouseMove:
		{
			QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
			qDebug("%d %d", mouseEvent->pos().x(), mouseEvent->pos().y());
			break;
		}
		case QEvent::MouseButtonPress:
		{
			QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
			qDebug("%d %d", mouseEvent->pos().x(), mouseEvent->pos().y());
			break;
		}
		default:
			break;
		}
	}

	return QWidget::eventFilter(obj,event);
}


void ImageProcessing::on_refreshFrame(const Mat frame)
{
	m_pImgShow=new QImage(Mat2QImage(frame));
	DrawFrame();
}

