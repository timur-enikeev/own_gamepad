#include "numberofplayerspage.h"
#include "ui_numberofplayerspage.h"
#include <QDebug>

numberOfPlayersPage::numberOfPlayersPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::numberOfPlayersPage)
{
    ui->setupUi(this);
//    ui->numberOfPlayers->setMinimum(0);
//    ui->numberOfPlayers->setMaximum(reinterpret_cast<GamePads*>(parent)->num_gamepads);
    connect(ui->numberOfPlayers, SIGNAL(valueChanged(int)), this, SIGNAL(completeChanged()));
    connect(ui->numberOfPlayers, SIGNAL(valueChanged(int)), this, SLOT(setNumberOfPlayers(int)));

}

numberOfPlayersPage::~numberOfPlayersPage()
{

    delete ui;
}

void numberOfPlayersPage::setMaxPlayers(int players)
{
    //ui->numberOfPlayers->
}

void numberOfPlayersPage::initializePage()
{
    ui->numberOfPlayers->setMaximum(maxPlayers);
    ui->numberOfPlayers->setValue(maxPlayers);
    //    numberOfPlayers = &(ui->numberOfPlayers->value());
    setNumberOfPlayers(ui->numberOfPlayers->value());
    //    emit ui->numberOfPlayers->valueChanged();
}

bool numberOfPlayersPage::isComplete() const
{
    if (ui->numberOfPlayers->value() >= 1 && ui->numberOfPlayers->value() <= maxPlayers)
        return true;
    else
        return false;

}

void numberOfPlayersPage::setNumberOfPlayers(int nop)
{
    numberOfPlayers = nop;
}

/*int numberOfPlayersPage::nextId() const
{
    qDebug() << "nextId called";
    return -1;
}*/


