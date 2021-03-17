/*
 * Copyright 2019-2020 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */

#include "mainwindow.h"
#include "gettype.h"

#include <Alert.h>
#include <LayoutBuilder.h>
#include <Catalog.h>
#include <Application.h>

#include <iostream>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "MainWindow"


MainWindow::MainWindow(float left, float top, float right, float bottom)
	: 
	BWindow(BRect(left,top,right,bottom), "Kottan", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{

	//initialize GUI objects
	fTopMenuBar = new BMenuBar("topmenubar");

	fMessageFileTextControl = new BTextControl(B_TRANSLATE("Message File"),"", 
											new BMessage(MW_ENTERED_MESSAGEFILE));
	
	

	fChooseMessageFileButton = new BButton(B_TRANSLATE("Choose Message File"),
											new BMessage(MW_BUTTON_CHOOSEMESSAGEFILE));
											
	fMessageInfoView = new MessageView(this);
	
	
	
	
	
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


	


}


MainWindow::~MainWindow()
{

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

		// Help/About was clicked
		case MW_MENU_ABOUT:
		{
			be_app->PostMessage(B_ABOUT_REQUESTED);
			break;
		}
		
		// Choose file button was clicked
		case MW_BUTTON_CHOOSEMESSAGEFILE:
		{
			
			fOpenFilePanel->Show();
			break;
		}
			
		// message file path was entered manually	
		case MW_ENTERED_MESSAGEFILE:
		{
			
			BString messagefile_path(fMessageFileTextControl->Text());
			messagefile_path.Trim();
			
			if (messagefile_path.IsEmpty())
			{
				BAlert *nomessagefile_alert = new BAlert("Kottan",
													B_TRANSLATE("Please specify a file to analyze!"), 
													"OK");	
				nomessagefile_alert->Go();
			}
			
			else
			{
				SetTitle(messagefile_path.String());
				
				BEntry messagefile_entry(messagefile_path);
				entry_ref ref;
				messagefile_entry.GetRef(&ref);
				
				BMessage inspect_message(MW_INSPECTMESSAGEFILE);
				inspect_message.AddRef("msgfile",&ref);
				be_app->PostMessage(&inspect_message);
			}

			break;
		}
			
		//message file was supplied via file dialog or drag&drop	
		case MW_REF_MESSAGEFILE:
		{
			//get filename from file ref
			entry_ref ref;
			msg->FindRef("refs", &ref);
			BEntry target_file(&ref, true);
			BPath target_path(&target_file);
			
			
			fMessageFileTextControl->SetText(target_path.Path());
			SetTitle(target_path.Path());
			
			BMessage inspect_message(MW_INSPECTMESSAGEFILE);
			inspect_message.AddRef("msgfile",&ref);
			be_app->PostMessage(&inspect_message);
				
			break;
		}

		//get back the data message from the app object
		case MW_OPEN_REPLY:
		{
			bool open_success;
			msg->FindBool("success", &open_success);

			fMessageInfoView->Clear();	
			
			if (open_success)
			{
				void *data_msg_pointer;
				status_t openmsg_result = msg->FindPointer("data_msg_pointer", &data_msg_pointer);
				
				BMessage *data_message = static_cast<BMessage*>(data_msg_pointer);
				fMessageInfoView->SetDataMessage(data_message);
			}
			
			else 
			{
				
				const char *error_text; 
				msg->FindString("error_text", &error_text);
				BAlert *message_open_alert = new BAlert("Kottan",
												error_text, 
												"OK");
				message_open_alert->Go();								
			}
			
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









