#include "moviment.h"
#include "enums.h"

Moviment::Moviment()
{
    numPosicions = 0;
    numCaptures = 0;
    esValid = true;
    colorPeca = COLOR_BLANC;
}

Moviment::Moviment(ColorFitxa color)
{
    numPosicions = 0;
    numCaptures = 0;
    esValid = true;
    colorPeca = color;
}

void Moviment::getPosicioCami(int index, int& fila, int& columna) const
{
    if (index >= 0 && index < numPosicions)
    {
        fila = cami[index].getFila();
        columna = cami[index].getColumna();
    }
    else
    {
        fila = -1;
        columna = -1;
    }
}

void Moviment::afegirPosicio(int fila, int columna)
{
    if (numPosicions < 16)
    {
        cami[numPosicions] = Posicio(fila, columna);
        numPosicions++;
    }
}

int Moviment::getNumPosicions() const
{
    return numPosicions;
}

void Moviment::getPosicioCaptura(int index, int& fila, int& columna) const
{
    if (index >= 0 && index < numCaptures)
    {
        fila = pecesCapturades[index].getFila();
        columna = pecesCapturades[index].getColumna();
    }
    else
    {
        fila = -1;
        columna = -1;
    }
}

void Moviment::afegirCaptura(int fila, int columna)
{
    if (numCaptures < MAX_PECES)
    {
        pecesCapturades[numCaptures] = Posicio(fila, columna);
        numCaptures++;
    }
}

int Moviment::getNumCaptures() const
{
    return numCaptures;
}

bool Moviment::esMovimentValid() const
{
    return esValid;
}

void Moviment::setValidesa(bool valid)
{
    esValid = valid;
}

ColorFitxa Moviment::getColorPeca() const
{
    return colorPeca;
}

bool Moviment::esCaptura() const
{
    if (numCaptures > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Moviment::getPosicioInicial(int& fila, int& columna) const
{
    if (numPosicions > 0)
    {
        fila = cami[0].getFila();
        columna = cami[0].getColumna();
    }
    else
    {
        fila = -1;
        columna = -1;
    }
}

void Moviment::getPosicioFinal(int& fila, int& columna) const
{
    if (numPosicions > 0)
    {
        fila = cami[numPosicions - 1].getFila();
        columna = cami[numPosicions - 1].getColumna();
    }
    else
    {
        fila = -1;
        columna = -1;
    }
}