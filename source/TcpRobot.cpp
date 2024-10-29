
#include "../include/TcpRobot.h"

namespace TCP_ROBOT
{
	TcpRobotCommunication::TcpRobotCommunication(QObject* parent)
	{

		// ´´½¨ ±ê×¼Ö¡
		RobotFrame * ps = createRobotFrame("PS");
		RobotFrame * go = createRobotFrame("GO");
		RobotFrame * vc = createRobotFrame("VC");
		RobotFrame * ft = createRobotFrame("FT");
		RobotFrame * ig = createRobotFrame("IG");
		RobotFrame * gi = createRobotFrame("GI");

		QMap<QString, TCPXVIEWBASE_NAMESPACE::StandFrame*> robotFrames;
		robotFrames.insert("PS", ps);
		robotFrames.insert("GO", go);
		robotFrames.insert("VC", vc);
		robotFrames.insert("FT", ft);
		robotFrames.insert("IG", ig);
		robotFrames.insert("GI", gi);

		// ×¢²áÍê³É
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
		}
		if (reciveData.contains("ST,READY"))
		{
			setCurrentFrameIsReceived(true);
		}
		if (reciveData.contains("ST,VCING"))
		{
			setCurrentFrameIsReceived(false);
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
		}
		if (reciveData.contains("GO,OVER"))
		{
			setCurrentFrameIsReceived(true);
		}
		if (reciveData.contains("FT,OVER"))
		{
			setCurrentFrameIsReceived(true);
		}
		if (reciveData.contains("ER"))
		{
			QStringList dataList = reciveData.split(",");
			if (dataList.constLast() == "10")
			{
				QMessageBox* messageBox = new QMessageBox(QMessageBox::Warning, __StandQString("´íÎó"), __StandQString("ÊÓ¾õÅÄÕÕÊ§°Ü!"));
				messageBox->exec();
				//QMessageBox::warning(nullptr, "´íÎó", "µç»úÇý¶¯Ê§°Ü!");
			}
			if (dataList.constLast() == "20")
			{
				QMessageBox* messageBox = new QMessageBox(QMessageBox::Warning, __StandQString("´íÎó"), __StandQString("µ¼¹ìÒÆ¶¯Ê§°Ü!"));
				messageBox->exec();
			}
			if (dataList.constLast() == "30")
			{
				QMessageBox* messageBox = new QMessageBox(QMessageBox::Warning, __StandQString("´íÎó"), __StandQString("×ªÅÌÔËÐÐ´íÎó!"));
				messageBox->exec();
			}
			if (dataList.constLast() == "40")
			{
				QMessageBox* messageBox = new QMessageBox(QMessageBox::Warning, __StandQString("´íÎó"), __StandQString("Î»ÖÃ²»¿É´ï!"));
				messageBox->exec();
			}
			setCurrentFrameIsReceived(true);
		}

		return true;
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
		if (reciveData.contains(getReciveStandFrameHearder()))
		{
			QStringList dataList = reciveData.split(",");
			if (dataList.size() == 10)
			{
				QStringList valueList = dataList.mid(1, 8);
				QString value = valueList.join(",");
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