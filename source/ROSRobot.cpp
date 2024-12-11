#include "../include/ROSRobot.h"

namespace TCP_ROBOT
{


	/**************************************** 构造/析构函数 ********************************************************/
	ROSRobot::ROSRobot(QWidget* parent)
	{
		LOG_FUNCTION_LINE_MESSAGE;
		initUI();
		initConnect();
	}
	ROSRobot::~ROSRobot()
	{
		LOG_FUNCTION_LINE_MESSAGE;
		if(m_tcpRobotCom)
		{
			delete m_tcpRobotCom;
			m_tcpRobotCom = nullptr;
		}
	}

	QWidget* ROSRobot::show7103ShapePreview(QWidget* parent)
	{
		LOG_FUNCTION_LINE_MESSAGE;
		QWidget* preview = new QWidget(parent);
		QHBoxLayout* mainLayout = new QHBoxLayout(preview);

		QTabWidget* tabWidget = new QTabWidget(preview);
		mainLayout->addWidget(tabWidget, 1);

		m_robotPreview->setParent(preview);

		tabWidget->addTab(showPreview(preview), __StandQString("工件配置"));
		tabWidget->addTab(showGuidePreview(preview), __StandQString("导轨配置"));
		tabWidget->addTab(showRobotPreview(preview), __StandQString("机器人配置"));
		tabWidget->addTab(m_robotCore->initZeroWidget(preview), __StandQString("零点设置"));
		mainLayout->addWidget(m_robotPreview, 5);
		m_robotPreview->resize(600, 600);

		return preview;
	}

	QWidget* ROSRobot::showPreview(QWidget* parent)
	{
		LOG_FUNCTION_LINE_MESSAGE;
		QWidget* preview = new QWidget(parent);
		preview->setWindowTitle(__StandQString("工件预览"));
		/*********************************************************/
		/***** 列表******/  /*****工件显示*****/
		/**********/
		// 主布局 QH
		QHBoxLayout* mainLayout = new QHBoxLayout(preview);

		// 左侧布局 QV
		QWidget* leftWidget = new QWidget(preview);

		QVBoxLayout* layLeft = new QVBoxLayout(leftWidget);
		layLeft->addWidget(new QLabel(__StandQString("工件列表:")));
		QListWidget* workListWidget = new QListWidget(leftWidget);
		workListWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
		workListWidget->setViewMode(QListView::ListMode);
		layLeft->addWidget(workListWidget);
		//leftWidget->setFixedWidth(200);

		QHBoxLayout* layoutButton = new QHBoxLayout(leftWidget);
		QPushButton* seletedWorkButton = new QPushButton(leftWidget);
		seletedWorkButton->setText(__StandQString("选择"));

		QPushButton* addButton = new QPushButton(leftWidget);
		addButton->setText(__StandQString("添加"));

		QPushButton* deleteButton = new QPushButton(leftWidget);
		deleteButton->setText(__StandQString("删除"));

		QPushButton* saveButton = new QPushButton(leftWidget);
		saveButton->setText(__StandQString("保存"));
		//saveButton->setEnabled(false);

		layoutButton->addWidget(seletedWorkButton);
		layoutButton->addWidget(addButton);
		layoutButton->addWidget(deleteButton);
		layoutButton->addWidget(saveButton);
		layLeft->addLayout(layoutButton);

		mainLayout->addWidget(leftWidget, 1);

		// 右侧布局 QV
		QWidget* rightWidget = new QWidget(preview);
		mainLayout->addWidget(rightWidget, 1);
		QVBoxLayout* layRight = new QVBoxLayout(rightWidget);
		ShapeCommondPreview* preViewWork = new ShapeCommondPreview(rightWidget);
		preViewWork->setShapeType(ShapeType_Work);
		layRight->addWidget(preViewWork);
		QWidget* leftHoleWidget = new QWidget(rightWidget);
		layRight->addWidget(leftHoleWidget);

		QHBoxLayout* layLeftHoleMain = new QHBoxLayout(leftHoleWidget);

		QVBoxLayout* layLeftHole = new QVBoxLayout(leftHoleWidget);
		layLeftHole->addWidget(new QLabel(__StandQString("焊缝列表:")));
		QListWidget* holeListWidget = new QListWidget(leftHoleWidget);
		holeListWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
		holeListWidget->setViewMode(QListView::ListMode);
		layLeftHole->addWidget(holeListWidget);

		QHBoxLayout* layoutButtonHole = new QHBoxLayout(leftHoleWidget);
		QPushButton* addHoleButton = new QPushButton(leftHoleWidget);
		addHoleButton->setText(__StandQString("添加焊缝"));

		QPushButton* deleteHoleButton = new QPushButton(leftHoleWidget);
		deleteHoleButton->setText(__StandQString("删除焊缝"));

		// 保存按钮
		QPushButton* saveButtonHole = new QPushButton(leftHoleWidget);
		saveButtonHole->setText(__StandQString("保存焊缝"));
		saveButtonHole->setEnabled(false);

		layoutButtonHole->addWidget(addHoleButton);
		layoutButtonHole->addWidget(deleteHoleButton);
		layoutButtonHole->addWidget(saveButtonHole);
		layLeftHole->addLayout(layoutButtonHole);


		layLeftHoleMain->addLayout(layLeftHole, 1);

		ShapeCommondPreview* preViewHole = new ShapeCommondPreview(leftHoleWidget);
		preViewHole->setShapeType(ShapeType_Hole);
		layLeftHoleMain->addWidget(preViewHole, 1);

		// 连杆设置
		preViewHole->setLinkIsVisable(false);
		preViewWork->setLinkIsVisable(false);

		/*RobotPreview* robotPreview = new RobotPreview(rightWidget);
		mainLayout->addWidget(robotPreview, 5);*/

		preViewHole->setRobotPreviewPoint(m_robotPreview);
		preViewWork->setRobotPreviewPoint(m_robotPreview);

		{
			// 读取工件列表
			QDir dir(WORKPATH);
			dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
			QStringList workList = dir.entryList();
			foreach(QString workName, workList)
			{
				QListWidgetItem* item = new QListWidgetItem(workName);
				workListWidget->addItem(item);
			}
		}

		// 信号链接
		connect(workListWidget, &QListWidget::itemClicked, [=](QListWidgetItem* item) {
			// 读取文件
			m_shapeMap.clear();
			// robotPreview->removeAllPreview();
			QString fileName = WORKPATH.append("/").append(item->text()).append("/").append(WORKCONFIGPATH);
			QFile file(fileName);
			qDebug() << "工件配置文件:" << fileName;
			if (file.open(QIODevice::ReadOnly))
			{
				QTextStream in(&file);
				QJsonDocument doc = QJsonDocument::fromJson(in.readAll().toUtf8());
				QVariantMap variantMap = doc.toVariant().toMap();
				if (variantMap.isEmpty())
				{
					qDebug() << "工件配置文件为空";
					//打印输出
					for (auto it = variantMap.begin(); it != variantMap.end(); ++it)
					{
						qDebug() << it.key() << it.value();
					}
					return;
				}
				// 工件更新
				SHAPESTRUCT shapeStruct;
				shapeStruct.setShapeVariantMap(variantMap[WORKPATHNAME].toMap());
				preViewWork->setShapeStruct(shapeStruct);
				m_shapeMap.insert(item->text(), shapeStruct);
				// 焊缝 
				holeListWidget->clear();
				foreach(QString key, variantMap.keys())
				{
					// 工件更新
					SHAPESTRUCT shapeStructHole;
					shapeStructHole.setShapeVariantMap(variantMap[key].toMap());
					if (shapeStructHole.shapeType == ShapeType_Hole)
					{
						QListWidgetItem* item = new QListWidgetItem(shapeStructHole.ShapeName);
						holeListWidget->addItem(item);
						m_shapeMap.insert(item->text(), shapeStructHole);
						preViewHole->setShapeStruct(shapeStructHole);
					}
				}
				preViewHole->setShapeStruct(SHAPESTRUCT());

				file.close();

			}
			saveButton->setEnabled(true);

			});
		connect(addButton, &QPushButton::clicked, [=]() {
			QString workName = __StandQString("Work_").append(QString::number(workListWidget->count() + 1));
			// 创建文件夹
			QDir().mkdir(WORKPATH.append("/").append(workName));
			// 创建文件
			QFile file(WORKPATH.append("/").append(workName).append("/").append(WORKCONFIGPATH));

			QVariantMap variantMap;
			SHAPESTRUCT shapeStruct;
			shapeStruct.shapeType = ShapeType_Work;
			shapeStruct.ShapeName = workName;
			variantMap.insert(WORKPATHNAME, shapeStruct.getShapeVariantMap());

			if (file.open(QIODevice::WriteOnly))
			{
				QTextStream out(&file);
				QJsonDocument doc(QJsonObject::fromVariantMap(variantMap));
				out << doc.toJson();
				file.close();
			}

			// 刷新列表
			QListWidgetItem* item = new QListWidgetItem(workName);
			workListWidget->addItem(item);

			// 刷新预览
			holeListWidget->clear();
			preViewWork->setShapeStruct(SHAPESTRUCT());
			preViewHole->setShapeStruct(SHAPESTRUCT());
			//	
			m_shapeMap.insert(workName, shapeStruct);
			});
		connect(deleteButton, &QPushButton::clicked, [=]() {
			// 删除文件夹
			QDir().rmdir(WORKPATH.append("/").append(workListWidget->currentItem()->text()));
			// 删除列表项
			int row = workListWidget->currentRow();
			workListWidget->takeItem(row);

			// 刷新预览
			holeListWidget->clear();
			preViewWork->setShapeStruct(SHAPESTRUCT());
			preViewHole->setShapeStruct(SHAPESTRUCT());
			//	
			/*if (!workListWidget->currentItem())
			{
				if (m_shapeMap.contains(workListWidget->currentItem()->text()))
				{
					m_shapeMap.remove(workListWidget->currentItem()->text());
				}

			}*/


			});

		connect(seletedWorkButton, &QPushButton::clicked, [=]() {
			m_currentWork = workListWidget->currentItem()->text();

			// 判断路径是否存在
			if (!QDir().exists(WORKPATH.append("/").append(m_currentWork)))
			{
				// 创建文件夹
				QDir().mkdir(WORKPATH.append("/").append(m_currentWork));
				// 创建文件
				QFile file(WORKPATH.append("/").append("CurrentWork.json"));

				QVariantMap variantMap;
				variantMap.insert("CurrentWork", m_currentWork);
				if (file.open(QIODevice::WriteOnly))
				{
					QTextStream out(&file);
					QJsonDocument doc(QJsonObject::fromVariantMap(variantMap));
					out << doc.toJson();
					file.close();
				}
			}
			else
			{
				QFile file(WORKPATH.append("/").append("CurrentWork.json"));

				QVariantMap variantMap;
				variantMap.insert("CurrentWork", m_currentWork);
				if (file.open(QIODevice::WriteOnly))
				{
					QTextStream out(&file);
					QJsonDocument doc(QJsonObject::fromVariantMap(variantMap));
					out << doc.toJson();
					file.close();
				}
			}



			m_robotCore->loadWorkShapes(SHAPEMODEPATH(m_currentWork));
			m_robotCore->loadRobotShape(ROBOTPATH.append("/").append(ROBOTCONFIGPATH));
			m_robotCore->loadOtherShape(OTHERPATH.append("/").append(OTERDATAPATH));

			emit seletedWorkChanged(workListWidget->currentItem()->text());
			});
		connect(saveButton, &QPushButton::clicked, [=]() {

			QString key = workListWidget->currentItem()->text();
			QString fileName = WORKPATH.append("/").append(key).append("/").append(WORKCONFIGPATH);
			QFile file(fileName);
			QVariantMap variantMap;
			m_shapeMap.insert(key, preViewWork->getShapeStruct());
			preViewWork->getShapeStruct().ShapeAngleX;

			SHAPESTRUCT shapeWorkStruct;
			QStringList keys = QStringList();
			foreach(QString key, m_shapeMap.keys())
			{
				SHAPESTRUCT shapeStruct = m_shapeMap[key];
				if (shapeStruct.shapeType == ShapeType_Work)
				{
					shapeWorkStruct = shapeStruct;
					break;
				}
			}
			foreach(QString key, m_shapeMap.keys())
			{
				SHAPESTRUCT shapeStruct = m_shapeMap[key];
				if (shapeStruct.shapeType == ShapeType_Work)
				{
					variantMap.insert(WORKPATHNAME, shapeStruct.getShapeVariantMap());
				}
				else
				{
					variantMap.insert(key, shapeStruct.getShapeVariantMap());
					keys.append(key);
				}

			}
			shapeWorkStruct.nextShapeNames = keys;
			variantMap.insert(WORKPATHNAME, shapeWorkStruct.getShapeVariantMap());
			// 保存文件
			if (file.open(QIODevice::WriteOnly))
			{
				QJsonDocument doc(QJsonObject::fromVariantMap(variantMap));
				QTextStream out(&file);
				out << doc.toJson();
			}
			file.close();
			});

		connect(holeListWidget, &QListWidget::itemClicked, [=](QListWidgetItem* item) {
			preViewHole->setShapeStruct(m_shapeMap[item->text()]);
			saveButtonHole->setEnabled(true);
			});
		connect(addHoleButton, &QPushButton::clicked, [=]() {
			QString workName = __StandQString("Hole_").append(QString::number(holeListWidget->count() + 1));
			holeListWidget->addItem(new QListWidgetItem(workName));
			SHAPESTRUCT shapeStruct;
			shapeStruct.shapeType = ShapeType_Hole;
			shapeStruct.ShapeName = workName;
			preViewHole->setShapeStruct(shapeStruct);
			m_shapeMap.insert(workName, shapeStruct);


			});
		connect(deleteHoleButton, &QPushButton::clicked, [=]() {
			// 刷新预览
			preViewHole->setShapeStruct(SHAPESTRUCT());
			m_shapeMap.remove(holeListWidget->currentItem()->text());
			saveButtonHole->setEnabled(false);

			// 删除列表项
			int row = holeListWidget->currentRow();
			holeListWidget->takeItem(row);
			});

		connect(saveButtonHole, &QPushButton::clicked, [=]() {
			m_shapeMap.insert(holeListWidget->currentItem()->text(), preViewHole->getShapeStruct());
			});

		return preview;
	}

