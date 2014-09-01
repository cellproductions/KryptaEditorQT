#ifndef PRJWIZARD_H
#define PRJWIZARD_H

#include "dialogresult.h"
#include <QDialog>
#include <QListWidgetItem>

namespace Ui
{
    class PrjWizard;
}

class PrjSetupDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit PrjSetupDialog(QWidget *parent = 0);
        ~PrjSetupDialog();

        DialogResult showDialog();

        inline Ui::PrjWizard* getUI() const;

    private:
        Ui::PrjWizard *ui;
        DialogResult lastresult;
};

class LayerOptions : public QListWidgetItem
{
    public:
        LayerOptions();
        inline void setDescription(const QString& description);
        inline void setSize(const QSize& dimensions);
        inline void setWidth(int w);
        inline void setHeight(int h);

        inline const QString& getDescription() const;
        inline const QSize& getSize() const;
        inline int getWidth() const;
        inline int getHeight() const;

        static const QListWidgetItem::ItemType LayerOptionType = static_cast<QListWidgetItem::ItemType>(1);

    private:
        QString descr;
        QSize dim;
};


Ui::PrjWizard* PrjSetupDialog::getUI() const
{
    return ui;
}


void LayerOptions::setDescription(const QString& description)
{
    descr = description;
}

void LayerOptions::setSize(const QSize& dimensions)
{
    dim = dimensions;
}

void LayerOptions::setWidth(int w)
{
    dim.setWidth(w);
}

void LayerOptions::setHeight(int h)
{
    dim.setHeight(h);
}

const QString& LayerOptions::getDescription() const
{
    return descr;
}

const QSize& LayerOptions::getSize() const
{
    return dim;
}

int LayerOptions::getWidth() const
{
    return dim.width();
}

int LayerOptions::getHeight() const
{
    return dim.height();
}

#endif // PRJWIZARD_H
