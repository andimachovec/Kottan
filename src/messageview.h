/*
 * Copyright 2019-2021 Andi Machovec <andi.machovec@gmail.com>
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
	void create_data_rows(BMessage *message, BRow *parent = NULL);
	BMessage *fDataMessage;

};

#endif
