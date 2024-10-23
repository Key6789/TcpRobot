#pragma once

#ifndef WIDGET_COMMON_H
#define WIDGET_COMMON_H

#include <QWidget>
#include <QObject>
#include "TcpRobotManger_global.h"
// 建立一个模板类，用于继承QWidget，并添加一些共用的属性和方法
class LabelLineString : public QWidget
{
	Q_OBJECT
public:
	LabelLineString(QWidget* parent = nullptr) : QWidget(parent) {

		QHBoxLayout* layout = new QHBoxLayout(this);
		m_label = new QLabel(this);
		m_lineEdit = new QLineEdit(this);
		layout->addWidget(m_label, 1);
		layout->addWidget(m_lineEdit, 3);
	}

	// 设置标签文本
	void setLabelText(QString text) { m_label->setText(text); }
	// 设置行编辑文本
	void setLineEditText(QString text) { m_lineEdit->setText(text); }
	// 设置只读状态
	void setReadOnly(bool isReadOnly) { m_lineEdit->setReadOnly(isReadOnly); }
	// 设置占位符文本
	void setPlaceholderText(QString text) { m_lineEdit->setPlaceholderText(text); }
	// 设置单位文本
	void setUnitText(QString text) { m_label->setText(m_label->text() + " " + text); }
	// 获取行编辑文本
	QString getLineEditText() { return m_lineEdit->text(); }
	// 获取行编辑控件
	QLineEdit* getLineEdit() { return m_lineEdit; }


private:
	QLabel* m_label = nullptr;
	QLineEdit* m_lineEdit = nullptr;
};

// LabelDoubleSpinbox
class LabelDoubleSpinbox : public QWidget
{
	Q_OBJECT
public:
	LabelDoubleSpinbox(QWidget* parent = nullptr) : QWidget(parent) {
		this->setContentsMargins(0, 0, 0, 0);

		QHBoxLayout* layout = new QHBoxLayout(this);
		layout->setContentsMargins(0, 0, 0, 0);
		layout->setMargin(0);
		layout->setSpacing(0);
		m_label = new QLabel(this);
		m_spinBox = new QDoubleSpinBox(this);
		m_spinBox->setMaximum(INT_MAX);
		m_spinBox->setMinimum(INT_MIN);
		m_spinBox->setValue(0);
		m_spinBox->setAlignment(Qt::AlignLeft);
		layout->addWidget(m_label, 1);
		layout->addWidget(m_spinBox, 3);
	}

	// 设置标签文本
	void setLabelText(QString text) { m_label->setText(text); }
	// 设置值
	void setValue(double value) { m_spinBox->setValue(value); }
	// 设置最小值
	void setMinimum(double minimum) { m_spinBox->setMinimum(minimum); }
	// 设置最大值
	void setMaximum(double maximum) { m_spinBox->setMaximum(maximum); }
	// 设置精度
	void setDecimals(int decimals) { m_spinBox->setDecimals(decimals); }
	// 设置单位文本
	void setUnitText(QString text) { m_label->setText(m_label->text() + " " + text); }
	// 获取值
	double getValue() { return m_spinBox->value(); }
	// 获取控件
	QDoubleSpinBox* getSpinBox() { return m_spinBox; }
	// 设置范围
	void setRange(double minimum, double maximum) { m_spinBox->setRange(minimum, maximum); }

private:
	QLabel* m_label = nullptr;
	QDoubleSpinBox* m_spinBox = nullptr;
};

