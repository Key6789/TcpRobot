#ifndef T3TCPROBOT_H_
#define T3TCPROBOT_H_

#include "TcpXViewBase.h"
#include "TcpRobotManger_global.h"

namespace TCP_ROBOT
{
	class RobotFrame;
	// 通讯衍生类 TcpRobot
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

		// 界面增加指令到位反馈，指令发出、收到、执行完成等
		QWidget* commandFeedbackUI(QWidget* parent = nullptr);

	signals:
		void signalRobotCommandFeedback(RobotState state);
	};

	class TCPROBOT_EXPORT RobotFrame : public TCPXVIEWBASE_NAMESPACE::StandFrame
	{
		Q_OBJECT
	public:
		RobotFrame(QObject* parent = nullptr);
		virtual ~RobotFrame();

		void parseFrame(const QByteArray& byte) override;
		void sendStandValue() override;

	signals:
			void signalAllReceived(const QString& command);


	};
}
#endif // T3TCPROBOT_H_
