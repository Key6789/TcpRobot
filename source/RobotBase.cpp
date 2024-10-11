#include "../include/RobotBase.h"

#ifdef WNT
#include <WNT_Window.hxx>
#elif defined(__APPLE__) && !defined(MACOSX_USE_GLX)
#include <Cocoa_Window.hxx>
#else
#undef Bool
#undef CursorShape
#undef None
#undef KeyPress
#undef KeyRelease
#undef FocusIn
#undef FocusOut
#undef FontChange
#undef Expose
#include <Xw_Window.hxx>
#endif
#include <BRepTools_WireExplorer.hxx>




namespace TCP_ROBOT
{


	/**************************************** 构造/析构函数 ********************************************************/
	RobotBase::RobotBase(QWidget* parent)
		: QWidget(parent),
		myXmin(0),
		myYmin(0),
		myXmax(0),
		myYmax(0),
		myCurrentMode(CurAction3d_DynamicRotation),
		myDegenerateModeIsOn(Standard_True),
		myRectBand(NULL)
	{
		// 在没有背景的情况下设置背景角色为NoRole
		setBackgroundRole(QPalette::NoRole);
		// 将焦点策略设置为StrongFocus，以便从键盘处理QContextMenuEvent
		setFocusPolicy(Qt::StrongFocus);
		setAttribute(Qt::WA_PaintOnScreen);
		setAttribute(Qt::WA_NoSystemBackground);

		// Enable the mouse tracking, by default the mouse tracking is disabled.
		setMouseTracking(true);


		initRobotView();
		//reset();
		//connect(ATJSTOOL, &ATJSTOOLAPP::signalUpdataRobotShap, this, &RobotBase::slotUpdataRobotShaps);

	}
	RobotBase::~RobotBase()
	{
	}

	QWidget* RobotBase::createWorkView(QWidget* parent)
	{
		QWidget* workView = new QWidget(parent);
		QVBoxLayout* layout = new QVBoxLayout(workView);
		layout->setContentsMargins(0, 0, 0, 0);
		layout->setSpacing(0);

		// 选择工件类型
		QComboBox* comboBox = new QComboBox(workView);
		comboBox->addItem(__StandQString("喷嘴"));
		comboBox->addItem(__StandQString("待设置"));

		// 请输入新的工件名称
		QLabel* label = new QLabel(workView);
		label->setText("请输入新的工件名称:");
		// 输入框
		QLineEdit* editName = new QLineEdit(workView);

		// 选择文件夹
		QPushButton* button = new QPushButton(workView);
		button->setText("选择文件夹");


		return workView;
	}

	QWidget* RobotBase::createTopShapeView(QWidget* parent)
	{
		// 创建顶部视图
		QWidget* topShapeView = new QWidget(parent);
		QVBoxLayout* layout = new QVBoxLayout(topShapeView);
		layout->setContentsMargins(0, 0, 0, 0);
		layout->setSpacing(0);

		// 模型文件读入
		QPushButton* button = new QPushButton(topShapeView);
		button->setText("导入模型");

		// 用OCC 显示模型

		return nullptr;
	}

	QWidget* RobotBase::createTeachView(QWidget* parent)
	{
		return nullptr;
	}

	QWidget* RobotBase::showShapeView(QWidget* parent)
	{
		QWidget* shapeView = new QWidget(parent);
		QVBoxLayout* layout = new QVBoxLayout(shapeView);
		layout->setContentsMargins(0, 0, 0, 0);
		layout->setSpacing(0);

		// 显示模型
		// 创建 Aspect_DisplayConnection
		Handle(Aspect_DisplayConnection) aDisplayConnection = new Aspect_DisplayConnection();

		// 如果图形驱动程序不存在，则初始化
		if (GetGraphicDriver().IsNull())
		{
			GetGraphicDriver() = new OpenGl_GraphicDriver(aDisplayConnection);
		}

		// 获取窗口句柄。这将返回适用于所有平台的内容。
		WId window_handle = (WId)winId();


		// Create appropriate window for platform
#ifdef WNT
		Handle(WNT_Window) wind = new WNT_Window((Aspect_Handle)window_handle);
#elif defined(__APPLE__) && !defined(MACOSX_USE_GLX)
		Handle(Cocoa_Window) wind = new Cocoa_Window((NSView*)window_handle);
#else

	//Handle(Xw_Window) wind = new Xw_Window(aDisplayConnection, (Window)window_handle);
#endif

	/// 创建 V3dViewer 和 V3d_View
		V3d_Viewer* myViewerTemp = new V3d_Viewer(GetGraphicDriver());
		Handle(V3d_View) myViewTemp = myViewerTemp->CreateView();

		//WId win_handle = winId();
		Handle(WNT_Window) window = new WNT_Window((Aspect_Handle)window_handle);
		myViewTemp->SetWindow(window);

		// Map the window if it is not already mapped
		if (!window->IsMapped())
		{
			window->Map();
		}

		// 创建 AISInteractiveContext
		AIS_InteractiveContext* myContextTemp = new AIS_InteractiveContext(myViewerTemp);

		// 设置光源等
		myViewerTemp->SetDefaultLights();
		myViewerTemp->SetLightOn();

		myViewTemp->SetBackgroundColor(Quantity_NOC_BLACK);
		myViewTemp->MustBeResized();

		// 坐标轴
		myViewTemp->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.05, V3d_ZBUFFER);
		//myViewer->Resize(w, h);

