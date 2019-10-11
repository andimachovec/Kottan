#include "mainwindow.h"

#include <Alert.h>
#include <LayoutBuilder.h>
#include <Catalog.h>
#include <Application.h>
#include <FilePanel.h>

#include <iostream>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "MainWindow"


MainWindow::MainWindow(float left, float top, float right, float bottom)
	: 
	BWindow(BRect(left,top,right,bottom), "Kottan", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{

	//initialize GUI objects
	fTopMenuBar = new BMenuBar("topmenubar");
	fMessageFileTextControl = new BTextControl(B_TRANSLATE("Message File"), "", NULL);
	fChooseMessageFileButton = new BButton(B_TRANSLATE("Choose Message File"),
											new BMessage(MW_BUTTON_CHOOSEMESSAGEFILE));
											
	fAnalyzeMessageFileButton = new BButton(B_TRANSLATE("Analyze Message File"),
											new BMessage(MW_BUTTON_ANALYZEMESSAGEFILE));
											

	
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
			.Add(fMessageFileTextControl)
			.Add(fChooseMessageFileButton)
		.End()
		.Add(fAnalyzeMessageFileButton)
		.AddGlue()
	.Layout();

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
		
			BFilePanel *messagefile_filepanel = new BFilePanel(B_OPEN_PANEL, 
																new BMessenger(this), 
																NULL,
																B_FILE_NODE,
																false,
																new BMessage(MW_REF_MESSAGEFILE));
			messagefile_filepanel->Show();
			break;
		}

		
		case MW_BUTTON_ANALYZEMESSAGEFILE:
		{

			BString messagefile_name(fMessageFileTextControl->Text());
			messagefile_name.Trim();
			
			if (messagefile_name != "")
			{
				
				BFile *message_file = new BFile();
				BMessage *message = new BMessage();
					
				status_t fileopen_result = message_file->SetTo(messagefile_name.String(), B_READ_ONLY);
					
				if (fileopen_result == B_OK)
				{
				
					status_t unflatten_result = message->Unflatten(message_file);
					
					if (unflatten_result == B_OK)
					{
				
					
						message->PrintToStream();
				
				
				
				
				
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
				delete message;
				
			}
			else
			{
				BAlert *nomessagefile_alert = new BAlert("Kottan",
													B_TRANSLATE("Please specify a file to analyze!"), 
													"OK");
			
				nomessagefile_alert->Go();
			
			}
			
			break;
		}
		
		
		case MW_REF_MESSAGEFILE:
		{
			
			entry_ref ref;
			msg->FindRef("refs", &ref);
			BEntry target_dir(&ref, true);
			BPath target_path(&target_dir);
			fMessageFileTextControl->SetText(target_path.Path());
			 
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
