/********************************************************************************
** Form generated from reading UI file 'CaseSpaceDialog.ui'
**
** Created: Thu Nov 3 16:04:49 2011
**      by: Qt User Interface Compiler version 4.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CASESPACEDIALOG_H
#define UI_CASESPACEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
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
#include <QtGui/QWidget>
#include "qvtkwidget.h"

QT_BEGIN_NAMESPACE

class Ui_CaseSpaceDialog
{
public:
    QHBoxLayout *horizontalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVTKWidget *caseSpaceViewWidget;
    QGroupBox *caseDataGroupBox;
    QGridLayout *gridLayout;
    QLabel *queryCaseLabel;
    QLabel *queryCaseNameLabel;
    QSpacerItem *horizontalSpacer_2;
    QLabel *matchCaseLabel;
    QLabel *matchCaseNameLabel;
    QSpacerItem *horizontalSpacer_3;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_7;
    QHBoxLayout *compareTestButtonsLayout;
    QPushButton *compareCasesPushButton;
    QPushButton *testPushButton;
    QSpacerItem *horizontalSpacer;
    QDialogButtonBox *buttonBox;
    QGroupBox *selectOverlapDataGroupBox;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *XValuesPushButton;
    QPushButton *YValuesPushButton;
    QPushButton *XYDataAnglePushButton;
    QGroupBox *backgroundColorGroupBox;
    QHBoxLayout *horizontalLayout_4;
    QRadioButton *backgroundBlackRadioButton;
    QRadioButton *backgroundWhiteRadioButton;
    QRadioButton *backgroundRampRadioButton;
    QGroupBox *MIRangeGroupBox;
    QHBoxLayout *horizontalLayout_6;
    QLabel *MIMinLabel;
    QSlider *MIRangeSlider;
    QLabel *MIMaxLabel;
    QGroupBox *selectViewGroupBox;
    QRadioButton *XYViewRadioButton;
    QRadioButton *XMIViewRadioButton;
    QRadioButton *YMIViewRadioButton;
    QRadioButton *obliqueViewRadioButton;
    QPushButton *resetViewPushButton;

    void setupUi(QDialog *CaseSpaceDialog)
    {
        if (CaseSpaceDialog->objectName().isEmpty())
            CaseSpaceDialog->setObjectName(QString::fromUtf8("CaseSpaceDialog"));
        CaseSpaceDialog->resize(962, 692);
        horizontalLayout = new QHBoxLayout(CaseSpaceDialog);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        scrollArea = new QScrollArea(CaseSpaceDialog);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setEnabled(true);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 942, 672));
        scrollAreaWidgetContents->setMinimumSize(QSize(935, 660));
        caseSpaceViewWidget = new QVTKWidget(scrollAreaWidgetContents);
        caseSpaceViewWidget->setObjectName(QString::fromUtf8("caseSpaceViewWidget"));
        caseSpaceViewWidget->setGeometry(QRect(10, 62, 920, 569));
        caseSpaceViewWidget->setMinimumSize(QSize(920, 569));
        caseDataGroupBox = new QGroupBox(caseSpaceViewWidget);
        caseDataGroupBox->setObjectName(QString::fromUtf8("caseDataGroupBox"));
        caseDataGroupBox->setGeometry(QRect(1, 2, 566, 62));
        gridLayout = new QGridLayout(caseDataGroupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        queryCaseLabel = new QLabel(caseDataGroupBox);
        queryCaseLabel->setObjectName(QString::fromUtf8("queryCaseLabel"));
        queryCaseLabel->setAutoFillBackground(false);

        gridLayout->addWidget(queryCaseLabel, 0, 0, 2, 1);

        queryCaseNameLabel = new QLabel(caseDataGroupBox);
        queryCaseNameLabel->setObjectName(QString::fromUtf8("queryCaseNameLabel"));

        gridLayout->addWidget(queryCaseNameLabel, 0, 1, 2, 1);

        horizontalSpacer_2 = new QSpacerItem(422, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 2, 2, 2);

        matchCaseLabel = new QLabel(caseDataGroupBox);
        matchCaseLabel->setObjectName(QString::fromUtf8("matchCaseLabel"));

        gridLayout->addWidget(matchCaseLabel, 2, 0, 1, 1);

        matchCaseNameLabel = new QLabel(caseDataGroupBox);
        matchCaseNameLabel->setObjectName(QString::fromUtf8("matchCaseNameLabel"));

        gridLayout->addWidget(matchCaseNameLabel, 2, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(382, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 1, 3, 2, 1);

        layoutWidget = new QWidget(scrollAreaWidgetContents);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 635, 920, 27));
        horizontalLayout_7 = new QHBoxLayout(layoutWidget);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        compareTestButtonsLayout = new QHBoxLayout();
        compareTestButtonsLayout->setObjectName(QString::fromUtf8("compareTestButtonsLayout"));
        compareCasesPushButton = new QPushButton(layoutWidget);
        compareCasesPushButton->setObjectName(QString::fromUtf8("compareCasesPushButton"));

        compareTestButtonsLayout->addWidget(compareCasesPushButton);

        testPushButton = new QPushButton(layoutWidget);
        testPushButton->setObjectName(QString::fromUtf8("testPushButton"));

        compareTestButtonsLayout->addWidget(testPushButton);


        horizontalLayout_7->addLayout(compareTestButtonsLayout);

        horizontalSpacer = new QSpacerItem(1274, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer);

        buttonBox = new QDialogButtonBox(layoutWidget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        horizontalLayout_7->addWidget(buttonBox);

        selectOverlapDataGroupBox = new QGroupBox(scrollAreaWidgetContents);
        selectOverlapDataGroupBox->setObjectName(QString::fromUtf8("selectOverlapDataGroupBox"));
        selectOverlapDataGroupBox->setGeometry(QRect(10, 2, 220, 56));
        horizontalLayout_2 = new QHBoxLayout(selectOverlapDataGroupBox);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        XValuesPushButton = new QPushButton(selectOverlapDataGroupBox);
        XValuesPushButton->setObjectName(QString::fromUtf8("XValuesPushButton"));
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(XValuesPushButton->sizePolicy().hasHeightForWidth());
        XValuesPushButton->setSizePolicy(sizePolicy);
        XValuesPushButton->setMinimumSize(QSize(65, 0));

        horizontalLayout_2->addWidget(XValuesPushButton);

        YValuesPushButton = new QPushButton(selectOverlapDataGroupBox);
        YValuesPushButton->setObjectName(QString::fromUtf8("YValuesPushButton"));
        sizePolicy.setHeightForWidth(YValuesPushButton->sizePolicy().hasHeightForWidth());
        YValuesPushButton->setSizePolicy(sizePolicy);
        YValuesPushButton->setMinimumSize(QSize(65, 0));

        horizontalLayout_2->addWidget(YValuesPushButton);

        XYDataAnglePushButton = new QPushButton(selectOverlapDataGroupBox);
        XYDataAnglePushButton->setObjectName(QString::fromUtf8("XYDataAnglePushButton"));
        sizePolicy.setHeightForWidth(XYDataAnglePushButton->sizePolicy().hasHeightForWidth());
        XYDataAnglePushButton->setSizePolicy(sizePolicy);
        XYDataAnglePushButton->setMinimumSize(QSize(65, 0));

        horizontalLayout_2->addWidget(XYDataAnglePushButton);

        backgroundColorGroupBox = new QGroupBox(scrollAreaWidgetContents);
        backgroundColorGroupBox->setObjectName(QString::fromUtf8("backgroundColorGroupBox"));
        backgroundColorGroupBox->setGeometry(QRect(234, 2, 175, 56));
        horizontalLayout_4 = new QHBoxLayout(backgroundColorGroupBox);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        backgroundBlackRadioButton = new QRadioButton(backgroundColorGroupBox);
        backgroundBlackRadioButton->setObjectName(QString::fromUtf8("backgroundBlackRadioButton"));
        backgroundBlackRadioButton->setChecked(true);

        horizontalLayout_4->addWidget(backgroundBlackRadioButton);

        backgroundWhiteRadioButton = new QRadioButton(backgroundColorGroupBox);
        backgroundWhiteRadioButton->setObjectName(QString::fromUtf8("backgroundWhiteRadioButton"));

        horizontalLayout_4->addWidget(backgroundWhiteRadioButton);

        backgroundRampRadioButton = new QRadioButton(backgroundColorGroupBox);
        backgroundRampRadioButton->setObjectName(QString::fromUtf8("backgroundRampRadioButton"));

        horizontalLayout_4->addWidget(backgroundRampRadioButton);

        MIRangeGroupBox = new QGroupBox(scrollAreaWidgetContents);
        MIRangeGroupBox->setObjectName(QString::fromUtf8("MIRangeGroupBox"));
        MIRangeGroupBox->setGeometry(QRect(724, 2, 206, 56));
        horizontalLayout_6 = new QHBoxLayout(MIRangeGroupBox);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        MIMinLabel = new QLabel(MIRangeGroupBox);
        MIMinLabel->setObjectName(QString::fromUtf8("MIMinLabel"));
        MIMinLabel->setMinimumSize(QSize(40, 0));

        horizontalLayout_6->addWidget(MIMinLabel);

        MIRangeSlider = new QSlider(MIRangeGroupBox);
        MIRangeSlider->setObjectName(QString::fromUtf8("MIRangeSlider"));
        MIRangeSlider->setMaximum(1000);
        MIRangeSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_6->addWidget(MIRangeSlider);

        MIMaxLabel = new QLabel(MIRangeGroupBox);
        MIMaxLabel->setObjectName(QString::fromUtf8("MIMaxLabel"));
        MIMaxLabel->setMinimumSize(QSize(40, 0));

        horizontalLayout_6->addWidget(MIMaxLabel);

        selectViewGroupBox = new QGroupBox(scrollAreaWidgetContents);
        selectViewGroupBox->setObjectName(QString::fromUtf8("selectViewGroupBox"));
        selectViewGroupBox->setGeometry(QRect(414, 2, 306, 56));
        XYViewRadioButton = new QRadioButton(selectViewGroupBox);
        XYViewRadioButton->setObjectName(QString::fromUtf8("XYViewRadioButton"));
        XYViewRadioButton->setGeometry(QRect(10, 26, 35, 17));
        XMIViewRadioButton = new QRadioButton(selectViewGroupBox);
        XMIViewRadioButton->setObjectName(QString::fromUtf8("XMIViewRadioButton"));
        XMIViewRadioButton->setGeometry(QRect(63, 26, 45, 17));
        YMIViewRadioButton = new QRadioButton(selectViewGroupBox);
        YMIViewRadioButton->setObjectName(QString::fromUtf8("YMIViewRadioButton"));
        YMIViewRadioButton->setGeometry(QRect(117, 26, 44, 17));
        obliqueViewRadioButton = new QRadioButton(selectViewGroupBox);
        obliqueViewRadioButton->setObjectName(QString::fromUtf8("obliqueViewRadioButton"));
        obliqueViewRadioButton->setGeometry(QRect(170, 26, 59, 17));
        obliqueViewRadioButton->setChecked(true);
        resetViewPushButton = new QPushButton(selectViewGroupBox);
        resetViewPushButton->setObjectName(QString::fromUtf8("resetViewPushButton"));
        resetViewPushButton->setGeometry(QRect(235, 23, 65, 23));
        sizePolicy.setHeightForWidth(resetViewPushButton->sizePolicy().hasHeightForWidth());
        resetViewPushButton->setSizePolicy(sizePolicy);
        resetViewPushButton->setMinimumSize(QSize(65, 0));
        scrollArea->setWidget(scrollAreaWidgetContents);
        selectOverlapDataGroupBox->raise();
        backgroundColorGroupBox->raise();
        MIRangeGroupBox->raise();
        selectViewGroupBox->raise();
        caseSpaceViewWidget->raise();
        layoutWidget->raise();

        horizontalLayout->addWidget(scrollArea);


        retranslateUi(CaseSpaceDialog);

        QMetaObject::connectSlotsByName(CaseSpaceDialog);
    } // setupUi

    void retranslateUi(QDialog *CaseSpaceDialog)
    {
        CaseSpaceDialog->setWindowTitle(QApplication::translate("CaseSpaceDialog", "Case Space", 0, QApplication::UnicodeUTF8));
        caseDataGroupBox->setTitle(QString());
        queryCaseLabel->setText(QApplication::translate("CaseSpaceDialog", "Query:", 0, QApplication::UnicodeUTF8));
        queryCaseNameLabel->setText(QApplication::translate("CaseSpaceDialog", "<not selected>", 0, QApplication::UnicodeUTF8));
        matchCaseLabel->setText(QApplication::translate("CaseSpaceDialog", "Match:", 0, QApplication::UnicodeUTF8));
        matchCaseNameLabel->setText(QApplication::translate("CaseSpaceDialog", "<not selected>", 0, QApplication::UnicodeUTF8));
        compareCasesPushButton->setText(QApplication::translate("CaseSpaceDialog", "Compare Cases", 0, QApplication::UnicodeUTF8));
        testPushButton->setText(QApplication::translate("CaseSpaceDialog", "Test", 0, QApplication::UnicodeUTF8));
        selectOverlapDataGroupBox->setTitle(QApplication::translate("CaseSpaceDialog", "Select XY Data", 0, QApplication::UnicodeUTF8));
        XValuesPushButton->setText(QApplication::translate("CaseSpaceDialog", "X Values", 0, QApplication::UnicodeUTF8));
        YValuesPushButton->setText(QApplication::translate("CaseSpaceDialog", "Y Values", 0, QApplication::UnicodeUTF8));
        XYDataAnglePushButton->setText(QApplication::translate("CaseSpaceDialog", "Angle", 0, QApplication::UnicodeUTF8));
        backgroundColorGroupBox->setTitle(QApplication::translate("CaseSpaceDialog", "Background", 0, QApplication::UnicodeUTF8));
        backgroundBlackRadioButton->setText(QApplication::translate("CaseSpaceDialog", "black", 0, QApplication::UnicodeUTF8));
        backgroundWhiteRadioButton->setText(QApplication::translate("CaseSpaceDialog", "white", 0, QApplication::UnicodeUTF8));
        backgroundRampRadioButton->setText(QApplication::translate("CaseSpaceDialog", "ramp", 0, QApplication::UnicodeUTF8));
        MIRangeGroupBox->setTitle(QApplication::translate("CaseSpaceDialog", "MI Range", 0, QApplication::UnicodeUTF8));
        MIMinLabel->setText(QApplication::translate("CaseSpaceDialog", "Minimum", 0, QApplication::UnicodeUTF8));
        MIMaxLabel->setText(QApplication::translate("CaseSpaceDialog", "Maximum", 0, QApplication::UnicodeUTF8));
        selectViewGroupBox->setTitle(QApplication::translate("CaseSpaceDialog", "Select View", 0, QApplication::UnicodeUTF8));
        XYViewRadioButton->setText(QApplication::translate("CaseSpaceDialog", "XY", 0, QApplication::UnicodeUTF8));
        XMIViewRadioButton->setText(QApplication::translate("CaseSpaceDialog", "X-MI", 0, QApplication::UnicodeUTF8));
        YMIViewRadioButton->setText(QApplication::translate("CaseSpaceDialog", "Y-MI", 0, QApplication::UnicodeUTF8));
        obliqueViewRadioButton->setText(QApplication::translate("CaseSpaceDialog", "Oblique", 0, QApplication::UnicodeUTF8));
        resetViewPushButton->setText(QApplication::translate("CaseSpaceDialog", "Reset", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CaseSpaceDialog: public Ui_CaseSpaceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CASESPACEDIALOG_H
