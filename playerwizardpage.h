#ifndef PLAYERWIZARDPAGE_H
#define PLAYERWIZARDPAGE_H

#include <QWizardPage>
#include <QDebug>
#include  "numberofplayerspage.h"
//#include "assignwizard.h"


namespace Ui {
class playerWizardPage;
}

class playerWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit playerWizardPage(QWidget *parent = 0);
    ~playerWizardPage();
    int numberOfPlayers;
//    int nextId() const Q_DECL_OVERRIDE;
    bool isComplete() const Q_DECL_OVERRIDE;
    void initializePage() ;
    numberOfPlayersPage * nop;
    void assignNextPlayer();
    QVector <int> * playersGamepads;
public slots:
    void buttonPressed(int gamepad);



private:
    Ui::playerWizardPage *ui;
    int currentPlayer = 0;
};

#endif // PLAYERWIZARDPAGE_H
