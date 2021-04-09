/*
 * Copyright 2019-2021 Andi Machovec <andi.machovec@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */

#include <String.h>

static BString 
get_type(type_code typecode)
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
