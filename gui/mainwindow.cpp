#include "mainwindow.hpp"
#include "ui/ui_mainwindow.hpp"

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QPixmap>
#include <QBrush>
#include <QTimer>
#include <QFileInfo>
#include <QFileDialog>
#include <QAction>
#include <QKeyEvent>
#include <QMessageBox>

#include <chrono>
#include <iostream>
#include <thread>

void MainWindow::setup_UI()
{
    ui->setupUi(this);

    auto * action_load_maze = new QAction("Load Map");
    this->menuBar()->addAction(action_load_maze);
    connect(action_load_maze, &QAction::triggered, this, &MainWindow::loadMaze);

    auto * action_settings = new QAction("Settings");
    this->menuBar()->addAction(action_settings);
    connect(action_settings, &QAction::triggered, this, &MainWindow::load_settings);


    ui->view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QPixmap pixmap(":/maze2.png");
    scene = new MyScene;
    load_map(pixmap);

    unsigned int number_of_CPU_threads = std::thread::hardware_concurrency();
    ui->threadSpinbox->setMaximum(static_cast<int>(number_of_CPU_threads));
    ui->threadSpinbox->setValue(static_cast<int>(number_of_CPU_threads / 2));
}

void MainWindow::setup_signals()
{
    // Mouse events
    connect(scene, &MyScene::mousePressEventSignal, this, &MainWindow::handleMousePressEvent);
    connect(scene, &MyScene::mouseReleaseEventSignal, this, &MainWindow::handleMouseReleaseEvent);

    // Button signals
    connect(ui->startButton, &QPushButton::clicked, this, [&]{
        startTimer(ui->timerSpinbox->value());
    });

    connect(timer, &QTimer::timeout, this, &MainWindow::update);

    connect(ui->threadSpinbox, &QSpinBox::valueChanged, this, [&]{
        localisation->set_threads(ui->threadSpinbox->value());
    });

    connect(ui->laserSpinBox, &QSpinBox::valueChanged, this, [&]{
        setup_lasers(ui->laserSpinBox->value());
    });
}

void MainWindow::allocate_particles(unsigned int number_of_particles)
{
    particle_items.resize(number_of_particles);
    particles.resize(number_of_particles);

    for (unsigned int i = 0; i < number_of_particles; i++) {
        this->particle_items[i] = std::make_unique<QGraphicsEllipseItem>(0, 0, 3, 3);
        this->particle_items[i]->setBrush(QBrush(Qt::red));
        this->particle_items[i]->setPen(QPen(Qt::red));
        if (i < 500) {
            scene->addItem(particle_items[i].get());
        }
    }
}

bool MainWindow::is_black(const QRgb & pixel)
{
    int grayscale = (qRed(pixel) * 11 + qGreen(pixel) * 16 + qBlue(pixel) * 5) / 32;
    return grayscale <= 128;
}

void MainWindow::load_map(const QPixmap &pixmap)
{
    ui->view->setRenderHint(QPainter::SmoothPixmapTransform);

    // Set the sceneRect to match the pixmap's dimensions
    scene->setSceneRect(pixmap.rect());

    // Create a QGraphicsPixmapItem to display the QPixmap
    QGraphicsPixmapItem * pixmapItem = scene->addPixmap(pixmap);

    // Fit the view to the pixmap while preserving the aspect ratio
    ui->view->fitInView(pixmapItem, Qt::IgnoreAspectRatio);

    QImage image = pixmap.toImage();
    auto width  = image.width();   // width of map
    auto height = image.height();  // height of map

    map = std::vector<std::vector<bool>>(height, std::vector<bool>(width));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            map[y][x] = is_black(image.pixel(x, y));
        }
    }

    ui->view->setScene(scene);
}

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow), key(NONE)
{
    setFocusPolicy(Qt::StrongFocus);

    setup_UI();

    angles = {-1.57079632679,-0.78539816339, 0, 0.78539816339, 1.57079632679};
    z_t.resize(angles.size());
    lasers.resize(angles.size());

    localisation = std::make_unique<Localisation>(map, angles);

    setup_robot();

    timer = new QTimer(this);

    setup_signals();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W) key = UP;
    else if(event->key() == Qt::Key_S) key = DOWN;
    else if(event->key() == Qt::Key_A) key = LEFT;
    else if(event->key() == Qt::Key_D) key = RIGHT;
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if     (event->key() == Qt::Key_W && key == 1) key = NONE;
    else if(event->key() == Qt::Key_S && key == 2) key = NONE;
    else if(event->key() == Qt::Key_A && key == 3) key = NONE;
    else if(event->key() == Qt::Key_D && key == 4) key = NONE;
}

void MainWindow::update_robot()
{
    u_t = {};
    double dist = 0.0;

    switch(key) {
        case UP:
            dist = 10.0;
            break;
        case DOWN:
            dist = -10.0;
            break;
        case LEFT:
            robot.h += u_t[2] = -0.261799388 / 2;
            break;
        case RIGHT:
            robot.h += u_t[2] =  0.261799388 / 2;
    }

    robot.x += u_t[0] = cos(robot.h) * dist;
    robot.y += u_t[1] = sin(robot.h) * dist;

    scene->removeItem(robot_item.get());
    robot_item = std::make_unique<QGraphicsEllipseItem>(robot.x, robot.y, 6, 6);
    robot_item->setBrush(QBrush(Qt::green));

    localisation->measure_particle(robot);
    z_t = robot.distance;
    for(unsigned int i = 0; i < angles.size() ; i++) {
        double x2 = robot.x + cos(robot.h + angles[i]) * z_t[i];
        double y2 = robot.y + sin(robot.h + angles[i]) * z_t[i];
        lasers[i] = std::make_unique<QGraphicsLineItem>(robot.x + 3, robot.y + 3, x2, y2);
        lasers[i]->setPen(QPen(Qt::green));
    }
}

