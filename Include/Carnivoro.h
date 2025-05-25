#ifndef IMPIANTO_CARNIVORO_H
#define IMPIANTO_CARNIVORO_H

#include "Impianto.h"

using namespace std;

//Author: Leonardo Cracco
// Classe  piante carnivore
class ImpiantoCarnivoro : public Impianto {
    static constexpr double INTERVALLO_ORE = 1.5;  // Tempo che deve passare tra un'attivazione automatica e l'altra (1.5 ore)
    static constexpr double DURATA_ATTIVAZIONE_ORE = 1.5;  // Durata automatica dell'attivazione (1.5 ore)

    Orario prossimaAttivazione;  // Orario in cui l'impianto verrà attivato automaticamente
    Orario prossimoSpegnimento;  // Orario in cui l'impianto verrà spento automaticamente

public:
    // Costruttore: inizializza l’impianto con id, nome e tasso di consumo
    ImpiantoCarnivoro(int id, const string& nome, double tassoConsumo = 5.0);

    // Metodo chiamato per aggiornare lo stato dell'impianto in base al tempo trascorso
    bool aggiorna(const Orario& orarioPrecedente, const Orario& orarioAttuale) override;

    // Imposta manualmente un timer per programmare accensione e spegnimento
    bool impostaTimer(const Orario& oraInizio, const Orario& oraFine) override;

    // Rimuove il timer e spegne l’impianto se era acceso
    bool rimuoviTimer() override;

    // Restituisce una descrizione testuale dello stato dell’impianto
    string stampaStato() const override;

    // Ritorna una copia dell’oggetto corrente (utile per operazioni polimorfiche)
    Impianto* clone() const override;

    // Calcola quando sarà la prossima attivazione automatica, a partire dall’orario attuale
    void calcolaProssimaAttivazione(const Orario& orarioAttuale);

    // Accende l’impianto e programma lo spegnimento automatico dopo 1.5 ore
    bool accendi(const Orario& orario) override;

    // Ritorna la durata automatica in minuti dell'attivazione
    int getDurataAutomatica() const override;
};

#endif // IMPIANTO_CARNIVORO_H