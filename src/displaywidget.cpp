#include "displaywidget.h"
#include "flingmodel.h"

#include <cmath>

// Qt
#include <QPainter>
#include <QDebug>
#include <QSvgRenderer>
#include <QMouseEvent>
#include <QSettings>

DisplayWidget::DisplayWidget(QWidget *parent)
    : QWidget(parent)
    , m_personRect(0, 0, 32, 32)
    , m_renderer(0)
    , m_model(0)
    , m_offsetPos(QPointF())
    , m_height(30)
    , m_width(50)
    , m_gravity(9.8)
{
    m_renderer = new QSvgRenderer(this);
    m_renderer->load(QString(":/images/person.svg"));
    setMouseTracking(true);
    readSettings();
}

void DisplayWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    QRectF cr = contentsRect();
    p.translate(0.5, 0.5);

    // Paint achsis
    const qreal margin = 2;
    cr.adjust(margin, margin, -margin, -margin);

    p.setPen(QPen(Qt::lightGray, 0.5));
    for(int i = 1; i <= m_width; ++i) { // vertical lines
        qreal x1 = cr.left() + qreal(cr.width()) / m_width * i;
        p.drawLine(x1, cr.top(), x1, cr.bottom());
    }

    for(int i = 1; i <= m_height; ++i) { // horizontal lines
        qreal y = cr.bottom() - float(cr.height()) / m_height * i;
        p.drawLine(cr.left(), y, cr.right(), y);
    }

    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(QPen(Qt::black, 1));
    p.drawLine(cr.topLeft(), cr.bottomLeft());
    p.drawLine(cr.bottomLeft(), cr.bottomRight());
    p.setBrush(Qt::black);

    // draw arrows
    QPainterPath path(cr.topLeft());
    path.lineTo(cr.left() + 5, cr.top() + 10);
    path.lineTo(cr.left(), cr.top() + 10);
    path.closeSubpath();
    p.drawPath(path);

    path = QPainterPath(cr.bottomRight());
    path.lineTo(cr.right() - 10, cr.bottom() - 5);
    path.lineTo(cr.right() - 10, cr.bottom());
    path.closeSubpath();
    p.drawPath(path);

    // draw indices
    // TODO

    // draw curves
    const qreal xunit = cr.width() / m_width;
    const qreal yunit = cr.height() / m_height;

    if (m_model) {
        foreach(Fling* f, m_model->flings()) {
            p.setPen(QPen(f->color, 1, Qt::DashLine));
            qreal t = 0;
            qreal xpos = 0;
            qreal ypos = 0;
            qreal xoffset = m_personRect.center().x();
            qreal yoffset = m_personRect.center().y();

            QPointF lastPoint(m_personRect.center());
            while (yoffset - ypos <= cr.bottom() && xoffset + xpos <= cr.right()) {
                t += 0.05;
                xpos = (f->velocity * cos(f->angle * M_PI / 180.0) * t) * xunit;
                ypos = (f->velocity * sin(f->angle * M_PI / 180.0) * t + 0.5 * -m_gravity * (t * t)) * yunit;
                QPointF point(xoffset + xpos, yoffset - ypos);
                p.drawLine(lastPoint, point);
                lastPoint = point;
            }
        }
    }

    // paint player icons
    m_renderer->render(&p, m_personRect);
}

void DisplayWidget::mousePressEvent(QMouseEvent* event)
{
  if (event->button() != Qt::LeftButton || !m_personRect.contains(event->posF()))
    return;

  m_offsetPos = event->posF() - m_personRect.topLeft();
  setCursor(Qt::ClosedHandCursor);
  update();
}

void DisplayWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_offsetPos.isNull()) {
        m_personRect.contains(event->posF()) ? setCursor(Qt::OpenHandCursor) : setCursor(Qt::ArrowCursor);

       return;
    }

  setCursor(Qt::ClosedHandCursor);
  QPointF pos = event->posF() - m_offsetPos;
  m_personRect.moveTopLeft(pos);
  QRectF cr = contentsRect();

  if (m_personRect.top() < cr.top())       m_personRect.moveTop(cr.top());
  if (m_personRect.bottom() > cr.bottom()) m_personRect.moveBottom(cr.bottom());
  if (m_personRect.right() > cr.right())   m_personRect.moveRight(cr.right());
  if (m_personRect.left() < cr.left())     m_personRect.moveLeft(cr.left());

  m_personRelativePos = QPointF(qreal(m_personRect.left()) / cr.width(),
                                qreal(m_personRect.top()) / cr.height());
  update();
}

void DisplayWidget::mouseReleaseEvent(QMouseEvent* event)
{
  Q_UNUSED(event);
  m_personRect.contains(event->posF()) ? setCursor(Qt::OpenHandCursor) : setCursor(Qt::ArrowCursor);
  m_offsetPos = QPoint();

  update();
}

void DisplayWidget::setModel(FlingModel* model)
{
    if (model == m_model)
        return;
    m_model = model;
    update();
}

FlingModel* DisplayWidget::model() const
{
    return m_model;
}
void DisplayWidget::updateGravity(double gravity)
{
    if (gravity == m_gravity)
        return;
    m_gravity = gravity;
    update();
}

void DisplayWidget::updateAreaHeight(double height)
{
    if (height == m_height)
        return;
    m_height = height;
    update();
}

void DisplayWidget::updateAreaWidth(double width)
{
    if (width == m_width)
        return;
    m_width = width;
    update();
}

void DisplayWidget::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    m_personRect.moveTop(height() * m_personRelativePos.y());
    m_personRect.moveLeft(width() * m_personRelativePos.x());
}

void DisplayWidget::writeSettings()
{
    QSettings s;
    s.setValue("displaywidget/personrelativepos", m_personRelativePos);
}

void DisplayWidget::readSettings()
{
    QSettings s;
    m_personRelativePos =  s.value("displaywidget/personrelativepos").toPointF();
}
