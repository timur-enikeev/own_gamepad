#include "assignwizard.h"
#include "ui_assignwizard.h"
#include <QDebug>

assignWizard::assignWizard(QWidget *parent, int num_of_gamepads) :
    QWizard(parent), max_players(num_of_gamepads),
    ui(new Ui::assignWizard)
{
    ui->setupUi(this);
    nop.maxPlayers = max_players;
    pp.nop = &nop;
    addPage(&nop);
    addPage(&pp);
//    connect(this, SIGNAL(finished(int)), this, SLOT(finishedSlot(int)));
}

assignWizard::~assignWizard()
{
    delete ui;
}



