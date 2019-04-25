#ifndef PAINTSCENE_H
#define PAINTSCENE_H
#include <QList>
#include <QGraphicsScene>
#include "paintgroup.h"

class PaintScene : public QGraphicsScene
{
public:
	PaintScene(QObject *parent = 0);
	PaintGroup *createItemGroup(const QList<QGraphicsItem *> &items);//��֤��������Ѿ���ϵ�����µ���
};

#endif // PAINTSCENE_H
