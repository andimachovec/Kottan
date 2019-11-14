/*
 * Copyright 2019 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */

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

