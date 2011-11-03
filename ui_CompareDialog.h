/********************************************************************************
** Form generated from reading UI file 'CompareDialog.ui'
**
** Created: Thu Nov 3 16:04:49 2011
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
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSlider>
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
    QGroupBox *viewGroupBox;
    QLabel *viewBladderCheckboxShadowLabel;
    QCheckBox *viewRectumCheckBox;
    QCheckBox *viewPTVCheckBox;
    QCheckBox *viewBladderCheckBox;
    QCheckBox *viewFemoralHeadsCheckBox;
    QGroupBox *projectionDisplayGroupBox;
    QPushButton *gantryAnglePushButton;
    QCheckBox *flatShadedCheckBox;
    QCheckBox *originCheckBox;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_6;
    QLabel *transparencyLabel;
    QSpinBox *transparencySpinBox;
    QSlider *transparencySlider;
    QLabel *maxTransparencyLabel;
    QGroupBox *ctAxisGroupBox;
    QRadioButton *axialRadioButton;
    QRadioButton *sagittalRadioButton;
    QRadioButton *coronalRadioButton;
    QPushButton *autoPlayPushButton;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_5;
    QLabel *sliceSelectionLabel;
    QSpinBox *sliceSelectionSpinBox;
    QSlider *sliceSelectionSlider;
    QLabel *maxSliceLabel;
    QGroupBox *DVHGroupBox;
    QPushButton *removeSelectedOverlayPushButton;
    QPushButton *overlaySelectionPushButton;
    QCheckBox *overlayDVHCheckBox;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout;
    QVTKWidget *queryCTImageWidget;
    QVTKWidget *queryProjectionWidget;
    QVTKWidget *overlayDVHWidget;
    QGroupBox *queryGroupBox;
    QLabel *queryCaseInstitutionLabel;
    QSpinBox *querySelectSpinBox;
    QPushButton *queryInstitutionPushButton;
    QLabel *queryCaseNumberLabel;
    QWidget *layoutWidget3;
    QHBoxLayout *horizontalLayout_2;
    QVTKWidget *matchCTImageWidget;
    QVTKWidget *matchProjectionWidget;
    QVTKWidget *matchDVHWidget;
    QGroupBox *matchGroupBox;
    QPushButton *matchInstitutionPushButton;
    QSpinBox *matchSelectSpinBox;
    QWidget *layoutWidget4;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *OKPushButton;
    QPushButton *cancelPushButton;
    QWidget *layoutWidget5;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *historyPushButton;
    QPushButton *removeCurrentMatchPushButton;

    void setupUi(QDialog *CompareDialog)
    {
        if (CompareDialog->objectName().isEmpty())
            CompareDialog->setObjectName(QString::fromUtf8("CompareDialog"));
        CompareDialog->resize(991, 667);
        CompareDialog->setMinimumSize(QSize(0, 0));
        verticalLayout = new QVBoxLayout(CompareDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        scrollArea = new QScrollArea(CompareDialog);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setMinimumSize(QSize(0, 0));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 971, 647));
        scrollAreaWidgetContents->setMinimumSize(QSize(955, 634));
        viewGroupBox = new QGroupBox(scrollAreaWidgetContents);
        viewGroupBox->setObjectName(QString::fromUtf8("viewGroupBox"));
        viewGroupBox->setGeometry(QRect(514, 582, 184, 58));
        viewBladderCheckboxShadowLabel = new QLabel(viewGroupBox);
        viewBladderCheckboxShadowLabel->setObjectName(QString::fromUtf8("viewBladderCheckboxShadowLabel"));
        viewBladderCheckboxShadowLabel->setGeometry(QRect(30, 38, 46, 13));
        viewRectumCheckBox = new QCheckBox(viewGroupBox);
        viewRectumCheckBox->setObjectName(QString::fromUtf8("viewRectumCheckBox"));
        viewRectumCheckBox->setGeometry(QRect(11, 13, 59, 17));
        viewPTVCheckBox = new QCheckBox(viewGroupBox);
        viewPTVCheckBox->setObjectName(QString::fromUtf8("viewPTVCheckBox"));
        viewPTVCheckBox->setGeometry(QRect(76, 13, 41, 17));
        viewBladderCheckBox = new QCheckBox(viewGroupBox);
        viewBladderCheckBox->setObjectName(QString::fromUtf8("viewBladderCheckBox"));
        viewBladderCheckBox->setGeometry(QRect(11, 36, 59, 16));
        viewFemoralHeadsCheckBox = new QCheckBox(viewGroupBox);
        viewFemoralHeadsCheckBox->setObjectName(QString::fromUtf8("viewFemoralHeadsCheckBox"));
        viewFemoralHeadsCheckBox->setGeometry(QRect(76, 36, 97, 16));
        projectionDisplayGroupBox = new QGroupBox(scrollAreaWidgetContents);
        projectionDisplayGroupBox->setObjectName(QString::fromUtf8("projectionDisplayGroupBox"));
        projectionDisplayGroupBox->setGeometry(QRect(416, -1, 287, 51));
        gantryAnglePushButton = new QPushButton(projectionDisplayGroupBox);
        gantryAnglePushButton->setObjectName(QString::fromUtf8("gantryAnglePushButton"));
        gantryAnglePushButton->setGeometry(QRect(196, 18, 85, 23));
        flatShadedCheckBox = new QCheckBox(projectionDisplayGroupBox);
        flatShadedCheckBox->setObjectName(QString::fromUtf8("flatShadedCheckBox"));
        flatShadedCheckBox->setGeometry(QRect(292, 24, 79, 17));
        originCheckBox = new QCheckBox(projectionDisplayGroupBox);
        originCheckBox->setObjectName(QString::fromUtf8("originCheckBox"));
        originCheckBox->setGeometry(QRect(286, 24, 51, 17));
        layoutWidget = new QWidget(projectionDisplayGroupBox);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 18, 177, 26));
        horizontalLayout_6 = new QHBoxLayout(layoutWidget);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        transparencyLabel = new QLabel(layoutWidget);
        transparencyLabel->setObjectName(QString::fromUtf8("transparencyLabel"));

        horizontalLayout_6->addWidget(transparencyLabel);

        transparencySpinBox = new QSpinBox(layoutWidget);
        transparencySpinBox->setObjectName(QString::fromUtf8("transparencySpinBox"));
        transparencySpinBox->setMinimum(0);
        transparencySpinBox->setMaximum(100);

        horizontalLayout_6->addWidget(transparencySpinBox);

        transparencySlider = new QSlider(layoutWidget);
        transparencySlider->setObjectName(QString::fromUtf8("transparencySlider"));
        transparencySlider->setMinimum(0);
        transparencySlider->setMaximum(100);
        transparencySlider->setPageStep(20);
        transparencySlider->setValue(0);
        transparencySlider->setOrientation(Qt::Horizontal);
        transparencySlider->setTickPosition(QSlider::TicksAbove);
        transparencySlider->setTickInterval(20);

        horizontalLayout_6->addWidget(transparencySlider);

        maxTransparencyLabel = new QLabel(layoutWidget);
        maxTransparencyLabel->setObjectName(QString::fromUtf8("maxTransparencyLabel"));

        horizontalLayout_6->addWidget(maxTransparencyLabel);

        layoutWidget->raise();
        originCheckBox->raise();
        gantryAnglePushButton->raise();
        flatShadedCheckBox->raise();
        ctAxisGroupBox = new QGroupBox(scrollAreaWidgetContents);
        ctAxisGroupBox->setObjectName(QString::fromUtf8("ctAxisGroupBox"));
        ctAxisGroupBox->setGeometry(QRect(8, -1, 403, 51));
        axialRadioButton = new QRadioButton(ctAxisGroupBox);
        axialRadioButton->setObjectName(QString::fromUtf8("axialRadioButton"));
        axialRadioButton->setGeometry(QRect(4, 22, 46, 17));
        axialRadioButton->setChecked(true);
        sagittalRadioButton = new QRadioButton(ctAxisGroupBox);
        sagittalRadioButton->setObjectName(QString::fromUtf8("sagittalRadioButton"));
        sagittalRadioButton->setGeometry(QRect(50, 22, 59, 17));
        coronalRadioButton = new QRadioButton(ctAxisGroupBox);
        coronalRadioButton->setObjectName(QString::fromUtf8("coronalRadioButton"));
        coronalRadioButton->setGeometry(QRect(110, 22, 60, 17));
        autoPlayPushButton = new QPushButton(ctAxisGroupBox);
        autoPlayPushButton->setObjectName(QString::fromUtf8("autoPlayPushButton"));
        autoPlayPushButton->setGeometry(QRect(338, 18, 58, 23));
        autoPlayPushButton->setFocusPolicy(Qt::NoFocus);
        layoutWidget1 = new QWidget(ctAxisGroupBox);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(176, 18, 154, 26));
        horizontalLayout_5 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        sliceSelectionLabel = new QLabel(layoutWidget1);
        sliceSelectionLabel->setObjectName(QString::fromUtf8("sliceSelectionLabel"));

        horizontalLayout_5->addWidget(sliceSelectionLabel);

        sliceSelectionSpinBox = new QSpinBox(layoutWidget1);
        sliceSelectionSpinBox->setObjectName(QString::fromUtf8("sliceSelectionSpinBox"));
        sliceSelectionSpinBox->setMinimum(0);
        sliceSelectionSpinBox->setMaximum(512);

        horizontalLayout_5->addWidget(sliceSelectionSpinBox);

        sliceSelectionSlider = new QSlider(layoutWidget1);
        sliceSelectionSlider->setObjectName(QString::fromUtf8("sliceSelectionSlider"));
        sliceSelectionSlider->setMinimum(0);
        sliceSelectionSlider->setMaximum(100);
        sliceSelectionSlider->setPageStep(10);
        sliceSelectionSlider->setValue(0);
        sliceSelectionSlider->setOrientation(Qt::Horizontal);
        sliceSelectionSlider->setTickPosition(QSlider::TicksAbove);
        sliceSelectionSlider->setTickInterval(20);

        horizontalLayout_5->addWidget(sliceSelectionSlider);

        maxSliceLabel = new QLabel(layoutWidget1);
        maxSliceLabel->setObjectName(QString::fromUtf8("maxSliceLabel"));

        horizontalLayout_5->addWidget(maxSliceLabel);

        DVHGroupBox = new QGroupBox(scrollAreaWidgetContents);
        DVHGroupBox->setObjectName(QString::fromUtf8("DVHGroupBox"));
        DVHGroupBox->setGeometry(QRect(708, -1, 253, 51));
        removeSelectedOverlayPushButton = new QPushButton(DVHGroupBox);
        removeSelectedOverlayPushButton->setObjectName(QString::fromUtf8("removeSelectedOverlayPushButton"));
        removeSelectedOverlayPushButton->setGeometry(QRect(106, 18, 91, 23));
        removeSelectedOverlayPushButton->setMinimumSize(QSize(91, 0));
        overlaySelectionPushButton = new QPushButton(DVHGroupBox);
        overlaySelectionPushButton->setObjectName(QString::fromUtf8("overlaySelectionPushButton"));
        overlaySelectionPushButton->setGeometry(QRect(9, 18, 91, 23));
        overlaySelectionPushButton->setMinimumSize(QSize(91, 0));
        overlayDVHCheckBox = new QCheckBox(DVHGroupBox);
        overlayDVHCheckBox->setObjectName(QString::fromUtf8("overlayDVHCheckBox"));
        overlayDVHCheckBox->setGeometry(QRect(214, 19, 37, 17));
        layoutWidget2 = new QWidget(scrollAreaWidgetContents);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(74, 58, 886, 258));
        horizontalLayout = new QHBoxLayout(layoutWidget2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        queryCTImageWidget = new QVTKWidget(layoutWidget2);
        queryCTImageWidget->setObjectName(QString::fromUtf8("queryCTImageWidget"));
        queryCTImageWidget->setMinimumSize(QSize(256, 256));
        QFont font;
        font.setPointSize(8);
        queryCTImageWidget->setFont(font);
        queryCTImageWidget->setAutoFillBackground(true);

        horizontalLayout->addWidget(queryCTImageWidget);

        queryProjectionWidget = new QVTKWidget(layoutWidget2);
        queryProjectionWidget->setObjectName(QString::fromUtf8("queryProjectionWidget"));
        queryProjectionWidget->setMinimumSize(QSize(256, 256));
        queryProjectionWidget->setAutoFillBackground(true);

        horizontalLayout->addWidget(queryProjectionWidget);

        overlayDVHWidget = new QVTKWidget(layoutWidget2);
        overlayDVHWidget->setObjectName(QString::fromUtf8("overlayDVHWidget"));
        overlayDVHWidget->setMinimumSize(QSize(360, 256));
        overlayDVHWidget->setAutoFillBackground(true);

        horizontalLayout->addWidget(overlayDVHWidget);

        queryGroupBox = new QGroupBox(scrollAreaWidgetContents);
        queryGroupBox->setObjectName(QString::fromUtf8("queryGroupBox"));
        queryGroupBox->setGeometry(QRect(8, 58, 58, 258));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(queryGroupBox->sizePolicy().hasHeightForWidth());
        queryGroupBox->setSizePolicy(sizePolicy);
        queryGroupBox->setMaximumSize(QSize(81, 258));
        QFont font1;
        font1.setPointSize(12);
        queryGroupBox->setFont(font1);
        queryCaseInstitutionLabel = new QLabel(queryGroupBox);
        queryCaseInstitutionLabel->setObjectName(QString::fromUtf8("queryCaseInstitutionLabel"));
        queryCaseInstitutionLabel->setGeometry(QRect(0, 101, 59, 24));
        queryCaseInstitutionLabel->setMinimumSize(QSize(59, 24));
        queryCaseInstitutionLabel->setMaximumSize(QSize(59, 24));
        queryCaseInstitutionLabel->setFont(font);
        queryCaseInstitutionLabel->setAlignment(Qt::AlignCenter);
        querySelectSpinBox = new QSpinBox(queryGroupBox);
        querySelectSpinBox->setObjectName(QString::fromUtf8("querySelectSpinBox"));
        querySelectSpinBox->setGeometry(QRect(10, 202, 38, 20));
        querySelectSpinBox->setFont(font);
        querySelectSpinBox->setFrame(true);
        querySelectSpinBox->setReadOnly(false);
        querySelectSpinBox->setMinimum(1);
        querySelectSpinBox->setMaximum(500);
        querySelectSpinBox->setValue(12);
        queryInstitutionPushButton = new QPushButton(queryGroupBox);
        queryInstitutionPushButton->setObjectName(QString::fromUtf8("queryInstitutionPushButton"));
        queryInstitutionPushButton->setGeometry(QRect(10, 50, 59, 23));
        queryInstitutionPushButton->setMinimumSize(QSize(59, 23));
        queryInstitutionPushButton->setMaximumSize(QSize(59, 23));
        queryInstitutionPushButton->setFont(font);
        queryCaseNumberLabel = new QLabel(queryGroupBox);
        queryCaseNumberLabel->setObjectName(QString::fromUtf8("queryCaseNumberLabel"));
        queryCaseNumberLabel->setGeometry(QRect(-4, 131, 59, 24));
        queryCaseNumberLabel->setMinimumSize(QSize(59, 24));
        queryCaseNumberLabel->setMaximumSize(QSize(59, 24));
        queryCaseNumberLabel->setFont(font);
        queryCaseNumberLabel->setAlignment(Qt::AlignCenter);
        layoutWidget3 = new QWidget(scrollAreaWidgetContents);
        layoutWidget3->setObjectName(QString::fromUtf8("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(74, 322, 886, 258));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget3);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        matchCTImageWidget = new QVTKWidget(layoutWidget3);
        matchCTImageWidget->setObjectName(QString::fromUtf8("matchCTImageWidget"));
        matchCTImageWidget->setMinimumSize(QSize(256, 256));
        matchCTImageWidget->setAutoFillBackground(true);

        horizontalLayout_2->addWidget(matchCTImageWidget);

        matchProjectionWidget = new QVTKWidget(layoutWidget3);
        matchProjectionWidget->setObjectName(QString::fromUtf8("matchProjectionWidget"));
        matchProjectionWidget->setMinimumSize(QSize(256, 256));
        matchProjectionWidget->setAutoFillBackground(true);

        horizontalLayout_2->addWidget(matchProjectionWidget);

        matchDVHWidget = new QVTKWidget(layoutWidget3);
        matchDVHWidget->setObjectName(QString::fromUtf8("matchDVHWidget"));
        matchDVHWidget->setMinimumSize(QSize(360, 256));
        matchDVHWidget->setAutoFillBackground(true);

        horizontalLayout_2->addWidget(matchDVHWidget);

        matchGroupBox = new QGroupBox(scrollAreaWidgetContents);
        matchGroupBox->setObjectName(QString::fromUtf8("matchGroupBox"));
        matchGroupBox->setGeometry(QRect(8, 322, 58, 258));
        matchGroupBox->setFont(font1);
        matchInstitutionPushButton = new QPushButton(matchGroupBox);
        matchInstitutionPushButton->setObjectName(QString::fromUtf8("matchInstitutionPushButton"));
        matchInstitutionPushButton->setGeometry(QRect(4, 91, 49, 23));
        matchInstitutionPushButton->setMinimumSize(QSize(49, 23));
        matchInstitutionPushButton->setMaximumSize(QSize(59, 23));
        matchInstitutionPushButton->setFont(font);
        matchSelectSpinBox = new QSpinBox(matchGroupBox);
        matchSelectSpinBox->setObjectName(QString::fromUtf8("matchSelectSpinBox"));
        matchSelectSpinBox->setGeometry(QRect(10, 138, 39, 20));
        matchSelectSpinBox->setFont(font);
        matchSelectSpinBox->setMinimum(1);
        matchSelectSpinBox->setMaximum(500);
        matchSelectSpinBox->setValue(12);
        layoutWidget4 = new QWidget(scrollAreaWidgetContents);
        layoutWidget4->setObjectName(QString::fromUtf8("layoutWidget4"));
        layoutWidget4->setGeometry(QRect(804, 593, 158, 25));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget4);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        OKPushButton = new QPushButton(layoutWidget4);
        OKPushButton->setObjectName(QString::fromUtf8("OKPushButton"));

        horizontalLayout_3->addWidget(OKPushButton);

        cancelPushButton = new QPushButton(layoutWidget4);
        cancelPushButton->setObjectName(QString::fromUtf8("cancelPushButton"));

        horizontalLayout_3->addWidget(cancelPushButton);

        layoutWidget5 = new QWidget(scrollAreaWidgetContents);
        layoutWidget5->setObjectName(QString::fromUtf8("layoutWidget5"));
        layoutWidget5->setGeometry(QRect(8, 593, 170, 25));
        horizontalLayout_4 = new QHBoxLayout(layoutWidget5);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        historyPushButton = new QPushButton(layoutWidget5);
        historyPushButton->setObjectName(QString::fromUtf8("historyPushButton"));

        horizontalLayout_4->addWidget(historyPushButton);

        removeCurrentMatchPushButton = new QPushButton(layoutWidget5);
        removeCurrentMatchPushButton->setObjectName(QString::fromUtf8("removeCurrentMatchPushButton"));

        horizontalLayout_4->addWidget(removeCurrentMatchPushButton);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);

#ifndef QT_NO_SHORTCUT
        sliceSelectionLabel->setBuddy(sliceSelectionSpinBox);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(scrollArea, axialRadioButton);
        QWidget::setTabOrder(axialRadioButton, sagittalRadioButton);
        QWidget::setTabOrder(sagittalRadioButton, coronalRadioButton);
        QWidget::setTabOrder(coronalRadioButton, sliceSelectionSpinBox);
        QWidget::setTabOrder(sliceSelectionSpinBox, sliceSelectionSlider);
        QWidget::setTabOrder(sliceSelectionSlider, transparencySpinBox);
        QWidget::setTabOrder(transparencySpinBox, transparencySlider);
        QWidget::setTabOrder(transparencySlider, gantryAnglePushButton);
        QWidget::setTabOrder(gantryAnglePushButton, overlaySelectionPushButton);
        QWidget::setTabOrder(overlaySelectionPushButton, flatShadedCheckBox);
        QWidget::setTabOrder(flatShadedCheckBox, originCheckBox);
        QWidget::setTabOrder(originCheckBox, removeSelectedOverlayPushButton);
        QWidget::setTabOrder(removeSelectedOverlayPushButton, overlayDVHCheckBox);
        QWidget::setTabOrder(overlayDVHCheckBox, queryInstitutionPushButton);
        QWidget::setTabOrder(queryInstitutionPushButton, querySelectSpinBox);
        QWidget::setTabOrder(querySelectSpinBox, matchInstitutionPushButton);
        QWidget::setTabOrder(matchInstitutionPushButton, matchSelectSpinBox);
        QWidget::setTabOrder(matchSelectSpinBox, historyPushButton);
        QWidget::setTabOrder(historyPushButton, removeCurrentMatchPushButton);
        QWidget::setTabOrder(removeCurrentMatchPushButton, viewRectumCheckBox);
        QWidget::setTabOrder(viewRectumCheckBox, viewBladderCheckBox);
        QWidget::setTabOrder(viewBladderCheckBox, viewPTVCheckBox);
        QWidget::setTabOrder(viewPTVCheckBox, viewFemoralHeadsCheckBox);
        QWidget::setTabOrder(viewFemoralHeadsCheckBox, OKPushButton);
        QWidget::setTabOrder(OKPushButton, cancelPushButton);

        retranslateUi(CompareDialog);

        QMetaObject::connectSlotsByName(CompareDialog);
    } // setupUi

    void retranslateUi(QDialog *CompareDialog)
    {
        CompareDialog->setWindowTitle(QApplication::translate("CompareDialog", "Compare Cases", 0, QApplication::UnicodeUTF8));
        viewGroupBox->setTitle(QApplication::translate("CompareDialog", "View", 0, QApplication::UnicodeUTF8));
        viewBladderCheckboxShadowLabel->setText(QApplication::translate("CompareDialog", "Bladder", 0, QApplication::UnicodeUTF8));
        viewRectumCheckBox->setText(QApplication::translate("CompareDialog", "Rectum", 0, QApplication::UnicodeUTF8));
        viewPTVCheckBox->setText(QApplication::translate("CompareDialog", "PTV", 0, QApplication::UnicodeUTF8));
        viewBladderCheckBox->setText(QApplication::translate("CompareDialog", "Bladder", 0, QApplication::UnicodeUTF8));
        viewFemoralHeadsCheckBox->setText(QApplication::translate("CompareDialog", "Femoral  Heads", 0, QApplication::UnicodeUTF8));
        projectionDisplayGroupBox->setTitle(QApplication::translate("CompareDialog", "Beam's Eye View", 0, QApplication::UnicodeUTF8));
        gantryAnglePushButton->setText(QApplication::translate("CompareDialog", "Gantry Angle", 0, QApplication::UnicodeUTF8));
        flatShadedCheckBox->setText(QApplication::translate("CompareDialog", "Flat shaded", 0, QApplication::UnicodeUTF8));
        originCheckBox->setText(QApplication::translate("CompareDialog", "Axes", 0, QApplication::UnicodeUTF8));
        transparencyLabel->setText(QApplication::translate("CompareDialog", "%Transp", 0, QApplication::UnicodeUTF8));
        maxTransparencyLabel->setText(QApplication::translate("CompareDialog", "100", 0, QApplication::UnicodeUTF8));
        ctAxisGroupBox->setTitle(QApplication::translate("CompareDialog", "CT Display", 0, QApplication::UnicodeUTF8));
        axialRadioButton->setText(QApplication::translate("CompareDialog", "Axial", 0, QApplication::UnicodeUTF8));
        sagittalRadioButton->setText(QApplication::translate("CompareDialog", "Sagittal", 0, QApplication::UnicodeUTF8));
        coronalRadioButton->setText(QApplication::translate("CompareDialog", "Coronal", 0, QApplication::UnicodeUTF8));
        autoPlayPushButton->setText(QApplication::translate("CompareDialog", "Auto-play", 0, QApplication::UnicodeUTF8));
        sliceSelectionLabel->setText(QApplication::translate("CompareDialog", "Z slice:", 0, QApplication::UnicodeUTF8));
        maxSliceLabel->setText(QApplication::translate("CompareDialog", "max", 0, QApplication::UnicodeUTF8));
        DVHGroupBox->setTitle(QApplication::translate("CompareDialog", "DVH Overlay", 0, QApplication::UnicodeUTF8));
        removeSelectedOverlayPushButton->setText(QApplication::translate("CompareDialog", "Remove Selected", 0, QApplication::UnicodeUTF8));
        overlaySelectionPushButton->setText(QApplication::translate("CompareDialog", "Select Overlay", 0, QApplication::UnicodeUTF8));
        overlayDVHCheckBox->setText(QApplication::translate("CompareDialog", "On", 0, QApplication::UnicodeUTF8));
        queryGroupBox->setTitle(QApplication::translate("CompareDialog", "Query", 0, QApplication::UnicodeUTF8));
        queryCaseInstitutionLabel->setText(QApplication::translate("CompareDialog", "Duke", 0, QApplication::UnicodeUTF8));
        querySelectSpinBox->setPrefix(QString());
        queryInstitutionPushButton->setText(QApplication::translate("CompareDialog", "Duke", 0, QApplication::UnicodeUTF8));
        queryCaseNumberLabel->setText(QApplication::translate("CompareDialog", "#666", 0, QApplication::UnicodeUTF8));
        matchGroupBox->setTitle(QApplication::translate("CompareDialog", "Match", 0, QApplication::UnicodeUTF8));
        matchInstitutionPushButton->setText(QApplication::translate("CompareDialog", "Duke", 0, QApplication::UnicodeUTF8));
        OKPushButton->setText(QApplication::translate("CompareDialog", "OK", 0, QApplication::UnicodeUTF8));
        cancelPushButton->setText(QApplication::translate("CompareDialog", "Cancel", 0, QApplication::UnicodeUTF8));
        historyPushButton->setText(QApplication::translate("CompareDialog", "History", 0, QApplication::UnicodeUTF8));
        removeCurrentMatchPushButton->setText(QApplication::translate("CompareDialog", "Remove Current", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CompareDialog: public Ui_CompareDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMPAREDIALOG_H
