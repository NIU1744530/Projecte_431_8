#include "tauler.hpp"
#include <string>
using namespace std;

Tauler::Tauler()
{
    for (int i = 0; i < N_FILES; i++)
    {
        for (int j = 0; j < N_COLUMNES; j++)
        {
            m_tauler[i][j] = Fitxa();
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
                m_tauler[i][j] = Fitxa();   
            }
        }

        char tipus;
        char columnaChar;
        int filaNum;

        while (fitxer >> tipus >> columnaChar >> filaNum)
        {
            int fila = 8 - filaNum;
            int columna = columnaChar - 'a';

            TipusFitxa tipusFitxa;
            ColorFitxa colorFitxa;

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
            m_tauler[fila][columna] = Fitxa(tipusFitxa, colorFitxa);
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
    Fitxa fitxa = m_tauler[fila][columna];
    bool esBuida = fitxa.esBuida();

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
        Fitxa& fitxa = m_tauler[posicio.getFila()][posicio.getColumna()];
        fitxa.inicialitzaMoviments();

        bool fitxaNoBuida = !fitxa.esBuida();

        if (fitxaNoBuida)
        {
            Moviment movimentsCaptura[MAX_MOVIMENTS];
            int numMovimentsCaptura = 0;
            buscaCapturesRecursiu(posicio, fitxa.getColor(), fitxa.esDama(), Moviment(fitxa.getColor()), movimentsCaptura, numMovimentsCaptura);

            bool hiHaCaptures = numMovimentsCaptura > 0;

            if (hiHaCaptures)
            {
                for (int i = 0; i < numMovimentsCaptura; i++)
                {
                    fitxa.afegeixMovimentValid(movimentsCaptura[i]);
                }
            }

            int direccio = fitxa.getDireccioMoviment();
            bool esDama = fitxa.esDama();

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
                        Moviment movNou(fitxa.getColor());
                        movNou.afegirPosicio(posicio.getFila(), posicio.getColumna());
                        movNou.afegirPosicio(novaFila, novaCol);
                        fitxa.afegeixMovimentValid(movNou);
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
                            Moviment mov(fitxa.getColor());
                            mov.afegirPosicio(posicio.getFila(), posicio.getColumna());
                            mov.afegirPosicio(novaFila, novaCol);
                            fitxa.afegeixMovimentValid(mov);
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

void Tauler::buscaCapturesRecursiu(const Posicio& posActual, ColorFitxa color, bool esDama, Moviment movimentActual, Moviment movimentsCaptura[], int& numMoviments)
{
    bool capturaTrobada = false;
    int direccioBase = 1;
    if (color == COLOR_BLANC)
    {
        direccioBase = -1;
    }

    if (movimentActual.getNumPosicions() == 0)
    {
        movimentActual.afegirPosicio(posActual.getFila(), posActual.getColumna());
    }

    int direccions[4][2] = { {1,1}, {1,-1}, {-1,1}, {-1,-1} };

    for (int i = 0; i < 4; i++)
    {
        int dFila = direccions[i][0];
        int dCol = direccions[i][1];

        if (esDama || dFila == direccioBase)
        {
            int fila = posActual.getFila() + dFila;
            int col = posActual.getColumna() + dCol;

            bool trobadaFitxaEnemiga = false;
            bool sortir = false;
            int filaEnemiga = -1, colEnemiga = -1;

            while (validaPosicio(Posicio(fila, col)) && !sortir)
            {
                if (esPosicioBuida(Posicio(fila, col)))
                {
                    if (trobadaFitxaEnemiga)
                    {
                        bool jaCapturada = false;
                        for (int j = 0; j < movimentActual.getNumCaptures(); j++)
                        {
                            int capFila, capCol;
                            movimentActual.getPosicioCaptura(j, capFila, capCol);
                            if (capFila == filaEnemiga && capCol == colEnemiga)
                            {
                                jaCapturada = true;
                            }
                        }

                        if (!jaCapturada)
                        {
                            capturaTrobada = true;
                            Moviment nouMoviment = movimentActual;
                            nouMoviment.afegirPosicio(fila, col);
                            nouMoviment.afegirCaptura(filaEnemiga, colEnemiga);

                            if (nouMoviment.getNumPosicions() > 1)
                            {
                                movimentsCaptura[numMoviments] = nouMoviment;
                                numMoviments++;
                            }

                            buscaCapturesRecursiu(Posicio(fila, col), color, esDama, nouMoviment, movimentsCaptura, numMoviments);

                            if (!esDama)
                            {
                                sortir = true;
                            }
                        }
                    }

                    fila += dFila;
                    col += dCol;
                }
                else
                {
                    Fitxa f = m_tauler[fila][col];
                    if (!trobadaFitxaEnemiga && f.getColor() != color)
                    {
                        trobadaFitxaEnemiga = true;
                        filaEnemiga = fila;
                        colEnemiga = col;
                        fila += dFila;
                        col += dCol;
                    }
                    else
                    {
                        sortir = true;
                    }
                }
            }
        }
    }
}


void Tauler::buscaMovimentsCaptura(const Posicio& posicio, Fitxa& fitxa)
{
    Moviment movimentsCaptura[MAX_MOVIMENTS];
    int numMoviments = 0;

    buscaCapturesRecursiu(posicio, fitxa.getColor(), fitxa.esDama(), Moviment(fitxa.getColor()), movimentsCaptura, numMoviments);

    for (int i = 0; i < numMoviments; i++)
    {
        fitxa.afegeixMovimentValid(movimentsCaptura[i]);
    }
}

void Tauler::eliminaFitxa(const Posicio& posicio)
{
    bool posicioValida = validaPosicio(posicio);
    if (posicioValida)
    {
        int fila = posicio.getFila();
        int columna = posicio.getColumna();
        m_tauler[fila][columna] = Fitxa();
    }
}

void Tauler::mouFitxaInterna(const Posicio& origen, const Posicio& desti)
{
    int filaOrigen = origen.getFila();
    int colOrigen = origen.getColumna();
    int filaDesti = desti.getFila();
    int colDesti = desti.getColumna();

    m_tauler[filaDesti][colDesti] = m_tauler[filaOrigen][colOrigen];
    m_tauler[filaOrigen][colOrigen] = Fitxa();
}


bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti)
{
    if (!validaPosicio(origen) || !validaPosicio(desti))
    {
        return false;
    }

    Fitxa& fitxaOrigen = m_tauler[origen.getFila()][origen.getColumna()];
    if (fitxaOrigen.esBuida())
    {
        return false;
    }

    ColorFitxa colorJugador = fitxaOrigen.getColor();
    actualitzaMovimentsValids();
    bool origenTeniaCaptura = fitxaOrigen.teCapturaObligatoria();

    
    Posicio pecesAmbCaptura[MAX_MOVIMENTS];
    int nPecesAmbCaptura = 0;

    for (int i = 0; i < N_FILES; ++i)
    {
        for (int j = 0; j < N_COLUMNES; ++j)
        {
            if (i == origen.getFila() && j == origen.getColumna()) continue;

            const Fitxa& f = m_tauler[i][j];
            if (f.getColor() == colorJugador && f.teCapturaObligatoria())
                pecesAmbCaptura[nPecesAmbCaptura++] = Posicio(i, j);
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

            Fitxa& novaFitxa = m_tauler[desti.getFila()][desti.getColumna()];
            if ((novaFitxa.getColor() == COLOR_BLANC && desti.getFila() == 0) ||
                (novaFitxa.getColor() == COLOR_NEGRE && desti.getFila() == N_FILES - 1))
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

            
            for (int i = 0; i < nPecesAmbCaptura; ++i)
            {
                eliminaFitxa(pecesAmbCaptura[i]);
            }
        }
        actualitzaMovimentsValids();
    }
    return movimentTrobat;
}



void Tauler::getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]) const
{
    nPosicions = 0;

    if (validaPosicio(origen))
    {
        int fila = origen.getFila();
        int col = origen.getColumna();
        const Fitxa& fitxa = m_tauler[fila][col];
        int numMoviments = fitxa.getNumMoviments();

        for (int i = 0; i < numMoviments; i++)
        {
            int finalFila, finalCol;
            fitxa.getMoviment(i).getPosicioFinal(finalFila, finalCol);
            posicionsPossibles[nPosicions] = Posicio(finalFila, finalCol);
            nPosicions++;
        }
    }
}

string Tauler::toString() const
{
    string resultat;

    for (int i = 0; i < N_FILES; i++)
    {
        resultat += to_string(8 - i) + ": ";

        for (int j = 0; j < N_COLUMNES; j++)
        {
            const Fitxa& f = m_tauler[i][j];

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