		myContextTemp->SetDisplayMode(AIS_Shaded, Standard_True);
		return nullptr;
	}

	QWidget* RobotBase::createRobotSettingView(QWidget* parent)
	{
		// 显示部分
		QWidget* robotSettingView = new QWidget(parent);
		QVBoxLayout* layout = new QVBoxLayout(robotSettingView);
		layout->setContentsMargins(0, 0, 0, 0);
		layout->setSpacing(0);

		// 显示部分

		// 设置部分

		QWidget* settingView = new QWidget(parent);
		QGridLayout* layout2 = new QGridLayout(settingView);
		layout2->setContentsMargins(0, 0, 0, 0);
		layout2->setSpacing(0);

		// 设置模型名称
		QLabel* label = new QLabel(settingView);
		label->setText(__StandQString("模型名称:"));
		QLineEdit* editName = new QLineEdit(settingView);

		// 设置模型颜色
		QLabel* label2 = new QLabel(settingView);
		label2->setText(__StandQString("模型颜色:"));
		QLineEdit* editColor = new QLineEdit(settingView);
		editColor->setReadOnly(true);
		QPushButton* buttonColor = new QPushButton(settingView);
		buttonColor->setText(__StandQString("选择颜色"));
		connect(buttonColor, &QPushButton::clicked, this, [=]() {QColor color = QColorDialog::getColor(Qt::white, this); editColor->setText(color.name()); });

		// 设置模型尺寸
		QLabel* label3 = new QLabel(settingView);
		label3->setText(__StandQString("模型尺寸:"));
		QLineEdit* editSize = new QLineEdit(settingView);

		// 设置模型位置
		QLabel* label4 = new QLabel(settingView);
		label4->setText(__StandQString("模型-世界坐标:"));
		QLineEdit* editPosX = new QLineEdit(settingView);
		QLineEdit* editPosY = new QLineEdit(settingView);
		QLineEdit* editPosZ = new QLineEdit(settingView);

		return nullptr;
	}

	QWidget* RobotBase::createWorkSettingView(QWidget* parent)
	{
		// 设置部分
		QWidget* settingView = new QWidget(parent);
		QGridLayout* layout2 = new QGridLayout(settingView);
		layout2->setContentsMargins(0, 0, 0, 0);
		layout2->setSpacing(0);

		// 设置工件名称
		QLabel* label = new QLabel(settingView);
		label->setText(__StandQString("工件名称:"));
		QLineEdit* editName = new QLineEdit(settingView);
		layout2->addWidget(label, 0, 0);
		layout2->addWidget(editName, 0, 1);

		// 设置存储路径
		QLabel* label6 = new QLabel(settingView);
		label6->setText(__StandQString("存储路径:"));
		QLineEdit* editStorePath = new QLineEdit(settingView);
		layout2->addWidget(label6, 1, 0);
		layout2->addWidget(editStorePath, 1, 1);

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
		connect(editName, &QLineEdit::textChanged, this, [=]() {QString storePathName = WORKPATH.append(editName->text()); editStorePath->setText(storePathName); });

		// 工件路径
		QLabel* label5 = new QLabel(settingView);
		label5->setText(__StandQString("工件路径:"));
		QLineEdit* editPath = new QLineEdit(settingView);
		editPath->setReadOnly(true);
		QPushButton* buttonPath = new QPushButton(settingView);
		buttonPath->setText(__StandQString("选择路径"));
		connect(buttonPath, &QPushButton::clicked, this, [=]() {QString fileName = QFileDialog::getOpenFileName(this, __StandQString("选择文件"), "", __StandQString("STEP文件(*.step)")); editPath->setText(fileName); });
		layout2->addWidget(label5, 2, 0);
		layout2->addWidget(editPath, 2, 1);
		layout2->addWidget(buttonPath, 2, 2);


		// 设置工件颜色
		QLabel* label2 = new QLabel(settingView);
		label2->setText(__StandQString("工件颜色:"));
		QLineEdit* editColor = new QLineEdit(settingView);
		editColor->setReadOnly(true);
		QPushButton* buttonColor = new QPushButton(settingView);
		buttonColor->setText(__StandQString("选择颜色"));
		connect(buttonColor, &QPushButton::clicked, this, [=]() {QColor color = QColorDialog::getColor(Qt::white, this); editColor->setText(color.name()); });

		layout2->addWidget(label2, 3, 0);
		layout2->addWidget(editColor, 3, 1);
		layout2->addWidget(buttonColor, 3, 2);

		// 设置工件尺寸
		QLabel* label3 = new QLabel(settingView);
		label3->setText(__StandQString("工件尺寸:"));
		QLineEdit* editSize = new QLineEdit(settingView);
		layout2->addWidget(label3, 4, 0);
		layout2->addWidget(editSize, 4, 1);

		// 设置工件位置
		QLabel* label4 = new QLabel(settingView);
		label4->setText(__StandQString("工件-世界坐标XYZ:"));
		QLineEdit* editPosX = new QLineEdit(settingView);
		QLineEdit* editPosY = new QLineEdit(settingView);
		QLineEdit* editPosZ = new QLineEdit(settingView);

		layout2->addWidget(label4, 5, 0);
		layout2->addWidget(editPosX, 5, 1);
		layout2->addWidget(editPosY, 5, 2);
		layout2->addWidget(editPosZ, 5, 3);

		// 设置工件自转角度
		QLabel* label7 = new QLabel(settingView);
		label5->setText(__StandQString("工件自转角度XYZ:"));
		QLineEdit* editRotX = new QLineEdit(settingView);
		QLineEdit* editRotY = new QLineEdit(settingView);
		QLineEdit* editRotZ = new QLineEdit(settingView);

		layout2->addWidget(label7, 6, 0);
		layout2->addWidget(editRotX, 6, 1);
		layout2->addWidget(editRotY, 6, 2);
		layout2->addWidget(editRotZ, 6, 3);

		// 确认与取消按钮
		QPushButton* buttonConfirm = new QPushButton(settingView);
		buttonConfirm->setText(__StandQString("确认"));
		QPushButton* buttonCancel = new QPushButton(settingView);
		buttonCancel->setText(__StandQString("取消"));
		layout2->addWidget(buttonConfirm, 7, 0);
		layout2->addWidget(buttonCancel, 7, 1);

		// 信号与槽
		connect(buttonConfirm, &QPushButton::clicked, [=]() {
			// 创建文件夹
			if (!QDir(editStorePath->text()).exists())
			{
				QDir().mkdir(editStorePath->text());
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



		return settingView;
	}

	/**************************************** 界面初始化 **********/
	void RobotBase::initRobotView()
	{
		// 创建互斥锁和锁定器的过程
		m_mutex = new QMutex();
		m_locker = new QMutexLocker(m_mutex);
		m_locker->unlock();

		// 创建 Aspect_DisplayConnection
		Handle(Aspect_DisplayConnection) aDisplayConnection = new Aspect_DisplayConnection();

		// 如果图形驱动程序不存在，则初始化
		if (GetGraphicDriver().IsNull())
		{
			GetGraphicDriver() = new OpenGl_GraphicDriver(aDisplayConnection);
		}

		// 获取窗口句柄。这将返回适用于所有平台的内容。
		WId window_handle = (WId)winId();


		// Create appropriate window for platform
#ifdef WNT
		Handle(WNT_Window) wind = new WNT_Window((Aspect_Handle)window_handle);
#elif defined(__APPLE__) && !defined(MACOSX_USE_GLX)
		Handle(Cocoa_Window) wind = new Cocoa_Window((NSView*)window_handle);
#else

	//Handle(Xw_Window) wind = new Xw_Window(aDisplayConnection, (Window)window_handle);
#endif

	/// 创建 V3dViewer 和 V3d_View
		myViewer = new V3d_Viewer(GetGraphicDriver());
		myView = myViewer->CreateView();

		//WId win_handle = winId();
		Handle(WNT_Window) window = new WNT_Window((Aspect_Handle)window_handle);
		myView->SetWindow(window);

		// Map the window if it is not already mapped
		if (!window->IsMapped())
		{
			window->Map();
		}

		// 创建 AISInteractiveContext
		myContext = new AIS_InteractiveContext(myViewer);

		// 设置光源等
		myViewer->SetDefaultLights();
		myViewer->SetLightOn();

		myView->SetBackgroundColor(Quantity_NOC_BLACK);
		myView->MustBeResized();

		// 坐标轴
		myView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.05, V3d_ZBUFFER);
		//myViewer->Resize(w, h);

		myContext->SetDisplayMode(AIS_Shaded, Standard_True);
		//myView->SetSize(400);
		slotUpdataRobotShaps();

	}

	/********************************数据获取 ***************************************/
	TopoDS_Shape RobotBase::getShape()
	{
		return shape;
	}
	void RobotBase::loadModel(const QString& filePath)
	{
		// 显示模型
		myContext->RemoveAll(true);


		// 加载STEP模型并在上下文中显示形状
		QVector<TopoDS_Shape> shapes = loadWorkSTEPModel(filePath);
		for (int i = 0; i < shapes.size(); i++)
		{
			Handle(AIS_Shape) aiShape = new AIS_Shape(shapes[i]);
			myContext->Display(aiShape, Standard_True);
		}
		displayAllRobotShapes(m_CoordinateAxes);
	}
	const Handle(AIS_InteractiveContext)& RobotBase::getContext() const
	{

		return myContext;
	}



	bool RobotBase::loadSTEPModel(const QString& filePath, TopoDS_Shape& shape)
	{
		// 读取 
		//橘红色

		Quantity_Color colorYellow = Quantity_Color(255 / 255.0, 80 / 255.0, 0 / 255.0, Quantity_TOC_sRGB); // 颜色
		QStringList filePathsq;
		filePathsq << m_myRobotDHData[DAOGUIMOVE].path;
		filePathsq << m_myRobotDHData[SHORTGUIDE].path;
		filePathsq << m_myRobotDHData[YUANPAN].path;
		filePathsq << m_myRobotDHData[GONGJIAN].path;
		filePathsq << m_myRobotDHData[ROBOT_1].path;
		filePathsq << m_myRobotDHData[ROBOT_2].path;
		filePathsq << m_myRobotDHData[ROBOT_3].path;
		filePathsq << m_myRobotDHData[ROBOT_4].path;
		filePathsq << m_myRobotDHData[ROBOT_5].path;
		filePathsq << m_myRobotDHData[ROBOT_6].path;
		filePathsq << m_myRobotDHData[ROBOT_7].path;
		filePathsq << m_myRobotDHData[ROBOT_8].path;
		filePathsq << m_myRobotDHData[ROBOT_9].path;

		//QVector<TopoDS_Shape> vector11 = loadWorkSTEPModel(filePathsq[10]);
		QVector<LoadResult> result = loadFilesInParallel(filePathsq);
		QVector<TopoDS_Shape> vector1 = getShapesFromResult(filePathsq[0], result);
		QVector<TopoDS_Shape> vector2 = getShapesFromResult(filePathsq[1], result);
		QVector<TopoDS_Shape> vector3 = getShapesFromResult(filePathsq[2], result);
		QVector<TopoDS_Shape> vector4 = getShapesFromResult(filePathsq[3], result);
		QVector<TopoDS_Shape> vector5 = getShapesFromResult(filePathsq[4], result);
		QVector<TopoDS_Shape> vector6 = getShapesFromResult(filePathsq[5], result);
		QVector<TopoDS_Shape> vector7 = getShapesFromResult(filePathsq[6], result);
		QVector<TopoDS_Shape> vector8 = getShapesFromResult(filePathsq[7], result);
		QVector<TopoDS_Shape> vector9 = getShapesFromResult(filePathsq[8], result);
		QVector<TopoDS_Shape> vector10 = getShapesFromResult(filePathsq[9], result);
		QVector<TopoDS_Shape> vector11 = getShapesFromResult(filePathsq[10], result);
		QVector<TopoDS_Shape> vector12 = getShapesFromResult(filePathsq[11], result);
		QVector<TopoDS_Shape> vector13 = getShapesFromResult(filePathsq[12], result);
		//放大为原来的两倍



		Quantity_Color Rcolor = Quantity_Color(112 / 255.0, 119 / 255.0, 116 / 255.0, Quantity_TOC_sRGB);
		ADDROBOTDATA addX1Robot = m_myRobotDHData.value(DAOGUIMOVE);
		addX1Robot.shapes = vector1;
		addX1Robot.name = "LongGuide";//名称 长导轨
		addX1Robot.nextShapeName = "ShortGuide"; // 连接的下一个工件名称
		addX1Robot.assemblyPoint = gp_Pnt(addX1Robot.x, addX1Robot.y, addX1Robot.z); // 装配点
		//addX1Robot.nextAssemblyPoint = gp_Pnt(0, 1000, 150); // 连接的下一个工件的装配点
		//addX1Robot.angleX = 90; // 旋转轴
		addX1Robot.color = Rcolor;// 颜色
		addX1Robot.xDirCurrent = getTransformationMatrix(addX1Robot, addX1Robot.xDir);
		addX1Robot.yDirCurrent = getTransformationMatrix(addX1Robot, addX1Robot.yDir);
		addX1Robot.zDirCurrent = getTransformationMatrix(addX1Robot, addX1Robot.zDir);
		m_myRobotData.insert(addX1Robot.name, addX1Robot);



		ADDROBOTDATA addX2Robot = m_myRobotDHData.value(SHORTGUIDE);
		addX2Robot.shapes = vector2;
		addX2Robot.name = "ShortGuide";//名称 短导轨
		addX2Robot.nextShapeName = "RotatingTable"; // 装配点
		addX2Robot.assemblyPoint = gp_Pnt(addX2Robot.x, addX2Robot.y, addX2Robot.z); // 装配点
		//addX2Robot.nextAssemblyPoint = gp_Pnt(0, 1000, 200); // 连接的下一个工件的装配点
		//addX2Robot.angleX = 90; // 旋转轴
		addX2Robot.color = Rcolor; // 颜色
		addX2Robot.xDirCurrent = getTransformationMatrix(addX2Robot, addX2Robot.xDir);
		addX2Robot.yDirCurrent = getTransformationMatrix(addX2Robot, addX2Robot.yDir);
		addX2Robot.zDirCurrent = getTransformationMatrix(addX2Robot, addX2Robot.zDir);
		m_myRobotData.insert(addX2Robot.name, addX2Robot);

		ADDROBOTDATA addR3Robot = m_myRobotDHData.value(YUANPAN);
		addR3Robot.shapes = vector3;
		addR3Robot.name = YUANPAN;//名称 旋转托盘
		addR3Robot.nextShapeName = GONGJIAN; // 连接的下一个工件名称
		addR3Robot.assemblyPoint = gp_Pnt(addR3Robot.x, addR3Robot.y, addR3Robot.z); // 装配点
		//addR3Robot.nextAssemblyPoint = gp_Pnt(0, 1000, 820); // 连接的下一个工件的装配点
		//addR3Robot.angleX = 180; // 旋转轴
		addR3Robot.color = Rcolor;; // 颜色
		addR3Robot.xDirCurrent = getTransformationMatrix(addR3Robot, addR3Robot.xDir);
		addR3Robot.yDirCurrent = getTransformationMatrix(addR3Robot, addR3Robot.yDir);
		addR3Robot.zDirCurrent = getTransformationMatrix(addR3Robot, addR3Robot.zDir);
		m_myRobotData.insert(addR3Robot.name, addR3Robot);

		ADDROBOTDATA addWork1Robot = m_myRobotDHData.value(GONGJIAN);
		addWork1Robot.shapes = vector4;
		addWork1Robot.name = GONGJIAN;//名称 工件
		addWork1Robot.assemblyPoint = gp_Pnt(addWork1Robot.x, addWork1Robot.y, addWork1Robot.z); // 装配点
		//addWork1Robot.angleX = 180; // 旋转轴
		addWork1Robot.xDirCurrent = getTransformationMatrix(addWork1Robot, addWork1Robot.xDir);
		addWork1Robot.yDirCurrent = getTransformationMatrix(addWork1Robot, addWork1Robot.yDir);
		addWork1Robot.zDirCurrent = getTransformationMatrix(addWork1Robot, addWork1Robot.zDir);
		// 银白色
		addWork1Robot.color = Quantity_Color(195 / 255.0, 195 / 255.0, 195 / 255.0, Quantity_TOC_RGB); // 颜色
		m_myRobotData.insert(addWork1Robot.name, addWork1Robot);

		ADDROBOTDATA addDiaoJianRobot = m_myRobotDHData.value(ROBOT_1);
		addDiaoJianRobot.shapes = vector5;
		addDiaoJianRobot.name = "Robot_1";//名称 工件
		//setCollisionDetection(addDiaoJianRobot, addDiaoJianRobot);

		//addDiaoJianRobot.assemblyPoint = gp_Pnt(0, 0, 0); // 装配点
		//addDiaoJianRobot.nextAssemblyPoint = gp_Pnt(0, 0, 0); // 连接的下一个工件的装配点
		//
		addDiaoJianRobot.nextShapeName = "Robot_2"; // 连接的下一个工件名称

		addDiaoJianRobot.assemblyPoint = getCurrentAssemblyPoint(1);
		addDiaoJianRobot.nextAssemblyPoint = getCurrentAssemblyPoint(2);
		gp_Pnt anglePoint1 = getCurrentRotationAngal(1);
		//addDiaoJianRobot.angleX = 90; // 旋转轴
		addDiaoJianRobot.angleY = anglePoint1.Y();
		addDiaoJianRobot.angleZ = anglePoint1.X(); // 旋转轴
		addDiaoJianRobot.angleX = anglePoint1.Z(); // 旋转轴


		addDiaoJianRobot.xDirCurrent = getTransformationMatrix(addDiaoJianRobot, addDiaoJianRobot.xDir);
		addDiaoJianRobot.yDirCurrent = getTransformationMatrix(addDiaoJianRobot, addDiaoJianRobot.yDir);
		addDiaoJianRobot.zDirCurrent = getTransformationMatrix(addDiaoJianRobot, addDiaoJianRobot.zDir);
		addDiaoJianRobot.color = colorYellow; // 颜色

		m_myRobotData.insert(addDiaoJianRobot.name, addDiaoJianRobot);

		ADDROBOTDATA addZhuanTaiRobot = m_myRobotDHData.value(ROBOT_2);
		addZhuanTaiRobot.shapes = vector6;
		addZhuanTaiRobot.name = "Robot_2";//名称 工件
		//addZhuanTaiRobot.angleX = 90; // 旋转轴
		gp_Pnt anglePoint2 = getCurrentRotationAngal(2);
		addZhuanTaiRobot.angleY = anglePoint2.Y();
		addZhuanTaiRobot.angleZ = anglePoint2.X(); // 旋转轴
		addZhuanTaiRobot.angleX = anglePoint2.Z(); // 旋转轴

		addZhuanTaiRobot.nextShapeName = "Robot_3"; // 连接的下一个工件名称
		//addZhuanTaiRobot.assemblyPoint = gp_Pnt(0, 0, 0); // 装配点
		//addZhuanTaiRobot.nextAssemblyPoint = gp_Pnt(150, 0, 200); // 连接的下一个工件的装配点
		//
		addZhuanTaiRobot.assemblyPoint = getCurrentAssemblyPoint(2);
		addZhuanTaiRobot.nextAssemblyPoint = getCurrentAssemblyPoint(3);

		addZhuanTaiRobot.xDirCurrent = getTransformationMatrix(addZhuanTaiRobot, addZhuanTaiRobot.xDir);
		addZhuanTaiRobot.yDirCurrent = getTransformationMatrix(addZhuanTaiRobot, addZhuanTaiRobot.yDir);
		addZhuanTaiRobot.zDirCurrent = getTransformationMatrix(addZhuanTaiRobot, addZhuanTaiRobot.zDir);
		addZhuanTaiRobot.color = colorYellow; // 颜色

		m_myRobotData.insert(addZhuanTaiRobot.name, addZhuanTaiRobot);

		ADDROBOTDATA addDaBiaoRobot = m_myRobotDHData.value(ROBOT_3);
		addDaBiaoRobot.shapes = vector7;
		addDaBiaoRobot.name = "Robot_3";//名称 工件
		addDaBiaoRobot.nextShapeName = "Robot_4";//名称 工件
		//addDaBiaoRobot.assemblyPoint = gp_Pnt(150, 0, 200); // 装配点
		//addDaBiaoRobot.nextAssemblyPoint = gp_Pnt(450, -60, 700); // 连接的下一个工件的装配点
		//

		addDaBiaoRobot.assemblyPoint = getCurrentAssemblyPoint(3);
		addDaBiaoRobot.nextAssemblyPoint = getCurrentAssemblyPoint(4);

		//addDaBiaoRobot.angleX = 180;
		//addDaBiaoRobot.angleY = -60; // 旋转轴
		gp_Pnt anglePoint3 = getCurrentRotationAngal(3);
		addDaBiaoRobot.angleX = anglePoint3.X(); // 旋转轴
		addDaBiaoRobot.angleZ = anglePoint3.Z(); // 旋转轴
		addDaBiaoRobot.angleY = anglePoint3.Y(); // 旋转轴


		addDaBiaoRobot.xDirCurrent = getTransformationMatrix(addDaBiaoRobot, addDaBiaoRobot.xDir);
		addDaBiaoRobot.yDirCurrent = getTransformationMatrix(addDaBiaoRobot, addDaBiaoRobot.yDir);
		addDaBiaoRobot.zDirCurrent = getTransformationMatrix(addDaBiaoRobot, addDaBiaoRobot.zDir);
		addDaBiaoRobot.color = colorYellow; // 颜色

		m_myRobotData.insert(addDaBiaoRobot.name, addDaBiaoRobot);

		ADDROBOTDATA addJingAoA5Robot = m_myRobotDHData.value(ROBOT_4);
		addJingAoA5Robot.shapes = vector8;
		addJingAoA5Robot.name = "Robot_4";//名称 工件
		addJingAoA5Robot.nextShapeName = "Robot_5"; // 连接的下一个工件名称
		//addJingAoA5Robot.assemblyPoint = gp_Pnt(450, -60, 700); // 装配点
		//addJingAoA5Robot.nextAssemblyPoint = gp_Pnt(900, 0, 750); // 连接的下一个工件的装配点
		//
		addJingAoA5Robot.assemblyPoint = getCurrentAssemblyPoint(4);
		addJingAoA5Robot.nextAssemblyPoint = getCurrentAssemblyPoint(6);


		//addJingAoA5Robot.angleX = 175;
		gp_Pnt anglePoint4 = getCurrentRotationAngal(4);
		addJingAoA5Robot.angleX = anglePoint4.X(); // 旋转轴
		addJingAoA5Robot.angleZ = anglePoint4.Z(); // 旋转轴
		addJingAoA5Robot.angleY = anglePoint4.Y(); // 旋转轴

		addJingAoA5Robot.xDirCurrent = getTransformationMatrix(addJingAoA5Robot, addJingAoA5Robot.xDir);
		addJingAoA5Robot.yDirCurrent = getTransformationMatrix(addJingAoA5Robot, addJingAoA5Robot.yDir);
		addJingAoA5Robot.zDirCurrent = getTransformationMatrix(addJingAoA5Robot, addJingAoA5Robot.zDir);

		addJingAoA5Robot.color = colorYellow; // 颜色


		// 旋转轴
		m_myRobotData.insert(addJingAoA5Robot.name, addJingAoA5Robot);

		ADDROBOTDATA addA4Robot = m_myRobotDHData.value(ROBOT_5);
		addA4Robot.shapes = vector9;
		addA4Robot.name = "Robot_5";//名称 工件
		addA4Robot.nextShapeName = "Robot_6"; // 连接的下一个工件名称
		//addA4Robot.assemblyPoint = gp_Pnt(900, 0, 750); // 装配点
		//addA4Robot.nextAssemblyPoint = gp_Pnt(1060, 0, 730); // 连接的下一个工件的装配点

		addA4Robot.assemblyPoint = getCurrentAssemblyPoint(6);
		addA4Robot.nextAssemblyPoint = getCurrentAssemblyPoint(8);
		gp_Pnt pnt1 = getCurrentRotationAngal(6);
		addA4Robot.angleZ = pnt1.Z(); // 旋转轴
		addA4Robot.angleY = pnt1.Y(); // 旋转轴
		addA4Robot.angleX = pnt1.X(); // 旋转轴

		addA4Robot.xDirCurrent = getTransformationMatrix(addA4Robot, addA4Robot.xDir);
		addA4Robot.yDirCurrent = getTransformationMatrix(addA4Robot, addA4Robot.yDir);
		addA4Robot.zDirCurrent = getTransformationMatrix(addA4Robot, addA4Robot.zDir);

		addA4Robot.color = colorYellow; // 颜色

		m_myRobotData.insert(addA4Robot.name, addA4Robot);

		ADDROBOTDATA addA6Robot = m_myRobotDHData.value(ROBOT_6);
		addA6Robot.shapes = vector10;
		addA6Robot.name = "Robot_6";//名称 工件

		//addA6Robot.assemblyPoint = gp_Pnt(1060, 0, 730); // 装配点
		//addA6Robot.nextAssemblyPoint = gp_Pnt(1060, 0, 730); // 连接的下一个工件的装配点
		//
		addA6Robot.assemblyPoint = getCurrentAssemblyPoint(8);; // 装配点
		addA6Robot.nextAssemblyPoint = getCurrentAssemblyPoint(9); // 连接的下一个工件的装配点

		//addA6Robot.angleX = 90; // 旋转轴
		//addA6Robot.angleY = 180; // 旋转轴
		//addA6Robot.angleZ = 90; // 旋转轴

		gp_Pnt pnt2 = getCurrentRotationAngal(8);
		addA6Robot.angleX = pnt2.X(); // 旋转轴
		addA6Robot.angleZ = pnt2.Z(); // 旋转轴
		addA6Robot.angleY = pnt2.Y(); // 旋转轴

		addA6Robot.xDirCurrent = getTransformationMatrix(addA6Robot, addA6Robot.xDir);
		addA6Robot.yDirCurrent = getTransformationMatrix(addA6Robot, addA6Robot.yDir);
		addA6Robot.zDirCurrent = getTransformationMatrix(addA6Robot, addA6Robot.zDir);

		addA6Robot.color = colorYellow; // 颜色
		m_myRobotData.insert(addA6Robot.name, addA6Robot);

		ADDROBOTDATA addA7Robot = m_myRobotDHData.value(ROBOT_7);
		addA7Robot.name = "Robot_7";//名称 工件
		//addA7Robot.shapes = vector10;
		//addA6Robot.assemblyPoint = gp_Pnt(1060, 0, 730); // 装配点
		//addA6Robot.nextAssemblyPoint = gp_Pnt(1060, 0, 730); // 连接的下一个工件的装配点
		//
		addA7Robot.assemblyPoint = getCurrentAssemblyPoint(9);; // 装配点
		addA7Robot.nextAssemblyPoint = getCurrentAssemblyPoint(10); // 连接的下一个工件的装配点

		//addA6Robot.angleX = 90; // 旋转轴
		//addA6Robot.angleY = 180; // 旋转轴
		//addA6Robot.angleZ = 90; // 旋转轴

		gp_Pnt pnt9 = getCurrentRotationAngal(9);
		addA7Robot.angleX = pnt9.X(); // 旋转轴
		addA7Robot.angleZ = pnt9.Z(); // 旋转轴
		addA7Robot.angleY = pnt9.Y(); // 旋转轴

		addA7Robot.xDirCurrent = getTransformationMatrix(addA7Robot, addA7Robot.xDir);
		addA7Robot.yDirCurrent = getTransformationMatrix(addA7Robot, addA7Robot.yDir);
		addA7Robot.zDirCurrent = getTransformationMatrix(addA7Robot, addA7Robot.zDir);

		addA7Robot.color = colorYellow; // 颜色
		m_myRobotData.insert(addA7Robot.name, addA7Robot);

		ADDROBOTDATA addA8Robot = m_myRobotDHData.value(ROBOT_8);
		addA8Robot.name = "Robot_8";//名称 工件

		//addA6Robot.assemblyPoint = gp_Pnt(1060, 0, 730); // 装配点
		//addA6Robot.nextAssemblyPoint = gp_Pnt(1060, 0, 730); // 连接的下一个工件的装配点
		//
		addA8Robot.assemblyPoint = getCurrentAssemblyPoint(10);; // 装配点
		addA8Robot.nextAssemblyPoint = getCurrentAssemblyPoint(11); // 连接的下一个工件的装配点

		//addA6Robot.angleX = 90; // 旋转轴
		//addA6Robot.angleY = 180; // 旋转轴
		//addA6Robot.angleZ = 90; // 旋转轴

		gp_Pnt pnt10 = getCurrentRotationAngal(10);
		addA8Robot.angleX = pnt10.X(); // 旋转轴
		addA8Robot.angleZ = pnt10.Z(); // 旋转轴
		addA8Robot.angleY = pnt10.Y(); // 旋转轴

		addA8Robot.xDirCurrent = getTransformationMatrix(addA8Robot, addA8Robot.xDir);
		addA8Robot.yDirCurrent = getTransformationMatrix(addA8Robot, addA8Robot.yDir);
		addA8Robot.zDirCurrent = getTransformationMatrix(addA8Robot, addA8Robot.zDir);

		addA8Robot.color = colorYellow; // 颜色
		m_myRobotData.insert(addA8Robot.name, addA8Robot);

		ADDROBOTDATA addA9Robot = m_myRobotDHData.value(ROBOT_9);
		addA9Robot.name = "Robot_9";//名称 工件

		//addA9Robot.assemblyPoint = gp_Pnt(1060, 0, 730); // 装配点
		//addA9Robot.nextAssemblyPoint = gp_Pnt(1060, 0, 730); // 连接的下一个工件的装配点
		//
		addA9Robot.assemblyPoint = getCurrentAssemblyPoint(11);; // 装配点
		addA9Robot.nextAssemblyPoint = getCurrentAssemblyPoint(11); // 连接的下一个工件的装配点

		//addA9Robot.angleX = 90; // 旋转轴
		//addA9Robot.angleY = 180; // 旋转轴
		//addA9Robot.angleZ = 90; // 旋转轴

		gp_Pnt pnt11 = getCurrentRotationAngal(11);
		addA9Robot.angleX = pnt11.X(); // 旋转轴
		addA9Robot.angleZ = pnt11.Z(); // 旋转轴
		addA9Robot.angleY = pnt11.Y(); // 旋转轴

		addA9Robot.xDirCurrent = getTransformationMatrix(addA9Robot, addA9Robot.xDir);
		addA9Robot.yDirCurrent = getTransformationMatrix(addA9Robot, addA9Robot.yDir);
		addA9Robot.zDirCurrent = getTransformationMatrix(addA9Robot, addA9Robot.zDir);

		addA9Robot.color = colorYellow; // 颜色
		m_myRobotData.insert(addA9Robot.name, addA9Robot);

		// 比例放大
		double scaleFactor = 2.0;
		findAndScaleShapes(ROBOT_1, scaleFactor);
		findAndScaleShapes(ROBOT_2, scaleFactor);
		findAndScaleShapes(ROBOT_3, scaleFactor);
		findAndScaleShapes(ROBOT_4, scaleFactor);
		findAndScaleShapes(ROBOT_5, scaleFactor);
		findAndScaleShapes(ROBOT_6, scaleFactor);
		findAndScaleShapes(DAOGUIMOVE, scaleFactor);
		findAndScaleShapes(SHORTGUIDE, scaleFactor);
		findAndScaleShapes(YUANPAN, scaleFactor);
		findAndScaleShapes(GONGJIAN, scaleFactor);
		findAndScaleShapes(ROBOT_7, scaleFactor);
		findAndScaleShapes(ROBOT_8, scaleFactor);
		findAndScaleShapes(ROBOT_9, scaleFactor);

		return true;
	}

	QVector<TopoDS_Shape> RobotBase::loadWorkSTEPModel(const QString& filePath)
	{
		QVector<TopoDS_Shape> vector = QVector<TopoDS_Shape>();
		if (filePath.isEmpty())
		{
			return vector;
		}
		
		STEPControl_Reader reader;
		IFSelect_ReturnStatus status = reader.ReadFile(filePath.toStdString().c_str());
		if (status != IFSelect_RetDone) {
			qDebug() << "Error: Unable to read file " << filePath << endl;
			return vector;
		}
		reader.TransferRoots();

		for (int i = 1; i <= reader.NbShapes(); i++)
		{
			TopoDS_Shape aShape = reader.Shape(i);
			vector.push_back(aShape);
		}
		return vector;
	}

	void RobotBase::displayCoordinateAxes()
	{
		// 创建坐标轴的长度
		Standard_Real axisLength = 5000.0;

		// 创建三个箭头表示XYZ轴的起点和终点
		gp_Pnt origin(0, 0, 0);
		gp_Pnt xAxisEnd(axisLength, 0, 0);
		gp_Pnt yAxisEnd(0, axisLength, 0);
		gp_Pnt zAxisEnd(0, 0, axisLength);

		// 使用BRepBuilderAPI创建线段
		TopoDS_Edge xEdge = BRepBuilderAPI_MakeEdge(origin, xAxisEnd).Edge();
		TopoDS_Edge yEdge = BRepBuilderAPI_MakeEdge(origin, yAxisEnd).Edge();
		TopoDS_Edge zEdge = BRepBuilderAPI_MakeEdge(origin, zAxisEnd).Edge();

		// 创建AIS_Shape对象来显示线段
		Handle(AIS_Shape) aisXAxis = new AIS_Shape(xEdge);
		Handle(AIS_Shape) aisYAxis = new AIS_Shape(yEdge);
		Handle(AIS_Shape) aisZAxis = new AIS_Shape(zEdge);
		LoadRobotTransformResult xAxis;
		xAxis.robotName = "XAxis";
		xAxis.myAisShapes = { aisXAxis };
		m_CoordinateAxes.push_back(xAxis);

		LoadRobotTransformResult yAxis;
		yAxis.robotName = "YAxis";
		yAxis.myAisShapes = { aisYAxis };
		m_CoordinateAxes.push_back(yAxis);

		LoadRobotTransformResult zAxis;
		zAxis.robotName = "ZAxis";
		zAxis.myAisShapes = { aisZAxis };
		m_CoordinateAxes.push_back(zAxis);


		aisXAxis->SetColor(Quantity_NOC_RED);
		aisYAxis->SetColor(Quantity_NOC_GREEN);
		aisZAxis->SetColor(Quantity_NOC_BLUE);
		// 假设上下文中已经有其他图形对象，你需要获取或创建这个上下文

		Standard_Real gridSize = 20000.0; // 网格大小
		Standard_Integer gridDivisions = 200; // 网格分割数

		// 创建平面
		gp_Pln plane(gp::Origin(), gp_Dir(0, 0, 1));

		// 创建平面实体
		BRepBuilderAPI_MakeFace mkFace(plane, -gridSize / 2, gridSize / 2, -gridSize / 2, gridSize / 2);
		TopoDS_Face face = mkFace.Face();



		// 显示平面
		if (m_gridShape.IsNull())
		{
			m_gridShape = new AIS_Shape(face);
			m_gridShape->SetColor(Quantity_NOC_GRAY25); // 灰色背景

			LoadRobotTransformResult grid;
			grid.robotName = "Grid";
			grid.myAisShapes = { m_gridShape };
			m_CoordinateAxes.push_back(grid);
		}



		if (!myContext.IsNull()) {
			// 使用默认的显示模式（例如，AIS_Shaded 或 AIS_WireFrame）
			Standard_Integer displayMode = 1; // 通常0表示默认模式
			myContext->Display(aisXAxis, displayMode);
			myContext->Display(aisYAxis, displayMode);
			myContext->Display(aisZAxis, displayMode);
			myContext->Display(m_gridShape, displayMode);
			//addArrow(axisLength, origin, gp_Dir(1, 0, 0), context); // X轴箭头
			//addArrow(axisLength, origin, gp_Dir(0, 1, 0), context); // Y轴箭头
			//addArrow(axisLength, origin, gp_Dir(0, 0, 1), context); // Z轴箭头
		}
		else {
			std::cerr << "Interactive context is null." << std::endl;
		}

		//// 更新视图以显示新添加的坐标轴
		//if (!myViewer.IsNull()) {
		//	myViewer->Redraw();
		//}
	}
	void RobotBase::addArrow(Standard_Real axisLength, gp_Pnt& origin, const gp_Dir& direction, Handle(AIS_InteractiveContext)& context) {
		// 箭头参数
		Standard_Real arrowLength = 100.0; // 箭头长度
		Standard_Real arrowRadius = 20.0;  // 箭头底面半径

		// 创建箭头所在的轴
		gp_Ax1 arrowAxis(origin, direction);

		// 使用BRepPrimAPI创建一个圆锥作为箭头形状
		TopoDS_Shape coneShape = BRepPrimAPI_MakeCone(arrowRadius, arrowRadius * 2, arrowLength).Shape();

		// 将圆锥沿箭头方向放置



		gp_Vec vecDirection(direction.XYZ() * axisLength); // 箭头末端位置向量


		// 创建变换对象并设置平移
		gp_Trsf transform;
		transform.SetTranslation(vecDirection); // 先设置平移
		BRepBuilderAPI_Transform translateTransform(coneShape, gp_Trsf());
		TopoDS_Shape translatedCone = translateTransform.Shape();

		// 注意：根据箭头应该指向的轴，旋转逻辑可能需要调整。
		// 下面的旋转逻辑示例是为X轴箭头设计的，对于Y和Z轴，您可能需要不同的旋转策略。
		// 对于Y轴箭头，可能无需旋转；对于Z轴箭头，可能需要围绕X轴旋转。
		// 这里省略旋转逻辑，因为具体旋转方式依赖于箭头如何正确对齐轴向。

		// 假设不需要额外旋转，直接使用已平移的圆锥
		// 将直线段和箭头融合为一个形状
		// 注意：这里直接使用原始的translatedCone，假设不需要旋转或已正确旋转
		BRepAlgoAPI_Fuse aFuse(translatedCone,
			BRepBuilderAPI_MakeEdge(origin,
				origin.Translated(
					gp_Vec(direction.XYZ()) * axisLength)).Shape());
		if (aFuse.IsDone()) {
			TopoDS_Shape fusedShape = aFuse.Shape();

			// 创建AIS_Shape对象并设置属性
			Handle(AIS_Shape) aisArrow = new AIS_Shape(fusedShape);
			Handle(Prs3d_Drawer) drawer = aisArrow->Attributes();
			drawer->SetColor(Quantity_NOC_YELLOW); // 设置箭头颜色，可以根据需要调整
			Standard_Integer displayMode = 0; // 通常0表示默认模式
			// 显示箭头
			context->Display(aisArrow, displayMode);
		}
		else {
			std::cerr << "Failed to fuse shapes for the arrow." << std::endl;
		}
	}
	void RobotBase::addShapeTitleText(const char* theText, gp_Pnt& point, Handle(AIS_Shape) aisShape)
	{
		TCollection_ExtendedString tostr = displayText("焊缝标号-1");
		//Resource_Unicode::ConvertGBToUnicode(str, tostr);
		Handle(AIS_TextLabel) aLabel = new AIS_TextLabel();
		aLabel->SetPosition(gp_Pnt(0, 0, 0));
		aLabel->SetText(tostr);
		aLabel->SetHJustification(Graphic3d_HTA_CENTER);
		aLabel->SetVJustification(Graphic3d_VTA_CENTER);
		aLabel->SetHeight(50);
		aLabel->SetZoomable(false);//如果允许缩放,你会发现放大后字太难看了
		aLabel->SetAngle(0);
		aLabel->SetColor(Quantity_NOC_YELLOW);
		aLabel->SetFont("SimHei");//一定要设置合适的字体，不然不能实现功能
		//如果有一天你需要鼠标选中文本时,下面代码有用,高亮文本
		Handle(Prs3d_Drawer) aStyle = new Prs3d_Drawer(); // 获取高亮风格
		aLabel->SetHilightAttributes(aStyle);
		aStyle->SetMethod(Aspect_TOHM_COLOR);
		aStyle->SetColor(Quantity_NOC_RED);    // 设置高亮颜色
		aStyle->SetDisplayMode(1); // 整体高亮
		aLabel->SetDynamicHilightAttributes(aStyle);

		// 构建复合体


		myContext->Display(aLabel, 0, 0, true);
	}
	void RobotBase::addComponentRobotALL()
	{
		// 加载机器人模型
		QVector<LoadRobotTransformResult> results = loadRobotTransformInParallel();
		displayAllRobotShapes(results);
		m_initMyRobotData = m_myRobotData;
		myView->Redraw();

		displayCoordinateAxes();
		myView->FitAll();
	}
	void RobotBase::workTransform(TRANSFORMDATA data)
	{
		ADDROBOTDATA addRobot = m_myRobotData.value(data.name);
		// 实现动画效果
		double x = data.translation.X();
		double y = data.translation.Y();
		double z = data.translation.Z();

		double angleX = data.angleX;
		double angleY = data.angleY;
		double angleZ = data.angleZ;


		gp_Vec yDir = getVectorByAngle(0, 90, 0);

		if (x != 0)
		{

			workTransformRecursively(data.name, data);
		}
		if (y != 0)
		{
			workTransformRecursively(data.name, data);
		}
		if (z != 0)
		{
			workTransformRecursively(data.name, data);
		}


		if (angleX != 0)
		{
			/*int count = abs(angleX) / 3;
			for (int i = 0; i < count; ++i)
			{
				if (angleX > 0)
				{
					data.angleX = 3;
					data.rotationAngle = 3;
				}
				else
				{
					data.angleX = -3;
					data.rotationAngle = -3;
				}
				data.rotationAngle = angleX;
				data.rotationAxis = getTransformationMatrix(addRobot, data.xDir);
				workTransformRecursively(data.name, data);
			}*/
			data.rotationAngle = angleX;
			gp_Dir roteX = addRobot.xDirCurrent;
			gp_Trsf trsf;
			trsf.SetRotation(gp_Ax1((addRobot.assemblyPoint.XYZ() + addRobot.assemblyPoint.XYZ()), data.rotationAxis), data.angleX / 180.0 * M_PI); // X轴旋转
			gp_Dir roteXTemp = roteX.Transformed(trsf);
			data.rotationAxis = roteXTemp;
			m_myRobotData.insert(data.name, addRobot);
			workTransformRecursively(data.name, data);
		}
		if (angleY != 0)
		{
			//int count = abs(angleY) / 3;
			//for (int i = 0; i < count; ++i)
			//{
			//	if (angleY > 0)
			//	{
			//		data.angleY = 3;
			//		data.rotationAngle = 3;

			//	}
			//	else
			//	{
			//		data.angleY = -3;
			//		data.rotationAngle = -3;
			//	}
			//	data.rotationAngle = angleY;
			//	data.rotationAxis = data.rotationAxis = getTransformationMatrix(addRobot, data.yDir);
			//	workTransformRecursively(data.name, data);
			//	//workTransformRecursivelySec(data.name, data);
			//}
			data.rotationAngle = angleY;

			gp_Dir roteY = addRobot.yDirCurrent;

			data.rotationAxis = roteY;
			m_myRobotData.insert(data.name, addRobot);
			workTransformRecursively(data.name, data);
		}
		if (angleZ != 0)
		{
			/*int count = abs(angleZ) / 3;
			for (int i = 0; i < count; ++i)
			{
				if (angleZ > 0)
				{
					data.angleZ = 3;
					data.rotationAngle = 3;
				}
				else
				{
					data.angleZ = -3;
					data.rotationAngle = -3;
				}
				data.rotationAngle = angleZ;
				data.rotationAxis = getTransformationMatrix(addRobot, data.zDir);
				workTransformRecursively(data.name, data);
			}*/
			data.rotationAngle = angleZ;
			gp_Dir roteZ = addRobot.zDirCurrent;
			gp_Trsf trsf;
			trsf.SetRotation(gp_Ax1((addRobot.assemblyPoint.XYZ() + addRobot.assemblyPoint.XYZ()), data.rotationAxis), data.angleZ / 180.0 * M_PI); // Z轴旋转
			gp_Dir roteZTemp = roteZ.Transformed(trsf);
			data.rotationAxis = roteZTemp;
			m_myRobotData.insert(data.name, addRobot);
			workTransformRecursively(data.name, data);
		}

		//workTransformRecursively(data.name, data);
		//myView->Redraw();
	}
	void RobotBase::setMyShapColor()
	{
		ADDROBOTDATA addRobot = m_myRobotData.value("Work");
		QVector<Handle(AIS_Shape)> vectortemp;
		for (auto& oldAisShape : addRobot.myAisShapes)
		{
			Handle(AIS_Shape) newShape = oldAisShape;
			myContext->Remove(oldAisShape, Standard_True);
			// 设置颜色
			Quantity_Color silverColor;
			silverColor.SetValues(192.0 / 255, 192.0 / 255, 192.0 / 255, Quantity_TOC_RGB);

			newShape->SetColor(silverColor);
			vectortemp.push_back(newShape);
			myContext->Display(newShape, Standard_True);
		}
		addRobot.myAisShapes = vectortemp;
		m_myRobotData.insert("Work", addRobot);
	}
	void RobotBase::setCollisionDetection()
	{
		// 开启显示模式
		myContext->SetDisplayMode(AIS_Shaded, Standard_True);
		myContext->SetDisplayMode(AIS_WireFrame, Standard_True);

		// 获取当前所有形状
		// 获取所有模型
		TopTools_HSequenceOfShape allShapes;
		QVector<Handle(AIS_Shape)> aisShape;

		foreach(ADDROBOTDATA addRobot, m_myRobotData)
		{
			for (auto& ais : addRobot.myAisShapes)
			{
				aisShape.push_back(ais);
			}
		}

		// 遍历所有形状，检测碰撞
		for (const auto& ais : aisShape)
		{
			// 从 AIS_Shape 获取形状
			TopoDS_Shape shape = ais->Shape();

			// 将形状添加到序列中
			allShapes.Append(shape);
		}
		// 遍历所有形状，检测碰撞
		for (int i = 1; i <= allShapes.Length(); ++i)
		{
			TopoDS_Shape shape1 = allShapes.Value(i);
			for (int j = i + 1; j <= allShapes.Length(); ++j)
			{
				TopoDS_Shape shape2 = allShapes.Value(j);
				if (!BRepAlgoAPI_Common(shape1, shape2).Shape().IsNull())
				{
					// 发生碰撞  
					std::cout << "Collision detected between shapes: " << i << " and " << j << std::endl;
					// 处理碰撞
					//handleCollision(shape1, shape2);
				}
			}
		}
	}
	void RobotBase::setCollisionDetection(Handle(AIS_Shape) shape1, Handle(AIS_Shape) shape2)
	{
		// 计算每个实体的边界框
		/*Bnd_Box box1, box2;
		box1 =  shape1->BoundingBox();
		box2 = shape2->BoundingBox();*/

		// 计算每个实体的边界框 
		Bnd_Box box1, box2;
		BRepBndLib::Add(shape1->Shape(), box1);
		BRepBndLib::Add(shape2->Shape(), box2);
		// 计算两个实体的相交结果
		// 检查边界框是否相交
		double distance = box1.Distance(box2);
		if (distance < BOXDISTANCE) {
			// 相交结果为空，且边界框不相交，说明实体不相交
			//QMessageBox::information(this, "Information", "No collision detected between shapes: ");
			emit singalCollision(false);
		}
		bool isIntersecting = box1.IsOut(box2) || box2.IsOut(box1);
		// 检查是否发生碰撞
		if (isIntersecting) {
			// 相交结果为空，且边界框不相交，说明实体不相交
			//QMessageBox::information(this, "Information", "No collision detected between shapes: ");
		}
		else {
			// 相交结果不为空，或边界框相交，说明实体相交
			//QMessageBox::warning(this, "Warning", "碰撞");

			emit singalCollision(false);
		}
	}

	void RobotBase::setCollisionDetection(ADDROBOTDATA data, ADDROBOTDATA data2)
	{
		//hideOnceShadBox(data);
		//hideOnceShadBox(data2);
		// 计算每个实体的边界框
		foreach(Handle(AIS_Shape) ais, data.myAisShapes)
		{
			Bnd_Box box1, box2;
			BRepBndLib::Add(ais->Shape(), box1);
			for (auto& ais2 : data2.myAisShapes)
			{
				BRepBndLib::Add(ais2->Shape(), box2);
				// 计算两个实体的相交结果
				// 检查边界框是否相交
				double distance = box1.Distance(box2);
				if (distance < BOXDISTANCE) {
					// 相交结果为空，且边界框不相交，说明实体不相交
					//QMessageBox::information(this, "Information", "No collision detected between shapes: ");
					emit singalCollision(false);
				}
				bool isIntersecting = box1.IsOut(box2) || box2.IsOut(box1);
				// 检查是否发生碰撞
				if (isIntersecting) {
					// 相交结果为空，且边界框不相交，说明实体不相交
					//QMessageBox::information(this, "Information", "No collision detected between shapes: ");
				}
				else {
					//showOnceShadBox(data);
					//showOnceShadBox(data2);
					// 相交结果不为空，或边界框相交，说明实体相交
					//QMessageBox::warning(this, "Warning", "碰撞");
					emit singalCollision(false);
				}
			}
		}
	}

	void RobotBase::setCollisionDetection(ADDROBOTDATA data, Handle(AIS_Shape) shape2)
	{
		//hideOnceShadBox(data);
		// 计算每个实体的边界框
		foreach(Handle(AIS_Shape) ais, data.myAisShapes)
		{
			Bnd_Box box1, box2;
			BRepBndLib::Add(ais->Shape(), box1);
			BRepBndLib::Add(shape2->Shape(), box2);
			// 计算两个实体的相交结果
			// 检查边界框是否相交
			// 检查两个盒子的距离
			double distance = box1.Distance(box2);
			if (distance < BOXDISTANCE) {
				// 相交结果为空，且边界框不相交，说明实体不相交
				//QMessageBox::information(this, "Information", "No collision detected between shapes: ");
				emit singalCollision(false);
			}
			bool isIntersecting = box1.IsOut(box2) || box2.IsOut(box1);
			// 检查是否发生碰撞
			if (isIntersecting) {
				// 相交结果为空，且边界框不相交，说明实体不相交
				//QMessageBox::information(this, "Information", "No collision detected between shapes: ");
			}
			else {
				//showOnceShadBox(data);
				// 相交结果不为空，或边界框相交，说明实体相交
				//QMessageBox::warning(this, "Warning", "碰撞");
				emit singalCollision(false);
			}
		}
	}

	void RobotBase::addComponentRobot(ADDROBOTDATA data)
	{
		QVector<TopoDS_Shape> vector = data.shapes;
		QVector<Handle(AIS_Shape)> vectortemp;
		QVector<Handle(AIS_Shape)> ShapesBox;
		// 确保data.myAisShapes与vector同步
		//assert(data.myAisShapes.size() == vector.size());

		for (int i = 0; i < vector.size(); ++i)
		{
			if (!data.myAisShapes.isEmpty())
			{
				myContext->Remove(data.myAisShapes[i], Standard_True); // 移除旧的形状
			}

			// 保存原始位置，以便稍后恢复
			gp_Pnt originalPosition = data.assemblyPoint;

			// 将形状平移到原点
			gp_Trsf moveToOrigin;
			moveToOrigin.SetTranslation(gp_Pnt(0.0, 0.0, 0.0).XYZ());
			TopoDS_Shape atOrigin = BRepBuilderAPI_Transform(vector[i], moveToOrigin).Shape();

			// 角度偏移（现在是在局部坐标系中）
			gp_Trsf localRotation;
			localRotation.SetRotation(data.axisX, data.angleX / 180.0 * M_PI); // X轴旋转
			TopoDS_Shape rotatedX = BRepBuilderAPI_Transform(atOrigin, localRotation).Shape();

			localRotation.SetRotation(data.axisY, data.angleY / 180.0 * M_PI); // Y轴旋转
			TopoDS_Shape rotatedXY = BRepBuilderAPI_Transform(rotatedX, localRotation).Shape();

			localRotation.SetRotation(data.axisZ, data.angleZ / 180.0 * M_PI); // Z轴旋转
			TopoDS_Shape rotatedXYZ = BRepBuilderAPI_Transform(rotatedXY, localRotation).Shape();

			// 利用 欧拉角 进行模型的旋转处理
			//// 创建一个变换对象
			//gp_Trsf trsf;
			//gp_EulerSequence eulerSeq(gp_YawPitchRoll);

			//gp_Quaternion quat;
			//quat.SetEulerAngles(eulerSeq, data.angleX / 180.0 * M_PI, data.angleY / 180.0 * M_PI, data.angleZ / 180.0 * M_PI);
			//trsf.SetRotationPart(quat);
			//// 应用变换
			//TopLoc_Location loc(trsf);
			//BRepBuilderAPI_Transform transform(atOrigin, loc, Standard_True); // Standard_True 表示更新拓扑结构
			//TopoDS_Shape rotatedXYZ = transform.Shape();

			// 将形状移回原始位置
			gp_Trsf moveBack;
			moveBack.SetTranslation(originalPosition.XYZ());
			TopoDS_Shape transformedComponent = BRepBuilderAPI_Transform(rotatedXYZ, moveBack).Shape();

			// 位移
			gp_Trsf finalTransform;
			finalTransform.SetTranslation(gp_Vec(data.assemblyPoint.XYZ()));
			transformedComponent = BRepBuilderAPI_Transform(transformedComponent, finalTransform).Shape();



			aisShape = new AIS_Shape(transformedComponent);
			aisShape->SetColor(data.color);
			myContext->Display(aisShape, Standard_True);
			vectortemp.push_back(aisShape);

			// 计算形状的边界框
			Bnd_Box box;
			BRepBndLib::Add(transformedComponent, box);
			ShapesBox.push_back(aisShape);


		}

		m_myRobotData[data.name].myAisShapes = vectortemp;
		m_myRobotData[data.name].disPlayShapes = vectortemp;
		m_myRobotData[data.name].ShapesBox = ShapesBox;
		m_myRobotData[data.name].assemblyPoint = data.assemblyPoint;

		//myView->Redraw();
		//myView->FitAll();

	}
	TCollection_ExtendedString RobotBase::displayText(const char* theText)
	{
		//const char* gbStr = "焊缝标号-1";
		// 使用Standard_WideString进行转换
		// 创建一个转换器，将GB2312转换为UTF-16
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

		// 转换GB2312字符串为宽字符串
		std::wstring wideStr = converter.from_bytes(theText);

		// 将宽字符串转换为TCollection_ExtendedString
		TCollection_ExtendedString extStr(wideStr.c_str());

		// 将宽字符串转换为TCollection_ExtendedString
		TCollection_ExtendedString tostr = TCollection_ExtendedString(wideStr.c_str());
		return tostr;
	}
	QVector<TopoDS_Shape> RobotBase::getShapesFromResult(const QString& name, const QVector<LoadResult>& shapes)
	{
		foreach(auto result, shapes)
		{
			if (result.filePath == name)
			{
				//QVector<TopoDS_Shape> SacleShapes = result.shapes;
				//for (auto& shape : SacleShapes) {
				//	// 创建一个 gp_Trsf 对象
				//	gp_Trsf transformation;
				//	transformation.SetScale(gp_Pnt(0, 0, 0), 2.0); // 放大两倍

				//	// 使用 BRepBuilderAPI_Transform 来应用变换
				//	BRepBuilderAPI_Transform transformer(shape, transformation, true);
				//	shape = transformer.Shape();

				//}
				return result.shapes;;
			}
		}
		return QVector<TopoDS_Shape>();
	}
	QVector<TopoDS_Shape> RobotBase::findAndScaleShapes(const QString& name, double scale)
	{
		ADDROBOTDATA addRobot = m_myRobotData.value(name);
		QVector<TopoDS_Shape> SacleShapes = addRobot.shapes;
		for (auto& shape : SacleShapes) {
			// 创建一个 gp_Trsf 对象
			gp_Trsf transformation;
			transformation.SetScale(gp_Pnt(0, 0, 0), scale); // 放大两倍

			// 使用 BRepBuilderAPI_Transform 来应用变换
			BRepBuilderAPI_Transform transformer(shape, transformation, true);
			shape = transformer.Shape();

		}
		addRobot.shapes = SacleShapes;
		m_myRobotData.insert(name, addRobot);
		return SacleShapes;
	}

	QVector<TopoDS_Shape> RobotBase::scaleShapes(QVector<TopoDS_Shape> shapes, double scale)
	{
		QVector<TopoDS_Shape> SacleShapes = shapes;
		for (auto& shape : SacleShapes) {
			// 创建一个 gp_Trsf 对象
			gp_Trsf transformation;
			transformation.SetScale(gp_Pnt(0, 0, 0), scale); // 放大两倍

			// 使用 BRepBuilderAPI_Transform 来应用变换
			BRepBuilderAPI_Transform transformer(shape, transformation, true);
			shape = transformer.Shape();

		}
		return SacleShapes;
	}

	QVector<LoadResult> RobotBase::loadFilesInParallel(const QStringList& filePaths)
	{
		QThreadPool* pool = QThreadPool::globalInstance();
		QVector<QFuture<LoadResult>> futures;

		for (const auto& filePath : filePaths)
		{
			futures.append(QtConcurrent::run([filePath, this]() -> LoadResult
				{
					QVector<TopoDS_Shape> shapes = loadWorkSTEPModel(filePath);
					return { filePath, shapes };
				}));
		}

		QVector<LoadResult> results;
		for (auto& future : futures) {
			results.append(future.result());
		}
		//delete pool;
		pool->waitForDone();
		return results;
	}
	QVector<LoadRobotTransformResult> RobotBase::loadRobotTransformInParallel()
	{
		QThreadPool* pool = QThreadPool::globalInstance();
		QVector<QFuture<LoadRobotTransformResult>> futures;
		QStringList filePaths = m_myRobotData.keys();

		for (const auto& filePath : filePaths)
		{
			futures.append(QtConcurrent::run([filePath, this]() -> LoadRobotTransformResult
				{
					QVector<Handle(AIS_Shape)> shapes = RobotTransformParallel(m_myRobotData.value(filePath));
					return { filePath, shapes };
				}));
		}
		QVector<LoadRobotTransformResult> results;
		for (auto& future : futures) {
			results.append(future.result());
		}
		//delete pool;
		pool->waitForDone();
		return results;
	}
	QVector<LoadRobotTransformResult> RobotBase::loadRobotTransformInParallel(QString currentName)
	{
		QThreadPool* pool = QThreadPool::globalInstance();
		QVector<QFuture<LoadRobotTransformResult>> futures;
		QStringList filePaths = getCurrentAndNextName(currentName);



		for (const auto& filePath : filePaths)
		{
			futures.append(QtConcurrent::run([filePath, this]() -> LoadRobotTransformResult
				{
					QVector<Handle(AIS_Shape)> shapes = RobotTransformParallel(m_myRobotData.value(filePath));
					return { filePath, shapes };
				}));
		}
		// 检测是否结束
		for (auto& future : futures) {
			future.waitForFinished();
		}
		//delete pool;
		QVector<LoadRobotTransformResult> results;
		for (auto& future : futures) {
			results.append(future.result());
		}
		/*foreach(const auto & robotName, m_myRobotData.keys())
		{
			if (filePaths.contains(robotName))
			{
				continue;
			}
			LoadRobotTransformResult temp;
			temp.robotName = robotName;
			temp.myAisShapes = m_myRobotData.value(robotName).myAisShapes;
			results.push_back(temp);
		}
		results.append(m_CoordinateAxes);*/
		return results;
	}
	void RobotBase::displayRobotShapes(LoadRobotTransformResult result)
	{

		QVector<Handle(AIS_Shape)> myShapes = result.myAisShapes;
		qDebug() << "displayRobotShapes" << result.robotName;

		for (Handle(AIS_Shape) shape : myShapes) {
			if (shape.IsNull()) { // 检查是否创建成功
				continue;
			}
			QElapsedTimer timer;
			timer.start();
			myContext->Remove(shape, Standard_True); // 移除旧的形状
			myContext->Display(shape, Standard_True);
			qDebug() << "Load" << result.robotName << timer.elapsed() << "ms";
		}

	}
	void RobotBase::displayRobotShapes(QVector<AIS_Shape> results)
	{
	}
	void RobotBase::displayAllRobotShapes(QVector<LoadRobotTransformResult> results)
	{
		if (m_initRobot) {
			return;
		}
		m_initRobot = true;
		//myContext->RemoveAll(false); // 移除旧的形状
		//myContext->ClearAllShapes();
		/*QThreadPool* poolDisplay = QThreadPool::globalInstance();
		QVector<QFuture<void>> futuresDisplay;*/
		foreach(auto result, results)
		{
			displayRobotShapes(result);
			/*futuresDisplay.append(QtConcurrent::run([result, this]() -> void
				{
					displayRobotShapes(result);
				}));*/
		}
		// 删除线程池
		//poolDisplay->waitForDone();
		m_initRobot = false;
	}
	void RobotBase::displayAllRobotShapes()
	{
		QVector<LoadRobotTransformResult> results;
		foreach(const auto & robotName, m_myRobotData.keys())
		{
			LoadRobotTransformResult temp;
			temp.robotName = robotName;
			temp.myAisShapes = m_myRobotData.value(robotName).myAisShapes;
			results.push_back(temp);
		}
		displayAllRobotShapes(results);
	}
	QVector<Handle(AIS_Shape)> RobotBase::RobotTransformParallel(ADDROBOTDATA data)
	{
		QVector<TopoDS_Shape> vector = data.shapes;
		QVector<Handle(AIS_Shape)> vectortemp;

		foreach(Handle(AIS_Shape) ais, data.myAisShapes)
		{
			if (ais->Shape().IsNull())
			{
				continue;
			}
			m_mutex->lock();
			myContext->Remove(ais, Standard_True); // 移除旧的形状
			m_mutex->unlock();
		}

		for (int i = 0; i < vector.size(); ++i)
		{

			// 保存原始位置，以便稍后恢复
			gp_Pnt originalPosition = data.assemblyPoint;

			// 将形状平移到原点
			gp_Trsf moveToOrigin;
			moveToOrigin.SetTranslation(gp_Pnt(0.0, 0.0, 0.0).XYZ());
			TopoDS_Shape atOrigin = BRepBuilderAPI_Transform(vector[i], moveToOrigin).Shape();

			// 角度偏移（现在是在局部坐标系中）
			gp_Trsf localRotation;
			localRotation.SetRotation(data.axisX, data.angleX / 180.0 * M_PI); // X轴旋转
			TopoDS_Shape rotatedX = BRepBuilderAPI_Transform(atOrigin, localRotation).Shape();

			localRotation.SetRotation(data.axisY, data.angleY / 180.0 * M_PI); // Y轴旋转
			TopoDS_Shape rotatedXY = BRepBuilderAPI_Transform(rotatedX, localRotation).Shape();

			localRotation.SetRotation(data.axisZ, data.angleZ / 180.0 * M_PI); // Z轴旋转
			TopoDS_Shape rotatedXYZ = BRepBuilderAPI_Transform(rotatedXY, localRotation).Shape();


			// 将形状移回原始位置
			gp_Trsf moveBack;
			moveBack.SetTranslation(originalPosition.XYZ());
			TopoDS_Shape transformedComponent = BRepBuilderAPI_Transform(rotatedXYZ, moveBack).Shape();

			// 位移
			gp_Trsf finalTransform;
			finalTransform.SetTranslation(gp_Vec(data.assemblyPoint.XYZ()));
			transformedComponent = BRepBuilderAPI_Transform(transformedComponent, finalTransform).Shape();

			Handle(AIS_Shape) aisShapeTemp = new AIS_Shape(transformedComponent);
			aisShapeTemp->SetColor(data.color);
			vectortemp.push_back(aisShapeTemp);
		}
		for (auto& shape : m_myRobotData[data.name].myAisShapes)
		{
			shape = NULL;
		}
		m_myRobotData[data.name].myAisShapes = vectortemp;
		m_myRobotData[data.name].assemblyPoint = data.assemblyPoint;
		return vectortemp;
	}
	QStringList RobotBase::getCurrentAndNextName(QString name)
	{
		QStringList tempList = QStringList();
		if (name != ROBOT_1)
		{
			tempList << SHORTGUIDE << YUANPAN << GONGJIAN;
		}
		else
		{
			tempList << ROBOT_1 << ROBOT_2 << ROBOT_3 << ROBOT_4 << ROBOT_5 << ROBOT_6;
		}

		return tempList;
	}
	void RobotBase::applyTranslation(ADDROBOTDATA data)
	{
		QVector<TopoDS_Shape> vector = data.shapes;
		QVector<Handle(AIS_Shape)> vectortemp;
		for (int i = 0; i < vector.size(); ++i)
		{
			if (!data.myAisShapes.isEmpty())
			{
				myContext->Remove(data.myAisShapes[i], Standard_False); // 移除旧的形状
			}

			// 保存原始位置，以便稍后恢复
			gp_Pnt originalPosition = data.assemblyPoint;

			// 将形状平移到原点
			gp_Trsf moveToOrigin;
			moveToOrigin.SetTranslation(gp_Pnt(0.0, 0.0, 0.0).XYZ());
			TopoDS_Shape atOrigin = BRepBuilderAPI_Transform(vector[i], moveToOrigin).Shape();

			// 角度偏移（现在是在局部坐标系中）
			gp_Trsf localRotation;
			localRotation.SetRotation(data.axisX, data.angleX / 180.0 * M_PI); // X轴旋转
			TopoDS_Shape rotatedX = BRepBuilderAPI_Transform(atOrigin, localRotation).Shape();

			localRotation.SetRotation(data.axisY, data.angleY / 180.0 * M_PI); // Y轴旋转
			TopoDS_Shape rotatedXY = BRepBuilderAPI_Transform(rotatedX, localRotation).Shape();

			localRotation.SetRotation(data.axisZ, data.angleZ / 180.0 * M_PI); // Z轴旋转
			TopoDS_Shape rotatedXYZ = BRepBuilderAPI_Transform(rotatedXY, localRotation).Shape();

			// 将形状移回原始位置
			gp_Trsf moveBack;
			moveBack.SetTranslation(originalPosition.XYZ());
			TopoDS_Shape transformedComponent = BRepBuilderAPI_Transform(rotatedXYZ, moveBack).Shape();

			// 位移
			gp_Trsf finalTransform;
			finalTransform.SetTranslation(gp_Vec(data.assemblyPoint.XYZ()));
			transformedComponent = BRepBuilderAPI_Transform(transformedComponent, finalTransform).Shape();

			aisShape = new AIS_Shape(transformedComponent);
			//myContext->Display(aisShape, Standard_True);
			vectortemp.push_back(aisShape);
		}

		m_myRobotData[data.name].myAisShapes = vectortemp;
	}
	// 辅助函数：用于在指定轴上应用旋转
	void RobotBase::applyRotation(gp_Trsf& trsf, const gp_Ax1& axis, double angle)
	{
		trsf.SetRotation(axis, angle);
	}
	gp_Dir RobotBase::getTransformationMatrix(const ADDROBOTDATA& robot, gp_Dir axis)
	{
		// 应用旋转
		gp_Trsf rotX, rotY, rotZ;
		gp_Dir worldRotationAxis;
		rotX.SetRotation(robot.axisX, robot.angleX / 180.0 * M_PI);
		worldRotationAxis = axis.Transformed(rotX);
		rotY.SetRotation(robot.axisY, robot.angleY / 180.0 * M_PI);
		worldRotationAxis = worldRotationAxis.Transformed(rotY);
		rotZ.SetRotation(robot.axisZ, robot.angleZ / 180.0 * M_PI);
		worldRotationAxis = worldRotationAxis.Transformed(rotZ);

		gp_Dir roundedDir(worldRotationAxis.X(), worldRotationAxis.Y(), worldRotationAxis.Z());
		/*if (fabs(roundedDir.X()) < 1e-2) roundedDir.SetX(0);
		if (fabs(roundedDir.Y()) < 1e-2) roundedDir.SetY(0);
		if (fabs(roundedDir.Z()) < 1e-2) roundedDir.SetZ(0);*/
		roundedDir.SetX(abs(roundedDir.X()));
		roundedDir.SetY(abs(roundedDir.Y()));
		roundedDir.SetZ(abs(roundedDir.Z()));


		return roundedDir;
	}
	void RobotBase::showAllShadBox()
	{
		hideAllShadBox();
		updateALlShadBox();
		// 显示
		for (auto& addRobot : m_myRobotData)
		{
			QVector<Handle(AIS_Shape)> shapesBox = addRobot.ShapesBox;
			for (auto& shapeBox : shapesBox)
			{
				myContext->Display(shapeBox, Standard_True);
			}
		}
	}
	void RobotBase::hideAllShadBox()
	{
		// 移除旧的边界框
		for (auto& addRobot : m_myRobotData)
		{
			QVector<Handle(AIS_Shape)> shapesBox = addRobot.ShapesBox;
			for (auto& shapeBox : shapesBox)
			{
				myContext->Erase(shapeBox, Standard_True);
			}
		}
	}
	void RobotBase::updateALlShadBox()
	{
		// 更新
		for (auto& addRobot : m_myRobotData)
		{
			QVector<Handle(AIS_Shape)> shapes = addRobot.myAisShapes;
			QVector<Handle(AIS_Shape)> shapesBox;
			for (auto& shape : shapes)
			{
				Bnd_Box box1;
				BRepBndLib::Add(shape->Shape(), box1);
				AIS_Shape box1Shape = BRepPrimAPI_MakeBox(box1.CornerMin(), box1.CornerMax());
				Handle(AIS_Shape) box1Ais = new AIS_Shape(box1Shape);
				box1Ais->SetColor(Quantity_NOC_RED);
				shapesBox.push_back(box1Ais);
			}
			addRobot.ShapesBox = shapesBox;
			m_myRobotData.insert(addRobot.name, addRobot);
		}
	}
	void RobotBase::showOnceShadBox(ADDROBOTDATA data)
	{
		hideOnceShadBox(data);
		updateOnceShadBox(data);
		// 显示
		QVector<Handle(AIS_Shape)> shapesBox = data.ShapesBox;
		for (auto& shapeBox : shapesBox)
		{
			// 设置为线框显示
			myContext->Display(shapeBox, Standard_True);
		}
	}
	void RobotBase::hideOnceShadBox(ADDROBOTDATA data)
	{
		// 移除旧的边界框
		QVector<Handle(AIS_Shape)> shapesBox = data.ShapesBox;
		for (auto& shapeBox : shapesBox)
		{
			myContext->Erase(shapeBox, Standard_True);
		}
	}
	void RobotBase::updateOnceShadBox(ADDROBOTDATA data)
	{
		// 更新
		QVector<Handle(AIS_Shape)> shapes = data.myAisShapes;
		QVector<Handle(AIS_Shape)> shapesBox;
		for (auto& shape : shapes)
		{
			Bnd_Box box1;
			BRepBndLib::Add(shape->Shape(), box1);
			AIS_Shape box1Shape = BRepPrimAPI_MakeBox(box1.CornerMin(), box1.CornerMax());
			Handle(AIS_Shape) box1Ais = new AIS_Shape(box1Shape);
			box1Ais->SetColor(Quantity_NOC_RED);
			shapesBox.push_back(box1Ais);
		}
		data.ShapesBox = shapesBox;
		m_myRobotData.insert(data.name, data);
	}

	gp_Pnt RobotBase::getNewAssemblyPoint(const gp_Pnt& assemblyPoint, const TRANSFORMDATA& data)
	{
		return gp_Pnt();
	}
	//void RobotBase::getEulerAngles(const gp_Vec& targetX, const gp_Vec& targetY, const gp_Vec& targetZ, double& angleX, double& angleY, double& angleZ)
	//{
	//	// 确保向量已单位化
	//	gp_Vec targetXCopy = targetX;
	//	gp_Vec targetYCopy = targetY;
	//	gp_Vec targetZCopy = targetZ;
	//	targetXCopy.Normalize();
	//	targetYCopy.Normalize();
	//	targetZCopy.Normalize();
	//
	//	// 构建旋转矩阵
	//	double R[3][3] = {
	//		{targetXCopy.X(), targetYCopy.Y(), targetZCopy.Z()},
	//		{targetXCopy.X(), targetYCopy.Y(), targetZCopy.Z()},
	//		{targetXCopy.X(), targetYCopy.Y(), targetZCopy.Z()}
	//	};
	//
	//	// 提取欧拉角 (XYZ顺序)
	//	if (R[2][0] > -1 && R[2][0] < 1) {
	//		angleY = asin(-R[2][0]);
	//		angleX = atan2(R[1][0] / cos(angleY), R[0][0] / cos(angleY));
	//		angleZ = atan2(R[2][1] / cos(angleY), R[2][2] / cos(angleY));
	//	}
	//	else {
	//		// 处理奇异情况
	//		angleY = M_PI / 2 * sign(R[2][0]);
	//		angleX = atan2(-R[0][1], R[0][2]);
	//		angleZ = 0; // 任意值
	//	}
	//
	//
	//	// 转换为度数
	//	angleX = angleX * 180 / M_PI;
	//	angleY = angleY * 180 / M_PI;
	//	angleZ = angleZ * 180 / M_PI;
	//
	//	/*gp_Dir stdX(1, 0, 0);
	//	gp_Dir stdY(0, 1, 0);
	//	gp_Dir stdZ(0, 0, 1);
	//
	//	gp_Trsf trsf;
	//	trsf.SetRotation(gp_Ax1(gp::Origin(), stdZ), 45 * M_PI / 180);
	//
	//	gp_Dir rotatedX = stdX.Transformed(trsf);
	//	gp_Dir rotatedY = stdY.Transformed(trsf);
	//	gp_Dir rotatedZ = stdZ.Transformed(trsf);
	//
	//	gp_Vec vecX(rotatedX.X(), rotatedX.Y(), rotatedX.Z());
	//	vecX.Normalize();
	//	gp_Vec vecY(rotatedY.X(), rotatedY.Y(), rotatedY.Z());
	//	vecY.Normalize();
	//	gp_Vec vecZ(rotatedZ.X(), rotatedZ.Y(), rotatedZ.Z());
	//	vecZ.Normalize();
	//
	//	std::cout << "Rotated X: " << rotatedX.X() << ", " << rotatedX.Y() << ", " << rotatedX.Z() << std::endl;
	//	std::cout << "Rotated Y: " << rotatedY.X() << ", " << rotatedY.Y() << ", " << rotatedY.Z() << std::endl;
	//	std::cout << "Rotated Z: " << rotatedZ.X() << ", " << rotatedZ.Y() << ", " << rotatedZ.Z() << std::endl;*/
	//}
	void RobotBase::workTransformRecursively(QString name, const TRANSFORMDATA& data)
	{
		ADDROBOTDATA addRobot = m_myRobotData.value(name);
		QVector<Handle(AIS_Shape)> newAisShapes;

		// 获取当前模型的装配点
		gp_Pnt assemblyPoint = addRobot.assemblyPoint;

		// 递归结束，统一删除旧模型并添加新模型
		for (auto& oldAisShape : addRobot.myAisShapes)
		{
			myContext->Remove(oldAisShape, Standard_True);
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
			newAisShapes.push_back(aisShapeTemp);
		}
		//addRobot.myAisShapes = newAisShapes;
		m_myRobotData.insert(name, addRobot);
		// 递归处理下一个组件
		if (!addRobot.nextShapeName.isEmpty())
		{
			TRANSFORMDATA dataSec = data;
			dataSec.angleX = 0;
			dataSec.angleY = 0;
			dataSec.angleZ = 0;

			workTransformRecursively(addRobot.nextShapeName, data);
		}
		m_myRobotData[name].myAisShapes = newAisShapes;
		/*for (const auto& shape : m_myRobotData[name].myAisShapes)
		{
			shape->SetColor(addRobot.color);
			myContext->Display(shape, Standard_True);
		}*/
	}
	gp_Vec RobotBase::getVectorByAngle(double angleX, double angleY, double angleZ)
	{
		// 定义初始向量
		gp_Vec vec(0, 0, 0);

		// 定义轴的起点
		gp_Pnt oriPoint(0, 0, 0);

		// 定义方向
		gp_Dir dirX(1, 0, 0);
		gp_Dir dirY(0, 1, 0);
		gp_Dir dirZ(0, 0, 1);

		// 定义旋转轴
		gp_Ax1 axX(oriPoint, dirX);
		gp_Ax1 axY(oriPoint, dirY);
		gp_Ax1 axZ(oriPoint, dirZ);

		// 初始化变换
		gp_Trsf trsfX, trsfY, trsfZ;
		trsfX.SetRotation(axX, angleX * M_PI / 180);
		trsfY.SetRotation(axY, angleY * M_PI / 180);
		trsfZ.SetRotation(axZ, angleZ * M_PI / 180);

		// 求 X轴的向量
		gp_Vec vecX = dirX.Transformed(trsfX);
		vecX = vecX.Transformed(trsfY);
		vecX = vecX.Transformed(trsfZ);
		// 求 Y轴的向量
		gp_Vec vecY = dirY.Transformed(trsfX);
		vecY = vecY.Transformed(trsfY);
		vecY = vecY.Transformed(trsfZ);
		// 求 Z轴的向量
		gp_Vec vecZ = dirZ.Transformed(trsfX);
		vecZ = vecZ.Transformed(trsfY);
		vecZ = vecZ.Transformed(trsfZ);
		return vec;
	}




	/*!
	Get paint engine for the OpenGL viewer. [ virtual public ]
	*/
	QPaintEngine* RobotBase::paintEngine() const
	{
		return 0;
	}

	void RobotBase::paintEvent(QPaintEvent* /*theEvent*/)
	{
		myView->Redraw();
	}

	void RobotBase::resizeEvent(QResizeEvent* /*theEvent*/)
	{
		if (!myView.IsNull())
		{
			myView->MustBeResized();
		}
	}

	void RobotBase::fitAll(void)
	{
		myView->FitAll();
		myView->ZFitAll();
		myView->Redraw();
	}

	void RobotBase::reset(void)
	{
		myView->Reset();
	}

	void RobotBase::pan(void)
	{
		myCurrentMode = CurAction3d_DynamicPanning;
	}

	void RobotBase::zoom(void)
	{
		myCurrentMode = CurAction3d_DynamicZooming;
	}

	void RobotBase::rotate(void)
	{
		myCurrentMode = CurAction3d_DynamicRotation;
		// 沿Z轴旋转
		//定义轴的起点
		gp_Pnt oriPoint(0, 0, 0);
		//定义方向
		gp_Dir dir(0, 1, 0);
		//定义旋转轴
		gp_Ax1 ax(oriPoint, dir);
		//定义一个变换
		gp_Trsf trsf;
		//设置旋转轴和旋转角度，这里以90°为例
		trsf.SetRotation(ax, 90 * 3.14 / 180);

		//执行旋转变换
		//获取要操作的shape
		TopoDS_Shape tempshape = getShape();
		BRepBuilderAPI_Transform aBRespTrsf(tempshape, trsf, true);
		aBRespTrsf.Build();
		TopoDS_Shape resShape = aBRespTrsf.Shape();
	}

	void RobotBase::slotUpdataRobotShaps(void)
	{
		// 
		QVariantMap robotData = QVariantMap();//ATJSTOOL->readJsonFile(COMPONENTATAMIROBOTDH).toVariantMap();
		if (m_DHRobot != nullptr)
		{
			m_DHRobot->close();
			m_DHRobot = nullptr;
		}
		m_DHRobot = new SDHRobot(this);
		readRobotDhParameter(robotData);

		// 移除所有模型
		myContext->RemoveAll(true);

		loadSTEPModel("", shape);

		addComponentRobotALL();
		//showSetParameter();

		initZeroParameter();
		initZeroPosition();

		//connect(AGACCORE, &AGACCORESLOT::singalRobotActiveData, this, &RobotBase::setRobotIsActive);
		//initZeroOrientation();
		// 开启定时器
		QTimer* timer1 = new QTimer(this);
		timer1->start(1000);
		connect(timer1, &QTimer::timeout, [=]() {
			/*connect(AGACCORE, &AGACCORESLOT::signalRobotZeroData, this, &RobotBase::initRobotRotateZero);
			connect(AGACCORE, &AGACCORESLOT::signalRobotInitZeroData, [=](QString zero) {
				initZeroParameter();
				initZeroOrientation();
				initRobotRotateZeroReStart(zero);

				});*/
			timer1->stop();
			});
	}


	void RobotBase::setRotateAngle(QVector<double> rotateAngle)
	{
		m_DHRobot->setDHParametersJointAngle(rotateAngle);
		// 机器人旋转角度
		setRotateAngle(ROBOT_1, 1);
		setRotateAngle(ROBOT_2, 2);
		setRotateAngle(ROBOT_3, 3);
		setRotateAngle(ROBOT_4, 4);
		setRotateAngle(ROBOT_5, 6);
		setRotateAngle(ROBOT_6, 8);

		addComponentRobotALL();
	}

	void RobotBase::setRotateAngle(QString name, int size)
	{
		ADDROBOTDATA addRobot1 = m_myRobotData.value(name);
		addRobot1.assemblyPoint = getCurrentAssemblyPoint(size);
		gp_Pnt temp = getCurrentRotationAngal(size);
		addRobot1.angleX = temp.X();
		addRobot1.angleY = temp.Y();
		addRobot1.angleZ = temp.Z();

		m_myRobotData.insert(name, addRobot1);
	}

	void RobotBase::readRobotDhParameter(QVariantMap data)
	{
		QVariantMap jointData1 = data.value("Joint_1").toMap();
		QVariantMap jointData2 = data.value("Joint_2").toMap();
		QVariantMap jointData3 = data.value("Joint_3").toMap();
		QVariantMap jointData4 = data.value("Joint_4").toMap();
		QVariantMap jointData5_1 = data.value("Joint_5_1").toMap();
		QVariantMap jointData5_2 = data.value("Joint_5_2").toMap();
		QVariantMap jointData6_1 = data.value("Joint_6_1").toMap();
		QVariantMap jointData6_2 = data.value("Joint_6_2").toMap();
		QVariantMap jointData7 = data.value("Joint_7").toMap();
		QVariantMap jointData8 = data.value("Joint_8").toMap();
		QVariantMap jointData9 = data.value("Joint_9").toMap();

		// 读取工件路径
		QVariantMap workData = data.value("Part").toMap();
		QVariantMap Long_Guide = data.value("Long_Guide").toMap();
		QVariantMap Short_Guide = data.value("Short_Guide").toMap();
		QVariantMap Turn_Table = data.value("Turn_Table").toMap();

		QVector<QVector<double>> dhParameter;
		QVector<double> temp1, temp2, temp3, temp4, temp5_1, temp5_2, temp6_1, temp6_2, temp7, temp8, temp9;
		temp1.append(jointData1.value("a").toDouble());
		temp1.append(jointData1.value("alpha").toDouble());
		temp1.append(jointData1.value("d").toDouble());
		temp1.append(jointData1.value("theta").toDouble());

		temp2.append(jointData2.value("a").toDouble());
		temp2.append(jointData2.value("alpha").toDouble());
		temp2.append(jointData2.value("d").toDouble());
		temp2.append(jointData2.value("theta").toDouble());

		temp3.append(jointData3.value("a").toDouble());
		temp3.append(jointData3.value("alpha").toDouble());
		temp3.append(jointData3.value("d").toDouble());
		temp3.append(jointData3.value("theta").toDouble());

		temp4.append(jointData4.value("a").toDouble());
		temp4.append(jointData4.value("alpha").toDouble());
		temp4.append(jointData4.value("d").toDouble());
		temp4.append(jointData4.value("theta").toDouble());

		temp5_1.append(jointData5_1.value("a").toDouble());
		temp5_1.append(jointData5_1.value("alpha").toDouble());
		temp5_1.append(jointData5_1.value("d").toDouble());
		temp5_1.append(jointData5_1.value("theta").toDouble());

		temp5_2.append(jointData5_2.value("a").toDouble());
		temp5_2.append(jointData5_2.value("alpha").toDouble());
		temp5_2.append(jointData5_2.value("d").toDouble());
		temp5_2.append(jointData5_2.value("theta").toDouble());


		temp6_1.append(jointData6_1.value("a").toDouble());
		temp6_1.append(jointData6_1.value("alpha").toDouble());
		temp6_1.append(jointData6_1.value("d").toDouble());
		temp6_1.append(jointData6_1.value("theta").toDouble());

		temp6_2.append(jointData6_2.value("a").toDouble());
		temp6_2.append(jointData6_2.value("alpha").toDouble());
		temp6_2.append(jointData6_2.value("d").toDouble());
		temp6_2.append(jointData6_2.value("theta").toDouble());

		temp7.append(jointData7.value("a").toDouble());
		temp7.append(jointData7.value("alpha").toDouble());
		temp7.append(jointData7.value("d").toDouble());
		temp7.append(jointData7.value("theta").toDouble());

		temp8.append(jointData8.value("a").toDouble());
		temp8.append(jointData8.value("alpha").toDouble());
		temp8.append(jointData8.value("d").toDouble());
		temp8.append(jointData8.value("theta").toDouble());

		temp9.append(jointData9.value("a").toDouble());
		temp9.append(jointData9.value("alpha").toDouble());
		temp9.append(jointData9.value("d").toDouble());
		temp9.append(jointData9.value("theta").toDouble());

		dhParameter.append(temp1);
		dhParameter.append(temp2);
		dhParameter.append(temp3);
		dhParameter.append(temp4);
		dhParameter.append(temp5_1);
		dhParameter.append(temp5_2);
		dhParameter.append(temp6_1);
		dhParameter.append(temp6_2);
		dhParameter.append(temp7);
		dhParameter.append(temp8);
		dhParameter.append(temp9);

		m_DHRobot->setDHParameters(dhParameter);

		ADDROBOTDATA addRobot1;
		addRobot1.a = jointData1.value("a").toDouble();
		addRobot1.alpha = jointData1.value("alpha").toDouble();
		addRobot1.d = jointData1.value("d").toDouble();
		addRobot1.theta = jointData1.value("theta").toDouble();
		addRobot1.path = jointData1.value("path").toString();

		m_myRobotDHData.insert(ROBOT_1, addRobot1);

		ADDROBOTDATA addRobot2;
		addRobot2.a = jointData2.value("a").toDouble();
		addRobot2.alpha = jointData2.value("alpha").toDouble();
		addRobot2.d = jointData2.value("d").toDouble();
		addRobot2.theta = jointData2.value("theta").toDouble();
		addRobot2.path = jointData2.value("path").toString();

		m_myRobotDHData.insert(ROBOT_2, addRobot2);

		ADDROBOTDATA addRobot3;
		addRobot3.a = jointData3.value("a").toDouble();
		addRobot3.alpha = jointData3.value("alpha").toDouble();
		addRobot3.d = jointData3.value("d").toDouble();
		addRobot3.theta = jointData3.value("theta").toDouble();
		addRobot3.path = jointData3.value("path").toString();

		m_myRobotDHData.insert(ROBOT_3, addRobot3);

		ADDROBOTDATA addRobot4;
		addRobot4.a = jointData4.value("a").toDouble();
		addRobot4.alpha = jointData4.value("alpha").toDouble();
		addRobot4.d = jointData4.value("d").toDouble();
		addRobot4.theta = jointData4.value("theta").toDouble();
		addRobot4.path = jointData4.value("path").toString();

		m_myRobotDHData.insert(ROBOT_4, addRobot4);

		ADDROBOTDATA addRobot5; 
		addRobot5.a = jointData5_2.value("a").toDouble();
		addRobot5.alpha = jointData5_2.value("alpha").toDouble();
		addRobot5.d = jointData5_2.value("d").toDouble();
		addRobot5.theta = jointData5_2.value("theta").toDouble();
		addRobot5.path = jointData5_1.value("path").toString();

		m_myRobotDHData.insert(ROBOT_5, addRobot5);

		ADDROBOTDATA addRobot6;
		addRobot6.a = jointData6_1.value("a").toDouble();
		addRobot6.alpha = jointData6_1.value("alpha").toDouble();
		addRobot6.d = jointData6_1.value("d").toDouble();
		addRobot6.theta = jointData6_1.value("theta").toDouble();
		addRobot6.path = jointData6_1.value("path").toString();

		m_myRobotDHData.insert(ROBOT_6, addRobot6);

		ADDROBOTDATA work;
		work.x = workData.value("x").toDouble();
		work.y = workData.value("y").toDouble();
		work.z = workData.value("z").toDouble();
		work.angleX = workData.value("x_angle").toDouble();
		work.angleY = workData.value("y_angle").toDouble();
		work.angleZ = workData.value("z_angle").toDouble();
		work.path = workData.value("path").toString();
		m_myRobotDHData.insert(GONGJIAN, work);

		ADDROBOTDATA LongGuideADD;
		LongGuideADD.x = Long_Guide.value("x").toDouble();
		LongGuideADD.y = Long_Guide.value("y").toDouble();
		LongGuideADD.z = Long_Guide.value("z").toDouble();
		LongGuideADD.angleX = Long_Guide.value("x_angle").toDouble();
		LongGuideADD.angleY = Long_Guide.value("y_angle").toDouble();
		LongGuideADD.angleZ = Long_Guide.value("z_angle").toDouble();
		LongGuideADD.path = Long_Guide.value("path").toString();
		m_myRobotDHData.insert(DAOGUIMOVE, LongGuideADD);

		ADDROBOTDATA ShortGuideADD;
		ShortGuideADD.x = Short_Guide.value("x").toDouble();
		ShortGuideADD.y = Short_Guide.value("y").toDouble();
		ShortGuideADD.z = Short_Guide.value("z").toDouble();
		ShortGuideADD.angleX = Short_Guide.value("x_angle").toDouble();
		ShortGuideADD.angleY = Short_Guide.value("y_angle").toDouble();
		ShortGuideADD.angleZ = Short_Guide.value("z_angle").toDouble();
		ShortGuideADD.path = Short_Guide.value("path").toString();
		m_myRobotDHData.insert(SHORTGUIDE, ShortGuideADD);

		ADDROBOTDATA TurnTableADD;
		TurnTableADD.x = Turn_Table.value("x").toDouble();
		TurnTableADD.y = Turn_Table.value("y").toDouble();
		TurnTableADD.z = Turn_Table.value("z").toDouble();
		TurnTableADD.angleX = Turn_Table.value("x_angle").toDouble();
		TurnTableADD.angleY = Turn_Table.value("y_angle").toDouble();
		TurnTableADD.angleZ = Turn_Table.value("z_angle").toDouble();
		TurnTableADD.path = Turn_Table.value("path").toString();
		m_myRobotDHData.insert(YUANPAN, TurnTableADD);


		{
			QStringList dataList = data.value("Zero").toString().split(",");
			if (dataList.size() == 8)
			{
				TRANSFORMDATA data1;
				data1.name = "Robot_1";
				data1.rotationAngle = dataList[0].toDouble();

				TRANSFORMDATA data2;
				data2.name = "Robot_2";
				data2.rotationAngle = dataList[1].toDouble();

				TRANSFORMDATA data3;
				data3.name = "Robot_3";
				data3.rotationAngle = dataList[2].toDouble();

				TRANSFORMDATA data4;
				data4.name = "Robot_4";
				data4.rotationAngle = dataList[3].toDouble();

				TRANSFORMDATA data5;
				data5.name = "Robot_5";
				data5.rotationAngle = dataList[4].toDouble();

				TRANSFORMDATA data6;
				data6.name = "Robot_6";
				data6.rotationAngle = dataList[5].toDouble();

				m_lastTransformData.insert(ROBOT_1, data1);
				m_lastTransformData.insert(ROBOT_2, data2);
				m_lastTransformData.insert(ROBOT_3, data3);
				m_lastTransformData.insert(ROBOT_4, data4);
				m_lastTransformData.insert(ROBOT_5, data5);
				m_lastTransformData.insert(ROBOT_6, data6);

				m_shortMove = dataList[6].toInt();
				m_workRotateAngle = dataList[7].toInt();
			}
			else
			{
				TRANSFORMDATA data1;
				data1.name = "Robot_1";
				data1.rotationAngle = 0;

				TRANSFORMDATA data2;
				data2.name = "Robot_2";
				data2.rotationAngle = 0;

				TRANSFORMDATA data3;
				data3.name = "Robot_3";
				data3.rotationAngle = 0;

				TRANSFORMDATA data4;
				data4.name = "Robot_4";
				data4.rotationAngle = 0;

				TRANSFORMDATA data5;
				data5.name = "Robot_5";
				data5.rotationAngle = 0;

				TRANSFORMDATA data6;
				data6.name = "Robot_6";
				data6.rotationAngle = 0;

				m_shortMove = 0;
				m_workRotateAngle = 0;

				m_lastTransformData.insert(ROBOT_1, data1);
				m_lastTransformData.insert(ROBOT_2, data2);
				m_lastTransformData.insert(ROBOT_3, data3);
				m_lastTransformData.insert(ROBOT_4, data4);
				m_lastTransformData.insert(ROBOT_5, data5);
				m_lastTransformData.insert(ROBOT_6, data6);
			}

		}

	}

	void RobotBase::initZeroParameter()
	{
		QVariantMap data = QVariantMap();// ATJSTOOL->readJsonFile(COMPONENTATAMIROBOTDH).toVariantMap();
		QStringList dataList = data.value("Zero").toString().split(",");
		QStringList dataActiveList = data.value("ZeroActive").toString().split(",");

		if (dataList.size() == 8 && dataActiveList.size() == 8)
		{
			TRANSFORMDATA data1;
			data1.name = "Robot_1";
			data1.rotationAngle = dataList[0].toDouble();
			data1.rotationActiveAngle = dataActiveList[0].toInt();

			TRANSFORMDATA data2;
			data2.name = "Robot_2";
			data2.rotationAngle = dataList[1].toDouble();
			data2.rotationActiveAngle = dataActiveList[1].toInt();

			TRANSFORMDATA data3;
			data3.name = "Robot_3";
			data3.rotationAngle = dataList[2].toDouble();
			data3.rotationActiveAngle = dataActiveList[2].toInt();

			TRANSFORMDATA data4;
			data4.name = "Robot_4";
			data4.rotationAngle = dataList[3].toDouble();
			data4.rotationActiveAngle = dataActiveList[3].toInt();

			TRANSFORMDATA data5;
			data5.name = "Robot_5";
			data5.rotationAngle = dataList[4].toDouble();
			data5.rotationActiveAngle = dataActiveList[4].toInt();

			TRANSFORMDATA data6;
			data6.name = "Robot_6";
			data6.rotationAngle = dataList[5].toDouble();
			data6.rotationActiveAngle = dataActiveList[5].toInt();

			TRANSFORMDATA data7;
			data7.name = "platMove";
			data7.rotationAngle = dataList[6].toDouble();
			data7.rotationActiveAngle = dataActiveList[6].toInt();

			TRANSFORMDATA data8;
			data8.name = "workRotate";
			data8.rotationAngle = dataList[7].toDouble();
			data8.rotationActiveAngle = dataActiveList[7].toInt();

			m_lastTransformData.insert(ROBOT_1, data1);
			m_lastTransformData.insert(ROBOT_2, data2);
			m_lastTransformData.insert(ROBOT_3, data3);
			m_lastTransformData.insert(ROBOT_4, data4);
			m_lastTransformData.insert(ROBOT_5, data5);
			m_lastTransformData.insert(ROBOT_6, data6);
			m_lastTransformData.insert("platMove", data7);
			m_lastTransformData.insert("workRotate", data8);

			m_originTransformData = m_lastTransformData;

			m_shortMove = dataList[6].toInt();
			m_shortActiveMove = dataActiveList[6].toInt();

			m_workRotateAngle = dataList[7].toInt();
			m_workRotateActiveAngle = dataActiveList[7].toInt();
		}
	}

	void RobotBase::initZeroPosition()
	{
		QVector<ROTATEDATA> angles;
		ROTATEDATA rotateData1;
		rotateData1.name = "Robot_1";
		rotateData1.angle = m_originTransformData["Robot_1"].rotationAngle;;
		rotateData1.direction = MoveDirection::MoveDirection_ZAxis;
		angles.push_back(rotateData1);

		ROTATEDATA rotateData2;
		rotateData2.name = "Robot_2";
		rotateData2.angle = m_originTransformData["Robot_2"].rotationAngle;
		rotateData2.direction = MoveDirection::MoveDirection_ZAxis;
		angles.push_back(rotateData2);

		ROTATEDATA rotateData3;
		rotateData3.name = "Robot_3";
		rotateData3.angle = m_originTransformData["Robot_3"].rotationAngle;
		rotateData3.direction = MoveDirection::MoveDirection_ZAxis;
		angles.push_back(rotateData3);

		ROTATEDATA rotateData4;
		rotateData4.name = "Robot_4";
		rotateData4.angle = m_originTransformData["Robot_4"].rotationAngle;
		rotateData4.direction = MoveDirection::MoveDirection_ZAxis;
		angles.push_back(rotateData4);

		ROTATEDATA rotateData5;
		rotateData5.name = "Robot_5";
		rotateData5.angle = m_originTransformData["Robot_5"].rotationAngle;
		rotateData5.direction = MoveDirection::MoveDirection_YAxis;
		angles.push_back(rotateData5);

		ROTATEDATA rotateData6;
		rotateData6.name = "Robot_6";
		rotateData6.angle = m_originTransformData["Robot_6"].rotationAngle;
		rotateData6.direction = MoveDirection::MoveDirection_ZAxis;
		angles.push_back(rotateData6);

		robotRotate(angles);
		platformMove(m_shortMove);
		addWorkRotot(m_workRotateAngle, GONGJIAN);
		//workRotate(180, GONGJIAN, MoveDirection_ZAxis);
		//ADDROBOTDATA work = m_myRobotData.value(GONGJIAN);
		//work.assemblyPoint = gp_Pnt(m_shortMove, work.assemblyPoint.Y(), work.assemblyPoint.Z());
		///*foreach(auto workShaps, work.myAisShapes)
		//{
		//	myContext->Erase(workShaps, Standard_True);


		//}*/
		//work.angleZ = 90;
		//m_myRobotData.insert(GONGJIAN, work);
		//addComponentRobot(work);
		//workRotate(m_workRotateAngle, GONGJIAN,MoveDirection_ZAxis);
		//workRotate(m_workRotateAngle, YUANPAN);
	}

	void RobotBase::initZeroOrientation()
	{
		TRANSFORMDATA data1;
		data1.name = "Robot_1";
		data1.rotationAngle = 0;
		data1.rotationActiveAngle = 0;

		TRANSFORMDATA data2;
		data2.name = "Robot_2";
		data2.rotationAngle = 0;
		data2.rotationActiveAngle = 0;

		TRANSFORMDATA data3;
		data3.name = "Robot_3";
		data3.rotationAngle = 0;
		data3.rotationActiveAngle = 0;

		TRANSFORMDATA data4;
		data4.name = "Robot_4";
		data4.rotationAngle = 0;
		data4.rotationActiveAngle = 0;

		TRANSFORMDATA data5;
		data5.name = "Robot_5";
		data5.rotationAngle = 0;
		data5.rotationActiveAngle = 0;

		TRANSFORMDATA data6;
		data6.name = "Robot_6";
		data6.rotationAngle = 0;
		data6.rotationActiveAngle = 0;

		TRANSFORMDATA data7;
		data7.name = "platMove";
		data7.rotationAngle = 0;
		data7.rotationActiveAngle = 0;

		TRANSFORMDATA data8;
		data8.name = "workRotate";
		data8.rotationAngle = 0;
		data8.rotationActiveAngle = 0;

		m_shortMove = 0;
		m_shortActiveMove = 0;
		m_workRotateAngle = 0;
		m_workRotateActiveAngle = 0;

		m_lastTransformData.insert(ROBOT_1, data1);
		m_lastTransformData.insert(ROBOT_2, data2);
		m_lastTransformData.insert(ROBOT_3, data3);
		m_lastTransformData.insert(ROBOT_4, data4);
		m_lastTransformData.insert(ROBOT_5, data5);
		m_lastTransformData.insert(ROBOT_6, data6);
		m_lastTransformData.insert("platMove", data7);
		m_lastTransformData.insert("workRotate", data8);

		m_originTransformData = m_lastTransformData;
	}


	gp_Pnt RobotBase::getCurrentAssemblyPoint(int size)
	{
		QVector<double> temp = m_DHRobot->getCurrentExtractPosition(size);
		return gp_Pnt(temp[0], temp[1], temp[2]);
	}

	gp_Pnt RobotBase::getCurrentRotationAngal(int size)
	{
		QVector<double> temp = m_DHRobot->getCurrentExtractRotation(size);
		return gp_Pnt(temp[0], temp[1], temp[2]);
	}

	void RobotBase::showSetParameter()
	{
		if (m_diaog != nullptr)
		{
			m_diaog->close();
			m_diaog = nullptr;
		}
		m_diaog = new QDialog(this);
		m_diaog->setWindowTitle("调参窗口");
		QGridLayout* gridLayout = new QGridLayout(m_diaog);

		// 选择模型
		QLabel* label1 = new QLabel(m_diaog);
		label1->setText("选择模型：");
		gridLayout->addWidget(label1, 0, 0);

		QComboBox* comboBox1 = new QComboBox(m_diaog);
		QStringList items = m_myRobotData.keys();
		comboBox1->addItems(items);
		comboBox1->setCurrentText(items[0]);
		gridLayout->addWidget(comboBox1, 0, 1);

		ADDROBOTDATA addRobot = m_myRobotData.value(comboBox1->currentText());
		// 选择X旋转轴
		QLabel* label2 = new QLabel(m_diaog);
		label2->setText("X旋转轴：");
		gridLayout->addWidget(label2, 1, 0);

		QDoubleSpinBox* spin = new QDoubleSpinBox(m_diaog);
		spin->setValue(addRobot.angleX);
		spin->setRange(-180, 180);
		spin->setSingleStep(1);
		spin->setDecimals(0);
		gridLayout->addWidget(spin, 1, 1);

		// 选择Y旋转轴
		QLabel* label3 = new QLabel(m_diaog);
		label3->setText("Y旋转轴：");
		gridLayout->addWidget(label3, 2, 0);

		QDoubleSpinBox* spinY = new QDoubleSpinBox(m_diaog);
		spinY->setValue(addRobot.angleY);
		spinY->setRange(-180, 180);
		spinY->setSingleStep(1);
		spinY->setDecimals(0);
		gridLayout->addWidget(spinY, 2, 1);

		// 选择Z旋转轴
		QLabel* label4 = new QLabel(m_diaog);
		label4->setText("Z旋转轴：");
		gridLayout->addWidget(label4, 3, 0);

		QDoubleSpinBox* spinZ = new QDoubleSpinBox(m_diaog);
		spinZ->setValue(addRobot.angleZ);
		spinZ->setRange(-180, 180);
		spinZ->setSingleStep(1);
		spinZ->setDecimals(0);
		gridLayout->addWidget(spinZ, 3, 1);

		// 选择装配点
		QLabel* label5 = new QLabel(m_diaog);
		label5->setText("装配点X：");
		gridLayout->addWidget(label5, 4, 0);

		QLineEdit* lineEdit1 = new QLineEdit(m_diaog);
		lineEdit1->setText(QString("%1").arg(addRobot.assemblyPoint.X()));

		gridLayout->addWidget(lineEdit1, 4, 1);

		QLabel* label6 = new QLabel(m_diaog);
		label6->setText("装配点Y：");
		gridLayout->addWidget(label6, 5, 0);

		QLineEdit* lineEdit2 = new QLineEdit(m_diaog);
		lineEdit2->setText(QString("%1").arg(addRobot.assemblyPoint.Y()));
		gridLayout->addWidget(lineEdit2, 5, 1);

		QLabel* label7 = new QLabel(m_diaog);
		label7->setText("装配点Z：");
		gridLayout->addWidget(label7, 6, 0);

		QLineEdit* lineEdit3 = new QLineEdit(m_diaog);
		lineEdit3->setText(QString("%1").arg(addRobot.assemblyPoint.Z()));
		gridLayout->addWidget(lineEdit3, 6, 1);

		// 选择平移点
		QLabel* label8 = new QLabel(m_diaog);
		label8->setText("平移X：");
		gridLayout->addWidget(label8, 7, 0);

		QLineEdit* lineEdit4 = new QLineEdit(m_diaog);
		lineEdit4->setText(QString("0"));
		gridLayout->addWidget(lineEdit4, 7, 1);

		QLabel* label9 = new QLabel(m_diaog);
		label9->setText("颜色设置:");
		gridLayout->addWidget(label9, 8, 0);

		/*QLineEdit* lineEdit5 = new QLineEdit(diaog);
		lineEdit5->setText(QString("0"));
		gridLayout->addWidget(lineEdit5, 8, 1);*/

		QPushButton* button1 = new QPushButton(m_diaog);
		button1->setText("打开颜色设置");
		gridLayout->addWidget(button1, 8, 1);
		connect(button1, &QPushButton::clicked, [=]() {
			ADDROBOTDATA addRobotA = m_myRobotData.value(comboBox1->currentText());
			QColor color = QColorDialog::getColor(QColor(addRobotA.color.Red() * 255,
				addRobotA.color.Green() * 255,
				addRobotA.color.Blue() * 255),
				this, "选择颜色");
			if (color.isValid()) {

				double red = color.red() / 255.0;
				double green = color.green() / 255.0;
				double blue = color.blue() / 255.0;
				Quantity_Color colorQuantity(red, green, blue, Quantity_TOC_sRGB);
				addRobotA.color = colorQuantity;
				addComponentRobot(addRobotA);
			}
			});


		QLabel* label10 = new QLabel(m_diaog);
		label10->setText("平移点Z：");
		gridLayout->addWidget(label10, 9, 0);

		QLineEdit* lineEdit6 = new QLineEdit(m_diaog);
		lineEdit6->setText(QString("%1").arg(addRobot.translationPoint.Z()));
		gridLayout->addWidget(lineEdit6, 9, 1);




		connect(comboBox1, &QComboBox::currentTextChanged, [=](const QString& text) {
			ADDROBOTDATA addRobotA = m_myRobotData.value(comboBox1->currentText());
			spin->setValue(addRobotA.angleX);
			spinY->setValue(addRobotA.angleY);
			spinZ->setValue(addRobotA.angleZ);
			lineEdit1->setText(QString("%1").arg(addRobotA.assemblyPoint.X()));
			lineEdit2->setText(QString("%1").arg(addRobotA.assemblyPoint.Y()));
			lineEdit3->setText(QString("%1").arg(addRobotA.assemblyPoint.Z()));
			lineEdit4->setText(QString("%1").arg(addRobotA.translationPoint.X()));
			//lineEdit5->setText(QString("%1").arg(addRobotA.translationPoint.Y()));
			lineEdit6->setText(QString("%1").arg(addRobotA.translationPoint.Z()));
			addComponentRobot(addRobotA);
			});


		connect(spin, &QDoubleSpinBox::editingFinished, [=]() {
			ADDROBOTDATA addRobotA = m_myRobotData.value(comboBox1->currentText());
			addRobotA.angleX = spin->value();
			m_myRobotData.insert(comboBox1->currentText(), addRobotA);
			addComponentRobot(addRobotA);

			});

		connect(spinY, &QDoubleSpinBox::editingFinished, [=]() {
			ADDROBOTDATA addRobotA = m_myRobotData.value(comboBox1->currentText());
			addRobotA.angleY = spinY->value();
			m_myRobotData.insert(comboBox1->currentText(), addRobotA);
			addComponentRobot(addRobotA);

			});

		connect(spinZ, &QDoubleSpinBox::editingFinished, [=]() {
			ADDROBOTDATA addRobotA = m_myRobotData.value(comboBox1->currentText());
			addRobotA.angleZ = spinZ->value();
			m_myRobotData.insert(comboBox1->currentText(), addRobotA);
			addComponentRobot(addRobotA);

			});

		connect(lineEdit1, &QLineEdit::editingFinished, [=]() {
			ADDROBOTDATA addRobotA = m_myRobotData.value(comboBox1->currentText());
			double x = lineEdit1->text().toDouble();
			double y = lineEdit2->text().toDouble();
			double z = lineEdit3->text().toDouble();
			addRobotA.assemblyPoint = gp_Pnt(x, y, z);
			m_myRobotData.insert(comboBox1->currentText(), addRobotA);
			addComponentRobot(addRobotA);

			});

		connect(lineEdit2, &QLineEdit::editingFinished, [=]() {
			ADDROBOTDATA addRobotA = m_myRobotData.value(comboBox1->currentText());
			double x = lineEdit1->text().toDouble();
			double y = lineEdit2->text().toDouble();
			double z = lineEdit3->text().toDouble();
			addRobotA.assemblyPoint = gp_Pnt(x, y, z);
			m_myRobotData.insert(comboBox1->currentText(), addRobotA);
			addComponentRobot(addRobotA);

			});

		connect(lineEdit3, &QLineEdit::editingFinished, [=]() {
			ADDROBOTDATA addRobotA = m_myRobotData.value(comboBox1->currentText());
			double x = lineEdit1->text().toDouble();
			double y = lineEdit2->text().toDouble();
			double z = lineEdit3->text().toDouble();
			addRobotA.assemblyPoint = gp_Pnt(x, y, z);
			m_myRobotData.insert(comboBox1->currentText(), addRobotA);
			addComponentRobot(addRobotA);

			});

		connect(lineEdit4, &QLineEdit::editingFinished, [=]() {
			TRANSFORMDATA transformData;
			transformData.name = comboBox1->currentText();

			transformData.translation = gp_XYZ(lineEdit4->text().toDouble(), 0, 0);
			workTransform(transformData);

			});

		/*connect(lineEdit5, &QLineEdit::editingFinished, [=]() {
			TRANSFORMDATA transformData;
			transformData.name = comboBox1->currentText();
			ADDROBOTDATA addRobotA = m_myRobotData.value(comboBox1->currentText());
			transformData.assemblyPoint = addRobotA.assemblyPoint;
			transformData.orientationAngleX = addRobotA.angleX;
			transformData.orientationAngleY = addRobotA.angleY;
			transformData.orientationAngleZ = addRobotA.angleZ;

			transformData.angleZ = lineEdit5->text().toDouble();
			workTransform(transformData);

			});*/

		connect(lineEdit6, &QLineEdit::editingFinished, [=]() {
			/*TRANSFORMDATA transformData;
			transformData.name = comboBox1->currentText();
			ADDROBOTDATA addRobotA = m_myRobotData.value(comboBox1->currentText());
			transformData.assemblyPoint = addRobotA.assemblyPoint;
			transformData.orientationAngleX = addRobotA.angleX;
			transformData.orientationAngleY = addRobotA.angleY;
			transformData.orientationAngleZ = addRobotA.angleZ;

			transformData.angleY = lineEdit6->text().toDouble();
			workTransform(transformData);*/

			QStringList items = lineEdit6->text().split(',');
			if (items.size() == 6) {

				/*double r1 = items[0].toDouble();
				double r2 = items[1].toDouble();
				double r3 = items[2].toDouble();
				double r4 = items[3].toDouble();
				double r5 = items[4].toDouble();
				double r6 = items[5].toDouble();
				double r7 = items[6].toDouble();
				double r8 = items[7].toDouble();*/
				double r1 = items[0].toDouble();
				double r2 = items[1].toDouble();
				double r3 = items[2].toDouble();
				double r4 = items[3].toDouble();
				double r5 = items[4].toDouble();
				double r6 = items[5].toDouble();

				QVector<ROTATEDATA> angles;
				ROTATEDATA rotateData1;
				rotateData1.name = "Robot_1";
				rotateData1.angle = r1;
				rotateData1.direction = MoveDirection::MoveDirection_ZAxis;
				angles.push_back(rotateData1);

				ROTATEDATA rotateData2;
				rotateData2.name = "Robot_2";
				rotateData2.angle = r2;
				rotateData2.direction = MoveDirection::MoveDirection_ZAxis;
				angles.push_back(rotateData2);

				ROTATEDATA rotateData3;
				rotateData3.name = "Robot_3";
				rotateData3.angle = r3;
				rotateData3.direction = MoveDirection::MoveDirection_ZAxis;
				angles.push_back(rotateData3);

				ROTATEDATA rotateData4;
				rotateData4.name = "Robot_4";
				rotateData4.angle = r4;
				rotateData4.direction = MoveDirection::MoveDirection_ZAxis;
				angles.push_back(rotateData4);

				ROTATEDATA rotateData5;
				rotateData5.name = "Robot_5";
				rotateData5.angle = r5;
				rotateData5.direction = MoveDirection::MoveDirection_ZAxis;
				angles.push_back(rotateData5);

				ROTATEDATA rotateData6;
				rotateData6.name = "Robot_6";
				rotateData6.angle = r6;
				rotateData6.direction = MoveDirection::MoveDirection_ZAxis;
				angles.push_back(rotateData6);

				/*ROTATEDATA rotateData7;
				rotateData7.name = "Robot_7";
				rotateData7.angle = r7;
				rotateData7.direction = MoveDirection::MoveDirection_ZAxis;
				angles.push_back(rotateData7);*/

				/*ROTATEDATA rotateData8;
				rotateData8.name = "Robot_8";
				rotateData8.angle = r8;
				rotateData8.direction = MoveDirection::MoveDirection_ZAxis;
				angles.push_back(rotateData8);*/

				robotRotate(angles);
			}
			});
		m_diaog->show();

	}

	void RobotBase::platformMove(double moveValue, MoveDirection direction)
	{
		addWorkMove(moveValue, SHORTGUIDE);
		QVector<LoadRobotTransformResult> results = loadRobotTransformInParallel(SHORTGUIDE);
		displayAllRobotShapes(results);
		//displayCoordinateAxes();
	}

	void RobotBase::workRotate(double angle, QString workName, MoveDirection direction)
	{
		TRANSFORMDATA data;
		data.name = GONGJIAN;
		ADDROBOTDATA addRobotA = m_myRobotData.value(GONGJIAN);
		data.assemblyPoint = addRobotA.assemblyPoint;
		data.rotationAngle = angle;
		switch (direction)
		{
		case MoveDirection::MoveDirection_XAxis:
		{
			data.rotationAxis = m_myRobotData.value(GONGJIAN).xDirCurrent;
			data.angleX = angle;

			break;
		}
		case MoveDirection::MoveDirection_YAxis:
		{
			data.rotationAxis = m_myRobotData.value(GONGJIAN).yDirCurrent;
			data.angleY = angle;


			break;
		}
		case MoveDirection::MoveDirection_ZAxis:
		{
			data.angleZ = angle;


			data.rotationAxis = m_myRobotData.value(GONGJIAN).zDirCurrent;
			break;
		}
		default:
			break;
		}
		m_myRobotData.insert(GONGJIAN, addRobotA);
		//m_lastTransformData.insert(keys[count], data);
		workTransform(data);

	}

	bool RobotBase::isEqualDouble(double a, double b)
	{
		if (fabs(a - b) < m_equalCompareValue)
		{
			return true;
		}
		return false;
	}

	void RobotBase::setEqualCompareValue(double value)
	{
		m_equalCompareValue = value;
	}

	// 该方法用于检查机器人当前的移动值是否与上一次的移动值相等
	// 如果相等则返回true，否则更新移动值并返回false
	bool RobotBase::platMoveCurrent(double moveValue)
	{
		bool isEqual = isEqualDouble(moveValue, m_lastMove);
		if (isEqual)
		{
			return isEqual;
		}
		m_lastMove = moveValue;
		return false;
	}


	bool RobotBase::workRotateCurrent(double angle)
	{
		bool isEqual = isEqualDouble(angle, m_lastWorkRotateAngle);
		if (isEqual)
		{
			return isEqual;
		}
		m_lastWorkRotateAngle = angle;
		return false;
	}

	bool RobotBase::setRobotTransform(QString data)
	{
		QStringList items = data.split(',');
		QStringList keys = m_lastRobotRotateAngle.split(',');
		bool isEqual = true;
		for (int i = 0; i < items.size(), i < keys.size(); i++)
		{
			double angle = items[i].toDouble();
			double key = keys[i].toDouble();
			isEqual = isEqualDouble(angle, key);
			if (!isEqual)
			{
				m_lastRobotRotateAngle = data;
				return true;
			}
		}
		return false;
	}

	void RobotBase::initRobotRotateZero(QString zero)
	{
		/*if (!getRobotIsActive())
		{
			return;
		}*/
		QStringList items = zero.split(',');
		if (items.size() == 8) {

			double r1 = items[0].toDouble();
			double r2 = items[1].toDouble();
			double r3 = items[2].toDouble();
			double r4 = items[3].toDouble();
			double r5 = items[4].toDouble();
			double r6 = items[5].toDouble();
			double r7 = items[6].toDouble();
			double r8 = items[7].toDouble();

			// 机器人实际位置
			QStringList keys;
			keys << items[0] << items[1] << items[2] << items[3] << items[4] << items[5];
			QString currentRobot = keys.join(",");

			// 实际变量 = 当前位置 - 零点
			double currentRobot1 = r1 - m_originTransformData["Robot_1"].rotationActiveAngle;
			double currentRobot2 = r2 - m_originTransformData["Robot_2"].rotationActiveAngle;
			double currentRobot3 = r3 - m_originTransformData["Robot_3"].rotationActiveAngle;
			double currentRobot4 = r4 - m_originTransformData["Robot_4"].rotationActiveAngle;
			double currentRobot5 = r5 - m_originTransformData["Robot_5"].rotationActiveAngle;
			double currentRobot6 = r6 - m_originTransformData["Robot_6"].rotationActiveAngle;

			double platMoveValue = r7 - m_originTransformData["platMove"].rotationActiveAngle;
			double workRotateAngle = r8 - m_originTransformData["workRotate"].rotationActiveAngle;

			// 模型移动角度  = 模型零点角度 + 实际变量
			double robotShapsAngle1 = m_originTransformData["Robot_1"].rotationAngle + currentRobot1;
			double robotShapsAngle2 = m_originTransformData["Robot_2"].rotationAngle + currentRobot2;
			double robotShapsAngle3 = m_originTransformData["Robot_3"].rotationAngle + currentRobot3;
			double robotShapsAngle4 = m_originTransformData["Robot_4"].rotationAngle + currentRobot4;
			double robotShapsAngle5 = m_originTransformData["Robot_5"].rotationAngle + currentRobot5;
			double robotShapsAngle6 = m_originTransformData["Robot_6"].rotationAngle + currentRobot6;

			double platMove = m_originTransformData["platMove"].rotationAngle + platMoveValue;
			double workRotate = m_originTransformData["workRotate"].rotationAngle + workRotateAngle;

			// 模型移动
			if (platMoveCurrent(platMove))
			{
				platformMove(platMove);
			}
			if (workRotateCurrent(workRotateAngle))
			{
				addWorkRotot(workRotateAngle, GONGJIAN);
			}
			if (setRobotTransform(currentRobot))
			{
				removeRobotResult();
				QVector<ROTATEDATA> angles;
				ROTATEDATA rotateData1;
				rotateData1.name = "Robot_1";
				rotateData1.angle = robotShapsAngle1;
				rotateData1.direction = MoveDirection::MoveDirection_ZAxis;
				angles.push_back(rotateData1);

				ROTATEDATA rotateData2;
				rotateData2.name = "Robot_2";
				rotateData2.angle = robotShapsAngle2;
				rotateData2.direction = MoveDirection::MoveDirection_ZAxis;
				angles.push_back(rotateData2);

				ROTATEDATA rotateData3;
				rotateData3.name = "Robot_3";
				rotateData3.angle = robotShapsAngle3;
				rotateData3.direction = MoveDirection::MoveDirection_ZAxis;
				angles.push_back(rotateData3);

				ROTATEDATA rotateData4;
				rotateData4.name = "Robot_4";
				rotateData4.angle = robotShapsAngle4;
				rotateData4.direction = MoveDirection::MoveDirection_ZAxis;
				angles.push_back(rotateData4);

				ROTATEDATA rotateData5;
				rotateData5.name = "Robot_5";
				rotateData5.angle = robotShapsAngle5;
				rotateData5.direction = MoveDirection::MoveDirection_YAxis;
				angles.push_back(rotateData5);

				ROTATEDATA rotateData6;
				rotateData6.name = "Robot_6";
				rotateData6.angle = robotShapsAngle6;
				rotateData6.direction = MoveDirection::MoveDirection_ZAxis;
				angles.push_back(rotateData6);

				robotRotate(angles);
			}
			//workRotate(r8, GONGJIAN, MoveDirection::MoveDirection_ZAxis);
			//m_shortMove = r7;
			//m_workRotateAngle = r8;
		}


	}

	void RobotBase::initRobotRotateZeroReStart(QString zero)
	{
		QStringList items = zero.split(',');
		if (items.size() == 8) {

			double r1 = items[0].toDouble();
			double r2 = items[1].toDouble();
			double r3 = items[2].toDouble();
			double r4 = items[3].toDouble();
			double r5 = items[4].toDouble();
			double r6 = items[5].toDouble();
			double r7 = items[6].toDouble();
			double r8 = items[7].toDouble();

			// 机器人实际位置
			QStringList keys;
			keys << items[0] << items[1] << items[2] << items[3] << items[4] << items[5];
			QString currentRobot = keys.join(",");

			// 实际变量 = 当前位置 - 零点
			double currentRobot1 = r1;// -m_originTransformData["Robot_1"].rotationActiveAngle;
			double currentRobot2 = r2;// -m_originTransformData["Robot_1"].rotationActiveAngle;
			double currentRobot3 = r3;// -m_originTransformData["Robot_1"].rotationActiveAngle;
			double currentRobot4 = r4;// -m_originTransformData["Robot_1"].rotationActiveAngle;
			double currentRobot5 = r5;// -m_originTransformData["Robot_1"].rotationActiveAngle;
			double currentRobot6 = r6;// -m_originTransformData["Robot_1"].rotationActiveAngle;

			double platMoveValue = r7;// -m_originTransformData["Robot_1"].rotationActiveAngle;
			double workRotateAngle = r8;// -m_originTransformData["Robot_1"].rotationActiveAngle;

			// 模型移动角度  = 模型零点角度 + 实际变量
			double robotShapsAngle1 = currentRobot1;
			double robotShapsAngle2 = currentRobot2;
			double robotShapsAngle3 = currentRobot3;
			double robotShapsAngle4 = currentRobot4;
			double robotShapsAngle5 = currentRobot5;
			double robotShapsAngle6 = currentRobot6;

			double platMove = platMoveValue;
			double workRotate = workRotateAngle;
			platformMove(platMove);
			addWorkRotot(workRotateAngle, GONGJIAN);
			// 模型移动
			/*if (platMoveCurrent(platMove))
			{
				platformMove(platMove);
			}
			if (workRotateCurrent(workRotateAngle))
			{
				addWorkRotot(workRotateAngle, GONGJIAN);
			}*/
			if (1)//setRobotTransform(currentRobot))
			{
				removeRobotResult();
				QVector<ROTATEDATA> angles;
				ROTATEDATA rotateData1;
				rotateData1.name = "Robot_1";
				rotateData1.angle = robotShapsAngle1;
				rotateData1.direction = MoveDirection::MoveDirection_ZAxis;
				angles.push_back(rotateData1);

				ROTATEDATA rotateData2;
				rotateData2.name = "Robot_2";
				rotateData2.angle = robotShapsAngle2;
				rotateData2.direction = MoveDirection::MoveDirection_ZAxis;
				angles.push_back(rotateData2);

				ROTATEDATA rotateData3;
				rotateData3.name = "Robot_3";
				rotateData3.angle = robotShapsAngle3;
				rotateData3.direction = MoveDirection::MoveDirection_ZAxis;
				angles.push_back(rotateData3);

				ROTATEDATA rotateData4;
				rotateData4.name = "Robot_4";
				rotateData4.angle = robotShapsAngle4;
				rotateData4.direction = MoveDirection::MoveDirection_ZAxis;
				angles.push_back(rotateData4);

				ROTATEDATA rotateData5;
				rotateData5.name = "Robot_5";
				rotateData5.angle = robotShapsAngle5;
				rotateData5.direction = MoveDirection::MoveDirection_YAxis;
				angles.push_back(rotateData5);

				ROTATEDATA rotateData6;
				rotateData6.name = "Robot_6";
				rotateData6.angle = robotShapsAngle6;
				rotateData6.direction = MoveDirection::MoveDirection_ZAxis;
				angles.push_back(rotateData6);

				robotRotate(angles);
			}
			//workRotate(r8, GONGJIAN, MoveDirection::MoveDirection_ZAxis);
			//m_shortMove = r7;
			//m_workRotateAngle = r8;
		}
	}

	void RobotBase::addWorkMove(double moveValue, QString workName, MoveDirection direction)
	{

		ADDROBOTDATA workMove = m_myRobotData.value(workName);
		gp_Pnt temp = workMove.assemblyPoint;
		switch (direction)
		{
		case MoveDirection::MoveDirection_XAxis:
		{
			temp.SetX(moveValue);

			break;
		}
		case MoveDirection::MoveDirection_YAxis:
		{
			temp.SetY(moveValue);
			break;
		}
		case MoveDirection::MoveDirection_ZAxis:
		{
			temp.SetZ(moveValue);
			break;
		}
		}
		workMove.assemblyPoint = temp;
		m_myRobotData.insert(workName, workMove);
		if (!workMove.nextShapeName.isEmpty())
		{
			addWorkMove(moveValue, workMove.nextShapeName);
		}
	}

	void RobotBase::addWorkRotot(double moveValue, QString workName, MoveDirection direction)
	{
		ADDROBOTDATA workMove = m_myRobotData.value(workName);
		switch (direction)
		{
		case MoveDirection::MoveDirection_XAxis:
		{
			workMove.angleX = moveValue;
			break;
		}
		case MoveDirection::MoveDirection_YAxis:
		{
			workMove.angleY = moveValue;
			break;
		}
		case MoveDirection::MoveDirection_ZAxis:
		{
			workMove.angleZ = moveValue;
			break;
		}
		}
		addComponentRobot(workMove);
	}

	void RobotBase::robotRotate(QVector<ROTATEDATA> data)
	{
		// 记录程序运行时间
		// 记录程序运行时间

		QVector<double> angles;
		for (auto it = data.begin(); it != data.end(); it++)
		{
			angles.push_back(it->angle);
		}
		if (0)
		{
			setRotateAngle(angles);
		}
		else
		{
			QStringList keys = {
			ROBOT_1,
			ROBOT_2,
			ROBOT_3,
			ROBOT_4,
			ROBOT_5,
			ROBOT_6
			};
			int count = 0;
			for (auto it = data.begin(); it != data.end(); it++)
			{
				TRANSFORMDATA data;
				data.name = keys[count];
				ADDROBOTDATA addRobotA = m_myRobotData.value(keys[count]);
				data.assemblyPoint = addRobotA.assemblyPoint;
				data.rotationAngle = it->angle;
				switch (it->direction)
				{
				case MoveDirection::MoveDirection_XAxis:
				{
					data.rotationAxis = m_myRobotData.value(keys[count]).xDirCurrent;
					data.angleX = it->angle;

					break;
				}
				case MoveDirection::MoveDirection_YAxis:
				{
					data.rotationAxis = m_myRobotData.value(keys[count]).yDirCurrent;
					data.angleY = it->angle;


					break;
				}
				case MoveDirection::MoveDirection_ZAxis:
				{
					data.angleZ = it->angle;


					data.rotationAxis = m_myRobotData.value(keys[count]).zDirCurrent;
					break;
				}
				default:
					break;
				}
				m_myRobotData.insert(keys[count], addRobotA);
				//m_lastTransformData.insert(keys[count], data);
				workTransform(data);
				//workTransformRecursively(keys[count], data);
				count++;
			}
		}
		//displayAllRobotShapes();
		//QVector<LoadRobotTransformResult> results = loadRobotTransformInParallel(ROBOT_1);

		initRobotRotateResult();
		//removeRobotResult();
		// 各个模型实时形状更新
		ADDROBOTDATA Robot3 = m_myRobotData.value(ROBOT_3);
		ADDROBOTDATA Robot4 = m_myRobotData.value(ROBOT_4);
		ADDROBOTDATA Robot5 = m_myRobotData.value(ROBOT_5);
		ADDROBOTDATA Robot6 = m_myRobotData.value(ROBOT_6);
		ADDROBOTDATA Work = m_myRobotData.value(GONGJIAN);
		ADDROBOTDATA DaoGui = m_myRobotData.value(DAOGUIMOVE);
		ADDROBOTDATA YuanPan = m_myRobotData.value(YUANPAN);

		//updateALlShadBox();

		// 检测与地面是否有碰撞
		setCollisionDetection(Robot3, m_gridShape);
		setCollisionDetection(Robot4, m_gridShape);
		setCollisionDetection(Robot5, m_gridShape);
		setCollisionDetection(Robot6, m_gridShape);

		// 检测是否与工件碰撞
		setCollisionDetection(Work, Robot3);
		setCollisionDetection(Work, Robot4);
		setCollisionDetection(Work, Robot5);
		setCollisionDetection(Work, Robot6);

		// 检测是否与DaoGui碰撞
		setCollisionDetection(DaoGui, Robot3);
		setCollisionDetection(DaoGui, Robot4);
		setCollisionDetection(DaoGui, Robot5);
		setCollisionDetection(DaoGui, Robot6);

		// 检测是否与YuanPan碰撞
		setCollisionDetection(YuanPan, Robot3);
		setCollisionDetection(YuanPan, Robot4);
		setCollisionDetection(YuanPan, Robot5);
		setCollisionDetection(YuanPan, Robot6);



	}

	void RobotBase::removeRobotShapes(QVector<Handle(AIS_Shape)> shapes)
	{
		foreach(Handle(AIS_Shape) shape, shapes)
		{
			myContext->Remove(shape, Standard_True);
		}
		shapes.clear();
	}

	void RobotBase::removeRobotResult()
	{

		removeRobotShapes(m_myRobotData.value(ROBOT_1).myAisShapes);
		removeRobotShapes(m_myRobotData.value(ROBOT_2).myAisShapes);
		removeRobotShapes(m_myRobotData.value(ROBOT_3).myAisShapes);
		removeRobotShapes(m_myRobotData.value(ROBOT_4).myAisShapes);
		removeRobotShapes(m_myRobotData.value(ROBOT_5).myAisShapes);
		removeRobotShapes(m_myRobotData.value(ROBOT_6).myAisShapes);

		foreach(auto result, m_Robotresults)
		{
			removeRobotShapes(result.myAisShapes);
		}

		m_myRobotData.insert(ROBOT_1, m_initMyRobotData.value(ROBOT_1));
		m_myRobotData.insert(ROBOT_2, m_initMyRobotData.value(ROBOT_2));
		m_myRobotData.insert(ROBOT_3, m_initMyRobotData.value(ROBOT_3));
		m_myRobotData.insert(ROBOT_4, m_initMyRobotData.value(ROBOT_4));
		m_myRobotData.insert(ROBOT_5, m_initMyRobotData.value(ROBOT_5));
		m_myRobotData.insert(ROBOT_6, m_initMyRobotData.value(ROBOT_6));

		m_Robotresults.clear();
	}

	void RobotBase::initRobotRotateResult()
	{
		m_Robotresults.clear();
		QVector<LoadRobotTransformResult> m_Robotresults;
		LoadRobotTransformResult r1;
		r1.robotName = ROBOT_1;
		r1.myAisShapes = m_myRobotData.value(ROBOT_1).myAisShapes;
		m_Robotresults.push_back(r1);
		LoadRobotTransformResult r2;
		r2.robotName = ROBOT_2;
		r2.myAisShapes = m_myRobotData.value(ROBOT_2).myAisShapes;
		m_Robotresults.push_back(r2);
		LoadRobotTransformResult r3;
		r3.robotName = ROBOT_3;
		r3.myAisShapes = m_myRobotData.value(ROBOT_3).myAisShapes;
		m_Robotresults.push_back(r3);
		LoadRobotTransformResult r4;
		r4.robotName = ROBOT_4;
		r4.myAisShapes = m_myRobotData.value(ROBOT_4).myAisShapes;
		m_Robotresults.push_back(r4);
		LoadRobotTransformResult r5;
		r5.robotName = ROBOT_5;
		r5.myAisShapes = m_myRobotData.value(ROBOT_5).myAisShapes;
		m_Robotresults.push_back(r5);
		LoadRobotTransformResult r6;
		r6.robotName = ROBOT_6;
		r6.myAisShapes = m_myRobotData.value(ROBOT_6).myAisShapes;
		m_Robotresults.push_back(r6);


		displayAllRobotShapes(m_Robotresults);
	}




	void RobotBase::mousePressEvent(QMouseEvent* theEvent)
	{
		if (theEvent->button() == Qt::LeftButton)
		{
			onLButtonDown((theEvent->buttons() | theEvent->modifiers()), theEvent->pos());
		}
		else if (theEvent->button() == Qt::MidButton)
		{
			onMButtonDown((theEvent->buttons() | theEvent->modifiers()), theEvent->pos());
		}
		else if (theEvent->button() == Qt::RightButton)
		{
			onRButtonDown((theEvent->buttons() | theEvent->modifiers()), theEvent->pos());
		}
	}

	void RobotBase::mouseReleaseEvent(QMouseEvent* theEvent)
	{
		if (theEvent->button() == Qt::LeftButton)
		{
			onLButtonUp(theEvent->buttons() | theEvent->modifiers(), theEvent->pos());
		}
		else if (theEvent->button() == Qt::MidButton)
		{
			onMButtonUp(theEvent->buttons() | theEvent->modifiers(), theEvent->pos());
		}
		else if (theEvent->button() == Qt::RightButton)
		{
			onRButtonUp(theEvent->buttons() | theEvent->modifiers(), theEvent->pos());
		}
	}

	void RobotBase::mouseMoveEvent(QMouseEvent* theEvent)
	{
		onMouseMove(theEvent->buttons(), theEvent->pos());
	}

	void RobotBase::wheelEvent(QWheelEvent* theEvent)
	{
		onMouseWheel(theEvent->buttons(), theEvent->delta(), theEvent->pos());
	}

	void RobotBase::onLButtonDown(const int /*theFlags*/, const QPoint thePoint)
	{
		// Save the current mouse coordinate in min.
		myXmin = thePoint.x();
		myYmin = thePoint.y();

		myXmax = thePoint.x();
		myYmax = thePoint.y();

	}

	void RobotBase::onMButtonDown(const int /*theFlags*/, const QPoint thePoint)
	{
		// Save the current mouse coordinate in min.
		myXmin = thePoint.x();
		myYmin = thePoint.y();
		myXmax = thePoint.x();
		myYmax = thePoint.y();

		if (myCurrentMode == CurAction3d_DynamicRotation)
		{
			myView->StartRotation(thePoint.x(), thePoint.y());
		}
		// 获取当前旋转的角度
		qDebug() << "Rotation angle: " << thePoint.x() << " " << thePoint.y() << endl;
	}

	void RobotBase::onRButtonDown(const int /*theFlags*/, const QPoint /*thePoint*/)
	{

	}

	void RobotBase::onMouseWheel(const int /*theFlags*/, const int theDelta, const QPoint thePoint)
	{
		Standard_Integer aFactor = 16;

		Standard_Integer aX = thePoint.x();
		Standard_Integer aY = thePoint.y();

		if (theDelta > 0)
		{
			aX += aFactor;
			aY += aFactor;
		}
		else
		{
			aX -= aFactor;
			aY -= aFactor;
		}

		myView->Zoom(thePoint.x(), thePoint.y(), aX, aY);
	}

	void RobotBase::addItemInPopup(QMenu* /*theMenu*/)
	{
	}

	void RobotBase::popup(const int /*x*/, const int /*y*/)
	{
	}

	void RobotBase::onLButtonUp(const int theFlags, const QPoint thePoint)
	{
		// Hide the QRubberBand
		if (myRectBand)
		{
			myRectBand->hide();
		}

		// Ctrl for multi selection.
		if (thePoint.x() == myXmin && thePoint.y() == myYmin)
		{
			if (theFlags & Qt::ControlModifier)
			{
				multiInputEvent(thePoint.x(), thePoint.y());
			}
			else
			{
				inputEvent(thePoint.x(), thePoint.y());
			}
		}

	}

	void RobotBase::onMButtonUp(const int /*theFlags*/, const QPoint thePoint)
	{
		if (thePoint.x() == myXmin && thePoint.y() == myYmin)
		{
			panByMiddleButton(thePoint);
		}
	}

	void RobotBase::onRButtonUp(const int /*theFlags*/, const QPoint thePoint)
	{
		popup(thePoint.x(), thePoint.y());
	}

	void RobotBase::onMouseMove(const int theFlags, const QPoint thePoint)
	{
		// 实现 旋转功能

		// Draw the rubber band.
		if (theFlags & Qt::LeftButton)
		{
			drawRubberBand(myXmin, myYmin, thePoint.x(), thePoint.y());

			dragEvent(thePoint.x(), thePoint.y());
		}

		// Ctrl for multi selection.
		if (theFlags & Qt::ControlModifier)
		{
			multiMoveEvent(thePoint.x(), thePoint.y());
		}
		else
		{
			moveEvent(thePoint.x(), thePoint.y());
		}

		// Middle button.
		if (theFlags & Qt::MidButton)
		{
			switch (myCurrentMode)
			{
			case CurAction3d_DynamicRotation:
				myView->Rotation(thePoint.x(), thePoint.y());
				break;

			case CurAction3d_DynamicZooming:
				myView->Zoom(myXmin, myYmin, thePoint.x(), thePoint.y());
				break;

			case CurAction3d_DynamicPanning:
				myView->Pan(thePoint.x() - myXmax, myYmax - thePoint.y());
				myXmax = thePoint.x();
				myYmax = thePoint.y();
				break;

			default:
				break;
			}
		}

	}

	void RobotBase::dragEvent(const int x, const int y)
	{
		myContext->Select(myXmin, myYmin, x, y, myView, Standard_True);

		emit selectionChanged();
	}

	void RobotBase::multiDragEvent(const int x, const int y)
	{
		myContext->ShiftSelect(myXmin, myYmin, x, y, myView, Standard_True);

		emit selectionChanged();

	}

	void RobotBase::inputEvent(const int x, const int y)
	{
		Q_UNUSED(x);
		Q_UNUSED(y);

		myContext->Select(Standard_True);

		emit selectionChanged();
	}

	void RobotBase::multiInputEvent(const int x, const int y)
	{
		Q_UNUSED(x);
		Q_UNUSED(y);

		myContext->ShiftSelect(Standard_True);

		emit selectionChanged();
	}

	void RobotBase::moveEvent(const int x, const int y)
	{
		myContext->MoveTo(x, y, myView, Standard_True);
	}

	void RobotBase::multiMoveEvent(const int x, const int y)
	{
		myContext->MoveTo(x, y, myView, Standard_True);
	}

	void RobotBase::drawRubberBand(const int minX, const int minY, const int maxX, const int maxY)
	{
		QRect aRect;

		// Set the rectangle correctly.
		(minX < maxX) ? (aRect.setX(minX)) : (aRect.setX(maxX));
		(minY < maxY) ? (aRect.setY(minY)) : (aRect.setY(maxY));

		aRect.setWidth(abs(maxX - minX));
		aRect.setHeight(abs(maxY - minY));

		if (!myRectBand)
		{
			myRectBand = new QRubberBand(QRubberBand::Rectangle, this);

			// setStyle is important, set to windows style will just draw
			// rectangle frame, otherwise will draw a solid rectangle.
			myRectBand->setStyle(QStyleFactory::create("windows"));
		}

		myRectBand->setGeometry(aRect);
		myRectBand->show();
	}

	void RobotBase::panByMiddleButton(const QPoint& thePoint)
	{
		Standard_Integer aCenterX = 0;
		Standard_Integer aCenterY = 0;

		QSize aSize = size();

		aCenterX = aSize.width() / 2;
		aCenterY = aSize.height() / 2;

		myView->Pan(aCenterX - thePoint.x(), thePoint.y() - aCenterY);
	}
	void RobotBase::showRobotParameterSet()
	{

		//asdl->exec();
	}
}

