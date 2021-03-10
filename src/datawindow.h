/*
 * Copyright 2019-2020 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */

#ifndef DATAWINDOW_H
#define DATAWINDOW_H

#include <Window.h>
#include <String.h>
#include <ColumnListView.h>
#include <Button.h>
#include <StringView.h>

#include <vector>


enum 
{
	DW_BUTTON_CLOSE = 'dw00',
	DW_ROW_CLICKED
};


class DataWindow : public BWindow {
public:
	DataWindow(BRect frame, BMessage *data_message, std::vector<int32> index_path);
	void MessageReceived(BMessage *msg);


private:
	void get_field_data(BMessage *data_message);
	void display_data();

	BColumnListView		*fDataView;
	BButton				*fCloseButton;
	BStringView			*fDataLabel;
	
	BMessage			*fDataMessage;
	std::vector<int32> 	fIndexPath;
	
	const char 			*fFieldName;
	type_code			fFieldType;
	int32				fItemCount;

};

#endif
