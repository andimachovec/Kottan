/*
 * Copyright 2019 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */

#include "datawindow.h"

#include <LayoutBuilder.h>
#include <Catalog.h>
#include <ColumnTypes.h>



#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "DataWindow"


DataWindow::DataWindow(BRect frame, BString fieldname, BString fieldtypename, std::vector<BString> messagedata)
	:
	BWindow(frame, B_TRANSLATE("Message Data"), B_TITLED_WINDOW,B_CLOSE_ON_ESCAPE)
{


	BString datalabeltext;
	datalabeltext+=fieldname;
	datalabeltext+=" (";
	datalabeltext+=fieldtypename;
	datalabeltext+=")";
	
	fDataLabel = new BStringView("datalabel", datalabeltext.String());
	BFont font(be_bold_font);
	fDataLabel->SetFont(&font);
	
	
	
	fDataView = new BColumnListView("dataview",0);
	BIntegerColumn *index_column = new BIntegerColumn(B_TRANSLATE("Index"),70,10,100);
	BStringColumn *value_column = new BStringColumn(B_TRANSLATE("Value"),200,50,1000,0);
	fDataView->AddColumn(index_column,0);
	fDataView->AddColumn(value_column,1);
	
	std::vector<BString>::iterator messagedata_iter;
	int index=0;
	
	for (messagedata_iter=messagedata.begin(); messagedata_iter!=messagedata.end(); ++messagedata_iter)
	{
	
		BRow *row = new BRow();
		
		BIntegerField *index_field = new BIntegerField(index);
		BStringField *value_field = new BStringField(messagedata_iter->String());
		row->SetField(index_field,0);
		row->SetField(value_field,1);
		
		fDataView->AddRow(row);
	
		++index;
	}
	
	
	fCloseButton = new BButton(B_TRANSLATE("Close"), new BMessage(DW_BUTTON_CLOSE));
	

	BLayoutBuilder::Group<>(this, B_VERTICAL,0)
		.SetInsets(5)
		.AddGroup(B_HORIZONTAL)
			.SetInsets(5)
			.Add(fDataLabel)
		.End()	
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
			Quit();
			break;
	
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}

	}

}
