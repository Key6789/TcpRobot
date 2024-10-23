#pragma once

#ifndef WIDGET_COMMON_H
#define WIDGET_COMMON_H

#include <QWidget>
#include <QObject>
#include "TcpRobotManger_global.h"
// ����һ��ģ���࣬���ڼ̳�QWidget�������һЩ���õ����Ժͷ���
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

	// ���ñ�ǩ�ı�
	void setLabelText(QString text) { m_label->setText(text); }
	// �����б༭�ı�
	void setLineEditText(QString text) { m_lineEdit->setText(text); }
	// ����ֻ��״̬
	void setReadOnly(bool isReadOnly) { m_lineEdit->setReadOnly(isReadOnly); }
	// ����ռλ���ı�
	void setPlaceholderText(QString text) { m_lineEdit->setPlaceholderText(text); }
	// ���õ�λ�ı�
	void setUnitText(QString text) { m_label->setText(m_label->text() + " " + text); }
	// ��ȡ�б༭�ı�
	QString getLineEditText() { return m_lineEdit->text(); }
	// ��ȡ�б༭�ؼ�
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

	// ���ñ�ǩ�ı�
	void setLabelText(QString text) { m_label->setText(text); }
	// ����ֵ
	void setValue(double value) { m_spinBox->setValue(value); }
	// ������Сֵ
	void setMinimum(double minimum) { m_spinBox->setMinimum(minimum); }
	// �������ֵ
	void setMaximum(double maximum) { m_spinBox->setMaximum(maximum); }
	// ���þ���
	void setDecimals(int decimals) { m_spinBox->setDecimals(decimals); }
	// ���õ�λ�ı�
	void setUnitText(QString text) { m_label->setText(m_label->text() + " " + text); }
	// ��ȡֵ
	double getValue() { return m_spinBox->value(); }
	// ��ȡ�ؼ�
	QDoubleSpinBox* getSpinBox() { return m_spinBox; }
	// ���÷�Χ
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
	// �����б�
	void clearList() { m_listWidget->clear(); }
	// ���ñ�ǩ�ı�
	void setLabelText(QString text) { m_label->setText(text); }
	// �����б���
	void setItems(QStringList items) { m_listWidget->clear(); m_listWidget->addItems(items); }
	// ����б���
	void addItem(QString text) { m_listWidget->addItem(text); }
	// ����ѡ����
	void setCurrentItem(int index) { m_listWidget->setCurrentRow(index); }
	// ��ȡѡ��������
	int getCurrentItemIndex() { return m_listWidget->currentRow(); }
	// ��ȡѡ�����ı�
	QString getCurrentItemText() { return m_listWidget->currentItem()->text(); }
	// ����ѡ�����ı�
	void setCurrentItemText(QString text) { m_listWidget->currentItem()->setText(text); }
	// ����ѡ�����ɫ
	void setCurrentItemBackground(QColor color) { m_listWidget->currentItem()->setBackground(color); }
	// ����ѡ����ǰ��ɫ
	void setCurrentItemForeground(QColor color) { m_listWidget->currentItem()->setForeground(color); }
	// ����ѡ��������
	void setCurrentItemFont(QFont font) { m_listWidget->currentItem()->setFont(font); }
	// ����ѡ����ͼ��
	void setCurrentItemIcon(QIcon icon) { m_listWidget->currentItem()->setIcon(icon); }
	// ����ѡ����״̬
	void setCurrentItemStatus(bool status) { m_listWidget->currentItem()->setHidden(!status); }
	// �����б���ǰ��ɫ
	void setItemForeground(int index, QColor color) { m_listWidget->item(index)->setForeground(color); }
	// �����б����ɫ
	void setItemBackground(int index, QColor color) { m_listWidget->item(index)->setBackground(color); }
	// �����б�������
	void setItemFont(int index, QFont font) { m_listWidget->item(index)->setFont(font); }
	// �����б���ͼ��
	void setItemIcon(int index, QIcon icon) { m_listWidget->item(index)->setIcon(icon); }
	// �����б���״̬
	void setItemStatus(int index, bool status) { m_listWidget->item(index)->setHidden(!status); }
	// �����б����ı�
	void setItemText(int index, QString text) { m_listWidget->item(index)->setText(text); }
	// ��ȡ�б�������
	int count() { return m_listWidget->count(); }
	// ��ȡ�б����ı�
	QString itemText(int index) { return m_listWidget->item(index)->text(); }
	// ��ȡ�б���ͼ��
	QIcon itemIcon(int index) { return m_listWidget->item(index)->icon(); }
	// ��ȡ�б���״̬
	bool itemStatus(int index) { return!m_listWidget->item(index)->isHidden(); }
	// ��ȡѡ����
	QListWidgetItem* currentItem() { return m_listWidget->currentItem(); }
	// ��ȡ������
	QList<QListWidgetItem*> items() { return m_listWidget->findItems("", Qt::MatchContains); }

	// ����ѡ��ģʽ
	void setSelectionMode(QAbstractItemView::SelectionMode mode) { m_listWidget->setSelectionMode(mode); }
	// ���ñ༭������
	void setEditTriggers(QAbstractItemView::EditTrigger trigger) { m_listWidget->setEditTriggers(trigger); }
	// ���ý������
	void setFocusPolicy(Qt::FocusPolicy policy) { m_listWidget->setFocusPolicy(policy); }

	// ���ð�ť�ı�
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
	// ���ð�ť״̬
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
	// ���ð�ť����ź�
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
	// ��ť��ʾ
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
	// ��ť����
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

// ��ǩ���༭�򡢰�ť��Ͽؼ�
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

	// ���ñ�ǩ�ı�
	void setLabelText(QString text) { m_label->setText(text); }
	// �����б༭�ı�
	void setLineEditText(QString text) { m_lineEdit->setText(text); }
	// ���ð�ť�ı�
	void setBtnText(QString text) { m_btn->setText(text); }
	// ����ֻ��״̬
	void setReadOnly(bool isReadOnly) { m_lineEdit->setReadOnly(isReadOnly); }
	// ����ռλ���ı�
	void setPlaceholderText(QString text) { m_lineEdit->setPlaceholderText(text); }
	// ���ð�ť����ź�
	void setBtnClicked(const std::function<void()>& func) { connect(m_btn, &QPushButton::clicked, func); }

private:
	QLabel* m_label = nullptr;
	QLineEdit* m_lineEdit = nullptr;
	QPushButton* m_btn = nullptr;
};
#endif // WIDGET_COMMON_H
