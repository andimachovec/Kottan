/*
 * Copyright 2019-2020 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */
 

#include "editwindow.h"

#include <LayoutBuilder.h>
#include <Catalog.h>


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "EditWindow"


EditWindow::EditWindow(BRect frame, type_code DataType, BMessage *Data)
	:
	BWindow(frame, B_TRANSLATE("Edit"), B_TITLED_WINDOW,B_CLOSE_ON_ESCAPE)
{

	fEditView = new EditView(DataType, Data);
	fCancelButton = new BButton(B_TRANSLATE("Cancel"), new BMessage(EW_BUTTON_CANCEL));
	fSaveButton = new BButton(B_TRANSLATE("Save"), new BMessage(EW_BUTTON_SAVE));

	if (!fEditView->IsEditable())
	{
		fSaveButton->SetEnabled(false);
	}

	BLayoutBuilder::Group<>(this, B_VERTICAL,0)
		.SetInsets(5)
		.Add(fEditView)
		.AddGroup(B_HORIZONTAL)
			.Add(fCancelButton)
			.Add(fSaveButton)
		.End()
	.Layout();
	
}


void
EditWindow::MessageReceived(BMessage *msg)
{

	switch (msg->what)
	{
	
		case EW_BUTTON_SAVE:
		case EW_BUTTON_CANCEL:
			Quit();
			break;
	
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}

	}

}




