#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
    class Dialog;
}

struct dialog_settings {
    double alpha_slow;
    double alpha_fast;
    double epsilon;
    double delta;
    unsigned int particle_count_min;
    unsigned int particle_count_max;
};

class Dialog : public QDialog
{
Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    dialog_settings get_dialog_data();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H

