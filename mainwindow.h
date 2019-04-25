#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidGets>
#include "paintscene.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);

private slots:
	void on_action_triggered();
	void on_action_2_triggered();
	void on_action_91_triggered();
	void on_action_90_triggered();
	void on_action_5_triggered();

private:

	Ui::MainWindow *ui;
	PaintScene *m_scene;
	QGraphicsView *m_view;
	QPoint m_lastPoint;
	QPoint m_endPoint;
	bool m_isRect;
	bool m_isEllipse;
	bool m_isDrawing;
	int m_rotateCountL;
	int m_rotateCountR;
	bool m_isTurnLeft;
	bool m_isTurnRight;
};

#endif // MAINWINDOW_H
