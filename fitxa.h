#ifndef FITXA_H
#define FITXA_H

#include "posicio.h"
#include "moviment.h"
#include "enums.h"
#include <string>

using namespace std;

class Fitxa
{
public:
    Fitxa() : m_tipus(TIPUS_EMPTY), m_color(COLOR_EMPTY), m_numMoviments(0) {}

    Fitxa(TipusFitxa tipus, ColorFitxa color) : m_tipus(tipus), m_color(color), m_numMoviments(0) {}

    TipusFitxa getTipus() const { return m_tipus; }
    ColorFitxa getColor() const { return m_color; }
    void setTipus(TipusFitxa nouTipus) { m_tipus = nouTipus; }
    void setColor(ColorFitxa nouColor) { m_color = nouColor; }
    void convertirEnDama();
    bool esDama() const { return m_tipus == TIPUS_DAMA; }
    bool esBuida() const { return m_tipus == TIPUS_EMPTY; }

    bool potMoureEnDireccio(int fila, int columna) const;
    int getDireccioMoviment() const;
    void inicialitzaMoviments() { m_numMoviments = 0; }
    void afegeixMovimentValid(const Moviment& moviment);
    int getNumMoviments() const { return m_numMoviments; }
    const Moviment& getMoviment(int index) const;
    bool teCapturaObligatoria() const;

    string toString() const;

private:
    TipusFitxa m_tipus;
    ColorFitxa m_color;
    static const int MAX_MOVIMENTS = 16;
    Moviment m_moviments[MAX_MOVIMENTS];
    int m_numMoviments;
};

#endif