void MainWindow::draw_robot()
{
    scene->addItem(robot_item.get());
    for(const auto & laser : lasers) {
        scene->addItem(laser.get());
    }
}

void MainWindow::update()
{
    auto start = std::chrono::high_resolution_clock::now();

    update_robot();
    update_canvas();

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Time taken by MCL: " << duration.count() << " microseconds" << std::endl;

    if (duration.count() / 1000 > timer->interval()) {
        this->timer->stop();

        QMessageBox::information(this, "Calculation is taking too long",
                                 "Calculation takes longer than the time set on the timer. "
                                 "Either increase time interval, increase number of threads "
                                 "or decrease the number of particles.");
    }
}


void MainWindow::handleMouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // Get the position of the mouse click
    QPointF position = event->scenePos();
    double x = position.x();
    double y = position.y();

    if(x != robot.x && y != robot.y) {
        robot.h = atan2(y - robot.y, x - robot.x);
    }

    update_robot();
    draw_robot();
}

void MainWindow::startTimer(int time)
{
    allocate_particles(ui->particleSpinbox->value());

    timer->stop(); // stop previous timer
    timer->start(time);
    generate_particles();
}

void MainWindow::loadMaze()
{
    QString path;
    QString name_of_file = QFileDialog::getOpenFileName(this, "Open file", path, "PNG image (*.png)");
    QFileInfo i(name_of_file);

    if(name_of_file.isEmpty()) {
        return;
    }

    QPixmap pixmap(i.absoluteFilePath());

    load_map(pixmap);
    //ui->view->scale(1.0, 0.92);
    localisation->set_map(map);
}

void MainWindow::load_settings()
{
    Dialog dialog;
    if (dialog.exec() == QDialog::Accepted){
        auto [a_slow, a_fast, epsilon, delta, min, max] = dialog.get_dialog_data();
        this->localisation->set_paramaters(a_slow, a_fast, epsilon, delta, min, max);
    }
}

void MainWindow::handleMousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Get the position of the mouse click
    QPointF position = event->scenePos();

    double x = position.x(), y = position.y();

    if (map[y][x]) {
        return;
    }

    robot.x = x;
    robot.y = y;
}

void MainWindow::setup_robot()
{
    robot = Particle(800, 300, 0);
    robot_item = std::make_unique<QGraphicsEllipseItem>(800,300,6,6);
    robot_item->setBrush(QBrush(Qt::green));
    scene->addItem(robot_item.get());

    for (auto & laser : lasers) {
        laser = std::make_unique<QGraphicsLineItem>();
    }

    robot_estimate_item = nullptr;

    update_robot();
    draw_robot();
}

void MainWindow::setup_lasers(int number_of_lasers)
{
    const std::vector<std::vector<double>> laser_angles = {
            {0},
            {0, 3.14159265359},
            {-0.78539816339, 0, 0.78539816339},
            {-0.78539816339, 0, 0.78539816339, 3.14159265359},
            {-1.57079632679,-0.78539816339, 0, 0.78539816339, 1.57079632679},
            {-1.57079632679,-0.78539816339, 0, 0.78539816339, 1.57079632679, 3.14159265359},
            {-2.35619449019,-1.57079632679,-0.78539816339, 0, 0.78539816339, 1.57079632679, 2.35619449019},
            {-2.35619449019,-1.57079632679,-0.78539816339, 0, 0.78539816339, 1.57079632679, 2.35619449019, 3.14159265359},
    };

    angles = laser_angles[number_of_lasers - 1];
    localisation->set_angles(angles);

    lasers.resize(number_of_lasers);
    setup_robot();
}

void MainWindow::generate_particles()
{
    localisation->generate_particles(particles, Problem(ui->problem_comboBox->currentIndex()), robot.getPose());
    for(unsigned int i  = 0; i < particles.size(); i++) {
        particle_items[i]->setPos(QPointF(particles[i].x, particles[i].y));
    }
}

void MainWindow::update_canvas()
{
    int option = ui->problem_comboBox->currentIndex();
    auto [x, y, h] = localise(option);

    unsigned int nop = std::min(500, static_cast<int>(particles.size()));
    std::partial_sort(particles.begin(), std::next(particles.begin(), nop), particles.end(), [](const Particle& p1, const Particle& p2) {
        return p1.weight > p2.weight;
    });

    for (unsigned int i = 0; i < 500; i++) {
        if (i >= particle_items.size()) {
            scene->removeItem(particle_items[i].get());
        }

        if (particles.size() >= i) {
            auto & item = particle_items[i];
            item = std::make_unique<QGraphicsEllipseItem>(particles[i].x, particles[i].y, 3, 3);
            item->setBrush(QBrush(Qt::red));
            item->setPen(QPen(Qt::red));
            scene->addItem(item.get());
        }
    }

    scene->removeItem(robot_estimate_item.get());
    robot_estimate_item = std::make_unique<QGraphicsEllipseItem>(x, y, 9, 9);
    robot_estimate_item->setBrush(QBrush(Qt::blue));
    scene->addItem(robot_estimate_item.get());

    draw_robot();
}

Pose MainWindow::localise(int option)
{
    switch(option) {
        case Problem::LOCAL:
        case Problem::GLOBAL:
            return localisation->mcl(particles, u_t, z_t);
        case Problem::KIDNAPPED_ROBOT:
            return localisation->augmented_mcl(particles, u_t, z_t);
        case Problem::MMCL:
            return localisation->mmcl(particles, u_t, z_t);
        default:
            return {};
    }
}
