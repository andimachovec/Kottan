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
#include <RecentItems.h>


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "MainWindow"


extern const char* kAppName;
extern const char* kAppSignature;


MainWindow::MainWindow(BRect geometry)
	:
	BWindow(geometry, kAppName, B_DOCUMENT_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{

	//initialize GUI objects
	fTopMenuBar = new BMenuBar("topmenubar");
	fMessageInfoView = new MessageView();
	fOpenFilePanel = new BFilePanel(B_OPEN_PANEL,
									NULL,
									NULL,
									B_FILE_NODE,
									false,
									new BMessage(B_REFS_RECEIVED));

	BMenuItem* openItem = new BMenuItem(BRecentFilesList::NewFileListMenu(
							B_TRANSLATE("Open" B_UTF8_ELLIPSIS),
							NULL, NULL, be_app, 9, true, NULL, kAppSignature),
							new BMessage(MW_OPEN_MESSAGEFILE));
	openItem->SetShortcut('O', 0);

	//define menu layout
	BLayoutBuilder::Menu<>(fTopMenuBar)
		.AddMenu(B_TRANSLATE("File"))
			.AddItem(openItem)
			.AddItem(B_TRANSLATE("Save"), MW_SAVE_MESSAGEFILE, 'S')
			.AddItem(B_TRANSLATE("Reload"), MW_RELOAD_FROM_FILE, 'R')
			.AddSeparator()
			.AddItem(B_TRANSLATE("Quit"), B_QUIT_REQUESTED, 'Q')
		.End()
		.AddMenu(B_TRANSLATE("Help"))
			.AddItem(B_TRANSLATE("About"), MW_MENU_ABOUT)
		.End()
	.End();

	fTopMenuBar->FindItem(MW_SAVE_MESSAGEFILE)->SetEnabled(false);
	fTopMenuBar->FindItem(MW_RELOAD_FROM_FILE)->SetEnabled(false);

	//define main layout
	BLayoutBuilder::Group<>(this, B_VERTICAL,0)
		.SetInsets(0)
		.Add(fTopMenuBar)
		.AddGroup(B_HORIZONTAL)
			.SetInsets(-1,-1,-1,-1)
			.Add(fMessageInfoView,20)
	.Layout();

	fUnsaved = false;
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
		msg->what = B_REFS_RECEIVED;
		be_app->PostMessage(msg);
		return;
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
			if(!ContinueAction(kNotsaved_alert_text,
								kNotsaved_alert_cancel,
								kNotsaved_alert_continue))
			{
				break;
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

		//get back the data message from the app object
		case MW_OPEN_REPLY:
		{
			bool open_success;
			BString window_title;
			msg->FindBool("success", &open_success);
			msg->FindString("filename", &window_title);

			fMessageInfoView->Clear();

			if (open_success)
			{
				void *data_msg_pointer;
				msg->FindPointer("data_msg_pointer", &data_msg_pointer);

				BMessage *data_message = static_cast<BMessage*>(data_msg_pointer);
				fMessageInfoView->SetDataMessage(data_message);
				fTopMenuBar->FindItem(MW_RELOAD_FROM_FILE)->SetEnabled(true);

				if (fUnsaved)
					switch_unsaved_state(false);

				window_title.Prepend(": ");
				window_title.Prepend(kAppName);

				SetTitle(window_title);
			}
			else
			{
				SetTitle(kAppName);

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


			BStringField *type_field = static_cast<BStringField*>(selected_row->GetField(2));
			if (type_field == NULL)
			{
				break;
			}

			BString selected_row_typename(type_field->String());
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
			switch_unsaved_state(true);

			break;
		}

		case MW_WAS_SAVED:
		{
			switch_unsaved_state(false);
			break;
		}

		//message file was changed
		case MW_CONFIRM_RELOAD:
		{
			if (ContinueAction(
				B_TRANSLATE("The message file has changed. Do you want to reload it?"),
				B_TRANSLATE("Cancel"),
				B_TRANSLATE("Reload")))
			{
				//trigger reload
				PostMessage(MW_RELOAD_FROM_FILE);
			}

			break;
		}

		// notify App() to reload the message from file
		case MW_RELOAD_FROM_FILE:
		{
			if (!ContinueAction(
				B_TRANSLATE("The message data was changed but not saved. Do you really want to reload?"),
				B_TRANSLATE("Cancel"),
				B_TRANSLATE("Reload")))
			{
				break;
			}

			be_app->PostMessage(msg);

			break;
		}

		// update MessageView with newly loaded data
		case MW_UPDATE_MESSAGEVIEW:
		{
			fMessageInfoView->UpdateData();
			switch_unsaved_state(false);
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

	if (!ContinueAction(
		B_TRANSLATE("The message data was changed but not saved. Do you really want to quit?"),
		B_TRANSLATE("Cancel"),
		B_TRANSLATE("Quit")))
	{
		return false;
	}

	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;

}


bool
MainWindow::ContinueAction(const char *alert_text,
							const char *button_label_cancel,
							const char *button_label_continue)
{
	if (!fUnsaved)
		return true;

	BAlert *not_saved_alert = new BAlert(
		"",
		alert_text,
		button_label_cancel,
		button_label_continue,
		NULL,
		B_WIDTH_AS_USUAL,
		B_WARNING_ALERT);

	if (not_saved_alert->Go() == 0)
	{
		return false;
	}

	return true;

}


void
MainWindow::switch_unsaved_state(bool unsaved_state)
{

	fTopMenuBar->FindItem(MW_SAVE_MESSAGEFILE)->SetEnabled(unsaved_state);

	if (unsaved_state)
	{
		if(!fUnsaved)  // do nothing if unsaved state is already set
		{
			BString title(Title());
			title.Prepend("*");
			SetTitle(title.String());

			fUnsaved = true;
		}
	}
	else
	{
		if (fUnsaved)
		{
			BString title(Title());
			title.RemoveChars(0,1);
			SetTitle(title.String());

			fUnsaved = false;
		}
	}

}
