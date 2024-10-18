
#include "../include/TcpRobot.h"

namespace TCPXVIEWBASE_NAMESPACE
{
	TcpRobotCommunication::TcpRobotCommunication(QObject* parent)
	{

		// ���� ��׼֡
		RobotFrame * ps = createRobotFrame("PS");
		RobotFrame * go = createRobotFrame("GO");
		RobotFrame * vc = createRobotFrame("VC");
		RobotFrame * ft = createRobotFrame("FT");
		RobotFrame * ig = createRobotFrame("IG");
		RobotFrame * gi = createRobotFrame("GI");

		QMap<QString, StandFrame*> robotFrames;
		robotFrames.insert("PS", ps);
		robotFrames.insert("GO", go);
		robotFrames.insert("VC", vc);
		robotFrames.insert("FT", ft);
		robotFrames.insert("IG", ig);
		robotFrames.insert("GI", gi);

		// ע�����
		setStandFrame(robotFrames);


	}
	TcpRobotCommunication::~TcpRobotCommunication()
	{
	}
	RobotFrame* TcpRobotCommunication::createRobotFrame(QString robotName)
	{
		RobotFrame *robotFrame = new RobotFrame(this);
		robotFrame->setFrameName(robotName);
		robotFrame->setFrameHeader(robotName.append(","));
		robotFrame->setFrameFooter(",J ");
		robotFrame->setFrameData("0,0,0,0,0,0,0,0");

		robotFrame->setReciveStandFrameHearder(robotName.append(","));
		robotFrame->setReciveStandFrameFooter(",J ");
		robotFrame->setReciveStandFrameOther("");

		robotFrame->setIsPolling(false);

		return robotFrame;
	}
	bool TcpRobotCommunication::parseFrame(const QByteArray& byte)
	{
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
		if (reciveData.contains("ER"))
		{
			QStringList dataList = reciveData.split(",");
			if (dataList.constLast() == "10")
			{
				QMessageBox* messageBox = new QMessageBox(QMessageBox::Warning, __StandQString("����"), __StandQString("�Ӿ�����ʧ��!"));
				messageBox->exec();
				//QMessageBox::warning(nullptr, "����", "�������ʧ��!");
			}
			if (dataList.constLast() == "20")
			{
				QMessageBox* messageBox = new QMessageBox(QMessageBox::Warning, __StandQString("����"), __StandQString("�����ƶ�ʧ��!"));
				messageBox->exec();
			}
			if (dataList.constLast() == "30")
			{
				QMessageBox* messageBox = new QMessageBox(QMessageBox::Warning, __StandQString("����"), __StandQString("ת�����д���!"));
				messageBox->exec();
			}
			if (dataList.constLast() == "40")
			{
				QMessageBox* messageBox = new QMessageBox(QMessageBox::Warning, __StandQString("����"), __StandQString("λ�ò��ɴ�!"));
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
		if (reciveData.startsWith(getReciveStandFrameHearder()))
		{
			QStringList dataList = reciveData.split(",");
			if (dataList.count() == 10)
			{
				QStringList valueList = dataList.mid(1, 8);
				emit signalReciveValue(valueList.join(",").toUtf8());
			}
		}
	}
}