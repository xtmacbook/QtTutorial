
#include "imgScene.h"


GraphicScene::GraphicScene(QObject *parent) :
	QGraphicsScene(parent)
{
}

GraphicScene::~GraphicScene()
{
	clearScene();
}

void GraphicScene::addImgItem(QGraphicsItem*item)
{
	imgItems_.push_back(item);
	addItem(item);
	update();
}


void GraphicScene::clearScene()
{
	destroyItems(imgItems_);
	destroyItems(labelItems_);
	clear();
}

void GraphicScene::setSceneShowRect(const QRect&viewRect, const QRect&itemRect)
{
	QRect sceneRect;

	(viewRect.width() > itemRect.width()) ? sceneRect.setWidth(viewRect.width()) : sceneRect.setWidth(itemRect.width());
	(viewRect.height() > itemRect.height()) ? sceneRect.setHeight(viewRect.height()) : sceneRect.setHeight(itemRect.height());

	setSceneRect(-sceneRect.width() / (2 * 1.0), -sceneRect.height() / (2 * 1.0), sceneRect.width(), sceneRect.height()); //将坐标原点设在显示窗口的中心点

}

void GraphicScene::addLabelItem(QGraphicsItem*item)
{
	addItem(item);
	labelItems_.push_back(item);
	update();
}

void GraphicScene::removeLabelItem(QGraphicsItem* item)
{
	
	std::vector<QGraphicsItem*>::iterator it = find(labelItems_.begin(), labelItems_.end(), item);
	if (it != labelItems_.end())
	{
		removeItem(item);
		labelItems_.erase(it);
	}
}

QGraphicsItem* GraphicScene::getImgItem() const
{
	if(imgItems_.size() > 0)
		return imgItems_.at(0);
    return nullptr;
}

void GraphicScene::showLabelItem(bool show)
{
    for(int i = 0;i < labelItems_.size();i++)
    {
        (show)?labelItems_.at(i)->show():labelItems_.at(i)->hide();
    }
}

void GraphicScene::destroyItems(std::vector<QGraphicsItem*>&container)
{
	for (int i = 0; i < container.size(); i++)
	{
		removeItem(container.at(i));
	}
    container.clear();
}

