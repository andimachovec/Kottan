#include "messageinfoview.h"


#include <iostream>


MessageInfoView::MessageInfoView(const char *name)
	:
	BColumnListView(name,0)
{
	SetMouseTrackingEnabled(true);

}


void 
MessageInfoView::MouseDown(BPoint point)
{

	std::cout << "Mouse Event registered" << std::endl;




}
