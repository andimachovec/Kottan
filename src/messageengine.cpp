#include "messageengine.h"

#include <NodeMonitor.h>


MessageEngine::MessageEngine()
{

	fMessage = new BMessage();
	fMessageFile = new BFile();
	fMessageLoaded = false;
	fFileSet = false;

}


MessageEngine::~MessageEngine()
{

	delete fMessage;
	delete fMessageFile;

}


status_t
MessageEngine::LoadFromFile()
{

	if (!fFileSet)
	{
		return B_ERROR;
	}

	status_t result = fMessage->Unflatten(fMessageFile);
	if (result == B_OK)
	{
		fMessageLoaded = true;
		start_file_monitoring();
	}
	else
	{
		fMessageLoaded = false;
	}

	return result;

}


status_t
MessageEngine::SaveToFile()
{

	fMessageFile->Seek(0, SEEK_SET);
	status_t result = fMessage->Flatten(fMessageFile);

	return result;
}


status_t
MessageEngine::SetFile(entry_ref ref)
{

	status_t result = fMessageFile->SetTo(&ref, B_READ_WRITE);
	if (result == B_OK)
	{
		fFileSet = true;
		fMessageFileRef = ref;
	}
	else
	{
		fFileSet = false;
	}

	return result;
}


void
MessageEngine::start_file_monitoring()
{

	/*
	BEntry entry(&ref);
	node_ref nref;
	entry.GetNodeRef(&nref);
	watch_node(&nref, B_WATCH_STAT|B_WATCH_INTERIM_STAT, this);
	*/
}


void
MessageEngine::stop_file_monitoring()
{

	//stop_watching(this);

}


