
#ifndef IMGSCENE_H
#define  IMGSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>

class GraphicScene :public QGraphicsScene
{
	Q_OBJECT
public:
	GraphicScene(QObject *parent = Q_NULLPTR);
    virtual ~GraphicScene();

	void		addImgItem(QGraphicsItem*);
	void		addLabelItem(QGraphicsItem*);
	void		removeLabelItem(QGraphicsItem*);

	void		clearScene();
	void		setSceneShowRect(const QRect&v,const QRect&i);
	QGraphicsItem*	getImgItem()const;
    void        showLabelItem(bool);
private:
	void destroyItems(std::vector<QGraphicsItem*>&);
private:
	std::vector<QGraphicsItem*>			imgItems_;
	std::vector<QGraphicsItem*>			labelItems_;
};


#endif // !1
