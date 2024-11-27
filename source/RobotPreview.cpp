#include "../include/RobotPreview.h"

namespace TCP_ROBOT
{
	RobotPreview::RobotPreview(QWidget* parent)
	{
		displayCoordinateAxes();
		myView->FitAll();
	}
	RobotPreview::~RobotPreview()
	{

	}
	
	void RobotPreview::slotUpdataRobotShaps(void)
	{
		displayCoordinateAxes();
		myView->FitAll();
	}

	void RobotPreview::slotReplaceModelByPath(SHAPESTRUCT shapeStruct)
	{

		// 读取文件
		// 转换为机器人数据
		ADDROBOTDATA robotData = createRobotData(shapeStruct);
		RemovePreview(m_mapPreviewData[shapeStruct.ShapeName]);
		m_mapPreviewData.insert(shapeStruct.ShapeName, robotData);

		displaySingalAddRobot(robotData);
	}

	void RobotPreview::slotChangPreviewColor(SHAPESTRUCT shapeStruct)
	{
		// 修改颜色
		QString colorStr = shapeStruct.ShapeColor;
		// 转换颜色格式
		QColor color(colorStr);
		// 转换为Quantity_Color
		Quantity_Color qColor(color.redF(), color.greenF(), color.blueF(), Quantity_TOC_sRGB);
		// 修改颜色
		for (auto shape : m_mapPreviewData[shapeStruct.ShapeName].myAisShapes)
		{
			shape->SetColor(qColor);
		}
	}

	void RobotPreview::slotChangedPreviewScale(SHAPESTRUCT shapeStruct)
	{
		slotReplaceModelByPath(shapeStruct);
	}

	void RobotPreview::slotChangedPreviewRotation(SHAPESTRUCT shapeStruct)
	{
		slotReplaceModelByPath(shapeStruct);
	}

	void RobotPreview::slotChangedPreviewTranslation(SHAPESTRUCT shapeStruct)
	{
		// 修改位置
		slotReplaceModelByPath(shapeStruct);
	}

	void RobotPreview::removePreview(SHAPESTRUCT shapeStruct)
	{
		RemovePreview(m_mapPreviewData[shapeStruct.ShapeName]);
	}

	void RobotPreview::removeAllPreview()
	{
		getContext()->EraseAll(true);
		m_mapPreviewData.clear();
		displayCoordinateAxes();
		myView->FitAll();
	}
	void RobotPreview::RemovePreview(ADDROBOTDATA addRobotData)
	{
		if (m_mapPreviewData.contains(addRobotData.name))
		{
			for (auto shape : m_mapPreviewData[addRobotData.name].myAisShapes)
			{
				getContext()->Erase(shape, Standard_True);
			}
			m_mapPreviewData.remove(addRobotData.name);
		}
	}
}