// ListWidgetBtn
class ListWidgetBtn : public QWidget
{
	Q_OBJECT
public:
	ListWidgetBtn(QWidget* parent = nullptr) : QWidget(parent) {
		this->setContentsMargins(0, 0, 0, 0);

		QVBoxLayout* layout = new QVBoxLayout(this);
		layout->setContentsMargins(0, 0, 0, 0);
		layout->setMargin(0);
		layout->setSpacing(0);
		m_label = new QLabel(this);

		m_listWidget = new QListWidget(this);
		m_listWidget->setAlternatingRowColors(true);
		m_listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
		m_listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

		QHBoxLayout* btnLayout = new QHBoxLayout(this);
		btnLayout->setContentsMargins(0, 0, 0, 0);
		btnLayout->setMargin(0);
		btnLayout->setSpacing(0);

		m_btnFirst = new QPushButton(this);
		m_btnSecond = new QPushButton(this);
		m_btnThird = new QPushButton(this);
		btnLayout->addWidget(m_btnFirst);
		btnLayout->addWidget(m_btnSecond);
		btnLayout->addWidget(m_btnThird);

		layout->addWidget(m_label, 0, Qt::AlignTop);
		layout->addWidget(m_listWidget, 1);
		layout->addLayout(btnLayout);
		connect(m_listWidget, &QListWidget::itemDoubleClicked, [=](QListWidgetItem* item) {m_listWidget->clearSelection(); });
	}
	// 清理列表
	void clearList() { m_listWidget->clear(); }
	// 设置标签文本
	void setLabelText(QString text) { m_label->setText(text); }
	// 设置列表项
	void setItems(QStringList items) { m_listWidget->clear(); m_listWidget->addItems(items); }
	// 添加列表项
	void addItem(QString text) { m_listWidget->addItem(text); }
	// 设置选中项
	void setCurrentItem(int index) { m_listWidget->setCurrentRow(index); }
	// 获取选中项索引
	int getCurrentItemIndex() { return m_listWidget->currentRow(); }
	// 获取选中项文本
	QString getCurrentItemText() { return m_listWidget->currentItem()->text(); }
	// 设置选中项文本
	void setCurrentItemText(QString text) { m_listWidget->currentItem()->setText(text); }
	// 设置选中项背景色
	void setCurrentItemBackground(QColor color) { m_listWidget->currentItem()->setBackground(color); }
	// 设置选中项前景色
	void setCurrentItemForeground(QColor color) { m_listWidget->currentItem()->setForeground(color); }
	// 设置选中项字体
	void setCurrentItemFont(QFont font) { m_listWidget->currentItem()->setFont(font); }
	// 设置选中项图标
	void setCurrentItemIcon(QIcon icon) { m_listWidget->currentItem()->setIcon(icon); }
	// 设置选中项状态
	void setCurrentItemStatus(bool status) { m_listWidget->currentItem()->setHidden(!status); }
	// 设置列表项前景色
	void setItemForeground(int index, QColor color) { m_listWidget->item(index)->setForeground(color); }
	// 设置列表项背景色
	void setItemBackground(int index, QColor color) { m_listWidget->item(index)->setBackground(color); }
	// 设置列表项字体
	void setItemFont(int index, QFont font) { m_listWidget->item(index)->setFont(font); }
	// 设置列表项图标
	void setItemIcon(int index, QIcon icon) { m_listWidget->item(index)->setIcon(icon); }
	// 设置列表项状态
	void setItemStatus(int index, bool status) { m_listWidget->item(index)->setHidden(!status); }
	// 设置列表项文本
	void setItemText(int index, QString text) { m_listWidget->item(index)->setText(text); }
	// 获取列表项数量
	int count() { return m_listWidget->count(); }
	// 获取列表项文本
	QString itemText(int index) { return m_listWidget->item(index)->text(); }
	// 获取列表项图标
	QIcon itemIcon(int index) { return m_listWidget->item(index)->icon(); }
	// 获取列表项状态
	bool itemStatus(int index) { return!m_listWidget->item(index)->isHidden(); }
	// 获取选中项
	QListWidgetItem* currentItem() { return m_listWidget->currentItem(); }
	// 获取所有项
	QList<QListWidgetItem*> items() { return m_listWidget->findItems("", Qt::MatchContains); }

