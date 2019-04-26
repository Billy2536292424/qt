#ifndef NODERECT_H
#define NODERECT_H
#include <QtWidgets>
#include <QSet>
#include "resizefocus.h"

class NodeRect : public QGraphicsItem
{
public:
	enum MouseMode{MOVE, RESIZE};
	enum { NodeRectType = UserType + 100 };//65636

	NodeRect(QGraphicsItem *parent = 0);
	NodeRect(int x, int y, int width, int height, int margin, QGraphicsItem *parent = 0);
	~NodeRect();

	void setValue(qreal width, qreal height);

	virtual int type() const override
	{
		return NodeRectType;
	}

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	QPainterPath shape() const;
protected:

	void mousePressEvent(QGraphicsSceneMouseEvent *) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;
	void showResizeFocus(bool visible);

private:
	int m_xPos;
	int m_yPos;
	int m_width;
	int m_height;
	int m_margin;
	QRectF m_boundingRect;
	void createResizeFocus();
	void calculatedAngle();
	void rotateNodeRect();
	void changeSize(ResizeFocus::PosInHost pos, qreal curX, qreal curY, qreal curWidth, qreal curHeight,\
	qreal hChanging, qreal wChanging);
	void addResizeFocus(ResizeFocus::PosInHost pos);

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

#endif // NODERECT_H
