/*
 * Copyright 2019-2021 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Catalog.h>
#include <Window.h>
#include <MenuBar.h>
#include <FilePanel.h>

#include "messageview.h"


enum
{
	MW_MENU_ABOUT ='mw00',
	MW_OPEN_MESSAGEFILE,
	MW_SAVE_MESSAGEFILE,
	MW_ENTERED_MESSAGEFILE,
	MW_INSPECTMESSAGEFILE,
	MW_OPEN_REPLY,
	MW_ROW_SELECTED,
	MW_WAS_EDITED,
	MW_WAS_SAVED,
	MW_DO_NOTHING,
	MW_RELOAD_FROM_FILE,
	MW_CONFIRM_RELOAD,
	MW_UPDATE_MESSAGEVIEW,
};


static const char *kNotsaved_alert_text =
		B_TRANSLATE_MARK(
		"The message data was changed but not saved. Do you really want to open another file?"
		);
static const char *kNotsaved_alert_cancel = B_TRANSLATE_MARK("Cancel");
static const char *kNotsaved_alert_continue = B_TRANSLATE_MARK("Open file");


class MainWindow : public BWindow {
public:
	MainWindow(BRect geometry);
	~MainWindow();
	void MessageReceived(BMessage *msg);
	bool QuitRequested();

	bool ContinueAction(const char *alert_text,
						 const char *button_label_cancel,
						 const char *button_label_continue);
private:
	void switch_unsaved_state(bool unsaved_state);

	BMenuBar			*fTopMenuBar;
	MessageView			*fMessageInfoView;
	BFilePanel			*fOpenFilePanel;
	bool				fUnsaved;
};

#endif
