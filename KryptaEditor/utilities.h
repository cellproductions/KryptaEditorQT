#ifndef UTILITIES_H
#define UTILITIES_H

#include <Utilities/String.h>
#include <QString>

QString kryToQString(const kry::Util::String& string)
{
    return QString::fromStdString(std::string(string.getData(), string.getLength()));
}

kry::Util::String qToKString(const QString& string)
{
    std::string conv(std::move(string.toStdString()));
    return kry::Util::String(conv.c_str(), conv.length());
}

#endif // UTILITIES_H
