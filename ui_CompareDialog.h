/********************************************************************************
** Form generated from reading UI file 'CompareDialog.ui'
**
** Created: Thu Nov 17 15:38:04 2011
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
    QGroupBox *viewGroupBox;
    QLabel *viewBladderCheckboxShadowLabel;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QCheckBox *viewRectumCheckBox;
    QCheckBox *viewPTVCheckBox;
    QCheckBox *viewBladderCheckBox;
    QCheckBox *viewFemoralHeadsCheckBox;
    QGroupBox *projectionDisplayGroupBox;
    QHBoxLayout *horizontalLayout_4;
    QLabel *transparencyLabel;
    QSpinBox *transparencySpinBox;
    QSlider *transparencySlider;
    QLabel *maxTransparencyLabel;
    QPushButton *gantryAnglePushButton;
    QGroupBox *ctAxisGroupBox;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *axialRadioButton;
    QRadioButton *sagittalRadioButton;
    QRadioButton *coronalRadioButton;
    QLabel *sliceSelectionLabel;
    QSlider *sliceSelectionSlider;
    QPushButton *isocenterPushButton;
    QPushButton *autoPlayPushButton;
    QWidget *layoutWidget1;
    QHBoxLayout *bottomBtnsHLayout;
    QPushButton *historyPushButton;
    QPushButton *removeCurrentMatchPushButton;
    QSpacerItem *horizontalSpacer;
    QDialogButtonBox *buttonBox;
    QGroupBox *DVHGroupBox;
    QHBoxLayout *horizontalLayout_5;
    QCheckBox *overlayDVHCheckBox;
    QPushButton *overlaySelectionPushButton;
    QPushButton *removeSelectedOverlayPushButton;
    QWidget *widget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QGroupBox *queryGroupBox;
    QPushButton *queryInstitutionPushButton;
    QLabel *queryCaseInstitutionLabel;
    QLabel *queryCaseNumberLabel;
    QSpinBox *querySelectSpinBox;
    QSpinBox *sliceSelectionSpinBox;
    QLabel *maxSliceLabel;
    QCheckBox *originCheckBox;
    QCheckBox *flatShadedCheckBox;
    QVTKWidget *queryCTImageWidget;
    QLabel *dvhLabel;
    QVTKWidget *queryProjectionWidget;
    QVTKWidget *overlayDVHWidget;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *matchGroupBox;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_4;
    QPushButton *matchInstitutionPushButton;
    QSpinBox *matchSelectSpinBox;
    QVTKWidget *matchCTImageWidget;
    QVTKWidget *matchProjectionWidget;
    QVTKWidget *matchDVHWidget;

    void setupUi(QDialog *CompareDialog)
    {
        if (CompareDialog->objectName().isEmpty())
            CompareDialog->setObjectName(QString::fromUtf8("CompareDialog"));
        CompareDialog->resize(1764, 1043);
        CompareDialog->setMinimumSize(QSize(0, 0));
        verticalLayout = new QVBoxLayout(CompareDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        scrollArea = new QScrollArea(CompareDialog);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setMinimumSize(QSize(0, 0));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1744, 1023));
        scrollAreaWidgetContents->setMinimumSize(QSize(1740, 1014));
        viewGroupBox = new QGroupBox(scrollAreaWidgetContents);
        viewGroupBox->setObjectName(QString::fromUtf8("viewGroupBox"));
        viewGroupBox->setGeometry(QRect(946, -1, 182, 51));
        viewBladderCheckboxShadowLabel = new QLabel(viewGroupBox);
        viewBladderCheckboxShadowLabel->setObjectName(QString::fromUtf8("viewBladderCheckboxShadowLabel"));
        viewBladderCheckboxShadowLabel->setGeometry(QRect(31, 37, 46, 13));
        layoutWidget = new QWidget(viewGroupBox);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(11, 12, 161, 42));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        viewRectumCheckBox = new QCheckBox(layoutWidget);
        viewRectumCheckBox->setObjectName(QString::fromUtf8("viewRectumCheckBox"));

        gridLayout->addWidget(viewRectumCheckBox, 0, 0, 1, 1);

        viewPTVCheckBox = new QCheckBox(layoutWidget);
        viewPTVCheckBox->setObjectName(QString::fromUtf8("viewPTVCheckBox"));

        gridLayout->addWidget(viewPTVCheckBox, 0, 1, 1, 1);

        viewBladderCheckBox = new QCheckBox(layoutWidget);
        viewBladderCheckBox->setObjectName(QString::fromUtf8("viewBladderCheckBox"));

        gridLayout->addWidget(viewBladderCheckBox, 1, 0, 1, 1);

        viewFemoralHeadsCheckBox = new QCheckBox(layoutWidget);
        viewFemoralHeadsCheckBox->setObjectName(QString::fromUtf8("viewFemoralHeadsCheckBox"));

        gridLayout->addWidget(viewFemoralHeadsCheckBox, 1, 1, 1, 1);

        projectionDisplayGroupBox = new QGroupBox(scrollAreaWidgetContents);
        projectionDisplayGroupBox->setObjectName(QString::fromUtf8("projectionDisplayGroupBox"));
        projectionDisplayGroupBox->setGeometry(QRect(565, -1, 375, 51));
        horizontalLayout_4 = new QHBoxLayout(projectionDisplayGroupBox);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        transparencyLabel = new QLabel(projectionDisplayGroupBox);
        transparencyLabel->setObjectName(QString::fromUtf8("transparencyLabel"));

        horizontalLayout_4->addWidget(transparencyLabel);

        transparencySpinBox = new QSpinBox(projectionDisplayGroupBox);
        transparencySpinBox->setObjectName(QString::fromUtf8("transparencySpinBox"));
        transparencySpinBox->setMinimum(0);
        transparencySpinBox->setMaximum(100);

        horizontalLayout_4->addWidget(transparencySpinBox);

        transparencySlider = new QSlider(projectionDisplayGroupBox);
        transparencySlider->setObjectName(QString::fromUtf8("transparencySlider"));
        transparencySlider->setMinimum(0);
        transparencySlider->setMaximum(100);
        transparencySlider->setPageStep(20);
        transparencySlider->setValue(0);
        transparencySlider->setOrientation(Qt::Horizontal);
        transparencySlider->setTickPosition(QSlider::TicksAbove);
        transparencySlider->setTickInterval(20);

        horizontalLayout_4->addWidget(transparencySlider);

        maxTransparencyLabel = new QLabel(projectionDisplayGroupBox);
        maxTransparencyLabel->setObjectName(QString::fromUtf8("maxTransparencyLabel"));

        horizontalLayout_4->addWidget(maxTransparencyLabel);

        gantryAnglePushButton = new QPushButton(projectionDisplayGroupBox);
        gantryAnglePushButton->setObjectName(QString::fromUtf8("gantryAnglePushButton"));

        horizontalLayout_4->addWidget(gantryAnglePushButton);

        transparencyLabel->raise();
        transparencySpinBox->raise();
        transparencySlider->raise();
        maxTransparencyLabel->raise();
        gantryAnglePushButton->raise();
        ctAxisGroupBox = new QGroupBox(scrollAreaWidgetContents);
        ctAxisGroupBox->setObjectName(QString::fromUtf8("ctAxisGroupBox"));
        ctAxisGroupBox->setGeometry(QRect(94, -1, 465, 51));
        horizontalLayout_3 = new QHBoxLayout(ctAxisGroupBox);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        axialRadioButton = new QRadioButton(ctAxisGroupBox);
        axialRadioButton->setObjectName(QString::fromUtf8("axialRadioButton"));
        axialRadioButton->setChecked(true);

        horizontalLayout_3->addWidget(axialRadioButton);

        sagittalRadioButton = new QRadioButton(ctAxisGroupBox);
        sagittalRadioButton->setObjectName(QString::fromUtf8("sagittalRadioButton"));

        horizontalLayout_3->addWidget(sagittalRadioButton);

        coronalRadioButton = new QRadioButton(ctAxisGroupBox);
        coronalRadioButton->setObjectName(QString::fromUtf8("coronalRadioButton"));

        horizontalLayout_3->addWidget(coronalRadioButton);

        sliceSelectionLabel = new QLabel(ctAxisGroupBox);
        sliceSelectionLabel->setObjectName(QString::fromUtf8("sliceSelectionLabel"));

        horizontalLayout_3->addWidget(sliceSelectionLabel);

        sliceSelectionSlider = new QSlider(ctAxisGroupBox);
        sliceSelectionSlider->setObjectName(QString::fromUtf8("sliceSelectionSlider"));
        sliceSelectionSlider->setMinimum(0);
        sliceSelectionSlider->setMaximum(100);
        sliceSelectionSlider->setPageStep(10);
        sliceSelectionSlider->setValue(0);
        sliceSelectionSlider->setOrientation(Qt::Horizontal);
        sliceSelectionSlider->setTickPosition(QSlider::TicksAbove);
        sliceSelectionSlider->setTickInterval(20);

        horizontalLayout_3->addWidget(sliceSelectionSlider);

        isocenterPushButton = new QPushButton(ctAxisGroupBox);
        isocenterPushButton->setObjectName(QString::fromUtf8("isocenterPushButton"));

        horizontalLayout_3->addWidget(isocenterPushButton);

        autoPlayPushButton = new QPushButton(ctAxisGroupBox);
        autoPlayPushButton->setObjectName(QString::fromUtf8("autoPlayPushButton"));
        autoPlayPushButton->setFocusPolicy(Qt::NoFocus);

        horizontalLayout_3->addWidget(autoPlayPushButton);

        layoutWidget1 = new QWidget(scrollAreaWidgetContents);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(9, 990, 1688, 25));
        bottomBtnsHLayout = new QHBoxLayout(layoutWidget1);
        bottomBtnsHLayout->setObjectName(QString::fromUtf8("bottomBtnsHLayout"));
        bottomBtnsHLayout->setContentsMargins(0, 0, 0, 0);
        historyPushButton = new QPushButton(layoutWidget1);
        historyPushButton->setObjectName(QString::fromUtf8("historyPushButton"));

        bottomBtnsHLayout->addWidget(historyPushButton);

        removeCurrentMatchPushButton = new QPushButton(layoutWidget1);
        removeCurrentMatchPushButton->setObjectName(QString::fromUtf8("removeCurrentMatchPushButton"));

        bottomBtnsHLayout->addWidget(removeCurrentMatchPushButton);

        horizontalSpacer = new QSpacerItem(1440, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        bottomBtnsHLayout->addItem(horizontalSpacer);

        buttonBox = new QDialogButtonBox(layoutWidget1);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        bottomBtnsHLayout->addWidget(buttonBox);

        DVHGroupBox = new QGroupBox(scrollAreaWidgetContents);
        DVHGroupBox->setObjectName(QString::fromUtf8("DVHGroupBox"));
        DVHGroupBox->setGeometry(QRect(1214, -1, 295, 51));
        horizontalLayout_5 = new QHBoxLayout(DVHGroupBox);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        overlayDVHCheckBox = new QCheckBox(DVHGroupBox);
        overlayDVHCheckBox->setObjectName(QString::fromUtf8("overlayDVHCheckBox"));

        horizontalLayout_5->addWidget(overlayDVHCheckBox);

        overlaySelectionPushButton = new QPushButton(DVHGroupBox);
        overlaySelectionPushButton->setObjectName(QString::fromUtf8("overlaySelectionPushButton"));
        overlaySelectionPushButton->setMinimumSize(QSize(91, 0));

        horizontalLayout_5->addWidget(overlaySelectionPushButton);

        removeSelectedOverlayPushButton = new QPushButton(DVHGroupBox);
        removeSelectedOverlayPushButton->setObjectName(QString::fromUtf8("removeSelectedOverlayPushButton"));
        removeSelectedOverlayPushButton->setMinimumSize(QSize(91, 0));

        horizontalLayout_5->addWidget(removeSelectedOverlayPushButton);

        widget = new QWidget(scrollAreaWidgetContents);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(10, 56, 1686, 932));
        verticalLayout_2 = new QVBoxLayout(widget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        queryGroupBox = new QGroupBox(widget);
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
        sliceSelectionSpinBox = new QSpinBox(queryGroupBox);
        sliceSelectionSpinBox->setObjectName(QString::fromUtf8("sliceSelectionSpinBox"));
        sliceSelectionSpinBox->setGeometry(QRect(22, 40, 39, 20));
        sliceSelectionSpinBox->setMinimum(0);
        sliceSelectionSpinBox->setMaximum(512);
        maxSliceLabel = new QLabel(queryGroupBox);
        maxSliceLabel->setObjectName(QString::fromUtf8("maxSliceLabel"));
        maxSliceLabel->setGeometry(QRect(31, 43, 20, 16));
        originCheckBox = new QCheckBox(queryGroupBox);
        originCheckBox->setObjectName(QString::fromUtf8("originCheckBox"));
        originCheckBox->setGeometry(QRect(21, 44, 51, 17));
        flatShadedCheckBox = new QCheckBox(queryGroupBox);
        flatShadedCheckBox->setObjectName(QString::fromUtf8("flatShadedCheckBox"));
        flatShadedCheckBox->setGeometry(QRect(8, 48, 79, 17));

        horizontalLayout->addWidget(queryGroupBox);

        queryCTImageWidget = new QVTKWidget(widget);
        queryCTImageWidget->setObjectName(QString::fromUtf8("queryCTImageWidget"));
        queryCTImageWidget->setMinimumSize(QSize(460, 460));
        queryCTImageWidget->setFont(font1);
        queryCTImageWidget->setAutoFillBackground(true);
        dvhLabel = new QLabel(queryCTImageWidget);
        dvhLabel->setObjectName(QString::fromUtf8("dvhLabel"));
        dvhLabel->setGeometry(QRect(-78, 40, 25, 25));
        dvhLabel->setFont(font);

        horizontalLayout->addWidget(queryCTImageWidget);

        queryProjectionWidget = new QVTKWidget(widget);
        queryProjectionWidget->setObjectName(QString::fromUtf8("queryProjectionWidget"));
        queryProjectionWidget->setMinimumSize(QSize(460, 460));
        queryProjectionWidget->setAutoFillBackground(true);

        horizontalLayout->addWidget(queryProjectionWidget);

        overlayDVHWidget = new QVTKWidget(widget);
        overlayDVHWidget->setObjectName(QString::fromUtf8("overlayDVHWidget"));
        overlayDVHWidget->setMinimumSize(QSize(663, 460));
        overlayDVHWidget->setAutoFillBackground(true);

        horizontalLayout->addWidget(overlayDVHWidget);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        matchGroupBox = new QGroupBox(widget);
        matchGroupBox->setObjectName(QString::fromUtf8("matchGroupBox"));
        matchGroupBox->setMinimumSize(QSize(79, 460));
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


        horizontalLayout_2->addWidget(matchGroupBox);

        matchCTImageWidget = new QVTKWidget(widget);
        matchCTImageWidget->setObjectName(QString::fromUtf8("matchCTImageWidget"));
        matchCTImageWidget->setMinimumSize(QSize(460, 460));
        matchCTImageWidget->setAutoFillBackground(true);

        horizontalLayout_2->addWidget(matchCTImageWidget);

        matchProjectionWidget = new QVTKWidget(widget);
        matchProjectionWidget->setObjectName(QString::fromUtf8("matchProjectionWidget"));
        matchProjectionWidget->setMinimumSize(QSize(460, 460));
        matchProjectionWidget->setAutoFillBackground(true);

        horizontalLayout_2->addWidget(matchProjectionWidget);

        matchDVHWidget = new QVTKWidget(widget);
        matchDVHWidget->setObjectName(QString::fromUtf8("matchDVHWidget"));
        matchDVHWidget->setMinimumSize(QSize(663, 460));
        matchDVHWidget->setAutoFillBackground(true);

        horizontalLayout_2->addWidget(matchDVHWidget);


        verticalLayout_2->addLayout(horizontalLayout_2);

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
        viewGroupBox->setTitle(QApplication::translate("CompareDialog", "View", 0, QApplication::UnicodeUTF8));
        viewBladderCheckboxShadowLabel->setText(QApplication::translate("CompareDialog", "Bladder", 0, QApplication::UnicodeUTF8));
        viewRectumCheckBox->setText(QApplication::translate("CompareDialog", "Rectum", 0, QApplication::UnicodeUTF8));
        viewPTVCheckBox->setText(QApplication::translate("CompareDialog", "PTV", 0, QApplication::UnicodeUTF8));
        viewBladderCheckBox->setText(QApplication::translate("CompareDialog", "Bladder", 0, QApplication::UnicodeUTF8));
        viewFemoralHeadsCheckBox->setText(QApplication::translate("CompareDialog", "Femoral Heads", 0, QApplication::UnicodeUTF8));
        projectionDisplayGroupBox->setTitle(QApplication::translate("CompareDialog", "Beam's Eye View Projection", 0, QApplication::UnicodeUTF8));
        transparencyLabel->setText(QApplication::translate("CompareDialog", "%Transparency", 0, QApplication::UnicodeUTF8));
        maxTransparencyLabel->setText(QApplication::translate("CompareDialog", "100", 0, QApplication::UnicodeUTF8));
        gantryAnglePushButton->setText(QApplication::translate("CompareDialog", "Gantry Angle", 0, QApplication::UnicodeUTF8));
        ctAxisGroupBox->setTitle(QApplication::translate("CompareDialog", "CT", 0, QApplication::UnicodeUTF8));
        axialRadioButton->setText(QApplication::translate("CompareDialog", "Axial", 0, QApplication::UnicodeUTF8));
        sagittalRadioButton->setText(QApplication::translate("CompareDialog", "Sagittal", 0, QApplication::UnicodeUTF8));
        coronalRadioButton->setText(QApplication::translate("CompareDialog", "Coronal", 0, QApplication::UnicodeUTF8));
        sliceSelectionLabel->setText(QApplication::translate("CompareDialog", "Z slice:", 0, QApplication::UnicodeUTF8));
        isocenterPushButton->setText(QApplication::translate("CompareDialog", "Isocenter", 0, QApplication::UnicodeUTF8));
        autoPlayPushButton->setText(QApplication::translate("CompareDialog", "Auto-play", 0, QApplication::UnicodeUTF8));
        historyPushButton->setText(QApplication::translate("CompareDialog", "History", 0, QApplication::UnicodeUTF8));
        removeCurrentMatchPushButton->setText(QApplication::translate("CompareDialog", "Remove Current", 0, QApplication::UnicodeUTF8));
        DVHGroupBox->setTitle(QApplication::translate("CompareDialog", "Dose Volume Histogram", 0, QApplication::UnicodeUTF8));
        overlayDVHCheckBox->setText(QApplication::translate("CompareDialog", " Overlay On", 0, QApplication::UnicodeUTF8));
        overlaySelectionPushButton->setText(QApplication::translate("CompareDialog", "Select Overlay", 0, QApplication::UnicodeUTF8));
        removeSelectedOverlayPushButton->setText(QApplication::translate("CompareDialog", "Remove Selected", 0, QApplication::UnicodeUTF8));
        queryGroupBox->setTitle(QApplication::translate("CompareDialog", "Query", 0, QApplication::UnicodeUTF8));
        queryInstitutionPushButton->setText(QApplication::translate("CompareDialog", "Duke", 0, QApplication::UnicodeUTF8));
        queryCaseInstitutionLabel->setText(QApplication::translate("CompareDialog", "Duke", 0, QApplication::UnicodeUTF8));
        queryCaseNumberLabel->setText(QApplication::translate("CompareDialog", "#666", 0, QApplication::UnicodeUTF8));
        querySelectSpinBox->setPrefix(QString());
        maxSliceLabel->setText(QApplication::translate("CompareDialog", "max", 0, QApplication::UnicodeUTF8));
        originCheckBox->setText(QApplication::translate("CompareDialog", "Axes", 0, QApplication::UnicodeUTF8));
        flatShadedCheckBox->setText(QApplication::translate("CompareDialog", "Flat shaded", 0, QApplication::UnicodeUTF8));
        dvhLabel->setText(QApplication::translate("CompareDialog", "Dose Volume Histogram", 0, QApplication::UnicodeUTF8));
        matchGroupBox->setTitle(QApplication::translate("CompareDialog", "Match", 0, QApplication::UnicodeUTF8));
        matchInstitutionPushButton->setText(QApplication::translate("CompareDialog", "Duke", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CompareDialog: public Ui_CompareDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMPAREDIALOG_H
