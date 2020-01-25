/*
 * Copyright 2019 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */

#include "mainwindow.h"
#include "datawindow.h"
#include "gettype.h"

#include <Alert.h>
#include <LayoutBuilder.h>
#include <Catalog.h>
#include <Application.h>


#include <iostream>
#include <sstream>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "MainWindow"


MainWindow::MainWindow(float left, float top, float right, float bottom)
	: 
	BWindow(BRect(left,top,right,bottom), "Kottan", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{

	//initialize GUI objects
	fTopMenuBar = new BMenuBar("topmenubar");

	fMessageFileTextControl = new BTextControl(B_TRANSLATE("Message File"), "", 
											new BMessage(MW_INSPECTMESSAGEFILE));

	fChooseMessageFileButton = new BButton(B_TRANSLATE("Choose Message File"),
											new BMessage(MW_BUTTON_CHOOSEMESSAGEFILE));
											
	fMessageInfoView = new MessageView();
	fMessageInfoView->SetInvocationMessage(new BMessage(MW_MSGINFO_CLICKED));
	
	
	
	
	fOpenFilePanel = new BFilePanel(B_OPEN_PANEL, 
									new BMessenger(this), 
									NULL,
									B_FILE_NODE,
									false,
									new BMessage(MW_REF_MESSAGEFILE));
	
	
	
	//define menu layout
	BLayoutBuilder::Menu<>(fTopMenuBar)
		.AddMenu(B_TRANSLATE("File"))
			.AddItem(B_TRANSLATE("Quit"), B_QUIT_REQUESTED, 'Q')
		.End()
		.AddMenu(B_TRANSLATE("Help"))
			.AddItem(B_TRANSLATE("About"), MW_MENU_ABOUT)
		.End()
	.End();

	//define main layout
	BLayoutBuilder::Group<>(this, B_VERTICAL,0)
		.SetInsets(0)
		.Add(fTopMenuBar)
		.AddGroup(B_HORIZONTAL)
			.SetInsets(5,5,5,5)
			.Add(fMessageFileTextControl)
			.Add(fChooseMessageFileButton)
		.End()
		.AddGroup(B_HORIZONTAL)
			.SetInsets(5,3,3,3)
			.Add(fMessageInfoView,20)
		//.AddGlue()
	.Layout();


	fCurrentMessage = new BMessage();


}


MainWindow::~MainWindow()
{

	delete fCurrentMessage;
	delete fOpenFilePanel;
	
}


void
MainWindow::MessageReceived(BMessage *msg)
{

	if(msg->WasDropped()) {
		msg->what = MW_REF_MESSAGEFILE;
	}

	switch(msg->what)
	{

		case MW_MENU_ABOUT:
		{
			be_app->PostMessage(B_ABOUT_REQUESTED);
			break;
		}
		
		case MW_BUTTON_CHOOSEMESSAGEFILE:
		{
			
			fOpenFilePanel->Show();
			break;
		}

		case MW_INSPECTMESSAGEFILE:
		{
			inspect_message_file();
			break;
		}
				
		case MW_REF_MESSAGEFILE:
		{
			entry_ref ref;
			msg->FindRef("refs", &ref);
			BEntry target_file(&ref, true);
			BPath target_path(&target_file);
			fMessageFileTextControl->SetText(target_path.Path());
			
			PostMessage(new BMessage(MW_INSPECTMESSAGEFILE));
			Activate(true);
			break;
		}

		case MW_MSGINFO_CLICKED:
		{
			show_message_data();
			break;
		}
				
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}

	}
}


bool
MainWindow::QuitRequested()
{

	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;

}


