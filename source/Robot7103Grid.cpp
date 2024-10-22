﻿#include "../include/Robot7103Grid.h"

namespace TCP_ROBOT
{
	Robot7103Grid::Robot7103Grid(QWidget* parent) : QTableWidget(parent)
	{
		initOriginalParams();

		initUI();
		initConnections();
		setObjectName("Robot7103Grid");
	}

	Robot7103Grid::~Robot7103Grid()
	{

	}


	void Robot7103Grid::initOriginalParams()
	{
		m_moveStruct.LoadJson(MOVESTRUCTPATH);
	}
	void Robot7103Grid::initParamTableWidget()
	{
		// 全部清理
		clear();
		// 表头
		initOriginalParams();

		initUI();
	}

	QWidget* Robot7103Grid::showTeachingWidget(QWidget* parent)
	{
		return nullptr;
	}

	void Robot7103Grid::setTcpCommunication(TcpRobotCommunication* tcpRobotCom)
	{
		m_tcpRobotCom = tcpRobotCom;
	}



	void Robot7103Grid::initUI()
	{
		// 表头初始化
		initTable();
		initGirdItems();

	}

	void Robot7103Grid::initTable()
	{
		// 共计六列，分别为工艺、焊缝、位置
		setColumnCount(7);
		// 行数不确定，先设置一个大概的数值
		setRowCount(10);
		// 仅选择单个单元格
		setSelectionBehavior(QAbstractItemView::SelectItems);
		//设置行标题隐藏
		verticalHeader()->setHidden(true);
		horizontalHeader()->setHidden(true);
		setAlternatingRowColors(true);
		// 将第一行作为表头设置
		insertRow(0);
		setCellWidget(0, 0, new QLabel(__StandQString("工艺"), this));
		setCellWidget(0, 1, new QLabel(__StandQString("焊缝"), this));
		setCellWidget(0, 2, new QLabel(__StandQString("位置"), this));

		// 接下来的几列合并为一列
		setSpan(0, 3, 1, 3);
		// 设置单元格内容
		// 设置网格可见
		setShowGrid(true);

		setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		// 数据铺满同比例
		horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

	}

	void Robot7103Grid::initGirdItems()
	{
		foreach(int index, m_moveStruct.MoveMap.keys())
		{
			SaftPointStruct saftPoint = m_moveStruct.MoveMap.value(index);
			foreach(int saftIndex, saftPoint.SaftPointMap.keys())
			{
				WorkpieceStruct workPoint = saftPoint.SaftPointMap.value(saftIndex);
				QStringList rowValues;
				rowValues << workPoint.WorkpieceName << workPoint.WorkpieceName << workPoint.PostioName;
				addWorkpieceJson(getRowIndex(), rowValues, index, saftIndex, -1);
				/*foreach(int holeIndex, workPoint.TrajectoryMap.keys())
				{
					TrajectoryStruct trajectory = workPoint.TrajectoryMap.value(holeIndex);
					
				}*/
			}
		}
	}

	void Robot7103Grid::initConnections()
	{
		connect(this, &QTableWidget::itemSelectionChanged, [=]() {selectRow(currentRow()); });
		// 设置选中行 为绿色
		connect(this, &QTableWidget::itemSelectionChanged, [=]() {
			QList<QTableWidgetItem*> items = selectedItems();
			foreach(QTableWidgetItem* item, items)
			{
				item->setBackground(QColor(0, 255, 0));
			}
		});
	}

	int Robot7103Grid::getRowIndex()
	{
		// 遍历所有行，返回没有内容的行号 // 排除第一行
		for (int i = 1; i < rowCount(); ++i)
		{
			if (item(i, 0) == nullptr)
			{
				return i;
			}
		}
		return rowCount();
	}

	void Robot7103Grid::updataComboBox()
	{
	}



	void Robot7103Grid::saveParamsToFile()
	{
	}

	QString Robot7103Grid::getCurrentItemText()
	{
		int row = this->currentRow();

		if (item(row, 0) != nullptr)
		{
			return item(row, 0)->text().append(",").append(item(row, 1)->text()).append(",").append(item(row, 2)->text());
		}
		return QString();
	}

	void Robot7103Grid::addWorkpieceJson(
		int row,
		const QStringList& rowValues,
		int saftIndex,
		int workpieceIndex,
		int trackIndex)
	{

		// 在当前行下方插入一行
		insertRow(row);
		int rowCount = rowValues.size();
		for (int i = 0; i < rowValues.size(); ++i)
		{
			setItem(row, i, new QTableWidgetItem(rowValues[i]));
		}
		setColumnCount(rowCount + 4);
		addPushButtonClicked(row, saftIndex, workpieceIndex, trackIndex);

	}

