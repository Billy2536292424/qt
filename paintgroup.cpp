#include "paintgroup.h"
#include "paintscene.h"
#include <cmath>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QGraphicsRectItem>
#include "nodeellipse.h"
#include "noderect.h"

PaintGroup::PaintGroup(QGraphicsItem *parent)
	: QGraphicsItemGroup(parent)
	, m_curResizeFocus(NULL)
	, m_isRotate(false)
	, m_mode(MOVE)
	, m_dashRect(NULL)
	, m_width(0)
	, m_height(0)
	, m_margin(6)
	, m_xPos(0)
	, m_yPos(0)
	, m_angle(0)
	, m_pos(ResizeFocus::OTHER)
	, m_widthScale(0)
	, m_heightScale(0)
	, m_isFirstPress(false)
	, m_count(0)
{
	setAcceptDrops(true);
	setAcceptHoverEvents(true);
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
	setHandlesChildEvents(true);
}

PaintGroup::~PaintGroup()
{}

void PaintGroup::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (m_isFirstPress == false)
	{
		m_width = QGraphicsItemGroup::boundingRect().width();
		m_height = QGraphicsItemGroup::boundingRect().height();
		m_xPos = QGraphicsItemGroup::boundingRect().x();
		m_yPos = QGraphicsItemGroup::boundingRect().y();
		createResizeFocus();
		m_isFirstPress = true;
	}
	m_count = 0;
	QTransform trans;
	m_curResizeFocus = qgraphicsitem_cast<ResizeFocus *>(scene()->itemAt(event->scenePos(), trans));
	if (m_curResizeFocus)
	{
		ResizeFocus::PosInHost pos = m_curResizeFocus->getInHost();
		if (event->button() == Qt::LeftButton && pos == ResizeFocus::NORTH_MIDDLE_UP)
		{
			m_isRotate = true;
			m_mode = MOVE;
			m_startRotate = event->scenePos();
		}
		else
		{
			m_mode = RESIZE;
			m_isRotate = false;
			m_lastPoint.setX(event->scenePos().x());
			m_lastPoint.setY(event->scenePos().y());
			m_dashRect = new QGraphicsRectItem();
			m_dashRect->setPen(Qt::DashLine);
			m_dashRect->setParentItem(this);
			m_dashRect->setRect(m_xPos + 6, m_yPos + 6, m_width - 12, m_height - 12);
		}
	}
	else
	{
		m_mode = MOVE;
		QGraphicsItem::mousePressEvent(event);
	}
}

void PaintGroup::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton && m_isRotate)
	{
		m_count++;
		if (m_count % 2 == 1)
		{
			m_endRotate = event->scenePos();
			m_centerRotate = boundingRect().center();
			calculatedAngle();
			rotatePaintGroup();
		}
	}
	else if (m_mode == RESIZE)
	{
		QPointF curPoint(event->scenePos());
		qreal curX = m_xPos + 6, curY = m_yPos + 6, curWidth = m_width - 6 * 2, curHeight = m_height - 6 * 2;
		qreal wChanging = curPoint.x() - m_lastPoint.x(),\
				hChanging = curPoint.y() - m_lastPoint.y();
		ResizeFocus::PosInHost pos = m_curResizeFocus->getInHost();
		changeSize(pos, curX, curY, curWidth, curHeight, wChanging, hChanging);
	}
	else
		QGraphicsItem::mouseMoveEvent(event);
}

void PaintGroup::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if (m_isRotate)
	{
		m_isRotate = false;
	}

	if (m_mode == RESIZE)
	{
		qreal curWidth = boundingRect().width();
		qreal curHeight = boundingRect().height();
		m_width = m_dashRect->rect().width() + 6 * 2;
		m_height = m_dashRect->rect().height() + 6 * 2;
		m_widthScale = m_width / curWidth;
		m_heightScale = m_height / curHeight;
		qreal curX = m_dashRect->rect().left() - 6;
		qreal curY = m_dashRect->rect().top() - 6;
		QRectF rectRet = m_dashRect->rect();
		QPoint curPos(curX, curY);
		setPos(curPos);
		changeInsideItemSize();
	}
		else
			QGraphicsItem::mouseReleaseEvent(event);
}

void PaintGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(painter);
	Q_UNUSED(option);
	Q_UNUSED(widget);
	if (option->state & QStyle::State_Selected)
	{
		showResizeFocus(true);
	}
	else
	{
		showResizeFocus(false);
	}
}

void PaintGroup::showResizeFocus(bool visible)
{
	for (int i = 0; i < m_resizeFocus.count(); i++)
	{
		m_resizeFocus.at(i)->locateInHost();
		m_resizeFocus.at(i)->setVisible(visible);
	}
}

