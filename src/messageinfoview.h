#ifndef MESSAGEINFOVIEW_H
#define MESSAGEINFOVIEW_H

#include <ColumnListView.h>

class MessageInfoView : public BColumnListView {
public:
	MessageInfoView(const char *name);
	virtual void MouseDown(BPoint point);
	
private:

};

#endif
