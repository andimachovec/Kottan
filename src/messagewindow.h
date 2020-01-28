/*
 * Copyright 2019-2020 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */

#ifndef MESSAGEWINDOW_H
#define MESSAGEWINDOW_H


#include <Window.h>
#include <Message.h>
#include <Button.h>
#include <StringView.h>
#include <String.h>
#include "messageview.h"


enum 
{
	MG_BUTTON_CLOSE = 'mg00'

};


class MessageWindow : public BWindow {
public:
		MessageWindow(BRect frame, BString title, BMessage *data_message);
		void MessageReceived(BMessage *msg);


private:
	MessageView 	*fMessageView;
	BButton			*fCloseButton;
	BStringView		*fMessageLabel;
};

#endif
