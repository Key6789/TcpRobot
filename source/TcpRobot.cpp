
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

		//go->setSendFront(true);
		vc->setIsResend(false);
		ft->setIsResend(false);
		go->setIsResend(false);
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
		LOG_FUNCTION_LINE_INFO("TcpRobotCommunication::parseFrame");
		LOG_FUNCTION_LINE_INFO(reciveData.toStdString());
		if (reciveData.contains("ST,MOVING"))
		{
			LOG_FUNCTION_LINE_INFO("ST,MOVING");
			setCurrentFrameIsReceived(false);
			emit signalRobotCommandFeedback(ST_MOVING);
		}
		if (reciveData.contains("ST,READY"))
		{
			LOG_FUNCTION_LINE_INFO("ST,READY");
			setCurrentFrameIsReceived(true);
			setSendStatus(true);
			emit signalRobotCommandFeedback(ST_READY);
		}
		if (reciveData.contains("ST,VCING"))
		{
			LOG_FUNCTION_LINE_INFO("ST,VCING");
			setCurrentFrameIsReceived(false);
			emit signalRobotCommandFeedback(ST_VCING);

		}
		if (reciveData.contains("ST,GI"))
		{
			setCurrentFrameIsReceived(false);
		}
		if (reciveData.contains("GI,OVER"))
		{
			setCurrentFrameIsReceived(false);
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
			setCurrentFrameIsReceived(false);
			emit signalRobotCommandFeedback(VC_OVER);
		}
		if (reciveData.contains("GO,OVER"))
		{
			setCurrentFrameIsReceived(false);
			emit signalRobotCommandFeedback(GO_OVER);
		}
		if (reciveData.contains("FT,OVER"))
		{
			setCurrentFrameIsReceived(false);
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
				messageBox->show();
			}
			setCurrentFrameIsReceived(true);
		}

		return true;
	}
	QWidget* TcpRobotCommunication::commandFeedbackUI(QWidget* parent)
	{
		CWidgetHLay *hLay = new CWidgetHLay(parent);

		QPushButton* buttonCommand = new QPushButton(hLay);
		buttonCommand->setText(__StandQString("下发指令"));
		hLay->addWidget(buttonCommand);

		QPushButton* buttonReset = new QPushButton(hLay);
		buttonReset->setText(__StandQString("指令状态"));
		hLay->addWidget(buttonReset);
		
		connect(this, &TcpRobotCommunication::signalSendSuccessValue, [=](QByteArray value) {
			QString strValue = value.data();
			qDebug() << "TcpRobotCommunication::signalSendSuccessValue " << strValue;
			if (strValue.contains("GO"))
			{
				buttonCommand->setText(__StandQString("位置信息发送"));
			}
			else if (strValue.contains("VC"))
			{
				buttonCommand->setText(__StandQString("视觉控制发送"));
			}
			else if (strValue.contains("FT"))
			{
				buttonCommand->setText(__StandQString("微调发送"));
			}
			else
			{
				buttonCommand->setText(__StandQString("指令发送"));
			}
			
			});
		connect(this, &TcpRobotCommunication::signalRobotCommandFeedback, [=](RobotState command) {
			if (command == ST_MOVING)
			{
				buttonReset->setText(__StandQString("移动中"));
			}
			else if (command == ST_READY)
			{
				buttonReset->setText(__StandQString("准备就绪"));
			}
			else if (command == ST_VCING)
			{
				buttonCommand->setText(__StandQString("视觉控制发送"));
				buttonReset->setText(__StandQString("视觉控制中"));
			}
			else if (command == VC_OVER)
			{
				buttonCommand->setText(__StandQString("视觉控制发送"));
				buttonReset->setText(__StandQString("视觉控制完成"));
			}
			else if (command == GO_OVER)
			{
				buttonCommand->setText(__StandQString("位置信息发送"));
				buttonReset->setText(__StandQString("位置信息完成"));
			}
			else if (command == FT_OVER)
			{
				buttonCommand->setText(__StandQString("微调发送"));
				buttonReset->setText(__StandQString("微调完成"));
			}
			else
			{
				buttonCommand->setText(__StandQString("指令发送"));
				buttonReset->setText(__StandQString("指令状态"));

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