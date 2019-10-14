#include "mainwindow.h"

#include <Alert.h>
#include <LayoutBuilder.h>
#include <Catalog.h>
#include <Application.h>
#include <FilePanel.h>
#include <ColumnTypes.h>

#include <iostream>
#include <sstream>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "MainWindow"


MainWindow::MainWindow(float left, float top, float right, float bottom)
	: 
	BWindow(BRect(left,top,right,bottom), "Kottan", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{

	//initialize GUI objects
	fTopMenuBar = new BMenuBar("topmenubar");
	fMessageFileTextControl = new BTextControl(B_TRANSLATE("Message File"), "", NULL);
	fChooseMessageFileButton = new BButton(B_TRANSLATE("Choose Message File"),
											new BMessage(MW_BUTTON_CHOOSEMESSAGEFILE));
											
	fInspectMessageFileButton = new BButton(B_TRANSLATE("Inspect Message File"),
											new BMessage(MW_BUTTON_INSPECTMESSAGEFILE));
											
	fMessageInfoView = new MessageInfoView("messageinfo");
	fMessageInfoView->SetInvocationMessage(new BMessage(MW_MSGINFO_CLICKED));
	BIntegerColumn *index_column = new BIntegerColumn(B_TRANSLATE("Index"),70,10,100);
	BStringColumn *name_column = new BStringColumn(B_TRANSLATE("Name"),200,50,1000,0);
	BStringColumn *type_column = new BStringColumn(B_TRANSLATE("Type"),200,50,1000,0);
	BIntegerColumn *count_column = new BIntegerColumn(B_TRANSLATE("Number of Items"),120,10,150);
	
	fMessageInfoView->AddColumn(index_column,0);
	fMessageInfoView->AddColumn(name_column,1);
	fMessageInfoView->AddColumn(type_column,2);
	fMessageInfoView->AddColumn(count_column,3);
	
	
	//define menu layout
	BLayoutBuilder::Menu<>(fTopMenuBar)
		.AddMenu(B_TRANSLATE("File"))
			.AddItem(B_TRANSLATE("Quit"), B_QUIT_REQUESTED, 'Q')
		.End()
		.AddMenu(B_TRANSLATE("Help"))
			.AddItem(B_TRANSLATE("About"), MW_MENU_ABOUT)
		.End()
	.End();

	//define main layout
	BLayoutBuilder::Group<>(this, B_VERTICAL,0)
		.SetInsets(0)
		.Add(fTopMenuBar)
		.AddGroup(B_HORIZONTAL)
			.SetInsets(5,5,5,5)
			.Add(fMessageFileTextControl)
			.Add(fChooseMessageFileButton)
		.End()
		.Add(fInspectMessageFileButton)
		.AddGroup(B_HORIZONTAL)
			.SetInsets(5,3,3,3)
			.Add(fMessageInfoView,20)
		//.AddGlue()
	.Layout();


	fCurrentMessage = new BMessage();


}


MainWindow::~MainWindow()
{

	delete fCurrentMessage;

}


void
MainWindow::MessageReceived(BMessage *msg)
{

	if(msg->WasDropped()) {
		msg->what = MW_REF_MESSAGEFILE;
	}

	switch(msg->what)
	{

		case MW_MENU_ABOUT:
		{
			be_app->PostMessage(B_ABOUT_REQUESTED);
			break;
		}

		
		case MW_BUTTON_CHOOSEMESSAGEFILE:
		{
		
			BFilePanel *messagefile_filepanel = new BFilePanel(B_OPEN_PANEL, 
																new BMessenger(this), 
																NULL,
																B_FILE_NODE,
																false,
																new BMessage(MW_REF_MESSAGEFILE));
			messagefile_filepanel->Show();
			break;
		}

		
		case MW_BUTTON_INSPECTMESSAGEFILE:
		{

			fMessageInfoView->Clear();
			
			
			BString messagefile_name(fMessageFileTextControl->Text());
			messagefile_name.Trim();
			
			if (messagefile_name != "")
			{
				
				BFile *message_file = new BFile();
					
				status_t fileopen_result = message_file->SetTo(messagefile_name.String(), B_READ_ONLY);
					
				if (fileopen_result == B_OK)
				{
				
					status_t unflatten_result = fCurrentMessage->Unflatten(message_file);
					
					if (unflatten_result == B_OK)
					{
				
						char *name;
						type_code type;
						int32 count;
						
						for (int32 i=0; fCurrentMessage->GetInfo(B_ANY_TYPE, i, &name, &type, &count) == B_OK; ++i)
						{
		
							BRow *row = new BRow();
							
							BIntegerField *index_field = new BIntegerField(i);
							BStringField *name_field = new BStringField(name);
							BStringField *type_field = new BStringField(get_type(type).String());
							BIntegerField *count_field = new BIntegerField(count);
							
							row->SetField(index_field,0);
							row->SetField(name_field,1);
							row->SetField(type_field,2);
							row->SetField(count_field,3);
							
							fMessageInfoView->AddRow(row);
					
						}
						
					}
					else
					{	
						BAlert *errorunflatten_alert = new BAlert("Kottan",
													B_TRANSLATE("Error reading the message from the file!"), 
													"OK");
						errorunflatten_alert->Go();
				
					}
				
				}
				else 
				{
					BAlert *erroropen_alert = new BAlert("Kottan",
													B_TRANSLATE("Error opening the message file!"), 
													"OK");
					erroropen_alert->Go();
				}


				delete message_file;
				
			}
			else
			{
				BAlert *nomessagefile_alert = new BAlert("Kottan",
													B_TRANSLATE("Please specify a file to analyze!"), 
													"OK");
			
				nomessagefile_alert->Go();
			
			}
			
			break;
		}
		
		
		case MW_REF_MESSAGEFILE:
		{
			
			entry_ref ref;
			msg->FindRef("refs", &ref);
			BEntry target_dir(&ref, true);
			BPath target_path(&target_dir);
			fMessageFileTextControl->SetText(target_path.Path());
			
			Activate(true);
			break;
		}


		case MW_MSGINFO_CLICKED:
		{
		
			
			BIntegerField* index_field = (BIntegerField*)fMessageInfoView->CurrentSelection()->GetField(0);
			int32 msg_index=index_field->Value();
			
			char *name;
			int32 items_count;
			type_code type;
			
			fCurrentMessage->GetInfo(B_ANY_TYPE, msg_index, &name, &type, &items_count);
			
			std::cout << "Message data for index " << msg_index << " requested..." << std::endl;
			std::cout << "Data type: " << get_type(type) << std::endl;
			
			
			BString message_data;
			
			
			for (int32 i=0; i < items_count; ++i)
			{
			
				message_data="";
				switch (type)
				{
				
					case B_STRING_TYPE:
						message_data=BString(fCurrentMessage->GetString(name, i, ""));
						break;
						
					case B_INT8_TYPE:
					{
						message_data<<fCurrentMessage->GetInt8(name,i,0);
						break;
					}

					case B_INT16_TYPE:
					{
						message_data<<fCurrentMessage->GetInt16(name,i,0);
						break;
					}

					case B_INT32_TYPE:
					{
						message_data<<fCurrentMessage->GetInt32(name,i,0);
						break;
					}

					case B_INT64_TYPE:
					{
						message_data<<fCurrentMessage->GetInt64(name,i,0);
						break;
					}

					case B_UINT8_TYPE:
					{
						message_data<<fCurrentMessage->GetUInt8(name,i,0);
						break;
					}

					case B_UINT16_TYPE:
					{
						message_data<<fCurrentMessage->GetUInt16(name,i,0);
						break;
					}

					case B_UINT32_TYPE:
					{
						message_data<<fCurrentMessage->GetUInt32(name,i,0);
						break;
					}

					case B_UINT64_TYPE:
					{
						message_data<<fCurrentMessage->GetUInt64(name,i,0);
						break;
					}
	
					case B_BOOL_TYPE:
						message_data=bool2bstring(fCurrentMessage->GetBool(name, i, false));
						break;
						
					default:
						message_data="data not printable";
						break;
				}
				
			
				std::cout << "	" << message_data << std::endl;
			
			}
			
			std::cout << std::endl;
		
			
			break;
		}
		
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}

	}
}


