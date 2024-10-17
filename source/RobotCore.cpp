#include "../include/RobotCore.h"

namespace TCP_ROBOT
{
	RobotCore::RobotCore(QWidget* parent)
	{
	}
	RobotCore::~RobotCore()
	{
	}
	void RobotCore::slotUpdataRobotShaps(void)
	{

	}
	void RobotCore::loadWorkShapes(QString filePath)
	{
		removeAllPreview();
		// 读取文件内容 json 格式
		QFile file(filePath);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			qDebug() << "open file failed";
			return;
		}
		QTextStream in(&file);
		QString jsonStr = in.readAll();
		file.close();

		// 解析 json 格式
		QJsonParseError jsonError;
		QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr.toUtf8(), &jsonError);
		if (jsonError.error != QJsonParseError::NoError)
		{
			qDebug() << "parse json failed";
			return;
		}
		QVariantMap jsonMap = jsonDoc.object().toVariantMap();
		foreach(QString key, jsonMap.keys())
		{
			QVariantMap shapeMap = jsonMap[key].toMap();
			SHAPESTRUCT shapeStruct;
			shapeStruct.setShapeVariantMap(shapeMap);
			// 加载模型
			slotReplaceModelByPath(shapeStruct);
		}

		QTimer* timer = new QTimer(this);
		timer->setInterval(1000);
		connect(timer, &QTimer::timeout, [=]() {slotWorkRotateShape(30); });
		timer->start();

		// 加载参数
		// m_mapPreviewData QMap<QString, ADDROBOTDATA>
	}
	void RobotCore::loadRobotShape(QString filePath)
	{
	}
	void RobotCore::slotWorkRotateShape(double angle, MoveDirection moveType)
	{

		ADDROBOTDATA workData;
		foreach(ADDROBOTDATA addRobotData, m_mapPreviewData.values())
		{
			if (addRobotData.shapeType == ShapeType_Work)
			{
				switch (moveType)
				{
				case MoveDirection_XAxis:
					addRobotData.angleX += angle;
					break;
				case MoveDirection_YAxis:
					addRobotData.angleY += angle;
					break;
				case MoveDirection_ZAxis:
					addRobotData.angleZ += angle;
					break;
				default:
					break;
				}

				workData = ShapesRotated(addRobotData, angle);
				m_mapPreviewData.insert(workData.name, workData);
				break;
			}
		}
		QStringList nextShapeNames = workData.nextShapeNames;
		if (!nextShapeNames.isEmpty())
		{
			foreach(QString nextShapeName, nextShapeNames)
			{
				if (m_mapPreviewData.contains(nextShapeName))
				{
					ADDROBOTDATA nextShapeData = m_mapPreviewData[nextShapeName];
					if (nextShapeData.shapeType == ShapeType_Hole)
					{
						switch (moveType)
						{
						case MoveDirection_XAxis:
							nextShapeData.angleX += angle;
							break;
						case MoveDirection_YAxis:
							nextShapeData.angleY += angle;
							break;
						case MoveDirection_ZAxis:
							nextShapeData.angleZ += angle;
							break;
						default:
							break;
						}
						nextShapeData = ShapesRotated(nextShapeData, workData.assemblyPoint, angle);
						m_mapPreviewData.insert(nextShapeName, nextShapeData);
					}
				}
			}
		}
	}
	ADDROBOTDATA RobotCore::ShapesRotated(ADDROBOTDATA addRobotData, gp_Pnt center, double angle)
	{
		QVector<TopoDS_Shape> vector = addRobotData.shapes;
		QVector<Handle(AIS_Shape)> vectortemp;
		QVector<Handle(AIS_Shape)> ShapesBox;
		for (int i = 0; i < vector.size(); ++i)
		{
			if (!addRobotData.myAisShapes.isEmpty())
			{
				myContext->Remove(addRobotData.myAisShapes[i], Standard_True); // 移除旧的形状
			}

			// 保存原始位置，以便稍后恢复
			// gp_Pnt originalPosition = addRobotData.assemblyPoint;
			// 将形状平移到原点
			gp_Trsf moveToOrigin;
			moveToOrigin.SetTranslation(gp_Pnt(0, 0, 0).XYZ());
			TopoDS_Shape atOrigin = BRepBuilderAPI_Transform(vector[i], moveToOrigin).Shape();

			gp_Trsf localRotation;
			localRotation.SetRotation(addRobotData.axisX, addRobotData.angleX / 180.0 * M_PI); // X轴旋转
			TopoDS_Shape rotatedX = BRepBuilderAPI_Transform(atOrigin, localRotation).Shape();

			localRotation.SetRotation(addRobotData.axisY, addRobotData.angleY / 180.0 * M_PI); // Y轴旋转
			TopoDS_Shape rotatedXY = BRepBuilderAPI_Transform(rotatedX, localRotation).Shape();

			localRotation.SetRotation(addRobotData.axisZ, addRobotData.angleZ / 180.0 * M_PI); // Z轴旋转
			TopoDS_Shape rotatedXYZ = BRepBuilderAPI_Transform(rotatedXY, localRotation).Shape();

			Eigen::Vector3d pointEn(addRobotData.assemblyPoint.X(), addRobotData.assemblyPoint.Y(), addRobotData.assemblyPoint.Z());
			Eigen::Vector3d pivot(center.X(), center.Y(), center.Z());
			// 目标点
			Eigen::Vector3d result =  rotateAroundZAxis(pointEn, pivot,angle/180.0*M_PI);
			gp_Pnt newPoint = gp_Pnt(result[0], result[1], result[2]);
			addRobotData.assemblyPoint = newPoint;

			qDebug() << "newPoint: " << newPoint.X() << " " << newPoint.Y() << " " << newPoint.Z();
			gp_Trsf moveBack;
			moveBack.SetTranslation(newPoint.XYZ());
			rotatedXYZ.Move(moveBack);
			
			// 将形状移回原始位置
			 // 初始点
			//Eigen::Vector3d pointEn(addRobotData.assemblyPoint.X(), addRobotData.assemblyPoint.Y(), addRobotData.assemblyPoint.Z());
			//Eigen::Vector3d pivot(center.X(), center.Y(), center.Z());
			//// 目标点
			//Eigen::Vector3d result =  rotateAroundZAxis(pointEn, pivot,angle/180.0*M_PI);
			//gp_Pnt newPoint = gp_Pnt(result[0], result[1], result[2]);
			//gp_Trsf moveBack;
			//moveBack.SetTranslation(newPoint.XYZ());
			//TopoDS_Shape transformedComponent = BRepBuilderAPI_Transform(rotatedXYZ, moveBack).Shape();
			//
			//qDebug() << "newPoint: " << newPoint.X() << " " << newPoint.Y() << " " << newPoint.Z();
			//qDebug() << "originalPosition: " << originalPosition.X() << " " << originalPosition.Y() << " " << originalPosition.Z();
			//addRobotData.assemblyPoint = newPoint;
			 //位移
			/*gp_Trsf finalTransform;
			finalTransform.SetTranslation(gp_Vec(addRobotData.assemblyPoint.XYZ()));
			transformedComponent = BRepBuilderAPI_Transform(transformedComponent, finalTransform).Shape();*/



			aisShape = new AIS_Shape(rotatedXYZ);
			aisShape->SetColor(addRobotData.color);
			myContext->Display(aisShape, Standard_True);
			vectortemp.push_back(aisShape);

			// 计算形状的边界框
			Bnd_Box box;
			BRepBndLib::Add(rotatedXYZ, box);
			ShapesBox.push_back(aisShape);


		}
		addRobotData.myAisShapes = vectortemp;
		addRobotData.ShapesBox = ShapesBox;
		return addRobotData;
	}
	void RobotCore::ShapesLink()
	{
		// 读取文件 获取 连杆的各个参数

		// 按照索引顺序 连接 连杆的各个参数 送到 参数计算类

		// 将 计算结果按照 连杆的索引 保存到 模型的 连杆参数及保存


	}

	TopoDS_Shape RobotCore::ShapeRotateX(TopoDS_Shape shape, double angleX)
	{
		// 将形状平移到原点
		gp_Trsf moveToOrigin;
		gp_Pnt originPnt(0, 0, 0);
		moveToOrigin.SetTranslation(originPnt.XYZ());

		// 旋转
		gp_Trsf rotateX;
		rotateX.SetRotation(gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(1, 0, 0)), angleX * M_PI / 180.0);
		TopoDS_Shape rotatedShape = BRepBuilderAPI_Transform(shape, moveToOrigin * rotateX).Shape();

		return rotatedShape;
	}

	TopoDS_Shape RobotCore::ShapeRotateY(TopoDS_Shape shape, double angleY)
	{
		// 将形状平移到原点
		gp_Trsf moveToOrigin;
		gp_Pnt originPnt(0, 0, 0);
		moveToOrigin.SetTranslation(originPnt.XYZ());

		// 旋转
		gp_Trsf rotateY;
		rotateY.SetRotation(gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(0, 1, 0)), angleY * M_PI / 180.0);
		TopoDS_Shape rotatedShape = BRepBuilderAPI_Transform(shape, moveToOrigin * rotateY).Shape();

		return rotatedShape;
	}

	TopoDS_Shape RobotCore::ShapeRotateZ(TopoDS_Shape shape, double angleZ)
	{
		// 将形状平移到原点
		gp_Trsf moveToOrigin;
		gp_Pnt originPnt(0, 0, 0);
		moveToOrigin.SetTranslation(originPnt.XYZ());

		// 旋转
		gp_Trsf rotateZ;
		rotateZ.SetRotation(gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1)), angleZ * M_PI / 180.0);
		TopoDS_Shape rotatedShape = BRepBuilderAPI_Transform(shape, moveToOrigin * rotateZ).Shape();

		return rotatedShape;
	}

	TopoDS_Shape RobotCore::ShapeMove(TopoDS_Shape shape, gp_Pnt movePoint)
	{
		// 平移
		gp_Trsf moveToPoint;
		moveToPoint.SetTranslation(movePoint.XYZ());
		TopoDS_Shape movedShape = BRepBuilderAPI_Transform(shape, moveToPoint).Shape();

		return movedShape;
	}

	Eigen::Vector3d RobotCore::rotateAroundZAxis(const Eigen::Vector3d& point, const Eigen::Vector3d& pivot, double theta)
	{
		// 将点移动到原点
		Eigen::Vector3d translatedPoint = point - pivot;

		// 构造绕Z轴旋转的矩阵
		Eigen::Matrix3d rotationMatrix;
		rotationMatrix << cos(theta), -sin(theta), 0,
			sin(theta), cos(theta), 0,
			0, 0, 1;

		// 应用旋转矩阵
		Eigen::Vector3d rotatedTranslatedPoint = rotationMatrix * translatedPoint;

		// 将点移回原来的位置
		Eigen::Vector3d rotatedPoint = rotatedTranslatedPoint + pivot;

		return rotatedPoint;
	}

	Eigen::Vector3d RobotCore::rotateAroundYAxis(const Eigen::Vector3d& point, double theta)
	{
		// 构造绕Y轴旋转的矩阵
		Eigen::Matrix3d rotationMatrix;
		rotationMatrix << 
			cos(theta), 0, sin(theta),
			0, 1, 0,
			-sin(theta), 0, cos(theta);

		// 应用旋转矩阵
		return rotationMatrix * point;
	}

	Eigen::Vector3d RobotCore::rotateAroundXAxis(const Eigen::Vector3d& point, double theta)
	{
		// 构造绕X轴旋转的矩阵
		Eigen::Matrix3d rotationMatrix;
		rotationMatrix << 
			1, 0, 0,
			0, cos(theta), -sin(theta),
			0, sin(theta), cos(theta);

		// 应用旋转矩阵
		return rotationMatrix * point;
	}

	ADDROBOTDATA RobotCore::ShapesRotated(ADDROBOTDATA addRobotData, double angle)
	{
		QVector<TopoDS_Shape> vector = addRobotData.shapes;
		QVector<Handle(AIS_Shape)> vectortemp;
		QVector<Handle(AIS_Shape)> ShapesBox;
		// 确保data.myAisShapes与vector同步
		//assert(data.myAisShapes.size() == vector.size());

		for (int i = 0; i < vector.size(); ++i)
		{
			if (!addRobotData.myAisShapes.isEmpty())
			{
				myContext->Remove(addRobotData.myAisShapes[i], Standard_True); // 移除旧的形状
			}

			// 保存原始位置，以便稍后恢复
			gp_Pnt originalPosition = addRobotData.assemblyPoint;

			// 将形状平移到原点
			gp_Trsf moveToOrigin;
			moveToOrigin.SetTranslation(gp_Pnt(0.0, 0.0, 0.0).XYZ());
			TopoDS_Shape atOrigin = BRepBuilderAPI_Transform(vector[i], moveToOrigin).Shape();

			// 角度偏移（现在是在局部坐标系中）
			gp_Trsf localRotation;
			localRotation.SetRotation(addRobotData.axisX, addRobotData.angleX / 180.0 * M_PI); // X轴旋转
			TopoDS_Shape rotatedX = BRepBuilderAPI_Transform(atOrigin, localRotation).Shape();

			localRotation.SetRotation(addRobotData.axisY, addRobotData.angleY / 180.0 * M_PI); // Y轴旋转
			TopoDS_Shape rotatedXY = BRepBuilderAPI_Transform(rotatedX, localRotation).Shape();

			localRotation.SetRotation(addRobotData.axisZ, addRobotData.angleZ / 180.0 * M_PI); // Z轴旋转
			TopoDS_Shape rotatedXYZ = BRepBuilderAPI_Transform(rotatedXY, localRotation).Shape();

			// 将形状移回原始位置
			gp_Trsf moveBack;
			moveBack.SetTranslation(originalPosition.XYZ());
			TopoDS_Shape transformedComponent = BRepBuilderAPI_Transform(rotatedXYZ, moveBack).Shape();

			// 位移
			gp_Trsf finalTransform;
			finalTransform.SetTranslation(gp_Vec(addRobotData.assemblyPoint.XYZ()));
			transformedComponent = BRepBuilderAPI_Transform(transformedComponent, finalTransform).Shape();



			aisShape = new AIS_Shape(transformedComponent);
			aisShape->SetColor(addRobotData.color);
			myContext->Display(aisShape, Standard_True);
			vectortemp.push_back(aisShape);

			// 计算形状的边界框
			Bnd_Box box;
			BRepBndLib::Add(transformedComponent, box);
			ShapesBox.push_back(aisShape);


		}
		addRobotData.myAisShapes = vectortemp;
		addRobotData.ShapesBox = ShapesBox;
		return addRobotData;
	}
} // namespace TCP_ROBOT