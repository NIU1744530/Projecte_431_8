#include "tauler.hpp"
#include "GraphicManager.h"
#include "info_joc.hpp"
#include <string>
using namespace std;


Tauler::Tauler()
{
    for (int i = 0; i < N_FILES; i++) 
    {
        for (int j = 0; j < N_COLUMNES; j++) 
        {
            m_tauler[i][j] = nullptr;
        }
    }
}

Tauler::~Tauler() 
{
    for (int i = 0; i < N_FILES; ++i) 
    {
        for (int j = 0; j < N_COLUMNES; ++j)
        {
            delete m_tauler[i][j];
        }
    }
        
}

void Tauler::inicialitza(const string& nomFitxer)
{
    fstream fitxer(nomFitxer);
    if (fitxer.is_open())
    {
        for (int i = 0; i < N_FILES; i++)
        {
            for (int j = 0; j < N_COLUMNES; j++)
            {
                delete m_tauler[i][j];         
                m_tauler[i][j] = new Fitxa();
            }
        }

        char tipus;
        char columnaChar;
        int filaNum;

        while (fitxer >> tipus >> columnaChar >> filaNum)
        {
            int fila = 8 - filaNum;
            int columna = columnaChar - 'a';

            TipusFitxa tipusFitxa = TIPUS_NORMAL;
            ColorFitxa colorFitxa = COLOR_EMPTY;

            if (tipus == 'O')
            {
                tipusFitxa = TIPUS_NORMAL;
                colorFitxa = COLOR_BLANC;
            }
            else if (tipus == 'X')
            {
                tipusFitxa = TIPUS_NORMAL;
                colorFitxa = COLOR_NEGRE;
            }
            else if (tipus == 'D')
            {
                tipusFitxa = TIPUS_DAMA;
                colorFitxa = COLOR_BLANC;
            }
            else if (tipus == 'R')
            {
                tipusFitxa = TIPUS_DAMA;
                colorFitxa = COLOR_NEGRE;
            }
            delete m_tauler[fila][columna];
            m_tauler[fila][columna] = new Fitxa(tipusFitxa, colorFitxa);      
        }

        fitxer.close();
        actualitzaMovimentsValids();
    }
}

bool Tauler::validaPosicio(const Posicio& posicio) const
{
    if (posicio.getFila() < 0)
    {
        return false;
    }
    if (posicio.getFila() >= N_FILES)
    {
        return false;
    }
    if (posicio.getColumna() < 0)
    {
        return false;
    }
    if (posicio.getColumna() >= N_COLUMNES)
    {
        return false;
    }
    return true;
}