	void Robot7103Grid::addPushButtonClicked(
		int row,
		int saftIndex,
		int workIndex,
		int trackIndex)
	{
		// 定位按钮
		QPushButton* locationBtn = new QPushButton(this);
		locationBtn->setText(__StandQString("定位"));

		QPushButton* calibrationBtn = new QPushButton(this);
		calibrationBtn->setText(__StandQString("校准"));
		//calibrationBtn->setEnabled(false);

		QPushButton* fineTuningBtn = new QPushButton(this);
		fineTuningBtn->setText(__StandQString("微调"));
		//fineTuningBtn->setEnabled(false);

		QPushButton* confirmBtn = new QPushButton(this);
		confirmBtn->setText(__StandQString("待确认"));

		confirmBtn->setStyleSheet("background-color:red");
		connect(confirmBtn, &QPushButton::clicked, [=]() {
			if (confirmBtn->text() == __StandQString("确认"))
			{
				confirmBtn->setStyleSheet("background-color:red");
				confirmBtn->setText(__StandQString("待确认"));
			}
			else
			{
				confirmBtn->setStyleSheet("background-color:green");
				confirmBtn->setText(__StandQString("确认"));
			}
			});


		// 按钮添加到单元格
		setCellWidget(row, 3, locationBtn);
		setCellWidget(row, 4, calibrationBtn);
		setCellWidget(row, 5, fineTuningBtn);
		setCellWidget(row, 6, confirmBtn);

		connect(locationBtn, &QPushButton::clicked, [=]() {slotLocationBtnClicked(saftIndex, workIndex, trackIndex); });
		connect(calibrationBtn, &QPushButton::clicked, [=]() {slotcalibrationBtnClicked(saftIndex, workIndex, trackIndex); });
		connect(fineTuningBtn, &QPushButton::clicked, [=]() {slotfineTuningBtnClicked(saftIndex, workIndex, trackIndex); });


	}
	void Robot7103Grid::slotLocationBtnClicked(
		int saftIndex,
		int workIndex,
		int trackIndex)
	{
		// 定位按钮点击事件
		// 获取点击的单元格的行号
		int row = currentRow();
		// 设置所在行的其他按钮可用
		QPushButton* calibrationBtn = static_cast<QPushButton*>(cellWidget(row, 4));
		QPushButton* fineTuningBtn = static_cast<QPushButton*>(cellWidget(row, 5));

		ISNULLPOINTER(calibrationBtn);
		ISNULLPOINTER(fineTuningBtn);

		calibrationBtn->setEnabled(true);
		fineTuningBtn->setEnabled(true);

		QString workpiece = item(row, 0)->text();
		QString hole = item(row, 1)->text();
		QString position = item(row, 2)->text();
		QString command = workpiece + "," + hole + "," + position;

	}

	void Robot7103Grid::slotcalibrationBtnClicked(
		int saftIndex,
		int workIndex,
		int trackIndex)
	{
		if (m_calibrationDialog != nullptr) {
			m_calibrationDialog->close();
			m_calibrationDialog = nullptr;
		}
		m_calibrationDialog = new QDialog(this);
		m_calibrationDialog->setWindowTitle(__StandQString("校准工件"));
		m_calibrationDialog->setModal(true);

		m_calibrationDialog->move(QCursor::pos());

		QWidget* widget = new QWidget(m_calibrationDialog);
		QHBoxLayout* formLayoutvc = new QHBoxLayout(widget);

		QVBoxLayout* layout = new QVBoxLayout(widget);
		formLayoutvc->addLayout(layout, 1);

		QLabel* vcSetLabel = new QLabel(__StandQString("VC参数设置"), widget);
		layout->addWidget(vcSetLabel);

		LabelDoubleSpinbox* obLabel = new LabelDoubleSpinbox(widget);
		obLabel->setLabelText(__StandQString("OB(mm)："));

		LabelDoubleSpinbox* ocLabel = new LabelDoubleSpinbox(widget);
		ocLabel->setLabelText(__StandQString("OC(mm)："));

		LabelDoubleSpinbox* A0Label = new LabelDoubleSpinbox(widget);
		A0Label->setLabelText(__StandQString("α0(°)："));
		A0Label->setRange(-180, 180);

		LabelDoubleSpinbox* A1Label = new LabelDoubleSpinbox(widget);
		A1Label->setLabelText(__StandQString("α1(°)："));
		A1Label->setRange(-180, 180);

		LabelDoubleSpinbox* B1Label = new LabelDoubleSpinbox(widget);
		B1Label->setLabelText(__StandQString("β1(°)："));
		B1Label->setRange(-180, 180);

		LabelDoubleSpinbox* A2Label = new LabelDoubleSpinbox(widget);
		A2Label->setLabelText(__StandQString("α2(°)："));
		A2Label->setRange(-180, 180);

		LabelDoubleSpinbox* B2Label = new LabelDoubleSpinbox(widget);
		B2Label->setLabelText(__StandQString("β2(°)："));
		B2Label->setRange(-180, 180);

		LabelDoubleSpinbox* L1Label = new LabelDoubleSpinbox(widget);
		L1Label->setLabelText(__StandQString("L1(mm)："));

		layout->addWidget(obLabel);
		layout->addWidget(ocLabel);
		layout->addWidget(A0Label);
		layout->addWidget(A1Label);
		layout->addWidget(B1Label);
		layout->addWidget(A2Label);
		layout->addWidget(B2Label);
		layout->addWidget(L1Label);

		QPushButton* buttonApply = new QPushButton(__StandQString("应用"), widget);
		layout->addWidget(buttonApply);


		ListWidgetBtn* vcList = new ListWidgetBtn(widget);
		vcList->setLabelText(__StandQString("拍摄列表"));
		vcList->setSelectionMode(QAbstractItemView::SingleSelection);
		vcList->setEditTriggers(QAbstractItemView::NoEditTriggers);
		vcList->setFocusPolicy(Qt::NoFocus);
		vcList->setBtnText(0, __StandQString("移动"));
		vcList->setBtnText(1, __StandQString("储存"));
		vcList->setBtnText(2, __StandQString("取消"));

		formLayoutvc->addWidget(vcList, 1);


		// 模块刷新
		WorkpieceStruct workPoint = WorkpieceStruct();
		if (saftIndex != -1 && workIndex != -1)
		{
			workPoint = m_moveStruct.MoveMap.value(saftIndex).SaftPointMap.value(workIndex);
		}

		obLabel->setValue(workPoint.VcOB.toDouble());
		ocLabel->setValue(workPoint.VcOC.toDouble());
		A0Label->setValue(workPoint.VcA0.toDouble());
		A1Label->setValue(workPoint.VcA1.toDouble());
		B1Label->setValue(workPoint.VcB1.toDouble());
		A2Label->setValue(workPoint.VcA2.toDouble());
		B2Label->setValue(workPoint.VcB2.toDouble());
		L1Label->setValue(workPoint.VcLen.toDouble());


		// 刷新 VC 拍摄位置
		vcList->clearList();
		if (!workPoint.getVCList().isEmpty())
		{
			vcList->setItems(workPoint.getVCList());
			vcList->setBtnStatus(1, true);
		}
		connect(buttonApply, &QPushButton::clicked, [=]()
			{
				QStringList applyList;
				applyList <<
					QString::number(obLabel->getValue()) <<
					QString::number(ocLabel->getValue()) <<
					QString::number(A0Label->getValue()) <<
					QString::number(A1Label->getValue()) <<
					QString::number(B1Label->getValue()) <<
					QString::number(A2Label->getValue()) <<
					QString::number(B2Label->getValue()) <<
					QString::number(L1Label->getValue());

				QString VcValue = applyList.join(",");
				if (getTcpCommunication())
				{
					getTcpCommunication()->sendValue("VC", VcValue);
				}
				m_vcMap.clear();
				vcList->clearList();
			});

		vcList->setBtnClicked(1, [=]() {

			WorkpieceStruct workPoint = WorkpieceStruct();
			if (saftIndex != -1 && workIndex != -1)
			{
				workPoint = m_moveStruct.MoveMap.value(saftIndex).SaftPointMap.value(workIndex);
			}
			workPoint.VcOB = QString::number(obLabel->getValue());
			workPoint.VcOC = QString::number(ocLabel->getValue());
			workPoint.VcA0 = QString::number(A0Label->getValue());
			workPoint.VcA1 = QString::number(A1Label->getValue());
			workPoint.VcB1 = QString::number(B1Label->getValue());
			workPoint.VcA2 = QString::number(A2Label->getValue());
			workPoint.VcB2 = QString::number(B2Label->getValue());
			workPoint.VcLen = QString::number(L1Label->getValue());
			QMap<int, WorkpieceStruct> SaftPointMap = m_moveStruct.MoveMap.value(saftIndex).SaftPointMap;
			SaftPointMap.insert(workIndex, workPoint);
			SaftPointStruct saftPoint = m_moveStruct.MoveMap.value(saftIndex);
			saftPoint.SaftPointMap = SaftPointMap;
			m_moveStruct.MoveMap.insert(saftIndex, saftPoint);
			m_moveStruct.SaveJson(MOVESTRUCTPATH);
			});
		vcList->setBtnClicked(2, [=]() {m_calibrationDialog->close(); });
		if (m_tcpRobotCom)
		{
			RobotFrame* robotFrame = qobject_cast<RobotFrame*>(m_tcpRobotCom->getStandFrame("VC"));
			connect(robotFrame, &RobotFrame::signalReciveValue, [=](QString vcName) {
				if (vcName.contains("VC0")) {
					vcList->addItem("α"); m_vcMap.insert("α", vcName);
				}
				else if (vcName.contains("VC1")) {
					vcList->addItem("β1"); m_vcMap.insert("β1", vcName);
				}
				else if (vcName.contains("VC2")) {
					vcList->addItem("β2"); m_vcMap.insert("β2", vcName);
				}
				else {}
				});
			connect(robotFrame, &RobotFrame::signalReciveStatus, [=](bool status) {
				if (status)
				{
					vcList->setItems(m_vcMap.keys());
					QMessageBox::information(this, __StandQString("提示"), __StandQString("VC参数设置成功！"));
				}
				});

			// 移动按钮
			vcList->setBtnClicked(0, [=]() {
				QString currentVc = vcList->getCurrentItemText();
				if (currentVc.isEmpty()) {
					QMessageBox::warning(this, __StandQString("提示"), __StandQString("请选择VC拍摄位置！"));
					return;
				}
				QString VcValue = m_vcMap[vcList->getCurrentItemText()];
				if (getTcpCommunication())
				{
					getTcpCommunication()->sendValue("GO", VcValue);
				}
				});

		}

		QVBoxLayout* dialogLay = new QVBoxLayout(m_calibrationDialog);
		dialogLay->addWidget(widget);
		m_calibrationDialog->resize(450, 300);
		m_calibrationDialog->exec();
	}

