#include "../include/SDHRobot.h"
namespace TCP_ROBOT
{
    SDHRobot::SDHRobot(QWidget* parent)
    {
        //// DH 参数
        //QVector<QVector<double>> dhParameters = {
        //    {0, 0, 0, 0},  // Joint 1: a, alpha, d, theta
        //    {0, 0, 232, 0},      // Joint 2: a, alpha, d, theta
        //    {330, 90, 419, 0}, // Joint 3: a, alpha, d, theta
        //    {1150, 0, 114, 0},   // Joint 4: a, alpha, d, theta
        //    {113, 90, -113, 0}, // Joint 5: a, alpha, d, theta
        //    {0, 0, 0, 0}        // Joint 6: a, alpha, d, theta
        //};
        //setDHParameters(dhParameters);

        //// 初始姿态
        //QVector<double> jointPositions1 = getCurrentExtractPosition(1);
        //QVector<double> jointAngles1 = getCurrentExtractRotation(1);

        //QVector<double> jointPositions2 = getCurrentExtractPosition(2);
        //QVector<double> jointAngles2 = getCurrentExtractRotation(2);

        //QVector<double> jointPositions3 = getCurrentExtractPosition(3);
        //QVector<double> jointAngles3 = getCurrentExtractRotation(3);

        //QVector<double> jointPositions4 = getCurrentExtractPosition(4);
        //QVector<double> jointAngles4 = getCurrentExtractRotation(4);

        //QVector<double> jointPositions5 = getCurrentExtractPosition(5);
        //QVector<double> jointAngles5 = getCurrentExtractRotation(5);

        //QVector<double> jointPositions6 = getCurrentExtractPosition(6);
        //QVector<double> jointAngles6 = getCurrentExtractRotation(6);

    }

    void SDHRobot::setDHParameters(QVector<QVector<double>> dhParameters)
    {
        m_dhParameters.clear();
        for (const auto& params : dhParameters) {
            double a = params[0];
            double alpha = params[1];
            double d = params[2];
            double theta = params[3]; // 注意：这里使用了默认值 0，实际应用中需要替换为具体的角度值
            DHParameters dhParams;
            dhParams.a = a;
            dhParams.alpha = alpha;
            dhParams.d = d;
            dhParams.theta = theta;
            // 这里可以做一些参数检查，比如 a, alpha, d 必须大于 0，theta 必须在 -180 到 180 度之间等等
            // 这里可以做一些参数转换，比如将角度转换为弧度等等
            m_dhParameters.push_back(dhParams);
        }
    }

    void SDHRobot::setDHParametersJointAngle(QVector<double> jointAngles)
    {
        // 这里可以做一些参数检查，比如 jointAngles 必须包含 6 个元素，且值在 -180 到 180 度之间等等
        // 这里可以做一些参数转换，比如将角度转换为弧度等等
        QVector<DHParameters> dhParamsTemp;
        for (size_t i = 0; i < jointAngles.size(); ++i) {
            DHParameters dhParams = m_dhParameters[i];
            dhParams.theta += jointAngles[i];
            dhParamsTemp.push_back(dhParams);
        }
        m_dhParameters = dhParamsTemp;
    }

    QVector<double> SDHRobot::getCurrentExtractPosition(int size)
    {
        QVector<QVector<double>> currentPosition = getCurrentCalculateEndEffectorTransformMatrix(size);
        QVector<double> endEffectorPosition = extractPosition(currentPosition);

        // 设定目标位置
        double targetX = endEffectorPosition[0];
        double targetY = endEffectorPosition[1];
        double targetZ = endEffectorPosition[2];

        return { targetX, targetY, targetZ };
    }

    QVector<double> SDHRobot::getCurrentExtractRotation(int size)
    {
        QVector<QVector<double>> currentPosition = getCurrentCalculateEndEffectorTransformMatrix(size);
        QVector<QVector<double>> endEffectorRotation = extractRotation(currentPosition);
        QVector<double> endEffectorEulerAngles = extractEulerAngles(endEffectorRotation);
        return endEffectorEulerAngles;
    }

