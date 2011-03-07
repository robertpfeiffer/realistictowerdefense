#include "launcher.h"
#include "ui_launcher.h"
#include "mapdata.h"

#include "QDir"
#include "QProcess"

Launcher::Launcher(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Launcher)
{
    ui->setupUi(this);
    this->_findAndAddMaps();
}

void Launcher::startGame()
{
    QListWidgetItem* currentItem = ui->mapListWidget->currentItem();
    MapData* mapData = reinterpret_cast<MapData*>(currentItem->data(Qt::UserRole).value<void*>());
    if (!mapData) return;

    QStringList parameters;
    parameters << "--map";
    parameters << mapData->getFilepath();

    parameters << "--stereo";
    parameters << _getStereoParamter();

    parameters << "--samples";
    parameters << ui->multiSampleSpinBox->text();

    if (ui->singleDisplayRadioButton->isChecked())
    {
        parameters << "--screen";
        parameters << ui->singleDisplaySpinBox->text();
    }

    if (ui->windowModeRadioButton->isChecked())
    {
        parameters << "--screen";
        parameters << ui->windowModeDisplaySpinBox->text();

        parameters << "--window";
        parameters << ui->windowModePosXSpinBox->text();
        parameters << ui->windowModePosYSpinBox->text();
        parameters << ui->windowModeWidthSpinBox->text();
        parameters << ui->windowModeHeightSpinBox->text();
    }

    QProcess *gameProcess = new QProcess(this);
    this->hide();
    gameProcess->start("Towerdefense", parameters);
    gameProcess->waitForFinished(-1);
    this->show();
}

QString Launcher::_getStereoParamter()
{
    switch (ui->stereoComboBox->currentIndex())
    {
    case 0:
        return "OFF";
    case 1:
        return "ON";
    case 2:
        return "ANAGLYPHIC";
    case 3:
        return "QUAD_BUFFER";
    case 4:
        return "HORIZONTAL_SPLIT";
    case 5:
        return "VERTICAL_SPLIT";
    case 6:
        return "LEFT_EYE";
    case 7:
        return "RIGHT_EYE";
    case 8:
        return "HORIZONTAL_INTERLACE";
    case 9:
        return "VERTICAL_INTERLACE";
    case 10:
        return "CHECKERBOARD";
    default:
        return "OFF";
    }
}

Launcher::~Launcher()
{
    delete ui;
}

void Launcher::_findAndAddMaps()
{
    QDir dir("maps");
    QStringList filters("*.tdmap");
    QFileInfoList filenames = dir.entryInfoList(filters, QDir::Files);
    foreach (QFileInfo filename, filenames) {
        this->_addMap(filename);
    }
    if (ui->mapListWidget->count() <= 0)
    {
        ui->startButton->setEnabled(false);
    }
    ui->mapListWidget->setCurrentRow(0);
}

void Launcher::_addMap(const QFileInfo fileinfo)
{
    QListWidgetItem* item = new QListWidgetItem();

    MapData* mapData = new MapData(this, fileinfo);

    item->setText(mapData->getDisplayName());
    item->setData(Qt::UserRole, QVariant::fromValue<void*>(mapData));
    ui->mapListWidget->addItem(item);
}
