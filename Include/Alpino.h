#ifndef IMPIANTO_ALPINO_H
#define IMPIANTO_ALPINO_H

#include "Impianto.h"

//Author: Leonardo Cracco
// Classe che rappresenta un impianto per piante alpine
class ImpiantoAlpino : public Impianto {
    static constexpr double INTERVALLO_ORE = 1;  // Ogni ora l'impianto può essere riattivato automaticamente
    static constexpr double DURATA_ATTIVAZIONE_ORE = 1;  // L'attivazione automatica dura 1 ora

    Orario prossimaAttivazione;  // Prossimo orario in cui l’impianto si attiverà automaticamente
    Orario prossimoSpegnimento;  // Orario in cui l’impianto verrà spento automaticamente

public:
    // Costruttore: inizializza l’impianto con id, nome e tasso di consumo idrico
    ImpiantoAlpino(int id, const string& nome, double tassoConsumo = 5.0);

    //
    // Metodo chiamato ad ogni aggiornamento temporale per attivare/spegnere l’impianto se serve
    bool aggiorna(const Orario& orarioPrecedente, const Orario& orarioAttuale) override;

    // Imposta manualmente un timer per attivazione e spegnimento automatici
    bool impostaTimer(const Orario& oraInizio, const Orario& oraFine) override;

    // Rimuove qualsiasi timer attivo e spegne l’impianto se è acceso
    bool rimuoviTimer() override;

    // Restituisce una stringa che rappresenta lo stato attuale dell’impianto
    string stampaStato() const override;

    // Ritorna una copia dell’oggetto corrente (per operazioni polimorfiche)
    Impianto* clone() const override;

    // Calcola la prossima attivazione automatica a partire dall’orario attuale
    void calcolaProssimaAttivazione(const Orario& orarioAttuale);

    // Accende l’impianto e imposta lo spegnimento automatico dopo 1 ora
    bool accendi(const Orario& orario) override;

    // Restituisce la durata in minuti dell’attivazione automatica
    int getDurataAutomatica() const override;
};

#endif // IMPIANTO_ALPINO_H