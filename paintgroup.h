#ifndef PAINTGROUP_H
#define PAINTGROUP_H
#include <QGraphicsItemGroup>
#include "resizefocus.h"
#include <QList>

class PaintGroup : public QGraphicsItemGroup
{
public:
	enum MouseMode{MOVE, RESIZE};
	enum { Type = UserType + 102 };
	PaintGroup(QGraphicsItem *parent = 0);
	~PaintGroup();

	virtual int type() const
	{
		return Type;
	}
	void mousePressEvent(QGraphicsSceneMouseEvent *);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *);

	void showResizeFocus(bool visible);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	QRectF boundingRect() const;

private:
	void createResizeFocus();
	QList<ResizeFocus *> m_resizeFocus;
	ResizeFocus *m_curResizeFocus;
	bool m_isRotate;
	QPointF m_startRotate;
	QPointF m_endRotate;
	QPointF m_centerRotate;
	QPointF m_lastPoint;
	MouseMode m_mode;
	QGraphicsRectItem *m_dashRect;
	qreal m_width;
	qreal m_height;
	qreal m_xPos;
	qreal m_yPos;
	qreal m_angle;
	ResizeFocus::PosInHost m_pos;
	qreal m_widthScale;
	qreal m_heightScale;
	bool m_isFirstPress;
	int m_count;
};

#endif // PAINTGROUP_H
