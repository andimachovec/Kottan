/*
 * Copyright 2019-2021 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */

#include "app.h"
#include "mainwindow.h"
#include "datawindow.h"
#include "editwindow.h"

#include <AboutWindow.h>
#include <Catalog.h>
#include <Resources.h>
#include <AppFileInfo.h>
#include <Path.h>
#include <File.h>


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "App"


const char* kAppName = "Kottan";
const char* kAppSignature = "application/x-vnd.BlueSky-Kottan";


App::App()
	: 
	BApplication(kAppSignature)

{

	fDataMessage = new BMessage();
	fMessageList = new BObjectList<IndexMessage>(20, false);
	fMessageFile = new BFile();
	
}


App::~App()
{

	delete fDataMessage;
	delete fMessageFile;
	
}


void
App::MessageReceived(BMessage *msg)
{

	switch(msg->what)
	{
		//receive file reference, read the file and extract into message
		case MW_INSPECTMESSAGEFILE:
		{
			entry_ref ref;
			msg->FindRef("msgfile", &ref);
					
			status_t fileopen_result = fMessageFile->SetTo(&ref, B_READ_WRITE);

			BString error_text;	
			bool message_read_success = false;
			
			if (fileopen_result == B_OK)
			{
				status_t unflatten_result = fDataMessage->Unflatten(fMessageFile);
					
				if (unflatten_result == B_OK)
				{
					message_read_success=true;
				}
				else
				{	
					error_text = B_TRANSLATE("Error reading the message from the file!"); 
				}
			}
			else 
			{
				error_text = B_TRANSLATE("Error opening the message file!");
			}

			BMessage open_reply_msg(MW_OPEN_REPLY);
			open_reply_msg.AddBool("success", message_read_success);

			if (message_read_success)
			{
				open_reply_msg.AddPointer("data_msg_pointer", fDataMessage);
			}			
			else
			{
				open_reply_msg.AddString("error_text", error_text.String());
			}
			
			fMainWindow->PostMessage(&open_reply_msg);
			
			break;
		}

		// get info about clicked row and open data window
		case MW_ROW_SELECTED:
		{	
			get_selection_data(msg);
				
			BMessage *dw_message;
			
			if (fMessageList->CountItems() > 0)
			{
				dw_message = fMessageList->FirstItem()->message;
			}
			else
			{
				dw_message = fDataMessage;
			}
			
			fDataWindow = new DataWindow(BRect(0,0,400,300),
										dw_message,
										fSelectedName,
										fSelectedType,
								        fSelectedItemCount);

			fDataWindow->CenterIn(fMainWindow->Frame());
			fDataWindow->MoveBy(0, 128);
			fDataWindow->MoveOnScreen(B_MOVE_IF_PARTIALLY_OFFSCREEN);
			fDataWindow->Show();
			
			break;
		}

		// get info about clicked row in DataWindow and open EditWindow
		case DW_ROW_CLICKED:	
		{	
			int32 field_index;
			msg->FindInt32("field_index", &field_index);
			
			BMessage *ew_message;
			
			if (fMessageList->CountItems() > 0)
			{
				ew_message = fMessageList->FirstItem()->message;
			}
			else
			{
				ew_message = fDataMessage;
			}
			
			EditWindow *edit_window = new EditWindow(BRect(0,0,0,0), 
													ew_message,
													fSelectedType,
													fSelectedName,
													field_index);
			edit_window->CenterIn(fDataWindow->Frame());
			edit_window->Show();
			
			break;
		}

		// save edited data to the main data message
		case EW_BUTTON_SAVE:
		{			
			if (fMessageList->CountItems() > 0)
			{
				for( int32 i = 1; i < fMessageList->CountItems(); ++i)
				{
					fMessageList->ItemAt(i)->message->ReplaceMessage(
														fMessageList->ItemAt(i-1)->field_name,
														fMessageList->ItemAt(i-1)->field_index,
														fMessageList->ItemAt(i-1)->message);											
				}	
	
				fDataMessage->ReplaceMessage(fMessageList->LastItem()->field_name,
											fMessageList->LastItem()->field_index,
											fMessageList->LastItem()->message);
			}	
			
			fDataWindow->PostMessage(DW_UPDATE);
			fMainWindow->PostMessage(MW_WAS_EDITED);
			
			break;
		}

		// save message data to file
		case MW_SAVE_MESSAGEFILE:
		{
			fMessageFile->Seek(0, SEEK_SET);
			status_t flatten_result = fDataMessage->Flatten(fMessageFile);
			
			if (flatten_result != B_OK)
			{
				
			}
			
			fMainWindow->PostMessage(MW_WAS_SAVED);
			
			break;
		}

		// delegate all unhandled messages to the base class
		default:
		{
			BApplication::MessageReceived(msg);
			break;
		}
	}
	
}


