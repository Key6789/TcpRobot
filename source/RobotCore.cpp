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

		QTimer* timer = new QTimer(this);
		timer->setInterval(1000);
		connect(timer, &QTimer::timeout, [=]() {
			slotShapesRotateShape(m_mapPreviewData, 30);
			slotShapesMoveShape(m_mapPreviewData, 1000);
			});
		timer->start();

		// 加载参数
		// m_mapPreviewData QMap<QString, ADDROBOTDATA>
	}
	void RobotCore::loadRobotShape(QString filePath)
	{
	}
	void RobotCore::slotShapesRotateShape(
		QMap<QString, ADDROBOTDATA>& robotMap,
		double angle,
		ShapeType shapeType,
		MoveDirection moveType)
	{
		foreach(ADDROBOTDATA addRobotData, robotMap.values())
		{
			if (addRobotData.shapeType == shapeType)
			{
				ShapesCreateTransformAngle(robotMap, addRobotData, angle, moveType);
				//ShapesCreateTransformDistance(robotMap, addRobotData, 1000, moveType);
			}
		}
	}

	void RobotCore::slotShapesMoveShape(
		QMap<QString, ADDROBOTDATA>& robotMap,
		double moveDistance,
		ShapeType shapeType,
		MoveDirection moveType)
	{
		foreach(ADDROBOTDATA addRobotData, robotMap.values())
		{
			if (addRobotData.shapeType == shapeType)
			{
				ShapesCreateTransformDistance(robotMap, addRobotData, moveDistance, moveType);
			}
		}
	}

	void RobotCore::ShapesLink()
	{
		// 读取文件 获取 连杆的各个参数

		// 按照索引顺序 连接 连杆的各个参数 送到 参数计算类

		// 将 计算结果按照 连杆的索引 保存到 模型的 连杆参数及保存


	}


	void RobotCore::ShapesTransformRecursively(
		QMap<QString, ADDROBOTDATA>& robotMap,
		QString name,
		const TRANSFORMDATA& data)
	{


		ADDROBOTDATA addRobot = robotMap.value(name);
		QVector<Handle(AIS_Shape)> newAisShapes;

		// 获取当前模型的装配点
		gp_Pnt assemblyPoint = addRobot.assemblyPoint;

		// 递归结束，统一删除旧模型并添加新模型
		for (auto& oldAisShape : addRobot.myAisShapes)
		{
			qDebug() << "remove old shape";
			getContext()->Remove(oldAisShape, Standard_True);
			TopoDS_Shape shape = oldAisShape->Shape();

			gp_Dir dir = data.rotationAxis;
			double newX = std::fabs(dir.X()); // 获取X分量的绝对值
			newX = std::round(newX * 1000) / 1000;

			double newY = std::fabs(dir.Y()); // 获取Y分量的绝对值
			newY = std::round(newY * 1000) / 1000;

			double newZ = std::fabs(dir.Z()); // 获取Z分量的绝对值
			newZ = std::round(newZ * 1000) / 1000;

			dir.SetX(newX); // 设置dir的X分量
			dir.SetY(newY);
			dir.SetZ(newZ);
			// 获取当前模型的Y轴向量

			gp_Trsf localRotationY1;
			//localRotationY1.SetRotation(gp_Ax1(data.assemblyPoint, data.rotationAxis), (data.rotationAngle) / 180.0 * M_PI); // Y轴旋转
			localRotationY1.SetRotation(gp_Ax1(data.assemblyPoint.XYZ() + data.assemblyPoint.XYZ(), data.rotationAxis), (data.rotationAngle) / 180.0 * M_PI); // Y轴旋转
			TopoDS_Shape rotatedshape = BRepBuilderAPI_Transform(shape, localRotationY1).Shape();

			// 旋转轴的累计量
			gp_Dir roteY = addRobot.yDirCurrent;
			gp_Dir roteYTemp = roteY.Transformed(localRotationY1);
			addRobot.yDirCurrent = roteYTemp;

			gp_Dir roteX = addRobot.xDirCurrent;
			gp_Dir roteXTemp = roteX.Transformed(localRotationY1);
			addRobot.xDirCurrent = roteXTemp;

			gp_Dir roteZ = addRobot.zDirCurrent;
			gp_Dir roteZTemp = roteZ.Transformed(localRotationY1);
			addRobot.zDirCurrent = roteZTemp;



			// 防位移跑偏
			gp_Trsf localRotationDD;
			localRotationDD.SetRotation(gp_Ax1(data.assemblyPoint.XYZ(), data.rotationAxis), (data.rotationAngle) / 180.0 * M_PI); // Y轴旋转
			addRobot.assemblyPoint = assemblyPoint.Transformed(localRotationDD); // 更新装配点

			// 角度偏移（现在是在局部坐标系中）

			// 平移部分
			gp_Trsf finalTransformTemp;
			finalTransformTemp.SetTranslation(gp_Vec(data.translation.XYZ()));
			TopoDS_Shape finalshape = BRepBuilderAPI_Transform(rotatedshape, finalTransformTemp).Shape();

			// 更新 平移坐标
			addRobot.assemblyPoint = addRobot.assemblyPoint.XYZ() + data.translation.XYZ(); // 更新装配点

			Handle(AIS_Shape) aisShapeTemp = new AIS_Shape(finalshape);
			aisShapeTemp->SetColor(addRobot.color);
			getContext()->Display(aisShapeTemp, Standard_True);
			newAisShapes.push_back(aisShapeTemp);
		}
		// edit 
		//addRobot.myAisShapes = newAisShapes;

		robotMap.insert(name, addRobot);
		// 递归处理下一个组件
		if (!addRobot.nextShapeNames.isEmpty())
		{
			foreach(QString nextShapeName, addRobot.nextShapeNames)
			{
				if (robotMap.contains(nextShapeName))
				{
					TRANSFORMDATA dataSec = data;
					dataSec.angleX = 0;
					dataSec.angleY = 0;
					dataSec.angleZ = 0;

					ShapesTransformRecursively(robotMap, nextShapeName, data);
				}
			}
		}
		robotMap[name].myAisShapes = newAisShapes;
	}
	void RobotCore::ShapesTransform(QMap<QString, ADDROBOTDATA>& robotMap, TRANSFORMDATA data)
	{
		ADDROBOTDATA addRobot = robotMap.value(data.name);
		// 实现动画效果
		double x = data.translation.X();
		double y = data.translation.Y();
		double z = data.translation.Z();

		double angleX = data.angleX;
		double angleY = data.angleY;
		double angleZ = data.angleZ;

		if (x != 0)
		{

			ShapesTransformRecursively(robotMap, data.name, data);
		}
		if (y != 0)
		{
			ShapesTransformRecursively(robotMap, data.name, data);
		}
		if (z != 0)
		{
			ShapesTransformRecursively(robotMap, data.name, data);
		}


		if (angleX != 0)
		{
			data.rotationAngle = angleX;
			gp_Dir roteX = addRobot.xDirCurrent;
			gp_Trsf trsf;
			trsf.SetRotation(gp_Ax1((addRobot.assemblyPoint.XYZ() + addRobot.assemblyPoint.XYZ()), data.rotationAxis), data.angleX / 180.0 * M_PI); // X轴旋转
			gp_Dir roteXTemp = roteX.Transformed(trsf);
			data.rotationAxis = roteXTemp;
			robotMap.insert(data.name, addRobot);
			ShapesTransformRecursively(robotMap, data.name, data);
		}
		if (angleY != 0)
		{
			data.rotationAngle = angleY;

			gp_Dir roteY = addRobot.yDirCurrent;

			data.rotationAxis = roteY;
			robotMap.insert(data.name, addRobot);
			ShapesTransformRecursively(robotMap, data.name, data);
		}
		if (angleZ != 0)
		{
			data.rotationAngle = angleZ;
			gp_Dir roteZ = addRobot.zDirCurrent;
			gp_Trsf trsf;
			trsf.SetRotation(gp_Ax1((addRobot.assemblyPoint.XYZ() + addRobot.assemblyPoint.XYZ()), data.rotationAxis), data.angleZ / 180.0 * M_PI); // Z轴旋转
			gp_Dir roteZTemp = roteZ.Transformed(trsf);
			data.rotationAxis = roteZTemp;
			robotMap.insert(data.name, addRobot);
			ShapesTransformRecursively(robotMap, data.name, data);
		}
	}
	void RobotCore::ShapesCreateTransformAngle(
		QMap<QString, ADDROBOTDATA>& robotMap, 
		ADDROBOTDATA originData, 
		double angle, 
		MoveDirection moveType
)
	{
		TRANSFORMDATA transformData;
		transformData.name = originData.name;
		transformData.assemblyPoint = originData.assemblyPoint;
		transformData.rotationAngle = angle;

		switch (moveType)
		{
		case MoveDirection::MoveDirection_XAxis:
		{
			transformData.rotationAxis = originData.xDirCurrent;
			transformData.angleX = angle;
			break;
		}
		case MoveDirection::MoveDirection_YAxis:
		{
			transformData.rotationAxis = originData.yDirCurrent;
			transformData.angleY = angle;
			break;
		}
		case MoveDirection::MoveDirection_ZAxis:
		{
			transformData.angleZ = angle;
			transformData.rotationAxis = originData.zDirCurrent;
			break;
		}
		default:
			break;
		}
		ShapesTransform(robotMap, transformData);
	}
	void RobotCore::ShapesCreateTransformDistance(QMap<QString, ADDROBOTDATA>& robotMap, ADDROBOTDATA originData, double moveDistance, MoveDirection moveType)
	{
		TRANSFORMDATA transformData;
		transformData.name = originData.name;
		switch (moveType)
		{
		case MoveDirection::MoveDirection_XAxis:
		{
			transformData.rotationAxis = originData.xDirCurrent;
			transformData.translation = gp_Pnt(moveDistance, 0, 0).XYZ();
			break;
		}
		case MoveDirection::MoveDirection_YAxis:
		{
			transformData.rotationAxis = originData.yDirCurrent;
			transformData.translation = gp_Pnt(0, moveDistance, 0).XYZ();
			break;
		}
		case MoveDirection::MoveDirection_ZAxis:
		{
			transformData.rotationAxis = originData.zDirCurrent;
			transformData.translation = gp_Pnt(0, 0, moveDistance).XYZ();
			break;
		}
		default:
			break;
		}
		ShapesTransform(robotMap, transformData);
	}
	QVariantMap RobotCore::readJsonFileToMap(QString filePath)
	{
		QFile file(filePath);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			qDebug() << "open file failed";
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
			return QVariantMap();
		}
		QVariantMap jsonMap = jsonDoc.object().toVariantMap();
		return jsonMap;
	}
} // namespace TCP_ROBOT