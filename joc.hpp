#ifndef JOC_H
#define JOC_H

#include "info_joc.hpp"
#include "tauler.hpp"
#include "cua_moviments.h"
#include "enums.h"
#include <vector>
#include <string>
#include <map>  
using namespace std;


class Joc
{
public:
    Joc();
    ~Joc();
    ColorFitxa m_guanyador = COLOR_EMPTY;
    void inicialitza(ModeJoc mode, const string& nomFitxerTauler, const string& nomFitxerMoviments);
    bool actualitza(int mousePosX, int mousePosY, bool mouseStatus);
    void finalitza();
    void setMode(ModeJoc mode, const string& fitxerMovs);
    Tauler& getTauler()
    {
        return m_tauler;
    }

private:
    ModeJoc m_mode;
    Tauler m_tauler;
    CuaMoviments m_cua;
    ColorFitxa m_torn;
    map<string, int> m_historial;
    bool m_empat = false;
    void anotaPosicioIDetectaEmpat();
    Posicio m_fitxaSeleccionada;
    vector<Posicio> m_movimentsValids;
    bool m_prevMouse;
    bool m_haAcabat;
    string m_nomFitxerMoviments;
    bool esClickDinsTauler(int x, int y, int& fila, int& col) const;
    void dibuixaTextModeITorn() const;
    void dibuixaPosicionsValides() const;
    void processaClickReplay(int fila, int col);
    void processaClickNormal(int fila, int col);
    bool esMovimentValid(const Posicio& pos) const;
};

#endif
