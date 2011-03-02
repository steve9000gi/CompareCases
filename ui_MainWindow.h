/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created: Tue Mar 1 14:56:48 2011
**      by: Qt User Interface Compiler version 4.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSelect_Duke_Directory;
    QAction *actionSelect_Pocono_Directory;
    QAction *actionSelect_High_Point_Directory;
    QAction *actionExit;
    QWidget *centralwidget;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QGroupBox *institutionsGroupBox;
    QFormLayout *formLayout;
    QRadioButton *justDukeRadioButton;
    QRadioButton *addPoconoRadioButton;
    QRadioButton *addHighPointRadioButton;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    QGroupBox *selectQueryCaseGroupBox;
    QGridLayout *gridLayout;
    QPushButton *selectFormDukePushButton;
    QPushButton *selectFromPoconoPushButton;
    QPushButton *selectFromHighPointPushButton;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout;
    QLabel *queryCaseLabel;
    QLabel *QueryCaseNameLabel;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *viewCaseSpaceButton;
    QSpacerItem *horizontalSpacer_8;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(338, 342);
        actionSelect_Duke_Directory = new QAction(MainWindow);
        actionSelect_Duke_Directory->setObjectName(QString::fromUtf8("actionSelect_Duke_Directory"));
        actionSelect_Pocono_Directory = new QAction(MainWindow);
        actionSelect_Pocono_Directory->setObjectName(QString::fromUtf8("actionSelect_Pocono_Directory"));
        actionSelect_High_Point_Directory = new QAction(MainWindow);
        actionSelect_High_Point_Directory->setObjectName(QString::fromUtf8("actionSelect_High_Point_Directory"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(30, 7, 280, 281));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        institutionsGroupBox = new QGroupBox(widget);
        institutionsGroupBox->setObjectName(QString::fromUtf8("institutionsGroupBox"));
        formLayout = new QFormLayout(institutionsGroupBox);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        justDukeRadioButton = new QRadioButton(institutionsGroupBox);
        justDukeRadioButton->setObjectName(QString::fromUtf8("justDukeRadioButton"));

        formLayout->setWidget(0, QFormLayout::LabelRole, justDukeRadioButton);

        addPoconoRadioButton = new QRadioButton(institutionsGroupBox);
        addPoconoRadioButton->setObjectName(QString::fromUtf8("addPoconoRadioButton"));

        formLayout->setWidget(1, QFormLayout::LabelRole, addPoconoRadioButton);

        addHighPointRadioButton = new QRadioButton(institutionsGroupBox);
        addHighPointRadioButton->setObjectName(QString::fromUtf8("addHighPointRadioButton"));

        formLayout->setWidget(2, QFormLayout::LabelRole, addHighPointRadioButton);


        horizontalLayout_2->addWidget(institutionsGroupBox);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        selectQueryCaseGroupBox = new QGroupBox(widget);
        selectQueryCaseGroupBox->setObjectName(QString::fromUtf8("selectQueryCaseGroupBox"));
        gridLayout = new QGridLayout(selectQueryCaseGroupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        selectFormDukePushButton = new QPushButton(selectQueryCaseGroupBox);
        selectFormDukePushButton->setObjectName(QString::fromUtf8("selectFormDukePushButton"));

        gridLayout->addWidget(selectFormDukePushButton, 0, 0, 1, 1);

        selectFromPoconoPushButton = new QPushButton(selectQueryCaseGroupBox);
        selectFromPoconoPushButton->setObjectName(QString::fromUtf8("selectFromPoconoPushButton"));

        gridLayout->addWidget(selectFromPoconoPushButton, 1, 0, 1, 1);

        selectFromHighPointPushButton = new QPushButton(selectQueryCaseGroupBox);
        selectFromHighPointPushButton->setObjectName(QString::fromUtf8("selectFromHighPointPushButton"));

        gridLayout->addWidget(selectFromHighPointPushButton, 2, 0, 1, 1);


        horizontalLayout_3->addWidget(selectQueryCaseGroupBox);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        queryCaseLabel = new QLabel(widget);
        queryCaseLabel->setObjectName(QString::fromUtf8("queryCaseLabel"));

        horizontalLayout->addWidget(queryCaseLabel);

        QueryCaseNameLabel = new QLabel(widget);
        QueryCaseNameLabel->setObjectName(QString::fromUtf8("QueryCaseNameLabel"));

        horizontalLayout->addWidget(QueryCaseNameLabel);


        horizontalLayout_4->addLayout(horizontalLayout);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_6);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_7);

        viewCaseSpaceButton = new QPushButton(widget);
        viewCaseSpaceButton->setObjectName(QString::fromUtf8("viewCaseSpaceButton"));

        horizontalLayout_5->addWidget(viewCaseSpaceButton);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_8);


        verticalLayout->addLayout(horizontalLayout_5);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 338, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionSelect_Duke_Directory);
        menuFile->addAction(actionSelect_Pocono_Directory);
        menuFile->addAction(actionSelect_High_Point_Directory);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Radiation Therapy Planning Tool", 0, QApplication::UnicodeUTF8));
        actionSelect_Duke_Directory->setText(QApplication::translate("MainWindow", "Select Duke Directory...", 0, QApplication::UnicodeUTF8));
        actionSelect_Pocono_Directory->setText(QApplication::translate("MainWindow", "Select Pocono Directory...", 0, QApplication::UnicodeUTF8));
        actionSelect_High_Point_Directory->setText(QApplication::translate("MainWindow", "Select High Point Directory...", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        institutionsGroupBox->setTitle(QApplication::translate("MainWindow", "Load institutions", 0, QApplication::UnicodeUTF8));
        justDukeRadioButton->setText(QApplication::translate("MainWindow", "Duke", 0, QApplication::UnicodeUTF8));
        addPoconoRadioButton->setText(QApplication::translate("MainWindow", "Duke + Pocono", 0, QApplication::UnicodeUTF8));
        addHighPointRadioButton->setText(QApplication::translate("MainWindow", "Duke + Pocono + High Point", 0, QApplication::UnicodeUTF8));
        selectQueryCaseGroupBox->setTitle(QApplication::translate("MainWindow", "Select Query Case from...", 0, QApplication::UnicodeUTF8));
        selectFormDukePushButton->setText(QApplication::translate("MainWindow", "Duke", 0, QApplication::UnicodeUTF8));
        selectFromPoconoPushButton->setText(QApplication::translate("MainWindow", "Pocono", 0, QApplication::UnicodeUTF8));
        selectFromHighPointPushButton->setText(QApplication::translate("MainWindow", "High Point", 0, QApplication::UnicodeUTF8));
        queryCaseLabel->setText(QApplication::translate("MainWindow", "Query Case:", 0, QApplication::UnicodeUTF8));
        QueryCaseNameLabel->setText(QApplication::translate("MainWindow", "<not selected>", 0, QApplication::UnicodeUTF8));
        viewCaseSpaceButton->setText(QApplication::translate("MainWindow", "View Case Space", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