bool Tauler::esPosicioBuida(const Posicio& posicio) const
{
    int fila = posicio.getFila();
    int columna = posicio.getColumna();
    Fitxa* fitxa = m_tauler[fila][columna];
    bool esBuida = fitxa->esBuida();

    if (esBuida)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Tauler::actualitzaMovimentsValids()
{
    for (int i = 0; i < N_FILES; i++)
    {
        for (int j = 0; j < N_COLUMNES; j++)
        {
            Posicio pos(i, j);
            actualitzaMovimentsFitxa(pos);
        }
    }
}

void Tauler::actualitzaMovimentsFitxa(const Posicio& posicio)
{
    bool posicioValida = validaPosicio(posicio);

    if (posicioValida)
    {
        Fitxa* fitxa = m_tauler[posicio.getFila()][posicio.getColumna()];
        fitxa->inicialitzaMoviments();

        bool fitxaNoBuida = !fitxa->esBuida();

        if (fitxaNoBuida)
        {
            vector<Moviment> movimentsCaptura;
            buscaCapturesRecursiu(posicio, fitxa->getColor(), fitxa->esDama(), Moviment(fitxa->getColor()), movimentsCaptura);

            for (const Moviment& m : movimentsCaptura) 
            {
                fitxa->afegeixMovimentValid(m);
            }

            int direccio = fitxa->getDireccioMoviment();
            bool esDama = fitxa->esDama();

            if (!esDama)
            {
                int direccions[2][2] = { {direccio, 1}, {direccio, -1} };

                for (int i = 0; i < 2; i++)
                {
                    int novaFila = posicio.getFila() + direccions[i][0];
                    int novaCol = posicio.getColumna() + direccions[i][1];
                    Posicio novaPos(novaFila, novaCol);

                    bool posicioValidaIBuida = validaPosicio(novaPos) && esPosicioBuida(novaPos);

                    if (posicioValidaIBuida)
                    {
                        Moviment movNou(fitxa->getColor());
                        movNou.afegirPosicio(posicio.getFila(), posicio.getColumna());
                        movNou.afegirPosicio(novaFila, novaCol);
                        fitxa->afegeixMovimentValid(movNou);
                    }
                }
            }
            else
            {
                int direccions[4][2] = { {1,1}, {1,-1}, {-1,1}, {-1,-1} };

                for (int i = 0; i < 4; i++)
                {
                    int novaFila = posicio.getFila() + direccions[i][0];
                    int novaCol = posicio.getColumna() + direccions[i][1];
                    bool continuar = validaPosicio(Posicio(novaFila, novaCol));

                    while (continuar)
                    {
                        bool posicioBuida = esPosicioBuida(Posicio(novaFila, novaCol));

                        if (posicioBuida)
                        {
                            Moviment mov(fitxa->getColor());
                            mov.afegirPosicio(posicio.getFila(), posicio.getColumna());
                            mov.afegirPosicio(novaFila, novaCol);
                            fitxa->afegeixMovimentValid(mov);
                        }
                        else
                        {
                            continuar = false;
                        }

                        if (continuar)
                        {
                            novaFila += direccions[i][0];
                            novaCol += direccions[i][1];
                            continuar = validaPosicio(Posicio(novaFila, novaCol));
                        }
                    }
                }
            }
        }
    }
}

void Tauler::buscaCapturesRecursiu(const Posicio& posActual, ColorFitxa color, bool esDama, Moviment movimentActual, vector<Moviment>& movimentsCaptura)
{
    int direccioBase;

    if (color == COLOR_BLANC)
    {
        direccioBase = -1;
    }
    else
    {
        direccioBase = 1;
    }

    if (movimentActual.getNumPosicions() == 0) 
    {
    movimentActual.afegirPosicio(posActual.getFila(), posActual.getColumna());
    }

    const int direccions[4][2] = { {1,1}, {1,-1}, {-1,1}, {-1,-1} };

    for (int i = 0; i < 4; ++i)
    {
        int dFila = direccions[i][0];
        int dCol = direccions[i][1];

        if (!esDama && dFila == direccioBase)
        {
            Posicio posRival(posActual.getFila() + dFila, posActual.getColumna() + dCol);
            Posicio posBuit(posRival.getFila() + dFila, posRival.getColumna() + dCol);

            if (validaPosicio(posBuit) && validaPosicio(posRival) && !esPosicioBuida(posRival) && esPosicioBuida(posBuit))
            {
                Fitxa* fRival = m_tauler[posRival.getFila()][posRival.getColumna()];
                if (fRival->getColor() != color)
                {
                    bool jaCapturada = false;
                    for (int j = 0; j < movimentActual.getNumCaptures(); ++j)
                    {
                        int capF, capC;
                        movimentActual.getPosicioCaptura(j, capF, capC);
                        if (capF == posRival.getFila() && capC == posRival.getColumna())
                        {
                            jaCapturada = true;
                        }
                    }

                    if (!jaCapturada)
                    {
                        Moviment nou = movimentActual;
                        nou.afegirPosicio(posBuit.getFila(), posBuit.getColumna());
                        nou.afegirCaptura(posRival.getFila(), posRival.getColumna());

                        movimentsCaptura.push_back(nou);
                        buscaCapturesRecursiu(posBuit, color, esDama, nou, movimentsCaptura);
                    }
                }
            }
        }
        else if (esDama)
        {
            int fila = posActual.getFila() + dFila;
            int col = posActual.getColumna() + dCol;
            bool trobadaRival = false;
            int filaRival = -1, colRival = -1;

            while (validaPosicio(Posicio(fila, col)))
            {
                if (esPosicioBuida(Posicio(fila, col)))
                {
                    if (trobadaRival)
                    {
                        bool jaCapturada = false;
                        for (int j = 0; j < movimentActual.getNumCaptures(); ++j)
                        {
                            int capF, capC;
                            movimentActual.getPosicioCaptura(j, capF, capC);
                            if (capF == filaRival && capC == colRival) 
                            {
                                jaCapturada = true;
                            }
                        }

                        if (!jaCapturada)
                        {
                            Moviment nou = movimentActual;
                            nou.afegirPosicio(fila, col);
                            nou.afegirCaptura(filaRival, colRival);

                            movimentsCaptura.push_back(nou);
                            buscaCapturesRecursiu(Posicio(fila, col), color, esDama, nou, movimentsCaptura);
                        }
                    }
                    fila += dFila;
                    col += dCol;
                }
                else
                {
                    Fitxa* f = m_tauler[fila][col];
                    if (!trobadaRival && f->getColor() != color)
                    {
                        trobadaRival = true;
                        filaRival = fila;
                        colRival = col;
                        fila += dFila;
                        col += dCol;
                    }
                    else 
                    {
                        break;
                    }
                }
            }
        }
    }
}

void Tauler::buscaMovimentsCaptura(const Posicio& posicio, Fitxa& fitxa)
{
    vector<Moviment> movimentsCaptura;

    buscaCapturesRecursiu(posicio, fitxa.getColor(), fitxa.esDama(), Moviment(fitxa.getColor()), movimentsCaptura);

    for (const Moviment& m : movimentsCaptura)
    {
        fitxa.afegeixMovimentValid(m);
    }
}


void Tauler::eliminaFitxa(const Posicio& posicio)
{
    bool posicioValida = validaPosicio(posicio);
    if (posicioValida)
    {
        int fila = posicio.getFila();
        int columna = posicio.getColumna();
        delete m_tauler[fila][columna];               
        m_tauler[fila][columna] = new Fitxa();
    }
}

void Tauler::mouFitxaInterna(const Posicio& origen, const Posicio& desti)
{
    int fO = origen.getFila(), cO = origen.getColumna();
    int fD = desti.getFila(), cD = desti.getColumna();

    delete m_tauler[fD][cD];                                  
    m_tauler[fD][cD] = m_tauler[fO][cO];                           
    m_tauler[fO][cO] = new Fitxa();
}


bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti)
{
    if (!validaPosicio(origen) || !validaPosicio(desti)) 
    {
        return false;
    }

    Fitxa& fitxaOrigen = *m_tauler[origen.getFila()][origen.getColumna()];
    if (fitxaOrigen.esBuida()) 
    {
        return false;
    }

    ColorFitxa colorJugador = fitxaOrigen.getColor();
    actualitzaMovimentsValids();
    bool origenTeniaCaptura = fitxaOrigen.teCapturaObligatoria();

    vector<Posicio> pecesAmbCaptura;

    for (int i = 0; i < N_FILES; ++i)
    {
        for (int j = 0; j < N_COLUMNES; ++j)
        {
            if (i == origen.getFila() && j == origen.getColumna()) continue;

            Fitxa* f = m_tauler[i][j];
            if (f->getColor() == colorJugador && f->teCapturaObligatoria())
                pecesAmbCaptura.push_back(Posicio(i, j));
        }
    }

    bool movimentTrobat = false;
    bool esMovimentCaptura = false;

    for (int i = 0; i < fitxaOrigen.getNumMoviments(); ++i)
    {
        const Moviment& moviment = fitxaOrigen.getMoviment(i);
        int fFinal, cFinal;
        moviment.getPosicioFinal(fFinal, cFinal);

        if (Posicio(fFinal, cFinal) == desti)
        {
            esMovimentCaptura = moviment.esCaptura();

            for (int j = 0; j < moviment.getNumCaptures(); ++j)
            {
                int fCap, cCap;
                moviment.getPosicioCaptura(j, fCap, cCap);
                eliminaFitxa(Posicio(fCap, cCap));
            }

            mouFitxaInterna(origen, desti);

            Fitxa& novaFitxa = *m_tauler[desti.getFila()][desti.getColumna()];
            if ((novaFitxa.getColor() == COLOR_BLANC && desti.getFila() == 0) || (novaFitxa.getColor() == COLOR_NEGRE && desti.getFila() == N_FILES - 1))
            {
                novaFitxa.convertirEnDama();
            }

            movimentTrobat = true;
        }
    }

    if (movimentTrobat)
    {
        if (!esMovimentCaptura)
        {
            if (origenTeniaCaptura)
            {
                eliminaFitxa(desti);
            }
            else if (!pecesAmbCaptura.empty())
            {
                eliminaFitxa(pecesAmbCaptura.front());
            }
        }
        actualitzaMovimentsValids();
    }

    return movimentTrobat;
}



