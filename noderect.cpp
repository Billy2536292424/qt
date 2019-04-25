#include "noderect.h"
#include <QRectF>
#include <cmath>
#include <QtDebug>
#include <iostream>
#include <fstream>
#include <QMatrix>

NodeRect::NodeRect(QGraphicsItem *parent)
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
    setAcceptDrops(true);//接受拖放事件
    setAcceptHoverEvents(true);//接受悬停事件
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    createResizeFocus();
}
NodeRect::NodeRect(int x, int y, int width, int height, int margin, QGraphicsItem *parent)
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

NodeRect::~NodeRect()
{
}

QRectF NodeRect::boundingRect() const
{
	qreal penWidth = 1;
	return QRectF(m_xPos - penWidth / 2, m_yPos - penWidth / 2, m_width + penWidth,
		m_height + penWidth);
}

void NodeRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	update();
	Q_UNUSED(widget);
	QColor fillColor = QColor(100, 100, 255);
	if (option->state & QStyle::State_Selected)//指示小部件是否被选择
		fillColor = fillColor.darker(150);
	if (option->state & QStyle::State_MouseOver)//指示小部件是否在鼠标下
		fillColor = fillColor.lighter(125);
	//painter->setCompositionMode(QPainter::CompositionMode_Exclusion);
	if (option->state & QStyle::State_Selected)
	{
		painter->fillRect(m_xPos + m_margin, m_yPos + m_margin, m_width - m_margin * 2, m_height - m_margin * 2, fillColor);
		painter->drawRect(m_xPos + m_margin, m_yPos + m_margin, m_width - m_margin * 2, m_height - m_margin * 2);
		showResizeFocus(true);
	}
	else
	{
		painter->fillRect(m_xPos + m_margin, m_yPos + m_margin, m_width - m_margin * 2, m_height - m_margin * 2, fillColor);
		painter->drawRect(m_xPos + m_margin, m_yPos + m_margin, m_width - m_margin * 2, m_height - m_margin * 2);
		showResizeFocus(false);
	}
}

QPainterPath NodeRect::shape() const
{
	QPainterPath path;
	path.addRect(boundingRect());
	return path;
}

void NodeRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
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
			m_count = 0;
			qDebug() << "startRotate" <<m_startRotate.x() << " " << m_startRotate.y();
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

void NodeRect::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (m_isRotate)
	{
		m_count++;
		if (m_count % 2 == 1) //防止MouseMoveEvent被响应两次
		{
			m_endRotate = event->scenePos();
			m_centerRotate = boundingRect().center();
			qDebug() << "endRotate" << m_endRotate.x() << " " << m_endRotate.y();
			qDebug() << "centerRotate" << m_centerRotate.x() << " " << m_centerRotate.y();
			qreal oneEdge = sqrt(pow(m_centerRotate.x() - m_startRotate.x(), 2) + \
				pow(m_centerRotate.y() - m_startRotate.y(), 2));  //两点间距离公式
			qreal otherEdge = sqrt(pow(m_centerRotate.x() - m_endRotate.x(), 2) + \
				pow(m_centerRotate.y() - m_endRotate.y(), 2));
			qreal oppositeSide = sqrt(pow(m_endRotate.x() - m_startRotate.x(), 2) + \
				pow(m_endRotate.y() - m_startRotate.y(), 2));
			qreal cosValue = (pow(oneEdge, 2) + pow(otherEdge, 2) - pow(oppositeSide, 2))\
				/ (2 * oneEdge * otherEdge);//余弦定理
			qreal angle = acos(cosValue);
			angle = angle * 180 / 3.1415;
			qreal dx1 = m_startRotate.x() - m_centerRotate.x();
			qreal dy1 = m_startRotate.y() - m_centerRotate.y();
			qreal dx2 = m_endRotate.x() - m_centerRotate.x();
			qreal dy2 = m_endRotate.y() - m_centerRotate.y();
			//qreal cosValue = (dx1 * dx2 + dy1 * dy2) / (oneEdge * otherEdge); //向量的点乘
			//qreal angle = acos(cosValue);
			
			if (dx1 * dy2 - dy1 * dx2 > 0) //使用向量积判断旋转方向
			{  //右转		顺时针
				m_angle += fabs(angle);
			}
			else
			{
				//左转		逆时针
				m_angle -= fabs(angle);
			}

			resetTransform();
			QTransform transformf = transform();
			qreal x = boundingRect().center().x();
			qreal y = boundingRect().center().y();
			transformf.translate(x, y);
			transformf.rotate(m_angle, Qt::ZAxis);
			
			transformf.translate(-x, -y);
			setTransform(transformf);
		}
	}
	else if (m_mode == RESIZE)
	{
		QPointF curPoint(event->scenePos());
		qreal curX = m_xPos + m_margin, curY = m_yPos + m_margin, curWidth = m_width - m_margin * 2, curHeight = m_height - m_margin * 2;
		qreal wChanging = curPoint.x() - m_lastPoint.x(),\
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
		if(curWidth < 20 || curHeight < 20)//minimal size
			return;
		m_dashRect->setRect(curX, curY, curWidth, curHeight);
	}
	else
		QGraphicsItem::mouseMoveEvent(event);
		
}

void NodeRect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if (m_isRotate)
	{
		m_isRotate = false;
	}
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

void NodeRect::showResizeFocus(bool visible)
{
	for (int i = 0; i < m_resizeFocus.count(); i++)
	{
		m_resizeFocus.at(i)->locateInHost();
		m_resizeFocus.at(i)->setVisible(visible);
	}
}

//修改数据
void NodeRect::setValue(qreal widthScale, qreal heightScale)
{
	m_width *= widthScale;
	m_height *= heightScale;
}

void NodeRect::createResizeFocus()
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
	ResizeFocus *north_middle_up = new ResizeFocus(m_xPos, m_yPos, m_margin, ResizeFocus::NORTH_MIDDLE_UP, this);
	m_resizeFocus.append(north_middle_up);
}