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
	void RobotCore::loadWorkShapes(QString filePath)
	{
		removeAllPreview();
		// 读取文件内容 json 格式
		QFile file(filePath);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			qDebug() << "open file failed";
			return;
		}
		QTextStream in(&file);
		QString jsonStr = in.readAll();
		file.close();

		// 解析 json 格式
		QJsonParseError jsonError;
		QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr.toUtf8(), &jsonError);
		if (jsonError.error != QJsonParseError::NoError)
		{
			qDebug() << "parse json failed";
			return;
		}
		QVariantMap jsonMap = jsonDoc.object().toVariantMap();
		foreach(QString key, jsonMap.keys())
		{
			QVariantMap shapeMap = jsonMap[key].toMap();
			SHAPESTRUCT shapeStruct;
			shapeStruct.setShapeVariantMap(shapeMap);
			// 加载模型
			slotReplaceModelByPath(shapeStruct);
		}
		// 加载参数

	}
	void RobotCore::loadRobotShape(QString filePath)
	{
	}
} // namespace TCP_ROBOT