vector<Posicio> Tauler::getPosicionsPossibles(const Posicio& origen) const
{
    vector<Posicio> posicionsPossibles;

    if (validaPosicio(origen))
    {
        int fila = origen.getFila();
        int col = origen.getColumna();
        const Fitxa& fitxa = *m_tauler[fila][col];
        int numMoviments = fitxa.getNumMoviments();

        for (int i = 0; i < numMoviments; i++)
        {
            int finalFila, finalCol;
            fitxa.getMoviment(i).getPosicioFinal(finalFila, finalCol);
            posicionsPossibles.push_back(Posicio(finalFila, finalCol));
        }
    }

    return posicionsPossibles;
}



string Tauler::toString() const
{
    string resultat;

    for (int i = 0; i < N_FILES; i++)
    {
        resultat += to_string(8 - i) + ": ";

        for (int j = 0; j < N_COLUMNES; j++)
        {
            const Fitxa& f = *m_tauler[i][j];

            if (f.esBuida())
            {
                resultat += "_ ";
            }
            else
            {
                resultat += f.toString() + " ";
            }
        }

        resultat += "\n";
    }

    resultat += "  a b c d e f g h\n";
    return resultat;
}

void Tauler::visualitza() const 
{
    GraphicManager::getInstance()->drawSprite(GRAFIC_TAULER, POS_X_TAULER, POS_Y_TAULER);

    for (int i = 0; i < N_FILES; i++) 
    {
        for (int j = 0; j < N_COLUMNES; j++) 
        {
            if (!m_tauler[i][j]->esBuida()) 
            {
                int posX = POS_X_TAULER + CASELLA_INICIAL_X + (j * AMPLADA_CASELLA);
                int posY = POS_Y_TAULER + CASELLA_INICIAL_Y + (i * ALCADA_CASELLA);
                m_tauler[i][j]->visualitza(posX, posY);
            }
        }
    }
}

