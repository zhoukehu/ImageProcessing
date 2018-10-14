#pragma once
#include <QDir>
#include <string>
#include <QImage>



#include "OpencvConfig.h"

#define TIP(str) QMessageBox::information(this,tr("��ʾ"),str,QMessageBox::Ok)


//�����ļ���
void TraversalDirs(const QString& path,const QString& suffix, QStringList& list);
Mat QImage2Mat(const QImage image);
QImage Mat2QImage(const cv::Mat& mat);
