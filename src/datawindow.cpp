/*
 * Copyright 2019-2021 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */

#include "datawindow.h"
#include "gettype.h"

#include <LayoutBuilder.h>
#include <Catalog.h>
#include <ColumnTypes.h>
#include <Entry.h>
#include <Path.h>
#include <Application.h>


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "DataWindow"


DataWindow::DataWindow(BRect frame,
					BMessage *data_message,
					BString field_name,
					type_code field_type,
					int32 item_count)
	:
	BWindow(frame, B_TRANSLATE("Message data"), B_DOCUMENT_WINDOW_LOOK, B_MODAL_APP_WINDOW_FEEL, B_CLOSE_ON_ESCAPE),
	fDataMessage(data_message),
	fFieldName(field_name),
	fFieldType(field_type),
	fItemCount(item_count)
{

	BString fieldtypename = get_type(fFieldType);
	BString datalabeltext;
	datalabeltext+=fFieldName;
	datalabeltext+=" (";
	datalabeltext+=fieldtypename;
	datalabeltext+=")";

	fDataLabel = new BStringView("datalabel", datalabeltext.String());
	BFont font(be_bold_font);
	fDataLabel->SetFont(&font);

	fDataView = new BColumnListView("dataview",0);
	fDataView->SetInvocationMessage(new BMessage(DW_ROW_CLICKED));
	BIntegerColumn *index_column = new BIntegerColumn(B_TRANSLATE("Index"),70,10,100);
	BStringColumn *value_column = new BStringColumn(B_TRANSLATE("Value"),200,50,1000,0);
	fDataView->AddColumn(index_column,0);
	fDataView->AddColumn(value_column,1);
	display_data();

	fCloseButton = new BButton(B_TRANSLATE("Close"), new BMessage(DW_BUTTON_CLOSE));

	BLayoutBuilder::Group<>(this, B_VERTICAL, B_USE_SMALL_SPACING)
		.SetInsets(B_USE_SMALL_SPACING)
		.Add(fDataLabel)
		.Add(fDataView)
		.Add(fCloseButton)
	.Layout();

}


void
DataWindow::MessageReceived(BMessage *msg)
{

	switch (msg->what)
	{
		case DW_BUTTON_CLOSE:
		{
			Quit();
			break;
		}

		case DW_ROW_CLICKED:
		{
			BRow *selected_row = fDataView->CurrentSelection();
			int32 field_index = static_cast<BIntegerField*>(selected_row->GetField(0))->Value();

			msg->AddInt32("field_index", field_index);
			msg->AddString("field_name", fFieldName);
			msg->AddUInt32("field_type", fFieldType);
			be_app->PostMessage(msg);

			break;
		}

		case DW_UPDATE:
		{
			fDataView->Clear();
			display_data();

			break;
		}

		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}

}


void
DataWindow::display_data()
{

	BString message_item_data;

	for (int32 i=0; i < fItemCount; ++i)
	{
		message_item_data = "";

		switch (fFieldType)
		{
			case B_STRING_TYPE:
				message_item_data=BString(fDataMessage->GetString(fFieldName, i, ""));
				break;

			case B_INT8_TYPE:
			{
				message_item_data<<fDataMessage->GetInt8(fFieldName,i,0);
				break;
			}

			case B_INT16_TYPE:
			{
				message_item_data<<fDataMessage->GetInt16(fFieldName,i,0);
				break;
			}

			case B_INT32_TYPE:
			{
				message_item_data<<fDataMessage->GetInt32(fFieldName,i,0);
				break;
			}

			case B_INT64_TYPE:
			{
				message_item_data<<fDataMessage->GetInt64(fFieldName,i,0);
				break;
			}

			case B_UINT8_TYPE:
			{
				message_item_data<<fDataMessage->GetUInt8(fFieldName,i,0);
				break;
			}

			case B_UINT16_TYPE:
			{
				message_item_data<<fDataMessage->GetUInt16(fFieldName,i,0);
				break;
			}

			case B_UINT32_TYPE:
			{
				message_item_data<<fDataMessage->GetUInt32(fFieldName,i,0);
				break;
			}

			case B_UINT64_TYPE:
			{
				message_item_data<<fDataMessage->GetUInt64(fFieldName,i,0);
				break;
			}

			case B_DOUBLE_TYPE:
			{
				message_item_data<<fDataMessage->GetDouble(fFieldName,i,0);
				break;
			}

			case B_FLOAT_TYPE:
			{
				message_item_data<<fDataMessage->GetFloat(fFieldName,i,0);
				break;
			}

			case B_BOOL_TYPE:
			{
				bool bool_value = fDataMessage->GetBool(fFieldName, i, false);

				if (bool_value)
				{
					message_item_data=B_TRANSLATE("true");
				}
				else
				{
					message_item_data=B_TRANSLATE("false");
				}

				break;
			}

			case B_RGB_COLOR_TYPE:
			{
				// BMessage->FindColor() or GetColor() don´t seem to work on B_RGB_COLOR_TYPE so we
				// have to use the raw FindData() here
				const void *color_ptr;
				ssize_t data_size = sizeof(rgb_color);
				fDataMessage->FindData(fFieldName, B_RGB_COLOR_TYPE, i, &color_ptr, &data_size);
				rgb_color color = *(static_cast<const rgb_color*>(color_ptr));

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
				BRect rect = fDataMessage->GetRect(fFieldName,i, default_rect);
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
				BSize size = fDataMessage->GetSize(fFieldName,i, default_size);
				message_item_data<<size.width;
				message_item_data+=", ";
				message_item_data<<size.height;

				break;
			}

			case B_POINT_TYPE:
			{
				BPoint default_point;
				BPoint point = fDataMessage->GetPoint(fFieldName,i, default_point);
				message_item_data<<point.x;
				message_item_data+=", ";
				message_item_data<<point.y;

				break;
			}


			case B_REF_TYPE:
			{
				entry_ref file_ref;
				status_t result = fDataMessage->FindRef(fFieldName, &file_ref);

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

		BRow *row = new BRow();

		BIntegerField *index_field = new BIntegerField(i);
		BStringField *value_field = new BStringField(message_item_data.String());
		row->SetField(index_field,0);
		row->SetField(value_field,1);

		fDataView->AddRow(row);
	}

}
