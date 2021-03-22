/*
 * Copyright 2019-2020 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */

#ifndef APP_H
#define APP_H

#include <Application.h>
#include <Message.h>
#include <ObjectList.h>
#include <File.h>


class MainWindow;

class IndexMessage {
public:
	BMessage 	*message;
	int32		field_index;
	const char  *field_name;
};



class App : public BApplication {
public:
	App();
	~App();
	void MessageReceived(BMessage *msg);
	void AboutRequested();
	bool QuitRequested();
	void ReadyToRun();

private:
		MainWindow					*fMainWindow;
		BMessage					*fDataMessage;
		BMessage					*fPathMessage;
		BObjectList<IndexMessage>	*fMessageList;
		BFile						*fMessageFile;
};

#endif

