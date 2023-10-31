/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_HPP
#define UI_MAINWINDOW_HPP

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actiontest;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_2;
    QGraphicsView *view;
    QHBoxLayout *horizontalLayout;
    QLabel *label_5;
    QSpinBox *laserSpinBox;
    QLabel *label_4;
    QSpinBox *threadSpinbox;
    QLabel *label_3;
    QSpinBox *timerSpinbox;
    QLabel *label;
    QLabel *label_2;
    QSpinBox *particleSpinbox;
    QComboBox *problem_comboBox;
    QSpacerItem *horizontalSpacer;
    QPushButton *startButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1220, 695);
        actiontest = new QAction(MainWindow);
        actiontest->setObjectName("actiontest");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        view = new QGraphicsView(centralwidget);
        view->setObjectName("view");

        verticalLayout_2->addWidget(view);


        verticalLayout->addLayout(verticalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName("label_5");

        horizontalLayout->addWidget(label_5);

        laserSpinBox = new QSpinBox(centralwidget);
        laserSpinBox->setObjectName("laserSpinBox");
        laserSpinBox->setMinimum(0);
        laserSpinBox->setMaximum(8);
        laserSpinBox->setValue(5);

        horizontalLayout->addWidget(laserSpinBox);

        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");

        horizontalLayout->addWidget(label_4);

        threadSpinbox = new QSpinBox(centralwidget);
        threadSpinbox->setObjectName("threadSpinbox");
        threadSpinbox->setMinimum(1);
        threadSpinbox->setValue(1);

        horizontalLayout->addWidget(threadSpinbox);

        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");

        horizontalLayout->addWidget(label_3);

        timerSpinbox = new QSpinBox(centralwidget);
        timerSpinbox->setObjectName("timerSpinbox");
        timerSpinbox->setMinimum(100);
        timerSpinbox->setMaximum(10000);
        timerSpinbox->setSingleStep(100);
        timerSpinbox->setValue(200);

        horizontalLayout->addWidget(timerSpinbox);

        label = new QLabel(centralwidget);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");

        horizontalLayout->addWidget(label_2);

        particleSpinbox = new QSpinBox(centralwidget);
        particleSpinbox->setObjectName("particleSpinbox");
        particleSpinbox->setMinimum(1000);
        particleSpinbox->setMaximum(1000000);
        particleSpinbox->setSingleStep(1000);
        particleSpinbox->setValue(100000);

        horizontalLayout->addWidget(particleSpinbox);

        problem_comboBox = new QComboBox(centralwidget);
        problem_comboBox->addItem(QString());
        problem_comboBox->addItem(QString());
        problem_comboBox->addItem(QString());
        problem_comboBox->addItem(QString());
        problem_comboBox->setObjectName("problem_comboBox");

        horizontalLayout->addWidget(problem_comboBox);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        startButton = new QPushButton(centralwidget);
        startButton->setObjectName("startButton");

        horizontalLayout->addWidget(startButton);


        verticalLayout->addLayout(horizontalLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1220, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        problem_comboBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Localization", nullptr));
        actiontest->setText(QCoreApplication::translate("MainWindow", "test", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Number of lasers", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Number of threads", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Timer", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", " ms ", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Number of particles", nullptr));
        problem_comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Local localization", nullptr));
        problem_comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Global localization", nullptr));
        problem_comboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Kidnapped robot problem", nullptr));
        problem_comboBox->setItemText(3, QCoreApplication::translate("MainWindow", "MMCL", nullptr));

        startButton->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_HPP

