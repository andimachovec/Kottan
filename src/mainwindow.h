/*
 * Copyright 2019-2021 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Window.h>
#include <MenuBar.h>
#include <FilePanel.h>

#include "messageview.h"


enum
{
	MW_MENU_ABOUT ='mw00',
	MW_OPEN_MESSAGEFILE,
	MW_SAVE_MESSAGEFILE,
	MW_REF_MESSAGEFILE,
	MW_ENTERED_MESSAGEFILE,
	MW_INSPECTMESSAGEFILE,
	MW_OPEN_REPLY,
	MW_ROW_SELECTED,
	MW_WAS_EDITED,
	MW_WAS_SAVED,
	MW_DO_NOTHING,
};

class MainWindow : public BWindow {
public:
	MainWindow(BRect geometry);
	~MainWindow();
	void MessageReceived(BMessage *msg);
	bool QuitRequested();

private:
	bool continue_action(const char *alert_text, 
						 const char *button_label_cancel,
						 const char *button_label_continue);
	
	BMenuBar			*fTopMenuBar;
	MessageView			*fMessageInfoView;
	BFilePanel			*fOpenFilePanel;
	bool				fNotSaved;
};

#endif
