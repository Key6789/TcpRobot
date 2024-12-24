#pragma once
#ifndef TCPROBOTMANAGER_GLOBAL_H
#define TCPROBOTMANAGER_GLOBAL_H
#include <QTimer>
#include <Eigen/Dense>
#include <QtCore/qglobal.h>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QAbstractItemView>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QColorDialog>
#include <QInputDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QPushButton>

#include <QObject>
#include <QDockWidget>
#include <QWidget>
#include <QMap>
#include <QTreeWidget>
#include <QTabWidget>
#include <QTreeWidgetItem>
#include <QDebug>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QFileDialog>
#include <QDialogButtonBox>

#include <Standard.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <IFSelect_ReturnStatus.hxx>
#include <TopoDS_Shape.hxx>
#include <Quantity_Color.hxx>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QDebug>
#include <QtConcurrent/qtconcurrentrun.h>
#include <gp_EulerSequence.hxx>
#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <gp_Pln.hxx>
#include <QTimer>
#include <gp_Lin2d.hxx>

#include <QTableWidget>

#include <Geom_ConicalSurface.hxx>
#include <Geom_ToroidalSurface.hxx>
#include <Geom_CylindricalSurface.hxx>

#include <GCE2d_MakeSegment.hxx>
#include <cmath> // for sin, cos
#include <TopoDS.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TColgp_Array1OfPnt2d.hxx>

#include <BRepLib.hxx>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>

#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>

#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepFilletAPI_MakeChamfer.hxx>

#include <BRepOffsetAPI_MakePipe.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>

#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>

#include <Quantity_NameOfColor.hxx>
#include <qmenu.h>
#include <qtoolbar.h>
#include <qmainwindow.h>
#include <qopengl.h>
#include <qopenglwidget.h>
#include <qpoint.h>
#include <qsize.h>
#include <qrect.h>
#include <qwidget.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qtoolbar.h>
#include <qstatusbar.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qradiobutton.h>
#include <qspinbox.h>
#include <qslider.h>
#include <qprogressbar.h>
#include <qcolordialog.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qfontdialog.h>
#include <qinputdialog.h>
#include <qevent.h>

#include <QtWidgets/QMainWindow>
#include <Standard_Handle.hxx>
#include <V3d_Viewer.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <WNT_Window.hxx>
#include <V3d_View.hxx>
#include <AIS_InteractiveContext.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <TopoDs_Shape.hxx>
#include <AIS_Shape.hxx>
#include <qboxlayout.h>
#include <qtoolbar.h>

#include <QWidget>
#include <TopoDS.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Ax1.hxx>
#include <gp_Trsf.hxx>
#include <gp_Mat.hxx>
#include <gp_Vec.hxx>
#include <vector>
#include <cmath>
#include <QVector>
#include <QDebug>
#include <QMessageBox>
#include <Qt3DCore/QEntity>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QCameraLens>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QOrbitCameraController>
#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QAttribute>

#include <Qt3DCore/QEntity>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QCameraLens>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QOrbitCameraController>
#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QAttribute>

#include <TopoDS_Face.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>

#include <BRepMesh_IncrementalMesh.hxx>
#include <Quantity_Color.hxx>
#include <gp_Quaternion.hxx>
#include <gp_Pln.hxx>
#include <gp.hxx>
#include <TopoDS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepMesh_IncrementalMesh.hxx>

#include <TopoDS_Shape.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <TopExp_Explorer.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <QMessageBox>

#include <gp_Trsf.hxx>

#include <TopExp_Explorer.hxx>
#include <BRepTools_WireExplorer.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRepAdaptor_Surface.hxx>

#include <STEPCAFControl_Reader.hxx>
#include <BRep_Builder.hxx>
#include <TopoDS_Compound.hxx>
#include <TDocStd_Document.hxx>
#include <STEPCAFControl_Writer.hxx>
#include <STEPCAFControl_DataMapOfLabelShape.hxx>
#include <gp_Ax1.hxx>
#include <gp_Pnt.hxx>
#include <gp_Trsf.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <AIS.hxx>
#include <AIS_Line.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <Prs3d_Drawer.hxx>
#include <V3d_View.hxx>
#include <AIS.hxx>
#include <AIS_Shape.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <Prs3d_Drawer.hxx>
#include <Prs3d_LineAspect.hxx>
#include <V3d_View.hxx>
#include <QThreadPool>
#include <QFuture>
#include <QFutureWatcher>
#include <QVector>
#include <QStringList>
#include <QMenu>
#include <QMouseEvent>
#include <QRubberBand>
#include <QStyleFactory>
#include <locale>
#include <codecvt>
#include <V3d_View.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx> // 用于创建实体
#include <BRepAlgoAPI_Common.hxx>
#include <QMessageBox>
#include <AIS_Shape.hxx>
#include <AIS_InteractiveContext.hxx>
#include <BRepCheck_Analyzer.hxx>
#include <Standard_Boolean.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <V3d_View.hxx>
#include <Prs3d_Drawer.hxx>
#include <gp_Ax3.hxx>
#include <BRepExtrema_DistShapeShape.hxx>
#include <Bnd_Box.hxx>
#include <Aspect_Handle.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <Quantity_Color.hxx>
#include <QElapsedTimer>
#include <AIS_TextLabel.hxx>
#include <gp_Pnt.hxx>
#include <TCollection_ExtendedString.hxx>
#include <TopoDS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx> // 用于创建实体
#include <BRepAlgoAPI_Common.hxx>
#include <QMessageBox>
#include <AIS_Shape.hxx>
#include <AIS_InteractiveContext.hxx>
#include <BRepCheck_Analyzer.hxx>
#include <Standard_Boolean.hxx>
#include <Standard_CString.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <TopoDS_Shape.hxx>
#include <TopExp.hxx>
#include <BRepCheck_Analyzer.hxx>
#include <TopTools_HSequenceOfShape.hxx>

