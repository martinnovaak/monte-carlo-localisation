#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "myscene.hpp"
#include "../lib/localisation.hpp"
#include "dialog.hpp"

class QGraphicsRectItem;
class QGraphicsEllipseItem;
class QTimer;

enum Direction : int {NONE = 0, UP = 1, DOWN = 2, LEFT = 3, RIGHT = 4};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
Q_OBJECT

    void setup_UI();
    void setup_signals();
    void allocate_particles(unsigned int number_of_particles);
    bool is_black(const QRgb & pixel);
    void load_map(const QPixmap & pixmap);

    void setup_robot();
    void setup_lasers(int number_of_lasers);
    void update_robot();
    void draw_robot();
    void generate_particles();

    void update_canvas();
    Pose localise(int option);
public:
    explicit MainWindow(QWidget * parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow * ui;
    MyScene * scene;

    QGraphicsRectItem * enviroment;
    std::vector<std::unique_ptr<QGraphicsEllipseItem>> particle_items;
    std::vector<Particle> particles;

    Particle robot;
    std::unique_ptr<QGraphicsEllipseItem> robot_item;
    std::unique_ptr<QGraphicsEllipseItem> robot_estimate_item;
    std::vector<std::unique_ptr<QGraphicsLineItem>> lasers;

    QTimer * timer;
    std::vector<std::vector<bool>> map;
    std::array <double, 3> u_t;
    std::vector<double> z_t;
    std::vector<double> angles;

    Direction key;

    std::unique_ptr<Localisation> localisation;
protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
private slots:
    void update();
    void handleMousePressEvent(QGraphicsSceneMouseEvent *event);
    void handleMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void startTimer(int time);
    void loadMaze();
    void load_settings();
};
#endif // MAINWINDOW_H
