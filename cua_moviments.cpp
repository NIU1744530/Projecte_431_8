#include "cua_moviments.h"
#include <iostream>
using namespace std;


CuaMoviments::CuaMoviments() : m_cap(nullptr), m_cua(nullptr), m_mode(MODE_JOC_NONE) { ; }

CuaMoviments::~CuaMoviments()
{
    finalitza();
    allibera();
}

void CuaMoviments::afegeixAlFinal(const Moviment& mov)
{
    Node* nou = new Node(mov);
    if (!m_cap)
    {
        m_cap = m_cua = nou;
    }
    else
    {
        m_cua->next = nou;
        m_cua = nou;
    }
}

void CuaMoviments::allibera()
{
    while (m_cap)
    {
        Node* tmp = m_cap;
        m_cap = m_cap->next;
        delete tmp;
    }
    m_cua = nullptr;
}

void CuaMoviments::carregaDeFitxer()
{
    ifstream fitxerEntrada;
    fitxerEntrada.open(m_nomFitxer);

    if (fitxerEntrada)
    {
        string origen, desti;
        while (fitxerEntrada >> origen >> desti)
        {
            if (origen.size() == 2 && desti.size() == 2)
            {
                Moviment mov;
                mov.afegirPosicio(8 - (origen[1] - '0'), origen[0] - 'a');
                mov.afegirPosicio(8 - (desti[1] - '0'), desti[0] - 'a');
                afegeixAlFinal(mov);
            }
        }
    }
}



void CuaMoviments::desaAFitxer() const
{
    if (m_mode == MODE_JOC_NORMAL && !buida())
    {
        ofstream out(m_nomFitxer);

        if (out.is_open())
        {
            Node* n = m_cap;
            while (n != nullptr)
            {
                int fi, ci, ff, cf;
                n->data.getPosicioInicial(fi, ci);
                n->data.getPosicioFinal(ff, cf);

                string origen;
                origen += char('a' + ci);
                origen += char('8' - fi);

                string desti;
                desti += char('a' + cf);
                desti += char('8' - ff);

                out << origen << ' ' << desti << '\n';

                n = n->next;
            }
        }
    }
}




void CuaMoviments::inicialitza(ModeJoc mode, const std::string& nomFitxer)
{
    finalitza();
    allibera();
    m_mode = mode;
    m_nomFitxer = nomFitxer;
    if (m_mode == MODE_JOC_REPLAY)
    {
        carregaDeFitxer();
    }   
}

void CuaMoviments::afegeixMoviment(const Moviment& mov)
{
    if (m_mode == MODE_JOC_NORMAL)
    {
        afegeixAlFinal(mov);
        desaAFitxer();
    }
}


bool CuaMoviments::extreuMoviment(Moviment& mov)
{
    if (buida())
    {
        return false;
    }

    Node* tmp = m_cap;
    mov = m_cap->data;
    m_cap = m_cap->next;

    if (!m_cap)
    {
        m_cap = nullptr;
    }

    delete tmp;
    return true;
}

void CuaMoviments::finalitza()
{
    desaAFitxer();
}