	// 设置选择模式
	void setSelectionMode(QAbstractItemView::SelectionMode mode) { m_listWidget->setSelectionMode(mode); }
	// 设置编辑触发器
	void setEditTriggers(QAbstractItemView::EditTrigger trigger) { m_listWidget->setEditTriggers(trigger); }
	// 设置焦点策略
	void setFocusPolicy(Qt::FocusPolicy policy) { m_listWidget->setFocusPolicy(policy); }

	// 设置按钮文本
	void setBtnText(int index, QString text) {
		switch (index) {
		case 0:
			m_btnFirst->setText(text);
			break;
		case 1:
			m_btnSecond->setText(text);
			break;
		case 2:
			m_btnThird->setText(text);
			break;
		default:
			break;
		}
	}
	// 设置按钮状态
	void setBtnStatus(int index, bool status) {
		switch (index) {
		case 0:
			m_btnFirst->setEnabled(status);
			break;
		case 1:
			m_btnSecond->setEnabled(status);
			break;
		case 2:
			m_btnThird->setEnabled(status);
			break;
		default:
			break;
		}
	}
	// 设置按钮点击信号
	void setBtnClicked(int index, const std::function<void()>& func) {
		switch (index) {
		case 0:
			connect(m_btnFirst, &QPushButton::clicked, func);
			break;
		case 1:
			connect(m_btnSecond, &QPushButton::clicked, func);
			break;
		case 2:
			connect(m_btnThird, &QPushButton::clicked, func);
			break;
		default:
			break;
		}
	}
	// 按钮显示
	void showBtn(int index) {
		switch (index) {
		case 0:
			m_btnFirst->show();
			break;
		case 1:
			m_btnSecond->show();
			break;
		case 2:
			m_btnThird->show();
			break;
		default:
			break;
		}
	}
	// 按钮隐藏
	void hideBtn(int index) {
		switch (index) {
		case 0:
			m_btnFirst->hide();
			break;
		case 1:
			m_btnSecond->hide();
			break;
		case 2:
			m_btnThird->hide();
			break;
		default:
			break;
		}
	}

private slots:
	void onContextMenu(const QPoint& pos) { emit contextMenuRequested(pos); }

signals:
	void contextMenuRequested(const QPoint& pos);

private:
	QListWidget* m_listWidget = nullptr;
	QLabel* m_label = nullptr;
	QPushButton* m_btnFirst = nullptr;
	QPushButton* m_btnSecond = nullptr;
	QPushButton* m_btnThird = nullptr;
};

// 标签、编辑框、按钮组合控件
class LabelLineEditBtn : public QWidget
{
	Q_OBJECT
public:
	LabelLineEditBtn(QWidget* parent = nullptr) : QWidget(parent) {
		this->setContentsMargins(0, 0, 0, 0);

		QHBoxLayout* layout = new QHBoxLayout(this);
		layout->setContentsMargins(0, 0, 0, 0);
		layout->setMargin(0);
		layout->setSpacing(0);
		m_label = new QLabel(this);
		m_lineEdit = new QLineEdit(this);
		m_btn = new QPushButton(this);
		layout->addWidget(m_label, 1);
		layout->addWidget(m_lineEdit, 3);
		layout->addWidget(m_btn, 1);
	}

	// 设置标签文本
	void setLabelText(QString text) { m_label->setText(text); }
	// 设置行编辑文本
	void setLineEditText(QString text) { m_lineEdit->setText(text); }
	// 设置按钮文本
	void setBtnText(QString text) { m_btn->setText(text); }
	// 设置只读状态
	void setReadOnly(bool isReadOnly) { m_lineEdit->setReadOnly(isReadOnly); }
	// 设置占位符文本
	void setPlaceholderText(QString text) { m_lineEdit->setPlaceholderText(text); }
	// 设置按钮点击信号
	void setBtnClicked(const std::function<void()>& func) { connect(m_btn, &QPushButton::clicked, func); }

private:
	QLabel* m_label = nullptr;
	QLineEdit* m_lineEdit = nullptr;
	QPushButton* m_btn = nullptr;
};
#endif // WIDGET_COMMON_H
