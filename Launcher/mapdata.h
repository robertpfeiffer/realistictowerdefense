#pragma once

#include <QString>
#include <QObject>
#include <QFileInfo>

class MapData : public QObject
{
    Q_OBJECT

public:
    explicit MapData(QObject *parent, const QFileInfo fileinfo);

    inline QString getFilepath() {return _fileinfo.filePath(); };
    QString getDisplayName();
private:
    QFileInfo _fileinfo;
};