    QVector<QVector<QVector<double>>> SDHRobot::buildCurrentTransformMatrices(int size)
    {
        QVector<QVector<QVector<double>>> allTransformMatrices;
        for (size_t i = 0; i < size; i++)
        {
            DHParameters dhParams = m_dhParameters[i];
            double a = dhParams.a;
            double alpha = dhParams.alpha;
            double d = dhParams.d;
            double theta = dhParams.theta; // 注意：这里使用了默认值 0，实际应用中需要替换为具体的角度值

            QVector<QVector<double>> H = buildTransformMatrix(theta, alpha, a, d);
            allTransformMatrices.push_back(H);
        }
        return allTransformMatrices;
    }

    double SDHRobot::angleToRadians(double angleDegrees)
    {
        return angleDegrees * M_PI / 180.0;
    }

    QVector<QVector<double>> SDHRobot::buildTransformMatrix(double theta, double alpha, double a, double d)
    {
        theta = angleToRadians(theta);
        alpha = angleToRadians(alpha);
        // 构建变换矩阵
        double H[4][4] = {
            {cos(theta), -sin(theta) * cos(alpha), sin(theta) * sin(alpha), a * cos(theta)},
            {sin(theta), cos(theta) * cos(alpha), -cos(theta) * sin(alpha), a * sin(theta)},
            {0, sin(alpha), cos(alpha), d},
            {0, 0, 0, 1}
        };
        QVector<QVector<double>> matrix(4, QVector<double>(4));
        matrix[0][0] = cos(theta);
        matrix[0][1] = -sin(theta) * cos(alpha);
        matrix[0][2] = sin(theta) * sin(alpha);
        matrix[0][3] = a * cos(theta);
        matrix[1][0] = sin(theta);
        matrix[1][1] = cos(theta) * cos(alpha);
        matrix[1][2] = -cos(theta) * sin(alpha);
        matrix[1][3] = a * sin(theta);
        matrix[2][0] = 0;
        matrix[2][1] = sin(alpha);
        matrix[2][2] = cos(alpha);
        matrix[2][3] = d;
        matrix[3][0] = 0;
        matrix[3][1] = 0;
        matrix[3][2] = 0;
        matrix[3][3] = 1;

        return matrix;
    }

    QVector<QVector<QVector<double>>> SDHRobot::buildAllTransformMatrices()
    {
        QVector<QVector<QVector<double>>> allTransformMatrices;
        for (const auto& params : m_dhParameters) {
            double a = params.a;
            double alpha = params.alpha;
            double d = params.d;
            double theta = params.theta; // 注意：这里使用了默认值 0，实际应用中需要替换为具体的角度值

            QVector<QVector<double>> H = buildTransformMatrix(theta, alpha, a, d);
            allTransformMatrices.push_back(H);
        }

        return allTransformMatrices;
    }

    QVector<QVector<double>> SDHRobot::multiplyMatrices(const QVector<QVector<double>>& A, const QVector<QVector<double>>& B)
    {
        size_t rowsA = A.size();
        size_t colsA = A[0].size();
        size_t colsB = B[0].size();

        QVector<QVector<double>> result(rowsA, QVector<double>(colsB, 0));

        for (size_t i = 0; i < rowsA; ++i) {
            for (size_t j = 0; j < colsB; ++j) {
                for (size_t k = 0; k < colsA; ++k) {
                    result[i][j] += A[i][k] * B[k][j];
                }
            }
        }

        return result;
    }

    QVector<QVector<double>> SDHRobot::calculateEndEffectorTransformMatrix()
    {
        QVector<QVector<QVector<double>>> allTransformMatrices = buildAllTransformMatrices();

        QVector<QVector<double>> totalTransformMatrix = allTransformMatrices[0];

        for (size_t i = 1; i < allTransformMatrices.size(); ++i) {
            totalTransformMatrix = multiplyMatrices(totalTransformMatrix, allTransformMatrices[i]);
        }

        return totalTransformMatrix;
    }

    QVector<QVector<double>> SDHRobot::getCurrentCalculateEndEffectorTransformMatrix(int size)
    {
        QVector<QVector<QVector<double>>> allTransformMatrices = buildCurrentTransformMatrices(size);

        QVector<QVector<double>> totalTransformMatrix = allTransformMatrices[0];

        for (size_t i = 1; i < allTransformMatrices.size(); ++i) {
            totalTransformMatrix = multiplyMatrices(totalTransformMatrix, allTransformMatrices[i]);
        }

        return totalTransformMatrix;
    }

