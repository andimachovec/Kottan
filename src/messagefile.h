#ifndef MESSAGEENGINE_H
#define MESSAGEENGINE_H


#include <File.h>
#include <Handler.h>
#include <Message.h>
#include <ObjectList.h>
#include <Looper.h>


class IndexMessage {
public:
	BMessage 	*message;
	int32		field_index;
	const char  *field_name;
};


class MessageFile : public BFile, public BHandler {
public:
	MessageFile(BLooper *target);
	~MessageFile();
	status_t LoadMessage();
	status_t SaveMessage();
	BMessage* GetMessage();
	BMessage* GetSubMessage(BMessage *path_message);
	void MessageReceived(BMessage *msg);


private:
	void		start_monitoring();
	void		stop_monitoring();

	BMessage					*fMessage;
	bool						fMessageLoaded;
	BObjectList<IndexMessage>	*fMessageList;
	const char 					*fSelectedName;
	type_code 					fSelectedType;
	int32 						fSelectedItemCount;
	BLooper						fTarget;
};

#endif