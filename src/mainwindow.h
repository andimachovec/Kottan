#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Window.h>
#include <Button.h>
#include <MenuBar.h>
#include <TextControl.h>
#include <String.h>
#include <Path.h>
#include <Message.h>
#include <File.h>

#include "messageinfoview.h"


enum
{
	MW_MENU_ABOUT ='mw00',
	MW_BUTTON_CHOOSEMESSAGEFILE,
	MW_REF_MESSAGEFILE,
	MW_BUTTON_INSPECTMESSAGEFILE,

};

class MainWindow : public BWindow {
public:
	MainWindow(float left, float top, float right, float bottom);
	void MessageReceived(BMessage *msg);
	bool QuitRequested();

private:
	
	BString get_type(type_code typecode);
	
	BMenuBar	*fTopMenuBar;
	
	BTextControl		*fMessageFileTextControl;
	BButton				*fChooseMessageFileButton;
	BButton				*fInspectMessageFileButton;
	MessageInfoView		*fMessageInfoView;

};

#endif
