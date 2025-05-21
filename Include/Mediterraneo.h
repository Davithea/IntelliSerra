#ifndef IMPIANTO_MEDITERRANEO_H
#define IMPIANTO_MEDITERRANEO_H

#include "Impianto.h"
#include <random>

//Temperatura che inizia da 28. Ogni minuto scende di un valore random (0.01–0.05).
//Si accende se è sotto i 25. Quando è acceso, ogni ora aumenta di un valore random (0.75-1), fino a 28.

class ImpiantoMediterraneo : public Impianto {
    static constexpr double INTERVALLO_ORE_STANDARD = 2.5;  // Intervallo fisso di attivazione (esempio)
    static constexpr double DURATA_ATTIVAZIONE_ORE = 0.5;   // Durata dell'attivazione in ore

    Orario prossimaAttivazione;   // Orario della prossima attivazione programmata
    Orario prossimoSpegnimento;   // Orario del prossimo spegnimento programmato
    double temperatura = 28.0;    // Inizializza a 28°C
    std::default_random_engine generatore;

public:
    ImpiantoMediterraneo(int id, const std::string& nome, bool _unused = false, double tassoConsumo = 4.0);

    bool aggiorna(const Orario& orarioPrecedente, const Orario& orarioAttuale) override;
    bool impostaTimer(const Orario& oraInizio, const Orario& oraFine) override;
    bool rimuoviTimer() override;
    std::string stampaStato() const override;
    Impianto* clone() const override;
};

#endif // IMPIANTO_MEDITERRANEO_H
