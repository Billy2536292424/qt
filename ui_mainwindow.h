/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action;
    QAction *action_2;
    QAction *action_3;
    QAction *action_4;
    QAction *action_90;
    QAction *action_91;
    QAction *action_5;
    QWidget *centralWidget;
    QLabel *mousePos;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(400, 376);
        action = new QAction(MainWindow);
        action->setObjectName(QStringLiteral("action"));
        action_2 = new QAction(MainWindow);
        action_2->setObjectName(QStringLiteral("action_2"));
        action_3 = new QAction(MainWindow);
        action_3->setObjectName(QStringLiteral("action_3"));
        action_4 = new QAction(MainWindow);
        action_4->setObjectName(QStringLiteral("action_4"));
        action_90 = new QAction(MainWindow);
        action_90->setObjectName(QStringLiteral("action_90"));
        action_91 = new QAction(MainWindow);
        action_91->setObjectName(QStringLiteral("action_91"));
        action_5 = new QAction(MainWindow);
        action_5->setObjectName(QStringLiteral("action_5"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setLayoutDirection(Qt::LeftToRight);
        mousePos = new QLabel(centralWidget);
        mousePos->setObjectName(QStringLiteral("mousePos"));
        mousePos->setGeometry(QRect(190, 215, 41, 20));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        menu_3 = new QMenu(menuBar);
        menu_3->setObjectName(QStringLiteral("menu_3"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menu->addAction(action);
        menu->addAction(action_2);
        menu_2->addAction(action_90);
        menu_2->addAction(action_91);
        menu_3->addAction(action_5);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        action->setText(QApplication::translate("MainWindow", "\347\237\251\345\275\242", 0));
        action_2->setText(QApplication::translate("MainWindow", "\346\244\255\345\234\206", 0));
        action_3->setText(QApplication::translate("MainWindow", "\346\227\213\350\275\254", 0));
        action_4->setText(QApplication::translate("MainWindow", "\347\273\204\345\220\210", 0));
        action_90->setText(QApplication::translate("MainWindow", "\345\220\221\345\267\246\346\227\213\350\275\25490\345\272\246", 0));
        action_91->setText(QApplication::translate("MainWindow", "\345\220\221\345\217\263\346\227\213\350\275\25490\345\272\246", 0));
        action_5->setText(QApplication::translate("MainWindow", "\347\273\204\345\220\210", 0));
        mousePos->setText(QString());
        menu->setTitle(QApplication::translate("MainWindow", "\346\217\222\345\205\245", 0));
        menu_2->setTitle(QApplication::translate("MainWindow", "\346\227\213\350\275\254", 0));
        menu_3->setTitle(QApplication::translate("MainWindow", "\347\273\204\345\220\210", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
