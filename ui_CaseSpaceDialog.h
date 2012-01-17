/********************************************************************************
** Form generated from reading UI file 'CaseSpaceDialog.ui'
**
** Created: Tue Jan 17 10:47:17 2012
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
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSlider>
#include <QtGui/QSplitter>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "qvtkwidget.h"

QT_BEGIN_NAMESPACE

class Ui_CaseSpaceDialog
{
public:
    QHBoxLayout *horizontalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_5;
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
    QGroupBox *selectViewGroupBox;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *XYViewRadioButton;
    QRadioButton *XMIViewRadioButton;
    QRadioButton *YMIViewRadioButton;
    QRadioButton *obliqueViewRadioButton;
    QPushButton *resetViewPushButton;
    QGroupBox *MIRangeGroupBox;
    QHBoxLayout *horizontalLayout_6;
    QLabel *MIMinLabel;
    QSlider *MIRangeSlider;
    QLabel *MIMaxLabel;
    QVTKWidget *caseSpaceViewWidget;
    QSplitter *splitter;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout;
    QLabel *queryCaseLabel;
    QLabel *matchCaseLabel;
    QWidget *layoutWidget2;
    QVBoxLayout *verticalLayout_2;
    QLabel *queryCaseNameLabel;
    QLabel *matchCaseNameLabel;
    QGroupBox *viewMIThresholdIndicatorsGroupBox;
    QCheckBox *viewAxisPlanesCheckBox;
    QCheckBox *viewThresholdAxesCheckBox;
    QCheckBox *viewThresholdPlaneCheckBox;
    QDialogButtonBox *buttonBox;
    QPushButton *compareCasesPushButton;
    QPushButton *testPushButton;

    void setupUi(QDialog *CaseSpaceDialog)
    {
        if (CaseSpaceDialog->objectName().isEmpty())
            CaseSpaceDialog->setObjectName(QString::fromUtf8("CaseSpaceDialog"));
        CaseSpaceDialog->resize(1522, 1030);
        horizontalLayout = new QHBoxLayout(CaseSpaceDialog);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        scrollArea = new QScrollArea(CaseSpaceDialog);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1502, 1010));
        scrollAreaWidgetContents->setMinimumSize(QSize(1495, 990));
        layoutWidget = new QWidget(scrollAreaWidgetContents);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(452, 6, 1032, 58));
        horizontalLayout_5 = new QHBoxLayout(layoutWidget);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        selectOverlapDataGroupBox = new QGroupBox(layoutWidget);
        selectOverlapDataGroupBox->setObjectName(QString::fromUtf8("selectOverlapDataGroupBox"));
        horizontalLayout_2 = new QHBoxLayout(selectOverlapDataGroupBox);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        XValuesPushButton = new QPushButton(selectOverlapDataGroupBox);
        XValuesPushButton->setObjectName(QString::fromUtf8("XValuesPushButton"));

        horizontalLayout_2->addWidget(XValuesPushButton);

        YValuesPushButton = new QPushButton(selectOverlapDataGroupBox);
        YValuesPushButton->setObjectName(QString::fromUtf8("YValuesPushButton"));

        horizontalLayout_2->addWidget(YValuesPushButton);

        XYDataAnglePushButton = new QPushButton(selectOverlapDataGroupBox);
        XYDataAnglePushButton->setObjectName(QString::fromUtf8("XYDataAnglePushButton"));

        horizontalLayout_2->addWidget(XYDataAnglePushButton);


        horizontalLayout_5->addWidget(selectOverlapDataGroupBox);

        backgroundColorGroupBox = new QGroupBox(layoutWidget);
        backgroundColorGroupBox->setObjectName(QString::fromUtf8("backgroundColorGroupBox"));
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


        horizontalLayout_5->addWidget(backgroundColorGroupBox);

        selectViewGroupBox = new QGroupBox(layoutWidget);
        selectViewGroupBox->setObjectName(QString::fromUtf8("selectViewGroupBox"));
        horizontalLayout_3 = new QHBoxLayout(selectViewGroupBox);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        XYViewRadioButton = new QRadioButton(selectViewGroupBox);
        XYViewRadioButton->setObjectName(QString::fromUtf8("XYViewRadioButton"));

        horizontalLayout_3->addWidget(XYViewRadioButton);

        XMIViewRadioButton = new QRadioButton(selectViewGroupBox);
        XMIViewRadioButton->setObjectName(QString::fromUtf8("XMIViewRadioButton"));

        horizontalLayout_3->addWidget(XMIViewRadioButton);

        YMIViewRadioButton = new QRadioButton(selectViewGroupBox);
        YMIViewRadioButton->setObjectName(QString::fromUtf8("YMIViewRadioButton"));

        horizontalLayout_3->addWidget(YMIViewRadioButton);

        obliqueViewRadioButton = new QRadioButton(selectViewGroupBox);
        obliqueViewRadioButton->setObjectName(QString::fromUtf8("obliqueViewRadioButton"));
        obliqueViewRadioButton->setChecked(true);

        horizontalLayout_3->addWidget(obliqueViewRadioButton);

        resetViewPushButton = new QPushButton(selectViewGroupBox);
        resetViewPushButton->setObjectName(QString::fromUtf8("resetViewPushButton"));

        horizontalLayout_3->addWidget(resetViewPushButton);


        horizontalLayout_5->addWidget(selectViewGroupBox);

        MIRangeGroupBox = new QGroupBox(layoutWidget);
        MIRangeGroupBox->setObjectName(QString::fromUtf8("MIRangeGroupBox"));
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


        horizontalLayout_5->addWidget(MIRangeGroupBox);

        caseSpaceViewWidget = new QVTKWidget(scrollAreaWidgetContents);
        caseSpaceViewWidget->setObjectName(QString::fromUtf8("caseSpaceViewWidget"));
        caseSpaceViewWidget->setGeometry(QRect(10, 68, 1473, 897));
        caseSpaceViewWidget->setMinimumSize(QSize(920, 569));
        splitter = new QSplitter(scrollAreaWidgetContents);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setGeometry(QRect(10, 8, 427, 34));
        splitter->setOrientation(Qt::Horizontal);
        layoutWidget1 = new QWidget(splitter);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        verticalLayout = new QVBoxLayout(layoutWidget1);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        queryCaseLabel = new QLabel(layoutWidget1);
        queryCaseLabel->setObjectName(QString::fromUtf8("queryCaseLabel"));

        verticalLayout->addWidget(queryCaseLabel);

        matchCaseLabel = new QLabel(layoutWidget1);
        matchCaseLabel->setObjectName(QString::fromUtf8("matchCaseLabel"));

        verticalLayout->addWidget(matchCaseLabel);

        splitter->addWidget(layoutWidget1);
        layoutWidget2 = new QWidget(splitter);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        verticalLayout_2 = new QVBoxLayout(layoutWidget2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        queryCaseNameLabel = new QLabel(layoutWidget2);
        queryCaseNameLabel->setObjectName(QString::fromUtf8("queryCaseNameLabel"));

        verticalLayout_2->addWidget(queryCaseNameLabel);

        matchCaseNameLabel = new QLabel(layoutWidget2);
        matchCaseNameLabel->setObjectName(QString::fromUtf8("matchCaseNameLabel"));

        verticalLayout_2->addWidget(matchCaseNameLabel);

        splitter->addWidget(layoutWidget2);
        viewMIThresholdIndicatorsGroupBox = new QGroupBox(scrollAreaWidgetContents);
        viewMIThresholdIndicatorsGroupBox->setObjectName(QString::fromUtf8("viewMIThresholdIndicatorsGroupBox"));
        viewMIThresholdIndicatorsGroupBox->setGeometry(QRect(594, 966, 305, 39));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(viewMIThresholdIndicatorsGroupBox->sizePolicy().hasHeightForWidth());
        viewMIThresholdIndicatorsGroupBox->setSizePolicy(sizePolicy);
        viewAxisPlanesCheckBox = new QCheckBox(viewMIThresholdIndicatorsGroupBox);
        viewAxisPlanesCheckBox->setObjectName(QString::fromUtf8("viewAxisPlanesCheckBox"));
        viewAxisPlanesCheckBox->setGeometry(QRect(10, 18, 77, 17));
        viewThresholdAxesCheckBox = new QCheckBox(viewMIThresholdIndicatorsGroupBox);
        viewThresholdAxesCheckBox->setObjectName(QString::fromUtf8("viewThresholdAxesCheckBox"));
        viewThresholdAxesCheckBox->setGeometry(QRect(93, 18, 97, 17));
        viewThresholdPlaneCheckBox = new QCheckBox(viewMIThresholdIndicatorsGroupBox);
        viewThresholdPlaneCheckBox->setObjectName(QString::fromUtf8("viewThresholdPlaneCheckBox"));
        viewThresholdPlaneCheckBox->setGeometry(QRect(196, 18, 99, 17));
        buttonBox = new QDialogButtonBox(scrollAreaWidgetContents);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(1326, 974, 156, 23));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        compareCasesPushButton = new QPushButton(scrollAreaWidgetContents);
        compareCasesPushButton->setObjectName(QString::fromUtf8("compareCasesPushButton"));
        compareCasesPushButton->setGeometry(QRect(12, 974, 83, 23));
        testPushButton = new QPushButton(scrollAreaWidgetContents);
        testPushButton->setObjectName(QString::fromUtf8("testPushButton"));
        testPushButton->setGeometry(QRect(101, 974, 75, 23));
        scrollArea->setWidget(scrollAreaWidgetContents);

        horizontalLayout->addWidget(scrollArea);


        retranslateUi(CaseSpaceDialog);

        QMetaObject::connectSlotsByName(CaseSpaceDialog);
    } // setupUi

    void retranslateUi(QDialog *CaseSpaceDialog)
    {
        CaseSpaceDialog->setWindowTitle(QApplication::translate("CaseSpaceDialog", "Case Space", 0, QApplication::UnicodeUTF8));
        selectOverlapDataGroupBox->setTitle(QApplication::translate("CaseSpaceDialog", "Select XY Data", 0, QApplication::UnicodeUTF8));
        XValuesPushButton->setText(QApplication::translate("CaseSpaceDialog", "X Values", 0, QApplication::UnicodeUTF8));
        YValuesPushButton->setText(QApplication::translate("CaseSpaceDialog", "Y Values", 0, QApplication::UnicodeUTF8));
        XYDataAnglePushButton->setText(QApplication::translate("CaseSpaceDialog", "Angle", 0, QApplication::UnicodeUTF8));
        backgroundColorGroupBox->setTitle(QApplication::translate("CaseSpaceDialog", "Background", 0, QApplication::UnicodeUTF8));
        backgroundBlackRadioButton->setText(QApplication::translate("CaseSpaceDialog", "black", 0, QApplication::UnicodeUTF8));
        backgroundWhiteRadioButton->setText(QApplication::translate("CaseSpaceDialog", "white", 0, QApplication::UnicodeUTF8));
        backgroundRampRadioButton->setText(QApplication::translate("CaseSpaceDialog", "ramp", 0, QApplication::UnicodeUTF8));
        selectViewGroupBox->setTitle(QApplication::translate("CaseSpaceDialog", "Select View", 0, QApplication::UnicodeUTF8));
        XYViewRadioButton->setText(QApplication::translate("CaseSpaceDialog", "XY", 0, QApplication::UnicodeUTF8));
        XMIViewRadioButton->setText(QApplication::translate("CaseSpaceDialog", "X-MI", 0, QApplication::UnicodeUTF8));
        YMIViewRadioButton->setText(QApplication::translate("CaseSpaceDialog", "Y-MI", 0, QApplication::UnicodeUTF8));
        obliqueViewRadioButton->setText(QApplication::translate("CaseSpaceDialog", "Oblique", 0, QApplication::UnicodeUTF8));
        resetViewPushButton->setText(QApplication::translate("CaseSpaceDialog", "Reset", 0, QApplication::UnicodeUTF8));
        MIRangeGroupBox->setTitle(QApplication::translate("CaseSpaceDialog", "MI Range", 0, QApplication::UnicodeUTF8));
        MIMinLabel->setText(QApplication::translate("CaseSpaceDialog", "Minimum", 0, QApplication::UnicodeUTF8));
        MIMaxLabel->setText(QApplication::translate("CaseSpaceDialog", "Maximum", 0, QApplication::UnicodeUTF8));
        queryCaseLabel->setText(QApplication::translate("CaseSpaceDialog", "Query:", 0, QApplication::UnicodeUTF8));
        matchCaseLabel->setText(QApplication::translate("CaseSpaceDialog", "Match:", 0, QApplication::UnicodeUTF8));
        queryCaseNameLabel->setText(QApplication::translate("CaseSpaceDialog", "<not selected>", 0, QApplication::UnicodeUTF8));
        matchCaseNameLabel->setText(QApplication::translate("CaseSpaceDialog", "<not selected>", 0, QApplication::UnicodeUTF8));
        viewMIThresholdIndicatorsGroupBox->setTitle(QApplication::translate("CaseSpaceDialog", "View MI Threshold Indicators", 0, QApplication::UnicodeUTF8));
        viewAxisPlanesCheckBox->setText(QApplication::translate("CaseSpaceDialog", "Axis Planes", 0, QApplication::UnicodeUTF8));
        viewThresholdAxesCheckBox->setText(QApplication::translate("CaseSpaceDialog", "Threshold Axes", 0, QApplication::UnicodeUTF8));
        viewThresholdPlaneCheckBox->setText(QApplication::translate("CaseSpaceDialog", "Threshold Plane", 0, QApplication::UnicodeUTF8));
        compareCasesPushButton->setText(QApplication::translate("CaseSpaceDialog", "Compare Cases", 0, QApplication::UnicodeUTF8));
        testPushButton->setText(QApplication::translate("CaseSpaceDialog", "Test", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CaseSpaceDialog: public Ui_CaseSpaceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CASESPACEDIALOG_H
