#include "../include/RobotCore.h"

namespace TCP_ROBOT
{
	RobotCore::RobotCore(QWidget* parent)
	{
	}
	RobotCore::~RobotCore()
	{
	}
	void RobotCore::slotShapMove(QString shapeValue)
	{
		qDebug() << "slotShapMove:" << shapeValue;
		QStringList shapeValues = shapeValue.split(",");
		if (shapeValues.size() < 6)
		{
			return;
		}
		// 取前6个参数
		QString robot1 = shapeValues.at(0);
		QString robot2 = shapeValues.at(1);
		QString robot3 = shapeValues.at(2);
		QString robot4 = shapeValues.at(3);
		QString robot5 = shapeValues.at(4);
		QString robot6 = shapeValues.at(5);

		// 取出导轨移动
		QString shortMove = shapeValues.at(6);

		// 取出旋转角度
		QString TableAngle = shapeValues.at(7);


		// 机器人旋转
		if (!robot1.isEmpty())
		{
			slotRobotRotateShape(m_robotMap, robot1.toDouble(), ShapeType::ShapeType_Robot, MoveDirection::MoveDirection_ZAxis, 1);
		}
		if (!robot2.isEmpty())
		{
			slotRobotRotateShape(m_robotMap, robot2.toDouble(), ShapeType::ShapeType_Robot, MoveDirection::MoveDirection_ZAxis, 2);
		}
		if (!robot3.isEmpty())
		{
			slotRobotRotateShape(m_robotMap, robot3.toDouble(), ShapeType::ShapeType_Robot, MoveDirection::MoveDirection_ZAxis, 3);
		}
		if (!robot4.isEmpty())
		{
			slotRobotRotateShape(m_robotMap, robot4.toDouble(), ShapeType::ShapeType_Robot, MoveDirection::MoveDirection_ZAxis, 4);
		}
		if (!robot5.isEmpty())
		{
			slotRobotRotateShape(m_robotMap, robot5.toDouble(), ShapeType::ShapeType_Robot, MoveDirection::MoveDirection_ZAxis, 5);
		}
		if (!robot6.isEmpty())
		{
			slotRobotRotateShape(m_robotMap, robot6.toDouble(), ShapeType::ShapeType_Robot, MoveDirection::MoveDirection_ZAxis, 6);
		}

		if (!shortMove.isEmpty())
		{
			slotShapesMoveShape(m_otherMap, shortMove.toDouble(), ShapeType::ShapeType_ShortGuide, MoveDirection_XAxis);
			slotShapesMoveShape(m_shapesMap, shortMove.toDouble(), ShapeType::ShapeType_Work, MoveDirection_XAxis);
		}
		if (!TableAngle.isEmpty())
		{
			slotShapesRotateShape(m_otherMap, TableAngle.toDouble(), ShapeType::ShapeType_RotatingTable);
			slotShapesRotateShape(m_shapesMap, TableAngle.toDouble(), ShapeType::ShapeType_Work);
		}

	}
	void RobotCore::slotUpdataRobotShaps(void)
	{

	}
	void RobotCore::loadWorkShapes(QString filePath)
	{
		for (QString shapeName : m_shapesMap.keys())
		{
			removeShapeModel(m_shapesMap, m_shapesMap[shapeName]);
		}

		m_shapesMap.clear();

		// 读取文件内容 json 格式
		QVariantMap jsonMap = readJsonFileToMap(filePath);
		foreach(QString key, jsonMap.keys())
		{
			QVariantMap shapeMap = jsonMap[key].toMap();
			SHAPESTRUCT shapeStruct;
			shapeStruct.setShapeVariantMap(shapeMap);
			// 加载模型
			loadShapesModel(m_shapesMap, shapeStruct);
		}
		// 加载参数
		// m_mapPreviewData QMap<QString, ADDROBOTDATA>
	}
	void RobotCore::loadRobotShape(QString filePath)
	{
		for (QString shapeName : m_robotMap.keys())
		{
			removeShapeModel(m_robotMap, m_robotMap[shapeName]);
		}
		m_robotMap.clear();

		QVariantMap jsonMap = readJsonFileToMap(filePath);
		foreach(QString key, jsonMap.keys())
		{
			QVariantMap shapeMap = jsonMap[key].toMap();
			SHAPESTRUCT shapeStruct;
			shapeStruct.setShapeVariantMap(shapeMap);
			// 加载模型
			loadShapesModel(m_robotMap, shapeStruct);
		}
	}
	void RobotCore::loadOtherShape(QString filePath)
	{
		for (QString shapeName : m_otherMap.keys())
		{
			removeShapeModel(m_otherMap, m_otherMap[shapeName]);
		}
		m_otherMap.clear();

		QVariantMap jsonMap = readJsonFileToMap(filePath);
		foreach(QString key, jsonMap.keys())
		{
			QVariantMap shapeMap = jsonMap[key].toMap();
			SHAPESTRUCT shapeStruct;
			shapeStruct.setShapeVariantMap(shapeMap);
			// 加载模型
			loadShapesModel(m_otherMap, shapeStruct);
		}
	}
	void RobotCore::slotShapesRotateShape(
		QMap<QString, ADDROBOTDATA>& robotMap,
		double angle,
		ShapeType shapeType,
		MoveDirection moveType)
	{
		QElapsedTimer timer;
		timer.start();
		foreach(ADDROBOTDATA addRobotData, robotMap.values())
		{
			if (addRobotData.shapeType == shapeType)
			{
				ShapesCreateTransformAngle(robotMap, addRobotData, angle, moveType);
				//ShapesCreateTransformDistance(robotMap, addRobotData, 1000, moveType);
			}
		}
		qDebug() << "slotShapesRotateShape time:" << timer.elapsed();
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

	void RobotCore::slotRobotRotateShape(QMap<QString, ADDROBOTDATA>& robotMap, double angle, ShapeType shapeType, MoveDirection moveType, int index)
	{
		foreach(ADDROBOTDATA addRobotData, robotMap.values())
		{
			if (addRobotData.shapeType == shapeType && addRobotData.ShapeLinkIndex != -1)
			{
				if (addRobotData.ShapeLinkIndex == index)
				{
					ShapesCreateTransformAngle(robotMap, addRobotData, angle, moveType);
					break;
				}
			}
		}
	}

	void RobotCore::slotRobotMoveShape(QMap<QString, ADDROBOTDATA>& robotMap, double moveDistance, ShapeType shapeType, MoveDirection moveType, int index)
	{
		foreach(ADDROBOTDATA addRobotData, robotMap.values())
		{
			if (addRobotData.shapeType == shapeType && addRobotData.ShapeLinkIndex != -1)
			{
				if (addRobotData.ShapeLinkIndex == index)
				{
					ShapesCreateTransformDistance(robotMap, addRobotData, moveDistance, moveType);
					break;
				}
			}
		}
	}

	void RobotCore::ShapesLink()
	{
		// 读取文件 获取 连杆的各个参数

		// 按照索引顺序 连接 连杆的各个参数 送到 参数计算类

		// 将 计算结果按照 连杆的索引 保存到 模型的 连杆参数及保存


	}
	// 辅助函数：打印 gp_Trsf 的详细信息


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
			// 获取当前模型的局部变换

			TopoDS_Shape tempShape = oldAisShape->Shape();
			//getContext()->Remove(oldAisShape, Standard_True);
			//oldAisShape = NULL;

			// 计算旋转轴的绝对值
			gp_Dir dir = data.rotationAxis;
			double newX = std::round(std::fabs(dir.X()) * 1000) / 1000;
			double newY = std::round(std::fabs(dir.Y()) * 1000) / 1000;
			double newZ = std::round(std::fabs(dir.Z()) * 1000) / 1000;
			dir.SetX(newX);
			dir.SetY(newY);
			dir.SetZ(newZ);
			// 旋转操作
			gp_Trsf localRotation;
			//localRotation.SetRotation(gp_Ax1(data.assemblyPoint.XYZ() * 2, data.rotationAxis), (data.rotationAngle) / 180.0 * M_PI);
			//localTransform = localTransform * localRotation;


			// 更新旋转轴的累计量
			addRobot.yDirCurrent = addRobot.yDirCurrent.Transformed(localRotation);
			addRobot.xDirCurrent = addRobot.xDirCurrent.Transformed(localRotation);
			addRobot.zDirCurrent = addRobot.zDirCurrent.Transformed(localRotation);

			//addRobot.assemblyPoint = assemblyPoint.Transformed(localRotation);
			qDebug() << addRobot.name << " Rotation Original *: " << assemblyPoint.X() << "," << assemblyPoint.Y() << "," << assemblyPoint.Z();
			qDebug() << addRobot.name << "Rotation New *: " << addRobot.assemblyPoint.X() << "," << addRobot.assemblyPoint.Y() << "," << addRobot.assemblyPoint.Z();
			// 平移操作
			gp_Trsf finalTransform;
			finalTransform.SetTranslation(gp_Vec(data.translation.XYZ()));
			//localTransform = localTransform * finalTransform;

			addRobot.assemblyPoint = addRobot.assemblyPoint.Transformed(finalTransform);


			qDebug() << addRobot.name << "Move Original **: " << assemblyPoint.X() << "," << assemblyPoint.Y() << "," << assemblyPoint.Z();
			qDebug() << addRobot.name << "Move New **: " << addRobot.assemblyPoint.X() << "," << addRobot.assemblyPoint.Y() << "," << addRobot.assemblyPoint.Z();
			
			// 更新模型的颜色和位置
			gp_Trsf localTransform;
			//localTransform = localTransform * finalTransform;
			localTransform = localTransform* localRotation * finalTransform;




			oldAisShape->SetLocalTransformation(localTransform);
			getContext()->SetLocation(oldAisShape, localTransform);
			getContext()->Update(oldAisShape, Standard_True);

			/*TopoDS_Shape tempShapeA = BRepBuilderAPI_Transform(tempShape, localRotation).Shape();
			TopoDS_Shape tempShapeB = BRepBuilderAPI_Transform(tempShapeA, finalTransform).Shape();
			Handle(AIS_Shape) newAisShape = new AIS_Shape(tempShapeB);*/


			/*getContext()->SetLocation(oldAisShape, oldAisShape->LocalTransformation() * localRotation);
			getContext()->Update(oldAisShape, Standard_True);
			oldAisShape->SetLocalTransformation(oldAisShape->LocalTransformation() * localRotation);
			getContext()->SetLocation(oldAisShape, oldAisShape->LocalTransformation() * finalTransform);
			getContext()->Update(oldAisShape, Standard_True);
			oldAisShape->SetLocalTransformation(oldAisShape->LocalTransformation() * localRotation);
			newAisShapes.append(oldAisShape);*/
			/*getContext()->Remove(oldAisShape, Standard_True);
			oldAisShape = NULL;
			newAisShapes.append(newAisShape);
			newAisShape->SetColor(addRobot.color);
			getContext()->Display(newAisShape, Standard_True);*/
		}
		// edit 
		addRobot.myAisShapes = newAisShapes;

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
		//robotMap[name].myAisShapes = newAisShapes;
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
			//data.rotationAngle = angleZ;
			//gp_Dir roteZ = addRobot.zDirCurrent;
			//gp_Trsf trsf;
			//trsf.SetRotation(gp_Ax1((addRobot.assemblyPoint.XYZ() + addRobot.assemblyPoint.XYZ()), data.rotationAxis), data.angleZ / 180.0 * M_PI); // Z轴旋转
			//gp_Dir roteZTemp = roteZ.Transformed(trsf);
			//data.rotationAxis = roteZTemp;
			//robotMap.insert(data.name, addRobot);
			//data.rotationAngle = angleZ;
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
		transformData.ShapeAxl3 = originData.ShapeAxl3;

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

