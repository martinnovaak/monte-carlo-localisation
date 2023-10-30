/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.5.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_HPP
#define UI_DIALOG_HPP

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QDoubleSpinBox *spin_box_alpha_slow;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_6;
    QDoubleSpinBox *spin_box_alpha_fast;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QDoubleSpinBox *spin_box_epsilon;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QDoubleSpinBox *spin_box_delta;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_7;
    QLineEdit *line_edit_min;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_8;
    QLineEdit *line_edit_max;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName("Dialog");
        Dialog->resize(408, 414);
        verticalLayout_4 = new QVBoxLayout(Dialog);
        verticalLayout_4->setObjectName("verticalLayout_4");
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        label_4 = new QLabel(Dialog);
        label_4->setObjectName("label_4");

        verticalLayout_2->addWidget(label_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        label_5 = new QLabel(Dialog);
        label_5->setObjectName("label_5");

        horizontalLayout_5->addWidget(label_5);

        spin_box_alpha_slow = new QDoubleSpinBox(Dialog);
        spin_box_alpha_slow->setObjectName("spin_box_alpha_slow");
        spin_box_alpha_slow->setAlignment(Qt::AlignCenter);
        spin_box_alpha_slow->setMinimum(0.010000000000000);
        spin_box_alpha_slow->setMaximum(1.000000000000000);
        spin_box_alpha_slow->setSingleStep(0.010000000000000);
        spin_box_alpha_slow->setValue(0.050000000000000);

        horizontalLayout_5->addWidget(spin_box_alpha_slow);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        label_6 = new QLabel(Dialog);
        label_6->setObjectName("label_6");

        horizontalLayout_6->addWidget(label_6);

        spin_box_alpha_fast = new QDoubleSpinBox(Dialog);
        spin_box_alpha_fast->setObjectName("spin_box_alpha_fast");
        spin_box_alpha_fast->setAlignment(Qt::AlignCenter);
        spin_box_alpha_fast->setMinimum(0.010000000000000);
        spin_box_alpha_fast->setMaximum(1.000000000000000);
        spin_box_alpha_fast->setSingleStep(0.010000000000000);
        spin_box_alpha_fast->setValue(0.500000000000000);

        horizontalLayout_6->addWidget(spin_box_alpha_fast);


        verticalLayout_2->addLayout(horizontalLayout_6);


        verticalLayout_3->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        label_2 = new QLabel(Dialog);
        label_2->setObjectName("label_2");

        verticalLayout->addWidget(label_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(Dialog);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        spin_box_epsilon = new QDoubleSpinBox(Dialog);
        spin_box_epsilon->setObjectName("spin_box_epsilon");
        spin_box_epsilon->setAlignment(Qt::AlignCenter);
        spin_box_epsilon->setMinimum(0.010000000000000);
        spin_box_epsilon->setMaximum(1.000000000000000);
        spin_box_epsilon->setSingleStep(0.010000000000000);

        horizontalLayout->addWidget(spin_box_epsilon);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_3 = new QLabel(Dialog);
        label_3->setObjectName("label_3");

        horizontalLayout_2->addWidget(label_3);

        spin_box_delta = new QDoubleSpinBox(Dialog);
        spin_box_delta->setObjectName("spin_box_delta");
        spin_box_delta->setAlignment(Qt::AlignCenter);
        spin_box_delta->setMinimum(0.010000000000000);
        spin_box_delta->setMaximum(1.000000000000000);
        spin_box_delta->setSingleStep(0.010000000000000);

        horizontalLayout_2->addWidget(spin_box_delta);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label_7 = new QLabel(Dialog);
        label_7->setObjectName("label_7");

        horizontalLayout_3->addWidget(label_7);

        line_edit_min = new QLineEdit(Dialog);
        line_edit_min->setObjectName("line_edit_min");
        line_edit_min->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(line_edit_min);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        label_8 = new QLabel(Dialog);
        label_8->setObjectName("label_8");

        horizontalLayout_4->addWidget(label_8);

        line_edit_max = new QLineEdit(Dialog);
        line_edit_max->setObjectName("line_edit_max");
        line_edit_max->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(line_edit_max);


        verticalLayout->addLayout(horizontalLayout_4);


        verticalLayout_3->addLayout(verticalLayout);


        verticalLayout_4->addLayout(verticalLayout_3);

        buttonBox = new QDialogButtonBox(Dialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_4->addWidget(buttonBox);


        retranslateUi(Dialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, Dialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, Dialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Settings dialog", nullptr));
        label_4->setText(QCoreApplication::translate("Dialog", "Atributes for augmented MCL", nullptr));
        label_5->setText(QCoreApplication::translate("Dialog", "alpha_slow", nullptr));
        label_6->setText(QCoreApplication::translate("Dialog", "alpha_fast", nullptr));
        label_2->setText(QCoreApplication::translate("Dialog", "Atributes for adaptive MCL", nullptr));
        label->setText(QCoreApplication::translate("Dialog", "Value of epsilon", nullptr));
        label_3->setText(QCoreApplication::translate("Dialog", "Value of delta", nullptr));
        label_7->setText(QCoreApplication::translate("Dialog", "Minimum particle count", nullptr));
        line_edit_min->setText(QCoreApplication::translate("Dialog", "100", nullptr));
        label_8->setText(QCoreApplication::translate("Dialog", "Maximum particle count", nullptr));
        line_edit_max->setText(QCoreApplication::translate("Dialog", "100000", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_HPP
