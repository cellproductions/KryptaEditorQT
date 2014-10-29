#include "ArrayWidget.h"
#include <QComboBox>
#include <QPushButton>
#include <QBoxLayout>

ArrayWidget::ArrayWidget(QWidget *parent) : QWidget(parent), cbIds(new QComboBox(this)), bAdd(new QPushButton("+", this)), bRemove(new QPushButton("-", this))
{
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	cbIds->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
	bAdd->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);
	bRemove->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);
	auto layout = new QHBoxLayout(this);
	layout->addWidget(cbIds);
	layout->addWidget(bAdd);
	layout->addWidget(bRemove);
	layout->setMargin(0);
	layout->setStretch(0, 80);
	layout->setStretch(1, 10);
	layout->setStretch(2, 10);
	layout->setSpacing(0);
	setLayout(layout);
	cbIds->setEditable(false);
	connect(bAdd, &QPushButton::clicked, [this](bool)
	{
		emit addClicked(cbIds);
	});
	connect(bRemove, &QPushButton::clicked, [this](bool)
	{
		emit removeClicked(cbIds);
	});
}

ArrayWidget::~ArrayWidget()
{
	delete cbIds;
	delete bAdd;
	delete bRemove;
}