	QWidget* ROSRobot::showRobotPreview(QWidget* parent)
	{
		LOG_FUNCTION_LINE_MESSAGE;
		QWidget* preview = new QWidget(parent);
		preview->setWindowTitle(__StandQString("机器人预览"));
		/**********/
		/***** 列表******/  /*****工件显示*****/
		/**********/
		// 主布局 QH
		QHBoxLayout* mainLayout = new QHBoxLayout(preview);

		// 左侧布局 QV
		QWidget* leftWidget = new QWidget(preview);

		QVBoxLayout* layLeft = new QVBoxLayout(leftWidget);
		layLeft->addWidget(new QLabel(__StandQString("机器人列表:")));
		QListWidget* robotListWidget = new QListWidget(leftWidget);
		robotListWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
		robotListWidget->setViewMode(QListView::ListMode);
		layLeft->addWidget(robotListWidget);

		QHBoxLayout* layoutButton = new QHBoxLayout(leftWidget);

		QPushButton* seletedRobotButton = new QPushButton(leftWidget);
		seletedRobotButton->setText(__StandQString("应用"));

		QPushButton* addButton = new QPushButton(leftWidget);
		addButton->setText(__StandQString("添加部件"));

		QPushButton* deleteButton = new QPushButton(leftWidget);
		deleteButton->setText(__StandQString("删除部件"));

		QPushButton* saveButton = new QPushButton(leftWidget);
		saveButton->setText(__StandQString("保存"));
		//saveButton->setEnabled(false);

		layoutButton->addWidget(seletedRobotButton);
		layoutButton->addWidget(addButton);
		layoutButton->addWidget(deleteButton);
		layoutButton->addWidget(saveButton);
		layLeft->addLayout(layoutButton);

		mainLayout->addWidget(leftWidget, 1);

		// 右侧布局 QV
		QWidget* rightWidget = new QWidget(preview);
		mainLayout->addWidget(rightWidget, 1);

		QVBoxLayout* layRight = new QVBoxLayout(rightWidget);

		ShapeCommondPreview* preViewRobot = new ShapeCommondPreview(rightWidget);
		preViewRobot->setShapeType(ShapeType_Robot);
		preViewRobot->setLinkIsVisable(true);
		layRight->addWidget(preViewRobot);

		/*RobotPreview* robotPreview = new RobotPreview(rightWidget);
		mainLayout->addWidget(robotPreview, 6);*/

		preViewRobot->setRobotPreviewPoint(m_robotPreview);
		rightWidget->setEnabled(false);
		saveButton->setEnabled(false);

		{
			QVariantMap variantMap;
			// 读取机器人列表
			createOrCheckDirectory(ROBOTPATH);
			QString fileName = ROBOTPATH.append("/").append(ROBOTCONFIGPATH);
			QFile file(fileName);
			qDebug() << "机器人配置文件:" << fileName;
			if (file.open(QIODevice::ReadOnly))
			{
				QTextStream in(&file);
				QJsonDocument doc = QJsonDocument::fromJson(in.readAll().toUtf8());
				QVariantMap variantMap = doc.toVariant().toMap();
				if (!variantMap.isEmpty())
				{

					// 部件 
					foreach(QString key, variantMap.keys())
					{
						SHAPESTRUCT shapeStruct;
						shapeStruct.setShapeVariantMap(variantMap[key].toMap());
						m_robotMap.insert(key, shapeStruct);
						preViewRobot->setShapeStruct(shapeStruct);

						QListWidgetItem* item = new QListWidgetItem(key);
						robotListWidget->addItem(item);
					}
					file.close();
				}
			}
		}
		connect(robotListWidget, &QListWidget::itemClicked, [=](QListWidgetItem* item) {
			// 机器人显示
			preViewRobot->setShapeStruct(m_robotMap[item->text()]);
			rightWidget->setEnabled(true);
			saveButton->setEnabled(true);
			});

		connect(addButton, &QPushButton::clicked, [=]() {
			QString robotName = __StandQString("Robot_").append(QString::number(robotListWidget->count() + 1));
			robotListWidget->addItem(new QListWidgetItem(robotName));
			SHAPESTRUCT shapeStruct;
			shapeStruct.shapeType = ShapeType_Robot;
			shapeStruct.ShapeName = robotName;
			preViewRobot->setShapeStruct(shapeStruct);
			m_robotMap.insert(robotName, shapeStruct);
			});

		connect(deleteButton, &QPushButton::clicked, [=]() {
			// 刷新预览
			preViewRobot->setShapeStruct(SHAPESTRUCT());
			m_robotMap.remove(robotListWidget->currentItem()->text());
			// 删除列表项
			int row = robotListWidget->currentRow();
			robotListWidget->takeItem(row);


			});

		connect(saveButton, &QPushButton::clicked, [=]() {
			// 保存
			m_robotMap.insert(robotListWidget->currentItem()->text(), preViewRobot->getShapeStruct());
			// 保存文件
			createOrCheckDirectory(ROBOTPATH);
			QString fileName = ROBOTPATH.append("/").append(ROBOTCONFIGPATH);
			QFile file(fileName);
			QVariantMap variantMap;
			foreach(QString key, m_robotMap.keys())
			{
				dealWithShapesPararmeter(m_robotMap);
				SHAPESTRUCT shapeStruct = m_robotMap[key];
				variantMap.insert(key, shapeStruct.getShapeVariantMap());
			}
			if (file.open(QIODevice::WriteOnly))
			{
				QJsonDocument doc(QJsonObject::fromVariantMap(variantMap));
				QTextStream out(&file);
				out << doc.toJson();
			}
			file.close();
			});

		connect(seletedRobotButton, &QPushButton::clicked, [=]() {
			m_robotCore->loadWorkShapes(SHAPEMODEPATH(m_currentWork));
			m_robotCore->loadRobotShape(ROBOTPATH.append("/").append(ROBOTCONFIGPATH));
			m_robotCore->loadOtherShape(OTHERPATH.append("/").append(OTERDATAPATH));
			});

		return preview;
	}

