
#include "zoomDialog.h"
#include "ui_zoomDialog.h"
#include "selRect.h"



double MX_SCAL = 12.0;
double MIN_SCAL = 0.9;

ZoomRect::ZoomRect(QWidget*p, ZoomDialog* d,ZOOMTYPE t):
	QLabel(p),
	type_(t),
	parent_(d)
{
	setFrameStyle(Box);
	setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(255, 20, 0);");
    setLineWidth(2);
}

ZoomRect::~ZoomRect()
{

}

void ZoomRect::paintEvent(QPaintEvent *event)
{
	if (type_ == MAX_ZOOM)
	{
		QPainter painter(this);
		painter.setPen(QPen(Qt::red, 2));//���û�����ʽ
		painter.drawLine(2, 6, 10, 6);//��ֱ��
		painter.drawLine(6, 2, 6, 10);//��ֱ��
	}
	else if (type_ == MIN_ZOOM)
	{
		QPainter painter(this);
		painter.setPen(QPen(Qt::red, 2));//���û�����ʽ
		painter.drawLine(2, 6, 10, 6);//��ֱ��
	}
	else
		QLabel::paintEvent(event);
}

void ZoomRect::mousePressEvent(QMouseEvent *ev) 
{
	QLabel::mousePressEvent(ev);
}

void ZoomRect::mouseReleaseEvent(QMouseEvent *ev) 
{
	if (type_ == MAX_ZOOM) emit maxView();
	else if (type_ == MIN_ZOOM) 	emit minView();
	QLabel::mouseReleaseEvent(ev);
}

ZoomDialog::ZoomDialog(QWidget *parent, Qt::WindowFlags f) :
	QDialog(parent,f)
{
    ui_ = new  Ui::Dialog;
    ui_->setupUi(this);


    setCursor(Qt::CrossCursor);
    selRect_ = new SelRect(ui_->graphicsView,this);
    selRect_->setSize(64, 64);
    selRect_->hide();

	Qt::WindowFlags flags = this->windowFlags();
	setWindowFlags(flags&~Qt::WindowContextHelpButtonHint);
	setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
	setFixedSize(this->width(), this->height());

	maxRect_ = new ZoomRect(ui_->graphicsView,this,MAX_ZOOM);
	minRect_ = new ZoomRect(ui_->graphicsView, this, MIN_ZOOM);
	defRect_ = new ZoomRect(ui_->graphicsView, this, DEFAULT);

//    connect(ui_->openBaseImageA,SIGNAL(triggered()), this, SLOT(openBaseImg()));
    ui_->graphicsView->connect(ui_->graphicsView->addAction(),SIGNAL(triggered()),this,SLOT(addPoint()));
	connect(maxRect_, SIGNAL(maxView()), this, SLOT(maxScale()));
	connect(minRect_, SIGNAL(minView()), this, SLOT(minScale()));
	connect(defRect_, SIGNAL(defaultView()), this, SLOT(defScale()));

	maxRect_->setMaximumSize(QSize(12, 12));
	minRect_->setMaximumSize(QSize(12, 12));
	defRect_->setMaximumSize(QSize(12, 12));

	ui_->gridLayout->addWidget(maxRect_,0,0,1,1, Qt::AlignBottom);
	ui_->gridLayout->addWidget(minRect_, 0, 0, 1, 1, Qt::AlignBottom);
	ui_->gridLayout->addWidget(defRect_, 0, 0, 1, 1, Qt::AlignBottom);

	scale_ = 4.0;
}

ZoomDialog::~ZoomDialog()
{

}
void ZoomDialog::setScene(QGraphicsScene *scene)
{
    ui_->graphicsView->setScene(scene);
}

void ZoomDialog::resizeView()
{

	QSize size = this->size();
	QGraphicsView * view = ui_->graphicsView;
	QRect grect = view->geometry();
	const QGraphicsScene * scene = view->scene();

	if (scene)
	{
		if (!scene->items().empty())
		{
			/*view->setSceneRect(-size.width() / (2 * 1.0), -size.height() / (2 * 1.0), size.width() - 2, size.height() - 2);*/
			ui_->graphicsView->scale(scale_, scale_);
			view->update();
		}
	}
}

ZoomView * ZoomDialog::getView()
{
	return ui_->graphicsView;
}

void ZoomDialog::resetViewSize()
{
	ui_->graphicsView->resetMatrix();
}

QRect ZoomDialog::getViewRect() const
{
    return ui_->graphicsView->fullViewSceneRect();
}

void ZoomDialog::mainSelRect(SelRect *mainS)
{
    mainS->setParent(selRect_);
    mainS->setChild(selRect_);
}

void ZoomDialog::showEvent(QShowEvent * event)
{
	QDialog::showEvent(event);
}

void ZoomDialog::resizeEvent(QResizeEvent *event)
{

	maxRect_->move(minRect_->pos() + QPoint(14, 0));
	defRect_->move(maxRect_->pos() + QPoint(14, 0));

    QDialog::resizeEvent(event);
}

void ZoomDialog::maxScale()
{
	scale_ += 1.0;
    if(scale_ >= MX_SCAL) scale_ = MX_SCAL;
	update();
}

void ZoomDialog::minScale()
{
	scale_ -= 1.0;
    if(scale_ <= MIN_SCAL) scale_ = MIN_SCAL;
	update();
}

void ZoomDialog::dMaxScale()
{
	scale_ *= 2.0;
    if(scale_ >= MX_SCAL) scale_ = MX_SCAL;
	update();
}

void ZoomDialog::dMinScale()
{
	scale_ *= 0.5;
    if(scale_ <= MIN_SCAL) scale_ = MIN_SCAL;
	update();
}

void ZoomDialog::defScale()
{
	scale_ = 4.0f; 
    update();
}

void ZoomDialog::addPoint()
{
    emit addSelPoint(ui_->graphicsView->contextPoint());
}


void ZoomDialog::update()
{
	QTransform tr = ui_->graphicsView->transform();
	
	QTransform copy(scale_,tr.m12(),tr.m13(),tr.m21(),scale_,tr.m23(),tr.m31(),tr.m32(),tr.m33());
	ui_->graphicsView->setTransform(copy);
	ui_->graphicsView->update();
}
