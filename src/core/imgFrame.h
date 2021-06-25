

#include <QFrame>

class ImgFrame :
	public QFrame
{
public:
	ImgFrame(QWidget *parent);
    virtual ~ImgFrame();

	void showSel();

private:
	//MoveEnableRect *  sel_;
};
