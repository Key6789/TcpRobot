#pragma once
#ifndef TCPROBOTMANAGER_GLOBAL_H
#define TCPROBOTMANAGER_GLOBAL_H


#include <QtCore/qglobal.h>

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

#include <Geom_ConicalSurface.hxx>
#include <Geom_ToroidalSurface.hxx>
#include <Geom_CylindricalSurface.hxx>

#include <GCE2d_MakeSegment.hxx>

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

#include "TcpXViewBase.h"
#include <QFormLayout>

#define   WORKPATH	QDir::currentPath().append("/RobotData/WorkPath")
#define   ROBOTPATH	QDir::currentPath().append("/RobotData/RobotPath")

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




#if defined(TCPROBOT_LIBRARY)
#define TCPROBOT_EXPORT Q_DECL_EXPORT
#else
#define TCPROBOT_EXPORT Q_DECL_IMPORT
#endif

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

	QVector<TopoDS_Shape> shapes;
	QVector<AIS_Shape> originalShapes;
	QVector<Handle(AIS_Shape)> myAisShapes;
	QVector<Handle(AIS_Shape)> disPlayShapes;
	QVector<Handle(AIS_Shape)> ShapesBox;

	QString nextShapeName;
	Quantity_Color color = Quantity_Color(179 / 255.0, 174 / 255.0, 170 / 255.0, Quantity_TOC_RGB);

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

	QVector<double> joints()
	{
		QVector<double> joints;
		joints.push_back(a);
		joints.push_back(alpha);
		joints.push_back(d);
		joints.push_back(theta);
		return joints;
	}
	// x，y，z	
	double x = 0.0;
	double y = 0.0;
	double z = 0.0;

	QString path;

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