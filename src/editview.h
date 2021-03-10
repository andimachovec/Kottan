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
	EditView(type_code datatype, BMessage *data);
	bool IsEditable();

private:
	type_code 			fDataType;
	BGroupLayout		*fMainLayout;
	bool				fEditable;
};

#endif
