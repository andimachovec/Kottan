/*
 * Copyright 2019-2021 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */

#include "editview.h"

#include <Alignment.h>
#include <FilePanel.h>
#include <Catalog.h>
#include <LayoutBuilder.h>
#include <limits>
#include <cstdlib>


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "EditView"


static double
roundTo(double value, uint32 n)
{
	return floor(value * pow(10.0, n) + 0.5) / pow(10.0, n);
}


EditView::EditView(BMessage *data_message,
					type_code data_type,
					const char *data_label,
					int32 data_index)
	:
	BView("editview", B_SUPPORTS_LAYOUT),
	fDataMessage(data_message),
	fDataType(data_type),
	fDataLabel(data_label),
	fDataIndex(data_index)
{

	fEditable=true;
	fDescFont = be_plain_font;
	fDescFont.SetFace(B_ITALIC_FACE);
	fDescColor = ui_color(B_WINDOW_INACTIVE_TEXT_COLOR);

	//create layout
	fMainLayout = new BGroupLayout(B_VERTICAL);
	SetLayout(fMainLayout);

	//initialize controls
	fPopUpMenu = new BPopUpMenu("");
	fPopUpMenu2 = new BPopUpMenu("");
	fRadioButton1 = new BRadioButton("", new BMessage(EV_DATA_CHANGED));
	fRadioButton2 = new BRadioButton("", new BMessage(EV_DATA_CHANGED));
	fIntegerSpinner1 = new BSpinner("","",new BMessage(EV_DATA_CHANGED));
	fIntegerSpinner2 = new BSpinner("","",new BMessage(EV_DATA_CHANGED));
	fIntegerSpinner3 = new BSpinner("","",new BMessage(EV_DATA_CHANGED));
	fIntegerSpinner4 = new BSpinner("","",new BMessage(EV_DATA_CHANGED));
	fDecimalSpinner1 = new BDecimalSpinner("","",new BMessage(EV_DATA_CHANGED));
	fDecimalSpinner2 = new BDecimalSpinner("","",new BMessage(EV_DATA_CHANGED));
	fDecimalSpinner3 = new BDecimalSpinner("","",new BMessage(EV_DATA_CHANGED));
	fDecimalSpinner4 = new BDecimalSpinner("","",new BMessage(EV_DATA_CHANGED));
	fSvDescription = new BStringView(NULL, "");
	fTextCtrl1 = new BTextControl("","",new BMessage(EV_DATA_CHANGED));
	fTextCtrl2 = new BTextControl("","",new BMessage(EV_DATA_CHANGED));
	fTextCtrl3 = new BTextControl("","",new BMessage(EV_DATA_CHANGED));
	fTextCtrl4 = new BTextControl("","",new BMessage(EV_DATA_CHANGED));

	fTextCtrl1->SetModificationMessage(new BMessage(EV_DATA_CHANGED));
	fTextCtrl2->SetModificationMessage(new BMessage(EV_DATA_CHANGED));
	fTextCtrl3->SetModificationMessage(new BMessage(EV_DATA_CHANGED));
	fTextCtrl4->SetModificationMessage(new BMessage(EV_DATA_CHANGED));

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
	if(!IsEditable()) {
		return B_NOT_ALLOWED;
	}

	switch(fDataType)
	{
		case B_BOOL_TYPE:
		{
			bool isTrue = fRadioButton1->Value() == B_CONTROL_ON; // fRadioButton1 represents "true"
			fDataMessage->ReplaceBool(fDataLabel,
									fDataIndex,
									isTrue);
			break;
		}

		case B_INT8_TYPE:
		{
			fDataMessage->ReplaceInt8(fDataLabel,
									fDataIndex,
									static_cast<int8>(fIntegerSpinner1->Value()));
			break;
		}

		case B_INT16_TYPE:
		{
			fDataMessage->ReplaceInt16(fDataLabel,
									fDataIndex,
									static_cast<int16>(fIntegerSpinner1->Value()));
			break;
		}

		case B_INT32_TYPE:
		{
			fDataMessage->ReplaceInt32(fDataLabel, fDataIndex, fIntegerSpinner1->Value());
			break;
		}

		case B_UINT8_TYPE:
		{
			fDataMessage->ReplaceUInt8(fDataLabel,
									fDataIndex,
									static_cast<uint8>(fIntegerSpinner1->Value()));
			break;
		}

		case B_UINT16_TYPE:
		{
			fDataMessage->ReplaceUInt16(fDataLabel,
										fDataIndex,
										static_cast<uint16>(fIntegerSpinner1->Value()));
			break;
		}

		case B_UINT32_TYPE:
		{
			fDataMessage->ReplaceUInt32(fDataLabel,
										fDataIndex,
										static_cast<uint32>(fIntegerSpinner1->Value()));
			break;
		}

		case B_FLOAT_TYPE:
		{
			float float_data = roundTo(std::atof(fDecimalSpinner1->TextView()->Text()), fDecimalSpinner1->Precision());
			fDataMessage->ReplaceFloat(fDataLabel, fDataIndex, float_data);
			break;
		}

		case B_DOUBLE_TYPE:
		{
			double double_data = roundTo(std::atof(fDecimalSpinner1->TextView()->Text()), fDecimalSpinner1->Precision());
			fDataMessage->ReplaceDouble(fDataLabel, fDataIndex, double_data);
			break;
		}

		case B_SIZE_TYPE:
		{
			BSize data_size;
			data_size.height = fDecimalSpinner1->Value();
			data_size.width = fDecimalSpinner2->Value();
			fDataMessage->ReplaceSize(fDataLabel, fDataIndex, data_size);
			break;
		}

		case B_POINT_TYPE:
		{
			BPoint data_point;
			data_point.x = fDecimalSpinner1->Value();
			data_point.y = fDecimalSpinner2->Value();
			fDataMessage->ReplacePoint(fDataLabel, fDataIndex, data_point);

			break;
		}

		case B_RECT_TYPE:
		{
			BRect data_rect;
			data_rect.left = fDecimalSpinner1->Value();
			data_rect.top = fDecimalSpinner2->Value();
			data_rect.right = fDecimalSpinner3->Value();
			data_rect.bottom = fDecimalSpinner4->Value();
			fDataMessage->ReplaceRect(fDataLabel, fDataIndex, data_rect);

			break;
		}

		case B_STRING_TYPE:
		{
			fDataMessage->ReplaceString(fDataLabel, fDataIndex, fTextCtrl1->Text());

			break;
		}

		case B_RGB_COLOR_TYPE:
		{
			rgb_color data_rgbcolor;
			data_rgbcolor.red =  static_cast<uint8>(fIntegerSpinner1->Value());
			data_rgbcolor.green = static_cast<uint8>(fIntegerSpinner2->Value());
			data_rgbcolor.blue = static_cast<uint8>(fIntegerSpinner3->Value());
			data_rgbcolor.alpha = static_cast<uint8>(fIntegerSpinner4->Value());
			fDataMessage->ReplaceData(fDataLabel, B_RGB_COLOR_TYPE, fDataIndex, &data_rgbcolor, sizeof(data_rgbcolor));

			break;
		}

		case B_ALIGNMENT_TYPE:
		{
			BAlignment alignment;
			int32 hindex = fPopUpMenu->IndexOf(fPopUpMenu->FindMarked());
			int32 vindex = fPopUpMenu2->IndexOf(fPopUpMenu2->FindMarked());
			switch(hindex) {
				case 0:
					alignment.SetHorizontal(B_ALIGN_LEFT);
					break;
				case 1:
					alignment.SetHorizontal(B_ALIGN_RIGHT);
					break;
				case 2:
					alignment.SetHorizontal(B_ALIGN_CENTER);
					break;
				case 5:
					alignment.SetHorizontal(B_ALIGN_USE_FULL_WIDTH);
					break;
				case 4:
				default:
					alignment.SetHorizontal(B_ALIGN_HORIZONTAL_UNSET);
					break;
			}
			switch(vindex) {
				case 0:
					alignment.SetVertical(B_ALIGN_TOP);
					break;
				case 1:
					alignment.SetVertical(B_ALIGN_MIDDLE);
					break;
				case 2:
					alignment.SetVertical(B_ALIGN_BOTTOM);
					break;
				case 5:
					alignment.SetVertical(B_ALIGN_USE_FULL_HEIGHT);
					break;
				case 4:
				default:
					alignment.SetVertical(B_ALIGN_VERTICAL_UNSET);
					break;
			}

			fDataMessage->ReplaceAlignment(fDataLabel, fDataIndex, alignment);
			break;
		}

		default:
			return B_BAD_DATA;
			break;
	}

	return B_OK;

}


