/*
 * Copyright 2019-2020 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */


#include "editview.h"


#include <FilePanel.h>
#include <Catalog.h>

#include <iostream>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "EditView"

EditView::EditView(BMessage *data_message, type_code data_type, const char *data_label, int32 data_index)
	:
	BView("editview", B_SUPPORTS_LAYOUT),
	fDataMessage(data_message),
	fDataType(data_type),	
	fDataLabel(data_label),
	fDataIndex(data_index)
{

	fEditable=true;

	//create layout
	fMainLayout = new BGroupLayout(B_VERTICAL);
	SetLayout(fMainLayout);

	//initialize controls
	fPopUpMenu = new BPopUpMenu("");
	fIntegerSpinner1 = new BSpinner("","",new BMessage(EV_DATA_CHANGED));
	fIntegerSpinner2 = new BSpinner("","",new BMessage(EV_DATA_CHANGED));
	fIntegerSpinner3 = new BSpinner("","",new BMessage(EV_DATA_CHANGED));
	fIntegerSpinner4 = new BSpinner("","",new BMessage(EV_DATA_CHANGED));
	fDecimalSpinner1 = new BDecimalSpinner("","",new BMessage(EV_DATA_CHANGED));
	fDecimalSpinner2 = new BDecimalSpinner("","",new BMessage(EV_DATA_CHANGED));
	fDecimalSpinner3 = new BDecimalSpinner("","",new BMessage(EV_DATA_CHANGED));
	fDecimalSpinner4 = new BDecimalSpinner("","",new BMessage(EV_DATA_CHANGED));
	fTextCtrl1 = new BTextControl("","",new BMessage(EV_DATA_CHANGED));
	fTextCtrl2 = new BTextControl("","",new BMessage(EV_DATA_CHANGED));
	fTextCtrl3 = new BTextControl("","",new BMessage(EV_DATA_CHANGED));
	fTextCtrl4 = new BTextControl("","",new BMessage(EV_DATA_CHANGED));
	
	
	//fill the controls needed for the specified data type with values and add them to the layout
	setup_controls();

}


bool
EditView::IsEditable()
{

	return fEditable;

}


status_t 
EditView::SaveData()
{
	const void *data;
		
	switch(fDataType)
	{	
		case B_BOOL_TYPE:
		{
			bool bool_data = static_cast<bool>(fPopUpMenu->FindMarkedIndex());
			data=&bool_data;
			break;	
		}
		
		case B_STRING_TYPE:
			data = fTextCtrl1->Text();
			break;
	
		default:
			return B_BAD_DATA;
			break;
	}
	
	fDataMessage->ReplaceData(fDataLabel, fDataType, fDataIndex, data, sizeof(data));
	
	return B_OK;
}


