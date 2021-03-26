/*
 * Copyright 2019-2021 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */
 

#include "editwindow.h"

#include <LayoutBuilder.h>
#include <Catalog.h>
#include <Application.h>

#include <iostream>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "EditWindow"


EditWindow::EditWindow(BRect frame, BMessage *data_message, type_code data_type, const char *data_label, int32 data_index)
	:
	BWindow(frame, B_TRANSLATE("Edit"), B_TITLED_WINDOW,B_CLOSE_ON_ESCAPE)
{

	fEditView = new EditView(data_message, data_type, data_label, data_index);
	fCancelButton = new BButton(B_TRANSLATE("Cancel"), new BMessage(EW_BUTTON_CANCEL));
	fSaveButton = new BButton(B_TRANSLATE("Save"), new BMessage(EW_BUTTON_SAVE));
	
	
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
		{
			fEditView->SaveData();
			be_app->PostMessage(msg);	
			Quit();
			break;
		}
		
		case EW_BUTTON_CANCEL:
			Quit();
			break;
				
		/*
		case EV_DATA_CHANGED:
			std::cout << "Data changed" << std::endl;
			fSaveButton->SetEnabled(true);
			break;
		*/
					
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}

}




