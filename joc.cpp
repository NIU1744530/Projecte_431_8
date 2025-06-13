#include "joc.hpp"
#include "GraphicManager.h"
#include <iostream>
#include "cua_moviments.h"
#include <map>
using namespace std;

Joc::Joc() : m_mode(MODE_JOC_NORMAL), m_torn(COLOR_BLANC), m_fitxaSeleccionada(-1, -1), m_prevMouse(false), m_haAcabat(false), m_empat(false), m_guanyador(COLOR_EMPTY) { ; }

Joc::~Joc()
{
    m_cua.finalitza();
}

void Joc::setMode(ModeJoc mode, const string& fitxerMovs)
{
    m_mode = mode;
    m_cua.inicialitza(mode, fitxerMovs);
}

void Joc::inicialitza(ModeJoc mode, const std::string& nomFitxerTauler, const std::string& nomFitxerMoviments)
{
    m_mode = mode;
    m_torn = COLOR_BLANC;
    m_fitxaSeleccionada = Posicio(-1, -1);
    m_movimentsValids.clear();
    m_haAcabat = false;
    m_nomFitxerMoviments = nomFitxerMoviments;

    m_tauler.inicialitza(nomFitxerTauler);

    if (mode == MODE_JOC_NORMAL)
    {
        m_cua.inicialitza(MODE_JOC_NORMAL, nomFitxerMoviments);
    }
    else if (mode == MODE_JOC_REPLAY)
    {
        m_cua.inicialitza(MODE_JOC_REPLAY, nomFitxerMoviments);
    }

    m_historial.clear();
    string clau0 = m_tauler.toString() + to_string(m_torn);
    m_historial[clau0] = 1;
}

bool Joc::actualitza(int mouseX, int mouseY, bool mouseStatus)
{
    GraphicManager::getInstance()->drawSprite(GRAFIC_FONS, 0, 0);
    m_tauler.visualitza();

    if (m_empat)
    {
        dibuixaTextModeITorn();
        dibuixaPosicionsValides();
        return true;
    }

    bool click = mouseStatus && !m_prevMouse;
    m_prevMouse = mouseStatus;

    int fila, col;
    if (click && esClickDinsTauler(mouseX, mouseY, fila, col))
    {
        if (m_mode == MODE_JOC_REPLAY)
        {
            processaClickReplay(fila, col);
        }
        else
        {
            processaClickNormal(fila, col);
        }
    }

    if (m_mode == MODE_JOC_NORMAL && m_guanyador == COLOR_EMPTY)
    {
        m_guanyador = m_tauler.comprovaGuanyador();
    }

    dibuixaTextModeITorn();

    dibuixaPosicionsValides();

    return m_haAcabat || m_empat;
}

void Joc::finalitza()
{
    if (m_mode == MODE_JOC_NORMAL)
    {
        m_cua.finalitza();
    }
}

bool Joc::esClickDinsTauler(int x, int y, int& fila, int& col) const
{
    if (x >= POS_X_TAULER + CASELLA_INICIAL_X && x < POS_X_TAULER + CASELLA_INICIAL_X + NUM_COLS_TAULER * AMPLADA_CASELLA && y >= POS_Y_TAULER + CASELLA_INICIAL_Y && y < POS_Y_TAULER + CASELLA_INICIAL_Y + NUM_FILES_TAULER * ALCADA_CASELLA)
    {
        col = (x - (POS_X_TAULER + CASELLA_INICIAL_X)) / AMPLADA_CASELLA;
        fila = (y - (POS_Y_TAULER + CASELLA_INICIAL_Y)) / ALCADA_CASELLA;
        return true;
    }
    return false;
}

