#include "messagefile.h"

#include <NodeMonitor.h>


MessageFile::MessageFile(BLooper *target)
	:
	BFile(),
	BHandler(),
	fTarget(target)
{

	fMessage = new BMessage();
	fMessageLoaded = false;
	fMessageList = new BObjectList<IndexMessage>(20, false);

}


MessageFile::~MessageFile()
{

	delete fMessage;
	delete fMessageList;
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


BMessage*
MessageFile::GetMessage()
{

	return fMessage;

}


BMessage*
MessageFile::GetSubMessage(BMessage *path_message)
{

	// follow the index path to the selected data
	BMessage *current_message = fMessage;

	fMessageList->MakeEmpty();

	int32 path_items_count;
	path_message->GetInfo("selection_path", NULL, &path_items_count);

	type_code current_type;
	char *current_name;
	int32 current_item_count;

	for (int32 path_index = path_items_count-1;  // loop through the index values in
		 path_index >= 0;				   		 // in reverse order
		 --path_index)
	{
		int32 current_index;
		path_message->FindInt32("selection_path", path_index, &current_index);

		current_message->GetInfo(B_ANY_TYPE,
								current_index,
								&current_name,
								&current_type,
								&current_item_count);

		if (current_type == B_MESSAGE_TYPE)
		{
			int32 member_index=0;

			if (current_item_count > 1)
			{
				--path_index;
				path_message->FindInt32("selection_path", path_index, &member_index);
			}

			BMessage *temp_message = new BMessage();
			current_message->FindMessage(current_name, member_index, temp_message);

			IndexMessage *index_message = new IndexMessage();
			index_message->message = new BMessage(*temp_message);
			index_message->field_index = member_index;
			index_message->field_name = current_name;
			fMessageList->AddItem(index_message, 0);

			current_message = temp_message;
		}
	}

	fSelectedName = current_name;
	fSelectedType = current_type;
	fSelectedItemCount = current_item_count;

	BMessage *sub_message = nullptr;
	if (fMessageList->CountItems() > 0)
	{
		sub_message = fMessageList->FirstItem()->message;
	}
	else
	{
		sub_message = fMessage;
	}

	return sub_message;

}


void
MessageFile::MessageReceived(BMessage *msg)
{

	switch(msg->what)
	{
		case B_NODE_MONITOR:
			
			int32 stat_changed_flags;
			msg->FindInt32("fields", &stat_changed_flags);

			// only compare messages when the file was modified
			if ((stat_changed_flags & B_STAT_MODIFICATION_TIME) != 0)
			{
				BMessage *temp_msg = new BMessage();
				Seek(0, SEEK_SET);
				temp_msg->Unflatten(this);

				//only request reload if the data in the message has actually changed
				if (!(temp_msg->HasSameData(*fMessage, false, true)))
				{
					fTarget->PostMessage(MF_RELOADED);
				}

				delete temp_msg;
			}	
			
			break;

		default:
			BHandler::MessageReceived(msg);
			break;
	}

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