void
EditView::setup_controls()
{

	switch(fDataType)
	{
		case B_BOOL_TYPE:
		{
			/* Setup controls */
			fRadioButton1->SetLabel(B_TRANSLATE("true"));
			fRadioButton2->SetLabel(B_TRANSLATE("false"));

			BView* view = new BView(NULL, B_SUPPORTS_LAYOUT);
			BLayoutBuilder::Group<>(view, B_VERTICAL)
				.SetExplicitAlignment(BAlignment(B_ALIGN_CENTER, B_ALIGN_MIDDLE))
				.AddGlue()
				.Add(fRadioButton1)
				.Add(fRadioButton2)
				.AddGlue()
			.End();

			/* Retrieve data and set initial status */
			bool data_bool;
			fDataMessage->FindBool(fDataLabel, fDataIndex, &data_bool);
			if(data_bool)
				fRadioButton1->SetValue(B_CONTROL_ON);
			else
				fRadioButton2->SetValue(B_CONTROL_ON);

			/* Add to view hierarchy */
			fMainLayout->AddView(view);

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
			int32 range_min = 0;
			int32 range_max = 0;
			int32 data_int;

			switch(fDataType)
			{
				case B_INT8_TYPE:
				{
					range_min=std::numeric_limits<int8>::lowest();
					range_max=std::numeric_limits<int8>::max();
					int8 data_int8;
					fDataMessage->FindInt8(fDataLabel, fDataIndex, &data_int8);
					data_int = static_cast<int32>(data_int8);
					break;
				}

				case B_INT16_TYPE:
					range_min=std::numeric_limits<int16>::lowest();
					range_max=std::numeric_limits<int16>::max();

					int16 data_int16;
					fDataMessage->FindInt16(fDataLabel, fDataIndex, &data_int16);
					data_int = static_cast<int32>(data_int16);

					break;

				case B_INT32_TYPE:
					range_min=std::numeric_limits<int32>::lowest();
					range_max=std::numeric_limits<int32>::max();
					fDataMessage->FindInt32(fDataLabel, fDataIndex, &data_int);

					break;

				case B_UINT8_TYPE:
				{
					range_min=std::numeric_limits<uint8>::lowest();
					range_max=std::numeric_limits<uint8>::max();
					uint8 data_uint8;
					fDataMessage->FindUInt8(fDataLabel, fDataIndex, &data_uint8);
					data_int = static_cast<int32>(data_uint8);

					break;
				}

				case B_UINT16_TYPE:
				{
					range_min=std::numeric_limits<uint16>::lowest();
					range_max=std::numeric_limits<uint16>::max();
					uint16 data_uint16;
					fDataMessage->FindUInt16(fDataLabel, fDataIndex, &data_uint16);
					data_int = static_cast<int32>(data_uint16);

					break;
				}

				{
				case B_UINT32_TYPE:
					range_min=std::numeric_limits<uint32>::lowest();
					range_max=2147483647;
					uint32 data_uint32;
					fDataMessage->FindUInt32(fDataLabel, fDataIndex, &data_uint32);
					data_int = static_cast<int32>(data_uint32);

					break;
				}
			}

			fIntegerSpinner1->SetRange(range_min, range_max);
			fIntegerSpinner1->SetValue(data_int);

			BString rangeText = BString("")
				.SetToFormat(B_TRANSLATE("Values from %d to %d."),
				static_cast<int32>(range_min), static_cast<int32>(range_max));
			if(fDataType == B_UINT32_TYPE) {
				rangeText.Append(BString("").SetToFormat(B_TRANSLATE("\nValues "
				"from %u to %u cannot be represented\n\tdue to software limitations."),
				static_cast<uint32>(range_max) + 1, (uint32)std::numeric_limits<uint32>::max()));
			}
			fSvDescription->SetText(rangeText.String());
			fSvDescription->SetFont(&fDescFont);
			fSvDescription->SetHighColor(fDescColor);

			fMainLayout->AddView(fIntegerSpinner1);
			fMainLayout->AddView(fSvDescription);

			break;
		}

		case B_FLOAT_TYPE:
		{
			float range_min = std::numeric_limits<float>::lowest();
			float range_max = std::numeric_limits<float>::max();

			float data_float = 0.0f;
			fDataMessage->FindFloat(fDataLabel, fDataIndex, &data_float);

			fDecimalSpinner1->SetRange(range_min, range_max);
			fDecimalSpinner1->SetPrecision(4);
			fDecimalSpinner1->SetStep(0.1);
			fDecimalSpinner1->SetValue(data_float);

			BString rangeText = BString("")
				.SetToFormat(B_TRANSLATE("Values from %e to %e." ),
					range_min, range_max);
			fSvDescription->SetText(rangeText.String());
			fSvDescription->SetFont(&fDescFont);
			fSvDescription->SetHighColor(fDescColor);

			fMainLayout->AddView(fDecimalSpinner1);
			fMainLayout->AddView(fSvDescription);

			break;
		}

		case B_DOUBLE_TYPE:
		{
			double range_min = std::numeric_limits<double>::lowest();
			double range_max = std::numeric_limits<double>::max();

			double data_double = 0.0;
			fDataMessage->FindDouble(fDataLabel, fDataIndex, &data_double);

			fDecimalSpinner1->SetRange(range_min, range_max);
			fDecimalSpinner1->SetPrecision(4);
			fDecimalSpinner1->SetStep(0.1);
			fDecimalSpinner1->SetValue(data_double);

			BString rangeText = BString("")
				.SetToFormat(B_TRANSLATE("Values from %e to %e." ),
					range_min, range_max);
			fSvDescription->SetText(rangeText.String());
			fSvDescription->SetFont(&fDescFont);
			fSvDescription->SetHighColor(fDescColor);

			fMainLayout->AddView(fDecimalSpinner1);
			fMainLayout->AddView(fSvDescription);

			break;
		}

		case B_SIZE_TYPE:
		{
			BSize data_size;
			fDataMessage->FindSize(fDataLabel, fDataIndex, &data_size);

			fDecimalSpinner1->SetMaxValue(10000);
			fDecimalSpinner2->SetMaxValue(10000);

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

			fDecimalSpinner1->SetMaxValue(10000);
			fDecimalSpinner2->SetMaxValue(10000);

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

			fDecimalSpinner1->SetMaxValue(10000);
			fDecimalSpinner2->SetMaxValue(10000);
			fDecimalSpinner3->SetMaxValue(10000);
			fDecimalSpinner4->SetMaxValue(10000);

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
			const void *color_ptr;
			ssize_t data_size = sizeof(rgb_color);
			fDataMessage->FindData(fDataLabel, B_RGB_COLOR_TYPE, fDataIndex, &color_ptr, &data_size);
			rgb_color data_rgbcolor = *(static_cast<const rgb_color*>(color_ptr));

			fIntegerSpinner1->SetRange(0, 255);
			fIntegerSpinner2->SetRange(0, 255);
			fIntegerSpinner3->SetRange(0, 255);
			fIntegerSpinner4->SetRange(0, 255);

			fIntegerSpinner1->SetLabel(B_TRANSLATE("Red:"));
			fIntegerSpinner1->SetValue(data_rgbcolor.red);
			fIntegerSpinner2->SetLabel(B_TRANSLATE("Green:"));
			fIntegerSpinner2->SetValue(data_rgbcolor.green);
			fIntegerSpinner3->SetLabel(B_TRANSLATE("Blue:"));
			fIntegerSpinner3->SetValue(data_rgbcolor.blue);
			fIntegerSpinner4->SetLabel(B_TRANSLATE("Alpha:"));
			fIntegerSpinner4->SetValue(data_rgbcolor.alpha);

			fMainLayout->AddView(fIntegerSpinner1);
			fMainLayout->AddView(fIntegerSpinner2);
			fMainLayout->AddView(fIntegerSpinner3);
			fMainLayout->AddView(fIntegerSpinner4);

			break;
		}

		case B_ALIGNMENT_TYPE:
		{
			BLayoutBuilder::Menu<>(fPopUpMenu)
				.AddItem(B_TRANSLATE("Left"), EV_DATA_CHANGED)
				.AddItem(B_TRANSLATE("Right"), EV_DATA_CHANGED)
				.AddItem(B_TRANSLATE("Center"), EV_DATA_CHANGED)
				.AddSeparator()
				.AddItem(B_TRANSLATE("No horizontal alignment"), EV_DATA_CHANGED)
				.AddItem(B_TRANSLATE("Use full width"), EV_DATA_CHANGED)
			.End();

			BLayoutBuilder::Menu<>(fPopUpMenu2)
				.AddItem(B_TRANSLATE("Top"), EV_DATA_CHANGED)
				.AddItem(B_TRANSLATE("Middle"), EV_DATA_CHANGED)
				.AddItem(B_TRANSLATE("Bottom"), EV_DATA_CHANGED)
				.AddSeparator()
				.AddItem(B_TRANSLATE("No vertical alignment"), EV_DATA_CHANGED)
				.AddItem(B_TRANSLATE("Use full height"), EV_DATA_CHANGED)
			.End();

			BAlignment alignment;
			fDataMessage->FindAlignment(fDataLabel, fDataIndex, &alignment);

			switch(alignment.horizontal) {
				case B_ALIGN_LEFT: // 0
				case B_ALIGN_RIGHT: // 1
				case B_ALIGN_CENTER: // 2
					fPopUpMenu->ItemAt((int32)alignment.horizontal)->SetMarked(true);
					break;
				case B_ALIGN_USE_FULL_WIDTH: // -2L
					fPopUpMenu->FindItem(B_TRANSLATE("Use full width"))->SetMarked(true);
					break;
				case B_ALIGN_HORIZONTAL_UNSET: // -1L
				default:
					fPopUpMenu->FindItem(B_TRANSLATE("No horizontal alignment"))->SetMarked(true);
					break;
			}

			switch(alignment.vertical) {
				case B_ALIGN_TOP:
					fPopUpMenu2->FindItem(B_TRANSLATE("Top"))->SetMarked(true);
					break;
				case B_ALIGN_MIDDLE:
					fPopUpMenu2->FindItem(B_TRANSLATE("Middle"))->SetMarked(true);
					break;
				case B_ALIGN_BOTTOM:
					fPopUpMenu2->FindItem(B_TRANSLATE("Bottom"))->SetMarked(true);
					break;
				case B_ALIGN_USE_FULL_HEIGHT:
					fPopUpMenu2->FindItem(B_TRANSLATE("Use full height"))->SetMarked(true);
					break;
				case B_ALIGN_VERTICAL_UNSET:
				default:
					fPopUpMenu2->FindItem(B_TRANSLATE("No vertical alignment"))->SetMarked(true);
					break;
			}

			BView* view = new BView(NULL, B_SUPPORTS_LAYOUT);
			BLayoutBuilder::Grid<>(view)
				.AddMenuField(new BMenuField(B_TRANSLATE("Horizontal alignment"), fPopUpMenu), 0, 0)
				.AddMenuField(new BMenuField(B_TRANSLATE("Vertical alignment"), fPopUpMenu2), 0, 1)
			.End();
			fMainLayout->AddView(view);

			break;
		}

		default:
		{
			BStringView *not_editable_text = new BStringView("",B_TRANSLATE("not editable"));
			fMainLayout->AddView(not_editable_text);
			fEditable=false;
			break;
		}
	}

}
