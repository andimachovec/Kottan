#ifndef MESSAGEENGINE_H
#define MESSAGEENGINE_H


#include <Message.h>
#include <File.h>


class MessageEngine {
public:
	MessageEngine();
	~MessageEngine();
	status_t LoadFromFile();
	status_t SaveToFile();
	status_t SetFile(entry_ref fileref);


private:
	void		start_file_monitoring();
	void		stop_file_monitoring();

	BMessage	*fMessage;
	BFile		*fMessageFile;
	entry_ref	fMessageFileRef;
	bool		fMessageLoaded;
	bool		fFileSet;

};

#endif