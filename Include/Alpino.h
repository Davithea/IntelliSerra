#ifndef IMPIANTO_ALPINO_H
#define IMPIANTO_ALPINO_H

#include "Impianto.h"

class ImpiantoAlpino : public Impianto {
    static constexpr double INTERVALLO_ORE = 1;  // Intervallo di attivazione in ore
    static constexpr double DURATA_ATTIVAZIONE_ORE = 1;  // Durata dell'attivazione modificata a 2.5 ore

    Orario prossimaAttivazione;  // Orario della prossima attivazione programmata
    Orario prossimoSpegnimento;  // Orario del prossimo spegnimento programmato

public:
    ImpiantoAlpino(int id, const string& nome, double tassoConsumo = 5.0);
    bool aggiorna(const Orario& orarioPrecedente, const Orario& orarioAttuale) override;
    bool impostaTimer(const Orario& oraInizio, const Orario& oraFine) override;
    bool rimuoviTimer() override;
    string stampaStato() const override;
    Impianto* clone() const override;
    void calcolaProssimaAttivazione(const Orario& orarioAttuale);

    // Override del metodo accendi per impostare lo spegnimento automatico dopo 2.5 ore
    bool accendi(const Orario& orario) override;

    int getDurataAutomatica() const override;
};

#endif // IMPIANTO_ALPINO_H