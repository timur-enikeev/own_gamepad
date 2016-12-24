#ifndef GAMEPADS_H
#define GAMEPADS_H

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <QObject>

class GamePads : public QObject
{
    Q_OBJECT
public:
    explicit GamePads(QObject *parent = 0);
    ~GamePads();
    int num_gamepads = 0;
    bool a = true;
    QList<SDL_Joystick*> gamepads;

signals:
    void buttonPressed(int gamepad);
public slots:
    void waitEvents();
};

#endif // GAMEPADS_H
