#include "posicio.h"
#include <string>
#include <iostream>

using namespace std;

Posicio::Posicio(const string& posicio) : fila(0), columna(0)
{
    columna = posicio[0] - 'a';
    fila = 8 - (posicio[1] - '0');
}

string Posicio::toString() const
{
    char letra = 'a' + columna;
    char numero = '8' - fila;

    std::string resultado;
    resultado += letra;
    resultado += numero;

    return resultado;
}

bool Posicio::esValida() const
{
    if (fila < 0)
    {
        return false;
    }
    if (fila >= 8)
    {
        return false;
    }
    if (columna < 0)
    {
        return false;
    }
    if (columna >= 8)
    {
        return false;
    }
    return true;
}

bool Posicio::operator==(const Posicio& altra) const
{
    if (fila != altra.fila)
    {
        return false;
    }
    if (columna != altra.columna)
    {
        return false;
    }
    return true;
}

bool Posicio::operator!=(const Posicio& altra) const
{
    bool iguales = (*this == altra);
    if (iguales)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Posicio::operator<(const Posicio& altra) const
{
    if (fila < altra.fila)
    {
        return true;
    }
    else if (fila == altra.fila)
    {
        if (columna < altra.columna)
        {
            return true;
        }
    }
    return false;
}

ostream& operator<<(ostream& out, const Posicio& pos)
{
    std::string text = pos.toString();
    out << text;
    return out;
}
