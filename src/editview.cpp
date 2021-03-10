/*
 * Copyright 2019-2020 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */


#include "editview.h"

#include <TextControl.h>
#include <StringView.h>
#include <MenuItem.h>
#include <MenuField.h>
#include <PopUpMenu.h>
#include <Spinner.h>
#include <FilePanel.h>

#include <iostream>


EditView::EditView(type_code datatype, BMessage *data)
	:
	BView("editview", B_SUPPORTS_LAYOUT),
	fDataType(datatype)	
{

	fEditable=true;

	//create layout
	fMainLayout = new BGroupLayout(B_VERTICAL);
	SetLayout(fMainLayout);

	//construct needed controls for the different data types and add them to the layout
	switch(fDataType)
	{

		case B_BOOL_TYPE:
		{
		
			BPopUpMenu *bool_popup = new BPopUpMenu("boolpopup");
			bool_popup->AddItem(new BMenuItem("false", NULL));
			bool_popup->AddItem(new BMenuItem("true", NULL));
			bool_popup->ItemAt(0)->SetMarked(true);
			BMenuField *bool_select = new BMenuField("",bool_popup);
			fMainLayout->AddView(bool_select);
			break;
		}

		//all integer types get the same input field but need different range constraints
		case B_INT8_TYPE:
		case B_INT16_TYPE:
		case B_INT32_TYPE:
		case B_INT64_TYPE:
		case B_UINT8_TYPE:
		case B_UINT16_TYPE:
		case B_UINT32_TYPE:
		case B_UINT64_TYPE:
		{
			BSpinner *integer_spinner = new BSpinner("","",new BMessage);
			fMainLayout->AddView(integer_spinner);
		
			int32 range_min, range_max;  // B_UINT64 only gets 32bit ranges for now, let´s hope no one finds out ;-)
		
			switch(fDataType)
			{			
				case B_INT8_TYPE:
					range_min=-128;
					range_max=127;
					break;
		
				case B_INT16_TYPE:
					range_min=-32768;
					range_max=32767;
					break;

				case B_INT32_TYPE:
				case B_INT64_TYPE:
					range_min=-2147483648;
					range_max=2147483647;
					break;
					
				case B_UINT8_TYPE:
					range_min=0;
					range_max=255;
					break;
		
				case B_UINT16_TYPE:
					range_min=0;
					range_max=65535;
					break;
		
				case B_UINT32_TYPE:
				case B_UINT64_TYPE:
					range_min=0;
					range_max=2147483647;
					break;
			}
			
			integer_spinner->SetRange(range_min, range_max);			
			break;
		}

		case B_RECT_TYPE:
		{	
			BTextControl *left_text = new BTextControl("left:","",new BMessage());
			BTextControl *top_text = new BTextControl("top:","",new BMessage());
			BTextControl *right_text = new BTextControl("right:","",new BMessage());
			BTextControl *bottom_text = new BTextControl("bottom:","",new BMessage());

			fMainLayout->AddView(left_text);
			fMainLayout->AddView(top_text);
			fMainLayout->AddView(right_text);
			fMainLayout->AddView(bottom_text);

			break;
		}

		case B_STRING_TYPE:
		{
			BTextControl *string_text = new BTextControl("","",new BMessage());
			fMainLayout->AddView(string_text);
			break;
		}

		default:
		{
			BStringView *not_editable_text = new BStringView("","not editable");
			fMainLayout->AddView(not_editable_text);
			fEditable=false;
			break;
		}
	}

}


bool
EditView::IsEditable()
{

	return fEditable;

}

