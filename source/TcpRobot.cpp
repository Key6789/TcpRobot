
#include "../include/TcpRobot.h"

namespace TCP_ROBOT
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

		go->setSendFront(true);

		QMap<QString, TCPXVIEWBASE_NAMESPACE::StandFrame*> robotFrames;
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
	QWidget* TcpRobotCommunication::commandFeedbackUI(QWidget* parent)
	{
		CWidgetHLay *hLay = new CWidgetHLay(parent);
		hLay->setStyleSheet("QLabel{color: #0000FF;}");
		QString midStr = __StandQString(">>");

		QLabel *labelHeader = new QLabel(parent);
		labelHeader->setText(__StandQString("׼����� "));

		QLabel *labelSend = new QLabel(parent);
		labelSend->setText(__StandQString("���� ").append(midStr));

		QLabel *labelMove = new QLabel(parent);
		labelMove->setText(__StandQString("�ƶ� ").append(midStr));

		QLabel* labelMoveEnd = new QLabel(parent);
		labelMoveEnd->setText(__StandQString("������... ").append(midStr));

		hLay->addWidget(labelHeader);
		hLay->addWidget(labelMoveEnd);
		hLay->addWidget(labelSend);
		hLay->addWidget(labelMove);
		

		// ȫ���û�
		labelHeader->setEnabled(false);
		labelSend->setEnabled(false);
		labelMove->setEnabled(false);
		labelMoveEnd->setEnabled(false);

		connect(this, &TcpRobotCommunication::signalSendSuccessValue, [=](QByteArray value) {
			QString strValue = value.data();
			qDebug() << "TcpRobotCommunication::signalSendSuccessValue " << strValue;
			if (strValue.contains("GO"))
			{
				labelSend->setText(__StandQString("λ����Ϣ����... ").append(midStr));
				labelSend->setEnabled(true);
			}
			else if (strValue.contains("VC"))
			{
				labelSend->setText(__StandQString("�Ӿ����Ʒ���... ").append(midStr));
				labelSend->setEnabled(true);
			}
			else if (strValue.contains("FT"))
			{
				labelSend->setText(__StandQString("΢������... ").append(midStr));
				labelSend->setEnabled(true);
			}
			else
			{
				labelSend->setText(__StandQString("����  ").append(midStr));
				labelSend->setEnabled(false);
			}
			
			});
		connect(this, &TcpRobotCommunication::signalRobotCommandFeedback, [=](RobotState command) {
			if (command == ST_MOVING)
			{
				labelMove->setText(__StandQString("�ƶ��� ").append(midStr));
				labelMove->setEnabled(true);
			}
			else if (command == ST_READY)
			{
				labelMove->setText(__StandQString("׼����� ").append(midStr));
				labelMove->setEnabled(false);
				labelMoveEnd->setEnabled(true);
			}
			else if (command == ST_VCING)
			{
				labelMove->setText(__StandQString("�Ӿ������� ").append(midStr));
				labelMove->setEnabled(true);
			}
			else if (command == VC_OVER)
			{
				labelMove->setText(__StandQString("�Ӿ����ƽ��� ").append(midStr));
				labelMove->setEnabled(false);
				labelMoveEnd->setEnabled(true);
			}
			else if (command == GO_OVER)
			{
				labelMove->setText(__StandQString("λ����Ϣ������� ").append(midStr));
				labelMove->setEnabled(false);
				labelMoveEnd->setEnabled(true);
			}
			else if (command == FT_OVER)
			{
				labelMove->setText(__StandQString("΢����� ").append(midStr));
				labelMove->setEnabled(false);
				labelMoveEnd->setEnabled(true);
			}
			else
			{
				labelMove->setText(__StandQString("�ƶ� ").append(midStr));
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