#include <TopoDS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx> // 用于创建实体
#include <BRepAlgoAPI_Common.hxx>
#include <QMessageBox>
#include <AIS_Shape.hxx>
#include <AIS_InteractiveContext.hxx>
#include <BRepCheck_Analyzer.hxx>
#include <Standard_Boolean.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include <QElapsedTimer>
#include <QWidget>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QFileDialog>
#include <QColorDialog>
#include <QMessageBox>
#include "TcpXViewBase.h"
#include <QFormLayout>

#include "QCusWidgetLib.h"
#include "LoggerInterface.h"

using namespace QCUSWIDGETLIB;

#define   WORKPATH	QApplication::applicationDirPath().append("/RobotData/WorkPath")
#define   ROBOTPATH	QApplication::applicationDirPath().append("/RobotData/RobotPath")
#define   OTHERPATH	QApplication::applicationDirPath().append("/RobotData/OtherPath")
#define   TEACHPATH	QApplication::applicationDirPath().append("/RobotData/TeachPath")



#define OCCQT occQt

// 导轨
#define  DAOGUIMOVE         "LongGuide"
#define  YUANPAN            "RotatingTable"
#define  SHORTGUIDE         "ShortGuide"
#define  GONGJIAN           "Work"
#define  ROBOT_1            "Robot_1"
#define  ROBOT_2            "Robot_2"
#define  ROBOT_3            "Robot_3"
#define  ROBOT_4            "Robot_4"
#define  ROBOT_5            "Robot_5"
#define  ROBOT_6            "Robot_6"
#define  ROBOT_7            "Robot_7"
#define  ROBOT_8            "Robot_8"
#define  ROBOT_9            "Robot_9"

#define  BOXDISTANCE		100

#endif // TCPROBOTMANAGER_GLOBAL_H


# if (defined(BUILD_EXE) && BUILD_EXE == 1) // 不修改
#   define TCPROBOT_EXPORT  // 根据自己的需求设置
#  else
#if defined(TCPROBOT_LIBRARY)  // 根据自己的需求设置
#  define TCPROBOT_EXPORT Q_DECL_EXPORT  // 根据自己的需求设置
#else
# define TCPROBOT_EXPORT Q_DECL_IMPORT  // 根据自己的需求设置
#endif  
#  endif

#define ROBOTUNABLEVALUE -1


//! mouse actions.
enum CurrentAction3d
{
	CurAction3d_Nothing,
	CurAction3d_DynamicZooming,
	CurAction3d_WindowZooming,
	CurAction3d_DynamicPanning,
	CurAction3d_GlobalPanning,
	CurAction3d_DynamicRotation
};

enum MoveDirection
{
	MoveDirection_XAxis,
	MoveDirection_YAxis,
	MoveDirection_ZAxis
};
enum ShapeType
{
	ShapeType_None = 0,
	ShapeType_Work,
	ShapeType_Hole,
	ShapeType_Robot,
	ShapeType_Link,
	ShapeType_LongGuide,
	ShapeType_RotatingTable,
	ShapeType_ShortGuide,
	ShapeType_Table,
	ShapeType_Robot_1,
	ShapeType_Robot_2,
	ShapeType_Robot_3,
	ShapeType_Robot_4,
	ShapeType_Robot_5,
	ShapeType_Robot_6,
	ShapeType_Robot_7,
	ShapeType_Robot_8,
	ShapeType_Robot_9,
	ShapeType_Robot_10,
	ShapeType_Robot_11,
	ShapeType_Robot_12,
	ShapeType_Robot_13,
	ShapeType_Clone_1,
	ShapeType_Clone_2,
	ShapeType_Clone_3,
	ShapeType_Clone_4,
	ShapeType_Clone_5,
	ShapeType_Clone_6,
	ShapeType_Clone_7,
	ShapeType_Clone_8,
	ShapeType_Clone_9,
	ShapeType_Clone_10,
	ShapeType_Clone_11,
	ShapeType_Clone_12,
};

// 获取图形驱动程序的静态实例
static Handle(Graphic3d_GraphicDriver)& GetGraphicDriver()
{
	static Handle(Graphic3d_GraphicDriver) aGraphicDriver;
	return aGraphicDriver;
}

// 定义一个结构体来存储加载结果
struct LoadResult
{
	QString filePath;
	QVector<TopoDS_Shape> shapes;
};

// 载入机器人的变换结果结构体
struct LoadRobotTransformResult
{
	QString robotName; // 机器人名称
	QVector<Handle(AIS_Shape)> myAisShapes; // 与机器人相关的AIS形状集合
};

// 定义关节数据结构
struct Joint {
	gp_Trsf transformation; // 当前变换
	double length;          // 到下一个关节的距离
};
struct addComponentRobotData {
	QString name;

	ShapeType shapeType = ShapeType_None;
	QVector<TopoDS_Shape> shapes;
	QVector<AIS_Shape> originalShapes;
	QVector<Handle(AIS_Shape)> myAisShapes;
	QVector<Handle(AIS_Shape)> disPlayShapes;
	QVector<Handle(AIS_Shape)> ShapesBox;

	bool isChanged = false;

	QString nextShapeName;
	QStringList nextShapeNames;
	Quantity_Color color = Quantity_Color(179 / 255.0, 174 / 255.0, 170 / 255.0, Quantity_TOC_RGB);
	int ShapeLinkIndex = -1;
	gp_Pnt center = gp_Pnt(0, 0, 0);
	gp_Dir xDir = gp_Dir(1, 0, 0);
	gp_Dir yDir = gp_Dir(0, 1, 0);
	gp_Dir zDir = gp_Dir(0, 0, 1);

