#include "playerwizardpage.h"
#include "ui_playerwizardpage.h"

playerWizardPage::playerWizardPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::playerWizardPage)
{
    ui->setupUi(this);
}

playerWizardPage::~playerWizardPage()
{
    delete ui;
}
/*
int playerWizardPage::nextId() const
{
    return 1;
}*/

bool playerWizardPage::isComplete() const
{
    if (currentPlayer >= nop->numberOfPlayers)
        return true;
    else
        return false;
}

void playerWizardPage::initializePage()
{
    playersGamepads->clear();
    currentPlayer = 0;
    assignNextPlayer();
/*    for (currentPlayer = 0; currentPlayer < nop->numberOfPlayers; currentPlayer++)
    {
        ui->playerNumber->setText(QString::number(currentPlayer + 1));
    }*/
}

void playerWizardPage::assignNextPlayer()
{
    ui->playerNumber->setText(QString::number(currentPlayer + 1));
}

void playerWizardPage::buttonPressed(int gamepad)
{
    if (playersGamepads->indexOf(gamepad) == -1)
    {
        playersGamepads->append(gamepad);
        currentPlayer++;
        if (currentPlayer < nop->numberOfPlayers)
            assignNextPlayer();
        else emit completeChanged();
    }
}