	QWidget* ROSRobot::showGuidePreview(QWidget* parent)
	{
		LOG_FUNCTION_LINE_MESSAGE;
		QWidget* preview = new QWidget(parent);
		preview->setWindowTitle(__StandQString("导轨配置"));

		QTabWidget* tabWidget = new QTabWidget(preview);

		ShapeCommondPreview* preViewLong = new ShapeCommondPreview(preview);
		preViewLong->setShapeType(ShapeType_LongGuide);
		preViewLong->setLinkIsVisable(false);

		tabWidget->addTab(preViewLong, __StandQString("长轴"));

		ShapeCommondPreview* preViewShort = new ShapeCommondPreview(preview);
		preViewShort->setShapeType(ShapeType_ShortGuide);
		preViewShort->setLinkIsVisable(false);



		tabWidget->addTab(preViewShort, __StandQString("短轴"));

		ShapeCommondPreview* preViewRotation = new ShapeCommondPreview(preview);
		preViewRotation->setShapeType(ShapeType_RotatingTable);
		preViewRotation->setLinkIsVisable(false);

		tabWidget->addTab(preViewRotation, __StandQString("转盘"));

		preViewLong->setRobotPreviewPoint(m_robotPreview);
		preViewShort->setRobotPreviewPoint(m_robotPreview);
		preViewRotation->setRobotPreviewPoint(m_robotPreview);

		QVBoxLayout* mainLayout = new QVBoxLayout(preview);
		mainLayout->addWidget(tabWidget);

		QPushButton* saveButton = new QPushButton(preview);
		saveButton->setText(__StandQString("保存"));
		mainLayout->addWidget(saveButton);
		{
			QVariantMap variantMap;
			// 读取导轨列表
			createOrCheckDirectory(OTHERPATH);
			QString fileName = OTHERPATH.append("/").append(OTERDATAPATH);
			QFile file(fileName);
			qDebug() << "导轨配置文件:" << fileName;
			if (file.open(QIODevice::ReadOnly))
			{
				QTextStream in(&file);
				QJsonDocument doc = QJsonDocument::fromJson(in.readAll().toUtf8());
				QVariantMap variantMap = doc.toVariant().toMap();
				if (!variantMap.isEmpty())
				{
					// 长轴
					if (variantMap.contains("long-guide"))
					{
						SHAPESTRUCT shapeStruct;
						shapeStruct.setShapeVariantMap(variantMap["long-guide"].toMap());
						shapeStruct.shapeType = ShapeType_LongGuide;
						shapeStruct.ShapeName = "long-guide";
						preViewLong->setShapeStruct(shapeStruct);
					}
					// 短轴
					if (variantMap.contains("short-guide"))
					{
						SHAPESTRUCT shapeStruct;
						shapeStruct.setShapeVariantMap(variantMap["short-guide"].toMap());
						shapeStruct.shapeType = ShapeType_ShortGuide;
						shapeStruct.ShapeName = "short-guide";

						preViewShort->setShapeStruct(shapeStruct);
					}
					// 转盘
					if (variantMap.contains("rotating-table"))
					{
						SHAPESTRUCT shapeStruct;
						shapeStruct.setShapeVariantMap(variantMap["rotating-table"].toMap());
						shapeStruct.shapeType = ShapeType_RotatingTable;
						shapeStruct.ShapeName = "rotating-table";
						preViewRotation->setShapeStruct(shapeStruct);
					}
					file.close();
				}
			}
			else
			{
				SHAPESTRUCT shapeStruct;
				shapeStruct.shapeType = ShapeType_LongGuide;
				shapeStruct.ShapeName = "long-guide";
				preViewLong->setShapeStruct(shapeStruct);

				shapeStruct.shapeType = ShapeType_ShortGuide;
				shapeStruct.ShapeName = "short-guide";
				preViewShort->setShapeStruct(shapeStruct);

				shapeStruct.shapeType = ShapeType_RotatingTable;
				shapeStruct.ShapeName = "rotating-table";
				preViewRotation->setShapeStruct(shapeStruct);
			}

		}

		connect(saveButton, &QPushButton::clicked, [=]() {
			QVariantMap variantMap;
			SHAPESTRUCT longShapeStruct = preViewLong->getShapeStruct();
			SHAPESTRUCT shortShapeStruct = preViewShort->getShapeStruct();
			SHAPESTRUCT rotationShapeStruct = preViewRotation->getShapeStruct();

			if (!longShapeStruct.nextShapeNames.contains(shortShapeStruct.ShapeName))
			{
				longShapeStruct.nextShapeNames.append(shortShapeStruct.ShapeName);
			}
			if (!shortShapeStruct.nextShapeNames.contains(rotationShapeStruct.ShapeName))
			{
				shortShapeStruct.nextShapeNames.append(rotationShapeStruct.ShapeName);
			}

			variantMap.insert(longShapeStruct.ShapeName, longShapeStruct.getShapeVariantMap());
			variantMap.insert(shortShapeStruct.ShapeName, shortShapeStruct.getShapeVariantMap());
			variantMap.insert(rotationShapeStruct.ShapeName, rotationShapeStruct.getShapeVariantMap());

			createOrCheckDirectory(OTHERPATH);
			QString fileName = OTHERPATH.append("/").append(OTERDATAPATH);
			QFile file(fileName);

			if (file.open(QIODevice::WriteOnly))
			{
				QJsonDocument doc(QJsonObject::fromVariantMap(variantMap));
				QTextStream out(&file);
				out << doc.toJson();
			}
			file.close();
			});

		return preview;
	}

