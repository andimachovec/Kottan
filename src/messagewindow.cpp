/*
 * Copyright 2019-2020 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */


#include "messagewindow.h"

#include <LayoutBuilder.h>
#include <Catalog.h>
#include <ColumnTypes.h>


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "MessageWindow"

MessageWindow::MessageWindow(BRect frame, BString title, BMessage *data_message)
	:
	BWindow(frame, title.String(), B_TITLED_WINDOW,B_CLOSE_ON_ESCAPE)
{

	fMessageView = new MessageView(this);
	fMessageView->SetDataMessage(data_message);

	fCloseButton = new BButton(B_TRANSLATE("Close"), new BMessage(MG_BUTTON_CLOSE));

	BLayoutBuilder::Group<>(this, B_VERTICAL,0)
		.SetInsets(5)
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
