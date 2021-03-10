/*
 * Copyright 2019-2020 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */


#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include <Window.h>
#include <Button.h>

#include "editview.h"


enum
{
	EW_BUTTON_CANCEL='ew00',
	EW_BUTTON_SAVE
};


class EditWindow : public BWindow {
public:
	EditWindow(BRect frame, type_code DataType, BMessage *Data);
	void MessageReceived(BMessage *msg);

private:

	BButton		*fCancelButton;
	BButton		*fSaveButton;
	EditView	*fEditView;

};

#endif
