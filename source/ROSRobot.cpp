#include "../include/ROSRobot.h"

namespace TCP_ROBOT
{


	/**************************************** 构造/析构函数 ********************************************************/
	ROSRobot::ROSRobot(QWidget* parent)
	{
	}
	ROSRobot::~ROSRobot()
	{
		if (m_robotPreview != nullptr)
		{
			delete m_robotPreview;
		}
	}

	QWidget* ROSRobot::showPreview(QWidget* parent)
	{
		QWidget* preview = new QWidget(parent);
		preview->setWindowTitle(__StandQString("工件预览"));

		QHBoxLayout* layout = new QHBoxLayout(preview);
		layout->setContentsMargins(0, 0, 0, 0);

		QWidget* leftWidget = new QWidget(preview);
		QVBoxLayout* layout2 = new QVBoxLayout(leftWidget);
		layout2->setContentsMargins(0, 0, 0, 0);

		

		QListWidget* list = new QListWidget(preview);

		// 获取指令文件夹下所有文件夹的名称
		QDir dir(WORKPATH);
		QStringList workNames = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
		for (int i = 0; i < workNames.size(); i++)
		{
			QListWidgetItem* item = new QListWidgetItem(workNames[i]);
			list->addItem(item);
		}
		// 双击修改
		list->setEditTriggers(QAbstractItemView::DoubleClicked);
		// 设置显示模式
		list->setViewMode(QListView::ListMode);
		// 设置显示行数

		layout2->addWidget(list);
		connect(list, &QListWidget::itemClicked, this, [=](QListWidgetItem* item) {
			if (m_rightWorkView != nullptr)
			{
				m_rightWorkView->close();
				m_rightWorkView = nullptr;
			}
			m_currentWorkName = item->text();
			m_rightWorkView = showWorkAndHolePreview(preview);
			layout->addWidget(m_rightWorkView, 5);
			});
		connect(list, &QListWidget::currentTextChanged, this, [=](const QString& text) {
			m_currentWorkName = text;
			});
		
		QHBoxLayout* layout3 = new QHBoxLayout();
		layout3->setContentsMargins(0, 0, 0, 0);
		QPushButton* selectButton = new QPushButton(preview);
		selectButton->setText(__StandQString("选择工件"));
		QPushButton* addButton = new QPushButton(preview);
		addButton->setText(__StandQString("添加工件"));
		QPushButton* cancelButton = new QPushButton(preview);
		cancelButton->setText(__StandQString("删除工件"));
		layout3->addWidget(selectButton);
		layout3->addWidget(addButton);
		layout3->addWidget(cancelButton);
		connect(selectButton, &QPushButton::clicked, this, [=]() {
			});
		connect(addButton, &QPushButton::clicked, this, [=]() {
			QString workName = __StandQString("工件-") + QString::number(list->count() + 1);
			QListWidgetItem* item = new QListWidgetItem(workName);
			list->addItem(item);
			if (m_rightWorkView != nullptr)
			{
				m_rightWorkView->close();
				m_rightWorkView = nullptr;
			}
			m_currentWorkName = workName;
			m_rightWorkView = showWorkAndHolePreview(preview);
			layout->addWidget(m_rightWorkView, 5);

			});
		connect(cancelButton, &QPushButton::clicked, this, [=]() {
			// 删除指定文件夹
			QDir dir(WORKPATH);
			dir.remove(m_currentWorkName);
			// 删除列表项
			int row = list->currentRow();
			list->takeItem(row);
			// 关闭右侧预览窗口
			if (m_rightWorkView != nullptr)
			{
				m_rightWorkView->close();
				m_rightWorkView = nullptr;
			}
			});

		layout2->addLayout(layout3);

		// 右侧工作区
		layout->addWidget(leftWidget, 1);
		m_rightWorkView = new QWidget(preview);
		layout->addWidget(m_rightWorkView);


		preview->adjustSize();
		return preview;
	}

