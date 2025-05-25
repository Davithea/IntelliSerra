#include "Alpino.h"
#include <sstream>
#include <iomanip>

// Costruttore della classe ImpiantoAlpino
ImpiantoAlpino::ImpiantoAlpino(int id, const string& nome, double tassoConsumo)
    : Impianto(id, nome, tassoConsumo, true, "alpino") {
    // L'impianto parte in modalità automatica
    // Le variabili prossimaAttivazione e prossimoSpegnimento vengono inizializzate a orario 00:00 di default
}

// Metodo che aggiorna lo stato dell'impianto in base al tempo passato Author: Leonardo Cracco
bool ImpiantoAlpino::aggiorna(const Orario& orarioPrecedente, const Orario& orarioAttuale) {
    bool statoModificato = false;  // Flag per capire se c'è stato un cambiamento

    Orario tempOrario = orarioPrecedente;  // Partiamo dall'orario precedente

    // Ciclo per gestire più eventi di attivazione/spegnimento in un intervallo di tempo
    while (true) {
        // Caso 1: Se l'impianto è spento e siamo arrivati o superati il momento di accenderlo
        if (!attivo && tempOrario < prossimaAttivazione && prossimaAttivazione <= orarioAttuale) {
            accendi(prossimaAttivazione);  // Accendo l'impianto
            statoModificato = true;        // Segno che lo stato è cambiato
            tempOrario = prossimaAttivazione;  // Aggiorno l'orario di riferimento
        }
        // Caso 2: Se l'impianto è acceso e siamo arrivati o superati il momento di spegnerlo
        else if (attivo && tempOrario < prossimoSpegnimento && prossimoSpegnimento <= orarioAttuale) {
            spegni(prossimoSpegnimento);  // Spengo l'impianto
            statoModificato = true;

            // Non programmo più nuove attivazioni automatiche (resetto gli orari)
            prossimaAttivazione = Orario();
            prossimoSpegnimento = Orario();

            tempOrario = prossimoSpegnimento;
        } else {
            // Se nessun evento da processare, esco dal ciclo
            break;
        }
    }

    return statoModificato;  // Ritorno se qualcosa è cambiato o no
}

//Author: Leonardo Cracco
// Imposta un timer automatico di accensione e spegnimento
bool ImpiantoAlpino::impostaTimer(const Orario& oraInizio, const Orario& oraFine) {
    if (!modalitaAutomatica) {
        return false;  // Se non siamo in modalità automatica, niente da fare
    }

    // Controllo che l'ora di fine sia dopo l'ora di inizio
    if (oraFine <= oraInizio) {
        return false;
    }

    // Calcolo la durata e controllo che non superi le 12 ore
    double durata = oraInizio.differenzaInOre(oraFine);
    if (durata > 12.0) {  // Limite massimo fissato per sicurezza
        return false;
    }

    // Imposto gli orari per la prossima attivazione e spegnimento
    prossimaAttivazione = oraInizio;
    prossimoSpegnimento = oraFine;

    return true;  // Timer impostato con successo
}

//Author: Leonardo Cracco
// Rimuove il timer automatico e spegne l'impianto se acceso
bool ImpiantoAlpino::rimuoviTimer() {
    if (!modalitaAutomatica) {
        return false;  // Se non in modalità automatica, non c'è timer da rimuovere
    }

    // Resetta gli orari di accensione e spegnimento
    prossimaAttivazione = Orario();
    prossimoSpegnimento = Orario();

    // Se l'impianto è acceso, lo spegniamo ora
    if (attivo) {
        Orario orarioCorrente;  // Orario di riferimento (default 00:00)
        spegni(orarioCorrente);
    }

    return true;
}

//Author Leonardo Cracco
// Stampa una stringa con lo stato dell'impianto
string ImpiantoAlpino::stampaStato() const {
    stringstream ss;

    // Stato base: nome, id, se è attivo o spento e consumo attuale
    string stato = attivo ? "Attivo" : "Spento";
    ss << "[Alpino] " << nome << " (ID: " << to_string(id) << ") - Stato: " << stato
       << " | Consumo: " << fixed << setprecision(2) << consumoIdrico << " L";

    // Se l'impianto è attivo, mostro anche l'ultima attivazione
    if (attivo) {
        ss << " | Ultima attivazione: " << ultimaAttivazione.toString();
    }

    return ss.str();
}

// Crea una copia esatta dell'impianto (utile per polimorfismo)
Impianto* ImpiantoAlpino::clone() const {
    return new ImpiantoAlpino(*this);
}

//Author: Leonardo Cracco
// Calcola la prossima attivazione automatica partendo dall'orario attuale
void ImpiantoAlpino::calcolaProssimaAttivazione(const Orario& orarioAttuale) {
    // L'attivazione avviene a intervalli regolari definiti da INTERVALLO_ORE

    // Converto l'intervallo in minuti
    int intervalloMinuti = static_cast<int>(INTERVALLO_ORE * 60);

    // Imposto la prossima attivazione aggiungendo l'intervallo all'orario attuale
    prossimaAttivazione = orarioAttuale;
    prossimaAttivazione.incrementa(intervalloMinuti);
}

//Author Leonardo Cracco
// Override del metodo accendi per aggiungere lo spegnimento automatico dopo un'ora
bool ImpiantoAlpino::accendi(const Orario& orario) {
    // Chiamo il metodo accendi della classe base
    bool acceso = Impianto::accendi(orario);

    if (acceso) {
        // Se l'impianto si accende, salvo l'orario e imposto lo spegnimento automatico dopo 60 minuti
        ultimaAttivazione = orario;
        prossimoSpegnimento = orario;
        prossimoSpegnimento.incrementa(60); // spegnimento automatico dopo 1 ora
    }

    return acceso;
}

//Author: Leonardo Cracco
// Restituisce la durata standard dell'attivazione automatica in minuti
int ImpiantoAlpino::getDurataAutomatica() const {
    return 60; // Per l'impianto alpino la durata automatica è 1 ora
}