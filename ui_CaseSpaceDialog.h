/********************************************************************************
** Form generated from reading UI file 'CaseSpaceDialog.ui'
**
** Created: Mon Feb 28 08:52:31 2011
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
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_CaseSpaceDialog
{
public:
    QDialogButtonBox *buttonBox;
    QPushButton *compareCasesPushButton;
    QLabel *queryCaseLabel;
    QLabel *QueryCaseNameLabel;
    QPushButton *templateTestPushButton;

    void setupUi(QDialog *CaseSpaceDialog)
    {
        if (CaseSpaceDialog->objectName().isEmpty())
            CaseSpaceDialog->setObjectName(QString::fromUtf8("CaseSpaceDialog"));
        CaseSpaceDialog->resize(973, 674);
        buttonBox = new QDialogButtonBox(CaseSpaceDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(713, 636, 156, 23));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        compareCasesPushButton = new QPushButton(CaseSpaceDialog);
        compareCasesPushButton->setObjectName(QString::fromUtf8("compareCasesPushButton"));
        compareCasesPushButton->setGeometry(QRect(71, 636, 83, 23));
        queryCaseLabel = new QLabel(CaseSpaceDialog);
        queryCaseLabel->setObjectName(QString::fromUtf8("queryCaseLabel"));
        queryCaseLabel->setGeometry(QRect(23, 18, 61, 13));
        QueryCaseNameLabel = new QLabel(CaseSpaceDialog);
        QueryCaseNameLabel->setObjectName(QString::fromUtf8("QueryCaseNameLabel"));
        QueryCaseNameLabel->setGeometry(QRect(90, 18, 723, 16));
        templateTestPushButton = new QPushButton(CaseSpaceDialog);
        templateTestPushButton->setObjectName(QString::fromUtf8("templateTestPushButton"));
        templateTestPushButton->setGeometry(QRect(159, 323, 75, 23));

        retranslateUi(CaseSpaceDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), CaseSpaceDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CaseSpaceDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(CaseSpaceDialog);
    } // setupUi

    void retranslateUi(QDialog *CaseSpaceDialog)
    {
        CaseSpaceDialog->setWindowTitle(QApplication::translate("CaseSpaceDialog", "Case Space", 0, QApplication::UnicodeUTF8));
        compareCasesPushButton->setText(QApplication::translate("CaseSpaceDialog", "Compare Cases", 0, QApplication::UnicodeUTF8));
        queryCaseLabel->setText(QApplication::translate("CaseSpaceDialog", "Query Case:", 0, QApplication::UnicodeUTF8));
        QueryCaseNameLabel->setText(QApplication::translate("CaseSpaceDialog", "<not selected>", 0, QApplication::UnicodeUTF8));
        templateTestPushButton->setText(QApplication::translate("CaseSpaceDialog", "template Test", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CaseSpaceDialog: public Ui_CaseSpaceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CASESPACEDIALOG_H