void 
EditView::setup_controls()
{

	switch(fDataType)
	{

		case B_BOOL_TYPE:
		{			
			bool data_bool; 
			fDataMessage->FindBool(fDataLabel, fDataIndex, &data_bool);	
			int32 default_item = static_cast<int32>(data_bool);
		
			fPopUpMenu->AddItem(new BMenuItem("false", new BMessage(EV_DATA_CHANGED)));
			fPopUpMenu->AddItem(new BMenuItem("true", new BMessage(EV_DATA_CHANGED)));
			fPopUpMenu->ItemAt(default_item)->SetMarked(true);
			
			BMenuField *bool_select = new BMenuField("",fPopUpMenu);
			fMainLayout->AddView(bool_select);
			break;
		}
		
		
		//all integer types get the same input field but need different range constraints
		case B_INT8_TYPE:
		case B_INT16_TYPE:
		case B_INT32_TYPE:
		case B_UINT8_TYPE:
		case B_UINT16_TYPE:
		case B_UINT32_TYPE:
		{
			int32 range_min, range_max;
			int32 data_int;
		
			switch(fDataType)
			{			
				case B_INT8_TYPE:
				{		
					range_min=-128;
					range_max=127;
					int8 data_int8;
					fDataMessage->FindInt8(fDataLabel, fDataIndex, &data_int8);
					data_int = static_cast<int32>(data_int8);
					break;
				}	
	
				case B_INT16_TYPE:
					range_min=-32768;
					range_max=32767;
					break;

				case B_INT32_TYPE:
					range_min=-2147483648;
					range_max=2147483647;
					fDataMessage->FindInt32(fDataLabel, fDataIndex, &data_int);
					
					break;
					
				case B_UINT8_TYPE:
				{
					range_min=0;
					range_max=255;
					uint8 data_uint8;
					fDataMessage->FindUInt8(fDataLabel, fDataIndex, &data_uint8);
					data_int = static_cast<int32>(data_uint8);
					
					break;
				}
				
				case B_UINT16_TYPE:
				{
					range_min=0;
					range_max=65535;
					break;
					
					uint16 data_uint16;
					fDataMessage->FindUInt16(fDataLabel, fDataIndex, &data_uint16);
					data_int = static_cast<int32>(data_uint16);
				}
		
				{
				case B_UINT32_TYPE:
					range_min=0;
					range_max=2147483647;
					break;
				}	
			}
			
			fIntegerSpinner1->SetRange(range_min, range_max);			
			fIntegerSpinner1->SetValue(data_int);
			fMainLayout->AddView(fIntegerSpinner1);
			
			break;
		}
		

		case B_SIZE_TYPE:
		{
			BSize data_size;
			fDataMessage->FindSize(fDataLabel, fDataIndex, &data_size);
			
			fDecimalSpinner1->SetLabel(B_TRANSLATE("Height:"));
			fDecimalSpinner1->SetValue(data_size.height);
			fDecimalSpinner2->SetLabel(B_TRANSLATE("Width:"));	
			fDecimalSpinner2->SetValue(data_size.width);	
			
			fMainLayout->AddView(fDecimalSpinner1);
			fMainLayout->AddView(fDecimalSpinner2);			
			
			break;
		}

		
		case B_POINT_TYPE:
		{
			BPoint data_point;
			fDataMessage->FindPoint(fDataLabel, fDataIndex, &data_point);
				
			fDecimalSpinner1->SetLabel("X:");
			fDecimalSpinner1->SetValue(data_point.x);
			fDecimalSpinner2->SetLabel("Y:");
			fDecimalSpinner2->SetValue(data_point.y);
			
			fMainLayout->AddView(fDecimalSpinner1);
			fMainLayout->AddView(fDecimalSpinner2);	
			
			break;
		}

	
		case B_RECT_TYPE:
		{	
			
			BRect data_rect;
			fDataMessage->FindRect(fDataLabel, fDataIndex, &data_rect);
			
			fDecimalSpinner1->SetLabel(B_TRANSLATE("Left:"));
			fDecimalSpinner1->SetValue(data_rect.left);
			fDecimalSpinner2->SetLabel(B_TRANSLATE("Top:"));
			fDecimalSpinner2->SetValue(data_rect.top);
			fDecimalSpinner3->SetLabel(B_TRANSLATE("Right:"));
			fDecimalSpinner3->SetValue(data_rect.right);
			fDecimalSpinner4->SetLabel(B_TRANSLATE("Bottom:"));
			fDecimalSpinner4->SetValue(data_rect.bottom);

			fMainLayout->AddView(fDecimalSpinner1);
			fMainLayout->AddView(fDecimalSpinner2);
			fMainLayout->AddView(fDecimalSpinner3);
			fMainLayout->AddView(fDecimalSpinner4);

			break;
		} 

		case B_STRING_TYPE:
		{
			const char *data_string; 
			fDataMessage->FindString(fDataLabel, fDataIndex, &data_string);
			
			fTextCtrl1->SetText(data_string);
			fMainLayout->AddView(fTextCtrl1);
			break;
		}

		case B_RGB_COLOR_TYPE:
		{
		
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
