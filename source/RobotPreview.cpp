#include "../include/RobotPreview.h"

namespace TCP_ROBOT
{
	RobotPreview::RobotPreview(QWidget* parent)
	{

	}
	RobotPreview::~RobotPreview()
	{

	}
	
	void RobotPreview::slotUpdataRobotShaps(void)
	{
		displayCoordinateAxes();
		myView->FitAll();
	}
}