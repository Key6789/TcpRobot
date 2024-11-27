#include "../include/RobotCore.h"

namespace TCP_ROBOT
{
	RobotCore::RobotCore(QWidget* parent)
	{
		displayCoordinateAxes();
		fitAll();

		initZeroData();
	}
	RobotCore::~RobotCore()
	{
	}
	QWidget* RobotCore::initZeroWidget(QWidget* parent)
	{
		CWidgetVLay* pLayout = new CWidgetVLay(parent);
		pLayout->setContentsMargins(0, 0, 0, 0);
		pLayout->setLaySpacing(0);

		QTableWidget* pTable = new QTableWidget(pLayout);
		pTable->setColumnCount(2);
		pTable->setRowCount(6);
		pTable->setHorizontalHeaderLabels(QStringList() << __StandQString("模型") << __StandQString("模型零点") << __StandQString("实际零点"));
		pTable->setVerticalHeaderLabels(QStringList() <<
			__StandQString("机器轴-1") <<
			__StandQString("机器轴-2") <<
			__StandQString("机器轴-3") <<
			__StandQString("机器轴-4") <<
			__StandQString("机器轴-5") <<
			__StandQString("机器轴-6"));

		pTable->setItem(0, 0, new QTableWidgetItem(0));
		pTable->setItem(1, 0, new QTableWidgetItem(0));
		pTable->setItem(2, 0, new QTableWidgetItem(0));
		pTable->setItem(3, 0, new QTableWidgetItem(0));
		pTable->setItem(4, 0, new QTableWidgetItem(0));
		pTable->setItem(5, 0, new QTableWidgetItem(0));

		pTable->setItem(0, 1, new QTableWidgetItem(0));
		pTable->setItem(1, 1, new QTableWidgetItem(0));
		pTable->setItem(2, 1, new QTableWidgetItem(0));
		pTable->setItem(3, 1, new QTableWidgetItem(0));
		pTable->setItem(4, 1, new QTableWidgetItem(0));
		pTable->setItem(5, 1, new QTableWidgetItem(0));
		QCheckBox* pCheckBox = new QCheckBox(__StandQString("模型刷新"), pLayout);
		pCheckBox->setChecked(false);
		connect(pCheckBox, &QCheckBox::stateChanged, [=](int state) {
			m_isUpdate = state;
			});


		{
			// 读取文件内容 json 格式
			QVariantMap jsonMap = readJsonFileToMap(ROBOTPATH.append("/").append("ZeroPoint.json"));
			foreach(QString key, jsonMap.keys()) {
				if (key == "update")
				{
					pCheckBox->setChecked(jsonMap[key].toBool());
					m_isUpdate = jsonMap[key].toBool();
					continue;
				}
				QVariantMap shapeMap = jsonMap[key].toMap();
				foreach(QString shapeName, shapeMap.keys()) {
					pTable->item(key.toInt(), shapeName.toInt())->setText(shapeMap[shapeName].toString());
				}
			}
		}

		CBtnsHBox* pBtns = new CBtnsHBox(pLayout);
		pBtns->setContentsMargins(0, 0, 0, 0);
		pBtns->setLaySpacing(0);
		pBtns->addBtn(0, __StandQString("更新"));
		pBtns->addBtn(1, __StandQString("保存"));
		pBtns->addBtn(2, __StandQString("应用"));

		pBtns->setConnect(2, [=]() {
			if (m_currentPostion.size() < 6)
			{

			}
			else
			{
				// 更新 前六位
				for (int i = 0; i < 6; i++)
				{
					QTableWidgetItem* pItem_1 = pTable->item(i, 1);
					if (pItem_1)
					{
						m_currentPostion[i] = pItem_1->text();
					}
				}
				m_isActiveMove = true;
				slotShapMove(m_currentPostion.join(","));
				m_isActiveMove = false;
			}
			});



		pLayout->addWidget(pTable);
		pLayout->addWidget(pCheckBox);
		pLayout->addWidget(pBtns);

		pBtns->setConnect(0, [=]() {
			// 更新
			for (int i = 0; i < 6; i++)
			{
				if (m_currentPostion.size() < 6)
				{
					// 自动补足
					QMessageBox::warning(parent, __StandQString("警告"), __StandQString("当前坐标轴数量不足6个，请补足坐标轴！"));
					return;
				}
				else
				{
					for (int j = 0; j < 6; j++)
					{
						QTableWidgetItem* pItem_1 = pTable->item(i, 1);
						if (pItem_1)
						{
							pItem_1->setText(m_currentPostion.at(j));
						}
					}
				}

			}

			});
		pBtns->setConnect(1, [=]() {
			QVariantMap jsonMap;
			for (int i = 0; i < 6; i++)
			{
				QTableWidgetItem* pItem_0 = pTable->item(i, 0);
				QTableWidgetItem* pItem_1 = pTable->item(i, 1);
				if (pItem_0->text().isEmpty() || pItem_1->text().isEmpty())
				{
					continue;
				}
				QVariantMap shapeMap;
				shapeMap.insert("0", pItem_0->text());
				shapeMap.insert("1", pItem_1->text());
				jsonMap.insert(QString::number(i), shapeMap);
			}
			jsonMap.insert("update", pCheckBox->isChecked());
			writeJsonFileFromMap(ROBOTPATH.append("/").append("ZeroPoint.json"), jsonMap);
			});
		return pLayout;
	}
	void RobotCore::initZeroData()
	{
		QStringList shapeNames;
		// 读取文件内容 json 格式
		QVariantMap jsonMap = readJsonFileToMap(ROBOTPATH.append("/").append("ZeroPoint.json"));

		if (jsonMap.keys().isEmpty())
		{
			m_currentPostion << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
		}
		else {
			foreach(QString key, jsonMap.keys()) {
				if (key == "update")
				{
					continue;
				}
				QVariantMap shapeMap = jsonMap[key].toMap();
				shapeNames.append(shapeMap["1"].toString());
			}
			m_currentPostion = shapeNames;
			m_currentPostion.append("0");
			m_currentPostion.append("0");
		}
	}
	void RobotCore::slotShapMove(QString shapeValue)
	{
		TCPXVIEWMODULE_LOG;
		QStringList shapeValues = shapeValue.split(",");
		if (shapeValues.size() < 6)
		{
			return;
		}
		// 控制界面刷新
		if (!m_isUpdate && !m_isActiveMove)
		{
			return;
		}
		// 判断 m_currentPostion = shapeValues 是否有变化
		if (m_currentPostion == shapeValues)
		{
			// 无变化 退出
			return;
		}
		else
		{
			emit signalUpdateRobotShaps(true);
			//m_currentPostion = shapeValues;
		}

		//emit signalUpdateRobotShaps(true);
		
		// 启动 定时器 20 ms 后开始触发，给界面反应时间
		QTimer *timer = new QTimer(this);
		timer->start();

		qDebug() << "timer.start:" ;
		
		connect(timer, &QTimer::timeout, [=]() {
			// 取前6个参数
			timer->stop();
			qDebug() << "updateRobotShaps:" ;
			double robot1 = shapeValues.at(0).toDouble() - m_currentPostion.at(0).toDouble();
			double robot2 = shapeValues.at(1).toDouble() - m_currentPostion.at(1).toDouble();
			double robot3 = shapeValues.at(2).toDouble() - m_currentPostion.at(2).toDouble();
			double robot4 = shapeValues.at(3).toDouble() - m_currentPostion.at(3).toDouble();
			double robot5 = shapeValues.at(4).toDouble() - m_currentPostion.at(4).toDouble();
			double robot6 = shapeValues.at(5).toDouble() - m_currentPostion.at(5).toDouble();

			// 取出导轨移动
			double shortMove = shapeValues.at(6).toDouble() - m_currentPostion.at(6).toDouble();

			// 取出旋转角度
			double TableAngle = shapeValues.at(7).toDouble() - m_currentPostion.at(7).toDouble();
			// 机器人旋转
			if (robot1 != 0.0)
			{
				qDebug() << "robot1:" << robot1;
				slotRobotRotateShape(m_robotMap, robot1, ShapeType::ShapeType_Robot, MoveDirection::MoveDirection_ZAxis, 2);
			}
			if (robot2 != 0.0)
			{
				qDebug() << "robot2:" << robot2;
				slotRobotRotateShape(m_robotMap, robot2, ShapeType::ShapeType_Robot, MoveDirection::MoveDirection_ZAxis, 3);
			}
			if (robot3 != 0.0)
			{
				qDebug() << "robot3:" << robot3;
				slotRobotRotateShape(m_robotMap, robot3, ShapeType::ShapeType_Robot, MoveDirection::MoveDirection_ZAxis, 4);
			}
			if (robot4 != 0.0)
			{
				qDebug() << "robot4:" << robot4;
				slotRobotRotateShape(m_robotMap, robot4, ShapeType::ShapeType_Robot, MoveDirection::MoveDirection_ZAxis, 5);
			}
			if (robot5 != 0.0)
			{
				qDebug() << "robot5:" << robot5;
				slotRobotRotateShape(m_robotMap, robot5, ShapeType::ShapeType_Robot, MoveDirection::MoveDirection_ZAxis, 6);
			}
			if (robot6 != 0.0)
			{
				qDebug() << "robot6:" << robot6;
				slotRobotRotateShape(m_robotMap, robot6, ShapeType::ShapeType_Robot, MoveDirection::MoveDirection_ZAxis, 7);
			}

			if (shortMove != 0.0)
			{
				slotShapesMoveShape(m_otherMap, shortMove, ShapeType::ShapeType_ShortGuide, MoveDirection_XAxis);
				slotShapesMoveShape(m_shapesMap, shortMove, ShapeType::ShapeType_Work, MoveDirection_XAxis);
			}
			if (TableAngle != 0.0)
			{
				slotShapesRotateShape(m_otherMap, TableAngle, ShapeType::ShapeType_RotatingTable);
				slotShapesRotateShape(m_shapesMap, TableAngle, ShapeType::ShapeType_Work);
			}
			emit signalUpdateRobotShaps(false);
			m_currentPostion = shapeValues;
			});

		timer->setInterval(20);
	}
	void RobotCore::slotUpdataRobotShaps(void)
	{
		// 添加箭头

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
				addRobotData.printAll();
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
				qDebug() << "slotShapesMoveShape:" << addRobotData.name << " moveDistance:" << moveDistance;
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
		addRobot.setShapeRotate(gp_Ax1(data.ShapeAxl3.Location(), data.ShapeAxl3.Direction()), data.rotationAngle / 180.0 * M_PI);

		addRobot.setShapeMoveX(data.translation.X());
		addRobot.setShapeMoveY(data.translation.Y());
		addRobot.setShapeMoveZ(data.translation.Z());

		addRobot.myAisShapes = RobotTransformParallelPreview(addRobot);

		robotMap.insert(name, addRobot);
		// 递归处理下一个组件
		if (!addRobot.nextShapeNames.isEmpty())
		{
			foreach(QString nextShapeNameA, addRobot.nextShapeNames)
			{
				if (robotMap.contains(nextShapeNameA))
				{
					ShapesTransformRecursively(robotMap, nextShapeNameA, data);
				}
			}
		}
		for (auto& newAisShape : addRobot.myAisShapes)
		{
			getContext()->Display(newAisShape, Standard_True);
		}
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
			ShapesTransformRecursively(robotMap, data.name, data);
		}
		if (angleY != 0)
		{
			ShapesTransformRecursively(robotMap, data.name, data);
		}
		if (angleZ != 0)
		{
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
		//transformData.printAll();
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
	void RobotCore::writeJsonFileFromMap(QString filePath, QVariantMap map)
	{
		QFile file(filePath);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			qDebug() << "open file failed";
		}
		QTextStream out(&file);
		QJsonDocument jsonDoc(QJsonObject::fromVariantMap(map));
		out << jsonDoc.toJson(QJsonDocument::Indented);
		file.close();
	}
} // namespace TCP_ROBOT