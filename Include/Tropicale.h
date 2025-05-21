#ifndef IMPIANTO_TROPICALE_H
#define IMPIANTO_TROPICALE_H

#include "Impianto.h"

class ImpiantoTropicale : public Impianto {
private:
    static constexpr double INTERVALLO_ORE = 2.5;  // Intervallo di attivazione in ore
    static constexpr double DURATA_ATTIVAZIONE_ORE = 2.5;  // Durata dell'attivazione modificata a 2.5 ore

    Orario prossimaAttivazione;  // Orario della prossima attivazione programmata
    Orario prossimoSpegnimento;  // Orario del prossimo spegnimento programmato

public:
    ImpiantoTropicale(int id, const std::string& nome, double tassoConsumo = 5.0);
    bool aggiorna(const Orario& orarioPrecedente, const Orario& orarioAttuale) override;
    bool impostaTimer(const Orario& oraInizio, const Orario& oraFine) override;
    bool rimuoviTimer() override;
    std::string stampaStato() const override;
    Impianto* clone() const override;
    void calcolaProssimaAttivazione(const Orario& orarioAttuale);

    // Override del metodo accendi per impostare lo spegnimento automatico dopo 2.5 ore
    bool accendi(const Orario& orario) override;

    int getDurataAutomatica() const override;
};

#endif // IMPIANTO_TROPICALE_H