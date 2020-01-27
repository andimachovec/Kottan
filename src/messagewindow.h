#ifndef MESSAGEWINDOW_H
#define MESSAGEWINDOW_H


#include <Window.h>
#include <Message.h>
#include "messageview.h"

class MessageWindow : public BWindow {
public:
		MessageWindow(BRect frame, BMessage *DataMessage);

private:
	MessageView *fMessageView;
};

#endif
