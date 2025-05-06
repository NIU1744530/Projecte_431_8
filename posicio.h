#ifndef POSICIO_H
#define POSICIO_H

#include <iostream>
#include <string>

using namespace std;

class Posicio
{
private:
    int fila;
    int columna;

public:
    Posicio() : fila(0), columna(0) {}
    Posicio(int f, int c) : fila(f), columna(c) {}
    Posicio(const string& posicio);

    int getFila() const { return fila; }
    int getColumna() const { return columna; }
    void setFila(int f) { fila = f; }
    void setColumna(int c) { columna = c; }

    string toString() const;
    bool esValida() const;

    bool operator==(const Posicio& altra) const;
    bool operator!=(const Posicio& altra) const;
    bool operator<(const Posicio& altra) const;
};

ostream& operator<<(ostream& out, const Posicio& pos);

#endif
