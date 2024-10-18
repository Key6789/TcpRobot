#ifndef T3TCPROBOT_H_
#define T3TCPROBOT_H_

#include "TcpXViewBase.h"
#include "TcpRobotManger_global.h"

namespace TCPXVIEWBASE_NAMESPACE
{
	class RobotFrame;
	// 通讯衍生类 TcpRobot
	class TCPROBOT_EXPORT TcpRobotCommunication : public TcpXViewBase
	{
		Q_OBJECT
	public:
		TcpRobotCommunication(QObject* parent = nullptr);
		virtual ~TcpRobotCommunication();

		RobotFrame* createRobotFrame(QString robotName);

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
