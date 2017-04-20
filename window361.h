#ifndef WINDOW_H
#define WINDOW_H

#include <memory>

#include "pageturner.h"
#include "drawable.h"
#include "renderarea361.h"

#include <QWidget>
#include <QPushButton>

class Window361 : public QWidget
{
    Q_OBJECT

public:
    Window361();
    //~Window361();
    Drawable *getDrawable();
    void setPageTurner(PageTurner *pageTurner);
	void setTitle(const char* title);
private:
    std::unique_ptr<RenderArea361> renderArea;
    std::unique_ptr<QPushButton> nextPageButton;
    PageTurner* pageTurner;


private slots:
    void nextPageClicked();

};

#endif // WINDOW_H
