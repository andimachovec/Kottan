#ifndef MESSAGEENGINE_H
#define MESSAGEENGINE_H


#include <File.h>
#include <Handler.h>
#include <Message.h>



class MessageFile : public BFile, public BHandler {
public:
	MessageFile();
	~MessageFile();
	status_t LoadMessage();
	status_t SaveMessage();


private:
	void		start_monitoring();
	void		stop_monitoring();

	BMessage	*fMessage;
	bool		fMessageLoaded;

};

#endif