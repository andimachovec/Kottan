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
#include <FindDirectory.h>
#include <NodeMonitor.h>


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "App"


const char* kAppName = "Kottan";
const char* kAppSignature = "application/x-vnd.BlueSky-Kottan";


App::App()
	:
	BApplication(kAppSignature)

{

	fDataMessage = new BMessage();
	fMessageList = new BObjectList<IndexMessage, false>(20);
	fMessageFile = new BFile();
	fDataWindow = NULL;
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

			stop_watching(be_app_messenger); //stop watching file nodes

			msg->FindRef("msgfile", &fMessageFileRef);

			status_t fileopen_result = fMessageFile->SetTo(&fMessageFileRef, B_READ_ONLY);

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

				// start watching the file for changes
				BEntry entry(&fMessageFileRef);
				node_ref nref;
				entry.GetNodeRef(&nref);
				watch_node(&nref, B_WATCH_STAT|B_WATCH_INTERIM_STAT, be_app_messenger);
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
			//fDataWindow->AddToSubset(fMainWindow);
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
			fDataWindow->SetFeel(B_NORMAL_WINDOW_FEEL);
			//edit_window->AddToSubset(fDataWindow);
			edit_window->Show();

			break;
		}

		// save edited data to the main data message
		case EW_BUTTON_SAVE:
		{
			fDataWindow->SetFeel(B_MODAL_APP_WINDOW_FEEL);

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

		// edit window closed, make data window modal again
		case EW_BUTTON_CANCEL:
		{
			fDataWindow->SetFeel(B_MODAL_APP_WINDOW_FEEL);
			break;
		}


		// save message data to file
		case MW_SAVE_MESSAGEFILE:
		{
			fMessageFile->SetTo(&fMessageFileRef, B_WRITE_ONLY|B_ERASE_FILE);
			fDataMessage->Flatten(fMessageFile);
			fMainWindow->PostMessage(MW_WAS_SAVED);

			break;
		}

		// triggered by the node monitor if one of the stat parameters changes
		case B_NODE_MONITOR:
		{
			int32 stat_changed_flags;
			msg->FindInt32("fields", &stat_changed_flags);

			// only compare messages when the file was modified
			if ((stat_changed_flags & B_STAT_MODIFICATION_TIME) != 0)
			{
				BMessage *temp_msg = new BMessage();
				fMessageFile->SetTo(&fMessageFileRef, B_READ_ONLY);
				temp_msg->Unflatten(fMessageFile);

				//only request reload if the data in the message has actually changed
				if (!(temp_msg->HasSameData(*fDataMessage, false, true)))
				{
					fMainWindow->PostMessage(MW_CONFIRM_RELOAD);
				}

				delete temp_msg;
			}

			break;
		}

		// reload message data from file and update main and data window
		case MW_RELOAD_FROM_FILE:
		{
			fMessageFile->SetTo(&fMessageFileRef, B_READ_ONLY);
			fDataMessage->Unflatten(fMessageFile);

			fMainWindow->PostMessage(MW_UPDATE_MESSAGEVIEW);

			if (fDataWindow != NULL)
			{
				fDataWindow->PostMessage(DW_UPDATE);
			}

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
	BString icon_designers;
	BString translators;

	code_contributors << B_TRANSLATE("Code contributions by:");
	code_contributors << "\n"
					  << "Humdinger\n"
					  << "Jaidyn Ann (JadedCtrl)\n";

	icon_designers 	<< B_TRANSLATE("Icon design by:");
	icon_designers 	<< "\n"
					<< "zuMi\n";

	translators << B_TRANSLATE("Translations by:");
	translators << "\n"
				<< "Begasus\n"
				<< "Alex Hitech\n"
				<< "Davidmp\n"
				<< "Florentina Mușat\n"
				<< "cafeina\n"
				<< "Fredrik Modéen\n"
				<< "Emir Sarı\n"
				<< "Amor\n"
				<< "Humdinger\n"
				<< "jt15s\n"
				<< "munecito\n"
				<< "Briseur\n"
				<< "Loïc\n"
				<< "tmtfx\n"
				<< "FaBE\n"
				<< "mazbrili\n"
				<< "zumikebbe\n"
				<< "Johan Wagenheim\n"
				<< "Victor Domingos\n"
				<< "al-popa\n"
				<< "itvanya\n"
				<< "butyoutried\n"
				<< "viktroniko\n";

	BString extra_info;
	extra_info.Append(code_contributors);
	extra_info.Append("\n");
	extra_info.Append(icon_designers);
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

	//save window frame
	BRect mainwindow_frame = fMainWindow->Frame();

	BFile *settings_file = new BFile(fSettingsFileName, B_READ_WRITE);

	BMessage settings_message;
	settings_message.Unflatten(settings_file);
	settings_message.ReplaceRect("mainwindow_frame", mainwindow_frame);
	settings_file->Seek(0, SEEK_SET); //rewind file position to beginning
	settings_message.Flatten(settings_file);

	delete settings_file;

	return true;
}


void
App::ReadyToRun()
{

	//determine settings file name
	BPath settings_path;
	find_directory(B_USER_SETTINGS_DIRECTORY, &settings_path);
	fSettingsFileName << settings_path.Path() << "/Kottan_settings";

	//get main window frame from settings file (if there is any)
	BRect mainwindow_frame;

	BFile *settings_file = new BFile(fSettingsFileName, B_READ_WRITE|B_CREATE_FILE);
	BMessage settings_message;
	status_t unflatten_result = settings_message.Unflatten(settings_file);

	bool frame_retrieved=false;

	if (unflatten_result == B_OK)
	{
		status_t frame_result = settings_message.FindRect("mainwindow_frame",
														&mainwindow_frame);
		if (frame_result == B_OK)
		{
			frame_retrieved = true;
		}
	}

	// set default frame and add to settings message
	if (!frame_retrieved)
	{
		mainwindow_frame.Set(100,100,750,500);
		settings_message.AddRect("mainwindow_frame", mainwindow_frame);
		settings_message.Flatten(settings_file);
	}

	// create and show main window
	fMainWindow = new MainWindow(mainwindow_frame);

	if (!frame_retrieved)
	{
		fMainWindow->CenterOnScreen();
	}
	fMainWindow->Show();

	delete settings_file;

}


void
App::ArgvReceived(int32 argc, char **argv)
{

	// construct file ref for the specified file
	BEntry entry(argv[1]);
	entry_ref ref;
	entry.GetRef(&ref);

	BMessage refMsg(B_REFS_RECEIVED);
	refMsg.AddRef("refs", &ref);

	RefsReceived(&refMsg);

}


void
App::RefsReceived(BMessage *msg)
{

	entry_ref ref;
	msg->FindRef("refs", &ref);

	// send inspect message to open the file
	BMessage inspect_message(MW_INSPECTMESSAGEFILE);
	inspect_message.AddRef("msgfile",&ref);
	PostMessage(&inspect_message);

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
