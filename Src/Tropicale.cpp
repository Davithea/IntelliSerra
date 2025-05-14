#include "./Tropicale.h"
#include <sstream>
#include <iomanip>
#include <cmath>

ImpiantoTropicale::ImpiantoTropicale(int id, const std::string& nome, double tassoConsumo)
    : Impianto(id, nome, tassoConsumo, true) {
    // Inizializza con modalità automatica
}

bool ImpiantoTropicale::aggiorna(const Orario& orarioPrecedente, const Orario& orarioAttuale) {
    bool statoModificato = false;

    // Se è la prima volta che viene chiamato aggiorna, inizializza la prossima attivazione
    if (prossimaAttivazione == Orario()) {
        calcolaProssimaAttivazione(orarioPrecedente);
    }

    // Verifica se nell'intervallo di tempo tra orarioPrecedente e orarioAttuale
    // dobbiamo attivare o disattivare l'impianto

    // Caso 1: impianto attualmente spento e siamo arrivati al momento di accenderlo
    if (!attivo && orarioPrecedente < prossimaAttivazione && prossimaAttivazione <= orarioAttuale) {
        attivo = true;
        ultimaAttivazione = prossimaAttivazione;
        statoModificato = true;

        // Aggiorna anche il prossimo spegnimento
        prossimoSpegnimento = prossimaAttivazione;
        prossimoSpegnimento.incrementa(static_cast<int>(DURATA_ATTIVAZIONE_ORE * 60));
    }

    // Caso 2: impianto attualmente acceso e siamo arrivati al momento di spegnerlo
    if (attivo && orarioPrecedente < prossimoSpegnimento && prossimoSpegnimento <= orarioAttuale) {
        spegni(prossimoSpegnimento);
        statoModificato = true;

        // Calcola la prossima attivazione
        calcolaProssimaAttivazione(prossimoSpegnimento);
    }

    return statoModificato;
}

bool ImpiantoTropicale::impostaTimer(const Orario& oraInizio, const Orario& oraFine) {
    if (!modalitaAutomatica) {
        return false;
    }

    // Verifica che l'orario di fine sia successivo a quello di inizio
    if (oraFine <= oraInizio) {
        return false;
    }

    // Imposta la prossima attivazione e il prossimo spegnimento
    prossimaAttivazione = oraInizio;
    prossimoSpegnimento = oraFine;

    return true;
}

bool ImpiantoTropicale::rimuoviTimer() {
    if (!modalitaAutomatica) {
        return false;
    }

    // Reimposta gli orari di attivazione/spegnimento
    prossimaAttivazione = Orario();
    prossimoSpegnimento = Orario();

    return true;
}

std::string ImpiantoTropicale::stampaStato() const {
    std::stringstream ss;

    // Prima chiamiamo la versione base
    ss << Impianto::stampaStato();

    // Aggiungiamo le informazioni specifiche dell'impianto tropicale
    if (modalitaAutomatica && prossimaAttivazione != Orario()) {
        ss << " - Prossima attivazione: " << prossimaAttivazione.toString();
        if (attivo) {
            ss << " - Prossimo spegnimento: " << prossimoSpegnimento.toString();
        }
    }

    ss << " - Tipo: Tropicale (attivazione ogni " << INTERVALLO_ORE << " ore)";

    return ss.str();
}

Impianto* ImpiantoTropicale::clone() const {
    return new ImpiantoTropicale(*this);
}

void ImpiantoTropicale::calcolaProssimaAttivazione(const Orario& orarioAttuale) {
    // Calcola la prossima attivazione a partire dall'orario attuale
    // Per le piante tropicali, l'attivazione avviene ogni INTERVALLO_ORE

    // Converti l'intervallo in minuti
    int intervalloMinuti = static_cast<int>(INTERVALLO_ORE * 60);

    // Crea un nuovo orario partendo dall'attuale e aggiungendo l'intervallo
    prossimaAttivazione = orarioAttuale;
    prossimaAttivazione.incrementa(intervalloMinuti);
}
