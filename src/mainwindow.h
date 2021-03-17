/*
 * Copyright 2019-2021 Andi Machovec <andi.machovec@gmail.com>
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
#include <File.h>
#include <FilePanel.h>
#include <ColumnTypes.h>

#include "messageview.h"

enum
{
	MW_MENU_ABOUT ='mw00',
	MW_BUTTON_CHOOSEMESSAGEFILE,
	MW_REF_MESSAGEFILE,
	MW_ENTERED_MESSAGEFILE,
	MW_INSPECTMESSAGEFILE,
	MW_OPEN_REPLY,
	MW_ROW_SELECTED,
};

class MainWindow : public BWindow {
public:
	MainWindow(float left, float top, float right, float bottom);
	~MainWindow();
	void MessageReceived(BMessage *msg);
	bool QuitRequested();

private:


	void inspect_message_file();
	
		
	BMenuBar	*fTopMenuBar;
	
	BTextControl		*fMessageFileTextControl;
	BButton				*fChooseMessageFileButton;
	MessageView			*fMessageInfoView;
	BFilePanel			*fOpenFilePanel;
	
};

#endif
