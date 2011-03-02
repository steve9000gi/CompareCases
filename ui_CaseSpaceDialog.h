/********************************************************************************
** Form generated from reading UI file 'CaseSpaceDialog.ui'
**
** Created: Tue Mar 1 14:56:48 2011
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
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>
#include "qvtkwidget.h"

QT_BEGIN_NAMESPACE

class Ui_CaseSpaceDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *queryCaseLabel;
    QLabel *QueryCaseNameLabel;
    QVTKWidget *caseSpaceViewWidget;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *compareCasesPushButton;
    QPushButton *testPushButton;

    void setupUi(QDialog *CaseSpaceDialog)
    {
        if (CaseSpaceDialog->objectName().isEmpty())
            CaseSpaceDialog->setObjectName(QString::fromUtf8("CaseSpaceDialog"));
        CaseSpaceDialog->resize(973, 674);
        buttonBox = new QDialogButtonBox(CaseSpaceDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(790, 633, 156, 23));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        queryCaseLabel = new QLabel(CaseSpaceDialog);
        queryCaseLabel->setObjectName(QString::fromUtf8("queryCaseLabel"));
        queryCaseLabel->setGeometry(QRect(23, 18, 61, 13));
        QueryCaseNameLabel = new QLabel(CaseSpaceDialog);
        QueryCaseNameLabel->setObjectName(QString::fromUtf8("QueryCaseNameLabel"));
        QueryCaseNameLabel->setGeometry(QRect(90, 18, 723, 16));
        caseSpaceViewWidget = new QVTKWidget(CaseSpaceDialog);
        caseSpaceViewWidget->setObjectName(QString::fromUtf8("caseSpaceViewWidget"));
        caseSpaceViewWidget->setGeometry(QRect(27, 45, 920, 573));
        caseSpaceViewWidget->setMinimumSize(QSize(920, 569));
        widget = new QWidget(CaseSpaceDialog);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(27, 632, 166, 25));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        compareCasesPushButton = new QPushButton(widget);
        compareCasesPushButton->setObjectName(QString::fromUtf8("compareCasesPushButton"));

        horizontalLayout->addWidget(compareCasesPushButton);

        testPushButton = new QPushButton(widget);
        testPushButton->setObjectName(QString::fromUtf8("testPushButton"));

        horizontalLayout->addWidget(testPushButton);


        retranslateUi(CaseSpaceDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), CaseSpaceDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CaseSpaceDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(CaseSpaceDialog);
    } // setupUi

    void retranslateUi(QDialog *CaseSpaceDialog)
    {
        CaseSpaceDialog->setWindowTitle(QApplication::translate("CaseSpaceDialog", "Case Space", 0, QApplication::UnicodeUTF8));
        queryCaseLabel->setText(QApplication::translate("CaseSpaceDialog", "Query Case:", 0, QApplication::UnicodeUTF8));
        QueryCaseNameLabel->setText(QApplication::translate("CaseSpaceDialog", "<not selected>", 0, QApplication::UnicodeUTF8));
        compareCasesPushButton->setText(QApplication::translate("CaseSpaceDialog", "Compare Cases", 0, QApplication::UnicodeUTF8));
        testPushButton->setText(QApplication::translate("CaseSpaceDialog", "Test", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CaseSpaceDialog: public Ui_CaseSpaceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CASESPACEDIALOG_H