	gp_Ax1 axisX = gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(1, 0, 0));
	gp_Ax1 axisY = gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(0, 1, 0));
	gp_Ax1 axisZ = gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1));

	double lengthX = 0.0;
	double lengthY = 0.0;
	double lengthZ = 0.0;

	gp_Pnt assemblyPoint = gp_Pnt(0, 0, 0);
	gp_Pnt nextAssemblyPoint = gp_Pnt(0, 0, 0);
	gp_Pnt translationPoint = gp_Pnt(0, 0, 0);

	double angleX = 0.0;
	double angleY = 0.0;
	double angleZ = 0.0;

	gp_Dir xDirCurrent;
	gp_Dir yDirCurrent;
	gp_Dir zDirCurrent;

	// 矩阵变换相关
	double a = 0.0;
	double alpha = 0.0;
	double d = 0.0;
	double theta = 0.0;

	double sacle = 1.0;

	// x，y，z	
	double x = 0.0;
	double y = 0.0;
	double z = 0.0;

	QString path;

	gp_Ax3 ShapeAxl3 = gp_Ax3();

	void initShapeAx3()
	{
		ShapeAxl3 = gp_Ax3();
		// 缩放
		setShapeScale(sacle);
		// 绕 X 轴旋转
		setShapeRotate(ShapeAxl3.Location(), ShapeAxl3.XDirection(), angleX / 180.0 * M_PI);
		// 绕 Y 轴旋转
		setShapeRotate(ShapeAxl3.Location(), ShapeAxl3.YDirection(), angleY / 180.0 * M_PI);
		// 绕 Z 轴旋转
		setShapeRotate(ShapeAxl3.Location(), ShapeAxl3.Direction(), angleZ / 180.0 * M_PI);



		// 平移
		setShapeMoveX(assemblyPoint.X());
		setShapeMoveY(assemblyPoint.Y());
		setShapeMoveZ(assemblyPoint.Z());
	}

	// 当前坐标系  Z X Y 方向
	gp_Dir ShapDirZ() { return ShapeAxl3.Direction(); }
	gp_Dir ShapDirX() { return ShapeAxl3.XDirection(); }
	gp_Dir ShapDirY() { return ShapeAxl3.YDirection(); }

	// 获取 旋转轴
	gp_Ax1 getRotationAxis() { return ShapeAxl3.Axis(); }

	// 获取 旋转角度
	// 当前坐标系  原点
	gp_Pnt ShapPnt() { return ShapeAxl3.Location(); }
	//  设置 变换 方向
	void setShapDirZ(gp_Dir dir) { ShapeAxl3.SetDirection(dir); }
	void setShapDirX(gp_Dir dir) { ShapeAxl3.SetXDirection(dir); }
	void setShapDirY(gp_Dir dir) { ShapeAxl3.SetYDirection(dir); }
	//  设置 变换 原点
	void setShapPnt(gp_Pnt pnt) { ShapeAxl3.SetLocation(pnt); }

	// 计算 X 方向 移动
	void setShapeMoveX(double distance) { ShapeAxl3.Translate(gp_Vec(distance, 0, 0)); }
	// 计算 Y 方向 移动
	void setShapeMoveY(double distance) { ShapeAxl3.Translate(gp_Vec(0, distance, 0)); }
	// 计算 Z 方向 移动
	void setShapeMoveZ(double distance) { ShapeAxl3.Translate(gp_Vec(0, 0, distance)); }

	// 计算旋转
	void setShapeRotate(gp_Pnt pnt, gp_Dir dir, double angle) { ShapeAxl3.Rotate(gp_Ax1(pnt, dir), angle); }
	void setShapeRotate(gp_Ax1 ax1, double angle) { ShapeAxl3.Rotate(ax1, angle); }

	// 计算缩放
	void setShapeScale(double scale) { ShapeAxl3 = ShapeAxl3.Scaled(ShapeAxl3.Location(), scale); }


	void printAll()
	{
		qDebug() << "name:" << name;
		qDebug() << "shapeType:" << shapeType;
		qDebug() << "shapes:" << shapes.size();
		qDebug() << "originalShapes:" << originalShapes.size();
		qDebug() << "myAisShapes:" << myAisShapes.size();
		qDebug() << "disPlayShapes:" << disPlayShapes.size();
		qDebug() << "ShapesBox:" << ShapesBox.size();
		qDebug() << "nextShapeName:" << nextShapeName;
		qDebug() << "nextShapeNames:" << nextShapeNames.size();
		qDebug() << "color:" << color.Name();
		qDebug() << "ShapeLinkIndex:" << ShapeLinkIndex;
		qDebug() << "center:" << center.X() << center.Y() << center.Z();
		qDebug() << "xDir:" << xDir.X() << xDir.Y() << xDir.Z();
		qDebug() << "yDir:" << yDir.X() << yDir.Y() << yDir.Z();
		qDebug() << "zDir:" << zDir.X() << zDir.Y() << zDir.Z();
		qDebug() << "axisX:" << axisX.Location().X() << axisX.Location().Y() << axisX.Location().Z() << axisX.Direction().X() << axisX.Direction().Y() << axisX.Direction().Z();
		qDebug() << "axisY:" << axisY.Location().X() << axisY.Location().Y() << axisY.Location().Z() << axisY.Direction().X() << axisY.Direction().Y() << axisY.Direction().Z();
		qDebug() << "axisZ:" << axisZ.Location().X() << axisZ.Location().Y() << axisZ.Location().Z() << axisZ.Direction().X() << axisZ.Direction().Y() << axisZ.Direction().Z();
		qDebug() << "lengthX:" << lengthX;
		qDebug() << "lengthY:" << lengthY;
		qDebug() << "lengthZ:" << lengthZ;
		qDebug() << "assemblyPoint:" << assemblyPoint.X() << assemblyPoint.Y() << assemblyPoint.Z();
		qDebug() << "nextAssemblyPoint:" << nextAssemblyPoint.X() << nextAssemblyPoint.Y() << nextAssemblyPoint.Z();
		qDebug() << "translationPoint:" << translationPoint.X() << translationPoint.Y() << translationPoint.Z();
		qDebug() << "angleX:" << angleX;
		qDebug() << "angleY:" << angleY;
		qDebug() << "angleZ:" << angleZ;
		qDebug() << "xDirCurrent:" << xDirCurrent.X() << xDirCurrent.Y() << xDirCurrent.Z();
		qDebug() << "yDirCurrent:" << yDirCurrent.X() << yDirCurrent.Y() << yDirCurrent.Z();
		qDebug() << "zDirCurrent:" << zDirCurrent.X() << zDirCurrent.Y() << zDirCurrent.Z();
		qDebug() << "a:" << a;
		qDebug() << "alpha:" << alpha;
		qDebug() << "d:" << d;
		qDebug() << "theta:" << theta;
		qDebug() << "sacle:" << sacle;
		qDebug() << "x:" << x;
		qDebug() << "y:" << y;
		qDebug() << "z:" << z;
		qDebug() << "ShapeAxl3:" << ShapeAxl3.Location().X() << ShapeAxl3.Location().Y() << ShapeAxl3.Location().Z() << ShapeAxl3.Direction().X() << ShapeAxl3.Direction().Y() << ShapeAxl3.Direction().Z() << ShapeAxl3.XDirection().X() << ShapeAxl3.XDirection().Y() << ShapeAxl3.XDirection().Z() << ShapeAxl3.YDirection().X() << ShapeAxl3.YDirection().Y() << ShapeAxl3.YDirection().Z();
	}

} typedef ADDROBOTDATA;
struct transformData
{
	QString name;
	// 平移
	gp_Vec translationVec = gp_Vec(0, 0, 0);
	// 旋转
	gp_Pnt rotationCenter = gp_Pnt(0, 0, 0);

