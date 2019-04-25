#include "resizefocus.h"
#include <QCursor>

ResizeFocus::ResizeFocus(qreal x, qreal y, PosInHost pos, QGraphicsItem *parent)
	: QGraphicsRectItem(x, y, 6, 6, parent)
	, m_posInHost(pos)
	, m_wsize(6)
{
	setAcceptHoverEvents(true);//表示该项接收悬停事件
	setVisible(false);//表示该项不可见鼠标事件直接通过不可见的项传递，并传递到后面的任何项，不可修改

	locateInHost();
}

ResizeFocus::ResizeFocus(qreal x, qreal y, qreal len, PosInHost pos, QGraphicsItem *parent)
	: QGraphicsRectItem(x, y, len, len, parent)
	, m_posInHost(pos)
	, m_wsize(len)
{
	setAcceptHoverEvents(true);
	setVisible(false);

	locateInHost();
}

ResizeFocus::~ResizeFocus()
{
}

void ResizeFocus::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
//接收此项的悬停enter事件
{
	switch (m_posInHost)
	{
		case NORTH_MIDDLE:
			setCursor(Qt::SizeVerCursor);
			break;
		case SOUTH_MIDDLE:
			setCursor(Qt::SizeVerCursor);
			break;
		case EAST_MIDDLE:
			setCursor(Qt::SizeHorCursor);
			break;
		case WEST_MIDDLE:
			setCursor(Qt::SizeHorCursor);
			break;
		case NORTH_WEST:
			setCursor(Qt::SizeFDiagCursor);
			break;
		case SOUTH_EAST:
			setCursor(Qt::SizeFDiagCursor);
			break;
		case NORTH_EAST:
			setCursor(Qt::SizeBDiagCursor);
			break;
		case SOUTH_WEST:
			setCursor(Qt::SizeBDiagCursor);
			break;
		case NORTH_MIDDLE_UP:
			setCursor(Qt::CrossCursor);
			break;
		default:
			break;
	}
	QGraphicsRectItem::hoverEnterEvent(event);
}

void ResizeFocus::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
//处理此项的悬停离开事件
{
	QGraphicsRectItem::hoverLeaveEvent(event);
}

void ResizeFocus::locateInHost()
{
	const QRectF parentRect = this->parentItem()->boundingRect();//返回该项父项的指针的边界矩形
	qreal x = 0, y = 0;
	switch(m_posInHost)
	{
		case NORTH_MIDDLE:
			x = parentRect.width() / 2 - m_wsize / 2;
			y = 0;
			break;
		case SOUTH_MIDDLE:
			x = parentRect.width() / 2 - m_wsize / 2;
			y = parentRect.height() - m_wsize;
			break;
		case EAST_MIDDLE:
			x = parentRect.width() - m_wsize;
			y = parentRect.height() / 2 - m_wsize / 2;
			break;
		case WEST_MIDDLE:
			x = 0;
			y = parentRect.height() / 2 - m_wsize / 2;
			break;
		case NORTH_WEST:
			x = 0;
			y = 0;
			break;
		case SOUTH_EAST:
			x = parentRect.width() - m_wsize;
			y = parentRect.height() - m_wsize;
			break;
		case NORTH_EAST:
			x = parentRect.width() - m_wsize;
			y = 0;
			break;
		case SOUTH_WEST:
			x = 0;
			y = parentRect.height() - m_wsize;
			break;
		case NORTH_MIDDLE_UP:
			x = parentRect.width() / 2 - m_wsize / 2;
			y = -6;
			break;
		default:
			break;
	}
	setPos(x, y);
}