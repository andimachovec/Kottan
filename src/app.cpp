#include "app.h"

#include <AboutWindow.h>
#include <Catalog.h>



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

	aboutwindow->AddCopyright(2019, "Andi Machovec");
	aboutwindow->AddAuthors(authors);
	aboutwindow->SetVersion("0.2.1");
	aboutwindow->AddDescription(B_TRANSLATE("a BMessage inspector"));
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

