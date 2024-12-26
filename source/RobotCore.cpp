#include "../include/RobotCore.h"

namespace TCP_ROBOT
{
	RobotCore::RobotCore(QWidget* parent)
	{
		LOG_FUNCTION_LINE_MESSAGE;
		displayCoordinateAxes();
		fitAll();

		initZeroData();
	}
	RobotCore::~RobotCore()
	{
	}
	QWidget* RobotCore::initZeroWidget(QWidget* parent)
	{
		LOG_FUNCTION_LINE_MESSAGE;
		CWidgetVLay* pLayout = new CWidgetVLay(parent);
		pLayout->setContentsMargins(0, 0, 0, 0);
		pLayout->setLaySpacing(0);

		QTableWidget* pTable = new QTableWidget(pLayout);
		pTable->setColumnCount(2);
		pTable->setRowCount(8);
		pTable->setHorizontalHeaderLabels(QStringList() << __StandQString("模型零点") << __StandQString("实际零点") << __StandQString("实际零点"));
		pTable->setVerticalHeaderLabels(QStringList() <<
			__StandQString("机器轴-1") <<
			__StandQString("机器轴-2") <<
			__StandQString("机器轴-3") <<
			__StandQString("机器轴-4") <<
			__StandQString("机器轴-5") <<
			__StandQString("机器轴-6") <<
			__StandQString("外部轴-1") <<
			__StandQString("外部轴-2"));

		pTable->setItem(0, 0, new QTableWidgetItem(0));
		pTable->setItem(1, 0, new QTableWidgetItem(0));
		pTable->setItem(2, 0, new QTableWidgetItem(0));
		pTable->setItem(3, 0, new QTableWidgetItem(0));
		pTable->setItem(4, 0, new QTableWidgetItem(0));
		pTable->setItem(5, 0, new QTableWidgetItem(0));
		pTable->setItem(6, 0, new QTableWidgetItem(0));
		pTable->setItem(7, 0, new QTableWidgetItem(0));

		pTable->setItem(0, 1, new QTableWidgetItem(0));
		pTable->setItem(1, 1, new QTableWidgetItem(0));
		pTable->setItem(2, 1, new QTableWidgetItem(0));
		pTable->setItem(3, 1, new QTableWidgetItem(0));
		pTable->setItem(4, 1, new QTableWidgetItem(0));
		pTable->setItem(5, 1, new QTableWidgetItem(0));
		pTable->setItem(6, 1, new QTableWidgetItem(0));
		pTable->setItem(7, 1, new QTableWidgetItem(0));

		CLabDoubleSpinBox* pDoubleSpinBox = new CLabDoubleSpinBox(pLayout);
		pDoubleSpinBox->setLabelText(__StandQString("行程比 360°: "));
		pDoubleSpinBox->setRange(0.0, INT_MAX);
		pDoubleSpinBox->setSingleStep(0.01);
		pDoubleSpinBox->setValue(1.0);
		pDoubleSpinBox->setSuffix(__StandQString("mm"));
		pLayout->addWidget(pDoubleSpinBox);

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
				if (key == "rowRatio")
				{
					pDoubleSpinBox->setValue(jsonMap[key].toDouble());
					m_rowRate = jsonMap[key].toDouble();
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
		pBtns->addBtn(0, __StandQString("更新坐标"));
		pBtns->addBtn(1, __StandQString("位置预览"));
		pBtns->addBtn(2, __StandQString("应用零点"));

		pBtns->setConnect(2, [=]() {
			QMessageBox* msgBox = new QMessageBox(parent);
			msgBox->setText(__StandQString("是否应用当前零点？"));
			msgBox->setInformativeText(__StandQString("应用零点将会重新加载模型文件、开启实时刷新且模型位置发生实时变化，是否继续？"));
			msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
			msgBox->setDefaultButton(QMessageBox::No);
			int ret = msgBox->exec();
			if (ret == QMessageBox::Yes)
			{
				// 保存零点
				QVariantMap jsonMap;
				for (int i = 0; i < 8; i++)
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
				jsonMap.insert("rowRatio", pDoubleSpinBox->value());
				writeJsonFileFromMap(ROBOTPATH.append("/").append("ZeroPoint.json"), jsonMap);

				if (m_currentPostion.size() < 8)
				{
					QMessageBox::warning(parent, __StandQString("警告"), __StandQString("当前坐标轴数量不足8个，请补足坐标轴！"));
					return;
				}
				else
				{
					// 界面停止刷新
					m_isUpdate = false;
					// 重新加载 模型文件
					initRobotCore();
					// 计算模型零点差值
					QStringList shapeZero = QStringList();

					m_zeroPositon = QString();
					for (int i = 0; i < 8; i++)
					{
						QTableWidgetItem* pItem_0 = pTable->item(i, 0);
						if (pItem_0->text().isEmpty())
						{
							continue;
						}
						shapeZero.append(pItem_0->text());
					}
					// 计算零点差值 补零
					if (shapeZero.size() < 8)
					{
						for (int i = 0; i < 9 - shapeZero.size(); i++)
						{
							shapeZero.append("0");
						}
					}
					m_zeroPositon = shapeZero.join(",");

					// 更新模型到实际零点
					slotInitMove();
					// 界面开始刷新 计算各个坐标轴差值
					m_currentPostion = QStringList();
					for (int i = 0; i < 8; i++)
					{
						QTableWidgetItem* pItem_1 = pTable->item(i, 1);
						if (pItem_1)
						{
							m_currentPostion.append(pItem_1->text());
						}
					}
					// 界面开始刷新
					m_isUpdate = true;
				}
			}
			else { return; }
			});

		pLayout->addWidget(pTable);
		pLayout->addWidget(pCheckBox);
		pLayout->addWidget(pBtns);

		pBtns->setConnect(0, [=]() {
			// 更新
			for (int i = 0; i < 8; i++)
			{
				QStringList postion = m_lastCurrentPostion.split(",");
				if (postion.size() < 8)
				{
					// 自动补足
					QMessageBox::warning(parent, __StandQString("警告"), __StandQString("当前坐标轴数量不足8个，请补足坐标轴！"));
					return;
				}
				else
				{
					for (int j = 0; j < 8; j++)
					{
						QTableWidgetItem* pItem_1 = pTable->item(j, 1);
						if (pItem_1)
						{
							pItem_1->setText(postion.at(j));
						}
					}

				}

			}

			});
		pBtns->setConnect(1, [=]() {
			// 界面停止刷新
			m_isUpdate = false;
			m_rowRate = pDoubleSpinBox->value();
			// 重新加载 模型文件
			if (!m_isUpdate)
			{
				QMessageBox* msgBox = new QMessageBox(parent);
				msgBox->setText(__StandQString("是否进行零点矫正？"));
				msgBox->setInformativeText(__StandQString("矫正模型将会重新加载模型文件、停止实时刷新且模型位置发生变化，是否继续？"));
				msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
				msgBox->setDefaultButton(QMessageBox::No);
				int ret = msgBox->exec();
				if (ret == QMessageBox::Yes)
				{
					initRobotCore();
				}
				else
				{
					m_isUpdate = true;
					return;
				}
			}
			QStringList shapeZero = QStringList();
			// 计算模型零点差值
			m_zeroPositon = QString();
			for (int i = 0; i < 8; i++)
			{
				QTableWidgetItem* pItem_0 = pTable->item(i, 0);
				if (pItem_0->text().isEmpty())
				{
					continue;
				}
				shapeZero.append(pItem_0->text());
			}
			m_zeroPositon = shapeZero.join(",");
			// 更新模型到实际零点
			slotInitMove();
			});

		pLayout->addStretch();
		// 


		return pLayout;
	}
	void RobotCore::initZeroData()
	{
		LOG_FUNCTION_LINE_MESSAGE;
		QStringList shapeNames = QStringList();
		QStringList shapeZeros = QStringList();
		// 读取文件内容 json 格式
		QVariantMap jsonMap = readJsonFileToMap(ROBOTPATH.append("/").append("ZeroPoint.json"));

		if (jsonMap.keys().isEmpty())
		{
			m_currentPostion << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0";
			m_zeroPositon = QString("0,0,0,0,0,0,0,0");
			m_rowRate = 1.0;
		}
		else {
			foreach(QString key, jsonMap.keys()) {
				if (key == "update")
				{
					continue;
				}
				if (key == "rowRatio")
				{
					m_rowRate = jsonMap[key].toDouble();
					continue;
				}
				QVariantMap shapeMap = jsonMap[key].toMap();
				shapeNames.append(shapeMap["1"].toString());
				shapeZeros.append(shapeMap["0"].toString());
			}
			// 如果小于 8 个 补足
			if (shapeNames.size() < 8)
			{
				for (int i = 0; i < 9 - shapeNames.size(); i++)
				{
					shapeNames.append("0");
				}
			}
			if (shapeZeros.size() < 8)
			{
				for (int i = 0; i < 9 - shapeZeros.size(); i++)
				{
					shapeZeros.append("0");
				}
			}
			m_currentPostion = shapeNames;
			m_zeroPositon = shapeZeros.join(",");
		}
	}
	void RobotCore::initRobotCore()
	{
		// 加载机器人 模型
		foreach(QString shapeName, m_shapesMap.keys())
		{
			ADDROBOTDATA& data = m_shapesMap[shapeName];
			data.initShapeAx3();
			data.isChanged = true;
		}
		foreach(QString shapeName, m_robotMap.keys())
		{
			ADDROBOTDATA& data = m_robotMap[shapeName];
			data.initShapeAx3();
			data.isChanged = true;
		}
		foreach(QString shapeName, m_otherMap.keys())
		{
			ADDROBOTDATA& data = m_otherMap[shapeName];
			data.initShapeAx3();
			data.isChanged = true;
		}
		updateShapTrsf(m_shapesMap);
		updateShapTrsf(m_robotMap);
		updateShapTrsf(m_otherMap);
		/*loadWorkShapes(m_currentWorkPath);
		loadRobotShape(m_currentRobotPath);
		loadOtherShape(m_currentOtherPath);*/
	}
	QVector<double> RobotCore::getChangedPostion(QString shapeValue)
	{
		LOG_FUNCTION_LINE_MESSAGE;
		QVector<double> changedPostion;
		QStringList shapeValues = shapeValue.split(",");
		if (shapeValues.size() < 8)
		{
			for (int i = 0; i < 8 - shapeValues.size(); i++)
			{
				shapeValues.append("0");
			}
		}
		return QVector<double>();
	}
	void RobotCore::slotShapMove(QString shapeValue)
	{
		TCPXVIEWMODULE_LOG;
		LOG_FUNCTION_LINE_MESSAGE;
		// 防冲入的
		if (m_isActiveUpdateShap)
		{
			return;
		}
		m_isActiveUpdateShap = true;
		QStringList shapeValues = shapeValue.split(",");
		if (shapeValues.size() < 6)
		{
			m_isActiveUpdateShap = false;
			return;
		}
		// 控制界面刷新
		if (!m_isUpdate && !m_isActiveMove)
		{
			m_isActiveUpdateShap = false;
			return;
		}
		// 判断 m_currentPostion = shapeValues 是否有变化
		if (m_currentPostion == shapeValues)
		{
			emit signalUpdateRobotShaps(false);
			m_isActiveUpdateShap = false;
			// 无变化 退出
			return;
		}
		else
		{
			emit signalUpdateRobotShaps(true);
		}
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
			slotRobotRotateShape(m_robotMap, robot1, ShapeType::ShapeType_Robot, MoveDirection::MoveDirection_ZAxis, 2);
		}
		if (robot2 != 0.0)
		{
			slotRobotRotateShape(m_robotMap, robot2, ShapeType::ShapeType_Robot, MoveDirection::MoveDirection_ZAxis, 3);
		}
		if (robot3 != 0.0)
		{
			slotRobotRotateShape(m_robotMap, robot3, ShapeType::ShapeType_Robot, MoveDirection::MoveDirection_ZAxis, 4);
		}
		if (robot4 != 0.0)
		{
			slotRobotRotateShape(m_robotMap, robot4, ShapeType::ShapeType_Robot, MoveDirection::MoveDirection_ZAxis, 5);
		}
		if (robot5 != 0.0)
		{
			slotRobotRotateShape(m_robotMap, robot5, ShapeType::ShapeType_Robot, MoveDirection::MoveDirection_ZAxis, 6);
		}
		if (robot6 != 0.0)
		{
			slotRobotRotateShape(m_robotMap, robot6, ShapeType::ShapeType_Robot, MoveDirection::MoveDirection_ZAxis, 7);
		}

		if (shortMove != 0.0)
		{
			double moveDistance = shortMove / 360.0 * m_rowRate;
			slotShapesMoveShape(m_otherMap, moveDistance, ShapeType::ShapeType_ShortGuide, MoveDirection_XAxis);
			slotShapesMoveShape(m_shapesMap, moveDistance, ShapeType::ShapeType_Work, MoveDirection_XAxis);
		}
		if (TableAngle != 0.0)
		{
			slotShapesRotateShape(m_otherMap, TableAngle, ShapeType::ShapeType_RotatingTable);
			slotShapesRotateShape(m_shapesMap, TableAngle, ShapeType::ShapeType_Work);
		}
		// 统一刷新
		foreach(QString shapeName, m_shapesMap.keys())
		{
			if (m_shapesMap[shapeName].isChanged)
			{
				m_shapesMap[shapeName].isChanged = false;
				updateShapeModel(m_shapesMap, shapeName);
			}
		}
		foreach(QString shapeName, m_robotMap.keys())
		{
			if (m_robotMap[shapeName].isChanged)
			{
				m_robotMap[shapeName].isChanged = false;
				updateShapeModel(m_robotMap, shapeName);
			}
		}
		foreach(QString shapeName, m_otherMap.keys())
		{
			if (m_otherMap[shapeName].isChanged)
			{
				m_otherMap[shapeName].isChanged = false;
				updateShapeModel(m_otherMap, shapeName);
			}
		}
		// 刷新界面
		emit signalUpdateRobotShaps(false);
		// 关闭定时器
		//timer->deleteLater();

		foreach(QString shapeName, m_shapesMap.keys())
		{
			foreach(QString robotName, m_robotMap.keys())
			{
				setCollisionDetection(m_shapesMap[shapeName], m_robotMap[robotName]);
			}
		}
		//});
		m_currentPostion = shapeValues;
		getContext()->UpdateCurrentViewer();
		m_isActiveUpdateShap = false;
		//timer->setInterval(20);
	}
	void RobotCore::updateLastCurrentPostion(QString shapeValue)
	{
		m_lastCurrentPostion = shapeValue;
	}
	void RobotCore::slotInitMove()
	{
		LOG_FUNCTION_LINE_MESSAGE;
		// 读取机器人 零点数据
		QStringList shapeValues = m_zeroPositon.split(",");
		if (shapeValues.size() < 8)
		{
			QMessageBox::warning(this, __StandQString("警告"), __StandQString("坐标轴数量不足8个，请补足坐标轴！"));
			return;
		}
		// 计算机器人 零点差值
		double robot1 = shapeValues.at(0).toDouble();
		double robot2 = shapeValues.at(1).toDouble();
		double robot3 = shapeValues.at(2).toDouble();
		double robot4 = shapeValues.at(3).toDouble();
		double robot5 = shapeValues.at(4).toDouble();
		double robot6 = shapeValues.at(5).toDouble();
		double shortMove = shapeValues.at(6).toDouble();
		double TableAngle = shapeValues.at(7).toDouble();
		// 机器人旋转
		if (robot1 != 0.0)
		{
			slotRobotRotateShape(m_robotMap, robot1, ShapeType::ShapeType_Robot, MoveDirection::MoveDirection_ZAxis, 2);
		}
		if (robot2 != 0.0)
		{
			slotRobotRotateShape(m_robotMap, robot2, ShapeType::ShapeType_Robot, MoveDirection::MoveDirection_ZAxis, 3);
		}
		if (robot3 != 0.0)
		{
			slotRobotRotateShape(m_robotMap, robot3, ShapeType::ShapeType_Robot, MoveDirection::MoveDirection_ZAxis, 4);
		}
		if (robot4 != 0.0)
		{
			slotRobotRotateShape(m_robotMap, robot4, ShapeType::ShapeType_Robot, MoveDirection::MoveDirection_ZAxis, 5);
		}
		if (robot5 != 0.0)
		{
			slotRobotRotateShape(m_robotMap, robot5, ShapeType::ShapeType_Robot, MoveDirection::MoveDirection_ZAxis, 6);
		}
		if (robot6 != 0.0)
		{
			slotRobotRotateShape(m_robotMap, robot6, ShapeType::ShapeType_Robot, MoveDirection::MoveDirection_ZAxis, 7);
		}
		// 导轨移动
		if (shortMove != 0.0)
		{
			double moveDistance = shortMove / 360.0 * m_rowRate;
			slotShapesMoveShape(m_otherMap, moveDistance, ShapeType::ShapeType_ShortGuide, MoveDirection_XAxis);
			slotShapesMoveShape(m_shapesMap, moveDistance, ShapeType::ShapeType_Work, MoveDirection_XAxis);
		}
		// 旋转台
		if (TableAngle != 0.0)
		{
			slotShapesRotateShape(m_otherMap, TableAngle, ShapeType::ShapeType_RotatingTable);
			slotShapesRotateShape(m_shapesMap, TableAngle, ShapeType::ShapeType_Work);
		}
		// 统一刷新
		foreach(QString shapeName, m_shapesMap.keys())
		{
			if (m_shapesMap[shapeName].isChanged)
			{
				m_shapesMap[shapeName].isChanged = false;
				updateShapeModel(m_shapesMap, shapeName);
			}
		}
		foreach(QString shapeName, m_robotMap.keys())
		{
			if (m_robotMap[shapeName].isChanged)
			{
				m_robotMap[shapeName].isChanged = false;
				updateShapeModel(m_robotMap, shapeName);
			}
		}
		foreach(QString shapeName, m_otherMap.keys())
		{
			if (m_otherMap[shapeName].isChanged)
			{
				m_otherMap[shapeName].isChanged = false;
				updateShapeModel(m_otherMap, shapeName);
			}
		}
		foreach(QString shapeName, m_shapesMap.keys())
		{
			foreach(QString robotName, m_robotMap.keys())
			{
				setCollisionDetection(m_shapesMap[shapeName], m_robotMap[robotName]);
			}
		}
		getContext()->UpdateCurrentViewer();
	}
	void RobotCore::slotUpdataRobotShaps(void)
	{
		// 添加箭头

	}
	void RobotCore::loadWorkShapes(QString filePath)
	{
		LOG_FUNCTION_LINE_MESSAGE;
		m_currentWorkPath = filePath;
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
		LOG_FUNCTION_LINE_MESSAGE;
		m_currentRobotPath = filePath;
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
		LOG_FUNCTION_LINE_MESSAGE;
		m_currentOtherPath = filePath;
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
		LOG_FUNCTION_LINE_MESSAGE;
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
		LOG_FUNCTION_LINE_MESSAGE;
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
		LOG_FUNCTION_LINE_MESSAGE;
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
		LOG_FUNCTION_LINE_MESSAGE;
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

		//addRobot.myAisShapes = RobotTransformParallelPreview(addRobot);
		addRobot.isChanged = true;
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
		/*for (auto& newAisShape : addRobot.myAisShapes)
		{
			getContext()->Display(newAisShape, Standard_True);
		}*/
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
	void RobotCore::updateShapeModel(QMap<QString, ADDROBOTDATA>& robotMap, QString shapeName)
	{
		ADDROBOTDATA addRobot = robotMap.value(shapeName);
		// 移除旧模型
		//addRobot.myAisShapes = RobotTransformParallelPreview(addRobot);
		addRobot.myAisShapes = updateShapTrsf(addRobot);
		robotMap.insert(shapeName, addRobot);
		/*for (auto& newAisShape : addRobot.myAisShapes)
		{
			getContext()->Update(newAisShape, Standard_True);
		}*/
	}
	QVector<Handle(AIS_Shape)> RobotCore::updateShapTrsf(ADDROBOTDATA data)
	{
		//data.initShapeAx3();
		QVector<TopoDS_Shape> vector = data.shapes;
		QVector<Handle(AIS_Shape)> vectortemp;

		gp_Trsf trsf;
		trsf.SetTransformation(data.ShapeAxl3, gp_Ax3());
		gp_Trsf trsfA;
		data.printAll();
		trsfA.SetRotation(gp_Ax1(data.ShapPnt(), gp_Dir(0, 0, 1)), 90.0 / 180.0 * M_PI);
		foreach(Handle(AIS_Shape) ais, data.myAisShapes)
		{
			//TopoDS_Shape shape = ais->Shape();
			//if (shape.IsNull())
			//{
			//	continue;
			//}
			//myContext->Remove(ais, Standard_True); // 移除旧的形状
			//// 
			//shape.Location(trsf);
			//ais->SetShape(shape);
			//myContext->Display(ais, Standard_True);
			//ais->UpdateTransformation();
			gp_Trsf trsfS = ais->LocalTransformation();

			ais->ResetTransformation();
			ais->SetLocalTransformation(trsf);
			ais->UpdateTransformation();

			/*ais->SetLocalTransformation(trsf);
			ais->UpdateTransformation();*/
			//getContext()->Update(ais, Standard_True);
			vectortemp.push_back(ais);

			//if (ais->Shape().IsNull())
			//{
			//	continue;
			//}
			//m_mutex->lock();
			//myContext->Remove(ais, Standard_True); // 移除旧的形状
			//m_mutex->unlock();
		}

		/*for (int i = 0; i < vector.size(); ++i)
		{

			TopoDS_Shape transformedShape = BRepBuilderAPI_Transform(vector[i], trsf).Shape();
			Handle(AIS_Shape) aisShapeTemp = new AIS_Shape(transformedShape);
			aisShapeTemp->SetColor(data.color);
			vectortemp.push_back(aisShapeTemp);
		}*/
		return vectortemp;
	}
	void RobotCore::updateShapTrsf(QMap<QString, ADDROBOTDATA>& robotMap)
	{
		foreach(QString shapeName, robotMap.keys())
		{
			if (robotMap[shapeName].isChanged)
			{
				robotMap[shapeName].isChanged = false;
				updateShapeModel(robotMap, shapeName);
			}
		}
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


	void RobotCoreClone::cloneRobot(int count, CloneData cloneData)
	{
		LOG_FUNCTION_LINE_MESSAGE;
		QMap<QString, ADDROBOTDATA> robotMap = getCurrentRobotMap();
		m_cloneOtherMap = getOtherMap();
		QMap<QString, ADDROBOTDATA> newRobotMap = robotMap;
		for (int i = 0; i < count; i++)
		{
			for (auto it = robotMap.begin(); it != robotMap.end(); ++it)
			{
				QString name = it.key();
				ADDROBOTDATA addRobotData = it.value();
				addRobotData.assemblyPoint = gp_Pnt(cloneData.x * i + addRobotData.assemblyPoint.X(), cloneData.y * i + addRobotData.assemblyPoint.Y(), cloneData.z * i + addRobotData.assemblyPoint.Z());
				addRobotData.angleX = cloneData.angleX * i + addRobotData.angleX;
				addRobotData.angleY = cloneData.angleY * i + addRobotData.angleY;
				addRobotData.angleZ = cloneData.angleZ * i + addRobotData.angleZ;
				addRobotData.sacle = cloneData.scale * i + addRobotData.sacle;

				addRobotData.shapes = scaleShapes(addRobotData.shapes, addRobotData.sacle);
				addRobotData.initShapeAx3();
				addRobotData.myAisShapes = RobotTransformParallelPreview(addRobotData);
				newRobotMap.insert(name, addRobotData);

				// 移除旧模型
				removeShapeModel(newRobotMap, addRobotData);
				// 显示新模型
				newRobotMap.insert(name, addRobotData);
				displaySingalAddRobot(addRobotData);
			}
			cloneData.count = i;
			cloneData.robotMap = newRobotMap;
			m_cloneData.insert(i, cloneData);
		}
	}
	void RobotCoreClone::slotUpdataCount(int count, QString Value)
	{
		QMap<QString, ADDROBOTDATA> robotMap = m_cloneData[count].robotMap;
		for (int i = 0; i < robotMap.keys().size(); i++)
		{
			QStringList values = Value.split(",");
			slotRobotRotateShape(m_cloneData[count].robotMap, values[i].toDouble(), ShapeType::ShapeType_Robot, MoveDirection::MoveDirection_ZAxis, i);
			foreach(QString shapeName, m_cloneData[count].robotMap.keys())
			{
				if (m_cloneData[count].robotMap[shapeName].isChanged)
				{
					m_cloneData[count].robotMap[shapeName].isChanged = false;
					updateShapeModel(m_cloneData[count].robotMap, shapeName);
				}
			}

		}
	}
	void RobotCoreClone::slotUpdate()
	{
		getContext()->UpdateCurrentViewer();

	}
	void RobotCoreClone::slotUpdataDemo(QString value)
	{
		QStringList values = value.split(",");
		if (values.size() != 5)
		{
			return;
		}
		slotShapesMoveShape(m_cloneOtherMap, values[0].toDouble(), ShapeType::ShapeType_Clone_1, MoveDirection::MoveDirection_YAxis);
		slotShapesMoveShape(m_cloneOtherMap, values[1].toDouble(), ShapeType::ShapeType_Clone_2, MoveDirection::MoveDirection_YAxis);
		slotShapesMoveShape(m_cloneOtherMap, values[2].toDouble(), ShapeType::ShapeType_Clone_3, MoveDirection::MoveDirection_XAxis);
		slotShapesMoveShape(m_cloneOtherMap, values[3].toDouble(), ShapeType::ShapeType_Clone_4, MoveDirection::MoveDirection_ZAxis);
		slotShapesRotateShape(m_cloneOtherMap, values[4].toDouble(), ShapeType::ShapeType_Clone_5, MoveDirection::MoveDirection_ZAxis);
		foreach(QString shapeName, m_cloneOtherMap.keys())
		{
			if (m_cloneOtherMap[shapeName].isChanged)
			{
				m_cloneOtherMap[shapeName].isChanged = false;
				updateShapeModel(m_cloneOtherMap, shapeName);
			}
		}
	}
	void RobotCoreClone::initMoveCloneShape()
	{
		for (int i = 0; i < m_cloneData.size(); i++)
		{
			initMoveCloneShape(i);
		}
	}
	void RobotCoreClone::initMoveCloneShape(int count)
	{
		if (!m_cloneData.contains(count))
		{
			QMessageBox::warning(nullptr, __TCPString("警告"), __TCPString("没有克隆数据%1").arg(count));
			return;
		}
		QMap<QString, ADDROBOTDATA> robotMap = m_cloneData[count].robotMap;
		for (auto it = robotMap.begin(); it != robotMap.end(); ++it)
		{
			QString name = it.key();
			ADDROBOTDATA addRobotData = it.value();
			addRobotData.initShapeAx3();
			addRobotData.myAisShapes = RobotTransformParallelPreview(addRobotData);
			m_cloneData[count].robotMap.insert(name, addRobotData);
		}
	}
	void RobotCoreClone::initConnect()
	{
		//foreach
	}
	QWidget* RobotCoreClone::initCloneShapeWidget(int count, QWidget* parent)
	{
		CWidgetVLay* widget = new CWidgetVLay(parent);
		QMap<QString, ADDROBOTDATA> robotMap = m_cloneData[count].robotMap;
		widget->addWidget(new QLabel(__TCPString("主控-%1").arg(count)));
		for (int i = 0; i < robotMap.keys().size(); i++)
		{
			CLabLineEditBtn* line = new CLabLineEditBtn(widget);
			line->setLabelText(robotMap.keys()[i]);
			line->setLineEditText("0");
			line->setBtnText(__TCPString("确认"));
			line->setLabelText(robotMap.keys()[i] + QString("(%1)").arg(0));
			m_cloneData[count].currentValueMap.insert(i, 0);
			line->connectBtnClicked([=]() {
				double value = line->getLineEditText().toDouble() + m_cloneData[count].currentValueMap[i];
				m_cloneData[count].currentValueMap.insert(i, value);
				line->setLabelText(robotMap.keys()[i] + QString("(%1)").arg(value));
				//initMoveCloneShape(count);
				slotRobotRotateShape(m_cloneData[count].robotMap, line->getLineEditText().toDouble(), ShapeType::ShapeType_Robot, MoveDirection::MoveDirection_ZAxis, i);
				foreach(QString shapeName, m_cloneData[count].robotMap.keys())
				{
					if (m_cloneData[count].robotMap[shapeName].isChanged)
					{
						m_cloneData[count].robotMap[shapeName].isChanged = false;
						updateShapeModel(m_cloneData[count].robotMap, shapeName);
					}
				}
				getContext()->UpdateCurrentViewer();
				});
			widget->addWidget(line);
			m_cloneData[count].labLineMap.insert(robotMap.keys()[i], line);
		}
		return widget;
	}
	QWidget* RobotCoreClone::initCloneRobotWidget(QWidget* parent)
	{
		CWidgetVLay* widget = new CWidgetVLay(parent);
		QMap<QString, ADDROBOTDATA> robotMap = getCurrentRobotMap();
		widget->addWidget(new QLabel(__TCPString("主控")));
		for (int i = 0; i < robotMap.keys().size(); i++)
		{
			CLabLineEditBtn* line = new CLabLineEditBtn(widget);
			line->setLabelText(robotMap.keys()[i]);
			line->setBtnText(__TCPString("确认"));
			line->setLineEditText(0);

			static int countValue = 0;
			line->setLabelText(robotMap.keys()[i] + QString("(%1)").arg(countValue));
			line->connectBtnClicked([=]() {
				countValue = countValue + line->getLineEditText().toDouble();
				line->setLabelText(robotMap.keys()[i] + QString("(%1)").arg(countValue));
				for (int count = 0; count < m_cloneData.size(); count++)
				{
					//initMoveCloneShape();
					slotRobotRotateShape(m_cloneData[count].robotMap, line->getLineEditText().toDouble(), ShapeType::ShapeType_Robot, MoveDirection::MoveDirection_ZAxis, i);
					foreach(QString shapeName, m_cloneData[count].robotMap.keys())
					{
						if (m_cloneData[count].robotMap[shapeName].isChanged)
						{
							m_cloneData[count].robotMap[shapeName].isChanged = false;
							updateShapeModel(m_cloneData[count].robotMap, shapeName);
						}
					}
				}
				getContext()->UpdateCurrentViewer();
				});
			widget->addWidget(line);
			m_cloneLabLineMap.insert(robotMap.keys()[i], line);
		}
		return widget;
	}
	QWidget* RobotCoreClone::initCloneSingleALLWidget(QWidget* parent)
	{
		CWidgetHLay* widget = new CWidgetHLay(parent);
		for (int i = 0; i < m_cloneData.size(); i++)
		{
			widget->addWidget(initCloneShapeWidget(i, parent));
		}
		return widget;
	}
	QWidget* RobotCoreClone::initCloneDemoWidget(QWidget* parent)
	{
		CWidgetVLay* widget = new CWidgetVLay(parent);
		widget->addWidget(new QLabel(__TCPString("射线控制机构")));


		CLabLineEditBtn* line = new CLabLineEditBtn(widget);
		line->setLabelText(__TCPString("部件-1"));
		line->setLineEditText("0");
		line->setBtnText(__TCPString("确认"));

		static int countValue = 0;
		line->setLabelText(__TCPString("部件-1(%1)").arg(countValue));
		line->connectBtnClicked([=]() {
			countValue = countValue + line->getLineEditText().toDouble();
			line->setLabelText(__TCPString("部件-1(%1)").arg(countValue));
			slotShapesMoveShape(m_cloneOtherMap, line->getLineEditText().toDouble(), ShapeType::ShapeType_Clone_1, MoveDirection::MoveDirection_YAxis);
			foreach(QString shapeName, m_cloneOtherMap.keys())
			{
				if (m_cloneOtherMap[shapeName].isChanged)
				{
					m_cloneOtherMap[shapeName].isChanged = false;
					updateShapeModel(m_cloneOtherMap, shapeName);
				}
			}
			getContext()->UpdateCurrentViewer();

			});

		CLabLineEditBtn* line2 = new CLabLineEditBtn(widget);
		line2->setLabelText(__TCPString("部件-2"));
		line2->setLineEditText("0");
		line2->setBtnText(__TCPString("确认"));
		static int countValue2 = 0;
		line2->setLabelText(__TCPString("部件-2(%1)").arg(countValue2));
		line2->connectBtnClicked([=]() {
			countValue2 = countValue2 + line2->getLineEditText().toDouble();
			line2->setLabelText(__TCPString("部件-2(%1)").arg(countValue2));
			slotShapesMoveShape(m_cloneOtherMap, line2->getLineEditText().toDouble(), ShapeType::ShapeType_Clone_2, MoveDirection::MoveDirection_YAxis);
			foreach(QString shapeName, m_cloneOtherMap.keys())
			{
				if (m_cloneOtherMap[shapeName].isChanged)
				{
					m_cloneOtherMap[shapeName].isChanged = false;
					updateShapeModel(m_cloneOtherMap, shapeName);
				}
			}
			getContext()->UpdateCurrentViewer();

			});

		CLabLineEditBtn* line3 = new CLabLineEditBtn(widget);
		line3->setLabelText(__TCPString("部件-3"));
		line3->setLineEditText("0");
		line3->setBtnText(__TCPString("确认"));
		static int countValue3 = 0;
		line3->setLabelText(__TCPString("部件-3(%1)").arg(countValue3));
		line3->connectBtnClicked([=]() {
			countValue3 = countValue3 + line3->getLineEditText().toDouble();
			line3->setLabelText(__TCPString("部件-3(%1)").arg(countValue3));
			slotShapesMoveShape(m_cloneOtherMap, line3->getLineEditText().toDouble(), ShapeType::ShapeType_Clone_3, MoveDirection::MoveDirection_XAxis);
			foreach(QString shapeName, m_cloneOtherMap.keys())
			{
				if (m_cloneOtherMap[shapeName].isChanged)
				{
					m_cloneOtherMap[shapeName].isChanged = false;
					updateShapeModel(m_cloneOtherMap, shapeName);
				}
			}
			getContext()->UpdateCurrentViewer();

			});

		CLabLineEditBtn* line4 = new CLabLineEditBtn(widget);
		line4->setLabelText(__TCPString("部件-4"));
		line4->setLineEditText("0");
		line4->setBtnText(__TCPString("确认"));
		static int countValue4 = 0;
		line4->setLabelText(__TCPString("部件-4(%1)").arg(countValue4));
		line4->connectBtnClicked([=]() {
			countValue4 = countValue4 + line4->getLineEditText().toDouble();
			line4->setLabelText(__TCPString("部件-4(%1)").arg(countValue4));
			slotShapesMoveShape(m_cloneOtherMap, line4->getLineEditText().toDouble(), ShapeType::ShapeType_Clone_4, MoveDirection::MoveDirection_ZAxis);
			foreach(QString shapeName, m_cloneOtherMap.keys())
			{
				if (m_cloneOtherMap[shapeName].isChanged)
				{
					m_cloneOtherMap[shapeName].isChanged = false;
					updateShapeModel(m_cloneOtherMap, shapeName);
				}
			}
			getContext()->UpdateCurrentViewer();
			});

		CLabLineEditBtn* line5 = new CLabLineEditBtn(widget);
		line5->setLabelText(__TCPString("部件-5"));
		line5->setLineEditText("0");
		line5->setBtnText(__TCPString("确认"));
		static int countValue5 = 0;
		line5->setLabelText(__TCPString("部件-5(%1)").arg(countValue5));
		line5->connectBtnClicked([=]() {
			countValue5 = countValue5 + line5->getLineEditText().toDouble();
			line5->setLabelText(__TCPString("部件-5(%1)").arg(countValue5));
			slotShapesRotateShape(m_cloneOtherMap, line5->getLineEditText().toDouble(), ShapeType::ShapeType_Clone_5, MoveDirection::MoveDirection_ZAxis);
			foreach(QString shapeName, m_cloneOtherMap.keys())
			{
				if (m_cloneOtherMap[shapeName].isChanged)
				{
					m_cloneOtherMap[shapeName].isChanged = false;
					updateShapeModel(m_cloneOtherMap, shapeName);
				}
			}
			getContext()->UpdateCurrentViewer();
			});

		widget->addWidget(line);
		widget->addWidget(line2);
		widget->addWidget(line3);
		widget->addWidget(line4);
		widget->addWidget(line5);
		return widget;
	}
} // namespace TCP_ROBOT