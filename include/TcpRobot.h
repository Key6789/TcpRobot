#ifndef T3TCPROBOT_H_
#define T3TCPROBOT_H_

#include "TcpXViewBase.h"
#include "TcpRobotManger_global.h"

namespace TCP_ROBOT
{
	class RobotFrame;
	// ͨѶ������ TcpRobot
	class TCPROBOT_EXPORT TcpRobotCommunication : public TCPXVIEWBASE_NAMESPACE::TcpXViewBase
	{
		enum RobotState {
			ST_MOVING = 0,
			ST_READY,
			ST_VCING,
			VC_OVER,
			GO_OVER,
			FT_OVER
		};
		Q_OBJECT
	public:
		TcpRobotCommunication(QObject* parent = nullptr);
		virtual ~TcpRobotCommunication();

		RobotFrame* createRobotFrame(QString robotName);
		void sendValue(const QString& FrameName, const QString& command);
		bool parseFrame(const QByteArray& byte) override;

		// ��������ָ�λ������ָ������յ���ִ����ɵ�
		QWidget* commandFeedbackUI(QWidget* parent = nullptr);
	signals:
		void signalRobotCommandFeedback(RobotState state);

	private:
		// ָ�����
		QMap<QString,QQueue<QString>> m_commandQueue = QMap<QString,QQueue<QString>>();

		static const QTime m_timeOut;
	};

	class TCPROBOT_EXPORT RobotFrame : public TCPXVIEWBASE_NAMESPACE::StandFrame
	{
		Q_OBJECT
	public:
		RobotFrame(QObject* parent = nullptr);
		virtual ~RobotFrame();

		void parseFrame(const QByteArray& byte) override;
		void sendStandValue() override;

		QString getCurrentPosition() const;

	signals:
		void signalAllReceived(const QString& command);

	private:
		QString m_currentPosition;
	};
}
#endif // T3TCPROBOT_H_
