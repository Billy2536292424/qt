#ifndef NODEELLIPSE_H
#define NODEELLIPSE_H
#include <QtWidgets>
#include <QSet>
#include "resizefocus.h"

class NodeEllipse : public QGraphicsItem
{
public:
	enum MouseMode {MOVE, RESIZE};
	enum {Type = UserType + 101}; //65637
	//UserType自定义项(QGraphicsItem或任何标准项的子类)的最低允许类型值

	NodeEllipse(QGraphicsItem *parent = 0);
	NodeEllipse(int x, int y, int width, int height, int margin, QGraphicsItem *parent = 0);
	~NodeEllipse();

	virtual int type() const
	{
		return Type;
	}

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	QPainterPath shape() const;

	void setValue(qreal widthScale, qreal heightScale);

protected:

	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void showResizeFocus(bool visible);

private:

	int m_xPos;
	int m_yPos;
	int m_width;
	int m_height;
	int m_margin;
	QRectF m_boundingRect;
	void createResizeFocus();

	MouseMode m_mode;
	QColor m_color;
	ResizeFocus *m_curResizeFocus;
	QPointF m_lastPoint;
	QGraphicsRectItem *m_dashRect;
	QList<ResizeFocus *> m_resizeFocus;
	bool m_isRotate;
	QPointF m_startRotate;
	QPointF m_endRotate;
	QPointF m_centerRotate;
	qreal m_angle;
	int m_count;
};

#endif // NODEELLIPSE_H