    QVector<double> SDHRobot::extractPosition(const QVector<QVector<double>>& transformMatrix)
    {
        QVector<double> position(3);
        position[0] = transformMatrix[0][3];
        position[1] = transformMatrix[1][3];
        position[2] = transformMatrix[2][3];

        return position;
    }

    QVector<QVector<double>> SDHRobot::extractRotation(const QVector<QVector<double>>& transformMatrix)
    {
        QVector<QVector<double>> rotationMatrix(3, QVector<double>(3));

        for (size_t i = 0; i < 3; ++i) {
            for (size_t j = 0; j < 3; ++j) {
                rotationMatrix[i][j] = transformMatrix[i][j];
            }
        }

        return rotationMatrix;
    }

    QVector<double> SDHRobot::extractEulerAngles(const QVector<QVector<double>>& rotationMatrix)
    {
        QVector<double> rpyAngles(3);

        if (fabs(rotationMatrix[2][0]) >= 1.0 - 1e-6) { // Gimbal lock
            // Gimbal lock: Pitch (θ) is ±90°
            rpyAngles[0] = 0; // Roll
            rpyAngles[1] = asin(-rotationMatrix[2][0]); // Pitch
            rpyAngles[2] = atan2(-rotationMatrix[0][1], rotationMatrix[0][2]); // Yaw
        }
        else {
            rpyAngles[0] = atan2(rotationMatrix[2][1], rotationMatrix[2][2]); // Roll
            rpyAngles[1] = asin(-rotationMatrix[2][0]); // Pitch
            rpyAngles[2] = atan2(rotationMatrix[1][0], rotationMatrix[0][0]); // Yaw
        }

        // 转换为角度
        double Roll = rpyAngles[0] * 180 / M_PI;
        double Pitch = rpyAngles[1] * 180 / M_PI;
        double Yaw = rpyAngles[2] * 180 / M_PI;

        // 调整角度范围
        // 确保角度在 [-180, 180] 区间内
        Roll = fmod(Roll + 180, 360) - 180;
        Pitch = fmod(Pitch + 180, 360) - 180;
        Yaw = fmod(Yaw + 180, 360) - 180;



        return { Roll,Pitch,Yaw };

    }

    JointAngle SDHRobot::solveFirstJointAngle(double targetX, double targetY)
    {
        // 使用 atan2 计算关节角度
        JointAngle firstJointAngle = atan2(targetY, targetX);

        // 考虑关节限制
        // 假设关节角度范围为 -180 到 180 度
        const double minAngle = -M_PI; // -180度
        const double maxAngle = M_PI;  // 180度

        // 确保角度在关节限制范围内
        while (firstJointAngle < minAngle) {
            firstJointAngle += 2 * M_PI;
        }
        while (firstJointAngle > maxAngle) {
            firstJointAngle -= 2 * M_PI;
        }

        return firstJointAngle;
    }

