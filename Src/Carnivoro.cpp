#include "Carnivoro.h"
#include <sstream>
#include <iomanip>

ImpiantoCarnivoro::ImpiantoCarnivoro(int id, const std::string& nome, double tassoConsumo)
    : Impianto(id, nome, tassoConsumo, true) {
    // Inizializza con modalità automatica
    // Le variabili prossimaAttivazione e prossimoSpegnimento sono inizializzate a 00:00
}

bool ImpiantoCarnivoro::aggiorna(const Orario& orarioPrecedente, const Orario& orarioAttuale) {
    bool statoModificato = false;

    // Gestione singolo ciclo attivazione-spegnimento
    Orario tempOrario = orarioPrecedente;

    while (true) {
        // Caso 1: accensione
        if (!attivo && tempOrario < prossimaAttivazione && prossimaAttivazione <= orarioAttuale) {
            accendi(prossimaAttivazione);
            statoModificato = true;

            tempOrario = prossimaAttivazione;
        }
        // Caso 2: spegnimento
        else if (attivo && tempOrario < prossimoSpegnimento && prossimoSpegnimento <= orarioAttuale) {
            spegni(prossimoSpegnimento);
            statoModificato = true;

            // Non programmare nuove attivazioni automatiche!
            prossimaAttivazione = Orario();
            prossimoSpegnimento = Orario();

            tempOrario = prossimoSpegnimento;
        } else {
            break;
        }
    }

    return statoModificato;
}


bool ImpiantoCarnivoro::impostaTimer(const Orario& oraInizio, const Orario& oraFine) {
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

bool ImpiantoCarnivoro::rimuoviTimer() {
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

std::string ImpiantoCarnivoro::stampaStato() const {
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

Impianto* ImpiantoCarnivoro::clone() const {
    return new ImpiantoCarnivoro(*this);
}

void ImpiantoCarnivoro::calcolaProssimaAttivazione(const Orario& orarioAttuale) {
    // Calcola la prossima attivazione a partire dall'orario attuale
    // Per le piante tropicali, l'attivazione avviene ogni INTERVALLO_ORE

    // Converti l'intervallo in minuti
    int intervalloMinuti = static_cast<int>(INTERVALLO_ORE * 60);

    // Crea un nuovo orario partendo dall'attuale e aggiungendo l'intervallo
    prossimaAttivazione = orarioAttuale;
    prossimaAttivazione.incrementa(intervalloMinuti);
}

// Override del metodo accendi per impostare lo spegnimento automatico dopo 1 ora e mezza
bool ImpiantoCarnivoro::accendi(const Orario& orario) {
    // Chiamiamo il metodo accendi della classe base
    bool acceso = Impianto::accendi(orario);

    if (acceso) {
        // Impianto acceso con successo, impostiamo lo spegnimento automatico dopo 1.5 ore
        ultimaAttivazione = orario;
        prossimoSpegnimento = orario;
        prossimoSpegnimento.incrementa(90); // 1.5 ore = 90 minuti
    }

    return acceso;
}

int ImpiantoCarnivoro::getDurataAutomatica() const {
    return 90; // I carnivori funzionano per 1,5 ore in automatico
}