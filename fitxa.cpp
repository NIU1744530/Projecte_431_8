#include "fitxa.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include "GraphicManager.h"


using namespace std;

int Fitxa::getDireccioMoviment() const
{
    if (m_color == COLOR_BLANC)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

void Fitxa::convertirEnDama()
{
    if (m_tipus == TIPUS_NORMAL)
    {
        m_tipus = TIPUS_DAMA;
    }
}

bool Fitxa::potMoureEnDireccio(int fila, int columna) const
{
    if (esBuida())
    {
        return false;
    }

    if (esDama())
    {
        return (abs(fila) == abs(columna));
    }
    else
    {
        return (fila == getDireccioMoviment() && abs(columna) == 1);
    }
}

void Fitxa::afegeixMovimentValid(const Moviment& moviment)
{
    if (m_numMoviments < MAX_MOVIMENTS)
    {
        m_moviments[m_numMoviments] = moviment;
        m_numMoviments++;
    }
}

const Moviment& Fitxa::getMoviment(int index) const
{
    static Moviment invalidMoviment;
    if (index < 0 || index >= m_numMoviments)
    {
        cout << "Moviment no valid";
        return invalidMoviment;
    }
    return m_moviments[index];
}

bool Fitxa::teCapturaObligatoria() const
{
    for (int i = 0; i < m_numMoviments; i++)
    {
        if (m_moviments[i].esCaptura())
        {
            return true;
        }
    }
    return false;
}

string Fitxa::toString() const
{
    if (esBuida())
    {
        return "_";
    }

    if (m_color == COLOR_BLANC)
    {
        if (esDama())
        {
            return "D";
        }
        else
        {
            return "O";
        }
    }
    else
    {
        if (esDama())
        {
            return "R";
        }
        else
        {
            return "X";
        }
    }
}

void Fitxa::visualitza(int posX, int posY) const
{
    if (!esBuida())
    {
        IMAGE_NAME graphic;

        if (m_color == COLOR_BLANC)
        {
            if (esDama())
            {
                graphic = GRAFIC_DAMA_BLANCA;
            }
            else
            {
                graphic = GRAFIC_FITXA_BLANCA;
            }
        }
        else
        {
            if (esDama())
            {
                graphic = GRAFIC_DAMA_NEGRA;
            }
            else
            {
                graphic = GRAFIC_FITXA_NEGRA;
            }
        }

        GraphicManager::getInstance()->drawSprite(graphic, posX, posY);
    }
}