	void Robot7103Grid::slotfineTuningBtnClicked(
		int saftIndex,
		int workIndex,
		int trackIndex)
	{
		if (m_fineTuningDialog)
		{
			m_fineTuningDialog->close();
			m_fineTuningDialog = nullptr;
		}
		m_fineTuningDialog = new QDialog(this);
		m_fineTuningDialog->setWindowTitle(__StandQString("微调工件"));
		m_fineTuningDialog->setModal(true);

		m_fineTuningDialog->move(QCursor::pos());

		QWidget* widget = new QWidget(m_fineTuningDialog);
		QFormLayout* layout = new QFormLayout(widget);
		layout->setContentsMargins(0, 0, 0, 4);
		layout->setSpacing(16);

		QLabel* FineTuningLabel = new QLabel(__StandQString("微调方向(mm)："), widget);
		FineTuningLabel->setAlignment(Qt::AlignLeft);

		QComboBox* tuningComboBox = new QComboBox(widget);
		tuningComboBox->addItem("X");
		tuningComboBox->addItem("Y");
		tuningComboBox->addItem("Z");
		tuningComboBox->addItem("A");
		tuningComboBox->addItem("B");
		tuningComboBox->addItem("C");

		layout->addRow(FineTuningLabel, tuningComboBox);

		QLabel* numberSetLabel = new QLabel(__StandQString("数值设置"), widget);
		QComboBox* unitComboBox = new QComboBox(widget);
		unitComboBox->addItem("1");
		unitComboBox->addItem("5");
		unitComboBox->addItem("10");
		layout->addRow(numberSetLabel, unitComboBox);
		QHBoxLayout* unitLayout = new QHBoxLayout(widget);
		unitLayout->setContentsMargins(0, 0, 0, 0);
		unitLayout->setSpacing(10);
		QPushButton* addBtn = new QPushButton("+", widget);
		QPushButton* subBtn = new QPushButton("-", widget);
		unitLayout->addWidget(addBtn);
		unitLayout->addWidget(subBtn);
		layout->addRow(unitLayout);

		connect(addBtn, &QPushButton::clicked, [=]() {
			QString direction = tuningComboBox->currentText();
			QString move = unitComboBox->currentText();
			if (getTcpCommunication())
			{
				QStringList applyList;
				applyList << __StandQString("+") << direction << move << "0" << "0" << "0" << "0" << "0";
				getTcpCommunication()->sendValue(__StandQString("FT"), applyList.join(","));
			}
			});
		connect(subBtn, &QPushButton::clicked, [=]() {
			QString direction = tuningComboBox->currentText();
			QString move = unitComboBox->currentText();
			if (getTcpCommunication())
			{
				QStringList applyList;
				applyList << __StandQString("-") << direction << move << "0" << "0" << "0" << "0" << "0";
				getTcpCommunication()->sendValue(__StandQString("FT"), applyList.join(","));
			}
			});

		QVBoxLayout* dialogLay = new QVBoxLayout(m_fineTuningDialog);
		dialogLay->addWidget(widget);
		m_fineTuningDialog->exec();
	}

