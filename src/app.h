/*
 * Copyright 2019-2020 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */

#ifndef APP_H
#define APP_H

#include <Application.h>
#include <Message.h>

class MainWindow;

class App : public BApplication {
public:
	App();
	void MessageReceived(BMessage *msg);
	void AboutRequested();
	bool QuitRequested();
	void ReadyToRun();

private:
		MainWindow		*fMainWindow;
		BMessage		*fDataMessage;
		BMessage		*fSelectedMessage;		
		BString 		fSelectedFieldName;
		type_code		fSelectedFieldType;
		int32			fSelectedFieldItemCount;

};

#endif

