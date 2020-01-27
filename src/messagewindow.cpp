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

MessageWindow::MessageWindow(BRect frame, BMessage *DataMessage)
	:
	BWindow(frame, B_TRANSLATE("Message Content"), B_TITLED_WINDOW,B_CLOSE_ON_ESCAPE)
{

	fMessageView = new MessageView(this);
	fMessageView->SetDataMessage(DataMessage);

	BLayoutBuilder::Group<>(this, B_VERTICAL,0)
		.SetInsets(5)
		.Add(fMessageView)
		//.Add(fCloseButton)
	.Layout();
	
}

