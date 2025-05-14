#ifndef IMPIANTO_MEDITERRANEO_H
#define IMPIANTO_MEDITERRANEO_H

#include "Impianto.h"
#include <vector>

//Temperatura. Inizia da 28. Ogni munto scende da un valore che va da 0.1-0.5 (random). Si accende se è sotto i 25
//Quando è acceso, ogni ora aumenta da 0.75 a 1 fino a che ritorna sopra i 28

class ImpiantoMediterraneo : public Impianto {
private:
    static constexpr double INTERVALLO_ORE_FERIALE = 2.0;  // Intervallo di attivazione nei giorni feriali
    static constexpr double INTERVALLO_ORE_WEEKEND = 3.0;  // Intervallo di attivazione nel weekend
    static constexpr double DURATA_ATTIVAZIONE_ORE = 0.5;  // Durata dell'attivazione in ore

    Orario prossimaAttivazione;  // Orario della prossima attivazione programmata
    Orario prossimoSpegnimento;  // Orario del prossimo spegnimento programmato
    bool isWeekend;              // Flag che indica se siamo nel weekend (simulato per il progetto)

public:
    ImpiantoMediterraneo(int id, const std::string& nome, bool _isWeekend = false, double tassoConsumo = 4.0);
    bool aggiorna(const Orario& orarioPrecedente, const Orario& orarioAttuale) override;
    bool impostaTimer(const Orario& oraInizio, const Orario& oraFine) override;
    bool rimuoviTimer() override;
    std::string stampaStato() const override;
    Impianto* clone() const override;
    void calcolaProssimaAttivazione(const Orario& orarioAttuale);
    void setWeekend(bool _isWeekend);
    bool getWeekend() const;
};

#endif // IMPIANTO_MEDITERRANEO_H
