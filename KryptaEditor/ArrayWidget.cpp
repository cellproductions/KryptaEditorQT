#include "ArrayWidget.h"
#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>

ArrayWidget::ArrayWidget(QWidget *parent) : QWidget(parent), tbEdit(new QLineEdit(this)), bAdd(new QPushButton("+", this)),
											bRemove(new QPushButton("-", this))
{
	tbEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
	auto layout = new QVBoxLayout(this);
	this->setLayout(layout);
	layout->addWidget(tbEdit);
	layout->addWidget(bAdd);
	layout->addWidget(bRemove);
	connect(bAdd, &QPushButton::clicked, [this](bool)
	{
		emit addClicked(tbEdit);
	});
	connect(bRemove, &QPushButton::clicked, [this](bool)
	{
		emit removeClicked(tbEdit);
	});
}

ArrayWidget::~ArrayWidget()
{
	delete tbEdit;
	delete bAdd;
	delete bRemove;
}
