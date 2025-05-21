#ifndef SERRA_H
#define SERRA_H

#include <vector>
#include <string>
#include "Impianto.h"
#include "Orario.h"

using namespace std;

class Serra {
    vector<Impianto*> impianti;  //Vettore di puntatori agli impianti che sono presenti nella serra
    Orario orarioAttuale;        //Orario attuale della serra
    int prossimoId;              //ID da assegnare al prossimo impianto
    size_t trovaIndiceImpianto(int id) const;
    size_t trovaIndiceImpiantoDaNome(const string& nome) const;

public:
    Serra();    //Costruttore di default per la serra
    ~Serra();   //Distruttore per la serra

    //Per commenti relativi alle funzioni della classe Serra vedi Serra.cpp
    int aggiungiImpianto(Impianto* impianto);
    bool rimuoviImpianto(int id);
    bool rimuoviImpiantoDaNome(const string& nome);
    bool accendiImpianto(const string& nome);
    bool spegniImpianto(const string& nome);
    bool impostaTimer(const string& nome, const Orario& oraInizio, const Orario& oraFine);
    bool rimuoviTimer(const string& nome);
    vector<string> impostaOrario(const Orario& nuovoOrario);
    Orario getOrarioAttuale() const;
    vector<string> resetOrario();
    vector<string> resetTimer();
    vector<string> resetAll();
    vector<string> mostraStatoImpianti() const;
    string mostraStatoImpianto(const string& nome) const;
    double getConsumoIdricoTotale() const;
    size_t getNumeroImpianti() const;
    bool esisteImpianto(const string& nome) const;
};

#endif //SERRA_H