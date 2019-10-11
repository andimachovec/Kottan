#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Window.h>
#include <Button.h>
#include <MenuBar.h>


enum
{
	MW_MENU_ABOUT ='mw00',
};

class MainWindow : public BWindow {
public:
	MainWindow(float left, float top, float right, float bottom);
	void MessageReceived(BMessage *msg);
	bool QuitRequested();

private:
	BMenuBar	*fTopMenuBar;

};

#endif
