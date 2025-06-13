#ifndef CUA_MOVIMENTS_H
#define CUA_MOVIMENTS_H

#include "moviment.h"
#include "info_joc.hpp"
#include <string>
#include <fstream>

class CuaMoviments 
{
public:
    CuaMoviments();
    ~CuaMoviments();

    void inicialitza(ModeJoc mode, const string& nomFitxer);
    void afegeixMoviment(const Moviment& mov);
    bool extreuMoviment(Moviment& mov);
    bool buida() const { return m_cap == nullptr; }
    void finalitza();

private:
    struct Node 
    {
        Moviment data;
        Node* next;
        Node(const Moviment& m) : data(m), next(nullptr) {}
    };

    Node* m_cap;
    Node* m_cua;
    string m_nomFitxer;
    ModeJoc m_mode;

    void allibera();
    void afegeixAlFinal(const Moviment& mov);
    void carregaDeFitxer();
    void desaAFitxer() const;
};

#endif
