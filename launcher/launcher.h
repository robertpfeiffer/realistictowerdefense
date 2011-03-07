#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <QMainWindow>
#include <QFileInfo>

namespace Ui {
    class Launcher;
}

class Launcher : public QMainWindow
{
    Q_OBJECT

public:
    explicit Launcher(QWidget *parent = 0);
    ~Launcher();

public slots:
    void startGame();

private:
    void _findAndAddMaps();
    void _addMap(const QFileInfo fileinfo);
    QString _getStereoParamter();

    Ui::Launcher *ui;
};

#endif // LAUNCHER_H
