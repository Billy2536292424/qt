#include "nodeellipse.h"
#include <QPainter>
#include <cmath>

NodeEllipse::NodeEllipse(QGraphicsItem *parent)
	: m_xPos(0)
	, m_yPos(0)
	, m_width(0)
	, m_height(0)
	, m_margin(6)
	, m_isRotate(false)
	, m_angle(0)
	, m_count(0)
	, QGraphicsItem(parent)
{
	setAcceptDrops(true);//表示接受拖放事件
	setAcceptHoverEvents(true);//表示接受悬停事件
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
	//项可以移动 | 项可以选中 | 允许项目接受焦点
	createResizeFocus();
}
NodeEllipse::NodeEllipse(int x, int y, int width, int height, int margin, QGraphicsItem *parent)
	: m_xPos(x)
	, m_yPos(y)
	, m_width(width)
	, m_height(height)
	, m_margin(margin)
	, m_isRotate(false)
	, m_angle(0)
	, m_count(0)
	, QGraphicsItem(parent)
{
	setAcceptDrops(true);
	setAcceptHoverEvents(true);
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
	createResizeFocus();
}

NodeEllipse::~NodeEllipse()
{}

QRectF NodeEllipse::boundingRect() const
{
	qreal penWidth = 1;
	return QRectF(m_xPos - penWidth / 2, m_yPos -  penWidth / 2, m_width + penWidth,\
		m_height + penWidth);
}

void NodeEllipse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	update();
	Q_UNUSED(widget);
	QColor fillColor = QColor(100, 100, 255);
	if (option->state & QStyle::State_Selected)//指示小部件是否被选择
		fillColor = fillColor.darker(150);
	if (option->state & QStyle::State_MouseOver)//指示小部件是否在鼠标下
		fillColor = fillColor.lighter(125);
	if (option->state & QStyle::State_Selected)
	{
		painter->setBrush(fillColor);
		painter->drawEllipse(m_xPos + m_margin, m_yPos + m_margin, m_width - 2 * m_margin, m_height - 2 * m_margin);
		showResizeFocus(true);
	}
	else
	{
		painter->setBrush(fillColor);
		painter->drawEllipse(m_xPos + m_margin, m_yPos + m_margin, m_width - 2 * m_margin, m_height - 2 * m_margin);
		showResizeFocus(false);
	}
}

QPainterPath NodeEllipse::shape() const
{
	QPainterPath path;
	path.addRect(boundingRect());
	return path;
}
void NodeEllipse::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QTransform trans;
	m_curResizeFocus = qgraphicsitem_cast<ResizeFocus *> (scene()->itemAt(event->scenePos(), trans));
	if (m_curResizeFocus)
	{
		ResizeFocus::PosInHost pos = m_curResizeFocus->getInHost();
		if (event->button() == Qt::LeftButton && pos == ResizeFocus::NORTH_MIDDLE_UP)
		{
			m_isRotate = true;
			m_mode = MOVE;
			m_count = 0;
			m_startRotate = event->scenePos();
		}
		else
		{
			m_mode = RESIZE;
			m_isRotate = false;
			m_lastPoint.setX(event->scenePos().x());
			m_lastPoint.setY(event->scenePos().y());
			m_dashRect = new QGraphicsRectItem();
			m_dashRect->setPen(QPen(Qt::DashLine));
			m_dashRect->setParentItem(this);
			m_dashRect->setRect(m_xPos + m_margin, m_yPos + m_margin, m_width - m_margin * 2, m_height - m_margin * 2);
		}
	}
	else
	{
		m_mode = MOVE;
		QGraphicsItem::mousePressEvent(event);
	}
}

