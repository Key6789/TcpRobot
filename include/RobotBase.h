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
		// 机器人界面显示
		// 创建工艺界面
		virtual QWidget* createWorkView(QWidget* parent = nullptr);
		virtual QWidget* createTopShapeView(QWidget* parent = nullptr);
		// 示教窗口
		virtual QWidget* createTeachView(QWidget* parent = nullptr);

		virtual QWidget* showShapeView(QWidget* parent = nullptr);

		// 机器人 模型设置
		virtual QWidget* createRobotSettingView(QWidget* parent = nullptr);
		virtual QWidget* createWorkSettingView(QWidget* parent = nullptr);
	protected:
		// 界面初始化 
		virtual void initRobotView(void);

		

	public:
		//  数据获取
		// 获取当前的交互上下文
		const Handle(AIS_InteractiveContext)& getContext() const;

		// 获取形状
		TopoDS_Shape getShape();

		void loadModel(const QString& filePath);

	signals:
		// 选择改变时调用的函数
		void selectionChanged(void);

		// 处理信号碰撞的函数
		void singalCollision(bool isCollision);


	public:
		// 工具函数
		// 判断浮点数是否相等
		bool isEqualDouble(double a, double b);

		// 设置比较的值
		void setEqualCompareValue(double value);

		// 移动当前平台，接受一个移动值作为输入
		bool platMoveCurrent(double moveValue);

		// 旋转当前工作状态，接受一个角度作为输入
		bool workRotateCurrent(double angle);

		// 设置机器人的变换数据，接受一个字符串作为输入
		bool setRobotTransform(QString data);

	public slots:
		// 接口封装
		// 参数接入
		// 平台平移
		void platformMove(double moveValue, MoveDirection direction = MoveDirection_XAxis);

		// 工件及底盘旋转
		void workRotate(double angle, QString workName = QString(), MoveDirection direction = MoveDirection_ZAxis);



		void initRobotRotateZero(QString zero);
		void initRobotRotateZeroReStart(QString zero);
		void addWorkMove(double moveValue, QString workName = QString(), MoveDirection direction = MoveDirection_XAxis);
		void addWorkRotot(double moveValue, QString workName = QString(), MoveDirection direction = MoveDirection_ZAxis);
		// 机器人旋转
		void robotRotate(QVector<ROTATEDATA> data);
		void removeRobotShapes(QVector<Handle(AIS_Shape)> shapes);
		void removeRobotResult();
		void initRobotRotateResult();




		virtual void slotUpdataRobotShaps(void);
		// 设置旋转角度
		void setRotateAngle(QVector<double> rotateAngle);
		void setRotateAngle(QString name, int size);

		void setRobotIsActive(bool isActive) { m_isRobotActive = isActive; };
		bool getRobotIsActive() { return m_isRobotActive; };
	private:

		bool m_isRobotActive = true;
		// 读取DH参数
		void readRobotDhParameter(QVariantMap data);
		SDHRobot* m_DHRobot = nullptr;
		void initZeroParameter();
		void initZeroPosition();
		void initZeroOrientation();
		QMap<QString, ADDROBOTDATA> m_myRobotDHData;

		gp_Pnt getCurrentAssemblyPoint(int size);
		gp_Pnt getCurrentRotationAngal(int size);


		void showSetParameter();
		QDialog* m_diaog = nullptr;
		QMessageBox* m_messageBox = nullptr;


	private:
		// DH 参数兼容
		void showRobotParameterSet();

	public:
		bool loadSTEPModel(const QString& filePath, TopoDS_Shape& shape);
		QVector<TopoDS_Shape> loadWorkSTEPModel(const QString& filePath);
		void displayCoordinateAxes();
		QVector< LoadRobotTransformResult> m_CoordinateAxes;
		void addArrow(Standard_Real axisLength, gp_Pnt& origin, const gp_Dir& direction, Handle(AIS_InteractiveContext)& context);
		QMap<QString, QVector<TopoDS_Shape>> m_myShapes;

		// 组装机器人组件
		void addComponentRobotALL();
		void workTransform(TRANSFORMDATA data);
		void workTransformRecursively(QString name, const TRANSFORMDATA& data);
		// 向量计算，根据旋转三个旋转角度算向量
		gp_Vec getVectorByAngle(double angleX, double angleY, double angleZ);

		// 自身的平移旋转
		void applyTranslation(ADDROBOTDATA data);
		void applyRotation(gp_Trsf& trsf, const gp_Ax1& axis, double angle);
		gp_Dir getTransformationMatrix(const ADDROBOTDATA& robot, gp_Dir axis);

		void showAllShadBox();
		void hideAllShadBox();
		void updateALlShadBox();
		void showOnceShadBox(ADDROBOTDATA data);
		void hideOnceShadBox(ADDROBOTDATA data);
		void updateOnceShadBox(ADDROBOTDATA data);
		// 新方案 求各组件的装配点坐标变换，及坐标轴的角度变化
		gp_Pnt getNewAssemblyPoint(const gp_Pnt& assemblyPoint, const TRANSFORMDATA& data);


		//根据向量计算旋转角度
		//void getEulerAngles(const gp_Vec& targetX, const gp_Vec& targetY, const gp_Vec& targetZ, double& angleX, double& angleY, double& angleZ);
		// 设置模型颜色
		void setMyShapColor();
		QVector<TopoDS_Shape> getShapesFromResult(const QString& name, const QVector<LoadResult>& shapes);
		QVector<TopoDS_Shape> findAndScaleShapes(const QString& name, double scale = 1.0);
		QVector<TopoDS_Shape> scaleShapes(QVector<TopoDS_Shape> shapes, double scale = 1.0);
		// 英文与中文转换，防止乱码
		TCollection_ExtendedString displayText(const char* theText);
		// 添加标题文本
		void addShapeTitleText(const char* theText, gp_Pnt& point, Handle(AIS_Shape) aisShape);



		// 设置碰撞检测
		void setCollisionDetection();
		void setCollisionDetection(Handle(AIS_Shape) shape1, Handle(AIS_Shape) shape2);
		void setCollisionDetection(ADDROBOTDATA data, ADDROBOTDATA data2);
		void setCollisionDetection(ADDROBOTDATA data, Handle(AIS_Shape) shape2);

		void addComponentRobot(ADDROBOTDATA data);

		// 地面形状
		Handle(AIS_Shape) m_gridShape;


		//! the occ viewer.
		Handle(V3d_Viewer) myViewer;

		Handle(AIS_Shape) aisShape;
		//! the occ view.
		Handle(V3d_View) myView;
		Handle(V3d_View) myView2;

		//! the occ context.
		Handle(AIS_InteractiveContext) myContext;

		QVector<LoadResult> loadFilesInParallel(const QStringList& filePaths);
		QVector <LoadRobotTransformResult> loadRobotTransformInParallel();
		QVector <LoadRobotTransformResult> loadRobotTransformInParallel(QString currentName);
		void displayRobotShapes(LoadRobotTransformResult results);
		void displayRobotShapes(QVector <AIS_Shape> results);
		void displayAllRobotShapes(QVector <LoadRobotTransformResult> results);
		void displayAllRobotShapes();
		QVector<Handle(AIS_Shape)> RobotTransformParallel(ADDROBOTDATA data);
		QVector<Handle(AIS_Shape)> RobotTransformParallelPreview(ADDROBOTDATA data);

		QStringList getCurrentAndNextName(QString name);
		

		QMap<QString, ADDROBOTDATA> m_myRobotData;
		QMap<QString, ADDROBOTDATA> m_initMyRobotData;

		QMap<QString, TRANSFORMDATA> m_lastTransformData;
		QMap<QString, TRANSFORMDATA> m_originTransformData;
		int m_shortMove = 0;
		int m_lastMove = 0;
		int m_shortActiveMove = 0;
		int m_workRotateAngle = 0;
		int m_lastWorkRotateAngle = 0;
		int m_workRotateActiveAngle = 0;

		QString m_lastRobotRotateAngle = QString();

		QString m_roboteLast = QString();


		bool m_initRobot = false;

		QMutex* m_mutex = nullptr;
		QMutexLocker* m_locker = nullptr;
		QVector<LoadRobotTransformResult> m_Robotresults;
		QTimer m_updataTimer;

		//  比较值
		double m_equalCompareValue = 0.5;

		// 显示功能区
	public slots:
		void pan(void);
		void fitAll(void);
		void reset(void);
		void zoom(void);
		void rotate(void);
	protected:
		virtual QPaintEngine* paintEngine() const;

		// Paint events.
		virtual void paintEvent(QPaintEvent* theEvent);
		virtual void resizeEvent(QResizeEvent* theEvent);

		// Mouse events.
		virtual void mousePressEvent(QMouseEvent* theEvent);
		virtual void mouseReleaseEvent(QMouseEvent* theEvent);
		virtual void mouseMoveEvent(QMouseEvent* theEvent);
		virtual void wheelEvent(QWheelEvent* theEvent);

		// Button events.
		virtual void onLButtonDown(const int theFlags, const QPoint thePoint);
		virtual void onMButtonDown(const int theFlags, const QPoint thePoint);
		virtual void onRButtonDown(const int theFlags, const QPoint thePoint);
		virtual void onMouseWheel(const int theFlags, const int theDelta, const QPoint thePoint);
		virtual void onLButtonUp(const int theFlags, const QPoint thePoint);
		virtual void onMButtonUp(const int theFlags, const QPoint thePoint);
		virtual void onRButtonUp(const int theFlags, const QPoint thePoint);
		virtual void onMouseMove(const int theFlags, const QPoint thePoint);

		// Popup menu.
		virtual void addItemInPopup(QMenu* theMenu);

	protected:

		void popup(const int x, const int y);
		void dragEvent(const int x, const int y);
		void inputEvent(const int x, const int y);
		void moveEvent(const int x, const int y);
		void multiMoveEvent(const int x, const int y);
		void multiDragEvent(const int x, const int y);
		void multiInputEvent(const int x, const int y);
		void drawRubberBand(const int minX, const int minY, const int maxX, const int maxY);
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
	};
}

#endif // !
