/********************************************************************************
** Form generated from reading ui file 'levelEditor.ui'
**
** Created: Sun Nov 8 13:37:22 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_LEVELEDITOR_H
#define UI_LEVELEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_levelEditor
{
public:
    QWidget *centralwidget;
    QGraphicsView *graphicsView;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *levelEditor)
    {
        if (levelEditor->objectName().isEmpty())
            levelEditor->setObjectName(QString::fromUtf8("levelEditor"));
        levelEditor->resize(800, 600);
        centralwidget = new QWidget(levelEditor);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(20, 20, 571, 431));
        graphicsView->setFrameShape(QFrame::NoFrame);
        graphicsView->setFrameShadow(QFrame::Plain);
        levelEditor->setCentralWidget(centralwidget);
        menubar = new QMenuBar(levelEditor);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 22));
        levelEditor->setMenuBar(menubar);
        statusbar = new QStatusBar(levelEditor);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        levelEditor->setStatusBar(statusbar);

        retranslateUi(levelEditor);

        QMetaObject::connectSlotsByName(levelEditor);
    } // setupUi

    void retranslateUi(QMainWindow *levelEditor)
    {
        levelEditor->setWindowTitle(QApplication::translate("levelEditor", "MainWindow", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(levelEditor);
    } // retranslateUi

};

namespace Ui {
    class levelEditor: public Ui_levelEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LEVELEDITOR_H
