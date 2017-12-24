#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "assignwizard.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), startShortcut(QKeySequence(Qt::Key_X), this),
    resetShortcut(QKeySequence(Qt::Key_Z), this), settings (QSettings::IniFormat, QSettings::UserScope, "Timur_Enikeev", "OwnGamepad"),
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
//    connect(ui->falseStartsEnable,SIGNAL(toggled(bool)),this,SLOT(falseStartChanged()));
//    connect(ui->voiceSignals,SIGNAL(toggled(bool)),this,SLOT(falseStartChanged()));
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
    getSettingsToControls();
    soundSignals.changeVolume(ui->volumeSlider->value());
//    falseStartChanged();
    connect(ui->volumeSlider,SIGNAL(valueChanged(int)),this, SLOT(setSettingsFromControls()));
    connect(ui->bannedListShow,SIGNAL(toggled(bool)), this, SLOT(setSettingsFromControls()));
    connect(ui->falseStartsEnable,SIGNAL(toggled(bool)), this, SLOT(setSettingsFromControls()));
    connect(ui->voiceSignals, SIGNAL(stateChanged(int)), this, SLOT(setSettingsFromControls()));
    connect(ui->falseStartVoice, SIGNAL(stateChanged(int)), this, SLOT(setSettingsFromControls()));
    connect(ui->banTime, SIGNAL(valueChanged(int)), this, SLOT(setSettingsFromControls()));
    connect(ui->actionParameters, SIGNAL(toggled(bool)), this, SLOT(showParameters(bool)));
    resetRound();
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
    qDebug() << "fs" << settings.value("falsestarts/falsestarts_enabled").toBool();
    int player = playersGamepads.indexOf(gamepad);
    if (stage == 2 || ((stage == 1) && (!settings.value("falsestarts/falsestarts_enabled").toBool())))
    {
        int player = playersGamepads.indexOf(gamepad);
        if (player >= 0 && (!settings.value("falsestarts/falsestarts_enabled").toBool() || ! banned.contains(player)))
        {
            ui->answeringPlayer->setText(QString::number(player+1));
            if (settings.value("sound/voice").toInt() ==  Qt::Checked || ((settings.value("sound/voice").toInt() == Qt::PartiallyChecked) && (settings.value("sound/falsestart_voice").toBool()) && (!banned.empty())))
                soundSignals.answer(player, false);
            else if (settings.value("sound/voice").toInt() == Qt::PartiallyChecked)
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
            if (settings.value("sound/falsestart_voice").toBool() && settings.value("sound/voice").toInt() > 0)
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
    if (settings.value("falsestarts/falsestarts_enabled").toBool() && (settings.value("falsestarts/ban_time").toInt() > 0))
    {
        banTimer.start(settings.value("falsestarts/ban_time").toInt() * 1000);
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
/*
void MainWindow::falseStartChanged()
{
    qDebug() << "falsestartchanged";
    setSettingsFromControls();
    if (!settings.value("falsestarts/falsestarts_enabled").toBool())
    {
//        settings.setValue("falsestarts/falsestarts_enabled", true);
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
//        settings.setValue("falsestarts/falsestarts_enabled", false);
        ui->banTime->setEnabled(true);
        ui->falseStartVoice->setEnabled(false);
        ui->bannedWidget->setVisible(ui->bannedListShow->isChecked());
        ui->bannedListShow->setEnabled(true);
    }

}
*/
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

void MainWindow::getSettingsToControls()
{

    ui->falseStartsEnable->setChecked(settings.value("falsestarts/falsestarts_enabled").toBool());
    ui->volumeSlider->setValue(settings.value("sound/volume", ui->volumeSlider->value()).toInt());
//    soundSignals.changeVolume(ui->volumeSlider->value());
    ui->banTime->setValue(settings.value("falsestarts/ban_time").toInt());
    ui->bannedListShow->setChecked(settings.value("view/show_banned").toBool());
    if (settings.value("sound/voice").toInt() == 0){
        ui->voiceSignals->setCheckState(Qt::Unchecked);}
    else if (settings.value("sound/voice").toInt() == 1)
        ui->voiceSignals->setCheckState(Qt::PartiallyChecked);
    else
        ui->voiceSignals->setCheckState(Qt::Checked);
    ui->falseStartVoice->setChecked(settings.value("sound/falsestart_voice").toBool());
}

void MainWindow::setSettingsFromControls()
{
    qDebug() << "setsettings";

    settings.setValue("falsestarts/falsestarts_enabled", ui->falseStartsEnable->isChecked());
    settings.setValue("sound/volume", ui->volumeSlider->value());
    settings.setValue("falsestarts/ban_time", ui->banTime->value());
    settings.setValue("view/show_banned", ui->bannedListShow->isChecked());
    settings.setValue("sound/voice", ui->voiceSignals->checkState());
    settings.setValue("sound/falsestart_voice", ui->falseStartVoice->isChecked());
    ui->banTime->setEnabled(settings.value("falsestarts/falsestarts_enabled").toBool());
    ui->falseStartVoice->setEnabled(settings.value("falsestarts/falsestarts_enabled").toBool() && settings.value("sound/voice").toBool());
}

void MainWindow::showParameters(bool page)
{
    ui->stackedWidget->setCurrentIndex(page);
}
