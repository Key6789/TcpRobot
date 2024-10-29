#pragma once

#ifndef ROSROBOTBASE_H
#define ROSROBOTBASE_H
#include "TcpRobotManger_global.h"
#include "SDHRobot.h"


class QMenu;
class QRubberBand;
namespace TCP_ROBOT
{
	class TCPROBOT_EXPORT RobotBase : public QWidget
	{
		Q_OBJECT
	public:
		// 构造
		RobotBase(QWidget* parent = nullptr);
		~RobotBase();


	public:
		//  数据获取
		// 获取当前的交互上下文
		const Handle(AIS_InteractiveContext)& getContext() const;

		// 获取形状
		TopoDS_Shape getShape();
	signals:

		// 选择改变时调用的函数
		void selectionChanged(void);

		// 处理信号碰撞的函数
		void singalCollision(bool isCollision);


	protected:
		// 界面初始化 
		virtual void initRobotView(void);

	public slots:
		// 模型更新
		virtual void slotUpdataRobotShaps(void);

	public:

		// 显示坐标轴
		void displayCoordinateAxes();
		// 显示添加机器人信号
		void displaySingalAddRobot(ADDROBOTDATA addRobotData);
		// 创建机器人数据
		ADDROBOTDATA createRobotData(SHAPESTRUCT shapeStruct);
		// 添加箭头到指定坐标轴
		void addArrow(Standard_Real axisLength, gp_Pnt& origin, const gp_Dir& direction, Handle(AIS_InteractiveContext)& context);
		// 缩放形状
		QVector<TopoDS_Shape> scaleShapes(QVector<TopoDS_Shape> shapes, double scale = 1.0);

		// 设置碰撞检测
		void setCollisionDetection(Handle(AIS_Shape) shape1, Handle(AIS_Shape) shape2);
		// 设置碰撞检测（重载方法）
		void setCollisionDetection(ADDROBOTDATA data, ADDROBOTDATA data2);
		// 设置碰撞检测（重载方法）
		void setCollisionDetection(ADDROBOTDATA data, Handle(AIS_Shape) shape2);

		// 加载工作STEP模型
		QVector<TopoDS_Shape> loadWorkSTEPModel(QString filePath);
		// 从结果中获取形状
		QVector<TopoDS_Shape> getShapesFromResult(const QString& name, const QVector<LoadResult>& shapes);
		// 并行加载文件
		QVector<LoadResult> loadFilesInParallel(const QStringList& filePaths);
		// 并行预览机器人变换
		QVector<Handle(AIS_Shape)> RobotTransformParallelPreview(ADDROBOTDATA data);

		// 互斥量指针
		QMutex* m_mutex = nullptr;
		// 互斥量锁定器指针
		QMutexLocker* m_locker = nullptr;
		// 存储坐标轴的变换结果
		QVector< LoadRobotTransformResult> m_CoordinateAxes;
		// 地面形状
		Handle(AIS_Shape) m_gridShape;


		// 显示功能区
	public slots:
		// 平移操作
		void pan(void);
		// 适合所有内容
		void fitAll(void);
		// 重置视图
		void reset(void);
		// 放大缩小操作
		void zoom(void);
		// 旋转操作
		void rotate(void);
	protected:
		// 返回画图引擎
		virtual QPaintEngine* paintEngine() const;

		// 绘制事件处理
		virtual void paintEvent(QPaintEvent* theEvent);
		// 窗口调整事件处理
		virtual void resizeEvent(QResizeEvent* theEvent);

		// 鼠标事件处理
		virtual void mousePressEvent(QMouseEvent* theEvent);
		virtual void mouseReleaseEvent(QMouseEvent* theEvent);
		virtual void mouseMoveEvent(QMouseEvent* theEvent);
		virtual void wheelEvent(QWheelEvent* theEvent);

		// 按钮事件处理
		virtual void onLButtonDown(const int theFlags, const QPoint thePoint);
		virtual void onMButtonDown(const int theFlags, const QPoint thePoint);
		virtual void onRButtonDown(const int theFlags, const QPoint thePoint);
		virtual void onMouseWheel(const int theFlags, const int theDelta, const QPoint thePoint);
		virtual void onLButtonUp(const int theFlags, const QPoint thePoint);
		virtual void onMButtonUp(const int theFlags, const QPoint thePoint);
		virtual void onRButtonUp(const int theFlags, const QPoint thePoint);
		virtual void onMouseMove(const int theFlags, const QPoint thePoint);

		// 弹出菜单处理
		virtual void addItemInPopup(QMenu* theMenu);

	protected:

		// 显示弹出菜单
		void popup(const int x, const int y);
		// 拖动事件处理
		void dragEvent(const int x, const int y);
		// 输入事件处理
		void inputEvent(const int x, const int y);
		// 移动事件处理
		void moveEvent(const int x, const int y);
		// 多点移动事件处理
		void multiMoveEvent(const int x, const int y);
		// 多点拖动事件处理
		void multiDragEvent(const int x, const int y);
		// 多点输入事件处理
		void multiInputEvent(const int x, const int y);
		// 绘制虚线框
		void drawRubberBand(const int minX, const int minY, const int maxX, const int maxY);
		// 通过中键平移
		void panByMiddleButton(const QPoint& thePoint);


	private:
		//! save the mouse position.
		Standard_Integer myXmin;
		Standard_Integer myYmin;
		Standard_Integer myZmin;
		Standard_Integer myXmax;
		Standard_Integer myYmax;
		Standard_Integer myZmax;

		//! the mouse current mode.
		CurrentAction3d myCurrentMode;

		//! save the degenerate mode state.
		Standard_Boolean myDegenerateModeIsOn;

		//! rubber rectangle for the mouse selection.
		QRubberBand* myRectBand;
		TopoDS_Shape shape;

	public:
		//! the occ viewer.
		Handle(V3d_Viewer) myViewer;

		Handle(AIS_Shape) aisShape;
		//! the occ view.
		Handle(V3d_View) myView;
		Handle(V3d_View) myView2;

		//! the occ context.
		Handle(AIS_InteractiveContext) myContext;
	};
}

#endif // !