void PaintGroup::createResizeFocus()
{
	int x = boundingRect().x();
	int y = boundingRect().y();
	addResizeFocus(ResizeFocus::NORTH_MIDDLE);
	addResizeFocus(ResizeFocus::NORTH_EAST);
	addResizeFocus(ResizeFocus::NORTH_WEST);
	addResizeFocus(ResizeFocus::SOUTH_MIDDLE);
	addResizeFocus(ResizeFocus::SOUTH_EAST);
	addResizeFocus(ResizeFocus::SOUTH_WEST);
	addResizeFocus(ResizeFocus::EAST_MIDDLE);
	addResizeFocus(ResizeFocus::WEST_MIDDLE);
	addResizeFocus(ResizeFocus::NORTH_MIDDLE_UP);
}

QRectF PaintGroup::boundingRect() const
{
	qreal penWidth = 1;
	return QRectF(m_xPos - penWidth / 2, m_yPos - penWidth / 2, \
		m_width + penWidth, m_height + penWidth);
}

void PaintGroup::calculatedAngle()
{
	qreal oneEdge = sqrt(pow(m_centerRotate.x() - m_startRotate.x(), 2) + \
		pow(m_centerRotate.y() - m_startRotate.y(), 2));
	qreal otherEdge = sqrt(pow(m_centerRotate.x() - m_endRotate.x(), 2) + \
		pow(m_centerRotate.y() - m_endRotate.y(), 2));
	qreal oppositeSide = sqrt(pow(m_endRotate.x() - m_startRotate.x(), 2) + \
		pow(m_endRotate.y() - m_startRotate.y(), 2));
	qreal cosValue = (pow(oneEdge, 2) + pow(otherEdge, 2) - pow(oppositeSide, 2))\
		/ (2 * oneEdge * otherEdge);//余弦定理
	qreal angle = acos(cosValue);
	qreal dx1 = m_startRotate.x() - m_centerRotate.x();
	qreal dy1 = m_startRotate.y() - m_centerRotate.y();
	qreal dx2 = m_endRotate.x() - m_centerRotate.x();
	qreal dy2 = m_endRotate.y() - m_centerRotate.y();
	if (dx1 * dy2 - dy1 * dx2 > 0)
	{	//右转  顺时针
		m_angle += angle;
	}
	else
	{	//左转   逆时针
		m_angle -= angle;
	}
}

void PaintGroup::rotatePaintGroup()
{
	resetTransform();
	QTransform transformf = transform();
	int x = boundingRect().center().x();
	int y = boundingRect().center().y();
	transformf.translate(x, y);
	transformf.rotate(m_angle, Qt::ZAxis);
	transformf.translate(-x, -y);
	setTransform(transformf);
}

void PaintGroup::changeSize(ResizeFocus::PosInHost pos, qreal curX, qreal curY, qreal curWidth, qreal curHeight, \
	qreal wChanging, qreal hChanging)
{
	switch (pos)
	{
	case ResizeFocus::NORTH_MIDDLE:
		curY += hChanging;
		curHeight -= hChanging;
		break;
	case ResizeFocus::SOUTH_MIDDLE:
		curHeight += hChanging;
		break;
	case ResizeFocus::EAST_MIDDLE:
		curWidth += wChanging;
		break;
	case ResizeFocus::WEST_MIDDLE:
		curX += wChanging;
		curWidth -= wChanging;
		break;
	case ResizeFocus::NORTH_WEST:
		curX += wChanging;
		curY += hChanging;
		curWidth -= wChanging;
		curHeight -= hChanging;
		break;
	case ResizeFocus::SOUTH_EAST:
		curWidth += wChanging;
		curHeight += hChanging;
		break;
	case ResizeFocus::NORTH_EAST:
		curY += hChanging;
		curWidth += wChanging;
		curHeight -= hChanging;
		break;
	case ResizeFocus::SOUTH_WEST:
		curX += wChanging;
		curWidth -= wChanging;
		curHeight += hChanging;
		break;
	default:
		break;
	}
	if (curWidth < 20 || curHeight < 20)//minimal size
		return;
	m_dashRect->setRect(curX, curY, curWidth, curHeight);
}

void PaintGroup::addResizeFocus(ResizeFocus::PosInHost pos)
{
	ResizeFocus *focus = new ResizeFocus(m_xPos, m_yPos, m_margin, pos, this);
	m_resizeFocus.append(focus);
}

void PaintGroup::changeInsideItemSize()
{
	QList<QGraphicsItem *> ret = this->childItems();
	QGraphicsRectItem *item = new QGraphicsRectItem();

	for (int i = 0; i < ret.count(); i++)
	{
		if (ret.at(i)->type() == 65637)
		{
			//椭圆
			dynamic_cast<NodeEllipse *>(ret.at(i))->setValue(m_widthScale, m_heightScale);
		}
		else if (ret.at(i)->type() == 65636)
		{
			//矩形
			dynamic_cast<NodeRect *>(ret.at(i))->setValue(m_widthScale, m_heightScale);
		}
	}
	delete m_dashRect;
}