	QWidget* ROSRobot::showWorkAndHolePreview(QWidget* parent)
	{
		QWidget* preview = new QWidget(parent);
		preview->setWindowTitle(__StandQString("工件预览"));

		// 创建一个水平布局并设置边距为0
		QHBoxLayout* layout = new QHBoxLayout(preview);
		layout->setContentsMargins(0, 0, 0, 0);

		// 创建机器人预览组件
		m_robotPreview = new RobotPreview();
		// 将左侧的 QWidget 和机器人预览组件添加到布局中

		int index = 0;

		// 
		// 设置部分
		QWidget* settingView = new QWidget(parent);
		layout->addWidget(settingView, 1);
		layout->addWidget(m_robotPreview, 5);
		QGridLayout* layout2 = new QGridLayout(settingView);

		// 设置工件名称
		QLabel* label = new QLabel(settingView);
		label->setText(__StandQString("工件名称:"));
		QLineEdit* editName = new QLineEdit(settingView);
		layout2->addWidget(label, index, 0);
		layout2->addWidget(editName, index, 1, 1, 2);
		editName->setText(m_currentWorkName);
		editName->setReadOnly(true);
		// 设置存储路径
		QLabel* label6 = new QLabel(settingView);
		label6->setText(__StandQString("存储路径:"));
		QLineEdit* editStorePath = new QLineEdit(settingView);

		index++;
		layout2->addWidget(label6, index, 0);
		layout2->addWidget(editStorePath, index, 1, 1, 2);

		// 判断是否存在文件夹
		if (!QDir(WORKPATH).exists())
		{
			// 创建文件夹
			QDir().mkdir(WORKPATH);
		}
		QString storePathName = WORKPATH.append("/").append(editName->text());
		editStorePath->setText(storePathName);
		editStorePath->setReadOnly(true);
		//QPushButton* buttonStorePath = new QPushButton(settingView);
		//buttonStorePath->setText("选择路径");
		//connect(buttonStorePath, &QPushButton::clicked, this, [=]() {QString fileName = QFileDialog::getExistingDirectory(this, "选择文件夹", ""); editStorePath->setText(fileName); });
		connect(editName, &QLineEdit::textChanged, this, [=]() {QString storePathName = WORKPATH.append("/").append(editName->text()); editStorePath->setText(storePathName); });

		// 工件路径
		QLabel* label5 = new QLabel(settingView);
		label5->setText(__StandQString("工件路径:"));
		QLineEdit* editPath = new QLineEdit(settingView);
		editPath->setReadOnly(true);
		QPushButton* buttonPath = new QPushButton(settingView);
		buttonPath->setText(__StandQString("选择路径"));
		connect(buttonPath, &QPushButton::clicked, this, [=]() {
			QString fileName = QFileDialog::getOpenFileName(this, __StandQString("选择文件"), "", __StandQString("STEP文件(*.step)"));
			editPath->setText(fileName);
			// 加载模型
			m_robotPreview->loadModel(fileName);

			});
		index++;
		layout2->addWidget(label5, index, 0);
		layout2->addWidget(editPath, index, 1);
		layout2->addWidget(buttonPath, index, 2);



		// 设置工件颜色
		QLabel* label2 = new QLabel(settingView);
		label2->setText(__StandQString("工件颜色:"));
		QLineEdit* editColor = new QLineEdit(settingView);
		editColor->setReadOnly(true);
		QPushButton* buttonColor = new QPushButton(settingView);
		buttonColor->setText(__StandQString("选择颜色"));
		connect(buttonColor, &QPushButton::clicked, this, [=]() {QColor color = QColorDialog::getColor(Qt::white, this); editColor->setText(color.name()); editColor->setStyleSheet("background-color: " + color.name() + ";"); });

		index++;
		layout2->addWidget(label2, index, 0);
		layout2->addWidget(editColor, index, 1);
		layout2->addWidget(buttonColor, index, 2);

		// 设置工件尺寸
		QLabel* label3 = new QLabel(settingView);
		label3->setText(__StandQString("工件尺寸:"));
		QLineEdit* editSize = new QLineEdit(settingView);

		index++;
		layout2->addWidget(label3, index, 0);
		layout2->addWidget(editSize, index, 1);

		// 设置工件位置
		QLabel* label4 = new QLabel(settingView);
		label4->setText(__StandQString("工件-世界坐标XYZ:"));
		QLabel* labelX = new QLabel(settingView);
		labelX->setText(__StandQString("X:"));
		QLabel* labelY = new QLabel(settingView);
		labelY->setText(__StandQString("Y:"));
		QLabel* labelZ = new QLabel(settingView);
		labelZ->setText(__StandQString("Z:"));


		QLineEdit* editPosX = new QLineEdit(settingView);
		QLineEdit* editPosY = new QLineEdit(settingView);
		QLineEdit* editPosZ = new QLineEdit(settingView);

		editPosX->setText("0");
		editPosY->setText("0");
		editPosZ->setText("0");

		index++;
		layout2->addWidget(label4, index, 0);
		index++;
		layout2->addWidget(labelX, index, 0);
		layout2->addWidget(editPosX, index, 1, 1, 2);
		index++;
		layout2->addWidget(labelY, index, 0);
		layout2->addWidget(editPosY, index, 1, 1, 2);
		index++;
		layout2->addWidget(labelZ, index, 0);
		layout2->addWidget(editPosZ, index, 1, 1, 2);

		// 设置工件自转角度
		QLabel* label7 = new QLabel(settingView);
		label7->setText(__StandQString("工件自转角度XYZ:"));

		QLabel* labelRotX = new QLabel(settingView);
		labelRotX->setText(__StandQString("X:"));
		QLabel* labelRotY = new QLabel(settingView);
		labelRotY->setText(__StandQString("Y:"));
		QLabel* labelRotZ = new QLabel(settingView);
		labelRotZ->setText(__StandQString("Z:"));
		QLineEdit* editRotX = new QLineEdit(settingView);
		QLineEdit* editRotY = new QLineEdit(settingView);
		QLineEdit* editRotZ = new QLineEdit(settingView);

		editRotX->setText("0");
		editRotY->setText("0");
		editRotZ->setText("0");

		index++;
		layout2->addWidget(label7, index, 0);
		index++;
		layout2->addWidget(labelRotX, index, 0);
		layout2->addWidget(editRotX, index, 1, 1, 2);
		index++;
		layout2->addWidget(labelRotY, index, 0);
		layout2->addWidget(editRotY, index, 1, 1, 2);
		index++;
		layout2->addWidget(labelRotZ, index, 0);
		layout2->addWidget(editRotZ, index, 1, 1, 2);
		index++;
		layout2->addWidget(new QLabel(__StandQString("焊缝设置")), index, 0);
		index++;
		layout2->addWidget(showHolePreview(parent), index, 0, 1, 3);
		// 确认与取消按钮
		QPushButton* buttonConfirm = new QPushButton(settingView);
		buttonConfirm->setText(__StandQString("确认"));
		//// 添加焊缝预览
		//QPushButton* buttonHolePreview = new QPushButton(settingView);
		//buttonHolePreview->setText(__StandQString("焊缝预览"));
		//connect(buttonHolePreview, &QPushButton::clicked, [=]() {
		//	if (m_holePreview == nullptr)
		//	{
		//		m_holePreview = showHolePreview(parent);
		//	}
		//	m_holePreview = showHolePreview(parent);
		//	m_holePreview->show();
		//	});
		QPushButton* buttonCancel = new QPushButton(settingView);
		buttonCancel->setText(__StandQString("取消"));

		index++;
		layout2->addWidget(buttonConfirm, index, 0);
		//layout2->addWidget(buttonHolePreview, index, 1);
		layout2->addWidget(buttonCancel, index, 2);

		// 信号与槽
		connect(buttonConfirm, &QPushButton::clicked, [=]() {
			// 创建文件夹
			if (!QDir(editStorePath->text()).exists())
			{
				qDebug() << __StandQString("创建文件夹") << editStorePath->text();
				QDir().mkpath(editStorePath->text());
			}
			WORKANDHOLE workAndHole;
			workAndHole.workName = editName->text();
			workAndHole.holeShape = m_holeMap;

			m_robotPreview->slotUpdateModel(workAndHole);
			// 判断 RobotPostion.json 文件是否存在
			QFile file(editStorePath->text().append("/ShapesPosition.json"));
			if (!file.exists())
			{
				QVariantMap mainMap;
				QString workName = editName->text();
				if (workName.isEmpty())
				{
					QMessageBox::warning(this, __StandQString("警告"), __StandQString("工件名称不能为空!"));
				}

				QVariantMap workMap;
				workMap.insert("WorkName", editName->text());
				workMap.insert("WorkPath", editPath->text());
				workMap.insert("WorkColor", editColor->text());
				workMap.insert("WorkSize", editSize->text());
				workMap.insert("x", editPosX->text().toDouble());
				workMap.insert("y", editPosY->text().toDouble());
				workMap.insert("z", editPosZ->text().toDouble());
				workMap.insert("rx", editRotX->text().toDouble());
				workMap.insert("ry", editRotY->text().toDouble());
				workMap.insert("rz", editRotZ->text().toDouble());
				mainMap.insert(QString("Work_").append(editName->text()), workMap);

				for (auto hole : m_holeMap)
				{
					QString holeName = hole.holeName;

					QVariantMap holeMap;
					holeMap.insert("HoleName", holeName);
					holeMap.insert("HolePath", hole.path);
					holeMap.insert("HoleColor", hole.color);
					holeMap.insert("HoleSize", hole.sacle);
					holeMap.insert("x", hole.positionX);
					holeMap.insert("y", hole.positionY);
					holeMap.insert("z", hole.positionZ);
					holeMap.insert("rx", hole.angleX);
					holeMap.insert("ry", hole.angleY);
					holeMap.insert("rz", hole.angleZ);
					mainMap.insert(QString("Hole_").append(holeName), holeMap);
				}


				// 转换为json格式
				QJsonDocument doc = QJsonDocument::fromVariant(mainMap);
				// 保存到文件
				QFile file(editStorePath->text().append("/ShapesPosition.json"));
				if (file.open(QIODevice::WriteOnly | QIODevice::Text))
				{
					QTextStream stream(&file);
					stream << doc.toJson();
					file.close();
				}
			}


			});
		connect(buttonCancel, &QPushButton::clicked, [=]() {settingView->close(); });



		return preview;
	}

