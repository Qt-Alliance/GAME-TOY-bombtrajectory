#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QWidget>
#include <QList>
#include <QColor>

class QSvgRenderer;
class FlingModel;
class DisplayWidget : public QWidget
{
    friend class MainWindow;
Q_OBJECT
public:
    explicit DisplayWidget(QWidget *parent = 0);
    void setModel(FlingModel* model);
    FlingModel* model() const;

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void resizeEvent(QResizeEvent* event);

public slots:
    void updateGravity(double gravity);
    void updateAreaWidth(double width);
    void updateAreaHeight(double height);

private:
    QRectF m_personRect;
    QSvgRenderer* m_renderer;
    FlingModel* m_model;
    QPointF m_offsetPos;
    double m_height; // in meters
    double m_width; // in meters
    double m_gravity;
    QPointF m_personRelativePos;

    void writeSettings();
    void readSettings();
};

#endif // DISPLAYWIDGET_H