	gp_Dir rotationAxis = gp_Dir(0, 1, 0);
	gp_Dir rotationAxisSEC = gp_Dir(0, 1, 0);
	gp_Ax1 rotationAxisAll = gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(0, 1, 0));
	gp_Dir xDir = gp_Dir(1, 0, 0);
	gp_Dir yDir = gp_Dir(0, 1, 0);
	gp_Dir zDir = gp_Dir(0, 0, 1);

	// 第一个装配点,必设
	gp_Pnt assemblyPoint = gp_Pnt(0, 0, 0);
	double orientationAngleX = 0.0;
	double orientationAngleY = 0.0;
	double orientationAngleZ = 0.0;


	double rotationAngle = 0.0;
	double rotationActiveAngle = 0.0;

	double angleX = 0.0;
	double angleY = 0.0;
	double angleZ = 0.0;

	gp_Pnt translation;

	gp_Ax3 ShapeAxl3 = gp_Ax3();

	void printAll()
	{
		qDebug() << "TRANSFORMDATA name:" << name;
		qDebug() << "translationVec:" << translationVec.X() << translationVec.Y() << translationVec.Z();
		qDebug() << "rotationCenter:" << rotationCenter.X() << rotationCenter.Y() << rotationCenter.Z();
		qDebug() << "rotationAxis:" << rotationAxis.X() << rotationAxis.Y() << rotationAxis.Z();
		qDebug() << "rotationAxisSEC:" << rotationAxisSEC.X() << rotationAxisSEC.Y() << rotationAxisSEC.Z();
		qDebug() << "rotationAxisAll:" << rotationAxisAll.Location().X() << rotationAxisAll.Location().Y() << rotationAxisAll.Location().Z() << rotationAxisAll.Direction().X() << rotationAxisAll.Direction().Y() << rotationAxisAll.Direction().Z();
		qDebug() << "xDir:" << xDir.X() << xDir.Y() << xDir.Z();
		qDebug() << "yDir:" << yDir.X() << yDir.Y() << yDir.Z();
		qDebug() << "zDir:" << zDir.X() << zDir.Y() << zDir.Z();
		qDebug() << "assemblyPoint:" << assemblyPoint.X() << assemblyPoint.Y() << assemblyPoint.Z();
		qDebug() << "orientationAngleX:" << orientationAngleX;
		qDebug() << "orientationAngleY:" << orientationAngleY;
		qDebug() << "orientationAngleZ:" << orientationAngleZ;
		qDebug() << "rotationAngle:" << rotationAngle;
		qDebug() << "rotationActiveAngle:" << rotationActiveAngle;
		qDebug() << "angleX:" << angleX;
		qDebug() << "angleY:" << angleY;
		qDebug() << "angleZ:" << angleZ;
		qDebug() << "ShapeAxl3:" << ShapeAxl3.Location().X() << ShapeAxl3.Location().Y() << ShapeAxl3.Location().Z() << ShapeAxl3.Direction().X() << ShapeAxl3.Direction().Y() << ShapeAxl3.Direction().Z() << ShapeAxl3.XDirection().X() << ShapeAxl3.XDirection().Y() << ShapeAxl3.XDirection().Z() << ShapeAxl3.YDirection().X() << ShapeAxl3.YDirection().Y() << ShapeAxl3.YDirection().Z();
	}
}typedef TRANSFORMDATA;

struct RobotRotateData
{
	QString name;
	double angle;
	MoveDirection direction;
}typedef ROTATEDATA;

// 定义工作区数据结构
struct WorkDATAStruct
{
	// 焊缝名称
	QString workName;

	//下一个形状名称
	QString nextShapeName = QString();
	QVector<QString> nextShapeNames = QVector<QString>();
	// 焊缝颜色
	QString color = "#ffffff";

	// 焊缝形状
	QVector<TopoDS_Shape> holeShape = QVector<TopoDS_Shape>();

	// 焊缝尺寸
	QString sacle = "1.0";

	// 焊缝位置
	QString positionX = "0.0";
	QString positionY = "0.0";
	QString positionZ = "0.0";

	// 焊缝角度
	QString angleX = "0.0";
	QString angleY = "0.0";
	QString angleZ = "0.0";

	QString path = QString();

	double alpha = 0.0;
	double aDistance = 0.0;
	double theta = 0.0;
	double dDistance = 0.0;
}typedef WORKDATA;

// 定义焊缝数据结构
struct HoleDATAStruct
{
	// 焊缝名称
	QString holeName;

	//下一个形状名称
	QString nextShapeName = QString();

	// 焊缝颜色
	QString color = "#ffffff";

	// 焊缝形状
	QVector<TopoDS_Shape> holeShape = QVector<TopoDS_Shape>();

	// 焊缝尺寸
	QString sacle = "1.0";

	// 焊缝位置
	QString positionX = "0.0";
	QString positionY = "0.0";
	QString positionZ = "0.0";

	// 焊缝角度
	QString angleX = "0.0";
	QString angleY = "0.0";
	QString angleZ = "0.0";

	QString path = QString();

	double alpha = 0.0;
	double aDistance = 0.0;
	double theta = 0.0;
	double dDistance = 0.0;

}typedef HOLEDATA;

