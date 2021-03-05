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
		
			BIntegerField* index_field = (BIntegerField*)CurrentSelection()->GetField(0);
			
			int32 field_index = index_field->Value();
			char *field_name;
			int32 items_count;
			type_code field_type;
			
			
			fDataMessage->GetInfo(B_ANY_TYPE, field_index, &field_name, &field_type, &items_count); 
			
			if (field_type != B_MESSAGE_TYPE)
			{
				show_message_data(field_name, field_type,items_count);
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

	char *name;
	type_code type;
	int32 count;
						
	for (int32 i=0; fDataMessage->GetInfo(B_ANY_TYPE, i, &name, &type, &count) == B_OK; ++i)
	{
		
		BRow *row = new BRow();
						
		BIntegerField *index_field = new BIntegerField(i);
		BStringField *name_field = new BStringField(name);
		BStringField *type_field = new BStringField(get_type(type).String());
		BIntegerField *count_field = new BIntegerField(count);
							
		row->SetField(index_field,0);
		row->SetField(name_field,1);
		row->SetField(type_field,2);
		row->SetField(count_field,3);
							
		AddRow(row);
	
	}

}


void
MessageView::show_message_data(const char *name, type_code type, int32 number_of_items)
{

				
	BString message_item_data;
	std::vector<BString> message_data;
			
	for (int32 i=0; i < number_of_items; ++i)
	{
			
		message_item_data="";
				
		switch (type)
		{
				
			case B_STRING_TYPE:
				message_item_data=BString(fDataMessage->GetString(name, i, ""));
				break;
						
			case B_INT8_TYPE:
			{
				message_item_data<<fDataMessage->GetInt8(name,i,0);
				break;
			}

			case B_INT16_TYPE:
			{
				message_item_data<<fDataMessage->GetInt16(name,i,0);
				break;
			}

			case B_INT32_TYPE:
			{
				message_item_data<<fDataMessage->GetInt32(name,i,0);
				break;
			}

			case B_INT64_TYPE:
			{
				message_item_data<<fDataMessage->GetInt64(name,i,0);
				break;
			}

			case B_UINT8_TYPE:
			{
				message_item_data<<fDataMessage->GetUInt8(name,i,0);
				break;
			}

			case B_UINT16_TYPE:
			{
				message_item_data<<fDataMessage->GetUInt16(name,i,0);
				break;
			}

			case B_UINT32_TYPE:
			{
				message_item_data<<fDataMessage->GetUInt32(name,i,0);
				break;
			}

			case B_UINT64_TYPE:
			{
				message_item_data<<fDataMessage->GetUInt64(name,i,0);
				break;
			}
	
			case B_DOUBLE_TYPE:
			{
				message_item_data<<fDataMessage->GetDouble(name,i,0);
				break;
			}

			case B_FLOAT_TYPE:
			{
				message_item_data<<fDataMessage->GetFloat(name,i,0);
				break;
			}
	
			case B_BOOL_TYPE:
			{
				message_item_data=bool2bstring(fDataMessage->GetBool(name, i, false));
				break;
			}

			case B_RGB_COLOR_TYPE:
			{
				rgb_color default_color;
				rgb_color color = fDataMessage->GetColor(name,i,default_color);
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
				BRect rect = fDataMessage->GetRect(name,i, default_rect);
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
				BSize size = fDataMessage->GetSize(name,i, default_size);
				message_item_data<<size.width;
				message_item_data+=", ";
				message_item_data<<size.height;
				
				break;
			}
			
			case B_POINT_TYPE:
			{
				BPoint default_point;
				BPoint point = fDataMessage->GetPoint(name,i, default_point);
				message_item_data<<point.x;
				message_item_data+=", ";
				message_item_data<<point.y;
				
				break;
			}
			
			
			case B_REF_TYPE:
			{
				entry_ref file_ref; 
				status_t result = fDataMessage->FindRef(name, &file_ref);
				
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
	
	
	DataWindow *data_window = new DataWindow(BRect(0, 0, 400,300), name, get_type(type), message_data);
	data_window->CenterOnScreen();
	data_window->Show();

}


