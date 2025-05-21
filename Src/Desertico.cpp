#include "Desertico.h"
#include <sstream>

ImpiantoDesertico::ImpiantoDesertico(int id, const std::string& nome, double tassoConsumo)
    : Impianto(id, nome, tassoConsumo, false) {  // Modalità automatica = false
    // L'impianto desertico inizia spento
}

bool ImpiantoDesertico::aggiorna(const Orario& orarioPrecedente, const Orario& orarioAttuale) {
    if (prossimaAttivazione == Orario() || prossimoSpegnimento == Orario()) {
        return false;
    }

    bool statoModificato = false;

    // Attiva l’impianto se si entra nell’intervallo
    if (!attivo && orarioPrecedente < prossimaAttivazione && prossimaAttivazione <= orarioAttuale) {
        attivo = true;
        ultimaAttivazione = prossimaAttivazione;
        statoModificato = true;
    }

    // Spegni l’impianto se si esce dall’intervallo
    if (attivo && orarioPrecedente < prossimoSpegnimento && prossimoSpegnimento <= orarioAttuale) {
        spegni(prossimoSpegnimento);
        statoModificato = true;

        // Una volta spento, il timer viene rimosso
        prossimaAttivazione = Orario();
        prossimoSpegnimento = Orario();
    }

    return statoModificato;
}

bool ImpiantoDesertico::impostaTimer(const Orario& oraInizio, const Orario& oraFine) {
    if (oraFine <= oraInizio) {
        return false;
    }

    prossimaAttivazione = oraInizio;
    prossimoSpegnimento = oraFine;

    return true;
}

bool ImpiantoDesertico::rimuoviTimer() {
    prossimaAttivazione = Orario();
    prossimoSpegnimento = Orario();

    return true;
}

std::string ImpiantoDesertico::stampaStato() const {
    std::stringstream ss;
    std::string stato = attivo ? "Attivo" : "Spento";

    ss << "[Desertico] " << nome << " (ID: " << id << ") - Stato: " << stato
       << " | Consumo: " << consumoIdrico << " L";

    if (modalitaAutomatica && prossimaAttivazione != Orario()) {
        ss << " - Prossima attivazione: " << prossimaAttivazione.toString();
        if (attivo) {
            ss << " - Prossimo spegnimento: " << prossimoSpegnimento.toString();
        }
    }

    ss << " - Modalità: Automatizzata";

    return ss.str();
}

Impianto* ImpiantoDesertico::clone() const {
    return new ImpiantoDesertico(*this);
}