bool
MainWindow::QuitRequested()
{

	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;

}


BString 
MainWindow::get_type(type_code typecode)
{

	BString typecode_name;

	switch(typecode)
	{
	
		case B_AFFINE_TRANSFORM_TYPE:
		{
			typecode_name="B_AFFINE_TRANSFORM_TYPE";
			break;
		}

		case B_ALIGNMENT_TYPE:
		{
			typecode_name="B_ALIGNMENT_TYPE";
			break;
		}

		case B_ANY_TYPE:
		{
			typecode_name="B_ANY_TYPE";
			break;
		}

		case B_ATOM_TYPE:
		{
			typecode_name="B_ATOM_TYPE";
			break;
		}

		case B_ATOMREF_TYPE:
		{
			typecode_name="B_ATOMREF_TYPE";
			break;
		}

		case B_BOOL_TYPE:
		{
			typecode_name="B_BOOL_TYPE";
			break;
		}

		case B_CHAR_TYPE:
		{
			typecode_name="B_CHAR_TYPE";
			break;
		}

		case B_COLOR_8_BIT_TYPE:
		{
			typecode_name="B_COLOR_8_BIT_TYPE";
			break;
		}

		case B_DOUBLE_TYPE:
		{
			typecode_name="B_DOUBLE_TYPE";
			break;
		}

		case B_FLOAT_TYPE:
		{
			typecode_name="B_FLOAT_TYPE";
			break;
		}

		case B_GRAYSCALE_8_BIT_TYPE:
		{
			typecode_name="B_GRAYSCALE_8_BIT_TYPE";
			break;
		}

		case B_INT16_TYPE:
		{
			typecode_name="B_INT16_TYPE";
			break;
		}

		case B_INT32_TYPE:
		{
			typecode_name="B_INT32_TYPE";
			break;
		}

		case B_INT64_TYPE:
		{
			typecode_name="B_INT64_TYPE";
			break;
		}

		case B_INT8_TYPE:
		{
			typecode_name="B_INT8_TYPE";
			break;
		}

		case B_LARGE_ICON_TYPE:
		{
			typecode_name="B_LARGE_ICON_TYPE";
			break;
		}

		case B_MEDIA_PARAMETER_GROUP_TYPE:
		{
			typecode_name="B_MEDIA_PARAMETER_GROUP_TYPE";
			break;
		}

		case B_MEDIA_PARAMETER_TYPE:
		{
			typecode_name="B_MEDIA_PARAMETER_TYPE";
			break;
		}

		case B_MEDIA_PARAMETER_WEB_TYPE:
		{
			typecode_name="B_MEDIA_PARAMETER_WEB_TYPE";
			break;
		}

		case B_MESSAGE_TYPE:
		{
			typecode_name="B_MESSAGE_TYPE";
			break;
		}

		case B_MESSENGER_TYPE:
		{
			typecode_name="B_MESSENGER_TYPE";
			break;
		}

		case B_MIME_TYPE:
		{
			typecode_name="B_MIME_TYPE";
			break;
		}

		case B_MINI_ICON_TYPE:
		{
			typecode_name="B_MINI_ICON_TYPE";
			break;
		}

		case B_MONOCHROME_1_BIT_TYPE:
		{
			typecode_name="B_MONOCHROME_1_BIT_TYPE";
			break;
		}

		case B_OBJECT_TYPE:
		{
			typecode_name="B_OBJECT_TYPE";
			break;
		}

		case B_OFF_T_TYPE:
		{
			typecode_name="B_OFF_T_TYPE";
			break;
		}

		case B_PATTERN_TYPE:
		{
			typecode_name="B_PATTERN_TYPE";
			break;
		}

		case B_POINTER_TYPE:
		{
			typecode_name="B_POINTER_TYPE";
			break;
		}

		case B_POINT_TYPE:
		{
			typecode_name="B_POINT_TYPE";
			break;
		}

		case B_PROPERTY_INFO_TYPE:
		{
			typecode_name="B_PROPERTY_INFO_TYPE";
			break;
		}

		case B_RAW_TYPE:
		{
			typecode_name="B_RAW_TYPE";
			break;
		}

		case B_RECT_TYPE:
		{
			typecode_name="B_RECT_TYPE";
			break;
		}

		case B_REF_TYPE:
		{
			typecode_name="B_REF_TYPE";
			break;
		}

		case B_RGB_32_BIT_TYPE:
		{
			typecode_name="B_RGB_32_BIT_TYPE";
			break;
		}

		case B_RGB_COLOR_TYPE:
		{
			typecode_name="B_RGB_COLOR_TYPE";
			break;
		}

		case B_SIZE_TYPE:
		{
			typecode_name="B_SIZE_TYPE";
			break;
		}

		case B_SIZE_T_TYPE:
		{
			typecode_name="B_SIZE_T_TYPE";
			break;
		}

		case B_SSIZE_T_TYPE:
		{
			typecode_name="B_SSIZE_T_TYPE";
			break;
		}

		case B_STRING_TYPE:
		{
			typecode_name="B_STRING_TYPE";
			break;
		}

		case B_STRING_LIST_TYPE:
		{
			typecode_name="B_STRING_LIST_TYPE";
			break;
		}

		case B_TIME_TYPE:
		{
			typecode_name="B_TIME_TYPE";
			break;
		}

		case B_UINT16_TYPE:
		{
			typecode_name="B_UINT16_TYPE";
			break;
		}

		case B_UINT32_TYPE:
		{
			typecode_name="B_UINT32_TYPE";
			break;
		}

		case B_UINT64_TYPE:
		{
			typecode_name="B_UINT64_TYPE";
			break;
		}

		case B_UINT8_TYPE:
		{
			typecode_name="B_UINT8_TYPE";
			break;
		}

		case B_VECTOR_ICON_TYPE:
		{
			typecode_name="B_VECTOR_ICON_TYPE";
			break;
		}

		case B_XATTR_TYPE:
		{
			typecode_name="B_XATTR_TYPE";
			break;
		}

		case B_NETWORK_ADDRESS_TYPE:
		{
			typecode_name="B_NETWORK_ADDRESS_TYPE";
			break;
		}

		case B_MIME_STRING_TYPE:
		{
			typecode_name="B_MIME_STRING_TYPE";
			break;
		}

		case B_ASCII_TYPE:
		{
			typecode_name="B_ASCII_TYPE";
			break;
		}

	
		default:
		{
			typecode_name = "unidentified"; 
		
		}
	
	}

	return typecode_name;

}


BString 
MainWindow::bool2bstring(bool value)
{

	if (value)
	{
		return BString("true");
	}
	else
	{
		return BString("false");
	}


}
