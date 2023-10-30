#ifndef MYSCENE_HPP
#define MYSCENE_HPP

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class MyScene : public QGraphicsScene
{
Q_OBJECT

public:
    explicit MyScene(QObject *parent = nullptr) : QGraphicsScene(parent) {}

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override {
        emit mousePressEventSignal(event);
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override {
        emit mouseReleaseEventSignal(event);
    }

signals:
    void mousePressEventSignal(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEventSignal(QGraphicsSceneMouseEvent *event);
};

#endif // MYSCENE_HPP

