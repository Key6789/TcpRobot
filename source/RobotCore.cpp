#include "../include/RobotCore.h"

namespace TCP_ROBOT
{
	RobotCore::RobotCore(QWidget* parent)
	{
	}
	RobotCore::~RobotCore()
	{
	}
	void RobotCore::slotUpdataRobotShaps(void)
	{

	}
	void RobotCore::loadShapes(QString filePath)
	{
		// 读取文件内容
		QFile file(filePath);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			qDebug() << "open file failed";
			return;
		}
		QTextStream in(&file);
		QString content = in.readAll();
		file.close();
	}
} // namespace TCP_ROBOT