	void Robot7103Grid::slotAddGrid(QStringList rowValues)
	{
		if (rowValues.size() < 3) {
			return;
		}
		blockSignals(true);
		// 添加到有值的最后一行
		int rowCount = this->rowCount();
		for (int i = 0; i < rowCount; ++i) {
			if (!item(i, 0) && !cellWidget(i, 0)) {
				int row = i;
				// 在当前行下方插入一行
				insertRow(row);
				// 设置新行的默认值
				setItem(row, 0, new QTableWidgetItem(rowValues.at(0)));
				setItem(row, 1, new QTableWidgetItem(rowValues.at(1)));
				setItem(row, 2, new QTableWidgetItem(rowValues.at(2)));
				addPushButtonClicked(row);

				break;
			}
		}
		updataComboBox();
		blockSignals(false);
	}
	RobotoDemonstrator::RobotoDemonstrator(QWidget* parent)
	{
		initData();
		initUI();
		initConnect();

	}
	RobotoDemonstrator::~RobotoDemonstrator()
	{
	}
	void RobotoDemonstrator::setTcpCommunication(TcpRobotCommunication* tcpRobotCom)
	{
		m_tcpRobotCom = tcpRobotCom;
	}
	void RobotoDemonstrator::initUI()
	{
		// 界面划分为两部分，比例为3:5.
		/*QVBoxLayout* mainLayout = new QVBoxLayout(this);
		RobotTeachingWidget *saft = new RobotTeachingWidget(__StandQString("安全点"), this);
		RobotTeachingWidget *workpiece = new RobotTeachingWidget(__StandQString("焊缝"), this);
		RobotTeachingWidget* workpiecePosition = new RobotTeachingWidget(__StandQString("焊缝-位置"), this);
		RobotTeachingWidget *track = new RobotTeachingWidget(__StandQString("轨迹"), this);
		mainLayout->addWidget(saft);
		saft->addWidget(workpiece);
		workpiece->addWidget(workpiecePosition);
		workpiecePosition->addWidget(track);*/
		QHBoxLayout* mainLayout = new QHBoxLayout(this);

		initTreeWidget();
		initTableWidget();

		m_tableWidget->setTabEnabled(1, false);
		m_tableWidget->setTabEnabled(2, false);

		connect(m_treeWidget, &QTreeWidget::itemClicked, [=](QTreeWidgetItem* item, int column) {
			m_treeItemStruct.saftIndex = -1;
			m_treeItemStruct.workIndex = -1;
			m_treeItemStruct.trackIndex = -1;

			m_tableWidget->setTabEnabled(1, true);
			m_tableWidget->setTabEnabled(2, true);
			//点击之后需要数据更新
			m_treeItemStruct.getSaftIndexFromItem(item);
			updataSaftWidget(-1);
			updateWorkpieceWidget(-1, -1);
			updateTrackWidget(-1, -1, -1);
			if (m_treeItemStruct.saftIndex != -1)
			{
				updataSaftWidget(m_treeItemStruct.saftIndex);
				m_curSaftPoint = m_moveStruct.MoveMap[m_treeItemStruct.saftIndex];
				m_btnDelSafe->setEnabled(true);
				m_tableWidget->setCurrentIndex(0);
				if (m_treeItemStruct.workIndex != -1)
				{
					updateWorkpieceWidget(m_treeItemStruct.saftIndex, m_treeItemStruct.workIndex);
					m_tableWidget->setTabEnabled(1, true);
					// 切换至轨迹标签页
					m_tableWidget->setCurrentIndex(1);
					m_curWorkpiece = m_curSaftPoint.SaftPointMap[m_treeItemStruct.workIndex];
					if (m_treeItemStruct.trackIndex != -1)
					{
						updateTrackWidget(m_treeItemStruct.saftIndex, m_treeItemStruct.workIndex, m_treeItemStruct.trackIndex);
						m_tableWidget->setTabEnabled(2, true);
						m_tableWidget->setCurrentIndex(2);
						m_curTrack = m_curWorkpiece.TrajectoryMap[m_treeItemStruct.trackIndex];
					}
				}
				//m_tableWidget->setTabEnabled(1, true);
			}
			});

		mainLayout->addWidget(m_safePointWidget);
		mainLayout->addWidget(m_tableWidget);
		
		setLayout(mainLayout);

	}
	void RobotoDemonstrator::updataSaftWidget(int index)
	{
		if (index == -1)
		{
			m_lineEditSafe->setText("");
			m_lineEditSaftIndex->setText(QString::number(-1));
			m_lineEditSafePosition->setText("");
		}
		else
		{
			// 安全点更新
			SaftPointStruct saftPoint = m_moveStruct.MoveMap[index];
			m_lineEditSafe->setText(saftPoint.SaftPointName);
			m_lineEditSaftIndex->setText(QString::number(saftPoint.SaftPointIndex));
			m_lineEditSafePosition->setText(saftPoint.SaftPointPosition);
		}
		
	}
	void RobotoDemonstrator::updateWorkpieceWidget(int saftindex, int workindex)
	{
		if (saftindex == -1 || workindex == -1)
		{
			m_lineEditWorkIndex->setText("");
			m_lineEditH->setText("");
			m_lineEditP->setText("");
			m_lineEditHP->setText("");
			m_comBoxMode->clear();
			m_comBoxMode->addItem("");
		}
		else
		{
			// 焊缝更新
			WorkpieceStruct workPoint = m_moveStruct.MoveMap[saftindex].SaftPointMap[workindex];
			m_lineEditWorkIndex->setText(QString::number(workPoint.WorkpieceIndex));
			m_lineEditH->setText(workPoint.WorkpieceName);
			m_lineEditP->setText(workPoint.PostioName);
			m_lineEditHP->setText(workPoint.HolePosition);
			m_comBoxMode->addItems(workPoint.HoleModeNames);
			m_comBoxMode->setCurrentText(workPoint.HoleModeName);
		}
		
	}
	void RobotoDemonstrator::updateTrackWidget(int saftindex, int workindex, int trackindex)
	{
		if (saftindex == -1 || workindex == -1 || trackindex == -1)
		{
			m_lineEditWeldIndex->setText("");
			m_lineEditTrackName->setText("");
			m_lineEditTrackPosition->setText("");
		}
		else
		{
			// 轨迹更新
			TrajectoryStruct trackPoint = m_moveStruct.MoveMap[saftindex].SaftPointMap[workindex].TrajectoryMap[trackindex];
			m_lineEditWeldIndex->setText(QString::number(trackPoint.swcIndex));
			m_lineEditTrackName->setText(trackPoint.TrajectoryName);
			m_lineEditTrackPosition->setText(trackPoint.trackPosition);
		}
	}
	void RobotoDemonstrator::initTreeWidget()
	{
		m_safePointWidget = new QWidget(this);
		QFormLayout* formLayoutSafe = new QFormLayout(m_safePointWidget);
		formLayoutSafe->setContentsMargins(0, 0, 0, 0);
		formLayoutSafe->setSpacing(8);

		// 安全点名称 + QLineEdit 可编辑可选择
		QLabel* labelSafe = new QLabel(__StandQString("安全点列表"), m_safePointWidget);
		formLayoutSafe->addRow(labelSafe);
		m_treeWidget = new QTreeWidget(m_safePointWidget);
		m_treeWidget->setHeaderHidden(true);
		formLayoutSafe->addRow(m_treeWidget);



		loadTreeWidget(m_treeWidget);
		m_treeWidget->expandAll();
	}
	void RobotoDemonstrator::initTableWidget()
	{
		// 添加标签页 检测点设定
		m_tableWidget = new QTabWidget(this);
		safeWidgetShow();
		workpieceWidgetShow();
		trackWidgetShow();
	}
	void RobotoDemonstrator::safeWidgetShow()
	{
		// 添加标签页 安全点设定
		QWidget* tabWidgetSafe = new QWidget(this);
		m_tableWidget->addTab(tabWidgetSafe, __StandQString("设置安全点"));
		QVBoxLayout* mainLayoutSafe = new QVBoxLayout(tabWidgetSafe);

		// 安全点名称 + QLineEdit 可编辑可选择
		QHBoxLayout* layoutSafe = new QHBoxLayout(tabWidgetSafe);
		QLabel* labelSafe = new QLabel(__StandQString("安全点名称"), tabWidgetSafe);
		m_lineEditSafe = new QLineEdit(tabWidgetSafe);
		m_lineEditSafe->setText(__StandQString("安全点-%1").arg(m_moveStruct.getMaxSaftPointIndex() + 1));



		layoutSafe->addWidget(labelSafe);
		layoutSafe->addWidget(m_lineEditSafe);
		mainLayoutSafe->addLayout(layoutSafe);

		// 安全点序号
		QHBoxLayout* layoutSafeIndex = new QHBoxLayout(tabWidgetSafe);
		QLabel* labelSafeIndex = new QLabel(__StandQString("安全点序号"), tabWidgetSafe);
		m_lineEditSaftIndex = new QLineEdit(tabWidgetSafe);
		m_lineEditSaftIndex->setText(__StandQString("%1").arg(m_moveStruct.getMaxSaftPointIndex() + 1));

		layoutSafeIndex->addWidget(labelSafeIndex);
		layoutSafeIndex->addWidget(m_lineEditSaftIndex);

		mainLayoutSafe->addLayout(layoutSafeIndex);

		// 安全点位置
		QHBoxLayout* layoutSafePosition = new QHBoxLayout(tabWidgetSafe);
		QLabel* labelSafePosition = new QLabel(__StandQString("安全坐标"), tabWidgetSafe);
		m_lineEditSafePosition = new QLineEdit(tabWidgetSafe);
		m_lineEditSafePosition->setText(__StandQString(""));
		m_btnSafePosition = new QPushButton(__StandQString("更新坐标"), tabWidgetSafe);
		layoutSafePosition->addWidget(labelSafePosition);
		layoutSafePosition->addWidget(m_lineEditSafePosition);
		layoutSafePosition->addWidget(m_btnSafePosition);
		m_lineEditSafePosition->setReadOnly(true);
		mainLayoutSafe->addLayout(layoutSafePosition);

		/*connect(m_lineEditSafe, &QLineEdit::textChanged, [=]() {m_curSaftPoint.SaftPointName = m_lineEditSafe->text(); });
		connect(m_lineEditSaftIndex, &QLineEdit::textChanged, [=]() {m_curSaftPoint.SaftPointIndex = m_lineEditSaftIndex->text().toInt(); });
		connect(m_lineEditSafePosition, &QLineEdit::textChanged, [=]() { m_curSaftPoint.SaftPointPosition = m_lineEditSafePosition->text(); });*/

		
		// 添加、删除 按钮
		QHBoxLayout* layoutSafeButton = new QHBoxLayout(tabWidgetSafe);
		m_btnAddSafe = new QPushButton(__StandQString("确定"), tabWidgetSafe);
		m_btnDelSafe = new QPushButton(__StandQString("删除"), tabWidgetSafe);

		m_btnDelSafe->setEnabled(false);
		layoutSafeButton->addWidget(m_btnAddSafe);
		layoutSafeButton->addWidget(m_btnDelSafe);
		mainLayoutSafe->addLayout(layoutSafeButton);

		mainLayoutSafe->addStretch();

		connect(m_btnAddSafe, &QPushButton::clicked, this, &RobotoDemonstrator::onSafePointOk);
		connect(m_btnDelSafe, &QPushButton::clicked, this, &RobotoDemonstrator::onSafePointCancel);
		connect(m_btnSafePosition, &QPushButton::clicked, this, &RobotoDemonstrator::onSafePointPosition);
	}
	void RobotoDemonstrator::workpieceWidgetShow()
	{

		// 添加标签页 安全点设定
		QWidget* tabWidget = new QWidget(this);
		m_tableWidget->addTab(tabWidget, __StandQString("设置焊缝"));

		QVBoxLayout* mainLayout = new QVBoxLayout(tabWidget);

		// 序号
		QHBoxLayout* layoutIndex = new QHBoxLayout(tabWidget);
		QLabel* labelIndex = new QLabel(__StandQString("焊缝序号"), tabWidget);
		m_lineEditWorkIndex = new QLineEdit(tabWidget);
		m_lineEditWorkIndex->setText("");
		layoutIndex->addWidget(labelIndex);
		layoutIndex->addWidget(m_lineEditWorkIndex);
		mainLayout->addLayout(layoutIndex);

		/*connect(m_lineEditWorkIndex, &QLineEdit::textChanged, [=]() {
			m_curWorkpiece.WorkpieceIndex = m_lineEditWorkIndex->text().toInt();
			m_curSaftPoint.SaftPointMap.insert(m_curWorkpiece.WorkpieceIndex, m_curWorkpiece); });*/


		// 焊缝 + QComBox 可编辑可选择
		QHBoxLayout* layoutH = new QHBoxLayout(tabWidget);
		QLabel* labelH = new QLabel(__StandQString("焊缝名称"), tabWidget);
		m_lineEditH = new QLineEdit(tabWidget);
		m_lineEditH->setText("");

		layoutH->addWidget(labelH);
		layoutH->addWidget(m_lineEditH);
		mainLayout->addLayout(layoutH);

		/*connect(m_lineEditH, &QLineEdit::textChanged, [=]() {
			m_curWorkpiece.HoleModeName = m_lineEditH->text();
			m_curSaftPoint.SaftPointMap.insert(m_curWorkpiece.WorkpieceIndex, m_curWorkpiece); });*/

		// 位置
		QHBoxLayout* layoutP = new QHBoxLayout(tabWidget);
		QLabel* labelP = new QLabel(__StandQString("位置名称"), tabWidget);
		m_lineEditP = new QLineEdit(tabWidget);
		m_lineEditP->setText("");
		layoutP->addWidget(labelP);

		/*connect(m_lineEditP, &QLineEdit::textChanged, [=]() {
			m_curWorkpiece.PostioName = m_lineEditP->text();
			m_curSaftPoint.SaftPointMap.insert(m_curWorkpiece.WorkpieceIndex, m_curWorkpiece); });*/

		layoutP->addWidget(m_lineEditP);
		mainLayout->addLayout(layoutP);

		// 焊缝位置
		QHBoxLayout* layoutHP = new QHBoxLayout(tabWidget);
		QLabel* labelHP = new QLabel(__StandQString("焊缝坐标"), tabWidget);
		QPushButton* btnHP = new QPushButton(__StandQString("更新坐标"), tabWidget);
		m_lineEditHP = new QLineEdit(tabWidget);
		m_lineEditHP->setText("");
		m_lineEditHP->setReadOnly(true);
		layoutHP->addWidget(labelHP);
		layoutHP->addWidget(m_lineEditHP);
		layoutHP->addWidget(btnHP);
		mainLayout->addLayout(layoutHP);
		/*connect(m_btnAddWorkpiece, &QPushButton::clicked, [=]() {
			m_lineEditHP->setText(getCurrentPosition());
			m_curWorkpiece.HolePosition = m_lineEditHP->text();
			m_curSaftPoint.SaftPointMap.insert(m_curWorkpiece.WorkpieceIndex, m_curWorkpiece); });*/

		// 模型对应焊缝
		QHBoxLayout* layoutModel = new QHBoxLayout(tabWidget);
		QLabel* labelModel = new QLabel(__StandQString("模型对应焊缝"), tabWidget);
		m_comBoxMode = new QComboBox(tabWidget);

		/*connect(m_comBoxMode, &QComboBox::currentTextChanged, [=]() {
			m_curWorkpiece.HoleModeName = m_comBoxMode->currentText();
			m_curSaftPoint.SaftPointMap.insert(m_curWorkpiece.WorkpieceIndex, m_curWorkpiece); });*/

		layoutModel->addWidget(labelModel, 1);
		layoutModel->addWidget(m_comBoxMode, 4);
		mainLayout->addLayout(layoutModel);

		// 添加确定/取消按钮
		QHBoxLayout* layoutButton = new QHBoxLayout(tabWidget);
		m_btnAddWorkpiece = new QPushButton(__StandQString("确定"), tabWidget);
		m_btnDelWorkpiece = new QPushButton(__StandQString("删除"), tabWidget);
		layoutButton->addWidget(m_btnAddWorkpiece);
		layoutButton->addWidget(m_btnDelWorkpiece);
		mainLayout->addLayout(layoutButton);

		connect(m_btnAddWorkpiece, &QPushButton::clicked, this, &RobotoDemonstrator::onWorkpieceOk);
		connect(m_btnDelWorkpiece, &QPushButton::clicked, this, &RobotoDemonstrator::onWorkpieceCancel);

		mainLayout->addStretch();

	}
	void RobotoDemonstrator::trackWidgetShow()
	{
		// 添加标签页 工艺路线设定
		QWidget* SafttabWidget = new QWidget(this);
		m_tableWidget->addTab(SafttabWidget, __StandQString("设置轨迹"));

		QVBoxLayout* mainLayout = new QVBoxLayout(SafttabWidget);

		// 序号
		QHBoxLayout* layoutIndex = new QHBoxLayout(SafttabWidget);
		QLabel* labelIndex = new QLabel(__StandQString("轨迹序号"), SafttabWidget);
		m_lineEditWeldIndex = new QLineEdit(SafttabWidget);
		m_lineEditWeldIndex->setText("");
		layoutIndex->addWidget(labelIndex);
		layoutIndex->addWidget(m_lineEditWeldIndex);
		mainLayout->addLayout(layoutIndex);

		/*connect(m_lineEditWeldIndex, &QLineEdit::textChanged, [=]() {
			m_curTrack.swcIndex = m_lineEditWeldIndex->text().toInt();
			m_curWorkpiece.TrajectoryMap.insert(m_curTrack.swcIndex, m_curTrack);
			m_curSaftPoint.SaftPointMap.insert(m_curWorkpiece.WorkpieceIndex, m_curWorkpiece); });*/


		// 位置
		QHBoxLayout* layoutP = new QHBoxLayout(SafttabWidget);
		QLabel* labelP = new QLabel(__StandQString("轨迹名称"), SafttabWidget);
		m_lineEditTrackName = new QLineEdit(SafttabWidget);
		m_lineEditTrackName->setText("");
		layoutP->addWidget(labelP);
		layoutP->addWidget(m_lineEditTrackName);
		mainLayout->addLayout(layoutP);

		// 焊缝位置
		QHBoxLayout* layoutHP = new QHBoxLayout(SafttabWidget);
		QLabel* labelHP = new QLabel(__StandQString("轨迹坐标"), SafttabWidget);
		QPushButton* btnHP = new QPushButton(__StandQString("更新坐标"), SafttabWidget);
		m_lineEditTrackPosition = new QLineEdit(SafttabWidget);
		m_lineEditTrackPosition->setText("");
		m_lineEditTrackPosition->setReadOnly(true);
		layoutHP->addWidget(labelHP);
		layoutHP->addWidget(m_lineEditTrackPosition);
		layoutHP->addWidget(btnHP);

		mainLayout->addLayout(layoutHP);
		/*connect(btnHP, &QPushButton::clicked, [=]() {
			m_lineEditTrackIndex->setText(getCurrentPosition());
			m_curTrack.trackPosition = m_lineEditTrackIndex->text();
			m_curWorkpiece.TrajectoryMap.insert(m_curTrack.swcIndex, m_curTrack);
			m_curSaftPoint.SaftPointMap.insert(m_curWorkpiece.WorkpieceIndex, m_curWorkpiece); });*/


		// 添加确定/取消按钮
		QHBoxLayout* layoutButton = new QHBoxLayout(SafttabWidget);
		m_btnAddWeld = new QPushButton(__StandQString("确定"), SafttabWidget);
		m_btnDelWeld = new QPushButton(__StandQString("删除"), SafttabWidget);
		layoutButton->addWidget(m_btnAddWeld);
		layoutButton->addWidget(m_btnDelWeld);
		mainLayout->addLayout(layoutButton);

		connect(m_btnAddWeld, &QPushButton::clicked, this, &RobotoDemonstrator::onTrackOk);
		connect(m_btnDelWeld, &QPushButton::clicked, this, &RobotoDemonstrator::onTrackCancel);

		mainLayout->addStretch();
	}
	void RobotoDemonstrator::initConnect()
	{

	}
	void RobotoDemonstrator::initData()
	{
		m_moveStruct.LoadJson(MOVESTRUCTPATH);
	}
	void RobotoDemonstrator::loadTreeWidget(QTreeWidget* treeWidget)
	{
		treeWidget->clear();
		m_treeItemStruct = treeItemStruct();
		QVector<saftItemStruct> vecSoft;
		QMap<int, SaftPointStruct> MoveMapTemp = m_moveStruct.MoveMap;
		foreach(int key, MoveMapTemp.keys())
		{
			// 安全点名称
			SaftPointStruct safePoint = MoveMapTemp.value(key);
			QTreeWidgetItem* item = new QTreeWidgetItem(treeWidget);
			item->setText(0, safePoint.SaftPointName);
			item->setData(0, Qt::UserRole, key);
			
			saftItemStruct saftItem;
			saftItem.index = key;
			saftItem.item = item;
			QVector<workitemStruct> vecwork;
			foreach(int workpieceKey, safePoint.SaftPointMap.keys())
			{
				// 工件名称
				WorkpieceStruct workpiece = safePoint.SaftPointMap.value(workpieceKey);
				QTreeWidgetItem* workpieceItem = new QTreeWidgetItem(item);
				workpieceItem->setText(0, workpiece.WorkpieceName);
				workpieceItem->setData(0, Qt::UserRole, workpieceKey);

				workitemStruct workitem;
				workitem.index = workpieceKey;
				workitem.item = workpieceItem;
				QVector< trackItemStruct> vecTrack;
				// 焊缝名称
				foreach(int trackKey, workpiece.TrajectoryMap.keys())
				{
					// 轨迹名称
					TrajectoryStruct track = workpiece.TrajectoryMap.value(trackKey);
					QTreeWidgetItem* trackItem = new QTreeWidgetItem(workpieceItem);
					trackItem->setText(0, track.TrajectoryName);
					trackItem->setData(0, Qt::UserRole, trackKey);

					trackItemStruct trackItemS;
					trackItemS.index = trackKey;
					trackItemS.item = trackItem;
					vecTrack.append(trackItemS);
				}
				workitem.vecTrack = vecTrack;
				vecwork.append(workitem);
				
			}
			saftItem.vecwork = vecwork;
			vecSoft.append(saftItem);
		}
		m_treeItemStruct.vecSoft = vecSoft;
		treeWidget->expandAll();
	}
	QString RobotoDemonstrator::getCurrentPosition()
	{
		ISNULLPOINTERSTRING(m_tcpRobotCom);
		TCPXVIEWBASE_NAMESPACE::StandFrame* standFram = m_tcpRobotCom->getStandFrame("PS");
		ISNULLPOINTERSTRING(standFram);

		return standFram->getFrameData();
	}
	void RobotoDemonstrator::onSafePointPosition()
	{
		m_lineEditSafePosition->setText(getCurrentPosition());
	}
	void RobotoDemonstrator::onSafePointOk()
	{
		SaftPointStruct saftPoint;
		if (m_moveStruct.MoveMap.keys().contains(m_lineEditSaftIndex->text().toInt()))
		{
			saftPoint = m_moveStruct.MoveMap.value(m_lineEditSaftIndex->text().toInt());
		}
		saftPoint.SaftPointName = m_lineEditSafe->text();
		saftPoint.SaftPointIndex = m_lineEditSaftIndex->text().toInt();
		saftPoint.SaftPointPosition = m_lineEditSafePosition->text();
		m_curSaftPoint = saftPoint;
		if (saftPoint.SaftPointIndex != -1)
		{
			m_moveStruct.MoveMap.insert(saftPoint.SaftPointIndex, saftPoint);
			m_moveStruct.SaveJson(MOVESTRUCTPATH);
			loadTreeWidget(m_treeWidget);
		}
		else
		{
			QMessageBox::warning(this, __StandQString("警告"), __StandQString("请先设置安全点序号！"));
		}

		//addSafePoint();
		m_treeWidget->expandAll();
	}
	void RobotoDemonstrator::onWorkpieceOk()
	{
		WorkpieceStruct workpiece;
		if (m_curSaftPoint.SaftPointMap.keys().contains(m_lineEditWorkIndex->text().toInt()))
		{
			workpiece = m_curSaftPoint.SaftPointMap.value(m_lineEditWorkIndex->text().toInt());
		}
		workpiece.WorkpieceName = m_lineEditH->text();
		workpiece.WorkpieceIndex = m_lineEditWorkIndex->text().toInt();
		workpiece.PostioName = m_lineEditP->text();
		workpiece.HolePosition = m_lineEditHP->text();
		workpiece.HoleModeName = m_comBoxMode->currentText();
		m_curWorkpiece = workpiece;

		if (m_curWorkpiece.WorkpieceIndex != -1)
		{
			m_curSaftPoint.SaftPointMap.insert(m_curWorkpiece.WorkpieceIndex, m_curWorkpiece);
			m_moveStruct.MoveMap.insert(m_curSaftPoint.SaftPointIndex, m_curSaftPoint);
			m_moveStruct.SaveJson(MOVESTRUCTPATH);
			loadTreeWidget(m_treeWidget);
		}
		else
		{
			QMessageBox::warning(this, __StandQString("警告"), __StandQString("请先设置工件序号！"));
		}
	}
	void RobotoDemonstrator::onTrackOk()
	{
		TrajectoryStruct track;
		if (m_curWorkpiece.TrajectoryMap.keys().contains(m_lineEditWeldIndex->text().toInt()))
		{
			track = m_curWorkpiece.TrajectoryMap.value(m_lineEditWeldIndex->text().toInt());
		}
		track.TrajectoryName = m_lineEditTrackName->text();
		track.swcIndex = m_lineEditWeldIndex->text().toInt();
		track.trackPosition = m_lineEditTrackPosition->text();
		m_curTrack = track;
		if (m_curTrack.swcIndex != -1)
		{
			m_curWorkpiece.TrajectoryMap.insert(m_curTrack.swcIndex, m_curTrack);
			m_curSaftPoint.SaftPointMap.insert(m_curWorkpiece.WorkpieceIndex, m_curWorkpiece);
			m_moveStruct.MoveMap.insert(m_curSaftPoint.SaftPointIndex, m_curSaftPoint);
			m_moveStruct.SaveJson(MOVESTRUCTPATH);
			loadTreeWidget(m_treeWidget);

		}
		else
		{
			QMessageBox::warning(this, __StandQString("警告"), __StandQString("请先设置轨迹序号！"));
		}
	}
	void RobotoDemonstrator::onSafePointCancel()
	{
		if (m_curSaftPoint.SaftPointIndex != -1)
		{
			m_moveStruct.MoveMap.remove(m_curSaftPoint.SaftPointIndex);
			m_moveStruct.SaveJson(MOVESTRUCTPATH);
			loadTreeWidget(m_treeWidget);
		}
		else
		{
			QMessageBox::warning(this, __StandQString("警告"), __StandQString("请先检查工件序号！"));
		}
	}
	void RobotoDemonstrator::onWorkpieceCancel()
	{
		if (m_curWorkpiece.WorkpieceIndex != -1)
		{
			m_curSaftPoint.SaftPointMap.remove(m_curWorkpiece.WorkpieceIndex);
			m_moveStruct.MoveMap.insert(m_curSaftPoint.SaftPointIndex, m_curSaftPoint);
			m_moveStruct.SaveJson(MOVESTRUCTPATH);
			loadTreeWidget(m_treeWidget);
		}
		else
		{
			QMessageBox::warning(this, __StandQString("警告"), __StandQString("请先检查工件序号！"));
		}
	}
	void RobotoDemonstrator::onTrackCancel()
	{
		if (m_curTrack.swcIndex != -1)
		{
			m_curWorkpiece.TrajectoryMap.remove(m_curTrack.swcIndex);
			m_curSaftPoint.SaftPointMap.insert(m_curWorkpiece.WorkpieceIndex, m_curWorkpiece);
			m_moveStruct.MoveMap.insert(m_curSaftPoint.SaftPointIndex, m_curSaftPoint);
			m_moveStruct.SaveJson(MOVESTRUCTPATH);
			loadTreeWidget(m_treeWidget);
		}
		else
		{
			QMessageBox::warning(this, __StandQString("警告"), __StandQString("请先检查轨迹序号！"));
		}
	}

	void RobotoDemonstrator::updataWidgetComboBox()
	{

	}

} // namespace Robot7103Grid

