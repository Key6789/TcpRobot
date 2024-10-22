﻿#pragma once
#ifndef TCPROBOTMANAGER_GLOBAL_H
#define TCPROBOTMANAGER_GLOBAL_H

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
#define   OTHERPATH	QDir::currentPath().append("/RobotData/OtherPath")
#define   TEACHPATH	QDir::currentPath().append("/RobotData/TeachPath")



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
enum ShapeType
{
	ShapeType_None = 0,
	ShapeType_Work,
	ShapeType_Hole,
	ShapeType_Robot,
	ShapeType_Link,
	ShapeType_LongGuide,
	ShapeType_RotatingTable,
	ShapeType_ShortGuide
	
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

	QString nextShapeName;
	QStringList nextShapeNames;
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


	QStringList FTDirList = {"X", "Y", "Z","A","B","C"}; 
	QString CurFTDir = "X"; // 当前方向
	QStringList FTValueList = {"1","5","10"}; // 值列表
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
	QString SaftPointName = __StandQString("安全点-").append(QString::number(SaftPointIndex)); // 点名称
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
#define MOVESTRUCTPATH WORKPATH + "/" + TEACHDATAPATH
struct MoveStruct
{
	QMap<int, SaftPointStruct> MoveMap; // 点参数

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