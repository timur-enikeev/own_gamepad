#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QThread>
#include <QTimer>
#include <QShortcut>
#include <QMessageBox>
#include <QSettings>
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
    QSettings settings;

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
//    void falseStartChanged();
    void gamepadUnplugged(int gamepad);
    void reassign();
    void getSettingsToControls(); //Set UI controls of settings from QSettings
    void setSettingsFromControls(); //Read data from UI controls of settings and write the data to QSettings
    void showParameters(bool);

};

#endif // MAINWINDOW_H