int Tauler::comptaFitxes(ColorFitxa color) const
{
    int n = 0;

    for (int i = 0; i < N_FILES; ++i)
    {
        for (int j = 0; j < N_COLUMNES; ++j)
        {
            if (m_tauler[i][j]->getColor() == color)
            {
                ++n;
            }
        }
    }

    return n;
}


ColorFitxa Tauler::comprovaGuanyador() const
{
    bool hiHaBlanques = false;
    bool hiHaNegres = false;
    bool potMoureBlanques = false;
    bool potMoureNegres = false;

    for (int i = 0; i < N_FILES; ++i)
    {
        for (int j = 0; j < N_COLUMNES; ++j)
        {
            Fitxa* p = m_tauler[i][j];

            if (p)
            {
                if (p->getColor() == COLOR_BLANC)
                {
                    hiHaBlanques = true;

                    if (p->getNumMoviments() > 0)
                    {
                        potMoureBlanques = true;
                    }
                }
                else if (p->getColor() == COLOR_NEGRE)
                {
                    hiHaNegres = true;

                    if (p->getNumMoviments() > 0)
                    {
                        potMoureNegres = true;
                    }
                }
            }
        }
    }

    if (!hiHaBlanques || (!potMoureBlanques && hiHaBlanques))
    {
        return COLOR_NEGRE;
    }

    if (!hiHaNegres || (!potMoureNegres && hiHaNegres))
    {
        return COLOR_BLANC;
    }

    return COLOR_EMPTY;
}

