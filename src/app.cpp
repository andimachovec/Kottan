/*
 * Copyright 2019 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */

#include "app.h"

#include <AboutWindow.h>
#include <Catalog.h>
#include <Resources.h>
#include <AppFileInfo.h>

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

