/*
 * Copyright 2019-2020 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */


#ifndef EDITVIEW_H
#define EDITVIEW_H

#include <View.h>
#include <Message.h>
#include <View.h>
#include <GroupLayout.h>


class EditView : public BView {
public:
	EditView(BMessage *data_message, type_code data_type, const char *data_label, int32 data_index);
	bool IsEditable();

private:
	
	void setup_controls();	
	
	BGroupLayout		*fMainLayout;
	bool				fEditable;
	
	BMessage			*fDataMessage;
	type_code			fDataType;
	const char			*fDataLabel;
	int32				fDataIndex;

};

#endif
