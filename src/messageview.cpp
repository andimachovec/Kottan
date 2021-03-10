/*
 * Copyright 2019-2020 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */


#include "messageview.h"
#include "gettype.h"
#include "datawindow.h"

#include <ColumnTypes.h>
#include <Catalog.h>
#include <Looper.h>
#include <Entry.h>
#include <Path.h>
#include <WindowStack.h>
#include <ObjectList.h>

#include <vector>
#include <iostream>


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
		
				
				//follow the path to the selected data		
				BMessage *current_message = fDataMessage;	
				std::vector<int32>::iterator data_index_path_iter;
				
				for(data_index_path_iter = data_index_path.begin();
					data_index_path_iter != data_index_path.end();
					++data_index_path_iter)
					
				{
					int32 current_index = *data_index_path_iter;
					
					type_code current_type;
					char *current_name;
					int32 current_item_count;
					
					current_message->GetInfo(B_ANY_TYPE, 
											current_index, 
											&current_name,
											&current_type,
											&current_item_count);		
					 
					 
					if (current_type == B_MESSAGE_TYPE)
					{
						
						int32 find_index=0;
						if (current_item_count > 1)
						{
							++data_index_path_iter;
							find_index = *data_index_path_iter;
						}
						
						BMessage *temp_message = new BMessage();
						status_t result = current_message->FindMessage(current_name, find_index, temp_message);
						current_message = temp_message;
												
					}
					
					if (data_index_path_iter == data_index_path.end()-1) //last item
					{
						show_message_data(current_message, current_name, current_type, current_item_count);
					}
					
				} 
			
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


void
MessageView::show_message_data(BMessage *data_message, const char *field_name, 
								type_code field_type, int32 number_of_items)
{

	BString message_item_data;
	std::vector<BString> message_data;
			
	for (int32 i=0; i < number_of_items; ++i)
	{
			
		message_item_data = "";
				
		switch (field_type)
		{
				
			case B_STRING_TYPE:
				message_item_data=BString(data_message->GetString(field_name, i, ""));
				break;
						
			case B_INT8_TYPE:
			{
				message_item_data<<data_message->GetInt8(field_name,i,0);
				break;
			}

			case B_INT16_TYPE:
			{
				message_item_data<<data_message->GetInt16(field_name,i,0);
				break;
			}

			case B_INT32_TYPE:
			{
				message_item_data<<data_message->GetInt32(field_name,i,0);
				break;
			}

			case B_INT64_TYPE:
			{
				message_item_data<<data_message->GetInt64(field_name,i,0);
				break;
			}

			case B_UINT8_TYPE:
			{
				message_item_data<<data_message->GetUInt8(field_name,i,0);
				break;
			}

			case B_UINT16_TYPE:
			{
				message_item_data<<data_message->GetUInt16(field_name,i,0);
				break;
			}

			case B_UINT32_TYPE:
			{
				message_item_data<<data_message->GetUInt32(field_name,i,0);
				break;
			}

			case B_UINT64_TYPE:
			{
				message_item_data<<data_message->GetUInt64(field_name,i,0);
				break;
			}
	
			case B_DOUBLE_TYPE:
			{
				message_item_data<<data_message->GetDouble(field_name,i,0);
				break;
			}

			case B_FLOAT_TYPE:
			{
				message_item_data<<data_message->GetFloat(field_name,i,0);
				break;
			}
	
			case B_BOOL_TYPE:
			{
				message_item_data=bool2bstring(data_message->GetBool(field_name, i, false));
				break;
			}

			case B_RGB_COLOR_TYPE:
			{
				rgb_color default_color;
				rgb_color color = data_message->GetColor(field_name,i,default_color);
				message_item_data<<color.red;
				message_item_data+=", ";
				message_item_data<<color.green;
				message_item_data+=", ";
				message_item_data<<color.blue;
				message_item_data+=", ";
				message_item_data<<color.alpha;
			
				break;
			}
			
			case B_RECT_TYPE:
			{
				BRect default_rect;
				BRect rect = data_message->GetRect(field_name,i, default_rect);
				message_item_data<<rect.left;
				message_item_data+=", ";
				message_item_data<<rect.top;
				message_item_data+=", ";
				message_item_data<<rect.right;
				message_item_data+=", ";
				message_item_data<<rect.bottom;
				
				break;
			}
			
			case B_SIZE_TYPE:
			{
				BSize default_size;
				BSize size = data_message->GetSize(field_name,i, default_size);
				message_item_data<<size.width;
				message_item_data+=", ";
				message_item_data<<size.height;
				
				break;
			}
			
			case B_POINT_TYPE:
			{
				BPoint default_point;
				BPoint point = data_message->GetPoint(field_name,i, default_point);
				message_item_data<<point.x;
				message_item_data+=", ";
				message_item_data<<point.y;
				
				break;
			}
			
			
			case B_REF_TYPE:
			{
				entry_ref file_ref; 
				status_t result = data_message->FindRef(field_name, &file_ref);
				
				if (result == B_OK)
				{	
					
					BEntry file_entry(&file_ref);
					BPath file_path(&file_entry);
					message_item_data << file_path.Path();
					
				}
			
				break;
			}
			
			default:
				message_item_data=B_TRANSLATE("data cannot be displayed");
				break;
		}
				
				
		message_data.push_back(message_item_data);
				
	}
	
	
	DataWindow *data_window = new DataWindow(BRect(0, 0, 400,300), field_name, field_type, message_data);
	data_window->CenterOnScreen();
	data_window->Show();

}