void Joc::dibuixaTextModeITorn() const
{
    string txtMode;
    if (m_mode == MODE_JOC_NORMAL)
    {
        txtMode = "Mode Joc: Normal";
    }
    else
    {
        txtMode = "Mode Joc: Replay";
    }

    string txtTorn;
    if (m_torn == COLOR_BLANC)
    {
        txtTorn = "Jugador actual: Blanques";
    }
    else
    {
        txtTorn = "Jugador actual: Negres";
    }

    GraphicManager::getInstance()->drawFont(FONT_WHITE_30, 50, 650, 0.8, txtMode);
    GraphicManager::getInstance()->drawFont(FONT_WHITE_30, 50, 700, 0.8, txtTorn);

    if (m_mode == MODE_JOC_REPLAY && m_cua.buida())
    {
        GraphicManager::getInstance()->drawFont(FONT_RED_30, 50, 750, 0.8, "NO HI HA MES MOVIMENTS PER REPRODUIR");
    }

    if (m_mode == MODE_JOC_NORMAL && m_guanyador != COLOR_EMPTY)
    {
        string missatge;
        if (m_guanyador == COLOR_BLANC)
        {
            missatge = "Guanyador: Blanques";
        }
        else
        {
            missatge = "Guanyador: Negres";
        }
        GraphicManager::getInstance()->drawFont(FONT_GREEN_30, 50, 750, 0.8, missatge);
    }

    if (m_empat && m_mode == MODE_JOC_NORMAL)
    {
        GraphicManager::getInstance()->drawFont(FONT_WHITE_30, 50, 780, 0.8, "EMPAT PER TRIPLE REPETICIO");
    }
}


void Joc::dibuixaPosicionsValides() const
{
    for (const Posicio& p : m_movimentsValids)
    {
        int posX = POS_X_TAULER + CASELLA_INICIAL_X + p.getColumna() * AMPLADA_CASELLA;
        int posY = POS_Y_TAULER + CASELLA_INICIAL_Y + p.getFila() * ALCADA_CASELLA;
        GraphicManager::getInstance()->drawSprite(GRAFIC_POSICIO_VALIDA, posX, posY);
    }
}

void Joc::processaClickReplay(int fila, int col)
{
    if (m_empat == false) 
    {
        if (m_cua.buida() == false) 
        {
            Moviment mov;
            bool extret = m_cua.extreuMoviment(mov);
            if (extret == true) 
            {
                int fi, ci, ff, cf;
                mov.getPosicioInicial(fi, ci);
                mov.getPosicioFinal(ff, cf);
                m_tauler.mouFitxa(Posicio(fi, ci), Posicio(ff, cf));

                if (m_torn == COLOR_BLANC) 
                {
                    m_torn = COLOR_NEGRE;
                }
                else 
                {
                    m_torn = COLOR_BLANC;
                }

                anotaPosicioIDetectaEmpat();
            }
        }
        else 
        {
            GraphicManager::getInstance()->drawFont(FONT_RED_30, 50, 750, 0.8, "No hi ha mes moviments per reproduir");
        }
    }
}


void Joc::processaClickNormal(int fila, int col)
{
    if (m_empat == false)
    {
        Posicio posClic(fila, col);
        if (m_fitxaSeleccionada.getFila() == -1)
        {
            Fitxa& f = m_tauler.getFitxa(posClic);
            if (f.esBuida() == false && f.getColor() == m_torn)
            {
                m_fitxaSeleccionada = posClic;
                m_movimentsValids = m_tauler.getPosicionsPossibles(posClic);
            }
        }
        else
        {
            if (esMovimentValid(posClic) == true)
            {
                m_tauler.mouFitxa(m_fitxaSeleccionada, posClic);
                Moviment m(m_torn);
                m.afegirPosicio(m_fitxaSeleccionada.getFila(), m_fitxaSeleccionada.getColumna());
                m.afegirPosicio(posClic.getFila(), posClic.getColumna());
                m_cua.afegeixMoviment(m);
                m_cua.finalitza();
                anotaPosicioIDetectaEmpat();

                if (m_torn == COLOR_BLANC)
                {
                    m_torn = COLOR_NEGRE;
                }
                else
                {
                    m_torn = COLOR_BLANC;
                }
            }
            m_fitxaSeleccionada = Posicio(-1, -1);
            m_movimentsValids.clear();
        }
    }
}

bool Joc::esMovimentValid(const Posicio& pos) const
{
    for (vector<Posicio>::const_iterator it = m_movimentsValids.cbegin(); it != m_movimentsValids.cend(); ++it)
    {
        if (*it == pos)
        {
            return true;
        }
    }
    return false;
}


void Joc::anotaPosicioIDetectaEmpat()
{
    string clau = m_tauler.toString() + to_string(m_torn);
    int& n = m_historial[clau];
    ++n;
    if (n == 3)
    {
        m_empat = true;
        m_haAcabat = true;
    }
}
