#include "Tropicale.h"
#include <sstream>
#include <iomanip>
#include <cmath>

ImpiantoTropicale::ImpiantoTropicale(int id, const std::string& nome, double tassoConsumo)
    : Impianto(id, nome, tassoConsumo, true) {
    // Inizializza con modalità automatica
    // Le variabili prossimaAttivazione e prossimoSpegnimento sono inizializzate a 00:00
}

bool ImpiantoTropicale::aggiorna(const Orario& orarioPrecedente, const Orario& orarioAttuale) {
    bool statoModificato = false;

    // Se è la prima volta che viene chiamato aggiorna o dopo un reset, inizializza la prossima attivazione
    if (prossimaAttivazione == Orario()) {
        calcolaProssimaAttivazione(orarioPrecedente);
    }

    // Verifica se nell'intervallo di tempo tra orarioPrecedente e orarioAttuale
    // dobbiamo attivare o disattivare l'impianto

    // Controlliamo se ci sono una o più attivazioni e spegnimenti nell'intervallo
    // Possono esserci diversi cicli di attivazione/spegnimento
    Orario tempOrario = orarioPrecedente;
    while (true) {
        // Caso 1: impianto attualmente spento e siamo arrivati al momento di accenderlo
        if (!attivo && tempOrario < prossimaAttivazione && prossimaAttivazione <= orarioAttuale) {
            attivo = true;
            ultimaAttivazione = prossimaAttivazione;
            statoModificato = true;

            // Aggiorna il prossimo spegnimento
            prossimoSpegnimento = prossimaAttivazione;
            prossimoSpegnimento.incrementa(static_cast<int>(DURATA_ATTIVAZIONE_ORE * 60));

            // Aggiorniamo tempOrario per il prossimo controllo
            tempOrario = prossimaAttivazione;
        }
        // Caso 2: impianto attualmente acceso e siamo arrivati al momento di spegnerlo
        else if (attivo && tempOrario < prossimoSpegnimento && prossimoSpegnimento <= orarioAttuale) {
            spegni(prossimoSpegnimento);
            statoModificato = true;

            // Calcola la prossima attivazione
            calcolaProssimaAttivazione(prossimoSpegnimento);

            // Aggiorniamo tempOrario per il prossimo controllo
            tempOrario = prossimoSpegnimento;
        }
        else {
            // Nessun altro evento da gestire nell'intervallo
            break;
        }
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

    // Verifica che la durata non sia eccessiva
    double durata = oraInizio.differenzaInOre(oraFine);
    if (durata > 12.0) {  // Limitiamo la durata a 12 ore per sicurezza
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

    // Se l'impianto è attivo, lo spegniamo
    if (attivo) {
        Orario orarioCorrente;  // Utilizziamo 00:00 come orario di riferimento
        spegni(orarioCorrente);
    }

    return true;
}

std::string ImpiantoTropicale::stampaStato() const {
    std::stringstream ss;

    // Prima parte: informazioni di base dell'impianto
    std::string stato = attivo ? "Attivo" : "Spento";
    ss << "[Tropicale] " << nome << " (ID: " << std::to_string(id) << ") - Stato: " << stato
       << " | Consumo: " << std::fixed << std::setprecision(2) << consumoIdrico << " L";

    // Aggiungiamo le informazioni specifiche dell'impianto tropicale
    if (attivo) {
        ss << " | Ultima attivazione: " << ultimaAttivazione.toString();
    }

    if (modalitaAutomatica && prossimaAttivazione != Orario()) {
        ss << " | Prossima attivazione: " << prossimaAttivazione.toString();
        if (attivo) {
            ss << " | Prossimo spegnimento: " << prossimoSpegnimento.toString();
        }
    }

    ss << " | Ciclo: " << INTERVALLO_ORE << "h ogni " << DURATA_ATTIVAZIONE_ORE << "h";

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