		int ox = originData.assemblyPoint.X();
		int oy = originData.assemblyPoint.Y();
		int oz = originData.assemblyPoint.Z();

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
	void RobotCore::loadShapesModel(QMap<QString, ADDROBOTDATA>& robotMap, SHAPESTRUCT shapeStruct)
	{
		ADDROBOTDATA robotData = createRobotData(shapeStruct);

		// 移除旧模型
		removeShapeModel(robotMap, robotData);
		robotMap.insert(robotData.name, robotData);
		displaySingalAddRobot(robotData);
	}
	void RobotCore::removeShapeModel(QMap<QString, ADDROBOTDATA>& robotMap, ADDROBOTDATA data)
	{
		// 如果 robotMap 中包含指定的名称，则移除其对应的 AIS 形状并从 robotMap 中删除该名称
		if (robotMap.contains(data.name))
		{
			foreach(Handle(AIS_Shape) aisShape, robotMap[data.name].myAisShapes)
			{
				getContext()->Remove(aisShape, Standard_True);
			}
			robotMap.remove(data.name);
		}
	}
	void RobotCore::removeAllShapes()
	{
		// 移除所有模型
		getContext()->EraseAll(Standard_True);
		m_shapesMap.clear();

		// 添加坐标轴
		displayCoordinateAxes();
		myView->FitAll();

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
	void RobotCore::slotChangeShapeColor(QString shapeName, QString color)
	{
		QColor qColorA(color);
		// 改变指定名称的模型颜色
		if (m_shapesMap.keys().contains(shapeName))
		{

			// 转换为Quantity_Color
			Quantity_Color qColor(qColorA.redF(), qColorA.greenF(), qColorA.blueF(), Quantity_TOC_sRGB);
			// 修改颜色
			for (auto shape : m_shapesMap[shapeName].myAisShapes)
			{
				shape->SetColor(qColor);
			}
			fitAll();
		}
	}
	void RobotCore::slotSeletedWorkChanged(QString shapeName)
	{

	}
} // namespace TCP_ROBOT