#ifndef NUMBEROFPLAYERSPAGE_H
#define NUMBEROFPLAYERSPAGE_H

#include <QWizardPage>
//#include "assignwizard.h"
#include "gamepads.h"
namespace Ui {
class numberOfPlayersPage;
}

class numberOfPlayersPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit numberOfPlayersPage(QWidget *parent = 0);
    ~numberOfPlayersPage();
    void setMaxPlayers(int players);
    void initializePage();
    bool isComplete() const Q_DECL_OVERRIDE;
    int maxPlayers;
    int numberOfPlayers;
//    int nextId() const;

public slots:
    void setNumberOfPlayers(int nop);

private:
    Ui::numberOfPlayersPage *ui;

};

#endif // NUMBEROFPLAYERSPAGE_H
