//  main.cpp
//
//  Copyright © 2018 Compiled Creations Limited. All rights reserved.
//

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined(_WIN64)

#include <iostream>
#define SDL_MAIN_HANDLED
#include <windows.h>
#include "../Graphic Lib/libreria.h"
#include "../Graphic Lib/NFont/NFont.h"

#elif __APPLE__
#include "../Graphic Lib/libreria.h"
#include "../Graphic Lib/NFont/NFont.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <SDL2/SDL.h>
#pragma clang diagnostic pop
#endif

#include "./joc.hpp"
#include "./info_joc.hpp"

int main()
{
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO);

    /* ── Elegir el modo ───────────────────────────────────────── */
    ModeJoc modeInicial = MODE_JOC_NORMAL;          // valor por defecto
    std::cout << "\n"
        << "=============================================\n"
        << "         P R O Y E C T E   D A M E S   \n"
        << "=============================================\n"
        << "   [0]  Jugar nova partida  (MODE NORMAL)\n"
        << "   [1]  Reproduir partida   (MODE REPLAY)\n"
        << "---------------------------------------------\n"
        << "  Introdueix la teva opcio i prem  ENTER  ";
    int op;
    if (std::cin >> op && op == 1)
        modeInicial = MODE_JOC_REPLAY;
    /* ─────────────────────────────────────────────────────────── */

    Screen pantalla(TAMANY_PANTALLA_X, TAMANY_PANTALLA_Y);
    pantalla.show();

    Joc joc;
    joc.inicialitza(modeInicial,
        "data/Games/tauler_inicial.txt",
        "data/Games/moviments.txt");

    while (!Keyboard_GetKeyTrg(KEYBOARD_ESCAPE))
    {
        pantalla.processEvents();
        joc.actualitza(Mouse_getX(), Mouse_getY(), Mouse_getBtnLeft());
        pantalla.update();
    }

    if (modeInicial == MODE_JOC_NORMAL)
        joc.finalitza();

    SDL_Quit();
    return 0;
}
