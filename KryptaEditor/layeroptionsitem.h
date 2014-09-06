#ifndef LAYEROPTIONSITEM_H
#define LAYEROPTIONSITEM_H

#include <QListWidgetItem>

class LayerOptionsItem : public QListWidgetItem
{
    public:
        LayerOptionsItem();
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


void LayerOptionsItem::setDescription(const QString& description)
{
    descr = description;
}

void LayerOptionsItem::setSize(const QSize& dimensions)
{
    dim = dimensions;
}

void LayerOptionsItem::setWidth(int w)
{
    dim.setWidth(w);
}

void LayerOptionsItem::setHeight(int h)
{
    dim.setHeight(h);
}

const QString& LayerOptionsItem::getDescription() const
{
    return descr;
}

const QSize& LayerOptionsItem::getSize() const
{
    return dim;
}

int LayerOptionsItem::getWidth() const
{
    return dim.width();
}

int LayerOptionsItem::getHeight() const
{
    return dim.height();
}


#endif // LAYEROPTIONSITEM_H
