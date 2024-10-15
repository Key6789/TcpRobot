#include "../include/ROSRobot.h"

namespace TCP_ROBOT
{


	/**************************************** 构造/析构函数 ********************************************************/
	ROSRobot::ROSRobot(QWidget* parent)
	{
	}
	ROSRobot::~ROSRobot()
	{
	}
	
	QWidget* ROSRobot::showPreview(QWidget* parent)
	{
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

		QHBoxLayout* layoutButton = new QHBoxLayout(leftWidget);
		QPushButton* seletedWorkButton = new QPushButton(leftWidget);
		seletedWorkButton->setText(__StandQString("选择工件"));

		QPushButton* addButton = new QPushButton(leftWidget);
		addButton->setText(__StandQString("添加工件"));

		QPushButton* deleteButton = new QPushButton(leftWidget);
		deleteButton->setText(__StandQString("删除工件"));

		QPushButton* saveButton = new QPushButton(leftWidget);
		saveButton->setText(__StandQString("保存工件"));
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


		RobotPreview* robotPreview = new RobotPreview(rightWidget);
		mainLayout->addWidget(robotPreview, 5);

		preViewHole->setRobotPreviewPoint(robotPreview);
		preViewWork->setRobotPreviewPoint(robotPreview);

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
			robotPreview->removeAllPreview();
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

		connect(seletedWorkButton, &QPushButton::clicked, [=]() {});
		connect(saveButton, &QPushButton::clicked, [=]() {

			QString key = workListWidget->currentItem()->text();
			QString fileName = WORKPATH.append("/").append(key).append("/").append(WORKCONFIGPATH);
			QFile file(fileName);
			QVariantMap variantMap;
			m_shapeMap.insert(key, preViewWork->getShapeStruct());
			preViewWork->getShapeStruct().ShapeAngleX;
			qDebug() << __StandQString("工件保存角度X:").append(preViewWork->getShapeStruct().ShapeAngleX);
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
				}

			}
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
			// 删除列表项
			int row = holeListWidget->currentRow();
			holeListWidget->takeItem(row);

			// 刷新预览
			preViewHole->setShapeStruct(SHAPESTRUCT());
			m_shapeMap.remove(holeListWidget->currentItem()->text());
			saveButtonHole->setEnabled(false);
			});

		connect(saveButtonHole, &QPushButton::clicked, [=]() {
			m_shapeMap.insert(holeListWidget->currentItem()->text(), preViewHole->getShapeStruct());
			});

		return preview;
	}

	void ROSRobot::createOrCheckDirectory(const QString& path)
	{
		if (!QDir(path).exists())
		{
			qDebug() << "创建文件夹" << path;
			QDir().mkpath(path);
		}
	}

	bool ROSRobot::removeDirectory(const QString& path)
	{
		QDir dir(path);
		return dir.removeRecursively();
	}

	QStringList ROSRobot::getSubDirectories(const QString& path)
	{
		QDir dir(path);
		return dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
	}

	bool ROSRobot::saveToJsonFile(const QVariantMap& data, const QString& filePath)
	{
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
		QGridLayout* layout = new QGridLayout(this);
		QLabel* labelName = new QLabel(this);
		labelName->setText(__StandQString("模型名称:"));
		m_shapeName = new QLineEdit(this);
		m_shapeName->setText(m_shapeStruct.ShapeName);
		layout->addWidget(labelName, 0, 0, 1, 1);
		layout->addWidget(m_shapeName, 0, 1, 1, 4);

		// 模型路径
		QLabel* labelPath = new QLabel(this);
		labelPath->setText(__StandQString("模型路径:"));
		m_shapePath = new QLineEdit(this);
		m_shapePath->setText(m_shapeStruct.ShapePath);
		m_shapePath->setReadOnly(true);
		m_readShapePathButton = new QPushButton(this);
		m_readShapePathButton->setText(__StandQString("选择路径"));
		layout->addWidget(labelPath, 1, 0, 1, 1);
		layout->addWidget(m_shapePath, 1, 1, 1, 3);
		layout->addWidget(m_readShapePathButton, 1, 4, 1, 1);

		// 模型颜色
		QLabel* labelColor = new QLabel(this);
		labelColor->setText(__StandQString("模型颜色:"));
		m_shapeColor = new QLineEdit(this);
		m_shapeColor->setReadOnly(true);
		m_shapeColor->setStyleSheet("background-color: " + m_shapeStruct.ShapeColor + ";");
		m_readShapeColorButton = new QPushButton(this);
		m_readShapeColorButton->setText(__StandQString("选择颜色"));
		layout->addWidget(labelColor, 2, 0, 1, 1);
		layout->addWidget(m_shapeColor, 2, 1, 1, 3);
		layout->addWidget(m_readShapeColorButton, 2, 4, 1, 1);

		// 模型尺寸
		QLabel* labelSize = new QLabel(this);
		labelSize->setText(__StandQString("模型尺寸:"));
		m_shapeScale = new QLineEdit(this);
		m_shapeScale->setText(m_shapeStruct.ShapeScale);
		layout->addWidget(labelSize, 3, 0, 1, 1);
		layout->addWidget(m_shapeScale, 3, 1, 1, 4);

		// XYZ坐标
		QLabel* labelX = new QLabel(this);
		labelX->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		labelX->setText(__StandQString("模型-世界坐标X:"));
		m_shapePosX = new QLineEdit(this);
		m_shapePosX->setText(m_shapeStruct.ShapePositionX);
		layout->addWidget(labelX, 4, 0, 1, 1);
		layout->addWidget(m_shapePosX, 4, 1, 1, 4);

		QLabel* labelY = new QLabel(this);
		labelY->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		labelY->setText(__StandQString("模型-世界坐标Y:"));
		m_shapePosY = new QLineEdit(this);
		m_shapePosY->setText(m_shapeStruct.ShapePositionY);
		layout->addWidget(labelY, 5, 0, 1, 1);
		layout->addWidget(m_shapePosY, 5, 1, 1, 4);

		QLabel* labelZ = new QLabel(this);
		labelZ->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		labelZ->setText(__StandQString("模型-世界坐标Z:"));
		m_shapePosZ = new QLineEdit(this);
		m_shapePosZ->setText(m_shapeStruct.ShapePositionZ);
		layout->addWidget(labelZ, 6, 0, 1, 1);
		layout->addWidget(m_shapePosZ, 6, 1, 1, 4);

		// 自转角度
		QLabel* labelRotX = new QLabel(this);
		labelRotX->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		labelRotX->setText(__StandQString("模型-自转角度X:"));
		m_shapeAngleX = new QLineEdit(this);
		m_shapeAngleX->setText(m_shapeStruct.ShapeAngleX);
		layout->addWidget(labelRotX, 7, 0, 1, 1);
		layout->addWidget(m_shapeAngleX, 7, 1, 1, 4);

		QLabel* labelRotY = new QLabel(this);
		labelRotY->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		labelRotY->setText(__StandQString("模型-自转角度Y:"));
		m_shapeAngleY = new QLineEdit(this);
		m_shapeAngleY->setText(m_shapeStruct.ShapeAngleY);
		layout->addWidget(labelRotY, 8, 0, 1, 1);
		layout->addWidget(m_shapeAngleY, 8, 1, 1, 4);

		QLabel* labelRotZ = new QLabel(this);
		labelRotZ->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		labelRotZ->setText(__StandQString("模型-自转角度Z:"));
		m_shapeAngleZ = new QLineEdit(this);
		m_shapeAngleZ->setText(m_shapeStruct.ShapeAngleZ);
		layout->addWidget(labelRotZ, 9, 0, 1, 1);
		layout->addWidget(m_shapeAngleZ, 9, 1, 1, 4);

		initConnect();

	}

	ShapeCommondPreview::~ShapeCommondPreview()
	{
	}

	void ShapeCommondPreview::setRobotPreviewPoint(RobotPreview* robotPreview)
	{
		m_robotPreview = robotPreview;
		m_robotPreview->slotReplaceModelByPath(m_shapeStruct);
		m_robotPreview->slotChangPreviewColor(m_shapeStruct);
	}

	void ShapeCommondPreview::setShapeStruct(SHAPESTRUCT shapeStruct)
	{
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

		ISNULLPOINTER(m_robotPreview);
		m_robotPreview->slotReplaceModelByPath(m_shapeStruct);
	}

	void ShapeCommondPreview::setShapeType(ShapeType shapeType)
	{
		m_shapeStruct.shapeType = shapeType;
	}
	void ShapeCommondPreview::initConnect()
	{
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

	}
	void ShapeCommondPreview::readShapePath()
	{
		// 打开文件选择对话框
		QString fileName = QFileDialog::getOpenFileName(this, __StandQString("选择模型文件"), QDir::currentPath(), __StandQString("*.step"));
		if (!fileName.isEmpty())
		{
			m_shapePath->setText(fileName);
			m_shapeStruct.ShapePath = fileName;
		}
		ISNULLPOINTER(m_robotPreview);
		m_robotPreview->slotReplaceModelByPath(m_shapeStruct);
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
		m_robotPreview->slotChangPreviewColor(m_shapeStruct);
	}
	void ShapeCommondPreview::readShapeScale()
	{
		m_shapeStruct.ShapeScale = m_shapeScale->text();
		m_robotPreview->slotChangedPreviewScale(m_shapeStruct);
	}
	void ShapeCommondPreview::readShapePosX()
	{
		m_shapeStruct.ShapePositionX = m_shapePosX->text();
		m_robotPreview->slotChangedPreviewTranslation(m_shapeStruct);
	}
	void ShapeCommondPreview::readShapePosY()
	{
		m_shapeStruct.ShapePositionY = m_shapePosY->text();
		m_robotPreview->slotChangedPreviewTranslation(m_shapeStruct);
	}
	void ShapeCommondPreview::readShapePosZ()
	{
		m_shapeStruct.ShapePositionZ = m_shapePosZ->text();
		m_robotPreview->slotChangedPreviewTranslation(m_shapeStruct);
	}
	void ShapeCommondPreview::readShapeAngleX()
	{
		m_shapeStruct.ShapeAngleX = m_shapeAngleX->text();
		m_robotPreview->slotChangedPreviewRotation(m_shapeStruct);
	}
	void ShapeCommondPreview::readShapeAngleY()
	{
		m_shapeStruct.ShapeAngleY = m_shapeAngleY->text();
		m_robotPreview->slotChangedPreviewRotation(m_shapeStruct);
	}
	void ShapeCommondPreview::readShapeAngleZ()
	{
		m_shapeStruct.ShapeAngleZ = m_shapeAngleZ->text();
		m_robotPreview->slotChangedPreviewRotation(m_shapeStruct);
	}
	void ShapeCommondPreview::readShapeName()
	{
		m_shapeStruct.ShapeName = m_shapeName->text();
	}
}

