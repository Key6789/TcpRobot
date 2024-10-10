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





		return nullptr;
	}

	QWidget* ROSRobot::showWorkAndHolePreview(QWidget* parent)
	{
		QWidget* preview = new QWidget(parent);
		preview->setWindowTitle(__StandQString("工件预览"));

		// 创建一个水平布局并设置边距为0
		QHBoxLayout* layout = new QHBoxLayout(preview);
		layout->setContentsMargins(0, 0, 0, 0);

		// 创建机器人预览组件
		RobotPreview* robotPreview = new RobotPreview();
		// 将左侧的 QWidget 和机器人预览组件添加到布局中
		
		int index = 0;

		// 
		// 设置部分
		QWidget* settingView = new QWidget(parent);
		layout->addWidget(settingView, 1);
		layout->addWidget(robotPreview, 1);
		QGridLayout* layout2 = new QGridLayout(settingView);

		// 设置工件名称
		QLabel* label = new QLabel(settingView);
		label->setText(__StandQString("工件名称:"));
		QLineEdit* editName = new QLineEdit(settingView);
		layout2->addWidget(label, index, 0);
		layout2->addWidget(editName, index, 1, 1, 2);

		// 设置存储路径
		QLabel* label6 = new QLabel(settingView);
		label6->setText(__StandQString("存储路径:"));
		QLineEdit* editStorePath = new QLineEdit(settingView);

		index ++;
		layout2->addWidget(label6, index, 0);
		layout2->addWidget(editStorePath, index, 1, 1, 2);

		// 判断是否存在文件夹
		if (!QDir(WORKPATH).exists())
		{
			// 创建文件夹
			QDir().mkdir(WORKPATH);
		}
		QString storePathName = WORKPATH.append(editName->text());
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
			robotPreview->loadModel(fileName);
			
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
		connect(buttonColor, &QPushButton::clicked, this, [=]() {QColor color = QColorDialog::getColor(Qt::white, this); editColor->setText(color.name()); });

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

		// 确认与取消按钮
		QPushButton* buttonConfirm = new QPushButton(settingView);
		buttonConfirm->setText(__StandQString("确认"));
		QPushButton* buttonCancel = new QPushButton(settingView);
		buttonCancel->setText(__StandQString("取消"));

		index++;
		layout2->addWidget(buttonConfirm, index, 0, 1, 2);
		layout2->addWidget(buttonCancel, index, 2, 1, 2);

		// 信号与槽
		connect(buttonConfirm, &QPushButton::clicked, [=]() {
			// 创建文件夹
			if (!QDir(editStorePath->text()).exists())
			{
				qDebug() << __StandQString("创建文件夹") << editStorePath->text();
				QDir().mkpath(editStorePath->text());
			}

			// 判断 RobotPostion.json 文件是否存在
			QFile file(editStorePath->text().append("/RobotPosition.json"));
			if (!file.exists())
			{
				QJsonObject json;
				QJsonArray jsonArray;
				QJsonObject jsonObj;
				jsonObj.insert("name", "RobotPosition");
				jsonObj.insert("x", 0);
				jsonObj.insert("y", 0);
				jsonObj.insert("z", 0);
				jsonArray.append(jsonObj);
				json.insert("RobotPosition", jsonArray);
				QJsonDocument doc(json);
				QFile file(editStorePath->text().append("/RobotPosition.json"));
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

}