	QWidget* ROSRobot::showTable(QWidget* parent)
	{
		LOG_FUNCTION_LINE_MESSAGE;
		QWidget* table = new QWidget(parent);
		QVBoxLayout* mainLayout = new QVBoxLayout(table);
		LabelComboBox* labelComboBox = new LabelComboBox(table);
		labelComboBox->setLabelText(__StandQString("选择工件:"));
		labelComboBox->setItems(getSubDirectories(WORKPATH));
		labelComboBox->setCurrentItemText(__StandQString("NULL"));
		mainLayout->addWidget(labelComboBox);

		connect(labelComboBox->currentItem(), &QComboBox::currentTextChanged, [=](const QString& text) {
			m_currentWork = text;
			QVariantMap variantMap;
			variantMap.insert("CurrentWork", text);
			saveToJsonFile(variantMap, WORKPATH + "/" + "CurrentWork.json");
			emit seletedWorkChanged(text);
			});

		labelComboBox->setCurrentItemText(m_currentWork);

		QLabel* label = new QLabel(table);
		label->setText(__StandQString("工艺信息"));
		m_robot7103Grid->setParent(table);
		mainLayout->addWidget(label);
		mainLayout->addWidget(m_robot7103Grid);

		return table;
	}

	QWidget* ROSRobot::showCoreRobot(QWidget* parent)
	{
		LOG_FUNCTION_LINE_MESSAGE;
		QWidget* robotCore = new QWidget(parent);
		robotCore->setWindowTitle(__StandQString("核心显示"));
		QVBoxLayout* mainLayout = new QVBoxLayout(robotCore);
		m_robotCore->setParent(robotCore);
		m_robotCore->loadWorkShapes(SHAPEMODEPATH(m_currentWork));
		m_robotCore->loadRobotShape(ROBOTPATH.append("/").append(ROBOTCONFIGPATH));
		m_robotCore->loadOtherShape(OTHERPATH.append("/").append(OTERDATAPATH));
		mainLayout->addWidget(m_robotCore);
		//initConnect();
		return robotCore;
	}

	QWidget* ROSRobot::showTeaching(QWidget* parent)
	{
		LOG_FUNCTION_LINE_MESSAGE;
		CWidgetVLay *teaching = new CWidgetVLay(parent);
		teaching->setWindowTitle(__StandQString("示教界面"));
		if (!m_teaching)
		{
			m_teaching->close();
			
		}
		m_teaching = new RobotoDemonstrator(parent);
		teaching->addWidget(m_teaching);
		//connect(this, &ROSRobot::seletedWorkChanged, m_teaching, &RobotoDemonstrator::slotSeletedWorkChanged);
		connect(m_teaching, &RobotoDemonstrator::sendWorkAndHole, m_robot7103Grid, &Robot7103Grid::slotAddGrid);
		connect(m_teaching, &RobotoDemonstrator::removeGrid, m_robot7103Grid, &Robot7103Grid::slotRemoveGrid);
		if (m_teaching != nullptr)
		{
			m_teaching->setTcpCommunication(m_tcpRobotCom);
		}
		
		return teaching;
	}

	void ROSRobot::setCommunicationPointer(TcpRobotCommunication* tcpRobotCom)
	{
		LOG_FUNCTION_LINE_MESSAGE;
		m_tcpRobotCom = tcpRobotCom;

		initConnect();
	}

	void ROSRobot::setIpAndPort(const QString& ip, const int& port)
	{
		LOG_FUNCTION_LINE_MESSAGE;
		if (m_tcpRobotCom == nullptr)
		{
			m_tcpRobotCom = new TcpRobotCommunication(this);
			m_tcpRobotCom->setIPAndPort(ip, port);
			initConnect();
			return;
		}
		m_tcpRobotCom->setIPAndPort(ip, port);
		if (m_robot7103Grid != nullptr)
		{
			m_robot7103Grid->setTcpCommunication(m_tcpRobotCom);
		}
		if (m_teaching != nullptr)
		{
			m_teaching->setTcpCommunication(m_tcpRobotCom);
		}
	}


	void ROSRobot::initUI()
	{
		LOG_FUNCTION_LINE_MESSAGE;
		// 读取工件列表
		createOrCheckDirectory(WORKPATH);
		QString fileName = WORKPATH.append("/").append("CurrentWork.json");
		QFile file(fileName);
		qDebug() << "工件配置文件:" << fileName;
		if (file.open(QIODevice::ReadOnly))
		{
			QTextStream in(&file);
			QJsonDocument doc = QJsonDocument::fromJson(in.readAll().toUtf8());
			QVariantMap variantMap = doc.toVariant().toMap();
			if (!variantMap.isEmpty())
			{
				// 工件
				if (variantMap.contains("CurrentWork"))
				{
					QString workName = variantMap["CurrentWork"].toString();
					m_currentWork = workName;
				}
				file.close();
			}
		}

		m_tcpRobotCom = new TcpRobotCommunication(this);
		m_tcpRobotCom->setIPAndPort("127.0.0.1", 8080);
		// 设置重发间隔
		m_tcpRobotCom->setRollInterval(5000);
		// 模型预览
		m_robotPreview = new RobotPreview(this);
		// 模型核心
		m_robotCore = new RobotCore(this);
		// 表格
		m_robot7103Grid = new Robot7103Grid(this);
		// 示教
		m_teaching = new RobotoDemonstrator(this);

		initConnect();
	}

	void ROSRobot::initConnect()
	{

		LOG_FUNCTION_LINE_MESSAGE;
		/*disconnect(m_robot7103Grid, &Robot7103Grid::slotSeletedWorkChanged, this, &ROSRobot::seletedWorkChanged);
		disconnect(m_robotCore, &RobotCore::slotSeletedWorkChanged, this, &ROSRobot::seletedWorkChanged);
		disconnect(m_teaching, &RobotoDemonstrator::slotSeletedWorkChanged, this, &ROSRobot::seletedWorkChanged);
		disconnect(m_teaching, &RobotoDemonstrator::sendWorkAndHole, m_robot7103Grid, &Robot7103Grid::slotAddGrid);
		disconnect(m_robot7103Grid, &Robot7103Grid::signalChangeShapeColor, m_robotCore, &RobotCore::slotChangeShapeColor);
		disconnect(m_standFrame, &TCPXVIEWBASE_NAMESPACE::StandFrame::signalReciveValue, m_robotCore, &RobotCore::slotShapMove);*/

		connect(this, &ROSRobot::seletedWorkChanged, m_robot7103Grid, &Robot7103Grid::slotSeletedWorkChanged);
		connect(this, &ROSRobot::seletedWorkChanged, m_robotCore, &RobotCore::slotSeletedWorkChanged);
		connect(this, &ROSRobot::seletedWorkChanged, m_teaching, &RobotoDemonstrator::slotSeletedWorkChanged);
		connect(m_teaching, &RobotoDemonstrator::sendWorkAndHole, m_robot7103Grid, &Robot7103Grid::slotAddGrid);
		connect(m_robot7103Grid, &Robot7103Grid::signalChangeShapeColor, m_robotCore, &RobotCore::slotChangeShapeColor);

		connect(m_standFrame, &TCPXVIEWBASE_NAMESPACE::StandFrame::signalReciveValue, m_robotCore, &RobotCore::slotShapMove);

		ISNULLPOINTER(m_tcpRobotCom);
		m_standFrame = m_tcpRobotCom->getStandFrame("PS");
		ISNULLPOINTER(m_standFrame);
		connect(m_standFrame, &TCPXVIEWBASE_NAMESPACE::StandFrame::signalReciveValue, m_robotCore, &RobotCore::slotShapMove);
		/*connect(m_standFrame, &TCPXVIEWBASE_NAMESPACE::StandFrame::signalReciveValue, [=](QString data) {
			qDebug() << "GO RECEIVE DATA:" << data;
			});*/
		if (m_robot7103Grid != nullptr)
		{
			m_robot7103Grid->setTcpCommunication(m_tcpRobotCom);
		}
		if (m_teaching != nullptr)
		{
			m_teaching->setTcpCommunication(m_tcpRobotCom);
		}
	}

	void ROSRobot::createOrCheckDirectory(const QString& path)
	{
		LOG_FUNCTION_LINE_MESSAGE;
		if (!QDir(path).exists())
		{
			qDebug() << "创建文件夹" << path;
			QDir().mkpath(path);
		}
	}

