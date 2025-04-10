/*
 * Copyright 2019-2021 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */

#include "editwindow.h"

#include <LayoutBuilder.h>
#include <Catalog.h>
#include <Application.h>


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "EditWindow"


EditWindow::EditWindow(BRect frame,
					BMessage *data_message,
					type_code data_type,
					const char *data_label,
					int32 data_index)
	:
	BWindow(frame, B_TRANSLATE("Edit"), B_DOCUMENT_WINDOW_LOOK,B_MODAL_APP_WINDOW_FEEL, B_CLOSE_ON_ESCAPE
		| B_NOT_ZOOMABLE | B_NOT_V_RESIZABLE | B_AUTO_UPDATE_SIZE_LIMITS)
{

	fEditView = new EditView(data_message, data_type, data_label, data_index);
	fCancelButton = new BButton(B_TRANSLATE("Cancel"), new BMessage(EW_BUTTON_CANCEL));
	fSaveButton = new BButton(B_TRANSLATE("Save"), new BMessage(EW_BUTTON_SAVE));
	fSaveButton->SetEnabled(false);

	BLayoutBuilder::Group<>(this, B_VERTICAL, B_USE_SMALL_SPACING)
		.SetInsets(B_USE_SMALL_SPACING)
		.Add(fEditView)
		.AddStrut(B_USE_BIG_SPACING)
		.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.Add(fCancelButton)
			.Add(fSaveButton)
			.AddGlue()
		.End()
		.AddGlue(100)
	.Layout();

}


void
EditWindow::MessageReceived(BMessage *msg)
{

	switch (msg->what)
	{
		case EW_BUTTON_SAVE:
		{
			fEditView->SaveData();
			be_app->PostMessage(msg);
			Quit();
			break;
		}

		case EW_BUTTON_CANCEL:
			be_app->PostMessage(msg);
			Quit();
			break;

		case EV_DATA_CHANGED:
			if(!fEditView->IsEditable())
				break;

			fSaveButton->SetEnabled(true);
			break;


		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}

}




