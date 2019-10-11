#ifndef APP_H
#define APP_H

#include <Application.h>

#include "mainwindow.h"

class App : public BApplication {
public:
	App();
	void MessageReceived(BMessage *msg);
	void AboutRequested();
	bool QuitRequested();
	void ReadyToRun();

private:
		MainWindow		*fMainWindow;

};

#endif

