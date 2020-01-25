#include "messageview.h"
#include "gettype.h"

#include <ColumnTypes.h>
#include <Catalog.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "MessageWindow"


MessageView::MessageView()
	:
	BColumnListView("messageview",0)
{

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
MessageView::ShowMessageData(BMessage *message)
{

	Clear();

	char *name;
	type_code type;
	int32 count;
						
	for (int32 i=0; message->GetInfo(B_ANY_TYPE, i, &name, &type, &count) == B_OK; ++i)
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




