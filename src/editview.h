/*
 * Copyright 2019-2021 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */

#ifndef EDITVIEW_H
#define EDITVIEW_H

#include <View.h>
#include <Message.h>
#include <GroupLayout.h>
#include <TextControl.h>
#include <StringView.h>
#include <MenuItem.h>
#include <MenuField.h>
#include <PopUpMenu.h>
#include <RadioButton.h>
#include <Spinner.h>
#include <DecimalSpinner.h>


enum
{
	EV_DATA_CHANGED='ev00',
};


class EditView : public BView {
public:
	EditView(BMessage *data_message, type_code data_type, const char *data_label, int32 data_index);
	bool IsEditable();
	status_t SaveData();

private:

	void setup_controls();

	BGroupLayout		*fMainLayout;
	bool				fEditable;

	BMessage			*fDataMessage;
	type_code			fDataType;
	const char			*fDataLabel;
	int32				fDataIndex;

	BPopUpMenu			*fPopUpMenu;
	BPopUpMenu			*fPopUpMenu2;
	BRadioButton		*fRadioButton1;
	BRadioButton		*fRadioButton2;
	BSpinner			*fIntegerSpinner1;
	BSpinner			*fIntegerSpinner2;
	BSpinner			*fIntegerSpinner3;
	BSpinner			*fIntegerSpinner4;
	BDecimalSpinner		*fDecimalSpinner1;
	BDecimalSpinner		*fDecimalSpinner2;
	BDecimalSpinner		*fDecimalSpinner3;
	BDecimalSpinner		*fDecimalSpinner4;
	BTextControl		*fTextCtrl1;
	BTextControl		*fTextCtrl2;
	BTextControl		*fTextCtrl3;
	BTextControl		*fTextCtrl4;
};

#endif