// 定义工作区和焊缝数据结构
struct workAndHoleStruct
{
	QString workName;
	WorkDATAStruct workData;
	QMap<QString, HOLEDATA> holeShape;

}typedef WORKANDHOLE;


struct ShapeLinkData
{
	QString LinkName;
	int index = -1;
	ShapeType shapeType = ShapeType_Link;

	double alpha = 0.0;
	double aDistance = 0.0;
	double theta = 0.0;
	double dDistance = 0.0;

	QVector<double> getDHParameter()
	{
		QVector<double> DHParameter;
		DHParameter.append(aDistance);
		DHParameter.append(alpha);
		DHParameter.append(dDistance);
		DHParameter.append(theta);

		return DHParameter;
	}
}typedef SHAPELINKDATA;
// 定义焊缝数据结构
struct ShapeDataStruct
{
	ShapeType shapeType = ShapeType_None;
	// 焊缝名称
	QString ShapeName;

	int ShapeIndex = -1; // 形状序号

	//下一个形状名称
	QString nextShapeName = QString();
	QStringList nextShapeNames = QStringList();

	// 焊缝颜色
	QString ShapeColor = "#ffffff";

	// 焊缝形状
	QVector<TopoDS_Shape> ShapeVector = QVector<TopoDS_Shape>();

	// 焊缝尺寸
	QString ShapeScale = "1.0";

	// 焊缝位置
	QString ShapePositionX = "0.0";
	QString ShapePositionY = "0.0";
	QString ShapePositionZ = "0.0";

	// 焊缝角度
	QString ShapeAngleX = "0.0";
	QString ShapeAngleY = "0.0";
	QString ShapeAngleZ = "0.0";

	QString ShapePath = QString();

	//double alpha = 0.0;
	//double aDistance = 0.0;
	//double theta = 0.0;
	//double dDistance = 0.0;

	bool ShapeLink = false;

	int ShapeLinkIndex = -1;

	QMap<QString, SHAPELINKDATA> shapeLinkData;

	QVariantMap getShapeVariantMap()
	{
		QVariantMap map;
		map.insert("ShapeName", ShapeName);
		map.insert("nextShapeName", nextShapeName);
		map.insert("ShapeColor", ShapeColor);
		map.insert("ShapeScale", ShapeScale);
		map.insert("ShapePositionX", ShapePositionX);
		map.insert("ShapePositionY", ShapePositionY);
		map.insert("ShapePositionZ", ShapePositionZ);
		map.insert("ShapeAngleX", ShapeAngleX);
		map.insert("ShapeAngleY", ShapeAngleY);
		map.insert("ShapeAngleZ", ShapeAngleZ);
		map.insert("ShapePath", ShapePath);
		map.insert("ShapeType", shapeType);
		map.insert("isChecked", ShapeLink);
		map.insert("nextShapeNames", nextShapeNames);
		map.insert("ShapeLinkIndex", ShapeLinkIndex);
		map.insert("ShapeIndex", ShapeIndex);

		QVariantMap linkMapTemp;
		for (auto it = shapeLinkData.begin(); it != shapeLinkData.end(); ++it)
		{
			QVariantMap linkMap;
			linkMap.insert("index", it.value().index);
			linkMap.insert("shapeType", it.value().shapeType);
			linkMap.insert("alpha", it.value().alpha);
			linkMap.insert("aDistance", it.value().aDistance);
			linkMap.insert("theta", it.value().theta);
			linkMap.insert("dDistance", it.value().dDistance);
			linkMapTemp.insert(it.key(), linkMap);
		}
		map.insert("LinkData", linkMapTemp);
		return map;
	}
	void setShapeVariantMap(QVariantMap map)
	{
		ShapeName = map.value("ShapeName").toString();
		nextShapeName = map.value("nextShapeName").toString();
		ShapeColor = map.value("ShapeColor").toString();
		ShapeScale = map.value("ShapeScale").toString();
		ShapePositionX = map.value("ShapePositionX").toString();
		ShapePositionY = map.value("ShapePositionY").toString();
		ShapePositionZ = map.value("ShapePositionZ").toString();
		ShapeAngleX = map.value("ShapeAngleX").toString();
		ShapeAngleY = map.value("ShapeAngleY").toString();
		ShapeAngleZ = map.value("ShapeAngleZ").toString();
		ShapePath = map.value("ShapePath").toString();
		shapeType = ShapeType(map.value("ShapeType").toInt());
		ShapeLink = map.value("isChecked").toBool();
		nextShapeNames = map.value("nextShapeNames").toStringList();
		ShapeLinkIndex = map.value("ShapeLinkIndex").toInt();
		ShapeIndex = map.value("ShapeIndex").toInt();

		QVariantMap linkMap = map.value("LinkData").toMap();
		for (auto it = linkMap.begin(); it != linkMap.end(); ++it)
		{
			SHAPELINKDATA linkData;
			linkData.LinkName = it.key();
			linkData.index = it.value().toMap().value("index").toInt();
			linkData.shapeType = ShapeType(it.value().toMap().value("shapeType").toInt());
			linkData.alpha = it.value().toMap().value("alpha").toDouble();
			linkData.aDistance = it.value().toMap().value("aDistance").toDouble();
			linkData.theta = it.value().toMap().value("theta").toDouble();
			linkData.dDistance = it.value().toMap().value("dDistance").toDouble();
			shapeLinkData.insert(it.key(), linkData);
		}

	}

}typedef SHAPESTRUCT;

#define  WORKPATHNAME         "WORK"
#define  WORKCONFIGPATH       "WorkConfig.json"
#define  ROBOTCONFIGPATH      "RobotConfig.json"
#define  OTERDATAPATH         "OtherConfig.json"
#define  TEACHDATAPATH        "TeachConfig.json"

#define  ISNULLPOINTER(p)     if(p==nullptr){return;}
#define  ISNULLPOINTERSTRING(p)      if(p==nullptr){return QString();}
// 轨迹
struct TrajectoryStruct
{
	int swcIndex = -1; // swc序号
	QString trackPosition = "0.0"; // 位置X
	QString TrajectoryName = ""; // 轨迹名称

