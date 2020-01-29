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

#include <sstream>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "MainWindow"


MainWindow::MainWindow(float left, float top, float right, float bottom)
	: 
	BWindow(BRect(left,top,right,bottom), "Kottan", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{

	//initialize GUI objects
	fTopMenuBar = new BMenuBar("topmenubar");

	fMessageFileTextControl = new BTextControl(B_TRANSLATE("Message File"), B_TRANSLATE("drag message file here"), 
											new BMessage(MW_INSPECTMESSAGEFILE));
	//set text face to italic
	BTextView *text_view = fMessageFileTextControl->TextView();
	BFont text_font;
	text_view->GetFontAndColor(0,&text_font,NULL);
	text_font.SetFace(B_ITALIC_FACE);
	text_view->SetFontAndColor(&text_font);
	

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
			
			//switch text face back to regular
			BTextView *text_view = fMessageFileTextControl->TextView();
			BFont text_font;
			text_view->GetFontAndColor(0,&text_font,NULL);
			text_font.SetFace(B_REGULAR_FACE|B_OUTLINED_FACE);
			text_view->SetFontAndColor(&text_font);
			
			
			
			fMessageFileTextControl->SetText(target_path.Path());
			SetTitle(target_path.Path());
			PostMessage(new BMessage(MW_INSPECTMESSAGEFILE));
			Activate(true);
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
				fMessageInfoView->SetDataMessage(fCurrentMessage);
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