	QWidget* ROSRobot::showHolePreview(QWidget* parent)
	{
		// 存储数据
		// 创建焊缝预览窗口的 QWidget 类实例
		QWidget* holePreview = new QWidget(parent);
		holePreview->setWindowTitle(__StandQString("焊缝预览"));

		//  核心布局
		QHBoxLayout* mainLayout = new QHBoxLayout(holePreview);
		mainLayout->setContentsMargins(0, 0, 0, 0);


		// 左侧部件布局，包含焊缝列表和按钮
		QWidget* settingView = new QWidget(holePreview);
		mainLayout->addWidget(settingView);


		// 左侧组件布局，包含添加和删除按钮
		QVBoxLayout* holeLayout = new QVBoxLayout(settingView);
		// 焊缝列表
		QListWidget* listWidget = new QListWidget(holePreview);
		holeLayout->addWidget(listWidget);

		QPushButton* buttonAdd = new QPushButton(holePreview);
		buttonAdd->setText(__StandQString("添加"));
		QPushButton* buttonDelete = new QPushButton(holePreview);
		buttonDelete->setText(__StandQString("删除"));
		// 连接添加按钮的点击事件
		connect(buttonAdd, &QPushButton::clicked, [=]() {
			QListWidgetItem* item = new QListWidgetItem(listWidget);
			item->setText(__StandQString("焊缝-").append(QString::number(listWidget->count())));
			item->setFlags(item->flags() | Qt::ItemIsEditable);
			listWidget->addItem(item);

			// 焊缝名称
			QString holeName = item->text();

			// 创建焊缝数据结构
			HoleDATAStruct holeItem;
			holeItem.holeName = holeName;

			// 插入数据
			m_holeMap.insert(holeName, holeItem);

			// 设置为增加项
			listWidget->setCurrentItem(item);
			});
		// 连接删除按钮的点击事件
		connect(buttonDelete, &QPushButton::clicked, [=]() {
			if (listWidget->currentRow() >= 0)
			{
				// 删除数据
				QString holeName = listWidget->item(listWidget->currentRow())->text();
				m_holeMap.remove(holeName);

				if (m_rightView)
				{
					m_rightView->close();
					m_rightView = nullptr;
				}
				// 删除列表项
				listWidget->takeItem(listWidget->currentRow());

			}



			});

		QHBoxLayout* buttonLayout = new QHBoxLayout();
		buttonLayout->addWidget(buttonAdd);
		buttonLayout->addWidget(buttonDelete);
		holeLayout->addLayout(buttonLayout);

		m_rightView = new QWidget(holePreview);
		mainLayout->addWidget(m_rightView);
		// 连接列表项点击事件
		connect(listWidget, &QListWidget::currentItemChanged, [=](QListWidgetItem* item) {
			if (m_rightView)
			{
				m_rightView->close();
				m_rightView = nullptr;
			}
			// 文本
			QString holeName = item->text();
			// 显示右侧组件
			//HoleDATAStruct holeData = 
			m_rightView = showHoleRightPreview(holeName, holePreview);
			mainLayout->addWidget(m_rightView);
			});



		return holePreview;
	}