	void setVariMap(QVariantMap map)
	{
		swcIndex = map.value("swcIndex").toInt();
		trackPosition = map.value("trackPosition").toString();
		TrajectoryName = map.value("TrajectoryName").toString();
	}
	QVariantMap getVariMap()
	{
		QVariantMap map;
		map.insert("swcIndex", swcIndex);
		map.insert("trackPosition", trackPosition);
		map.insert("TrajectoryName", TrajectoryName);
		return map;
	}
};

struct WorkpieceStruct
{
	int WorkpieceIndex = -1; // 工作件序号
	QString WorkpieceName = __StandQString("焊缝-").append(QString::number(WorkpieceIndex)); // 工作件名称
	QString HolePosition = ""; // 工作件位置X

	QString HoleModeName = "NULL";
	QString PostioName = "";
	QStringList HoleModeNames = { "NULL" };


	QStringList FTDirList = { "X", "Y", "Z","A","B","C" };
	QString CurFTDir = "X"; // 当前方向
	QStringList FTValueList = { "1","5","10" }; // 值列表
	QString CurFTValue = "0.0"; // 当前值

	// VC参数
	QString VcOB = "40"; // 起始点
	QString VcOC = "1265"; // 终止点
	QString VcA0 = "30"; // 
	QString VcA1 = "30"; // 
	QString VcA2 = "30"; // 
	QString VcB1 = "30"; //
	QString VcB2 = "30"; //
	QString VcLen = "1"; //

	QString VC0 = "";
	QString VC1 = "";
	QString VC2 = "";

	QStringList getVCList()
	{
		QStringList vcList;
		if (!VC0.isEmpty())
		{
			vcList << "VC0";
		}
		if (!VC1.isEmpty())
		{
			vcList << "VC1";
		}
		if (!VC2.isEmpty())
		{
			vcList << "VC2";
		}
		return vcList;
	}; // VC列表
	QString getVCValue(QString vcName)
	{
		if (vcName == "VC0")
		{
			return VC0;
		}
		else if (vcName == "VC1")
		{
			return VC1;
		}
		else if (vcName == "VC2")
		{
			return VC2;
		}
		else
		{
			return "";
		}
	}; // 获取VC值
	void setVCValue(QString vcName, QString value)
	{
		if (vcName == "VC0")
		{
			VcOB = value;
		}
		else if (vcName == "VC1")
		{
			VcOC = value;
		}
		else if (vcName == "VC2")
		{
			VcLen = value;
		}
	}; // 设置VC值


	QMap<int, TrajectoryStruct> TrajectoryMap; // 轨迹参数

	// 获取 进入 轨迹列表
	QStringList getEnterTrajectoryList()
	{
		QStringList enterTrajectoryList;
		for (auto it = TrajectoryMap.begin(); it != TrajectoryMap.end(); ++it)
		{
			if (it.value().swcIndex != -1)
			{
				enterTrajectoryList.append(it.value().trackPosition);
			}
		}
		return enterTrajectoryList;
	}
	// 获取 离开 轨迹列表
	QStringList getLeaveTrajectoryList()
	{
		QStringList leaveTrajectoryList;
		QStringList enterTrajectoryList = getEnterTrajectoryList();
		for (int i = enterTrajectoryList.size() - 1; i >= 0; i--)
		{
			leaveTrajectoryList.append(enterTrajectoryList[i]);
		}
		return leaveTrajectoryList;
	}

	QStringList getTrajectoryList()
	{
		QStringList trajectoryList;
		for (auto it = TrajectoryMap.begin(); it != TrajectoryMap.end(); ++it)
		{
			trajectoryList.append(it.value().TrajectoryName);
		}
		return trajectoryList;
	}

	//
	void setVariMap(QVariantMap map)
	{
		WorkpieceIndex = map.value("WorkpieceIndex").toInt();
		WorkpieceName = map.value("WorkpieceName").toString();
		HolePosition = map.value("HolePosition").toString();
		HoleModeName = map.value("HoleModeName").toString();
		HoleModeNames = map.value("HoleModeNames").toStringList();
		PostioName = map.value("PostioName").toString();
		FTDirList = map.value("FTDirList").toStringList();
		CurFTDir = map.value("CurFTDir").toString();
		FTValueList = map.value("FTValueList").toStringList();
		CurFTValue = map.value("CurFTValue").toString();
		VcOB = map.value("VcOB").toString();
		VcOC = map.value("VcOC").toString();
		VcA0 = map.value("VcA0").toString();
		VcA1 = map.value("VcA1").toString();
		VcA2 = map.value("VcA2").toString();
		VcB1 = map.value("VcB1").toString();
		VcB2 = map.value("VcB2").toString();
		VcLen = map.value("VcLen").toString();
		VC0 = map.value("VC0").toString();
		VC1 = map.value("VC1").toString();
		VC2 = map.value("VC2").toString();

		QVariantMap trajectoryMap = map.value("TrajectoryMap").toMap();
		for (auto it = trajectoryMap.begin(); it != trajectoryMap.end(); ++it)
		{
			TrajectoryStruct trajectory;
			trajectory.setVariMap(it.value().toMap());
			TrajectoryMap.insert(it.key().toInt(), trajectory);
		}
	}
	QVariantMap getVariMap()
	{
		QVariantMap map;
		map.insert("WorkpieceIndex", WorkpieceIndex);
		map.insert("WorkpieceName", WorkpieceName);
		map.insert("HolePosition", HolePosition);
		map.insert("HoleModeName", HoleModeName);
		map.insert("HoleModeNames", HoleModeNames);
		map.insert("PostioName", PostioName);
		map.insert("FTDirList", FTDirList);
		map.insert("CurFTDir", CurFTDir);
		map.insert("FTValueList", FTValueList);
		map.insert("CurFTValue", CurFTValue);
		map.insert("VcOB", VcOB);
		map.insert("VcOC", VcOC);
		map.insert("VcA0", VcA0);
		map.insert("VcA1", VcA1);
		map.insert("VcA2", VcA2);
		map.insert("VcB1", VcB1);
		map.insert("VcB2", VcB2);
		map.insert("VcLen", VcLen);
		map.insert("VC0", VC0);
		map.insert("VC1", VC1);
		map.insert("VC2", VC2);

		QVariantMap trajectoryMapTemp;
		for (auto it = TrajectoryMap.begin(); it != TrajectoryMap.end(); ++it)
		{
			trajectoryMapTemp.insert(QString::number(it.key()), it.value().getVariMap());
		}
		map.insert("TrajectoryMap", trajectoryMapTemp);
		return map;
	}

};

