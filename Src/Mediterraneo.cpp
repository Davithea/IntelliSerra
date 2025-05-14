#include "./Mediterraneo.h"
#include <sstream>
#include <iomanip>
#include <cmath>

ImpiantoMediterraneo::ImpiantoMediterraneo(int id, const std::string& nome, bool _isWeekend, double tassoConsumo)
    : Impianto(id, nome, tassoConsumo, true), isWeekend(_isWeekend) {
    // Inizializza con modalità automatica
}

bool ImpiantoMediterraneo::aggiorna(const Orario& orarioPrecedente, const Orario& orarioAttuale) {
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

bool ImpiantoMediterraneo::impostaTimer(const Orario& oraInizio, const Orario& oraFine) {
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

bool ImpiantoMediterraneo::rimuoviTimer() {
    if (!modalitaAutomatica) {
        return false;
    }

    // Reimposta gli orari di attivazione/spegnimento
    prossimaAttivazione = Orario();
    prossimoSpegnimento = Orario();

    return true;
}

std::string ImpiantoMediterraneo::stampaStato() const {
    std::stringstream ss;

    // Prima chiamiamo la versione base
    ss << Impianto::stampaStato();

    // Aggiungiamo le informazioni specifiche dell'impianto mediterraneo
    if (modalitaAutomatica && prossimaAttivazione != Orario()) {
        ss << " - Prossima attivazione: " << prossimaAttivazione.toString();
        if (attivo) {
            ss << " - Prossimo spegnimento: " << prossimoSpegnimento.toString();
        }
    }

    // Aggiungiamo l'informazione sul weekend
    double intervalloCorrente = isWeekend ? INTERVALLO_ORE_WEEKEND : INTERVALLO_ORE_FERIALE;
    ss << " - Tipo: Mediterraneo (attivazione ogni " << intervalloCorrente << " ore)";
    ss << " - Modalità: " << (isWeekend ? "Weekend" : "Feriale");

    return ss.str();
}

Impianto* ImpiantoMediterraneo::clone() const {
    return new ImpiantoMediterraneo(*this);
}

void ImpiantoMediterraneo::calcolaProssimaAttivazione(const Orario& orarioAttuale) {
    // Calcola la prossima attivazione a partire dall'orario attuale
    // Per le piante mediterranee, l'intervallo dipende dal fatto che sia weekend o meno

    // Determina l'intervallo corretto (in base al weekend)
    double intervalloOre = isWeekend ? INTERVALLO_ORE_WEEKEND : INTERVALLO_ORE_FERIALE;

    // Converti l'intervallo in minuti
    int intervalloMinuti = static_cast<int>(intervalloOre * 60);

    // Crea un nuovo orario partendo dall'attuale e aggiungendo l'intervallo
    prossimaAttivazione = orarioAttuale;
    prossimaAttivazione.incrementa(intervalloMinuti);
}

void ImpiantoMediterraneo::setWeekend(bool _isWeekend) {
    // Se cambia la modalità weekend/feriale, ricalcola la prossima attivazione
    if (isWeekend != _isWeekend) {
        isWeekend = _isWeekend;

        // Solo se l'impianto è in funzione automatica e non è attivo
        if (modalitaAutomatica && !attivo && prossimaAttivazione != Orario()) {
            // Ricalcola la prossima attivazione in base al nuovo intervallo
            Orario orarioCorrente = prossimaAttivazione;
            prossimaAttivazione = Orario(); // Reset
            calcolaProssimaAttivazione(orarioCorrente);
        }
    }
}

bool ImpiantoMediterraneo::getWeekend() const {
    return isWeekend;
}