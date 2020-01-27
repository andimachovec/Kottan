/*
 * Copyright 2019 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */


#include "messagewindow.h"

#include <LayoutBuilder.h>
#include <Catalog.h>
#include <ColumnTypes.h>


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "MessageWindow"

MessageWindow::MessageWindow(BRect frame, BMessage *data_message, BString name)
	:
	BWindow(frame, B_TRANSLATE("Message Content"), B_TITLED_WINDOW,B_CLOSE_ON_ESCAPE)
{

	fMessageView = new MessageView(this);
	fMessageView->SetDataMessage(data_message);





	fMessageLabel = new BStringView("messagelabel", name.String());
	BFont bold_font(be_bold_font);
	fMessageLabel->SetFont(&bold_font);

	fCloseButton = new BButton(B_TRANSLATE("Close"), new BMessage(MG_BUTTON_CLOSE));



	BLayoutBuilder::Group<>(this, B_VERTICAL,0)
		.SetInsets(5)
		.AddGroup(B_HORIZONTAL)
			.SetInsets(5)
			.Add(fMessageLabel)
		.End()
		.Add(fMessageView)
		.Add(fCloseButton)
	.Layout();
	
}


void
MessageWindow::MessageReceived(BMessage *msg)
{

	switch (msg->what)
	{
	
		case MG_BUTTON_CLOSE:
			Quit();
			break;
	
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}

	}

}