struct SaftPointStruct
{
	int SaftPointIndex = -1; // 点序号
	QString SaftPointName = __StandQString("HOME-").append(QString::number(SaftPointIndex)); // 点名称
	QString SaftPointJsonName = __StandQString("SaftPoint_").append(QString::number(SaftPointIndex)); // json点名称
	QString SaftPointPosition = ""; // 点位置X

	QMap<int, WorkpieceStruct> SaftPointMap; // 点参数

	QStringList getWorkpieceList()
	{
		QStringList workpieceList;
		for (auto it = SaftPointMap.begin(); it != SaftPointMap.end(); ++it)
		{
			workpieceList.append(it.value().WorkpieceName);
		}
		return workpieceList;
	}

	void setVariMap(QVariantMap map)
	{
		SaftPointIndex = map.value("SaftPointIndex").toInt();
		SaftPointName = map.value("SaftPointName").toString();
		SaftPointJsonName = map.value("SaftPointJsonName").toString();
		SaftPointPosition = map.value("SaftPointPosition").toString();

		QVariantMap workpieceMap = map.value("SaftPointMap").toMap();
		for (auto it = workpieceMap.begin(); it != workpieceMap.end(); ++it)
		{
			WorkpieceStruct workpiece;
			workpiece.setVariMap(it.value().toMap());
			SaftPointMap.insert(it.key().toInt(), workpiece);
		}
	}
	QVariantMap getVariMap()
	{
		QVariantMap map;
		map.insert("SaftPointIndex", SaftPointIndex);
		map.insert("SaftPointName", SaftPointName);
		map.insert("SaftPointJsonName", SaftPointJsonName);
		map.insert("SaftPointPosition", SaftPointPosition);

		QVariantMap workpieceMapTemp;
		for (auto it = SaftPointMap.begin(); it != SaftPointMap.end(); ++it)
		{
			workpieceMapTemp.insert(QString::number(it.key()), it.value().getVariMap());
		}
		map.insert("SaftPointMap", workpieceMapTemp);
		return map;
	}
};
#define MOVESTRUCTPATH(x) x.isEmpty()? WORKPATH + "/" +TEACHDATAPATH : WORKPATH + "/" + x + "/" +TEACHDATAPATH
#define SHAPEMODEPATH(x) x.isEmpty()? WORKPATH + "/" +WORKCONFIGPATH : WORKPATH + "/" + x + "/" +WORKCONFIGPATH

struct MoveStruct
{
	QMap<int, SaftPointStruct> MoveMap; // 点参数
	QString WorkName = "NULL"; // 工作名称

	void clear()
	{
		MoveMap.clear();
		WorkName = "NULL";
	}

	QStringList getSendValueList(int saftCurrent, int workCurrent, int nextSaft, int nextWork)
	{

		QStringList sendValueList;
		// 在同一个安全点下，工作件的切换
		if (saftCurrent == nextSaft)
		{
			// 点击 同一位置 返回
			/*if (workCurrent == nextWork)
			{
				return sendValueList;
			}*/
			// 获取当前工作件的 离开 轨迹列表
			QStringList currentSaftList;
			if (saftCurrent != -1 && workCurrent != -1)
			{
				currentSaftList = MoveMap[saftCurrent].SaftPointMap[workCurrent].getLeaveTrajectoryList();
				currentSaftList.append(MoveMap[saftCurrent].SaftPointMap[workCurrent].HolePosition);
			}

			// 获取 下一个工作件的 进入 轨迹列表
			QStringList nextSaftList;
			if (nextSaft != -1 && nextWork != -1)
			{
				nextSaftList.append(MoveMap[nextSaft].SaftPointMap[nextWork].HolePosition);
				nextSaftList.append(MoveMap[nextSaft].SaftPointMap[nextWork].getEnterTrajectoryList());
			}
			// 发送 离开 轨迹列表 进入 轨迹列表
			sendValueList << currentSaftList << nextSaftList;
		}
		else
		{
			// 点击 不同位置 返回

			// 获取当前工作件的 离开 轨迹列表
			QStringList currentSaftList;
			if (saftCurrent != -1 && workCurrent != -1)
			{
				currentSaftList = MoveMap[saftCurrent].SaftPointMap[workCurrent].getLeaveTrajectoryList();
				currentSaftList.append(MoveMap[saftCurrent].SaftPointMap[workCurrent].HolePosition);
			}

			// 获取 下一个安全点的 进入 轨迹列表
			QStringList nextSaftList;
			if (nextSaft != -1 && nextWork != -1)
			{
				nextSaftList.append(MoveMap[nextSaft].SaftPointMap[nextWork].HolePosition);
				nextSaftList.append(MoveMap[nextSaft].SaftPointMap[nextWork].getEnterTrajectoryList());
			}

			// 发送 离开 轨迹列表 安全点 进入 轨迹列表
			QStringList saftIndexList;
			if (saftCurrent != -1)
			{
				bool isEnter = saftCurrent < nextSaft ? true : false;
				if (isEnter)
				{
					for (auto it = MoveMap.begin(); it != MoveMap.end(); ++it)
					{
						if (it.key() >= saftCurrent && it.key() <= nextSaft)
						{
							saftIndexList.append(MoveMap[it.key()].SaftPointPosition);
						}
					}
				}
				else
				{
					QStringList saftIndexListTemp;
					for (auto it = MoveMap.begin(); it != MoveMap.end(); ++it)
					{
						if (it.key() >= nextSaft && it.key() <= saftCurrent)
						{
							saftIndexListTemp.append(MoveMap[it.key()].SaftPointPosition);
						}
					}
					for (int i = saftIndexListTemp.size() - 1; i >= 0; i--)
					{
						saftIndexList.append(saftIndexListTemp[i]);
					}

				}

			}
			else
			{
				saftIndexList.append(MoveMap[nextSaft].SaftPointPosition);
			}
			sendValueList << currentSaftList << saftIndexList << nextSaftList;
		}

		return sendValueList;

	}

