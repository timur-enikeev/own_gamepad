#ifndef ASSIGNWIZARD_H
#define ASSIGNWIZARD_H

#include <QWizard>
#include "numberofplayerspage.h"
#include "playerwizardpage.h"

namespace Ui {
class assignWizard;
}

class assignWizard : public QWizard
{
    Q_OBJECT

public:
    explicit assignWizard(QWidget *parent = 0, int max_players = 0);
    ~assignWizard();
    numberOfPlayersPage nop;
    playerWizardPage pp;
public slots:
/*    void finishedSlot(int result)
    {
        qDebug() << "result" << result;
    }
*/
private:
    Ui::assignWizard *ui;
    int max_players;
//    QList<playerWizardPage> playerPages;
};

#endif // ASSIGNWIZARD_H
