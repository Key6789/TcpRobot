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
		LOG_FUNCTION_LINE_MESSAGE;
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

	/**************************************** 界面初始化 **********/
	void RobotBase::initRobotView()
	{
		LOG_FUNCTION_LINE_MESSAGE;
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
	
	const Handle(AIS_InteractiveContext)& RobotBase::getContext() const
	{

		return myContext;
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
	
	void RobotBase::setCollisionDetection(Handle(AIS_Shape) shape1, Handle(AIS_Shape) shape2)
	{
		LOG_FUNCTION_LINE_MESSAGE;
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
			LOG_FUNCTION_LINE_INFO("No collision detected between shapes: ");
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
			LOG_FUNCTION_LINE_INFO("No collision detected between shapes: ");
		}
	}

	void RobotBase::setCollisionDetection(ADDROBOTDATA data, ADDROBOTDATA data2)
	{
		LOG_FUNCTION_LINE_MESSAGE;
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
					LOG_FUNCTION_LINE_INFO("No collision detected between shapes: ");
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
					LOG_FUNCTION_LINE_INFO("No collision detected between shapes: ");
				}
			}
		}
	}

	void RobotBase::setCollisionDetection(ADDROBOTDATA data, Handle(AIS_Shape) shape2)
	{
		LOG_FUNCTION_LINE_MESSAGE;
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
				LOG_FUNCTION_LINE_INFO("No collision detected between shapes: ");
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
				LOG_FUNCTION_LINE_INFO("No collision detected between shapes: ");

			}
		}
	}

	
	QVector<TopoDS_Shape> RobotBase::scaleShapes(QVector<TopoDS_Shape> shapes, double scale)
	{
		LOG_FUNCTION_LINE_MESSAGE;
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

	QVector<TopoDS_Shape> RobotBase::loadWorkSTEPModel(QString filePath)
	{
		QVector<TopoDS_Shape> vector;
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

	QVector<Handle(AIS_Shape)> RobotBase::RobotTransformParallelPreview(ADDROBOTDATA data)
	{
		//data.initShapeAx3();
		QVector<TopoDS_Shape> vector = data.shapes;
		QVector<Handle(AIS_Shape)> vectortemp;

		gp_Trsf trsf;
		trsf.SetTransformation(data.ShapeAxl3, gp_Ax3());
		// 去除旧有的显示
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
			
			TopoDS_Shape transformedShape = BRepBuilderAPI_Transform(vector[i], trsf).Shape();
			Handle(AIS_Shape) aisShapeTemp = new AIS_Shape(transformedShape);
			aisShapeTemp->SetColor(data.color);
			vectortemp.push_back(aisShapeTemp);
		}
		return vectortemp;
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
		LOG_FUNCTION_LINE_MESSAGE;
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
		QMenu* aMenu = new QMenu(this);

		QAction* aAction = new QAction(__StandQString("窗口适应"), aMenu);
		aAction->setShortcut(Qt::CTRL + Qt::Key_F);
		aAction->setStatusTip(__StandQString("窗口适应"));
		connect(aAction, SIGNAL(triggered()), this, SLOT(fitAll()));
		aMenu->addAction(aAction);

		// 窗口重置
		aAction = new QAction(__StandQString("窗口重置"), aMenu);
		aAction->setShortcut(Qt::CTRL + Qt::Key_R);
		aAction->setStatusTip(__StandQString("窗口重置"));
		connect(aAction, SIGNAL(triggered()), this, SLOT(fitAll()));
		aMenu->addAction(aAction);

		// 正视图
		aAction = new QAction(__StandQString("正视图"), aMenu);
		aAction->setShortcut(Qt::CTRL + Qt::Key_1);
		aAction->setStatusTip(__StandQString("正视图"));
		connect(aAction, SIGNAL(triggered()), this, SLOT(reset()));
		aMenu->addAction(aAction);



		aMenu->popup(QCursor::pos());
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
	


	
	ADDROBOTDATA RobotBase::createRobotData(SHAPESTRUCT shapeStruct)
	{
		LOG_FUNCTION_LINE_MESSAGE;
		ADDROBOTDATA robot;
		robot.name = shapeStruct.ShapeName;

		robot.shapeType = shapeStruct.shapeType;
		// 机器人坐标
		robot.x = shapeStruct.ShapePositionX.toDouble();
		robot.y = shapeStruct.ShapePositionY.toDouble();
		robot.z = shapeStruct.ShapePositionZ.toDouble();

		// 机器人姿态
		robot.angleX = shapeStruct.ShapeAngleX.toDouble();
		robot.angleY = shapeStruct.ShapeAngleY.toDouble();
		robot.angleZ = shapeStruct.ShapeAngleZ.toDouble();

		// 机器人 路径
		robot.path = shapeStruct.ShapePath;

		// 下一级形状名称
		robot.nextShapeNames = shapeStruct.nextShapeNames;

		// 机器人连杆
		/*robot.a = shapeStruct.aDistance;
		robot.alpha = shapeStruct.alpha;
		robot.theta = shapeStruct.theta;
		robot.d = shapeStruct.dDistance;*/
		robot.ShapeLinkIndex = shapeStruct.ShapeLinkIndex;
		// 机器人尺寸
		robot.sacle = shapeStruct.ShapeScale.toDouble();

		// 机器人下一个形状名称
		robot.nextShapeName = shapeStruct.nextShapeName;

		// 机器人装配点
		robot.assemblyPoint = gp_Pnt(shapeStruct.ShapePositionX.toDouble(), shapeStruct.ShapePositionY.toDouble(), shapeStruct.ShapePositionZ.toDouble());

		// 机器人颜色
		QString colorStr = shapeStruct.ShapeColor;
		// 转换颜色格式
		QColor color(colorStr);
		// 转换为Quantity_Color
		robot.color = Quantity_Color(color.redF(), color.greenF(), color.blueF(), Quantity_TOC_sRGB);

		// 机器人坐标系

		// 获取模型  修改为相对路径
		QString robotPath = QCoreApplication::applicationDirPath() + shapeStruct.ShapePath;
		robot.shapes = getShapesFromResult(robotPath, loadFilesInParallel({ robotPath }));
		robot.shapes = scaleShapes(robot.shapes, robot.sacle);
		robot.initShapeAx3();
		robot.myAisShapes = RobotTransformParallelPreview(robot);
		
		//robot.initShapeAx3();
		// 加入数据
		return robot;
	}
	void RobotBase::displaySingalAddRobot(ADDROBOTDATA addRobotData)
	{
		LOG_FUNCTION_LINE_MESSAGE;
		// 显示机器人
		for (auto shape : addRobotData.myAisShapes)
		{
			getContext()->Display(shape, Standard_True);
		}

	}
	

}