	QString getModeName(int saft, int work)
	{
		if (MoveMap.keys().contains(saft))
		{
			if (MoveMap[saft].SaftPointMap.contains(work))
			{
				return MoveMap[saft].SaftPointMap[work].HoleModeName;
			}
		}
		return "NULL";
	}
	// 获取最大安全点索引
	int getMaxSaftPointIndex()
	{
		int maxIndex = -1;
		for (auto it = MoveMap.begin(); it != MoveMap.end(); ++it)
		{
			if (it.key() > maxIndex)
			{
				maxIndex = it.key();
			}
		}
		return maxIndex;
	}

	void setVariMap(QVariantMap map)
	{
		QVariantMap saftPointMap = map.value("SaftPointMap").toMap();
		for (auto it = saftPointMap.begin(); it != saftPointMap.end(); ++it)
		{
			SaftPointStruct saftPoint;
			saftPoint.setVariMap(it.value().toMap());
			MoveMap.insert(it.key().toInt(), saftPoint);
		}
	}
	QVariantMap getVariMap()
	{
		QVariantMap map;
		QVariantMap saftPointMapTemp;
		for (auto it = MoveMap.begin(); it != MoveMap.end(); ++it)
		{
			saftPointMapTemp.insert(QString::number(it.key()), it.value().getVariMap());
		}
		map.insert("SaftPointMap", saftPointMapTemp);
		return map;
	}

	void SaveJson(QString filePath)
	{
		QString dirPath = filePath.left(filePath.lastIndexOf("/"));
		// 创建 文件夹及文件
		if (!QDir(dirPath).exists())
		{
			QDir().mkpath(dirPath);
		}

		QVariantMap map = getVariMap();
		QJsonDocument doc = QJsonDocument(QJsonObject::fromVariantMap(map));
		QFile file(filePath);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			qDebug() << "open file error";
			return;
		}
		QTextStream out(&file);
		out << doc.toJson();
		file.close();
	}

	void LoadJson(QString path)
	{
		clear();
		QFile file(path);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			qDebug() << "open file error";

			//创建默认文件
			MoveStruct move;
			move.SaveJson(path);
			return;
		}
		QTextStream in(&file);
		QJsonDocument doc = QJsonDocument::fromJson(in.readAll().toUtf8());
		QVariantMap map = doc.toVariant().toMap();
		setVariMap(map);
		file.close();
	}

	void changedModeNames(QStringList modeNames)
	{
		for (auto it = MoveMap.begin(); it != MoveMap.end(); ++it)
		{
			for (auto it2 = it.value().SaftPointMap.begin(); it2 != it.value().SaftPointMap.end(); ++it2)
			{
				it2.value().HoleModeNames = modeNames;
			}
		}

	}

	// 获取安全点列表
	QStringList getSaftPointList()
	{
		QStringList saftPointList;
		for (auto it = MoveMap.begin(); it != MoveMap.end(); ++it)
		{
			saftPointList.append(it.value().SaftPointName);
		}
		return saftPointList;
	}

	// 获取工作件列表
	QStringList getWorkpieceList(int saftPointIndex)
	{
		QStringList workpieceList;
		if (MoveMap.contains(saftPointIndex))
		{
			for (auto it = MoveMap[saftPointIndex].SaftPointMap.begin(); it != MoveMap[saftPointIndex].SaftPointMap.end(); ++it)
			{
				workpieceList.append(it.value().WorkpieceName);
			}
		}
		return workpieceList;
	}

	// 获取工作件轨迹列表
	QStringList getTrajectoryList(int saftPointIndex, int workpieceIndex)
	{
		QStringList trajectoryList;
		if (MoveMap.contains(saftPointIndex) && MoveMap[saftPointIndex].SaftPointMap.contains(workpieceIndex))
		{
			for (auto it = MoveMap[saftPointIndex].SaftPointMap[workpieceIndex].TrajectoryMap.begin(); it != MoveMap[saftPointIndex].SaftPointMap[workpieceIndex].TrajectoryMap.end(); ++it)
			{
				trajectoryList.append(QString::number(it.value().swcIndex));
			}
		}
		return trajectoryList;
	}
};

struct trackItemStruct
{
	int index = -1;
	QTreeWidgetItem* item = nullptr;
};
struct workitemStruct
{
	int index = -1;
	QTreeWidgetItem* item = nullptr;
	QVector< trackItemStruct> vecTrack;

};
struct saftItemStruct
{
	int index = -1;
	QTreeWidgetItem* item = nullptr;
	QVector<workitemStruct> vecwork;
};
struct treeItemStruct
{
	QVector<saftItemStruct> vecSoft;

	int saftIndex = -1;
	int workIndex = -1;
	int trackIndex = -1;

	bool isFindSaftAndWork(int saftIndex, int workIndex)
	{
		if (vecSoft.size() == 0)
		{
			return false;
		}
		for (int i = 0; i < vecSoft.size(); i++)
		{
			if (vecSoft[i].index == saftIndex)
			{
				for (int j = 0; j < vecSoft[i].vecwork.size(); j++)
				{
					if (vecSoft[i].vecwork[j].index == workIndex)
					{
						return true;
					}
				}
			}
		}
		return false;

	}

	void getSaftIndexFromItem(QTreeWidgetItem* itemCompare)
	{
		foreach(auto saftItem, vecSoft)
		{
			if (saftItem.item == itemCompare)
			{
				saftIndex = saftItem.index;
			}
			else
			{
				foreach(auto work, saftItem.vecwork)
				{
					if (work.item == itemCompare)
					{
						saftIndex = saftItem.index;
						workIndex = work.index;
					}
					else
					{
						foreach(auto track, work.vecTrack)
						{
							if (track.item == itemCompare)
							{
								saftIndex = saftItem.index;
								workIndex = work.index;
								trackIndex = track.index;
							}
						}
					}

				}
			}
		}

	}
};