void NodeEllipse::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_isRotate)
	{
		m_count++;
		if (m_count % 2 == 1)
		{
			m_endRotate = event->scenePos();
			m_centerRotate = boundingRect().center();
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
			if (dx1 * dy2 - dy1 * dx2 > 0) //使用向量积判断旋转方向
			{
				//右转 顺时针
				m_angle += angle;
			}
			else
			{
				//左转   逆时针
				m_angle -= angle;
			}
			resetTransform();
			QTransform transformf = transform();
			int x = boundingRect().center().x();
			int y = boundingRect().center().y();
			transformf.translate(x, y);
			transformf.rotate(m_angle, Qt::ZAxis);
			transformf.translate(-x, -y);
			setTransform(transformf);
		}
	}
	else if (m_mode == RESIZE)
	{
		QPointF curPoint(event->scenePos());
		qreal curX = m_xPos + m_margin, curY = m_yPos + m_margin, curWidth = m_width - 2 * m_margin, curHeight = m_height - 2 * m_margin;
		qreal wChanging = curPoint.x()- m_lastPoint.x(),
				hChanging = curPoint.y() - m_lastPoint.y();
		ResizeFocus::PosInHost pos = m_curResizeFocus->getInHost();
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
	else
		QGraphicsItem::mouseMoveEvent(event);
}

void NodeEllipse::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if (m_isRotate)
		m_isRotate = false;
	if (m_mode == RESIZE)
	{
		m_width = m_dashRect->rect().width() + m_margin * 2;
		m_height = m_dashRect->rect().height() + m_margin * 2;
		qreal curX = m_dashRect->rect().left() - m_margin;
		qreal curY = m_dashRect->rect().top() - m_margin;
		QPoint curPos(curX, curY);
		setPos(curPos);
		delete m_dashRect;
	}
	else
		QGraphicsItem::mouseReleaseEvent(event);
}

void NodeEllipse::showResizeFocus(bool visible)
{
	for (int i = 0; i < m_resizeFocus.count(); i++)
	{
		m_resizeFocus.at(i)->locateInHost();
		m_resizeFocus.at(i)->setVisible(visible);
	}
}

void NodeEllipse::createResizeFocus()
{
	ResizeFocus *north_middle = new ResizeFocus(m_xPos, m_yPos, m_margin, ResizeFocus::NORTH_MIDDLE, this);
	m_resizeFocus.append(north_middle);
	ResizeFocus *north_east = new ResizeFocus(m_xPos, m_yPos, m_margin, ResizeFocus::NORTH_EAST, this);
	m_resizeFocus.append(north_east);
	ResizeFocus *north_west = new ResizeFocus(m_xPos, m_yPos, m_margin, ResizeFocus::NORTH_WEST, this);
	m_resizeFocus.append(north_west);
	ResizeFocus *south_middle = new ResizeFocus(m_xPos, m_yPos, m_margin, ResizeFocus::SOUTH_MIDDLE, this);
	m_resizeFocus.append(south_middle);
	ResizeFocus *south_east = new ResizeFocus(m_xPos, m_yPos, m_margin, ResizeFocus::SOUTH_EAST, this);
	m_resizeFocus.append(south_east);
	ResizeFocus *south_west = new ResizeFocus(m_xPos, m_yPos, m_margin, ResizeFocus::SOUTH_WEST, this);
	m_resizeFocus.append(south_west);
	ResizeFocus *east_middle = new ResizeFocus(m_xPos, m_yPos, m_margin, ResizeFocus::EAST_MIDDLE, this);
	m_resizeFocus.append(east_middle);
	ResizeFocus *west_middle = new ResizeFocus(m_xPos, m_yPos, m_margin, ResizeFocus::WEST_MIDDLE, this);
	m_resizeFocus.append(west_middle);
	ResizeFocus *north_middle_up = new ResizeFocus(m_xPos, m_yPos, ResizeFocus::NORTH_MIDDLE_UP, this);
	m_resizeFocus.append(north_middle_up);
}

void NodeEllipse::setValue(qreal widthScale, qreal heightScale)
{
	m_width *= widthScale;
	m_height *= heightScale;
}