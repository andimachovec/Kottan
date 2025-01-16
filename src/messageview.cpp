/*
 * Copyright 2019-2021 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */

#include "messageview.h"
#include "gettype.h"

#include <ColumnTypes.h>
#include <Catalog.h>
#include <Window.h>


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "MessageView"


MessageView::MessageView()
	:
	BColumnListView("messageview",0)
{
	SetInvocationMessage(new BMessage(MV_ROW_CLICKED));

	BIntegerColumn *index_column = new BIntegerColumn(B_TRANSLATE("Index"),70,10,100);
	BStringColumn *name_column = new BStringColumn(B_TRANSLATE("Name"),200,50,1000,0);
	BStringColumn *type_column = new BStringColumn(B_TRANSLATE("Type"),200,50,1000,0);
	BIntegerColumn *count_column = new BIntegerColumn(B_TRANSLATE("Number of items"),120,10,150);

	AddColumn(index_column,0);
	AddColumn(name_column,1);
	AddColumn(type_column,2);
	AddColumn(count_column,3);

}


void
MessageView::SetDataMessage(BMessage *message)
{

	fDataMessage = message;
	create_data_rows(fDataMessage);
	if (CountRows() == 1)
	{
		ExpandOrCollapse(RowAt(0), true);
	}
}


void
MessageView::MessageDropped(BMessage *msg, BPoint point)
{

	Window()->PostMessage(msg);

}


void
MessageView::UpdateData()
{

	Clear();
	create_data_rows(fDataMessage);
	if (CountRows() == 1)
	{
		ExpandOrCollapse(RowAt(0), true);
	}
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
