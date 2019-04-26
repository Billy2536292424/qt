#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "nodeellipse.h"
#include "noderect.h"
#include "paintgroup.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)	
{
	ui->setupUi(this);
	this->resize(QSize(1000, 800));

	//场景
	m_scene = new PaintScene(this);
	//视图
	m_view = new QGraphicsView(m_scene, this);

	setCentralWidget(m_view);//将view设置为中心小部件
	m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//阻止自动加人滚动条
	m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//垂直的
	m_scene->setSceneRect(0, 0, static_cast<QWidget *>(m_scene->parent())->size().width(), \
		static_cast<QWidget *>(m_scene->parent())->size().height());
	m_isRect = false;
	m_isEllipse = false;
	m_isDrawing = false;
	m_rotateCountL = 0;
	m_rotateCountR = 0;
	m_isTurnLeft = false;
	m_isTurnRight = false;
	this->setWindowTitle("绘图系统");
}

MainWindow::~MainWindow()
{
	delete ui;
	delete m_view;
	m_view = NULL;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton && m_isDrawing)
	{
		m_lastPoint = event->pos();
		m_endPoint = m_lastPoint;
		grabMouse();
	}
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton && m_isDrawing)
	{
		m_endPoint = event->pos();
		event->setAccepted(false);
		update();
	}
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton && m_isDrawing)
	{
		m_endPoint = event->pos();
		releaseMouse();
		update();
	}

	int x = m_lastPoint.x();
	int y = m_lastPoint.y();
	int w = m_endPoint.x() - x;
	int h = m_endPoint.y() - y;
	int margin = 6;
	if (m_isRect)
	{
		NodeRect *rectItem = new NodeRect(x, y, w, h, margin);
		m_scene->addItem(rectItem);
		m_isRect = false;
		m_isDrawing = false;
	}
	if (m_isEllipse)
	{
		NodeEllipse *ellipseItem = new NodeEllipse(x, y, w, h, margin);
		m_scene->addItem(ellipseItem);
		m_isEllipse = false;
		m_isDrawing = false;
	}
}

void MainWindow::on_action_triggered()
{
	//矩形
	m_isRect = true;
	m_isDrawing = true;
}

void MainWindow::on_action_2_triggered()
{
	//椭圆
	m_isEllipse = true;
	m_isDrawing = true;
}

void MainWindow::on_action_91_triggered()
{
	//向右旋转90
	++m_rotateCountR;
	if (m_isTurnLeft)
	{
		++m_rotateCountR;
		m_isTurnLeft = false;
	}
	QList<QGraphicsItem *> items = m_scene->selectedItems();
	if (items.count() == 1)
	{
		QGraphicsItem *item = items.first();
		item->resetTransform();
		QTransform transformf = item->transform();
		int r = -90 * m_rotateCountR;
		int x = item->boundingRect().center().x();
		int y = item->boundingRect().center().y();
		transformf.translate(x, y);
		transformf.rotate(r, Qt::ZAxis);
		transformf.translate(-x, -y);
		item->setTransform(transformf);
		m_isTurnRight = true;
		update();

	}
}

void MainWindow::on_action_90_triggered()
{
	//向左旋转90
	++m_rotateCountL;
	if (m_isTurnRight)
	{
		++m_rotateCountL;
		m_isTurnRight = false;
	}
	QList<QGraphicsItem *> items = m_scene->selectedItems();
	if (items.count() == 1)
	{
		QGraphicsItem *item = items.first();
		item->resetTransform();//重置为单位矩阵
		QTransform transformf = item->transform();
		//获得项的矩阵
		int r = 90 * m_rotateCountL;
		int x = item->boundingRect().center().x();//获取中心点坐标
		int y = item->boundingRect().center().y();
		transformf.translate(x, y);//将坐标系移动到中心点
		transformf.rotate(r, Qt::ZAxis);//绕Z轴旋转r度
		transformf.translate(-x, -y);//将坐标系移动到原处
		item->setTransform(transformf);
		//设置矩阵为原来的矩阵
		m_isTurnLeft = true;
		update();
	}
}

void MainWindow::on_action_5_triggered()
{
	//组合
	PaintGroup *group = m_scene->createItemGroup(m_scene->selectedItems());
	m_scene->addItem(group);
}