	QWidget* ROSRobot::showHoleRightPreview(QString holeName, QWidget* parent)
	{
		QWidget* holeViewRight = new QWidget(parent);
		// 右侧组件布局，包含焊缝名称、焊缝颜色、焊缝尺寸、焊缝位置、焊缝自转角度
		m_holeDataCopy = m_holeMap[holeName];

		int holeRightIndex = 0;
		QGridLayout* formLayout = new QGridLayout(holeViewRight);
		QLabel* labelName = new QLabel(holeViewRight);
		labelName->setText(__StandQString("焊缝名称:"));
		QLineEdit* editName = new QLineEdit(holeViewRight);
		editName->setText(m_holeDataCopy.holeName);
		connect(editName, &QLineEdit::textChanged, [=]() { m_holeDataCopy.holeName = editName->text(); });

		formLayout->addWidget(labelName, holeRightIndex, 0);
		formLayout->addWidget(editName, holeRightIndex, 1, 1, 2);

		// 焊缝路径
		QLabel* labelPath = new QLabel(holeViewRight);
		labelPath->setText(__StandQString("焊缝路径:"));
		QLineEdit* editPath = new QLineEdit(holeViewRight);
		editPath->setText(m_holeDataCopy.path);
		editPath->setReadOnly(true);
		QPushButton* buttonPath = new QPushButton(holeViewRight);
		buttonPath->setText(__StandQString("选择路径"));
		connect(buttonPath, &QPushButton::clicked, [=]() {
			QString fileName = QFileDialog::getOpenFileName(this, __StandQString("选择文件"), "", __StandQString("STEP文件(*.step)"));
			editPath->setText(fileName);
			m_holeDataCopy.path = fileName;
			});
		holeRightIndex++;
		formLayout->addWidget(labelPath, holeRightIndex, 0);
		formLayout->addWidget(editPath, holeRightIndex, 1);
		formLayout->addWidget(buttonPath, holeRightIndex, 2);

		QLabel* labelColor = new QLabel(holeViewRight);
		labelColor->setText(__StandQString("焊缝颜色:"));
		QLineEdit* editColor = new QLineEdit(holeViewRight);
		editColor->setReadOnly(true);

		editColor->setStyleSheet("background-color: " + m_holeDataCopy.color + ";");

		QPushButton* buttonColor = new QPushButton(holeViewRight);
		buttonColor->setText(__StandQString("选择颜色"));
		connect(buttonColor, &QPushButton::clicked, [=]() {QColor color = QColorDialog::getColor(Qt::white, this); editColor->setStyleSheet("background-color: " + color.name() + ";"); m_holeDataCopy.color = color.name(); });
		holeRightIndex++;
		formLayout->addWidget(labelColor, holeRightIndex, 0);
		formLayout->addWidget(editColor, holeRightIndex, 1);
		formLayout->addWidget(buttonColor, holeRightIndex, 2);

		QLabel* labelSize = new QLabel(holeViewRight);
		labelSize->setText(__StandQString("焊缝尺寸:"));
		QLineEdit* editSize = new QLineEdit(holeViewRight);
		editSize->setText(m_holeDataCopy.sacle);
		connect(editSize, &QLineEdit::textChanged, [=]() { m_holeDataCopy.sacle = editSize->text(); });

		holeRightIndex++;
		formLayout->addWidget(labelSize, holeRightIndex, 0);
		formLayout->addWidget(editSize, holeRightIndex, 1);

		QLabel* labelPos = new QLabel(holeViewRight);
		labelPos->setText(__StandQString("焊缝-世界坐标XYZ:"));
		QLabel* labelX = new QLabel(holeViewRight);
		labelX->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		labelX->setText(__StandQString("X:"));
		QLabel* labelY = new QLabel(holeViewRight);
		labelY->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		labelY->setText(__StandQString("Y:"));
		QLabel* labelZ = new QLabel(holeViewRight);
		labelZ->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		labelZ->setText(__StandQString("Z:"));
		QLineEdit* editPosX = new QLineEdit(holeViewRight);
		QLineEdit* editPosY = new QLineEdit(holeViewRight);
		QLineEdit* editPosZ = new QLineEdit(holeViewRight);

		editPosX->setText(m_holeDataCopy.positionX);
		editPosY->setText(m_holeDataCopy.positionY);
		editPosZ->setText(m_holeDataCopy.positionZ);

		connect(editPosX, &QLineEdit::textChanged, [=]() { m_holeDataCopy.positionX = editPosX->text(); });
		connect(editPosY, &QLineEdit::textChanged, [=]() { m_holeDataCopy.positionY = editPosY->text(); });
		connect(editPosZ, &QLineEdit::textChanged, [=]() { m_holeDataCopy.positionZ = editPosZ->text(); });

		holeRightIndex++;
		formLayout->addWidget(labelPos, holeRightIndex, 0);
		holeRightIndex++;
		formLayout->addWidget(labelX, holeRightIndex, 0);
		formLayout->addWidget(editPosX, holeRightIndex, 1, 1, 2);
		holeRightIndex++;
		formLayout->addWidget(labelY, holeRightIndex, 0);
		formLayout->addWidget(editPosY, holeRightIndex, 1, 1, 2);
		holeRightIndex++;
		formLayout->addWidget(labelZ, holeRightIndex, 0);
		formLayout->addWidget(editPosZ, holeRightIndex, 1, 1, 2);

		QLabel* labelRot = new QLabel(holeViewRight);
		labelRot->setText(__StandQString("焊缝自转角度XYZ:"));
		QLabel* labelRotX = new QLabel(holeViewRight);
		labelRotX->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		labelRotX->setText(__StandQString("X:"));
		QLabel* labelRotY = new QLabel(holeViewRight);
		labelRotY->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		labelRotY->setText(__StandQString("Y:"));
		QLabel* labelRotZ = new QLabel(holeViewRight);
		labelRotZ->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		labelRotZ->setText(__StandQString("Z:"));
		QLineEdit* editRotX = new QLineEdit(holeViewRight);
		QLineEdit* editRotY = new QLineEdit(holeViewRight);
		QLineEdit* editRotZ = new QLineEdit(holeViewRight);
		editRotX->setText(m_holeDataCopy.angleX);
		editRotY->setText(m_holeDataCopy.angleY);
		editRotZ->setText(m_holeDataCopy.angleZ);

		connect(editRotX, &QLineEdit::textChanged, [=]() { m_holeDataCopy.angleX = editRotX->text(); });
		connect(editRotY, &QLineEdit::textChanged, [=]() { m_holeDataCopy.angleY = editRotY->text(); });
		connect(editRotZ, &QLineEdit::textChanged, [=]() { m_holeDataCopy.angleZ = editRotZ->text(); });

		holeRightIndex++;
		formLayout->addWidget(labelRot, holeRightIndex, 0);
		holeRightIndex++;
		formLayout->addWidget(labelRotX, holeRightIndex, 0);
		formLayout->addWidget(editRotX, holeRightIndex, 1, 1, 2);
		holeRightIndex++;
		formLayout->addWidget(labelRotY, holeRightIndex, 0);
		formLayout->addWidget(editRotY, holeRightIndex, 1, 1, 2);
		holeRightIndex++;
		formLayout->addWidget(labelRotZ, holeRightIndex, 0);
		formLayout->addWidget(editRotZ, holeRightIndex, 1, 1, 2);


		// 确认与取消按钮
		QPushButton* buttonConfirm = new QPushButton(holeViewRight);
		buttonConfirm->setText(__StandQString("确认"));

		/*	QPushButton* buttonApply = new QPushButton(holeViewRight);
			buttonApply->setText(__StandQString("应用"));*/

		QPushButton* buttonCancel = new QPushButton(holeViewRight);
		buttonCancel->setText(__StandQString("取消"));

		holeRightIndex++;
		formLayout->addWidget(buttonConfirm, holeRightIndex, 0);
		formLayout->addWidget(buttonCancel, holeRightIndex, 1);

		connect(buttonConfirm, &QPushButton::clicked, [=]() {
			// 保存数据
			qDebug() << __StandQString("保存数据");
			m_holeMap.insert(m_holeDataCopy.holeName, m_holeDataCopy);

			m_robotPreview->slotUpdateModel(createWorkAndHole(m_holeMap));
			});

		connect(buttonCancel, &QPushButton::clicked, [=]() {holeViewRight->close(); });
		/*connect(buttonApply, &QPushButton::clicked, [=]() {
			});*/

		return holeViewRight;
	}

	WORKANDHOLE ROSRobot::createWorkAndHole(QMap<QString, HoleDATAStruct> holeMap)
	{
		WORKANDHOLE workAndHole;
		workAndHole.workName = m_currentWorkName;
		workAndHole.holeShape = holeMap;
		return workAndHole;
	}

}

