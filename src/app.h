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
#include <File.h>
#include <String.h>


class DataWindow;
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
	void ArgvReceived(int32 argc, char **argv);

private:
		void get_selection_data(BMessage *selection_path_message);

		MainWindow					*fMainWindow;
		DataWindow					*fDataWindow;
		BMessage					*fDataMessage;
		BObjectList<IndexMessage>	*fMessageList;
		BFile						*fMessageFile;
		BString						fSettingsFileName;

		const char 					*fSelectedName;
		type_code 					fSelectedType;
		int32 						fSelectedItemCount;
};

#endif

