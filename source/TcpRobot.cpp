
#include "../include/TcpRobot.h"

namespace TCP_ROBOT
{
	TcpRobotCommunication::TcpRobotCommunication(QObject* parent)
	{

		// 创建 标准帧
		RobotFrame * ps = createRobotFrame("PS");
		RobotFrame * go = createRobotFrame("GO");
		RobotFrame * vc = createRobotFrame("VC");
		RobotFrame * ft = createRobotFrame("FT");
		RobotFrame * ig = createRobotFrame("IG");
		RobotFrame * gi = createRobotFrame("GI");

		go->setSendFront(true);

		QMap<QString, TCPXVIEWBASE_NAMESPACE::StandFrame*> robotFrames;
		robotFrames.insert("PS", ps);
		robotFrames.insert("GO", go);
		robotFrames.insert("VC", vc);
		robotFrames.insert("FT", ft);
		robotFrames.insert("IG", ig);
		robotFrames.insert("GI", gi);

		// 注册完成
		setStandFrame(robotFrames);


	}
	TcpRobotCommunication::~TcpRobotCommunication()
	{
	}
	RobotFrame* TcpRobotCommunication::createRobotFrame(QString robotName)
	{
		QString frameName = robotName.append(",");
		RobotFrame *robotFrame = new RobotFrame(this);
		robotFrame->setFrameName(robotName);
		robotFrame->setFrameHeader(frameName);
		robotFrame->setFrameFooter(",J ");
		robotFrame->setFrameData("0,0,0,0,0,0,0,0");


		robotFrame->setReciveStandFrameHearder(frameName);
		robotFrame->setReciveStandFrameFooter(",J ");
		robotFrame->setReciveStandFrameOther("");

		robotFrame->setIsPolling(false);

		return robotFrame;
	}
	void TcpRobotCommunication::sendValue(const QString& FrameName, const QString& command)
	{
		RobotFrame* robotFrame = qobject_cast<RobotFrame*>(getStandFrame(FrameName));
		if (robotFrame)
		{
			robotFrame->setFrameData(command);
			robotFrame->sendStandValue();
		}
	}
	bool TcpRobotCommunication::parseFrame(const QByteArray& byte)
	{
		qDebug() << "TcpRobotCommunication::parseFrame";
		QString reciveData = byte.data();
		reciveData = reciveData.trimmed();

		if (reciveData.contains("ST,MOVING"))
		{
			setCurrentFrameIsReceived(false);
			emit signalRobotCommandFeedback(ST_MOVING);
		}
		if (reciveData.contains("ST,READY"))
		{
			setCurrentFrameIsReceived(true);
			emit signalRobotCommandFeedback(ST_READY);
		}
		if (reciveData.contains("ST,VCING"))
		{
			setCurrentFrameIsReceived(false);
			emit signalRobotCommandFeedback(ST_VCING);

		}
		if (reciveData.contains("ST,GI"))
		{
			setCurrentFrameIsReceived(false);
		}
		if (reciveData.contains("GI,OVER"))
		{
			setCurrentFrameIsReceived(true);
		}
		if (reciveData.contains("ST,IG"))
		{
			setCurrentFrameIsReceived(false);
		}
		if (reciveData.contains("IG,OVER"))
		{
			setCurrentFrameIsReceived(true);
		}
		if (reciveData.contains("VC,OVER"))
		{
			setCurrentFrameIsReceived(true);
			emit signalRobotCommandFeedback(VC_OVER);
		}
		if (reciveData.contains("GO,OVER"))
		{
			setCurrentFrameIsReceived(true);
			emit signalRobotCommandFeedback(GO_OVER);
		}
		if (reciveData.contains("FT,OVER"))
		{
			setCurrentFrameIsReceived(true);
			emit signalRobotCommandFeedback(FT_OVER);
		}
		if (reciveData.contains("ER"))
		{
			QStringList dataList = reciveData.split(",");
			if (dataList.constLast() == "10")
			{
				QMessageBox* messageBox = new QMessageBox(QMessageBox::Warning, __StandQString("错误"), __StandQString("视觉拍照失败!"));
				messageBox->exec();
				//QMessageBox::warning(nullptr, "错误", "电机驱动失败!");
			}
			if (dataList.constLast() == "20")
			{
				QMessageBox* messageBox = new QMessageBox(QMessageBox::Warning, __StandQString("错误"), __StandQString("导轨移动失败!"));
				messageBox->exec();
			}
			if (dataList.constLast() == "30")
			{
				QMessageBox* messageBox = new QMessageBox(QMessageBox::Warning, __StandQString("错误"), __StandQString("转盘运行错误!"));
				messageBox->exec();
			}
			if (dataList.constLast() == "40")
			{
				QMessageBox* messageBox = new QMessageBox(QMessageBox::Warning, __StandQString("错误"), __StandQString("位置不可达!"));
				messageBox->exec();
			}
			setCurrentFrameIsReceived(true);
		}

		return true;
	}
	QWidget* TcpRobotCommunication::commandFeedbackUI(QWidget* parent)
	{
		CWidgetHLay *hLay = new CWidgetHLay(parent);
		hLay->setStyleSheet("QLabel{color: #0000FF;}");
		QString midStr = __StandQString(">>");

		QLabel *labelHeader = new QLabel(parent);
		labelHeader->setText(__StandQString("准备完成 "));

		QLabel *labelSend = new QLabel(parent);
		labelSend->setText(__StandQString("发送 ").append(midStr));

		QLabel *labelMove = new QLabel(parent);
		labelMove->setText(__StandQString("移动 ").append(midStr));

		QLabel* labelMoveEnd = new QLabel(parent);
		labelMoveEnd->setText(__StandQString("待命中... ").append(midStr));

		hLay->addWidget(labelHeader);
		hLay->addWidget(labelMoveEnd);
		hLay->addWidget(labelSend);
		hLay->addWidget(labelMove);
		

		// 全部置灰
		labelHeader->setEnabled(false);
		labelSend->setEnabled(false);
		labelMove->setEnabled(false);
		labelMoveEnd->setEnabled(false);

		connect(this, &TcpRobotCommunication::signalSendSuccessValue, [=](QByteArray value) {
			QString strValue = value.data();
			qDebug() << "TcpRobotCommunication::signalSendSuccessValue " << strValue;
			if (strValue.contains("GO"))
			{
				labelSend->setText(__StandQString("位置信息发送... ").append(midStr));
				labelSend->setEnabled(true);
			}
			else if (strValue.contains("VC"))
			{
				labelSend->setText(__StandQString("视觉控制发送... ").append(midStr));
				labelSend->setEnabled(true);
			}
			else if (strValue.contains("FT"))
			{
				labelSend->setText(__StandQString("微调发送... ").append(midStr));
				labelSend->setEnabled(true);
			}
			else
			{
				labelSend->setText(__StandQString("发送  ").append(midStr));
				labelSend->setEnabled(false);
			}
			
			});
		connect(this, &TcpRobotCommunication::signalRobotCommandFeedback, [=](RobotState command) {
			if (command == ST_MOVING)
			{
				labelMove->setText(__StandQString("移动中 ").append(midStr));
				labelMove->setEnabled(true);
			}
			else if (command == ST_READY)
			{
				labelMove->setText(__StandQString("准备完成 ").append(midStr));
				labelMove->setEnabled(false);
				labelMoveEnd->setEnabled(true);
			}
			else if (command == ST_VCING)
			{
				labelMove->setText(__StandQString("视觉控制中 ").append(midStr));
				labelMove->setEnabled(true);
			}
			else if (command == VC_OVER)
			{
				labelMove->setText(__StandQString("视觉控制结束 ").append(midStr));
				labelMove->setEnabled(false);
				labelMoveEnd->setEnabled(true);
			}
			else if (command == GO_OVER)
			{
				labelMove->setText(__StandQString("位置信息发送完成 ").append(midStr));
				labelMove->setEnabled(false);
				labelMoveEnd->setEnabled(true);
			}
			else if (command == FT_OVER)
			{
				labelMove->setText(__StandQString("微调完成 ").append(midStr));
				labelMove->setEnabled(false);
				labelMoveEnd->setEnabled(true);
			}
			else
			{
				labelMove->setText(__StandQString("移动 ").append(midStr));
				labelMove->setEnabled(false);
			}
			});

		return hLay;
	}
	RobotFrame::RobotFrame(QObject* parent)
	{
	}




	RobotFrame::~RobotFrame()
	{
	}
	void RobotFrame::parseFrame(const QByteArray& byte)
	{
		
		QString reciveData = byte.data();
		reciveData = reciveData.trimmed();
		emit signalAllReceived(reciveData);
		if (reciveData.contains(getReciveStandFrameHearder()))
		{
			
			QStringList dataList = reciveData.split(",");
			if (dataList.size() == 10)
			{
				QStringList valueList = dataList.mid(1, 8);
				QString value = valueList.join(",");
				setFrameData(value);
				emit signalReciveValue(value);
				qDebug() << getReciveStandFrameHearder() << " Send Value " << value;
			}
			if (dataList.contains("OVER"))
			{
				emit signalReciveStatus(true);
			}
		}
	}
	void RobotFrame::sendStandValue()
	{

		emit signalSendValue(getFrameHeader() + getFrameData() + getFrameFooter());
	}
}