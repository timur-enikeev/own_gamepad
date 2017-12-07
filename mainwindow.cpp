#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "assignwizard.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), startShortcut(QKeySequence(Qt::Key_X), this),
    resetShortcut(QKeySequence(Qt::Key_Z), this),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&gp, &GamePads::buttonPressed, &assignwiz.pp, &playerWizardPage::buttonPressed);

    gp.moveToThread(&gamepadsThread);

//    connect(&gamepadsThread, &QThread::finished, &gp, &GamePads::deleteLater);
    connect(&gamepadsThread, &QThread::started, &gp, &GamePads::waitEvents);
//    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::af);
    gamepadsThread.start();
    this->reassign();
    connect(&gp, &GamePads::gamepadUnplugged, this, &MainWindow::gamepadUnplugged);
    connect(ui->falseStartsEnable,SIGNAL(toggled(bool)),this,SLOT(falseStartChanged()));
    connect(ui->voiceSignals,SIGNAL(toggled(bool)),this,SLOT(falseStartChanged()));
    connect(ui->resetButton,SIGNAL(clicked()),this,SLOT(resetRound()));
    connect(ui->startButton,SIGNAL(clicked()),this,SLOT(startRound()));
    connect(ui->actionReassign,SIGNAL(triggered(bool)),this,SLOT(reassign()));

    banTimer.setSingleShot(true);
    connect(&banTimer, SIGNAL(timeout()), this, SLOT(unbanAll()));
    soundSignals.addPlayerNumberVoices(assignwiz.nop.numberOfPlayers);
    connect(ui->volumeSlider,SIGNAL(valueChanged(int)),&soundSignals, SLOT(changeVolume(int)));
    connect(ui->bannedListShow,SIGNAL(toggled(bool)),ui->bannedWidget,SLOT(setVisible(bool)));
//Setup keyboard shortcuts
    startShortcut.setAutoRepeat(false);
    connect(&startShortcut,SIGNAL(activated()),this,SLOT(startRound()));
    resetShortcut.setAutoRepeat(false);
    connect(&resetShortcut,SIGNAL(activated()),this,SLOT(resetRound()));

    ui->bannedWidget->hide();
    soundSignals.changeVolume(ui->volumeSlider->value());
    falseStartChanged();
}

MainWindow::~MainWindow()
{
    gp.a = false;
    gamepadsThread.quit();
    gamepadsThread.wait();
    delete ui;
}

void MainWindow::buttonPressed(int gamepad)
{
    int player = playersGamepads.indexOf(gamepad);
    if (stage == 2 || ((stage == 1) && (!ui->falseStartsEnable->isChecked())))
    {
        int player = playersGamepads.indexOf(gamepad);
        if (player >= 0 && (!ui->falseStartsEnable->isChecked() || ! banned.contains(player)))
        {
            ui->answeringPlayer->setText(QString::number(player+1));
            if (ui->voiceSignals->checkState() == Qt::Checked || ((ui->voiceSignals->checkState() == Qt::PartiallyChecked) && (ui->falseStartVoice->checkState() == Qt::Checked) && (!banned.empty())))
                soundSignals.answer(player, false);
            else if (ui->voiceSignals->checkState() == Qt::PartiallyChecked)
                soundSignals.answer(player, true);
            else
                soundSignals.answer(-1, false);
            stage = 0;
        }
    }
    else if (stage == 1)
    {
        if (!banned.contains(player))
        {
            banned.insert(player);
            if (ui->falseStartVoice->isChecked() && ui->voiceSignals->isChecked())
                soundSignals.falseStart(player);
            ui->bannedPlayers->addItem(QString::number(player+1));
        }
    }
    qDebug() << "bp" << gamepad;
}

void MainWindow::startRound()
{
    if (stage == 0 || stage == 2)
        unbanAll();
    ui->answeringPlayer->clear();

    stage = 2;
    if (ui->falseStartsEnable->isChecked() && (ui->banTime->value() > 0))
    {
        banTimer.start(ui->banTime->value() * 1000);
    }
    soundSignals.roundStart();
}

void MainWindow::resetRound()
{
    ui->answeringPlayer->clear();
    stage = 1;
    unbanAll();
    soundSignals.roundReset();
    banTimer.stop();
}

void MainWindow::falseStartChanged()
{
    if (!ui->falseStartsEnable->isChecked())
    {
        ui->banTime->setEnabled(false);
        ui->falseStartVoice->setEnabled(false);
        ui->bannedListShow->setEnabled(false);
        ui->bannedWidget->hide();
        unbanAll();
    }
    else if (ui->voiceSignals->isChecked())
    {
        ui->banTime->setEnabled(true);
        ui->falseStartVoice->setEnabled(true);
        ui->bannedListShow->setEnabled(true);
        ui->bannedWidget->setVisible(ui->bannedListShow->isChecked());
    }
    else
    {
        ui->banTime->setEnabled(true);
        ui->falseStartVoice->setEnabled(false);
        ui->bannedWidget->setVisible(ui->bannedListShow->isChecked());
        ui->bannedListShow->setEnabled(true);
    }

}

void MainWindow::unbanAll()
{
    banned.clear();
    ui->bannedPlayers->clear();
}

void MainWindow::gamepadUnplugged(int gamepad)
{
    QMessageBox::critical(this, tr("Unplugged"),
                                     tr("Gamepad was unplugged"));
}

void MainWindow::reassign()
{
    assignwiz.~assignWizard();
    new(&assignwiz) assignWizard(this, gp.num_gamepads);
    assignwiz.setModal(true);
    disconnect(&gp, &GamePads::buttonPressed, this, &MainWindow::buttonPressed);
    connect(&gp, &GamePads::buttonPressed, &assignwiz.pp, &playerWizardPage::buttonPressed);
    QVector<int> playersGamepadsTemp;
    assignwiz.pp.playersGamepads = &playersGamepadsTemp;
    assignwiz.exec();
    disconnect(&gp, &GamePads::buttonPressed, &assignwiz.pp, &playerWizardPage::buttonPressed);
    connect(&gp, &GamePads::buttonPressed, this, &MainWindow::buttonPressed);

    if (assignwiz.result()){
        playersGamepads = playersGamepadsTemp;
    }
    //    qDebug()<< "done" << assignwiz.result();

}
