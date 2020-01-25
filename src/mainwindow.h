/*
 * Copyright 2019 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Window.h>
#include <Button.h>
#include <MenuBar.h>
#include <TextControl.h>
#include <String.h>
#include <Path.h>
#include <Message.h>
#include <File.h>
#include <FilePanel.h>
#include <ColumnTypes.h>

#include "messageview.h"

enum
{
	MW_MENU_ABOUT ='mw00',
	MW_BUTTON_CHOOSEMESSAGEFILE,
	MW_REF_MESSAGEFILE,
	MW_MSGINFO_CLICKED,
	MW_INSPECTMESSAGEFILE
};

class MainWindow : public BWindow {
public:
	MainWindow(float left, float top, float right, float bottom);
	~MainWindow();
	void MessageReceived(BMessage *msg);
	bool QuitRequested();

private:


	void inspect_message_file();
	void show_message_data(int32 msg_index);
	
	BString bool2bstring(bool value);
	
	
	
	BMenuBar	*fTopMenuBar;
	
	BTextControl		*fMessageFileTextControl;
	BButton				*fChooseMessageFileButton;
	MessageView			*fMessageInfoView;
	BMessage			*fCurrentMessage;
	BFilePanel			*fOpenFilePanel;
	
};

#endif