void 
App::AboutRequested()
{

	BAboutWindow *aboutwindow = new BAboutWindow(kAppName, kAppSignature);

	const char *authors[] =
	{
		"Andi Machovec (BlueSky)",
		NULL
	};

	BString code_contributors;
	BString translators;

	code_contributors << B_TRANSLATE("Code contributions by:");
	code_contributors << "\n" 
					  << "Humdinger\n";

	translators << B_TRANSLATE("Translations by:");
	translators << "\n"
				<< "Begasus\n"
				<< "Alex Hitech\n";

	BString extra_info;
	extra_info.Append(code_contributors);
	extra_info.Append("\n");
	extra_info.Append(translators);

	BResources *appresource = BApplication::AppResources();
	size_t size;
	version_info *appversion = (version_info *)appresource->LoadResource('APPV',1,&size);
	BString version_string;
	version_string<<appversion->major;
	version_string+=".";
	version_string<<appversion->middle;
	version_string+=".";
	version_string<<appversion->minor;

	aboutwindow->AddCopyright(2019, "Andi Machovec");
	aboutwindow->AddAuthors(authors);
	aboutwindow->SetVersion(version_string.String());
	aboutwindow->AddDescription(B_TRANSLATE("An editor for archived BMessages"));
	aboutwindow->AddExtraInfo(extra_info.String());
	aboutwindow->Show();

}


bool 
App::QuitRequested()
{

	return true;
}


void
App::ReadyToRun()
{

	fMainWindow = new MainWindow(100,100,750,400);
	fMainWindow->CenterOnScreen();
	fMainWindow->Show();
	
}


void
App::get_selection_data(BMessage *selection_path_message)
{

	// follow the index path to the selected data		
	BMessage *current_message = fDataMessage;	

	fMessageList->MakeEmpty();

	int32 path_items_count;
	selection_path_message->GetInfo("selection_path", NULL, &path_items_count);
			
	type_code current_type;
	char *current_name;
	int32 current_item_count;
			
	for (int32 path_index = path_items_count-1;  // loop through the index values in 
		 path_index >= 0;				   		 // in reverse order
		 --path_index)
	{
		int32 current_index;
		selection_path_message->FindInt32("selection_path", path_index, &current_index);
					
		current_message->GetInfo(B_ANY_TYPE, 
								current_index, 
								&current_name,
								&current_type,
								&current_item_count);		
					 	 
		if (current_type == B_MESSAGE_TYPE)
		{	
			int32 member_index=0;
			
			if (current_item_count > 1)
			{
				--path_index;
				selection_path_message->FindInt32("selection_path", path_index, &member_index);
			}
						
			BMessage *temp_message = new BMessage();	
			current_message->FindMessage(current_name, member_index, temp_message);
				
			IndexMessage *index_message = new IndexMessage();
			index_message->message = new BMessage(*temp_message);
			index_message->field_index = member_index;
			index_message->field_name = current_name;
			fMessageList->AddItem(index_message, 0);
					
			current_message = temp_message;
		}										
	}	

	fSelectedName = current_name;
	fSelectedType = current_type;
	fSelectedItemCount = current_item_count;

}


int
main(int argc, char** argv)
{

	App *haiku_app = new App();
	haiku_app->Run();

	delete haiku_app;
	return 0;

}
