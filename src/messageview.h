/*
 * Copyright 2019-2020 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */


#ifndef MESSAGEVIEW_H
#define MESSAGEVIEW_H

#include <ColumnListView.h>
#include <Message.h>
#include <String.h>
#include <Looper.h>

enum
{
	MV_ROW_CLICKED ='mv00',
};


class MessageView : public BColumnListView {
public:
	MessageView(BLooper *TargetLooper);
	void MessageReceived(BMessage *msg);
	void SetDataMessage(BMessage *message);

private:
	void show_message_data(const char *name, type_code type, int32 number_of_items);
	BMessage *fDataMessage;

};

#endif