    JointAngle SDHRobot::solveSecondJointAngle(LinkLength link1, LinkLength link2, double targetX, double targetY, double targetZ, JointAngle firstJointAngle)
    {
        double rotatedX = targetX - link1 * cos(firstJointAngle);
        double rotatedY = targetY - link1 * sin(firstJointAngle);

        LinkLength effectiveLength = sqrt(rotatedX * rotatedX + rotatedY * rotatedY + targetZ * targetZ);

        // 防止除以零
        if (fabs(link1 * link2) < 1e-10) {
            // 处理除以零的情况
            return 0.0; // 或者返回一个合理的默认值
        }

        // 确保有效范围内的计算
        double numerator = link1 * link1 + link2 * link2 - effectiveLength * effectiveLength;
        if (numerator < -2 * link1 * link2 || numerator > 2 * link1 * link2) {
            // 处理无效输入的情况
            qDebug() << "Invalid input in solveSecondJointAngle.";
            return 0.0; // 或者返回一个合理的默认值
        }

        return acos(numerator / (2 * link1 * link2));
    }
    // 求解第三个关节的角度
    JointAngle SDHRobot::solveThirdJointAngle(LinkLength link1, LinkLength link2, double targetX, double targetY, double targetZ, JointAngle firstJointAngle, JointAngle secondJointAngle) {
        double rotatedX = targetX - link1 * cos(firstJointAngle);
        double rotatedY = targetY - link1 * sin(firstJointAngle);

        LinkLength effectiveLength = sqrt(rotatedX * rotatedX + rotatedY * rotatedY + targetZ * targetZ);

        // 防止除以零
        if (fabs(link2 * sin(secondJointAngle)) < 1e-10) {
            // 处理除以零的情况
            return 0.0; // 或者返回一个合理的默认值
        }

        // 确保有效范围内的计算
        double denominator = link1 + link2 * cos(secondJointAngle);
        if (fabs(denominator) < 1e-10) {
            // 处理除以零的情况
            qDebug() << "Invalid input in solveThirdJointAngle.";
            return 0.0; // 或者返回一个合理的默认值
        }

        return atan2(targetZ, sqrt(rotatedX * rotatedX + rotatedY * rotatedY)) - atan2(link2 * sin(secondJointAngle), denominator);
    }

    // 求解第四个关节的角度
    JointAngle SDHRobot::solveFourthJointAngle(RotationMatrix rotationMatrix, JointAngle firstJointAngle, JointAngle secondJointAngle, JointAngle thirdJointAngle) {
        // 假设使用 ZYZ 欧拉角
        double s1 = sin(firstJointAngle);
        double c1 = cos(firstJointAngle);
        double s2 = sin(secondJointAngle);
        double c2 = cos(secondJointAngle);
        double s3 = sin(thirdJointAngle);
        double c3 = cos(thirdJointAngle);

        double r11 = rotationMatrix[0][0];
        double r12 = rotationMatrix[0][1];
        double r13 = rotationMatrix[0][2];
        double r21 = rotationMatrix[1][0];
        double r22 = rotationMatrix[1][1];
        double r23 = rotationMatrix[1][2];
        double r31 = rotationMatrix[2][0];
        double r32 = rotationMatrix[2][1];
        double r33 = rotationMatrix[2][2];
        // 添加 判断条件
        return atan2(r31 * c1 + r32 * s1, -r31 * s1 + r32 * c1);
    }

    // 求解第五个关节的角度
    JointAngle SDHRobot::solveFifthJointAngle(RotationMatrix rotationMatrix, JointAngle firstJointAngle, JointAngle secondJointAngle, JointAngle thirdJointAngle, JointAngle fourthJointAngle) {
        // 假设使用 ZYZ 欧拉角
        double s1 = sin(firstJointAngle);
        double c1 = cos(firstJointAngle);
        double s2 = sin(secondJointAngle);
        double c2 = cos(secondJointAngle);
        double s3 = sin(thirdJointAngle);
        double c3 = cos(thirdJointAngle);
        double s4 = sin(fourthJointAngle);
        double c4 = cos(fourthJointAngle);

        double r11 = rotationMatrix[0][0];
        double r12 = rotationMatrix[0][1];
        double r13 = rotationMatrix[0][2];
        double r21 = rotationMatrix[1][0];
        double r22 = rotationMatrix[1][1];
        double r23 = rotationMatrix[1][2];
        double r31 = rotationMatrix[2][0];
        double r32 = rotationMatrix[2][1];
        double r33 = rotationMatrix[2][2];


        return atan2(r21 * c1 + r22 * s1, -r21 * s1 + r22 * c1);
    }

    // 求解第六个关节的角度
    JointAngle SDHRobot::solveSixthJointAngle(RotationMatrix rotationMatrix, JointAngle firstJointAngle, JointAngle secondJointAngle, JointAngle thirdJointAngle, JointAngle fourthJointAngle, JointAngle fifthJointAngle) {
        // 假设使用 ZYZ 欧拉角
        double s1 = sin(firstJointAngle);
        double c1 = cos(firstJointAngle);
        double s2 = sin(secondJointAngle);
        double c2 = cos(secondJointAngle);
        double s3 = sin(thirdJointAngle);
        double c3 = cos(thirdJointAngle);
        double s4 = sin(fourthJointAngle);
        double c4 = cos(fourthJointAngle);
        double s5 = sin(fifthJointAngle);
        double c5 = cos(fifthJointAngle);

        double r11 = rotationMatrix[0][0];
        double r12 = rotationMatrix[0][1];
        double r13 = rotationMatrix[0][2];
        double r21 = rotationMatrix[1][0];
        double r22 = rotationMatrix[1][1];
        double r23 = rotationMatrix[1][2];
        double r31 = rotationMatrix[2][0];
        double r32 = rotationMatrix[2][1];
        double r33 = rotationMatrix[2][2];

        return atan2(r11 * c1 + r12 * s1, -r11 * s1 + r12 * c1);
    }

