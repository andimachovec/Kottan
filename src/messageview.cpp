/*
 * Copyright 2019-2021 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */


#include "messageview.h"
#include "gettype.h"
#include "datawindow.h"

#include <ColumnTypes.h>
#include <Catalog.h>
#include <Looper.h>

#include <vector>


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "MessageView"


MessageView::MessageView(BLooper *TargetLooper)
	:
	BColumnListView("messageview",0)
{

	TargetLooper->AddHandler(this);
		
	SetInvocationMessage(new BMessage(MV_ROW_CLICKED));
	SetTarget(this);
	
	BIntegerColumn *index_column = new BIntegerColumn(B_TRANSLATE("Index"),70,10,100);
	BStringColumn *name_column = new BStringColumn(B_TRANSLATE("Name"),200,50,1000,0);
	BStringColumn *type_column = new BStringColumn(B_TRANSLATE("Type"),200,50,1000,0);
	BIntegerColumn *count_column = new BIntegerColumn(B_TRANSLATE("Number of Items"),120,10,150);

	AddColumn(index_column,0);
	AddColumn(name_column,1);
	AddColumn(type_column,2);
	AddColumn(count_column,3);

}


void
MessageView::MessageReceived(BMessage *msg)
{

	switch(msg->what)
	{

		case MV_ROW_CLICKED:
		{
		
			BRow *selected_row = CurrentSelection();
			BString selected_row_typename(static_cast<BStringField*>(selected_row->GetField(2))->String());
						
			if (selected_row_typename != "B_MESSAGE_TYPE")
			{
			
				//get index path to data of selected field
				std::vector<int32> data_index_path;
				BRow *parent_row;
				bool row_visible;
				BRow *current_row = selected_row;
			
				while (FindParent(current_row, &parent_row, NULL))
				{
					int32 field_index = static_cast<BIntegerField*>(current_row->GetField(0))->Value();
					data_index_path.insert(data_index_path.begin(), field_index);
					current_row = parent_row;
				}
			
				int32 top_parent_index = static_cast<BIntegerField*>(current_row->GetField(0))->Value();
				data_index_path.insert(data_index_path.begin(), top_parent_index);
		
				//create data window
				DataWindow *data_window = new DataWindow(BRect(0, 0, 400,300),
														fDataMessage, 
														data_index_path);
				data_window->CenterOnScreen();
				data_window->Show();
				
			}
			
			break;
		}
				
		default:
		{
			BColumnListView::MessageReceived(msg);
			break;
		}

	}

}


void
MessageView::SetDataMessage(BMessage *message)
{

	fDataMessage = message;
	Clear();

	create_data_rows(fDataMessage);
	
}


void 
MessageView::create_data_rows(BMessage *message, BRow *parent)
{

	char *name;
	type_code type;
	int32 count;
						
	for (int32 i=0; message->GetInfo(B_ANY_TYPE, i, &name, &type, &count) == B_OK; ++i)
	{
		
		BRow *row = new BRow();				
		BString type_name = get_type(type);
		
		BIntegerField *index_field = new BIntegerField(i);
		BStringField *name_field = new BStringField(name);
		BStringField *type_field = new BStringField(get_type(type).String());
		BIntegerField *count_field = new BIntegerField(count);
							
		row->SetField(index_field,0);
		row->SetField(name_field,1);
		row->SetField(type_field,2);
		row->SetField(count_field,3);
							
		AddRow(row, parent);

		if (type_name == "B_MESSAGE_TYPE")
		{			
			BRow *parent_row = row;
			
			for (int32 message_nr = 0; message_nr < count; ++message_nr)
			{
				
				if (count > 1)
				{
					BRow *header_row = new BRow();
					BIntegerField *header_index_field = new BIntegerField(message_nr);
					header_row->SetField(header_index_field,0);
					AddRow(header_row,row);
			
					parent_row = header_row;
				}
				
				BMessage *member_message = new BMessage();
				message->FindMessage(name, message_nr, member_message);
				
				create_data_rows(member_message, parent_row);
			}
		}

	}

}

