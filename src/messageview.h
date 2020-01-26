#ifndef MESSAGEVIEW_H
#define MESSAGEVIEW_H

#include <ColumnListView.h>
#include <Message.h>
#include <String.h>


enum
{
	MV_ROW_CLICKED ='mv00',
};


class MessageView : public BColumnListView {
public:
	MessageView();
	void MessageReceived(BMessage *msg);
	void ShowMessageInfo(BMessage *message);

private:
	

};

#endif
