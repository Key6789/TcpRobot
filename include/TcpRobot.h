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
		Q_OBJECT
	public:
		TcpRobotCommunication(QObject* parent = nullptr);
		virtual ~TcpRobotCommunication();

		RobotFrame* createRobotFrame(QString robotName);

		void sendValue(const QString& FrameName,const QString& command);

		bool parseFrame(const QByteArray& byte) override;
	};

	class TCPROBOT_EXPORT RobotFrame : public TCPXVIEWBASE_NAMESPACE::StandFrame
	{
		Q_OBJECT
	public:
		RobotFrame(QObject* parent = nullptr);
		virtual ~RobotFrame();

		void parseFrame(const QByteArray& byte) override;

	
	};
}
#endif // T3TCPROBOT_H_
