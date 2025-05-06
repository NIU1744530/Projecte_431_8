#ifndef MOVIMENT_H
#define MOVIMENT_H

#include "posicio.h"
#include "enums.h"

#define MAX_PECES 8

class Moviment
{
private:
    Posicio cami[16];
    Posicio pecesCapturades[MAX_PECES];
    bool esValid;
    ColorFitxa colorPeca;
    int numPosicions;
    int numCaptures;

public:
    Moviment();
    Moviment(ColorFitxa color);

    void getPosicioCami(int index, int& fila, int& columna) const;
    void afegirPosicio(int fila, int columna);
    int getNumPosicions() const;

    void getPosicioCaptura(int index, int& fila, int& columna) const;
    void afegirCaptura(int fila, int columna);
    int getNumCaptures() const;

    bool esMovimentValid() const;
    void setValidesa(bool valid);
    ColorFitxa getColorPeca() const;
    bool esCaptura() const;

    void getPosicioInicial(int& fila, int& columna) const;
    void getPosicioFinal(int& fila, int& columna) const;
};

#endif