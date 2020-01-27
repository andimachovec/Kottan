/*
 * Copyright 2019 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */

#ifndef MESSAGEWINDOW_H
#define MESSAGEWINDOW_H


#include <Window.h>
#include <Message.h>
#include "messageview.h"

class MessageWindow : public BWindow {
public:
		MessageWindow(BRect frame, BMessage *DataMessage);

private:
	MessageView *fMessageView;
};

#endif
