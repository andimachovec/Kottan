/*
 * Copyright 2019-2021 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */

#ifndef APP_H
#define APP_H

#include <Application.h>
#include <Message.h>
#include <ObjectList.h>
#include <String.h>


class DataWindow;
class MainWindow;
class MessageFile;


class App : public BApplication {
public:
	App();
	~App();
	void MessageReceived(BMessage *msg);
	void AboutRequested();
	bool QuitRequested();
	void ReadyToRun();
	void ArgvReceived(int32 argc, char **argv);
	void RefsReceived(BMessage* msg);

private:
		MainWindow					*fMainWindow;
		DataWindow					*fDataWindow;
		MessageFile					*fMessageFile;
		BString						fSettingsFileName;
};

#endif

