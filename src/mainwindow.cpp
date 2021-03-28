/*
 * Copyright 2019-2021 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */

#include "mainwindow.h"

#include <Alert.h>
#include <LayoutBuilder.h>
#include <Catalog.h>
#include <Application.h>
#include <ColumnTypes.h>
#include <Entry.h>
#include <Path.h>


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "MainWindow"


MainWindow::MainWindow(float left, float top, float right, float bottom)
	: 
	BWindow(BRect(left,top,right,bottom), "Kottan", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{

	//initialize GUI objects
	fTopMenuBar = new BMenuBar("topmenubar");										
	fMessageInfoView = new MessageView();
	fOpenFilePanel = new BFilePanel(B_OPEN_PANEL, 
									new BMessenger(this), 
									NULL,
									B_FILE_NODE,
									false,
									new BMessage(MW_REF_MESSAGEFILE));
	
	//define menu layout
	BLayoutBuilder::Menu<>(fTopMenuBar)
		.AddMenu(B_TRANSLATE("File"))
			.AddItem(B_TRANSLATE("Open"), MW_OPEN_MESSAGEFILE, 'O')
			.AddItem(B_TRANSLATE("Save"), MW_SAVE_MESSAGEFILE, 'S')
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
			.SetInsets(5,3,3,3)
			.Add(fMessageInfoView,20)		
	.Layout();

	fNotSaved = false;
	
}


MainWindow::~MainWindow()
{

	delete fOpenFilePanel;	
	
}


void
MainWindow::MessageReceived(BMessage *msg)
{

	if(msg->WasDropped()) 
	{
		if(fNotSaved)
		{
			if(continue_action())
			{
				msg->what = MW_REF_MESSAGEFILE;
			}
			else
			{
				msg->what = MW_DO_NOTHING;
			}
		
		}
		else
		{
			msg->what = MW_REF_MESSAGEFILE;
		}
	}

	switch(msg->what)
	{

		// Help/About was clicked
		case MW_MENU_ABOUT:
		{
			be_app->PostMessage(B_ABOUT_REQUESTED);
			break;
		}
		
		// Open file menu was selected
		case MW_OPEN_MESSAGEFILE:
		{
			if (fNotSaved)
			{
				if (!continue_action())
				{
					break;
				}
			}
			
			fOpenFilePanel->Show();
			break;
		}
			
		// Save file menu was selected	
		case MW_SAVE_MESSAGEFILE:
		{
			
			be_app->PostMessage(msg);
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
			
			SetTitle(target_path.Path());
			
			BMessage inspect_message(MW_INSPECTMESSAGEFILE);
			inspect_message.AddRef("msgfile",&ref);
			be_app->PostMessage(&inspect_message);
				
			break;
		}

		//get back the data message from the app object
		case MW_OPEN_REPLY:
		{
			fNotSaved = false;	
			
			bool open_success;
			msg->FindBool("success", &open_success);

			fMessageInfoView->Clear();	
			
			if (open_success)
			{
				void *data_msg_pointer;
				msg->FindPointer("data_msg_pointer", &data_msg_pointer);
				
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
		
		case MV_ROW_CLICKED:
		{
			BRow *selected_row = fMessageInfoView->CurrentSelection();
			BString selected_row_typename(
								static_cast<BStringField*>(selected_row->GetField(2))->String());
						
			if (selected_row_typename != "B_MESSAGE_TYPE")
			{
				//get index path to data of selected field
				BMessage selection_path_msg(MW_ROW_SELECTED);
				BRow *parent_row;
				BRow *current_row = selected_row;
			
				while (fMessageInfoView->FindParent(current_row, &parent_row, NULL))
				{
					int32 field_index = 
									static_cast<BIntegerField*>(current_row->GetField(0))->Value();
									
					selection_path_msg.AddInt32("selection_path",field_index);
					current_row = parent_row;
				}
			
				int32 top_parent_index = 
									static_cast<BIntegerField*>(current_row->GetField(0))->Value();
				selection_path_msg.AddInt32("selection_path",top_parent_index);
			
				be_app->PostMessage(&selection_path_msg);	
			}
			
			break;
		}
		
		case MW_WAS_EDITED:
		{	
			if(!fNotSaved)
			{
				BString title(Title());
				title.Prepend("*");
				SetTitle(title.String());
			
				fNotSaved = true;
			}

			break;
		}
		
		
		case MW_WAS_SAVED:
		{
			if (fNotSaved)
			{
				BString title(Title());
				title.RemoveChars(0,1);
				SetTitle(title.String());

				fNotSaved = false;
			}
		
			break;
		}
		
		//do nothing and don´t forward to base class
		case MW_DO_NOTHING:
			break;
		
		//forward all unhandled messages to the base class
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

	if (fNotSaved)
	{
		if (!continue_action())
		{
			return false;
		}	
	}

	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
			
}


bool
MainWindow::continue_action()
{
	BAlert *not_saved_alert = new BAlert(
		"", 
		B_TRANSLATE("The message data was changed but not saved. Do you really want to continue?"),
		B_TRANSLATE("No"),
		B_TRANSLATE("Yes"),
		NULL,
		B_WIDTH_AS_USUAL,
		B_WARNING_ALERT);
		
	if (not_saved_alert->Go() == 0)
	{
		return false;
	}		

	return true;

}