	void ROSRobot::dealWithShapesPararmeter(QMap<QString, SHAPESTRUCT>& shapesMap)
	{
		LOG_FUNCTION_LINE_MESSAGE;
		// 根据序列处理 nextShapeNames

		// 遍历 shapesMap 查找类型为 Robot

		// 排序
		QMap <int, QVector<SHAPESTRUCT>> tempMap;
		foreach(QString key, shapesMap.keys())
		{
			SHAPESTRUCT shapeStruct = shapesMap[key];
			if (shapeStruct.shapeType != ShapeType_Robot ||
				shapeStruct.ShapeLinkIndex == -1)
			{
				continue;
			}
			// 如果找到索引
			if (tempMap.keys().contains(shapeStruct.ShapeLinkIndex))
			{
				tempMap[shapeStruct.ShapeLinkIndex].push_back(shapeStruct);
			}
			else
			{
				QVector<SHAPESTRUCT> tempVec;
				tempVec.push_back(shapeStruct);
				tempMap.insert(shapeStruct.ShapeLinkIndex, tempVec);
			}
		}

		// 处理并插值
		QMap<QString, SHAPESTRUCT> resultMap;

		// DH 参数 列表
		QVector<QVector<double>> DHVec;

		// DH 类
		SDHRobot* robot = new SDHRobot();

		// 遍历排序后的map
		for (auto it = tempMap.begin(); it != tempMap.end(); ++it)
		{
			QVector<SHAPESTRUCT> vec = it.value();
			for (int i = 0; i < vec.size(); i++)
			{
				// DH 参数
				QVector<double> DH;
				QMap<QString, SHAPELINKDATA> linkData = vec[i].shapeLinkData;

				QMap<int, SHAPELINKDATA> linkDataSorted;
				foreach(QString key, linkData.keys())
				{
					SHAPELINKDATA link = linkData[key];
					if (link.index == -1)
					{
						continue;
					}
					else
					{
						linkDataSorted.insert(link.index, link);
					}
				}
				// 排序完成
				for (auto it2 = linkDataSorted.begin(); it2 != linkDataSorted.end(); ++it2)
				{
					SHAPELINKDATA link = it2.value();
					if (link.index == -1)
					{
						continue;
					}
					else
					{
						DHVec.push_back(link.getDHParameter());
					}
				}
				robot->setDHParameters(DHVec);

				if (vec[i].ShapeLink)
				{
					// poxisitionX
					QVector<double> position = robot->getCurrentExtractPosition(DHVec.size());

					// 处理位置
					vec[i].ShapePositionX = QString::number(position[0]);
					vec[i].ShapePositionY = QString::number(position[1]);
					vec[i].ShapePositionZ = QString::number(position[2]);

					QVector<double> angle = robot->getCurrentExtractRotation(DHVec.size());
					vec[i].ShapeAngleX = QString::number(angle[0]);
					vec[i].ShapeAngleY = QString::number(angle[1]);
					vec[i].ShapeAngleZ = QString::number(angle[2]);

				}
				// 下一个节点拼接
				auto tempIt = it + 1;
				if (tempIt != tempMap.end())
				{
					for (int j = 0; j < tempIt.value().size(); j++)
					{
						if (!vec[i].nextShapeNames.contains(tempIt.value()[j].ShapeName))
						{
							vec[i].nextShapeNames.append(tempIt.value()[j].ShapeName);
						}
					}
				}
				resultMap.insert(vec[i].ShapeName, vec[i]);
			}
		}
		shapesMap = resultMap;
	}

	bool ROSRobot::removeDirectory(const QString& path)
	{
		LOG_FUNCTION_LINE_MESSAGE;
		QDir dir(path);
		return dir.removeRecursively();
	}

	QStringList ROSRobot::getSubDirectories(const QString& path)
	{
		LOG_FUNCTION_LINE_MESSAGE;
		QDir dir(path);
		return dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
	}

	bool ROSRobot::saveToJsonFile(const QVariantMap& data, const QString& filePath)
	{
		LOG_FUNCTION_LINE_MESSAGE;
		QJsonDocument doc = QJsonDocument::fromVariant(data);
		QFile file(filePath);
		if (file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QTextStream stream(&file);
			stream << doc.toJson();
			file.close();
			return true;
		}
		return false;
	}



