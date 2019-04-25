#ifndef PAINTSCENE_H
#define PAINTSCENE_H
#include <QList>
#include <QGraphicsScene>
#include "paintgroup.h"

class PaintScene : public QGraphicsScene
{
public:
	PaintScene(QObject *parent = 0);
	PaintGroup *createItemGroup(const QList<QGraphicsItem *> &items);//保证可以组合已经组合的项和新的项
};

#endif // PAINTSCENE_H
