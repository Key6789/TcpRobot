#pragma once

#ifndef ROBOT7103GRID_H_
#define ROBOT7103GRID_H_

#include "TcpRobotManger_global.h"
#include "TcpRobot.h"
#include "WidgetCommon.h"


namespace TCP_ROBOT
{
	class TCPROBOT_EXPORT Robot7103Grid : public QTableWidget
	{
		Q_OBJECT

	public:
		Robot7103Grid(QWidget* parent = 0);
		~Robot7103Grid();

		QWidget* showTeachingWidget(QWidget* parent = nullptr);
		void setTcpCommunication(TcpRobotCommunication* tcpRobotCom);
		TcpRobotCommunication* getTcpCommunication() { return m_tcpRobotCom; };

		void setMoveStruct(MoveStruct moveStruct) { m_moveStruct = moveStruct; };
		MoveStruct getMoveStruct() { return m_moveStruct; };
	signals:
		void signalChangeShapeColor(QString shapeName, QString colorName);
	public: // 参数初始化
		// 初始化原始参数
		void initOriginalParams();

		// 初始化参数表格
		void initParamTableWidget();
	public slots:
		void slotLocationBtnClicked(int saftIndex = -1, int workIndex = -1, int trackIndex = -1);
		void slotcalibrationBtnClicked(int saftIndex = -1, int workIndex = -1, int trackIndex = -1);
		void slotfineTuningBtnClicked(int saftIndex = -1, int workIndex = -1, int trackIndex = -1);
		void slotAddGrid(QStringList rowValues, int saftIndex = -1, int workIndex = -1, int trackIndex = -1);
		void slotSeletedWorkChanged(QString text);
	private:
		void initUI();
		void initTable();
		void initGirdItems();
		void initConnections();

		// 获取未使用的表格行号
		int getRowIndex();

		QString getCurrentItemText();

		// 
		void addWorkpieceJson(int row, const QStringList& rowValues, int saftIndex = -1, int workIndex = -1, int trackIndex = -1);
		void addPushButtonClicked(int row, int saftIndex = -1, int workIndex = -1, int trackIndex = -1);


	private:
		MoveStruct m_moveStruct = MoveStruct();
		QString m_currentWork = "";

		QMap<int,QSet<int>> m_rowMap = QMap<int,QSet<int>>();

		// 校准窗口
		QDialog* m_calibrationDialog = nullptr;
		QDialog* m_fineTuningDialog = nullptr;

		TcpRobotCommunication* m_tcpRobotCom = nullptr;

		QMap<QString, QString> m_vcMap = QMap<QString, QString>();


		// 用于设置当前的安全点、工件、轨迹的索引
		int m_currentSaftIndex = ROBOTUNABLEVALUE;
		int m_currentWorkIndex = ROBOTUNABLEVALUE;
		int m_currentTrackIndex = ROBOTUNABLEVALUE;
	};

	class RobotoDemonstrator :public QWidget
	{
		Q_OBJECT
	public:
		RobotoDemonstrator(QWidget* parent = nullptr);
		~RobotoDemonstrator();

		void setTcpCommunication(TcpRobotCommunication* tcpRobotCom);
	signals:
		void sendMoveCommand(MoveStruct moveStruct);
		void sendWorkAndHole(QStringList values, int saftIndex = -1, int workIndex = -1, int trackIndex = -1);

		public slots:
			void slotSeletedWorkChanged(QString text);
	private:
		// 初始化UI
		void initUI();

		void updataSaftWidget(int index);
		void updateWorkpieceWidget(int saftindex, int workindex);
		void updateTrackWidget(int saftindex, int workindex, int trackindex);

		// 初始化树形控件
		void initTreeWidget();

		// 初始化表格控件
		void initTableWidget();

		// 安全点窗口
		void safeWidgetShow();

		// 工件窗口
		void workpieceWidgetShow();

		// 轨迹窗口
		void trackWidgetShow();

		// 连接信号槽
		void initConnect();

		// 初始化数据
		void initData();

		// 加载树形控件
		void loadTreeWidget(QTreeWidget* treeWidget);

		QString getCurrentPosition();
		void onSafePointPosition();
		void onWorkpiecePosition();
		void onTrackPosition();

		void onSafePointOk();
		void onWorkpieceOk();
		void onTrackOk();
		void onSafePointCancel();
		void onWorkpieceCancel();
		void onTrackCancel();


		void updataWidgetComboBox();

		QString m_currentWork = QString();
		// 安全点
		QLineEdit* m_lineEditSafe = nullptr;
		QLineEdit* m_lineEditSaftIndex = nullptr;
		QLineEdit* m_lineEditSafePosition = nullptr;
		QPushButton* m_btnAddSafe = nullptr;
		QPushButton* m_btnDelSafe = nullptr;
		QPushButton* m_btnSafePosition = nullptr;
		// 工件
		QLineEdit* m_lineEditWorkIndex = nullptr;
		QLineEdit* m_lineEditH = nullptr;
		QLineEdit* m_lineEditP = nullptr;
		QLineEdit* m_lineEditHP = nullptr;
		QPushButton* m_btnAddWorkpiece = nullptr;
		QPushButton* m_btnDelWorkpiece = nullptr;
		QComboBox* m_comBoxMode = nullptr;
		// 焊缝
		QLineEdit* m_lineEditWeldIndex = nullptr;
		QLineEdit* m_lineEditWeld = nullptr;
		QLineEdit* m_lineEditWeldHP = nullptr;
		QPushButton* m_btnAddWeld = nullptr;
		QPushButton* m_btnDelWeld = nullptr;
		// 轨迹
		QLineEdit* m_lineEditTrackPosition = nullptr;
		QLineEdit* m_lineEditTrackName = nullptr;


		QTreeWidget* m_treeWidget = nullptr;
		QTabWidget* m_tableWidget = nullptr;
		QWidget* m_safePointWidget = nullptr;

		LabelComboBox* m_labelComboBox = nullptr;

		// 点位存储
		MoveStruct m_moveStruct = MoveStruct();
		TcpRobotCommunication* m_tcpRobotCom = nullptr;

		SaftPointStruct m_curSaftPoint = SaftPointStruct();
		WorkpieceStruct m_curWorkpiece = WorkpieceStruct();
		TrajectoryStruct m_curTrack = TrajectoryStruct();

		treeItemStruct m_treeItemStruct = treeItemStruct();

		QMap<int, QTreeWidgetItem*> m_treeSaftItemMap = QMap<int, QTreeWidgetItem*>();

	};
}


#endif // !ROBOT7103GRID_H_
