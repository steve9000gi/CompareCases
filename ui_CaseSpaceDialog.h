/********************************************************************************
** Form generated from reading UI file 'CaseSpaceDialog.ui'
**
** Created: Tue May 10 09:56:34 2011
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
    QLabel *queryCaseNameLabel;
    QVTKWidget *caseSpaceViewWidget;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *compareCasesPushButton;
    QPushButton *testPushButton;
    QLabel *matchCaseLabel;
    QLabel *matchCaseNameLabel;

    void setupUi(QDialog *CaseSpaceDialog)
    {
        if (CaseSpaceDialog->objectName().isEmpty())
            CaseSpaceDialog->setObjectName(QString::fromUtf8("CaseSpaceDialog"));
        CaseSpaceDialog->resize(973, 700);
        buttonBox = new QDialogButtonBox(CaseSpaceDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(792, 657, 156, 23));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        queryCaseLabel = new QLabel(CaseSpaceDialog);
        queryCaseLabel->setObjectName(QString::fromUtf8("queryCaseLabel"));
        queryCaseLabel->setGeometry(QRect(28, 20, 61, 16));
        queryCaseNameLabel = new QLabel(CaseSpaceDialog);
        queryCaseNameLabel->setObjectName(QString::fromUtf8("queryCaseNameLabel"));
        queryCaseNameLabel->setGeometry(QRect(95, 20, 465, 16));
        caseSpaceViewWidget = new QVTKWidget(CaseSpaceDialog);
        caseSpaceViewWidget->setObjectName(QString::fromUtf8("caseSpaceViewWidget"));
        caseSpaceViewWidget->setGeometry(QRect(27, 71, 920, 573));
        caseSpaceViewWidget->setMinimumSize(QSize(920, 569));
        layoutWidget = new QWidget(CaseSpaceDialog);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(28, 656, 166, 25));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        compareCasesPushButton = new QPushButton(layoutWidget);
        compareCasesPushButton->setObjectName(QString::fromUtf8("compareCasesPushButton"));

        horizontalLayout->addWidget(compareCasesPushButton);

        testPushButton = new QPushButton(layoutWidget);
        testPushButton->setObjectName(QString::fromUtf8("testPushButton"));

        horizontalLayout->addWidget(testPushButton);

        matchCaseLabel = new QLabel(CaseSpaceDialog);
        matchCaseLabel->setObjectName(QString::fromUtf8("matchCaseLabel"));
        matchCaseLabel->setGeometry(QRect(28, 41, 60, 16));
        matchCaseNameLabel = new QLabel(CaseSpaceDialog);
        matchCaseNameLabel->setObjectName(QString::fromUtf8("matchCaseNameLabel"));
        matchCaseNameLabel->setGeometry(QRect(94, 41, 592, 16));

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
        compareCasesPushButton->setText(QApplication::translate("CaseSpaceDialog", "Compare Cases", 0, QApplication::UnicodeUTF8));
        testPushButton->setText(QApplication::translate("CaseSpaceDialog", "Test", 0, QApplication::UnicodeUTF8));
        matchCaseLabel->setText(QApplication::translate("CaseSpaceDialog", "Match Case:", 0, QApplication::UnicodeUTF8));
        matchCaseNameLabel->setText(QApplication::translate("CaseSpaceDialog", "<not selected>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CaseSpaceDialog: public Ui_CaseSpaceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CASESPACEDIALOG_H
