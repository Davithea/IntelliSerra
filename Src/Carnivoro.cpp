#include "Carnivoro.h"
#include <sstream>
#include <iomanip>

//Author: Leonardo Cracco
// Costruttore
ImpiantoCarnivoro::ImpiantoCarnivoro(int id, const string& nome, double tassoConsumo)
    : Impianto(id, nome, tassoConsumo, true, "carnivoro") {
    // Inizializza con modalità automatica
    // Le variabili prossimaAttivazione e prossimoSpegnimento sono inizializzate a 00:00
}

//Author: Leonardo Cracco
// Metodo che aggiorna lo stato dell'impianto tra due orari
bool ImpiantoCarnivoro::aggiorna(const Orario& orarioPrecedente, const Orario& orarioAttuale) {
    bool statoModificato = false;

    Orario tempOrario = orarioPrecedente;

    // Esegui cicli di accensione/spegnimento finché necessario
    while (true) {
        // Caso 1: accensione automatica
        if (!attivo && tempOrario < prossimaAttivazione && prossimaAttivazione <= orarioAttuale) {
            accendi(prossimaAttivazione);
            statoModificato = true;
            tempOrario = prossimaAttivazione;
        }
        // Caso 2: spegnimento automatico
        else if (attivo && tempOrario < prossimoSpegnimento && prossimoSpegnimento <= orarioAttuale) {
            spegni(prossimoSpegnimento);
            statoModificato = true;

            // Dopo lo spegnimento, rimuove le attivazioni automatiche programmate
            prossimaAttivazione = Orario();
            prossimoSpegnimento = Orario();

            tempOrario = prossimoSpegnimento;
        } else {
            break; // Se non c'è nessuna modifica da fare esce dal ciclo
        }
    }

    return statoModificato;
}

//Author: Leonardo Cracco
// Imposta un timer manuale per attivare e spegnere l’impianto
bool ImpiantoCarnivoro::impostaTimer(const Orario& oraInizio, const Orario& oraFine) {
    if (!modalitaAutomatica) {
        return false; // Timer non disponibile in modalità manuale
    }

    // Verifica che l'orario di fine sia dopo quello di inizio
    if (oraFine <= oraInizio) {
        return false;
    }

    // Impedisce timer troppo lunghi (oltre 12 ore)
    double durata = oraInizio.differenzaInOre(oraFine);
    if (durata > 12.0) {
        return false;
    }

    // Imposta i valori per la prossima attivazione e spegnimento
    prossimaAttivazione = oraInizio;
    prossimoSpegnimento = oraFine;

    return true;
}

//Author: Leonardo Cracco
// Rimuove eventuali timer impostati
bool ImpiantoCarnivoro::rimuoviTimer() {
    if (!modalitaAutomatica) {
        return false;
    }

    // Annulla le attivazioni automatiche
    prossimaAttivazione = Orario();
    prossimoSpegnimento = Orario();

    // Se l'impianto è acceso, lo spegne
    if (attivo) {
        Orario orarioCorrente; // Usa 00:00 come riferimento
        spegni(orarioCorrente);
    }

    return true;
}

//Author: Leonardo Cracco
// Ritorna una stringa con lo stato dell’impianto
string ImpiantoCarnivoro::stampaStato() const {
    stringstream ss;

    string stato = attivo ? "Attivo" : "Spento";
    ss << "[Carnivoro] " << nome << " (ID: " << to_string(id) << ") - Stato: " << stato
       << " | Consumo: " << fixed << setprecision(2) << consumoIdrico << " L";

    if (attivo) {
        ss << " | Ultima attivazione: " << ultimaAttivazione.toString();
    }

    return ss.str();
}

// Clona  l'oggetto impianto carnivoro
Impianto* ImpiantoCarnivoro::clone() const {
    return new ImpiantoCarnivoro(*this);
}

//Author: Leonardo Cracco
// Calcola il prossimo orario di attivazione automatica in base a un intervallo fisso
void ImpiantoCarnivoro::calcolaProssimaAttivazione(const Orario& orarioAttuale) {
    int intervalloMinuti = static_cast<int>(INTERVALLO_ORE * 60); // 1.5 ore → 90 minuti

    // Imposta la prossima attivazione aggiungendo l’intervallo all’orario attuale
    prossimaAttivazione = orarioAttuale;
    prossimaAttivazione.incrementa(intervalloMinuti);
}

//Author: Leonardo Cracco
// Accende l'impianto e imposta lo spegnimento automatico dopo 1.5 ore
bool ImpiantoCarnivoro::accendi(const Orario& orario) {
    bool acceso = Impianto::accendi(orario);

    if (acceso) {
        ultimaAttivazione = orario;
        prossimoSpegnimento = orario;
        prossimoSpegnimento.incrementa(90); // 90 minuti = 1.5 ore
    }

    return acceso;
}

//Author: Leonardo Cracco
// Ritorna la durata dell’attivazione automatica (in minuti)
int ImpiantoCarnivoro::getDurataAutomatica() const {
    return 90; // Durata automatica fissa: 1 ora e mezza
}