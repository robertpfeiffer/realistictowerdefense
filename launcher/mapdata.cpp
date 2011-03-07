#include "mapdata.h"

MapData::MapData(QObject *parent, const QFileInfo fileinfo) : QObject(parent)
{
    _fileinfo = fileinfo;
}

QString MapData::getDisplayName()
{
    return _fileinfo.baseName();
}
