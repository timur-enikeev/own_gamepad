#include "gamepads.h"
#include <QDebug>
GamePads::GamePads(QObject *parent) : QObject(parent)
{

    SDL_SetMainReady();
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC);
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");

    num_gamepads = SDL_NumJoysticks();
    for (int i = 0 ; i < num_gamepads ; i++)
        gamepads.append(SDL_JoystickOpen(i));

}

GamePads::~GamePads()
{
    SDL_Quit();
}

void GamePads::waitEvents()
{
    QList<SDL_Event> events;
    SDL_Event event;
    while (a)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_JOYBUTTONDOWN)
            {
                events.append(event);
//                qDebug() << "pressed" << event.jbutton.which << event.jbutton.button ;
                emit buttonPressed(event.jbutton.which);
            }
        }
        if (events.size() > 1)
        {    foreach (SDL_Event e, events)
            {
               qDebug() << e.jbutton.which;
            }
        qDebug() << "Next iteration";
        }
        events.clear();


    }
}
