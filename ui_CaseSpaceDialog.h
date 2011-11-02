/********************************************************************************
** Form generated from reading UI file 'CaseSpaceDialog.ui'
**
** Created: Tue Oct 25 12:21:08 2011
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
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
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
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QLabel *queryCaseLabel;
    QLabel *matchCaseLabel;
    QWidget *widget1;
    QVBoxLayout *verticalLayout_2;
    QLabel *queryCaseNameLabel;
    QLabel *matchCaseNameLabel;
    QWidget *widget2;
    QHBoxLayout *horizontalLayout_7;
    QHBoxLayout *compareTestButtonsLayout;
    QPushButton *compareCasesPushButton;
    QPushButton *testPushButton;
    QSpacerItem *horizontalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *CaseSpaceDialog)
    {
        if (CaseSpaceDialog->objectName().isEmpty())
            CaseSpaceDialog->setObjectName(QString::fromUtf8("CaseSpaceDialog"));
        CaseSpaceDialog->resize(1532, 1122);
        horizontalLayout = new QHBoxLayout(CaseSpaceDialog);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        scrollArea = new QScrollArea(CaseSpaceDialog);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1616, 1122));
        scrollAreaWidgetContents->setMinimumSize(QSize(1616, 1122));
        layoutWidget = new QWidget(scrollAreaWidgetContents);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(452, 1, 1032, 58));
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
        caseSpaceViewWidget->setGeometry(QRect(10, 62, 1475, 996));
        caseSpaceViewWidget->setMinimumSize(QSize(920, 569));
        splitter = new QSplitter(scrollAreaWidgetContents);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setGeometry(QRect(10, 2, 427, 34));
        splitter->setOrientation(Qt::Horizontal);
        widget = new QWidget(splitter);
        widget->setObjectName(QString::fromUtf8("widget"));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        queryCaseLabel = new QLabel(widget);
        queryCaseLabel->setObjectName(QString::fromUtf8("queryCaseLabel"));

        verticalLayout->addWidget(queryCaseLabel);

        matchCaseLabel = new QLabel(widget);
        matchCaseLabel->setObjectName(QString::fromUtf8("matchCaseLabel"));

        verticalLayout->addWidget(matchCaseLabel);

        splitter->addWidget(widget);
        widget1 = new QWidget(splitter);
        widget1->setObjectName(QString::fromUtf8("widget1"));
        verticalLayout_2 = new QVBoxLayout(widget1);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        queryCaseNameLabel = new QLabel(widget1);
        queryCaseNameLabel->setObjectName(QString::fromUtf8("queryCaseNameLabel"));

        verticalLayout_2->addWidget(queryCaseNameLabel);

        matchCaseNameLabel = new QLabel(widget1);
        matchCaseNameLabel->setObjectName(QString::fromUtf8("matchCaseNameLabel"));

        verticalLayout_2->addWidget(matchCaseNameLabel);

        splitter->addWidget(widget1);
        widget2 = new QWidget(scrollAreaWidgetContents);
        widget2->setObjectName(QString::fromUtf8("widget2"));
        widget2->setGeometry(QRect(10, 1062, 1476, 27));
        horizontalLayout_7 = new QHBoxLayout(widget2);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        compareTestButtonsLayout = new QHBoxLayout();
        compareTestButtonsLayout->setObjectName(QString::fromUtf8("compareTestButtonsLayout"));
        compareCasesPushButton = new QPushButton(widget2);
        compareCasesPushButton->setObjectName(QString::fromUtf8("compareCasesPushButton"));

        compareTestButtonsLayout->addWidget(compareCasesPushButton);

        testPushButton = new QPushButton(widget2);
        testPushButton->setObjectName(QString::fromUtf8("testPushButton"));

        compareTestButtonsLayout->addWidget(testPushButton);


        horizontalLayout_7->addLayout(compareTestButtonsLayout);

        horizontalSpacer = new QSpacerItem(1274, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer);

        buttonBox = new QDialogButtonBox(widget2);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        horizontalLayout_7->addWidget(buttonBox);

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
        compareCasesPushButton->setText(QApplication::translate("CaseSpaceDialog", "Compare Cases", 0, QApplication::UnicodeUTF8));
        testPushButton->setText(QApplication::translate("CaseSpaceDialog", "Test", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CaseSpaceDialog: public Ui_CaseSpaceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CASESPACEDIALOG_H