	ShapeCommondPreview::ShapeCommondPreview(QWidget* parent)
	{
		LOG_FUNCTION_LINE_MESSAGE;
		QHBoxLayout* mainLayout = new QHBoxLayout(this);

		QVBoxLayout* leftLayout = new QVBoxLayout(this);
		mainLayout->addLayout(leftLayout, 1);



		QLabel* labelName = new QLabel(this);
		labelName->setText(__StandQString("模型名称:"));
		m_shapeName = new QLineEdit(this);
		m_shapeName->setText(m_shapeStruct.ShapeName);

		QHBoxLayout* modeNameLayout = new QHBoxLayout(this);
		modeNameLayout->addWidget(labelName, 1);
		modeNameLayout->addWidget(m_shapeName, 3);
		leftLayout->addLayout(modeNameLayout);

		// 当前 模型索引
		QHBoxLayout* linkIndexLayout = new QHBoxLayout(m_linkWidget);
		QLabel* labelLinkIndexT = new QLabel(m_linkWidget);
		labelLinkIndexT->setText(__StandQString("模型索引:"));
		linkIndexLayout->addWidget(labelLinkIndexT, 1);
		m_shapIndex = new QLineEdit(m_linkWidget);
		m_shapIndex->setReadOnly(false);
		linkIndexLayout->addWidget(m_shapIndex, 3);

		leftLayout->addLayout(linkIndexLayout);

		//// 模型类型
		//QHBoxLayout* shapeTypeLayout = new QHBoxLayout(this);
		//QLabel* labelType = new QLabel(this);
		//labelType->setText(__StandQString("预设类型:"));
		//m_shapeTypeComboBox = new QComboBox(this);
		//shapeTypeLayout->addWidget(labelType, 1);
		//shapeTypeLayout->addWidget(m_shapeTypeComboBox, 3);
		//leftLayout->addLayout(shapeTypeLayout);



		// 模型路径
		QLabel* labelPath = new QLabel(this);
		labelPath->setText(__StandQString("模型路径:"));
		m_shapePath = new QLineEdit(this);
		m_shapePath->setText(m_shapeStruct.ShapePath);
		m_shapePath->setReadOnly(true);
		m_readShapePathButton = new QPushButton(this);
		m_readShapePathButton->setText(__StandQString("选择路径"));

		QHBoxLayout* modePathLayout = new QHBoxLayout(this);
		modePathLayout->addWidget(labelPath, 1);
		modePathLayout->addWidget(m_shapePath, 2);
		modePathLayout->addWidget(m_readShapePathButton, 1);
		leftLayout->addLayout(modePathLayout);


		// 模型颜色
		QLabel* labelColor = new QLabel(this);
		labelColor->setText(__StandQString("模型颜色:"));
		m_shapeColor = new QLineEdit(this);
		m_shapeColor->setReadOnly(true);
		m_shapeColor->setStyleSheet("background-color: " + m_shapeStruct.ShapeColor + ";");
		m_readShapeColorButton = new QPushButton(this);
		m_readShapeColorButton->setText(__StandQString("选择颜色"));

		QHBoxLayout* modeColorLayout = new QHBoxLayout(this);
		modeColorLayout->addWidget(labelColor, 1);
		modeColorLayout->addWidget(m_shapeColor, 2);
		modeColorLayout->addWidget(m_readShapeColorButton, 1);
		leftLayout->addLayout(modeColorLayout);


		// 模型尺寸
		QLabel* labelSize = new QLabel(this);
		labelSize->setText(__StandQString("模型尺寸:"));
		m_shapeScale = new QLineEdit(this);
		m_shapeScale->setText(m_shapeStruct.ShapeScale);

		QHBoxLayout* modeSizeLayout = new QHBoxLayout(this);
		modeSizeLayout->addWidget(labelSize, 1);
		modeSizeLayout->addWidget(m_shapeScale, 3);
		leftLayout->addLayout(modeSizeLayout);

		// XYZ坐标
		QLabel* labelX = new QLabel(this);

		labelX->setText(__StandQString("模型-世界坐标X:"));
		m_shapePosX = new QLineEdit(this);
		m_shapePosX->setText(m_shapeStruct.ShapePositionX);

		QHBoxLayout* modeXLayout = new QHBoxLayout(this);
		modeXLayout->addWidget(labelX, 1);
		modeXLayout->addWidget(m_shapePosX, 3);
		leftLayout->addLayout(modeXLayout);

		QLabel* labelY = new QLabel(this);

		labelY->setText(__StandQString("模型-世界坐标Y:"));
		m_shapePosY = new QLineEdit(this);
		m_shapePosY->setText(m_shapeStruct.ShapePositionY);

		QHBoxLayout* modeYLayout = new QHBoxLayout(this);
		modeYLayout->addWidget(labelY, 1);
		modeYLayout->addWidget(m_shapePosY, 3);
		leftLayout->addLayout(modeYLayout);

		QLabel* labelZ = new QLabel(this);

		labelZ->setText(__StandQString("模型-世界坐标Z:"));
		m_shapePosZ = new QLineEdit(this);
		m_shapePosZ->setText(m_shapeStruct.ShapePositionZ);

		QHBoxLayout* modeZLayout = new QHBoxLayout(this);
		modeZLayout->addWidget(labelZ, 1);
		modeZLayout->addWidget(m_shapePosZ, 3);
		leftLayout->addLayout(modeZLayout);

		// 自转角度
		QLabel* labelRotX = new QLabel(this);
		labelRotX->setText(__StandQString("模型-自转角度X:"));
		m_shapeAngleX = new QLineEdit(this);
		m_shapeAngleX->setText(m_shapeStruct.ShapeAngleX);

		QHBoxLayout* modeRotXLayout = new QHBoxLayout(this);
		modeRotXLayout->addWidget(labelRotX, 1);
		modeRotXLayout->addWidget(m_shapeAngleX, 3);
		leftLayout->addLayout(modeRotXLayout);

		QLabel* labelRotY = new QLabel(this);
		labelRotY->setText(__StandQString("模型-自转角度Y:"));
		m_shapeAngleY = new QLineEdit(this);
		m_shapeAngleY->setText(m_shapeStruct.ShapeAngleY);

		QHBoxLayout* modeRotYLayout = new QHBoxLayout(this);
		modeRotYLayout->addWidget(labelRotY, 1);
		modeRotYLayout->addWidget(m_shapeAngleY, 3);
		leftLayout->addLayout(modeRotYLayout);

		QLabel* labelRotZ = new QLabel(this);
		labelRotZ->setText(__StandQString("模型-自转角度Z:"));
		m_shapeAngleZ = new QLineEdit(this);
		m_shapeAngleZ->setText(m_shapeStruct.ShapeAngleZ);

		QHBoxLayout* modeRotZLayout = new QHBoxLayout(this);
		modeRotZLayout->addWidget(labelRotZ, 1);
		modeRotZLayout->addWidget(m_shapeAngleZ, 3);
		leftLayout->addLayout(modeRotZLayout);

		// 开启连杆
		m_checkLink = new QCheckBox(this);
		m_checkLink->setText(__StandQString("开启连杆"));
		m_checkLink->setChecked(m_shapeStruct.ShapeLink);

		QHBoxLayout* modeLinkLayout = new QHBoxLayout(this);
		modeLinkLayout->addWidget(m_checkLink);
		leftLayout->addLayout(modeLinkLayout);


		m_linkWidget = new QWidget(this);

		leftLayout->addWidget(m_linkWidget);

		QHBoxLayout* linkLayout = new QHBoxLayout(m_linkWidget);

		QVBoxLayout* linkLVLayout = new QVBoxLayout(m_linkWidget);




		m_linkList = new QListWidget(m_linkWidget);
		m_linkList->setSelectionMode(QAbstractItemView::SingleSelection);
		linkLVLayout->addWidget(m_linkList);

		QHBoxLayout* linkBtnLayout = new QHBoxLayout(m_linkWidget);
		m_addLinkButton = new QPushButton(m_linkWidget);
		m_addLinkButton->setText(__StandQString("添加连杆"));

		m_deleteLinkButton = new QPushButton(m_linkWidget);
		m_deleteLinkButton->setText(__StandQString("删除连杆"));

		linkBtnLayout->addWidget(m_addLinkButton);
		linkBtnLayout->addWidget(m_deleteLinkButton);
		linkLVLayout->addLayout(linkBtnLayout);

		QVBoxLayout* linkRVLayout = new QVBoxLayout(m_linkWidget);
		QLabel* labelLinkName = new QLabel(m_linkWidget);
		labelLinkName->setText(__StandQString("连杆名称:"));
		linkRVLayout->addWidget(labelLinkName);
		m_linkName = new QLineEdit(m_linkWidget);
		//m_linkName->setPlaceholderText(__StandQString("请输入连杆名称"));
		linkRVLayout->addWidget(m_linkName);



		QLabel* labelLinkIndex = new QLabel(m_linkWidget);
		labelLinkIndex->setText(__StandQString("连杆索引:"));
		linkRVLayout->addWidget(labelLinkIndex);
		m_linkIndex = new QLineEdit(m_linkWidget);
		//m_linkIndex->setPlaceholderText(__StandQString("请输入连杆索引"));
		linkRVLayout->addWidget(m_linkIndex);

		QLabel* labelLinkAlpha = new QLabel(m_linkWidget);
		labelLinkAlpha->setText(__StandQString("alpha :"));
		linkRVLayout->addWidget(labelLinkAlpha);
		m_linkAlpha = new QLineEdit(m_linkWidget);
		//m_linkAlpha->setPlaceholderText(__StandQString("alpha : "));
		linkRVLayout->addWidget(m_linkAlpha);

		QLabel* labelLinkADistance = new QLabel(m_linkWidget);
		labelLinkADistance->setText(__StandQString("a :"));
		linkRVLayout->addWidget(labelLinkADistance);
		m_linkADistance = new QLineEdit(m_linkWidget);
		//m_linkADistance->setPlaceholderText(__StandQString("a : "));
		linkRVLayout->addWidget(m_linkADistance);

		QLabel* labelLinkTheta = new QLabel(m_linkWidget);
		labelLinkTheta->setText(__StandQString("Theta :"));
		linkRVLayout->addWidget(labelLinkTheta);
		m_linkTheta = new QLineEdit(m_linkWidget);
		//m_linkTheta->setPlaceholderText(__StandQString("Theta : "));
		linkRVLayout->addWidget(m_linkTheta);

		QLabel* labelLinkDDistance = new QLabel(m_linkWidget);
		labelLinkDDistance->setText(__StandQString("d :"));
		linkRVLayout->addWidget(labelLinkDDistance);
		m_linkDDistance = new QLineEdit(m_linkWidget);
		//m_linkDDistance->setPlaceholderText(__StandQString("d : "));
		linkRVLayout->addWidget(m_linkDDistance);

		linkLayout->addLayout(linkLVLayout);
		linkLayout->addLayout(linkRVLayout);
		linkLayout->addStretch(1);

		m_checkLink->setChecked(false);

		if (m_checkLink->isChecked())
		{
			m_linkWidget->setVisible(true);
			m_shapePosX->setEnabled(false);
			m_shapePosY->setEnabled(false);
			m_shapePosZ->setEnabled(false);
			m_shapeAngleX->setEnabled(false);
			m_shapeAngleY->setEnabled(false);
			m_shapeAngleZ->setEnabled(false);
		}
		else
		{
			m_linkWidget->setVisible(false);
			m_shapePosX->setEnabled(true);
			m_shapePosY->setEnabled(true);
			m_shapePosZ->setEnabled(true);
			m_shapeAngleX->setEnabled(true);
			m_shapeAngleY->setEnabled(true);
			m_shapeAngleZ->setEnabled(true);
		}

		QPushButton* applyButton = new QPushButton(this);
		applyButton->setText(__StandQString("预览"));
		leftLayout->addWidget(applyButton);

		connect(applyButton, &QPushButton::clicked, [=]() {
			// 触发应用
			ISNULLPOINTER(m_robotPreview);
			m_robotPreview->slotReplaceModelByPath(m_shapeStruct);
			});

		leftLayout->addStretch(1);
		initConnect();

	}

	ShapeCommondPreview::~ShapeCommondPreview()
	{
		LOG_FUNCTION_LINE_MESSAGE;
	}

	void ShapeCommondPreview::setRobotPreviewPoint(RobotPreview* robotPreview)
	{
		m_robotPreview = robotPreview;
		m_robotPreview->slotReplaceModelByPath(m_shapeStruct);
		m_robotPreview->slotChangPreviewColor(m_shapeStruct);
	}