void
MainWindow::inspect_message_file()
{
		
	BString messagefile_name(fMessageFileTextControl->Text());
	messagefile_name.Trim();
			
	if (messagefile_name != "")
	{
				
		BFile *message_file = new BFile();
					
		status_t fileopen_result = message_file->SetTo(messagefile_name.String(), B_READ_ONLY);
					
		if (fileopen_result == B_OK)
		{
				
			status_t unflatten_result = fCurrentMessage->Unflatten(message_file);
					
			if (unflatten_result == B_OK)
			{
				
				fMessageInfoView->ShowMessageData(fCurrentMessage);
						
			}
			
			else
			{	
				BAlert *errorunflatten_alert = new BAlert("Kottan",
											B_TRANSLATE("Error reading the message from the file!"), 
											"OK");
				errorunflatten_alert->Go();
				
			}
				
		}
		else 
		{
			BAlert *erroropen_alert = new BAlert("Kottan",
											B_TRANSLATE("Error opening the message file!"), 
											"OK");
			erroropen_alert->Go();
		}


		delete message_file;
				
	}

	else
	{
		BAlert *nomessagefile_alert = new BAlert("Kottan",
											B_TRANSLATE("Please specify a file to analyze!"), 
											"OK");
			
		nomessagefile_alert->Go();
			
	}

}


void
MainWindow::show_message_data()
{

	BIntegerField* index_field = (BIntegerField*)fMessageInfoView->CurrentSelection()->GetField(0);
	int32 msg_index=index_field->Value();
			
	char *name;
	int32 items_count;
	type_code type;
			
	fCurrentMessage->GetInfo(B_ANY_TYPE, msg_index, &name, &type, &items_count);
			
	BString message_item_data;
	std::vector<BString> message_data;
			
	for (int32 i=0; i < items_count; ++i)
	{
			
		message_item_data="";
				
		switch (type)
		{
				
			case B_STRING_TYPE:
				message_item_data=BString(fCurrentMessage->GetString(name, i, ""));
				break;
						
			case B_INT8_TYPE:
			{
				message_item_data<<fCurrentMessage->GetInt8(name,i,0);
				break;
			}

			case B_INT16_TYPE:
			{
				message_item_data<<fCurrentMessage->GetInt16(name,i,0);
				break;
			}

			case B_INT32_TYPE:
			{
				message_item_data<<fCurrentMessage->GetInt32(name,i,0);
				break;
			}

			case B_INT64_TYPE:
			{
				message_item_data<<fCurrentMessage->GetInt64(name,i,0);
				break;
			}

			case B_UINT8_TYPE:
			{
				message_item_data<<fCurrentMessage->GetUInt8(name,i,0);
				break;
			}

			case B_UINT16_TYPE:
			{
				message_item_data<<fCurrentMessage->GetUInt16(name,i,0);
				break;
			}

			case B_UINT32_TYPE:
			{
				message_item_data<<fCurrentMessage->GetUInt32(name,i,0);
				break;
			}

			case B_UINT64_TYPE:
			{
				message_item_data<<fCurrentMessage->GetUInt64(name,i,0);
				break;
			}
	
			case B_DOUBLE_TYPE:
			{
				message_item_data<<fCurrentMessage->GetDouble(name,i,0);
				break;
			}

			case B_FLOAT_TYPE:
			{
				message_item_data<<fCurrentMessage->GetFloat(name,i,0);
				break;
			}
	
			case B_BOOL_TYPE:
			{
				message_item_data=bool2bstring(fCurrentMessage->GetBool(name, i, false));
				break;
			}

			case B_RGB_COLOR_TYPE:
			{
				rgb_color default_color;
				rgb_color color = fCurrentMessage->GetColor(name,i,default_color);
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
				BRect rect = fCurrentMessage->GetRect(name,i, default_rect);
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
				BSize size = fCurrentMessage->GetSize(name,i, default_size);
				message_item_data<<size.width;
				message_item_data+=", ";
				message_item_data<<size.height;
				
				break;
			}
			
			case B_POINT_TYPE:
			{
				BPoint default_point;
				BPoint point = fCurrentMessage->GetPoint(name,i, default_point);
				message_item_data<<point.x;
				message_item_data+=", ";
				message_item_data<<point.y;
				
				break;
			}
			
			
			case B_REF_TYPE:
			{
				entry_ref file_ref; 
				status_t result = fCurrentMessage->FindRef(name, &file_ref);
				
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





BString 
MainWindow::bool2bstring(bool value)
{

	if (value)
	{
		return BString("true");
	}
	else
	{
		return BString("false");
	}


}
