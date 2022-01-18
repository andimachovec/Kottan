#include "messagefile.h"

#include <NodeMonitor.h>


MessageFile::MessageFile()
	:
	BFile(),
	BHandler()
{

	fMessage = new BMessage();
	fMessageLoaded = false;

}


MessageFile::~MessageFile()
{

	delete fMessage;

}


status_t
MessageFile::LoadMessage()
{

	status_t result = fMessage->Unflatten(this);
	if (result == B_OK)
	{
		fMessageLoaded = true;
		start_monitoring();
	}
	else
	{
		fMessageLoaded = false;
	}

	return result;

}


status_t
MessageFile::SaveMessage()
{

	Seek(0, SEEK_SET);
	status_t result = fMessage->Flatten(this);

	return result;
}


void
MessageFile::start_monitoring()
{

	node_ref nref;
	GetNodeRef(&nref);
	watch_node(&nref, B_WATCH_STAT|B_WATCH_INTERIM_STAT, this);

}


void
MessageFile::stop_monitoring()
{

	stop_watching(this);

}


