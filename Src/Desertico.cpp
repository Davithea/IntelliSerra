//=== DESERTICO.CPP ===

#include "Desertico.h"
#include <sstream>
#include <iomanip>

//Costruttore dell'impianto Desertico
//Author: Pietro Stocchiero
ImpiantoDesertico::ImpiantoDesertico(int id, const string& nome, double tassoConsumo)
    : Impianto(id, nome, tassoConsumo, false, "desertico") {  // Modalità automatica = false
    // L'impianto desertico inizia spento
}

//Funzione per aggiornare lo stato dell'impianto in base all'orario attuale
//Author: Pietro Stocchiero
bool ImpiantoDesertico::aggiorna(const Orario& orarioPrecedente, const Orario& orarioAttuale) {
    if (prossimaAttivazione == Orario() || prossimoSpegnimento == Orario()) {
        return false;
    }

    bool statoModificato = false;

    // Attiva l'impianto se si entra nell'intervallo programmato
    if (!attivo && orarioPrecedente < prossimaAttivazione && prossimaAttivazione <= orarioAttuale) {
        attivo = true;
        ultimaAttivazione = prossimaAttivazione;
        statoModificato = true;
    }

    // Spegni l'impianto se si esce dall'intervallo programmato
    if (attivo && orarioPrecedente < prossimoSpegnimento && prossimoSpegnimento <= orarioAttuale) {
        spegni(prossimoSpegnimento);
        statoModificato = true;

        // Una volta spento, il timer viene rimosso automaticamente
        prossimaAttivazione = Orario();
        prossimoSpegnimento = Orario();
    }

    return statoModificato;
}

//Funzione per impostare il timer manuale dell'impianto (accensione/spegnimento)
//Author: Pietro Stocchiero
bool ImpiantoDesertico::impostaTimer(const Orario& oraInizio, const Orario& oraFine) {
    if (oraFine <= oraInizio) {
        return false;
    }

    prossimaAttivazione = oraInizio;
    prossimoSpegnimento = oraFine;

    return true;
}

//Funzione per disattivare il timer manuale dell'impianto
//Author: Pietro Stocchiero
bool ImpiantoDesertico::rimuoviTimer() {
    prossimaAttivazione = Orario();
    prossimoSpegnimento = Orario();

    return true;
}

//Funzione che restituisce una stringa con lo stato attuale dell'impianto
//Author: Pietro Stocchiero
string ImpiantoDesertico::stampaStato() const {
    stringstream ss;
    string stato = attivo ? "Attivo" : "Spento";

    ss << "[Desertico] " << nome << " (ID: " << id << ") - Stato: " << stato
    << " | Consumo: " << fixed << setprecision(2) << consumoIdrico << " L";

    return ss.str();
}

//Funzione per clonare l'oggetto impianto (usata per creare copie dinamiche)
//Author: Pietro Stocchiero
Impianto* ImpiantoDesertico::clone() const {
    return new ImpiantoDesertico(*this);
}