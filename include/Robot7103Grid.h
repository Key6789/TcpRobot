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

		QWidget * showTeachingWidget(QWidget* parent = nullptr);
		void setTcpCommunication(TcpRobotCommunication* tcpRobotCom);
		TcpRobotCommunication* getTcpCommunication() {return m_tcpRobotCom;};
	public: // 参数初始化
		// 初始化原始参数
		void initOriginalParams();

		// 初始化参数表格
		void initParamTableWidget();
	private slots:
		void slotLocationBtnClicked(int saftIndex = -1, int workIndex = -1, int trackIndex = -1);
		void slotcalibrationBtnClicked(int saftIndex = -1, int workIndex = -1, int trackIndex = -1);
		void slotfineTuningBtnClicked(int saftIndex = -1, int workIndex = -1, int trackIndex = -1);
		void slotAddGrid(QStringList rowValues);

	private:
		void initUI();
		void initTable();
		void initGirdItems();
		void initConnections();

		int getRowIndex();

		void updataComboBox();

		
		// 保存参数到文件
		void saveParamsToFile();
		QString getCurrentItemText();

		// 
		void addWorkpieceJson(int row, const QStringList& rowValues,int saftIndex = -1,int workIndex = -1,int trackIndex = -1);
		void addPushButtonClicked(int row, int saftIndex = -1, int workIndex = -1, int trackIndex = -1);


	private:
		MoveStruct m_moveStruct = MoveStruct();

		// 校准窗口
		QDialog* m_calibrationDialog = nullptr;
		QDialog* m_fineTuningDialog = nullptr;

		TcpRobotCommunication* m_tcpRobotCom = nullptr;

		QMap<QString, QString> m_vcMap = QMap<QString, QString>();
	};

	class RobotTeachingWidget : public QWidget
	{
		Q_OBJECT
	public:
		RobotTeachingWidget(QString Name, QWidget* parent = nullptr)
		{
			this->setContentsMargins(0, 0, 0, 0);
			
			QHBoxLayout* saftLayout = new QHBoxLayout(this);
			saftLayout->setContentsMargins(0, 0, 0, 0);
			saftLayout->setMargin(0);
			saftLayout->setSpacing(0);
			m_saftPointList = new ListWidgetBtn(this);
			m_saftPointList->setLabelText(__StandQString("%1").arg(Name).append(__StandQString("列表")));

			m_saftPointDetailLayout = new QVBoxLayout(this);
			m_saftPointDetailLayout->setContentsMargins(0, 0, 0, 0);
			m_saftPointDetailLayout->setSpacing(0);
			m_saftPointDetailLayout->setMargin(0);
			m_saftName = new LabelLineString(this);
			m_saftName->setLabelText(__StandQString("%1").arg(Name).append(__StandQString("名称：")));

			m_saftIndex = new LabelDoubleSpinbox(this);
			m_saftIndex->setLabelText(__StandQString("%1").arg(Name).append(__StandQString("序号：")));

			m_saftPosition = new LabelLineEditBtn(this);
			m_saftPosition->setLabelText(__StandQString("%1").arg(Name).append(__StandQString("坐标：")));
			m_saftPosition->setBtnText(__StandQString("更新坐标"));

			m_saftPointDetailLayout->addWidget(m_saftName);
			m_saftPointDetailLayout->addWidget(m_saftIndex);
			m_saftPointDetailLayout->addWidget(m_saftPosition);
			m_saftPointDetailLayout->addStretch(1);
			saftLayout->addWidget(m_saftPointList);
			saftLayout->addLayout(m_saftPointDetailLayout);
		}
		~RobotTeachingWidget() {}

		void setTcpCommunication(TcpRobotCommunication* tcpRobotCom) { m_tcpRobotCom = tcpRobotCom; }
		void addWidget(RobotTeachingWidget* widget) { m_saftPointDetailLayout->addWidget(widget); m_saftPointDetailLayout->addStretch(1); }
	private:
		ListWidgetBtn* m_saftPointList = nullptr;
		LabelLineString* m_saftName = nullptr;
		LabelDoubleSpinbox* m_saftIndex = nullptr;
		LabelLineEditBtn* m_saftPosition = nullptr;
		TcpRobotCommunication* m_tcpRobotCom = nullptr;
		QVBoxLayout* m_saftPointDetailLayout = nullptr;
	};

	class RobotoDemonstrator :public QWidget
	{
		Q_OBJECT
	public:
		RobotoDemonstrator(QWidget* parent = nullptr);
		~RobotoDemonstrator();

		void setTcpCommunication(TcpRobotCommunication* tcpRobotCom);

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
		void loadTreeWidget(QTreeWidget * treeWidget);

		QString getCurrentPosition();

		void onSafePointPosition();

		void onSafePointOk();
		void onWorkpieceOk();
		void onTrackOk();
		void onSafePointCancel();
		void onWorkpieceCancel();
		void onTrackCancel();


		void updataWidgetComboBox();

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

		// 点位存储
		MoveStruct m_moveStruct = MoveStruct();
		TcpRobotCommunication * m_tcpRobotCom = nullptr;

		SaftPointStruct m_curSaftPoint = SaftPointStruct();
		WorkpieceStruct m_curWorkpiece = WorkpieceStruct();
		TrajectoryStruct m_curTrack = TrajectoryStruct();

		treeItemStruct m_treeItemStruct = treeItemStruct();
		
		QMap<int, QTreeWidgetItem*> m_treeSaftItemMap = QMap<int, QTreeWidgetItem*>();
		
	};
}


#endif // !ROBOT7103GRID_H_
