//
// Created by cracc on 14/05/2025.
//

#ifndef ALPINO_H
#define ALPINO_H

#include "Impianto.h"

class Alpina : public Impianto {
private:
    static constexpr double INTERVALLO_ORE = 4.0;  // Intervallo di attivazione in ore
    static constexpr double DURATA_ATTIVAZIONE_ORE = 0.25;  // Durata dell'attivazione in ore

    Orario prossimaAttivazione;  // Orario della prossima attivazione programmata
    Orario prossimoSpegnimento;  // Orario del prossimo spegnimento programmato

public:
    Alpina(int id, const std::string& nome, double tassoConsumo = 2.0);
    bool aggiorna(const Orario& orarioPrecedente, const Orario& orarioAttuale) override;
    bool impostaTimer(const Orario& oraInizio, const Orario& oraFine) override;
    bool rimuoviTimer() override;
    std::string stampaStato() const override;
    Impianto* clone() const override;
    void calcolaProssimaAttivazione(const Orario& orarioAttuale);
};

#endif // ALPINO_H