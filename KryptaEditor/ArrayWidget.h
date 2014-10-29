#ifndef ARRAYWIDGET_H
#define ARRAYWIDGET_H

#include <QWidget>

class QLineEdit;
class QPushButton;

class ArrayWidget : public QWidget
{
	Q_OBJECT

	public:
		ArrayWidget(QWidget *parent);
		~ArrayWidget();

	signals:
		void addClicked(QLineEdit* edit);
		void removeClicked(QLineEdit* edit);

	private:
		QLineEdit* tbEdit;
		QPushButton* bAdd;
		QPushButton* bRemove;
};

#endif // SPAWNWIDGET_H
