/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created: Tue Oct 25 12:21:07 2011
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
#include <QtGui/QComboBox>
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
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
    QAction *actionAbout_CompareCases;
    QAction *action_View_documentation;
    QWidget *centralwidget;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *institutionsGroupBox;
    QFormLayout *formLayout;
    QPushButton *loadDukePushButton;
    QPushButton *loadPoconoPushButton;
    QPushButton *loadHighPointPushButton;
    QLineEdit *loadDukeLineEdit;
    QLineEdit *loadPoconoLineEdit;
    QLineEdit *loadHighPointLineEdit;
    QGroupBox *selectQueryCaseGroupBox;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *dukeQueryCaseComboBox;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *poconoQueryCaseComboBox;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QComboBox *highPointQueryCaseComboBox;
    QHBoxLayout *horizontalLayout_4;
    QLabel *queryCaseLabel;
    QLabel *queryCaseNameLabel;
    QPushButton *viewCaseSpaceButton;
    QSpacerItem *horizontalSpacer_4;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(327, 352);
        actionSelect_Duke_Directory = new QAction(MainWindow);
        actionSelect_Duke_Directory->setObjectName(QString::fromUtf8("actionSelect_Duke_Directory"));
        actionSelect_Pocono_Directory = new QAction(MainWindow);
        actionSelect_Pocono_Directory->setObjectName(QString::fromUtf8("actionSelect_Pocono_Directory"));
        actionSelect_High_Point_Directory = new QAction(MainWindow);
        actionSelect_High_Point_Directory->setObjectName(QString::fromUtf8("actionSelect_High_Point_Directory"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionAbout_CompareCases = new QAction(MainWindow);
        actionAbout_CompareCases->setObjectName(QString::fromUtf8("actionAbout_CompareCases"));
        action_View_documentation = new QAction(MainWindow);
        action_View_documentation->setObjectName(QString::fromUtf8("action_View_documentation"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(-2, 10, 330, 287));
        gridLayout_2 = new QGridLayout(layoutWidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_3, 0, 0, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        institutionsGroupBox = new QGroupBox(layoutWidget);
        institutionsGroupBox->setObjectName(QString::fromUtf8("institutionsGroupBox"));
        formLayout = new QFormLayout(institutionsGroupBox);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        loadDukePushButton = new QPushButton(institutionsGroupBox);
        loadDukePushButton->setObjectName(QString::fromUtf8("loadDukePushButton"));

        formLayout->setWidget(0, QFormLayout::LabelRole, loadDukePushButton);

        loadPoconoPushButton = new QPushButton(institutionsGroupBox);
        loadPoconoPushButton->setObjectName(QString::fromUtf8("loadPoconoPushButton"));

        formLayout->setWidget(1, QFormLayout::LabelRole, loadPoconoPushButton);

        loadHighPointPushButton = new QPushButton(institutionsGroupBox);
        loadHighPointPushButton->setObjectName(QString::fromUtf8("loadHighPointPushButton"));

        formLayout->setWidget(2, QFormLayout::LabelRole, loadHighPointPushButton);

        loadDukeLineEdit = new QLineEdit(institutionsGroupBox);
        loadDukeLineEdit->setObjectName(QString::fromUtf8("loadDukeLineEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, loadDukeLineEdit);

        loadPoconoLineEdit = new QLineEdit(institutionsGroupBox);
        loadPoconoLineEdit->setObjectName(QString::fromUtf8("loadPoconoLineEdit"));

        formLayout->setWidget(1, QFormLayout::FieldRole, loadPoconoLineEdit);

        loadHighPointLineEdit = new QLineEdit(institutionsGroupBox);
        loadHighPointLineEdit->setObjectName(QString::fromUtf8("loadHighPointLineEdit"));

        formLayout->setWidget(2, QFormLayout::FieldRole, loadHighPointLineEdit);


        verticalLayout_2->addWidget(institutionsGroupBox);

        selectQueryCaseGroupBox = new QGroupBox(layoutWidget);
        selectQueryCaseGroupBox->setObjectName(QString::fromUtf8("selectQueryCaseGroupBox"));
        gridLayout = new QGridLayout(selectQueryCaseGroupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(selectQueryCaseGroupBox);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        dukeQueryCaseComboBox = new QComboBox(selectQueryCaseGroupBox);
        dukeQueryCaseComboBox->setObjectName(QString::fromUtf8("dukeQueryCaseComboBox"));

        horizontalLayout->addWidget(dukeQueryCaseComboBox);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(selectQueryCaseGroupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        poconoQueryCaseComboBox = new QComboBox(selectQueryCaseGroupBox);
        poconoQueryCaseComboBox->setObjectName(QString::fromUtf8("poconoQueryCaseComboBox"));

        horizontalLayout_2->addWidget(poconoQueryCaseComboBox);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(selectQueryCaseGroupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        highPointQueryCaseComboBox = new QComboBox(selectQueryCaseGroupBox);
        highPointQueryCaseComboBox->setObjectName(QString::fromUtf8("highPointQueryCaseComboBox"));

        horizontalLayout_3->addWidget(highPointQueryCaseComboBox);


        verticalLayout->addLayout(horizontalLayout_3);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        verticalLayout_2->addWidget(selectQueryCaseGroupBox);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        queryCaseLabel = new QLabel(layoutWidget);
        queryCaseLabel->setObjectName(QString::fromUtf8("queryCaseLabel"));

        horizontalLayout_4->addWidget(queryCaseLabel);

        queryCaseNameLabel = new QLabel(layoutWidget);
        queryCaseNameLabel->setObjectName(QString::fromUtf8("queryCaseNameLabel"));

        horizontalLayout_4->addWidget(queryCaseNameLabel);


        verticalLayout_2->addLayout(horizontalLayout_4);

        viewCaseSpaceButton = new QPushButton(layoutWidget);
        viewCaseSpaceButton->setObjectName(QString::fromUtf8("viewCaseSpaceButton"));

        verticalLayout_2->addWidget(viewCaseSpaceButton);


        gridLayout_2->addLayout(verticalLayout_2, 0, 1, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_4, 0, 2, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 327, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
#ifndef QT_NO_SHORTCUT
        label->setBuddy(dukeQueryCaseComboBox);
        label_2->setBuddy(poconoQueryCaseComboBox);
        label_3->setBuddy(highPointQueryCaseComboBox);
#endif // QT_NO_SHORTCUT

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionSelect_Duke_Directory);
        menuFile->addAction(actionSelect_Pocono_Directory);
        menuFile->addAction(actionSelect_High_Point_Directory);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuHelp->addAction(action_View_documentation);
        menuHelp->addAction(actionAbout_CompareCases);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Radiation Therapy Planning Tool", 0, QApplication::UnicodeUTF8));
        actionSelect_Duke_Directory->setText(QApplication::translate("MainWindow", "Select Duke Directory...", 0, QApplication::UnicodeUTF8));
        actionSelect_Pocono_Directory->setText(QApplication::translate("MainWindow", "Select Pocono Directory...", 0, QApplication::UnicodeUTF8));
        actionSelect_High_Point_Directory->setText(QApplication::translate("MainWindow", "Select High Point Directory...", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindow", "E&xit", 0, QApplication::UnicodeUTF8));
        actionAbout_CompareCases->setText(QApplication::translate("MainWindow", "About CompareCases", 0, QApplication::UnicodeUTF8));
        action_View_documentation->setText(QApplication::translate("MainWindow", "&View documentation", 0, QApplication::UnicodeUTF8));
        institutionsGroupBox->setTitle(QApplication::translate("MainWindow", "Load institutions:", 0, QApplication::UnicodeUTF8));
        loadDukePushButton->setText(QApplication::translate("MainWindow", "Duke", 0, QApplication::UnicodeUTF8));
        loadPoconoPushButton->setText(QApplication::translate("MainWindow", "Pocono", 0, QApplication::UnicodeUTF8));
        loadHighPointPushButton->setText(QApplication::translate("MainWindow", "High Point", 0, QApplication::UnicodeUTF8));
        selectQueryCaseGroupBox->setTitle(QApplication::translate("MainWindow", "Select Query Case from...", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Duke:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Pocono:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "High Point:", 0, QApplication::UnicodeUTF8));
        queryCaseLabel->setText(QApplication::translate("MainWindow", "Query Case:", 0, QApplication::UnicodeUTF8));
        queryCaseNameLabel->setText(QApplication::translate("MainWindow", "<not selected>", 0, QApplication::UnicodeUTF8));
        viewCaseSpaceButton->setText(QApplication::translate("MainWindow", "View Case Space", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
