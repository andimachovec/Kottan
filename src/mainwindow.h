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
#include <ColumnListView.h>
#include <FilePanel.h>

enum
{
	MW_MENU_ABOUT ='mw00',
	MW_BUTTON_CHOOSEMESSAGEFILE,
	MW_REF_MESSAGEFILE,
	MW_MSGINFO_CLICKED,
	MW_INSPECTMESSAGEFILE
};

class MainWindow : public BWindow {
public:
	MainWindow(float left, float top, float right, float bottom);
	~MainWindow();
	void MessageReceived(BMessage *msg);
	bool QuitRequested();

private:


	void inspect_message_file();
	void show_message_data();
	
	BString get_type(type_code typecode);
	BString bool2bstring(bool value);
	
	
	
	BMenuBar	*fTopMenuBar;
	
	BTextControl		*fMessageFileTextControl;
	BButton				*fChooseMessageFileButton;
	BColumnListView		*fMessageInfoView;
	BMessage			*fCurrentMessage;
	BFilePanel			*fOpenFilePanel;
	
};

#endif
