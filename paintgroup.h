#ifndef PAINTGROUP_H
#define PAINTGROUP_H
#include <QGraphicsItemGroup>
#include "resizefocus.h"
#include <QList>

class PaintGroup : public QGraphicsItemGroup
{
public:
	enum MouseMode{MOVE, RESIZE};
	enum { PaintGroupType = UserType + 102 };
	PaintGroup(QGraphicsItem *parent = 0);
	~PaintGroup();

	virtual int type() const override
	{
		return PaintGroupType;
	}
	
protected:

	void showResizeFocus(bool visible);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	QRectF boundingRect() const;

	void mousePressEvent(QGraphicsSceneMouseEvent *) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;

private:
	void createResizeFocus();
	void calculatedAngle();
	void rotatePaintGroup();
	void changeSize(ResizeFocus::PosInHost pos, qreal curX, qreal curY, qreal curWidth, qreal curHeight, \
		qreal hChanging, qreal wChanging);
	void addResizeFocus(ResizeFocus::PosInHost pos);
	void changeInsideItemSize();
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
	qreal m_margin;
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
