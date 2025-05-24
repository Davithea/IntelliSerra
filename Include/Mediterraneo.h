#ifndef IMPIANTO_MEDITERRANEO_H
#define IMPIANTO_MEDITERRANEO_H

#include "Impianto.h"
#include <random>

//Definizione della classe ImpiantoMediterraneo, derivata da Impianto
//Author: Davide Gastaldello
class ImpiantoMediterraneo : public Impianto {
    Orario prossimaAttivazione;   //Orario in cui l’impianto si attiverà automaticamente --> non implementato nel mio caso
    Orario prossimoSpegnimento;   //Orario in cui l’impianto si spegnerà automaticamente --> non implementato nel mio caso
    double temperatura = 28.0;    //Temperatura iniziale impostata a 28°C
    default_random_engine generatore; //Generatore casuale per la variazione della temperatura
public:
    //Per commenti relativi alle funzioni della classe Mediterraneo vedi Mediterraneo.cpp
    ImpiantoMediterraneo(int id, const string& nome, double tassoConsumo = 4.0);
    bool aggiorna(const Orario& orarioPrecedente, const Orario& orarioAttuale) override;
    bool impostaTimer(const Orario& oraInizio, const Orario& oraFine) override;
    bool rimuoviTimer() override;
    string stampaStato() const override;
    Impianto* clone() const override;
};

#endif //IMPIANTO_MEDITERRANEO_H
