#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QThread>
#include <QTimer>
#include <QShortcut>
#include "gamepads.h"
#include "assignwizard.h"
#include "soundsignals.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QThread gamepadsThread;
    GamePads gp;
    QVector<int>playersGamepads; //Index is player's number, value is gamepad number.

private:
    Ui::MainWindow *ui;
    int stage = 0; // 0: don't accept answers, 1 : question is pronouncing, 2 : waiting for answers
    assignWizard assignwiz;
    QSet<int> banned;
    QTimer banTimer;
    SoundSignals soundSignals;
    QShortcut resetShortcut;
    QShortcut startShortcut;
public slots:
    void buttonPressed(int gamepad);
    void resetRound(void);
    void startRound(void);
    void unbanAll();
    void falseStartChanged();

};

#endif // MAINWINDOW_H
