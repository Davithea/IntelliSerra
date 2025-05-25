//=== DESERTICO.H ===

#ifndef IMPIANTO_DESERTICO_H
#define IMPIANTO_DESERTICO_H

#include "Impianto.h"

using namespace std;

//Definizione della classe ImpiantoDesertico, derivata da Impianto
//Author: Pietro Stocchiero
class ImpiantoDesertico : public Impianto {
    Orario prossimaAttivazione;   //Orario della prossima attivazione impostata manualmente
    Orario prossimoSpegnimento;   //Orario del prossimo spegnimento impostato manualmente

public:
    //Per commenti relativi alle funzioni della classe Desertico vedi Desertico.cpp
    ImpiantoDesertico(int id, const std::string& nome, double tassoConsumo = 2.0);
    bool aggiorna(const Orario& orarioPrecedente, const Orario& orarioAttuale) override;
    bool impostaTimer(const Orario& oraInizio, const Orario& oraFine) override;
    bool rimuoviTimer() override;
    string stampaStato() const override;
    Impianto* clone() const override;
};

#endif // IMPIANTO_DESERTICO_H