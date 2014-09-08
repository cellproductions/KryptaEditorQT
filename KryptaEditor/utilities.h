#ifndef UTILITIES_H
#define UTILITIES_H

#include <Utilities/String.h>
#include <QString>

QString kryToQString(const kry::Util::String& string);
kry::Util::String qToKString(const QString& string);

#endif // UTILITIES_H
