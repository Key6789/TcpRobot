#pragma once
#ifndef SDHROBOT_H
#define SDHROBOT_H
#include "TcpRobotManger_global.h"
#define SDHROBOT TCP_ROBOT::SDHRobot
namespace TCP_ROBOT
{
	struct  DHParameters {
		double a; // Link length
		double alpha; // Twist angle
		double d; // Link offset
		double theta; // Joint angle
	};

	// 定义关节角度类型
	using JointAngle = double;

	// 定义连杆长度类型
	using LinkLength = double;

	// 定义旋转矩阵类型
	using RotationMatrix = QVector<QVector<double>>;

	class TCPROBOT_EXPORT SDHRobot :public QWidget
	{
		Q_OBJECT
	public:
		SDHRobot(QWidget* parent = 0);
		~SDHRobot() {}
		void setDHParameters(QVector<QVector<double>> dhParameters);
		void setDHParametersJointAngle(QVector<double> jointAngles);

		QVector<double> getCurrentExtractPosition(int size = 6);
		QVector<double> getCurrentExtractRotation(int size = 6);

	private:
		// 角度转弧度
		double angleToRadians(double angleDegrees);

		// 构建单个关节的变换矩阵
		QVector<QVector<double>> buildTransformMatrix(double theta, double alpha, double a, double d);

		// 构建所有关节的变换矩阵
		QVector<QVector<QVector<double>>> buildAllTransformMatrices();
		QVector<QVector<QVector<double>>> buildCurrentTransformMatrices(int size = 6);
		// 用于矩阵乘法
		QVector<QVector<double>> multiplyMatrices(const QVector<QVector<double>>& A, const QVector<QVector<double>>& B);

		//	计算末端执行器的总变换矩阵
		QVector<QVector<double>> calculateEndEffectorTransformMatrix();
		QVector<QVector<double>> getCurrentCalculateEndEffectorTransformMatrix(int size = 6);

		// 从总变换矩阵中提取位置坐标
		QVector<double> extractPosition(const QVector<QVector<double>>& transformMatrix);

		// 从总变换矩阵中提取旋转矩阵
		QVector<QVector<double>> extractRotation(const QVector<QVector<double>>& transformMatrix);

		// 从旋转矩阵中提取欧拉角
		QVector<double> extractEulerAngles(const QVector<QVector<double>>& rotationMatrix);

		// 求解第一个关节的角度
		JointAngle solveFirstJointAngle(double targetX, double targetY);
		JointAngle solveSecondJointAngle(LinkLength link1, LinkLength link2, double targetX, double targetY, double targetZ, JointAngle firstJointAngle);
		JointAngle solveThirdJointAngle(LinkLength link1, LinkLength link2, double targetX, double targetY, double targetZ, JointAngle firstJointAngle, JointAngle secondJointAngle);
		JointAngle solveFourthJointAngle(RotationMatrix rotationMatrix, JointAngle firstJointAngle, JointAngle secondJointAngle, JointAngle thirdJointAngle);
		JointAngle solveFifthJointAngle(RotationMatrix rotationMatrix, JointAngle firstJointAngle, JointAngle secondJointAngle, JointAngle thirdJointAngle, JointAngle fourthJointAngle);
		JointAngle solveSixthJointAngle(RotationMatrix rotationMatrix, JointAngle firstJointAngle, JointAngle secondJointAngle, JointAngle thirdJointAngle, JointAngle fourthJointAngle, JointAngle fifthJointAngle);
		QVector<JointAngle> solveInverseKinematics(double targetX, double targetY, double targetZ, RotationMatrix targetRotationMatrix);

		void test();

	private:
		QVector<DHParameters> m_dhParameters; // DH参数

	};
}
#endif // SDHRobot_H