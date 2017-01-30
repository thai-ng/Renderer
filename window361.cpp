#include <iostream>
#include <QGridLayout>
#include "window361.h"

Window361::Window361() : renderArea(std::make_unique<RenderArea361>(nullptr)),
						 nextPageButton(std::make_unique<QPushButton>("next page"))
{
    //renderArea = new RenderArea361((QWidget *)0);
    //nextPageButton = new QPushButton("next page");
    connect(nextPageButton.get(), SIGNAL(clicked()), this, SLOT(nextPageClicked()));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(renderArea.get(), 0, 0);
    layout->addWidget(nextPageButton.get(), 0, 1);
    setLayout(layout);

    setWindowTitle(tr("361 Window"));
}
Drawable *Window361::getDrawable() {
	return renderArea.get();
}
//Window361::~Window361() {
//}

void Window361::setPageTurner(PageTurner *inPageTurner) {
    this->pageTurner = inPageTurner;
    pageTurner->nextPage();     // give them the change for the first page.
}

void Window361::nextPageClicked() {
    if(pageTurner != 0) {
        pageTurner->nextPage();
    }
}


