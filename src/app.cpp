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

#include <iostream>


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "App"


const char* kAppName = "Kottan";
const char* kAppSignature = "application/x-vnd.BlueSky-Kottan";


App::App()
	: 
	BApplication(kAppSignature)

{

}


void
App::MessageReceived(BMessage *msg)
{

	switch(msg->what)
	{

		case MW_INSPECTMESSAGEFILE:
		{
			entry_ref ref;
			msg->FindRef("msgfile", &ref);
			BFile *message_file = new BFile();
					
			status_t fileopen_result = message_file->SetTo(&ref, B_READ_ONLY);

			BString error_text;	
			bool message_read_success = false;
			
			
			if (fileopen_result == B_OK)
			{
				status_t unflatten_result = fDataMessage->Unflatten(message_file);
					
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
			
			delete message_file;
			break;
		}

		// a row was clicked in MessageView and we get the index path
		case MW_ROW_SELECTED:
		{	
			//follow the path to the selected data		
			BMessage *current_message = fDataMessage;	
			
			int32 path_items_count;
			msg->GetInfo("selection_path", NULL, &path_items_count);
			
			for (int32 path_index = path_items_count-1;  // we go through the index values in 
				 path_index >= 0;						// in reverse
				 --path_index)
			{
				int32 current_index;
				msg->FindInt32("selection_path", path_index, &current_index);
				
				type_code current_type;
				char *current_name;
				int32 current_item_count;
					
				current_message->GetInfo(B_ANY_TYPE, 
										current_index, 
										&current_name,
										&current_type,
										&current_item_count);		
					 	 
				if (current_type == B_MESSAGE_TYPE)
				{	
					int32 find_index=0;
					
					if (current_item_count > 1)
					{
						--path_index;
						msg->FindInt32("selection_path", path_index, &find_index);
					}
						
					BMessage *temp_message = new BMessage();
					status_t result = current_message->FindMessage(current_name, find_index, temp_message);
					current_message = temp_message;							
				}
			
				if (path_index == 0) //last item
				{
					
					fSelectedMessage = current_message;
					fSelectedFieldName = current_name;
					fSelectedFieldType = current_type;
					fSelectedFieldItemCount = current_item_count;
				
				}	
								
			}
			
			DataWindow *data_window = new DataWindow(BRect(0,0,400,300),
														fSelectedMessage,
														fSelectedFieldName,
														fSelectedFieldType,
												        fSelectedFieldItemCount);
													 
			data_window->CenterOnScreen();
			data_window->Show();		
			
			break;
		}
					
		case DW_ROW_CLICKED:	
		{	
			
			int32 field_index;
			msg->FindInt32("field_index", &field_index);
			
			EditWindow *edit_window = new EditWindow(BRect(0,0,300,200), fSelectedMessage, fSelectedFieldType, fSelectedFieldName, field_index);
			edit_window->CenterOnScreen();
			edit_window->Show();
			
			break;
		}

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
	aboutwindow->AddDescription(B_TRANSLATE("An inspector for archived BMessages"));
	aboutwindow->AddExtraInfo("");
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

	fDataMessage = new BMessage();

	fMainWindow = new MainWindow(100,100,750,400);
	fMainWindow->CenterOnScreen();
	fMainWindow->Show();
	
}


int
main(int argc, char** argv)
{

	App *haiku_app = new App();
	haiku_app->Run();

	delete haiku_app;

	return 0;

}

