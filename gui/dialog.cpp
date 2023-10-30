#include "dialog.hpp"
#include "ui/ui_dialog.hpp"

Dialog::Dialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

dialog_settings Dialog::get_dialog_data()
{
    return dialog_settings{
            ui->spin_box_alpha_slow->value(),
            ui->spin_box_alpha_fast->value(),
            ui->spin_box_epsilon->value(),
            ui->spin_box_delta->value(),
            ui->line_edit_min->text().toUInt(),
            ui->line_edit_max->text().toUInt()
    };
}
