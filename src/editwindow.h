/*
 * Copyright 2019-2021 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */

#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include <Window.h>
#include <Button.h>
#include <FilePanel.h>

#include "editview.h"


enum
{
	EW_BUTTON_CANCEL='ew00',
	EW_BUTTON_SAVE,
	EW_DATA_SAVE
};


class EditWindow : public BWindow {
public:
	EditWindow(BRect frame, BMessage *data_message, type_code data_type, const char *data_label, int32 data_index);
	virtual ~EditWindow();
	void MessageReceived(BMessage *msg);

private:
	BFilePanel  *fOpenPanel;
	BButton		*fCancelButton;
	BButton		*fSaveButton;
	EditView	*fEditView;
};

#endif
