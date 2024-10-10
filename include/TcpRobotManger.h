#pragma once
#ifndef TCPROBOTMANGER_H_
#define TCPROBOTMANGER_H_

#include "TcpRobotManger_global.h"

namespace TCP_ROBOT
{
	class TCPROBOT_EXPORT TcpRobotManger: public TCPXVIEWBASE_TYPE
	{
		Q_OBJECT
	public:
		TcpRobotManger(QObject* parent = nullptr);
		virtual ~TcpRobotManger();
	};
}
#endif // !TCPROBOTMANGER_H_