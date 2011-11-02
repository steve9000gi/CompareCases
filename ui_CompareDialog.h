/********************************************************************************
** Form generated from reading UI file 'CompareDialog.ui'
**
** Created: Tue Oct 25 12:21:08 2011
**      by: Qt User Interface Compiler version 4.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMPAREDIALOG_H
#define UI_COMPAREDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "qvtkwidget.h"

QT_BEGIN_NAMESPACE

class Ui_CompareDialog
{
public:
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *queryGroupBox;
    QPushButton *queryInstitutionPushButton;
    QLabel *queryCaseInstitutionLabel;
    QLabel *queryCaseNumberLabel;
    QSpinBox *querySelectSpinBox;
    QVTKWidget *queryCTImageWidget;
    QVTKWidget *queryProjectionWidget;
    QGroupBox *legendGroupBox;
    QVBoxLayout *verticalLayout_2;
    QLabel *PTVLabel;
    QLabel *rectumLabel;
    QLabel *bladderLabel;
    QLabel *leftFemLabel;
    QLabel *rightFemLabel;
    QVTKWidget *overlayDVHWidget;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *matchGroupBox;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_4;
    QPushButton *matchInstitutionPushButton;
    QSpinBox *matchSelectSpinBox;
    QVTKWidget *matchCTImageWidget;
    QVTKWidget *matchProjectionWidget;
    QVTKWidget *matchDVHWidget;
    QGroupBox *viewGroupBox;
    QLabel *viewBladderCheckboxShadowLabel;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout;
    QCheckBox *viewRectumCheckBox;
    QCheckBox *viewPTVCheckBox;
    QCheckBox *viewBladderCheckBox;
    QCheckBox *viewFemoralHeadsCheckBox;
    QPushButton *overlaySelectionPushButton;
    QLabel *dvhLabel;
    QPushButton *removeSelectedOverlayPushButton;
    QCheckBox *overlayDVHCheckBox;
    QGroupBox *projectionDisplayGroupBox;
    QPushButton *gantryAnglePushButton;
    QCheckBox *flatShadedCheckBox;
    QCheckBox *originCheckBox;
    QLabel *transparencyLabel;
    QSpinBox *transparencySpinBox;
    QSlider *transparencySlider;
    QLabel *maxTransparencyLabel;
    QGroupBox *ctAxisGroupBox;
    QWidget *layoutWidget_4;
    QHBoxLayout *horizontalLayout;
    QRadioButton *axialRadioButton;
    QRadioButton *sagittalRadioButton;
    QRadioButton *coronalRadioButton;
    QLabel *sliceSelectionLabel;
    QSpinBox *sliceSelectionSpinBox;
    QSlider *sliceSelectionSlider;
    QLabel *maxSliceLabel;
    QPushButton *autoPlayPushButton;
    QWidget *layoutWidget2;
    QHBoxLayout *bottomBtnsHLayout;
    QPushButton *historyPushButton;
    QPushButton *removeCurrentMatchPushButton;
    QSpacerItem *horizontalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *CompareDialog)
    {
        if (CompareDialog->objectName().isEmpty())
            CompareDialog->setObjectName(QString::fromUtf8("CompareDialog"));
        CompareDialog->resize(1900, 1180);
        CompareDialog->setMinimumSize(QSize(0, 0));
        verticalLayout = new QVBoxLayout(CompareDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        scrollArea = new QScrollArea(CompareDialog);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setMinimumSize(QSize(0, 0));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1880, 1160));
        scrollAreaWidgetContents->setMinimumSize(QSize(1858, 1122));
        layoutWidget = new QWidget(scrollAreaWidgetContents);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 51, 1836, 514));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        queryGroupBox = new QGroupBox(layoutWidget);
        queryGroupBox->setObjectName(QString::fromUtf8("queryGroupBox"));
        queryGroupBox->setMaximumSize(QSize(81, 16777215));
        QFont font;
        font.setPointSize(16);
        queryGroupBox->setFont(font);
        queryInstitutionPushButton = new QPushButton(queryGroupBox);
        queryInstitutionPushButton->setObjectName(QString::fromUtf8("queryInstitutionPushButton"));
        queryInstitutionPushButton->setGeometry(QRect(10, 106, 59, 23));
        queryInstitutionPushButton->setMinimumSize(QSize(59, 23));
        queryInstitutionPushButton->setMaximumSize(QSize(59, 23));
        QFont font1;
        font1.setPointSize(8);
        queryInstitutionPushButton->setFont(font1);
        queryCaseInstitutionLabel = new QLabel(queryGroupBox);
        queryCaseInstitutionLabel->setObjectName(QString::fromUtf8("queryCaseInstitutionLabel"));
        queryCaseInstitutionLabel->setGeometry(QRect(10, 206, 59, 24));
        queryCaseInstitutionLabel->setMinimumSize(QSize(59, 24));
        queryCaseInstitutionLabel->setMaximumSize(QSize(59, 24));
        queryCaseInstitutionLabel->setFont(font1);
        queryCaseInstitutionLabel->setAlignment(Qt::AlignCenter);
        queryCaseNumberLabel = new QLabel(queryGroupBox);
        queryCaseNumberLabel->setObjectName(QString::fromUtf8("queryCaseNumberLabel"));
        queryCaseNumberLabel->setGeometry(QRect(10, 226, 59, 24));
        queryCaseNumberLabel->setMinimumSize(QSize(59, 24));
        queryCaseNumberLabel->setMaximumSize(QSize(59, 24));
        queryCaseNumberLabel->setFont(font1);
        queryCaseNumberLabel->setAlignment(Qt::AlignCenter);
        querySelectSpinBox = new QSpinBox(queryGroupBox);
        querySelectSpinBox->setObjectName(QString::fromUtf8("querySelectSpinBox"));
        querySelectSpinBox->setGeometry(QRect(10, 408, 39, 20));
        querySelectSpinBox->setFont(font1);
        querySelectSpinBox->setFrame(true);
        querySelectSpinBox->setReadOnly(false);
        querySelectSpinBox->setMinimum(1);
        querySelectSpinBox->setMaximum(500);
        querySelectSpinBox->setValue(12);

        horizontalLayout_2->addWidget(queryGroupBox);

        queryCTImageWidget = new QVTKWidget(layoutWidget);
        queryCTImageWidget->setObjectName(QString::fromUtf8("queryCTImageWidget"));
        queryCTImageWidget->setMinimumSize(QSize(512, 512));
        queryCTImageWidget->setFont(font1);
        queryCTImageWidget->setAutoFillBackground(true);

        horizontalLayout_2->addWidget(queryCTImageWidget);

        queryProjectionWidget = new QVTKWidget(layoutWidget);
        queryProjectionWidget->setObjectName(QString::fromUtf8("queryProjectionWidget"));
        queryProjectionWidget->setMinimumSize(QSize(512, 512));
        queryProjectionWidget->setAutoFillBackground(true);
        legendGroupBox = new QGroupBox(queryProjectionWidget);
        legendGroupBox->setObjectName(QString::fromUtf8("legendGroupBox"));
        legendGroupBox->setGeometry(QRect(432, 12, 72, 109));
        legendGroupBox->setAutoFillBackground(false);
        legendGroupBox->setStyleSheet(QString::fromUtf8("background-color:  rgba(0, 0, 0, 255)"));
        legendGroupBox->setAlignment(Qt::AlignCenter);
        verticalLayout_2 = new QVBoxLayout(legendGroupBox);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        PTVLabel = new QLabel(legendGroupBox);
        PTVLabel->setObjectName(QString::fromUtf8("PTVLabel"));
        QPalette palette;
        QBrush brush(QColor(230, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(0, 0, 0, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(0, 0, 230, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        QBrush brush3(QColor(120, 120, 120, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        PTVLabel->setPalette(palette);
        QFont font2;
        font2.setBold(true);
        font2.setWeight(75);
        PTVLabel->setFont(font2);
        PTVLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(PTVLabel);

        rectumLabel = new QLabel(legendGroupBox);
        rectumLabel->setObjectName(QString::fromUtf8("rectumLabel"));
        QPalette palette1;
        QBrush brush4(QColor(139, 69, 19, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush5(QColor(172, 0, 172, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush5);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush5);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        rectumLabel->setPalette(palette1);
        rectumLabel->setFont(font2);
        rectumLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(rectumLabel);

        bladderLabel = new QLabel(legendGroupBox);
        bladderLabel->setObjectName(QString::fromUtf8("bladderLabel"));
        QPalette palette2;
        QBrush brush6(QColor(255, 215, 0, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush6);
        palette2.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush6);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        bladderLabel->setPalette(palette2);
        bladderLabel->setFont(font2);
        bladderLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(bladderLabel);

        leftFemLabel = new QLabel(legendGroupBox);
        leftFemLabel->setObjectName(QString::fromUtf8("leftFemLabel"));
        QPalette palette3;
        QBrush brush7(QColor(85, 85, 102, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush7);
        palette3.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush7);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        leftFemLabel->setPalette(palette3);
        leftFemLabel->setFont(font2);
        leftFemLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(leftFemLabel);

        rightFemLabel = new QLabel(legendGroupBox);
        rightFemLabel->setObjectName(QString::fromUtf8("rightFemLabel"));
        QPalette palette4;
        QBrush brush8(QColor(171, 171, 171, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush8);
        palette4.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette4.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette4.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush8);
        palette4.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette4.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        palette4.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette4.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        rightFemLabel->setPalette(palette4);
        rightFemLabel->setFont(font2);
        rightFemLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(rightFemLabel);


        horizontalLayout_2->addWidget(queryProjectionWidget);

        overlayDVHWidget = new QVTKWidget(layoutWidget);
        overlayDVHWidget->setObjectName(QString::fromUtf8("overlayDVHWidget"));
        overlayDVHWidget->setMinimumSize(QSize(713, 512));
        overlayDVHWidget->setAutoFillBackground(true);

        horizontalLayout_2->addWidget(overlayDVHWidget);

        overlayDVHWidget->raise();
        queryCTImageWidget->raise();
        queryProjectionWidget->raise();
        queryGroupBox->raise();
        layoutWidget_2 = new QWidget(scrollAreaWidgetContents);
        layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(10, 571, 1838, 514));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget_2);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        matchGroupBox = new QGroupBox(layoutWidget_2);
        matchGroupBox->setObjectName(QString::fromUtf8("matchGroupBox"));
        matchGroupBox->setFont(font);
        verticalLayout_5 = new QVBoxLayout(matchGroupBox);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        matchInstitutionPushButton = new QPushButton(matchGroupBox);
        matchInstitutionPushButton->setObjectName(QString::fromUtf8("matchInstitutionPushButton"));
        matchInstitutionPushButton->setMinimumSize(QSize(59, 23));
        matchInstitutionPushButton->setMaximumSize(QSize(59, 23));
        matchInstitutionPushButton->setFont(font1);

        verticalLayout_4->addWidget(matchInstitutionPushButton);

        matchSelectSpinBox = new QSpinBox(matchGroupBox);
        matchSelectSpinBox->setObjectName(QString::fromUtf8("matchSelectSpinBox"));
        matchSelectSpinBox->setFont(font1);
        matchSelectSpinBox->setMinimum(1);
        matchSelectSpinBox->setMaximum(500);
        matchSelectSpinBox->setValue(12);

        verticalLayout_4->addWidget(matchSelectSpinBox);


        verticalLayout_5->addLayout(verticalLayout_4);


        horizontalLayout_3->addWidget(matchGroupBox);

        matchCTImageWidget = new QVTKWidget(layoutWidget_2);
        matchCTImageWidget->setObjectName(QString::fromUtf8("matchCTImageWidget"));
        matchCTImageWidget->setMinimumSize(QSize(512, 512));
        matchCTImageWidget->setAutoFillBackground(true);

        horizontalLayout_3->addWidget(matchCTImageWidget);

        matchProjectionWidget = new QVTKWidget(layoutWidget_2);
        matchProjectionWidget->setObjectName(QString::fromUtf8("matchProjectionWidget"));
        matchProjectionWidget->setMinimumSize(QSize(512, 512));
        matchProjectionWidget->setAutoFillBackground(true);

        horizontalLayout_3->addWidget(matchProjectionWidget);

        matchDVHWidget = new QVTKWidget(layoutWidget_2);
        matchDVHWidget->setObjectName(QString::fromUtf8("matchDVHWidget"));
        matchDVHWidget->setMinimumSize(QSize(713, 512));
        matchDVHWidget->setAutoFillBackground(true);

        horizontalLayout_3->addWidget(matchDVHWidget);

        viewGroupBox = new QGroupBox(scrollAreaWidgetContents);
        viewGroupBox->setObjectName(QString::fromUtf8("viewGroupBox"));
        viewGroupBox->setGeometry(QRect(1044, -1, 183, 51));
        viewBladderCheckboxShadowLabel = new QLabel(viewGroupBox);
        viewBladderCheckboxShadowLabel->setObjectName(QString::fromUtf8("viewBladderCheckboxShadowLabel"));
        viewBladderCheckboxShadowLabel->setGeometry(QRect(30, 35, 46, 13));
        layoutWidget1 = new QWidget(viewGroupBox);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(11, 12, 161, 42));
        gridLayout = new QGridLayout(layoutWidget1);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        viewRectumCheckBox = new QCheckBox(layoutWidget1);
        viewRectumCheckBox->setObjectName(QString::fromUtf8("viewRectumCheckBox"));

        gridLayout->addWidget(viewRectumCheckBox, 0, 0, 1, 1);

        viewPTVCheckBox = new QCheckBox(layoutWidget1);
        viewPTVCheckBox->setObjectName(QString::fromUtf8("viewPTVCheckBox"));

        gridLayout->addWidget(viewPTVCheckBox, 0, 1, 1, 1);

        viewBladderCheckBox = new QCheckBox(layoutWidget1);
        viewBladderCheckBox->setObjectName(QString::fromUtf8("viewBladderCheckBox"));

        gridLayout->addWidget(viewBladderCheckBox, 1, 0, 1, 1);

        viewFemoralHeadsCheckBox = new QCheckBox(layoutWidget1);
        viewFemoralHeadsCheckBox->setObjectName(QString::fromUtf8("viewFemoralHeadsCheckBox"));

        gridLayout->addWidget(viewFemoralHeadsCheckBox, 1, 1, 1, 1);

        overlaySelectionPushButton = new QPushButton(scrollAreaWidgetContents);
        overlaySelectionPushButton->setObjectName(QString::fromUtf8("overlaySelectionPushButton"));
        overlaySelectionPushButton->setGeometry(QRect(1655, 25, 91, 23));
        overlaySelectionPushButton->setMinimumSize(QSize(91, 0));
        dvhLabel = new QLabel(scrollAreaWidgetContents);
        dvhLabel->setObjectName(QString::fromUtf8("dvhLabel"));
        dvhLabel->setGeometry(QRect(1343, 24, 223, 25));
        dvhLabel->setFont(font);
        removeSelectedOverlayPushButton = new QPushButton(scrollAreaWidgetContents);
        removeSelectedOverlayPushButton->setObjectName(QString::fromUtf8("removeSelectedOverlayPushButton"));
        removeSelectedOverlayPushButton->setGeometry(QRect(1752, 25, 91, 23));
        removeSelectedOverlayPushButton->setMinimumSize(QSize(91, 0));
        overlayDVHCheckBox = new QCheckBox(scrollAreaWidgetContents);
        overlayDVHCheckBox->setObjectName(QString::fromUtf8("overlayDVHCheckBox"));
        overlayDVHCheckBox->setGeometry(QRect(1256, 28, 81, 17));
        projectionDisplayGroupBox = new QGroupBox(scrollAreaWidgetContents);
        projectionDisplayGroupBox->setObjectName(QString::fromUtf8("projectionDisplayGroupBox"));
        projectionDisplayGroupBox->setGeometry(QRect(610, -1, 420, 51));
        gantryAnglePushButton = new QPushButton(projectionDisplayGroupBox);
        gantryAnglePushButton->setObjectName(QString::fromUtf8("gantryAnglePushButton"));
        gantryAnglePushButton->setGeometry(QRect(240, 21, 86, 23));
        flatShadedCheckBox = new QCheckBox(projectionDisplayGroupBox);
        flatShadedCheckBox->setObjectName(QString::fromUtf8("flatShadedCheckBox"));
        flatShadedCheckBox->setGeometry(QRect(336, 24, 79, 17));
        originCheckBox = new QCheckBox(projectionDisplayGroupBox);
        originCheckBox->setObjectName(QString::fromUtf8("originCheckBox"));
        originCheckBox->setGeometry(QRect(356, 24, 51, 17));
        transparencyLabel = new QLabel(projectionDisplayGroupBox);
        transparencyLabel->setObjectName(QString::fromUtf8("transparencyLabel"));
        transparencyLabel->setGeometry(QRect(11, 21, 76, 16));
        transparencySpinBox = new QSpinBox(projectionDisplayGroupBox);
        transparencySpinBox->setObjectName(QString::fromUtf8("transparencySpinBox"));
        transparencySpinBox->setGeometry(QRect(93, 23, 39, 20));
        transparencySpinBox->setMinimum(0);
        transparencySpinBox->setMaximum(100);
        transparencySlider = new QSlider(projectionDisplayGroupBox);
        transparencySlider->setObjectName(QString::fromUtf8("transparencySlider"));
        transparencySlider->setGeometry(QRect(141, 21, 61, 24));
        transparencySlider->setMinimum(0);
        transparencySlider->setMaximum(100);
        transparencySlider->setPageStep(20);
        transparencySlider->setValue(0);
        transparencySlider->setOrientation(Qt::Horizontal);
        transparencySlider->setTickPosition(QSlider::TicksAbove);
        transparencySlider->setTickInterval(20);
        maxTransparencyLabel = new QLabel(projectionDisplayGroupBox);
        maxTransparencyLabel->setObjectName(QString::fromUtf8("maxTransparencyLabel"));
        maxTransparencyLabel->setGeometry(QRect(208, 21, 18, 16));
        transparencyLabel->raise();
        transparencySpinBox->raise();
        transparencySlider->raise();
        maxTransparencyLabel->raise();
        originCheckBox->raise();
        gantryAnglePushButton->raise();
        flatShadedCheckBox->raise();
        ctAxisGroupBox = new QGroupBox(scrollAreaWidgetContents);
        ctAxisGroupBox->setObjectName(QString::fromUtf8("ctAxisGroupBox"));
        ctAxisGroupBox->setGeometry(QRect(108, -1, 483, 51));
        layoutWidget_4 = new QWidget(ctAxisGroupBox);
        layoutWidget_4->setObjectName(QString::fromUtf8("layoutWidget_4"));
        layoutWidget_4->setGeometry(QRect(10, 19, 463, 26));
        horizontalLayout = new QHBoxLayout(layoutWidget_4);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        axialRadioButton = new QRadioButton(layoutWidget_4);
        axialRadioButton->setObjectName(QString::fromUtf8("axialRadioButton"));
        axialRadioButton->setChecked(true);

        horizontalLayout->addWidget(axialRadioButton);

        sagittalRadioButton = new QRadioButton(layoutWidget_4);
        sagittalRadioButton->setObjectName(QString::fromUtf8("sagittalRadioButton"));

        horizontalLayout->addWidget(sagittalRadioButton);

        coronalRadioButton = new QRadioButton(layoutWidget_4);
        coronalRadioButton->setObjectName(QString::fromUtf8("coronalRadioButton"));

        horizontalLayout->addWidget(coronalRadioButton);

        sliceSelectionLabel = new QLabel(layoutWidget_4);
        sliceSelectionLabel->setObjectName(QString::fromUtf8("sliceSelectionLabel"));

        horizontalLayout->addWidget(sliceSelectionLabel);

        sliceSelectionSpinBox = new QSpinBox(layoutWidget_4);
        sliceSelectionSpinBox->setObjectName(QString::fromUtf8("sliceSelectionSpinBox"));
        sliceSelectionSpinBox->setMinimum(0);
        sliceSelectionSpinBox->setMaximum(512);

        horizontalLayout->addWidget(sliceSelectionSpinBox);

        sliceSelectionSlider = new QSlider(layoutWidget_4);
        sliceSelectionSlider->setObjectName(QString::fromUtf8("sliceSelectionSlider"));
        sliceSelectionSlider->setMinimum(0);
        sliceSelectionSlider->setMaximum(100);
        sliceSelectionSlider->setPageStep(10);
        sliceSelectionSlider->setValue(0);
        sliceSelectionSlider->setOrientation(Qt::Horizontal);
        sliceSelectionSlider->setTickPosition(QSlider::TicksAbove);
        sliceSelectionSlider->setTickInterval(20);

        horizontalLayout->addWidget(sliceSelectionSlider);

        maxSliceLabel = new QLabel(layoutWidget_4);
        maxSliceLabel->setObjectName(QString::fromUtf8("maxSliceLabel"));

        horizontalLayout->addWidget(maxSliceLabel);

        autoPlayPushButton = new QPushButton(layoutWidget_4);
        autoPlayPushButton->setObjectName(QString::fromUtf8("autoPlayPushButton"));
        autoPlayPushButton->setFocusPolicy(Qt::NoFocus);

        horizontalLayout->addWidget(autoPlayPushButton);

        layoutWidget2 = new QWidget(scrollAreaWidgetContents);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(9, 1090, 1840, 25));
        bottomBtnsHLayout = new QHBoxLayout(layoutWidget2);
        bottomBtnsHLayout->setObjectName(QString::fromUtf8("bottomBtnsHLayout"));
        bottomBtnsHLayout->setContentsMargins(0, 0, 0, 0);
        historyPushButton = new QPushButton(layoutWidget2);
        historyPushButton->setObjectName(QString::fromUtf8("historyPushButton"));

        bottomBtnsHLayout->addWidget(historyPushButton);

        removeCurrentMatchPushButton = new QPushButton(layoutWidget2);
        removeCurrentMatchPushButton->setObjectName(QString::fromUtf8("removeCurrentMatchPushButton"));

        bottomBtnsHLayout->addWidget(removeCurrentMatchPushButton);

        horizontalSpacer = new QSpacerItem(1440, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        bottomBtnsHLayout->addItem(horizontalSpacer);

        buttonBox = new QDialogButtonBox(layoutWidget2);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        bottomBtnsHLayout->addWidget(buttonBox);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);

#ifndef QT_NO_SHORTCUT
        sliceSelectionLabel->setBuddy(sliceSelectionSpinBox);
#endif // QT_NO_SHORTCUT

        retranslateUi(CompareDialog);

        QMetaObject::connectSlotsByName(CompareDialog);
    } // setupUi

    void retranslateUi(QDialog *CompareDialog)
    {
        CompareDialog->setWindowTitle(QApplication::translate("CompareDialog", "Compare Cases", 0, QApplication::UnicodeUTF8));
        queryGroupBox->setTitle(QApplication::translate("CompareDialog", "Query", 0, QApplication::UnicodeUTF8));
        queryInstitutionPushButton->setText(QApplication::translate("CompareDialog", "Duke", 0, QApplication::UnicodeUTF8));
        queryCaseInstitutionLabel->setText(QApplication::translate("CompareDialog", "Duke", 0, QApplication::UnicodeUTF8));
        queryCaseNumberLabel->setText(QApplication::translate("CompareDialog", "#666", 0, QApplication::UnicodeUTF8));
        querySelectSpinBox->setPrefix(QString());
        legendGroupBox->setTitle(QString());
        PTVLabel->setText(QApplication::translate("CompareDialog", "PTV", 0, QApplication::UnicodeUTF8));
        rectumLabel->setText(QApplication::translate("CompareDialog", "rectum", 0, QApplication::UnicodeUTF8));
        bladderLabel->setText(QApplication::translate("CompareDialog", "bladder", 0, QApplication::UnicodeUTF8));
        leftFemLabel->setText(QApplication::translate("CompareDialog", "left fem", 0, QApplication::UnicodeUTF8));
        rightFemLabel->setText(QApplication::translate("CompareDialog", "right fem", 0, QApplication::UnicodeUTF8));
        matchGroupBox->setTitle(QApplication::translate("CompareDialog", "Match", 0, QApplication::UnicodeUTF8));
        matchInstitutionPushButton->setText(QApplication::translate("CompareDialog", "Duke", 0, QApplication::UnicodeUTF8));
        viewGroupBox->setTitle(QApplication::translate("CompareDialog", "View", 0, QApplication::UnicodeUTF8));
        viewBladderCheckboxShadowLabel->setText(QApplication::translate("CompareDialog", "Bladder", 0, QApplication::UnicodeUTF8));
        viewRectumCheckBox->setText(QApplication::translate("CompareDialog", "Rectum", 0, QApplication::UnicodeUTF8));
        viewPTVCheckBox->setText(QApplication::translate("CompareDialog", "PTV", 0, QApplication::UnicodeUTF8));
        viewBladderCheckBox->setText(QApplication::translate("CompareDialog", "Bladder", 0, QApplication::UnicodeUTF8));
        viewFemoralHeadsCheckBox->setText(QApplication::translate("CompareDialog", "Femoral Heads", 0, QApplication::UnicodeUTF8));
        overlaySelectionPushButton->setText(QApplication::translate("CompareDialog", "Select Overlay", 0, QApplication::UnicodeUTF8));
        dvhLabel->setText(QApplication::translate("CompareDialog", "Dose Volume Histogram", 0, QApplication::UnicodeUTF8));
        removeSelectedOverlayPushButton->setText(QApplication::translate("CompareDialog", "Remove Selected", 0, QApplication::UnicodeUTF8));
        overlayDVHCheckBox->setText(QApplication::translate("CompareDialog", " Overlay On", 0, QApplication::UnicodeUTF8));
        projectionDisplayGroupBox->setTitle(QApplication::translate("CompareDialog", "Projection Display", 0, QApplication::UnicodeUTF8));
        gantryAnglePushButton->setText(QApplication::translate("CompareDialog", "Gantry Angle", 0, QApplication::UnicodeUTF8));
        flatShadedCheckBox->setText(QApplication::translate("CompareDialog", "Flat shaded", 0, QApplication::UnicodeUTF8));
        originCheckBox->setText(QApplication::translate("CompareDialog", "Axes", 0, QApplication::UnicodeUTF8));
        transparencyLabel->setText(QApplication::translate("CompareDialog", "%Transparency", 0, QApplication::UnicodeUTF8));
        maxTransparencyLabel->setText(QApplication::translate("CompareDialog", "100", 0, QApplication::UnicodeUTF8));
        ctAxisGroupBox->setTitle(QApplication::translate("CompareDialog", "CT Display", 0, QApplication::UnicodeUTF8));
        axialRadioButton->setText(QApplication::translate("CompareDialog", "Axial", 0, QApplication::UnicodeUTF8));
        sagittalRadioButton->setText(QApplication::translate("CompareDialog", "Sagittal", 0, QApplication::UnicodeUTF8));
        coronalRadioButton->setText(QApplication::translate("CompareDialog", "Coronal", 0, QApplication::UnicodeUTF8));
        sliceSelectionLabel->setText(QApplication::translate("CompareDialog", "Z slice:", 0, QApplication::UnicodeUTF8));
        maxSliceLabel->setText(QApplication::translate("CompareDialog", "max", 0, QApplication::UnicodeUTF8));
        autoPlayPushButton->setText(QApplication::translate("CompareDialog", "Auto-play", 0, QApplication::UnicodeUTF8));
        historyPushButton->setText(QApplication::translate("CompareDialog", "History", 0, QApplication::UnicodeUTF8));
        removeCurrentMatchPushButton->setText(QApplication::translate("CompareDialog", "Remove Current", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CompareDialog: public Ui_CompareDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMPAREDIALOG_H
