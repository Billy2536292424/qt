#ifndef RESIZEFOCUS_H
#define RESIZEFOCUS_H
#include <QGraphicsRectItem>

class ResizeFocus : public QGraphicsRectItem
{
public:
	enum PosInHost{NORTH_MIDDLE, NORTH_EAST, EAST_MIDDLE, SOUTH_EAST, SOUTH_MIDDLE, SOUTH_WEST, WEST_MIDDLE, NORTH_WEST, NORTH_MIDDLE_UP, OTHER};
	enum {Type = UserType + 1};
	ResizeFocus(qreal x, qreal y, PosInHost pos, QGraphicsItem *parent);
	ResizeFocus(qreal x, qreal y, qreal len, PosInHost pos, QGraphicsItem *parent);
	~ResizeFocus();

	int type() const
	{
		return Type;
	}

	void setInHost(PosInHost pos)
	{
		m_posInHost = pos;
	}

	PosInHost getInHost()
	{
		return m_posInHost;
	}

	void locateInHost();

protected:
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
	PosInHost m_posInHost;
	qreal m_wsize;
};

#endif // RESIZEFOCUS_H
