#ifndef ARRAYWIDGET_H
#define ARRAYWIDGET_H

#include <QWidget>

class QComboBox;
class QPushButton;

class ArrayWidget : public QWidget
{
	Q_OBJECT

	public:
		ArrayWidget(QWidget *parent);
		~ArrayWidget();

		inline QComboBox* getIDs();

	signals:
		void addClicked(QComboBox* ids);
		void removeClicked(QComboBox* ids);

	private:
		QComboBox* cbIds;
		QPushButton* bAdd;
		QPushButton* bRemove;
};


QComboBox* ArrayWidget::getIDs()
{
	return cbIds;
}

#endif // SPAWNWIDGET_H