	void ShapeCommondPreview::setShapeStruct(SHAPESTRUCT shapeStruct)
	{
		LOG_FUNCTION_LINE_MESSAGE;
		m_shapeStruct = shapeStruct;
		m_shapeName->setText(m_shapeStruct.ShapeName);
		m_shapePath->setText(m_shapeStruct.ShapePath);
		m_shapeColor->setStyleSheet("background-color: " + m_shapeStruct.ShapeColor + ";");
		m_shapeColor->setText(m_shapeStruct.ShapeColor);
		m_shapeScale->setText(m_shapeStruct.ShapeScale);
		m_shapePosX->setText(m_shapeStruct.ShapePositionX);
		m_shapePosY->setText(m_shapeStruct.ShapePositionY);
		m_shapePosZ->setText(m_shapeStruct.ShapePositionZ);
		m_shapeAngleX->setText(m_shapeStruct.ShapeAngleX);
		m_shapeAngleY->setText(m_shapeStruct.ShapeAngleY);
		m_shapeAngleZ->setText(m_shapeStruct.ShapeAngleZ);

		//m_shapeTypeComboBox->clear();
		/*m_shapeTypeComboBox->addItem(__StandQString("无类型"), ShapeType_None);
		m_shapeTypeComboBox->addItem(__StandQString("工件"), ShapeType_Work);
		m_shapeTypeComboBox->addItem(__StandQString("焊缝"), ShapeType_Hole);
		m_shapeTypeComboBox->addItem(__StandQString("机器人"), ShapeType_Robot);
		m_shapeTypeComboBox->addItem(__StandQString("连杆"), ShapeType_Link);
		m_shapeTypeComboBox->addItem(__StandQString("长导轨"), ShapeType_LongGuide);
		m_shapeTypeComboBox->addItem(__StandQString("转台"), ShapeType_RotatingTable);
		m_shapeTypeComboBox->addItem(__StandQString("短导轨"), ShapeType_ShortGuide);
		m_shapeTypeComboBox->addItem(__StandQString("平台"), ShapeType_Table);
		m_shapeTypeComboBox->addItem(__StandQString("机器人1"), ShapeType_Robot_1);
		m_shapeTypeComboBox->addItem(__StandQString("机器人2"), ShapeType_Robot_2);
		m_shapeTypeComboBox->addItem(__StandQString("机器人3"), ShapeType_Robot_3);
		m_shapeTypeComboBox->addItem(__StandQString("机器人4"), ShapeType_Robot_4);
		m_shapeTypeComboBox->addItem(__StandQString("机器人5"), ShapeType_Robot_5);
		m_shapeTypeComboBox->addItem(__StandQString("机器人6"), ShapeType_Robot_6);
		m_shapeTypeComboBox->addItem(__StandQString("机器人7"), ShapeType_Robot_7);
		m_shapeTypeComboBox->addItem(__StandQString("机器人8"), ShapeType_Robot_8);
		m_shapeTypeComboBox->addItem(__StandQString("机器人9"), ShapeType_Robot_9);
		m_shapeTypeComboBox->addItem(__StandQString("机器人10"), ShapeType_Robot_10);
		m_shapeTypeComboBox->addItem(__StandQString("机器人11"), ShapeType_Robot_11);
		m_shapeTypeComboBox->addItem(__StandQString("机器人12"), ShapeType_Robot_12);
		m_shapeTypeComboBox->addItem(__StandQString("机器人13"), ShapeType_Robot_13);

		m_shapeTypeComboBox->setCurrentIndex(m_shapeStruct.shapeType);*/

		m_shapIndex->setText(QString::number(m_shapeStruct.ShapeLinkIndex));

		m_checkLink->setChecked(m_shapeStruct.ShapeLink);
		m_linkWidget->setVisible(m_shapeStruct.ShapeLink);



		m_linkList->clear();
		for (auto linkName : m_shapeStruct.shapeLinkData.keys())
		{
			m_linkList->addItem(new QListWidgetItem(linkName));
		}
		SHAPELINKDATA linkData;
		m_linkName->setText("");
		m_linkIndex->setText("");
		m_linkAlpha->setText("");
		m_linkADistance->setText("");
		m_linkTheta->setText("");
		m_linkDDistance->setText("");

		m_linkName->setEnabled(false);
		m_linkIndex->setEnabled(false);
		m_linkAlpha->setEnabled(false);
		m_linkADistance->setEnabled(false);
		m_linkTheta->setEnabled(false);
		m_linkDDistance->setEnabled(false);


		ISNULLPOINTER(m_robotPreview);
		m_robotPreview->slotReplaceModelByPath(m_shapeStruct);
	}

	SHAPESTRUCT ShapeCommondPreview::getShapeStruct()
	{
		LOG_FUNCTION_LINE_MESSAGE;
		return m_shapeStruct;
	}

