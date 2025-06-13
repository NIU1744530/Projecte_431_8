#ifndef TAULER_HPP
#define TAULER_HPP

#include "fitxa.h"
#include "posicio.h"
#include "moviment.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>


using namespace std;

#define N_FILES 8
#define N_COLUMNES 8
#define MAX_MOVIMENTS 50

class Tauler
{
public:
    Tauler();
    ~Tauler();

    void inicialitza(const string& nomFitxer);
    void actualitzaMovimentsValids();
    bool mouFitxa(const Posicio& origen, const Posicio& desti);
    vector<Posicio> getPosicionsPossibles(const Posicio& origen) const;
    string toString() const;

    void visualitza() const;
    Fitxa& getFitxa(const Posicio& pos) { return *m_tauler[pos.getFila()][pos.getColumna()]; } 
    int comptaFitxes(ColorFitxa color) const;
    ColorFitxa comprovaGuanyador() const;



private:
    Fitxa* m_tauler[N_FILES][N_COLUMNES];

    bool validaPosicio(const Posicio& posicio) const;
    bool esPosicioBuida(const Posicio& posicio) const;
    void actualitzaMovimentsFitxa(const Posicio& posicio);
    void buscaMovimentsCaptura(const Posicio& posicio, Fitxa& fitxa);
    void buscaCapturesRecursiu(const Posicio& posActual, ColorFitxa color, bool esDama, Moviment movimentActual, vector<Moviment>& movimentsCapturas);
    void eliminaFitxa(const Posicio& posicio);
    void mouFitxaInterna(const Posicio& origen, const Posicio& desti);
};

#endif
