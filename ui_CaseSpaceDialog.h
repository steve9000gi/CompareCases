/********************************************************************************
** Form generated from reading UI file 'CaseSpaceDialog.ui'
**
** Created: Fri Sep 9 14:58:21 2011
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
#include <QtGui/QSlider>
#include <QtGui/QWidget>
#include "qvtkwidget.h"

QT_BEGIN_NAMESPACE

class Ui_CaseSpaceDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *queryCaseLabel;
    QLabel *queryCaseNameLabel;
    QVTKWidget *caseSpaceViewWidget;
    QLabel *matchCaseLabel;
    QLabel *matchCaseNameLabel;
    QWidget *layoutWidget;
    QHBoxLayout *compareTestButtonsLayout;
    QPushButton *compareCasesPushButton;
    QPushButton *testPushButton;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *backgroundColorGroupBox;
    QHBoxLayout *horizontalLayout_4;
    QRadioButton *backgroundBlackRadioButton;
    QRadioButton *backgroundWhiteRadioButton;
    QRadioButton *backgroundRampRadioButton;
    QGroupBox *selectViewGroupBox;
    QHBoxLayout *horizontalLayout_5;
    QRadioButton *XYViewRadioButton;
    QRadioButton *XMIViewRadioButton;
    QRadioButton *YMIViewRadioButton;
    QRadioButton *obliqueViewRadioButton;
    QGroupBox *MIRangeGroupBox;
    QHBoxLayout *horizontalLayout;
    QLabel *MIMinLabel;
    QSlider *MIRangeSlider;
    QLabel *MIMaxLabel;

    void setupUi(QDialog *CaseSpaceDialog)
    {
        if (CaseSpaceDialog->objectName().isEmpty())
            CaseSpaceDialog->setObjectName(QString::fromUtf8("CaseSpaceDialog"));
        CaseSpaceDialog->resize(1653, 1123);
        buttonBox = new QDialogButtonBox(CaseSpaceDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(1472, 1080, 156, 23));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        queryCaseLabel = new QLabel(CaseSpaceDialog);
        queryCaseLabel->setObjectName(QString::fromUtf8("queryCaseLabel"));
        queryCaseLabel->setGeometry(QRect(28, 20, 61, 16));
        queryCaseNameLabel = new QLabel(CaseSpaceDialog);
        queryCaseNameLabel->setObjectName(QString::fromUtf8("queryCaseNameLabel"));
        queryCaseNameLabel->setGeometry(QRect(95, 20, 628, 16));
        caseSpaceViewWidget = new QVTKWidget(CaseSpaceDialog);
        caseSpaceViewWidget->setObjectName(QString::fromUtf8("caseSpaceViewWidget"));
        caseSpaceViewWidget->setGeometry(QRect(27, 71, 1600, 996));
        caseSpaceViewWidget->setMinimumSize(QSize(920, 569));
        matchCaseLabel = new QLabel(CaseSpaceDialog);
        matchCaseLabel->setObjectName(QString::fromUtf8("matchCaseLabel"));
        matchCaseLabel->setGeometry(QRect(28, 41, 60, 16));
        matchCaseNameLabel = new QLabel(CaseSpaceDialog);
        matchCaseNameLabel->setObjectName(QString::fromUtf8("matchCaseNameLabel"));
        matchCaseNameLabel->setGeometry(QRect(94, 41, 519, 16));
        layoutWidget = new QWidget(CaseSpaceDialog);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(29, 1080, 166, 25));
        compareTestButtonsLayout = new QHBoxLayout(layoutWidget);
        compareTestButtonsLayout->setObjectName(QString::fromUtf8("compareTestButtonsLayout"));
        compareTestButtonsLayout->setContentsMargins(0, 0, 0, 0);
        compareCasesPushButton = new QPushButton(layoutWidget);
        compareCasesPushButton->setObjectName(QString::fromUtf8("compareCasesPushButton"));

        compareTestButtonsLayout->addWidget(compareCasesPushButton);

        testPushButton = new QPushButton(layoutWidget);
        testPushButton->setObjectName(QString::fromUtf8("testPushButton"));

        compareTestButtonsLayout->addWidget(testPushButton);

        widget = new QWidget(CaseSpaceDialog);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(864, 11, 764, 54));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        backgroundColorGroupBox = new QGroupBox(widget);
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


        horizontalLayout_2->addWidget(backgroundColorGroupBox);

        selectViewGroupBox = new QGroupBox(widget);
        selectViewGroupBox->setObjectName(QString::fromUtf8("selectViewGroupBox"));
        horizontalLayout_5 = new QHBoxLayout(selectViewGroupBox);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        XYViewRadioButton = new QRadioButton(selectViewGroupBox);
        XYViewRadioButton->setObjectName(QString::fromUtf8("XYViewRadioButton"));

        horizontalLayout_5->addWidget(XYViewRadioButton);

        XMIViewRadioButton = new QRadioButton(selectViewGroupBox);
        XMIViewRadioButton->setObjectName(QString::fromUtf8("XMIViewRadioButton"));

        horizontalLayout_5->addWidget(XMIViewRadioButton);

        YMIViewRadioButton = new QRadioButton(selectViewGroupBox);
        YMIViewRadioButton->setObjectName(QString::fromUtf8("YMIViewRadioButton"));

        horizontalLayout_5->addWidget(YMIViewRadioButton);

        obliqueViewRadioButton = new QRadioButton(selectViewGroupBox);
        obliqueViewRadioButton->setObjectName(QString::fromUtf8("obliqueViewRadioButton"));
        obliqueViewRadioButton->setChecked(true);

        horizontalLayout_5->addWidget(obliqueViewRadioButton);


        horizontalLayout_2->addWidget(selectViewGroupBox);

        MIRangeGroupBox = new QGroupBox(widget);
        MIRangeGroupBox->setObjectName(QString::fromUtf8("MIRangeGroupBox"));
        horizontalLayout = new QHBoxLayout(MIRangeGroupBox);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        MIMinLabel = new QLabel(MIRangeGroupBox);
        MIMinLabel->setObjectName(QString::fromUtf8("MIMinLabel"));
        MIMinLabel->setMinimumSize(QSize(40, 0));

        horizontalLayout->addWidget(MIMinLabel);

        MIRangeSlider = new QSlider(MIRangeGroupBox);
        MIRangeSlider->setObjectName(QString::fromUtf8("MIRangeSlider"));
        MIRangeSlider->setMinimumSize(QSize(238, 0));
        MIRangeSlider->setMaximum(1000);
        MIRangeSlider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(MIRangeSlider);

        MIMaxLabel = new QLabel(MIRangeGroupBox);
        MIMaxLabel->setObjectName(QString::fromUtf8("MIMaxLabel"));
        MIMaxLabel->setMinimumSize(QSize(40, 0));

        horizontalLayout->addWidget(MIMaxLabel);


        horizontalLayout_2->addWidget(MIRangeGroupBox);

        buttonBox->raise();
        queryCaseLabel->raise();
        queryCaseNameLabel->raise();
        caseSpaceViewWidget->raise();
        matchCaseLabel->raise();
        matchCaseNameLabel->raise();
        layoutWidget->raise();
        layoutWidget->raise();
        selectViewGroupBox->raise();

        retranslateUi(CaseSpaceDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), CaseSpaceDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CaseSpaceDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(CaseSpaceDialog);
    } // setupUi

    void retranslateUi(QDialog *CaseSpaceDialog)
    {
        CaseSpaceDialog->setWindowTitle(QApplication::translate("CaseSpaceDialog", "Case Space", 0, QApplication::UnicodeUTF8));
        queryCaseLabel->setText(QApplication::translate("CaseSpaceDialog", "Query Case:", 0, QApplication::UnicodeUTF8));
        queryCaseNameLabel->setText(QApplication::translate("CaseSpaceDialog", "<not selected>", 0, QApplication::UnicodeUTF8));
        matchCaseLabel->setText(QApplication::translate("CaseSpaceDialog", "Match Case:", 0, QApplication::UnicodeUTF8));
        matchCaseNameLabel->setText(QApplication::translate("CaseSpaceDialog", "<not selected>", 0, QApplication::UnicodeUTF8));
        compareCasesPushButton->setText(QApplication::translate("CaseSpaceDialog", "Compare Cases", 0, QApplication::UnicodeUTF8));
        testPushButton->setText(QApplication::translate("CaseSpaceDialog", "Test", 0, QApplication::UnicodeUTF8));
        backgroundColorGroupBox->setTitle(QApplication::translate("CaseSpaceDialog", "Background", 0, QApplication::UnicodeUTF8));
        backgroundBlackRadioButton->setText(QApplication::translate("CaseSpaceDialog", "black", 0, QApplication::UnicodeUTF8));
        backgroundWhiteRadioButton->setText(QApplication::translate("CaseSpaceDialog", "white", 0, QApplication::UnicodeUTF8));
        backgroundRampRadioButton->setText(QApplication::translate("CaseSpaceDialog", "ramp", 0, QApplication::UnicodeUTF8));
        selectViewGroupBox->setTitle(QApplication::translate("CaseSpaceDialog", "Select View", 0, QApplication::UnicodeUTF8));
        XYViewRadioButton->setText(QApplication::translate("CaseSpaceDialog", "XY", 0, QApplication::UnicodeUTF8));
        XMIViewRadioButton->setText(QApplication::translate("CaseSpaceDialog", "X-MI", 0, QApplication::UnicodeUTF8));
        YMIViewRadioButton->setText(QApplication::translate("CaseSpaceDialog", "Y-MI", 0, QApplication::UnicodeUTF8));
        obliqueViewRadioButton->setText(QApplication::translate("CaseSpaceDialog", "Oblique", 0, QApplication::UnicodeUTF8));
        MIRangeGroupBox->setTitle(QApplication::translate("CaseSpaceDialog", "MI Range", 0, QApplication::UnicodeUTF8));
        MIMinLabel->setText(QApplication::translate("CaseSpaceDialog", "Minimum", 0, QApplication::UnicodeUTF8));
        MIMaxLabel->setText(QApplication::translate("CaseSpaceDialog", "Maximum", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CaseSpaceDialog: public Ui_CaseSpaceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CASESPACEDIALOG_H