	void ShapeCommondPreview::setShapeType(ShapeType shapeType)
	{
		LOG_FUNCTION_LINE_MESSAGE;
		m_shapeStruct.shapeType = shapeType;
	}
	void ShapeCommondPreview::initConnect()
	{
		LOG_FUNCTION_LINE_MESSAGE;
		connect(m_shapeName, &QLineEdit::editingFinished, this, &ShapeCommondPreview::readShapeName);
		connect(m_readShapePathButton, &QPushButton::clicked, this, &ShapeCommondPreview::readShapePath);
		connect(m_readShapeColorButton, &QPushButton::clicked, this, &ShapeCommondPreview::readShapeColor);
		connect(m_shapeScale, &QLineEdit::editingFinished, this, &ShapeCommondPreview::readShapeScale);
		connect(m_shapePosX, &QLineEdit::editingFinished, this, &ShapeCommondPreview::readShapePosX);
		connect(m_shapePosY, &QLineEdit::editingFinished, this, &ShapeCommondPreview::readShapePosY);
		connect(m_shapePosZ, &QLineEdit::editingFinished, this, &ShapeCommondPreview::readShapePosZ);
		connect(m_shapeAngleX, &QLineEdit::editingFinished, this, &ShapeCommondPreview::readShapeAngleX);
		connect(m_shapeAngleY, &QLineEdit::editingFinished, this, &ShapeCommondPreview::readShapeAngleY);
		connect(m_shapeAngleZ, &QLineEdit::editingFinished, this, &ShapeCommondPreview::readShapeAngleZ);
		connect(m_checkLink, &QCheckBox::stateChanged, this, &ShapeCommondPreview::checkChangeLink);
		connect(m_linkName, &QLineEdit::editingFinished, this, &ShapeCommondPreview::readLinkName);
		connect(m_linkAlpha, &QLineEdit::editingFinished, this, &ShapeCommondPreview::readLinkAlpha);
		connect(m_linkTheta, &QLineEdit::editingFinished, this, &ShapeCommondPreview::readLinkTheta);
		connect(m_linkDDistance, &QLineEdit::editingFinished, this, &ShapeCommondPreview::readLinkDDistance);
		connect(m_linkADistance, &QLineEdit::editingFinished, this, &ShapeCommondPreview::readLinkADistance);
		connect(m_addLinkButton, &QPushButton::clicked, this, &ShapeCommondPreview::addLink);
		connect(m_deleteLinkButton, &QPushButton::clicked, this, &ShapeCommondPreview::deleteLink);
		connect(m_linkList, &QListWidget::itemSelectionChanged, this, &ShapeCommondPreview::linkListItemChanged);
		connect(m_shapIndex, &QLineEdit::editingFinished, this, &ShapeCommondPreview::readShapeLinkIndex);
		connect(m_linkIndex, &QLineEdit::editingFinished, this, &ShapeCommondPreview::readLinkIndex);
		//connect(m_shapeTypeComboBox, &QComboBox::currentTextChanged, this, &ShapeCommondPreview::readShapeType);
	}
	void ShapeCommondPreview::setLinkIsVisable(bool isVisable)
	{
		LOG_FUNCTION_LINE_MESSAGE;
		m_checkLink->setChecked(isVisable);
		m_checkLink->setVisible(isVisable);
		m_shapeStruct.ShapeLink = isVisable;
		//m_linkWidget->setVisible(isVisable);
	}
	void ShapeCommondPreview::readShapePath()
	{
		// 打开文件选择对话框
		QFileDialog * fileDialog = new QFileDialog(this);
		fileDialog->setFileMode(QFileDialog::ExistingFile);
		fileDialog->setNameFilter(__StandQString("*.step"));
		fileDialog->setViewMode(QFileDialog::Detail);
		if (fileDialog->exec())
		{
			QString fileName = fileDialog->selectedFiles()[0];
			m_shapePath->setText(fileName);
			m_shapeStruct.ShapePath = fileName;

			if (fileName.endsWith(".step") || fileName.endsWith(".STEP"))
			{
				// 拷贝到运行目录下 Models 文件夹
				QDir dir;
				dir.mkpath(QCoreApplication::applicationDirPath() + "/Models");
				QFile::copy(fileName, QCoreApplication::applicationDirPath() + "/Models/" + fileName.split("/").last());
				m_shapeStruct.ShapePath =  "/Models/" + fileName.split("/").last();
			}
		}
		//ISNULLPOINTER(m_robotPreview);
		//m_robotPreview->slotReplaceModelByPath(m_shapeStruct);
	}
	void ShapeCommondPreview::readShapeColor()
	{
		// 读取模型颜色
		QColor color = QColorDialog::getColor(Qt::white, this);
		if (color.isValid())
		{
			m_shapeColor->setStyleSheet("background-color: " + color.name() + ";");
			m_shapeStruct.ShapeColor = color.name();
			m_shapeColor->setText(color.name());
		}
		//m_robotPreview->slotChangPreviewColor(m_shapeStruct);
	}
	void ShapeCommondPreview::readShapeScale()
	{
		m_shapeStruct.ShapeScale = m_shapeScale->text();
		//m_robotPreview->slotChangedPreviewScale(m_shapeStruct);
	}
	void ShapeCommondPreview::readShapePosX()
	{
		m_shapeStruct.ShapePositionX = m_shapePosX->text();
		//m_robotPreview->slotChangedPreviewTranslation(m_shapeStruct);
	}
	void ShapeCommondPreview::readShapePosY()
	{
		m_shapeStruct.ShapePositionY = m_shapePosY->text();
		//m_robotPreview->slotChangedPreviewTranslation(m_shapeStruct);
	}
	void ShapeCommondPreview::readShapePosZ()
	{
		m_shapeStruct.ShapePositionZ = m_shapePosZ->text();
		//m_robotPreview->slotChangedPreviewTranslation(m_shapeStruct);
	}
	void ShapeCommondPreview::readShapeAngleX()
	{
		m_shapeStruct.ShapeAngleX = m_shapeAngleX->text();
		//m_robotPreview->slotChangedPreviewRotation(m_shapeStruct);
	}
	void ShapeCommondPreview::readShapeAngleY()
	{
		m_shapeStruct.ShapeAngleY = m_shapeAngleY->text();
		//m_robotPreview->slotChangedPreviewRotation(m_shapeStruct);
	}
	void ShapeCommondPreview::readShapeAngleZ()
	{
		m_shapeStruct.ShapeAngleZ = m_shapeAngleZ->text();
		//m_robotPreview->slotChangedPreviewRotation(m_shapeStruct);
	}
	void ShapeCommondPreview::readShapeName()
	{
		m_shapeStruct.ShapeName = m_shapeName->text();
	}
	void ShapeCommondPreview::addLink()
	{
		QString linkName = __StandQString("Link_").append(QString::number(m_linkList->count() + 1));
		m_linkList->addItem(new QListWidgetItem(linkName));
		//m_shapeStruct.ShapeLinkList.append(linkName);

		SHAPELINKDATA linkData;
		linkData.LinkName = linkName;
		m_shapeStruct.shapeLinkData.insert(linkName, linkData);

		m_linkName->setText(linkName);
		m_linkIndex->setText(QString::number(linkData.index));
		m_linkAlpha->setText(QString::number(linkData.alpha));
		m_linkADistance->setText(QString::number(linkData.aDistance));
		m_linkTheta->setText(QString::number(linkData.theta));
		m_linkDDistance->setText(QString::number(linkData.dDistance));
	}
	void ShapeCommondPreview::deleteLink()
	{
		int row = m_linkList->currentRow();
		if (row < 0)
		{
			return;
		}
		//m_shapeStruct.ShapeLinkList.removeAt(row);
		m_linkList->takeItem(row);

		SHAPELINKDATA linkData;

		m_linkName->setText("");

		m_linkIndex->setText(QString::number(linkData.index));
		m_linkAlpha->setText(QString::number(linkData.alpha));
		m_linkADistance->setText(QString::number(linkData.aDistance));
		m_linkTheta->setText(QString::number(linkData.theta));
		m_linkDDistance->setText(QString::number(linkData.dDistance));

		m_shapeStruct.shapeLinkData.remove(m_linkName->text());
	}
	void ShapeCommondPreview::checkChangeLink()
	{
		if (m_checkLink->checkState() == Qt::Checked)
		{
			m_linkWidget->setVisible(true);
			m_shapePosX->setEnabled(false);
			m_shapePosY->setEnabled(false);
			m_shapePosZ->setEnabled(false);
			m_shapeAngleX->setEnabled(false);
			m_shapeAngleY->setEnabled(false);
			m_shapeAngleZ->setEnabled(false);
		}
		else
		{
			m_linkWidget->setVisible(false);
			m_shapePosX->setEnabled(true);
			m_shapePosY->setEnabled(true);
			m_shapePosZ->setEnabled(true);
			m_shapeAngleX->setEnabled(true);
			m_shapeAngleY->setEnabled(true);
			m_shapeAngleZ->setEnabled(true);
		}
		m_shapeStruct.ShapeLink = m_checkLink->checkState() == Qt::Checked ? true : false;
	}
	void ShapeCommondPreview::linkListItemChanged()
	{
		int row = m_linkList->currentRow();
		if (row < 0)
		{
			return;
		}
		QString linkName = m_linkList->item(row)->text();
		SHAPELINKDATA linkData = m_shapeStruct.shapeLinkData.value(linkName);

		m_linkName->setEnabled(true);
		m_linkIndex->setEnabled(true);
		m_linkAlpha->setEnabled(true);
		m_linkADistance->setEnabled(true);
		m_linkTheta->setEnabled(true);
		m_linkDDistance->setEnabled(true);

		m_linkName->setText(linkName);
		m_linkIndex->setText(QString::number(linkData.index));
		m_linkAlpha->setText(QString::number(linkData.alpha));
		m_linkADistance->setText(QString::number(linkData.aDistance));
		m_linkTheta->setText(QString::number(linkData.theta));
		m_linkDDistance->setText(QString::number(linkData.dDistance));
	}
	void ShapeCommondPreview::readLinkName()
	{
		QString linkName = m_linkName->text();
		SHAPELINKDATA linkData = m_shapeStruct.shapeLinkData.value(linkName);
		linkData.LinkName = linkName;
		m_shapeStruct.shapeLinkData.insert(linkName, linkData);

	}
	void ShapeCommondPreview::readLinkAlpha()
	{
		QString linkName = m_linkName->text();
		SHAPELINKDATA linkData = m_shapeStruct.shapeLinkData.value(linkName);
		linkData.alpha = m_linkAlpha->text().toDouble();
		m_shapeStruct.shapeLinkData.insert(linkName, linkData);

	}
	void ShapeCommondPreview::readLinkTheta()
	{
		QString linkName = m_linkName->text();
		SHAPELINKDATA linkData = m_shapeStruct.shapeLinkData.value(linkName);
		linkData.theta = m_linkTheta->text().toDouble();
		m_shapeStruct.shapeLinkData.insert(linkName, linkData);
	}
	void ShapeCommondPreview::readLinkDDistance()
	{
		QString linkName = m_linkName->text();
		SHAPELINKDATA linkData = m_shapeStruct.shapeLinkData.value(linkName);
		linkData.dDistance = m_linkDDistance->text().toDouble();
		m_shapeStruct.shapeLinkData.insert(linkName, linkData);
	}
	void ShapeCommondPreview::readLinkADistance()
	{
		QString linkName = m_linkName->text();
		SHAPELINKDATA linkData = m_shapeStruct.shapeLinkData.value(linkName);
		linkData.aDistance = m_linkADistance->text().toDouble();
		m_shapeStruct.shapeLinkData.insert(linkName, linkData);
	}
	void ShapeCommondPreview::readLinkIndex()
	{
		QString linkName = m_linkName->text();
		SHAPELINKDATA linkData = m_shapeStruct.shapeLinkData.value(linkName);
		linkData.index = m_linkIndex->text().toInt();
		m_shapeStruct.shapeLinkData.insert(linkName, linkData);
	}
	void ShapeCommondPreview::readShapeLinkIndex()
	{
		m_shapeStruct.ShapeLinkIndex = m_shapIndex->text().toInt();
	}
	void ShapeCommondPreview::readShapeType()
	{
		m_shapeStruct.shapeType = (ShapeType)m_shapeTypeComboBox->currentIndex();
	}
}

