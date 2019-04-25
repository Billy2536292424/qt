#include "paintscene.h"

PaintScene::PaintScene(QObject *parent)
	: QGraphicsScene(parent)
{
}

PaintGroup *PaintScene::createItemGroup(const QList<QGraphicsItem *> &items)
{
	QList<QGraphicsItem *> ancestors;
	int n = 0;
	if (!items.isEmpty())
	{
		QGraphicsItem *parent = items.at(n++);
		while (parent = parent->parentItem())
			ancestors.append(parent);
	}
	QGraphicsItem *commonAncestor = NULL;
	if (!ancestors.isEmpty())
	{
		while (n < items.size())
		{
			int commonIndex = -1;
			QGraphicsItem *parent = items.at(n++);
			do
			{
				int index = ancestors.indexOf(parent, qMax(0, commonIndex));
				if (index != -1)
				{
					commonIndex = index;
					break;
				}
			}while ((parent = parent->parentItem()));

			if (commonIndex == -1)
			{
				commonAncestor = 0;
				break;
			}
			commonAncestor = ancestors.at(commonIndex);

		}
	}
	PaintGroup *group = new PaintGroup(commonAncestor);
	if (!commonAncestor)
	{
		addItem(group);
	}
	foreach (QGraphicsItem *item, items)
	{
		item->setSelected(false);
		QGraphicsItem *g = dynamic_cast<QGraphicsItemGroup *>(item->parentItem());
		if (!g)
		{
			group->addToGroup(item);
		}

	}
	return group;
}
