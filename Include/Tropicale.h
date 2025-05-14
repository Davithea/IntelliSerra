#ifndef IMPIANTO_TROPICALE_H
#define IMPIANTO_TROPICALE_H

#include "Impianto.h"

class ImpiantoTropicale : public Impianto {
private:
    static constexpr double INTERVALLO_ORE = 2.5;  // Intervallo di attivazione in ore
    static constexpr double DURATA_ATTIVAZIONE_ORE = 0.5;  // Durata dell'attivazione in ore
    
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
};

#endif // IMPIANTO_TROPICALE_H