    QVector<JointAngle> SDHRobot::solveInverseKinematics(double targetX, double targetY, double targetZ, RotationMatrix targetRotationMatrix)
    {
        // 求解第一个关节的角度
        JointAngle firstJointAngle = solveFirstJointAngle(targetX, targetY);

        // 求解第二个关节的角度
        LinkLength link1 = m_dhParameters[0].d; // d
        LinkLength link2 = m_dhParameters[2].a; // a
        JointAngle secondJointAngle = solveSecondJointAngle(link1, link2, targetX, targetY, targetZ, firstJointAngle);

        // 求解第三个关节的角度
        JointAngle thirdJointAngle = solveThirdJointAngle(link1, link2, targetX, targetY, targetZ, firstJointAngle, secondJointAngle);

        // 求解第四个关节的角度
        JointAngle fourthJointAngle = solveFourthJointAngle(targetRotationMatrix, firstJointAngle, secondJointAngle, thirdJointAngle);

        // 求解第五个关节的角度
        JointAngle fifthJointAngle = solveFifthJointAngle(targetRotationMatrix, firstJointAngle, secondJointAngle, thirdJointAngle, fourthJointAngle);

        // 求解第六个关节的角度
        JointAngle sixthJointAngle = solveSixthJointAngle(targetRotationMatrix, firstJointAngle, secondJointAngle, thirdJointAngle, fourthJointAngle, fifthJointAngle);

        return { firstJointAngle, secondJointAngle, thirdJointAngle, fourthJointAngle, fifthJointAngle, sixthJointAngle };
    }

    void SDHRobot::test()
    {
        // DH 参数
        QVector<QVector<double>> dhParameters = {
            {0, M_PI / 2, 50, 0},  // Joint 1: a, alpha, d, theta
            {200, 0, 0, 0},      // Joint 2: a, alpha, d, theta
            {0, -M_PI / 2, 150, 60}, // Joint 3: a, alpha, d, theta
            {0, M_PI / 2, 0, 60},   // Joint 4: a, alpha, d, theta
            {0, -M_PI / 2, 100, 0}, // Joint 5: a, alpha, d, theta
            {0, 0, 50, 0}        // Joint 6: a, alpha, d, theta
        };
        setDHParameters(dhParameters);

        // 正向求解
        QVector<QVector<double>> allTransformMatrices = calculateEndEffectorTransformMatrix();
        QVector<double> endEffectorPosition = extractPosition(allTransformMatrices);

        // 设定目标位置
        double targetX = endEffectorPosition[0];
        double targetY = endEffectorPosition[1];
        double targetZ = endEffectorPosition[2];

        // 设定目标姿态
        RotationMatrix targetRotationMatrix = {
            {1.0, 0.0, 0.0},
            {0.0, 1.0, 0.0},
            {0.0, 0.0, 1.0}
        };

        // 求解逆运动学
        QVector<JointAngle> jointAngles = solveInverseKinematics(targetX, targetY, targetZ, targetRotationMatrix);
        // 设置 DH 参数中的关节角度
        setDHParametersJointAngle(jointAngles);

        // 正向求解验证
        QVector<QVector<double>> newTransformMatrices = calculateEndEffectorTransformMatrix();
        QVector<double> newPosition = extractPosition(newTransformMatrices);
        qDebug() << "Original Position: (" << targetX << ", " << targetY << ", " << targetZ << ")";
        qDebug() << "New Position: (" << newPosition[0] << ", " << newPosition[1] << ", " << newPosition[2] << ")";

    }
}

