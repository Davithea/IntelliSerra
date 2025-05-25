//=== TROPICALE.CPP ===

#include "Tropicale.h"
#include <sstream>
#include <iomanip>

//Costruttore dell'impianto Tropicale
//Author: Pietro Stocchiero
ImpiantoTropicale::ImpiantoTropicale(int id, const string& nome, double tassoConsumo)
    : Impianto(id, nome, tassoConsumo, true, "tropicale") {
    // Inizializza con modalità automatica
    // Le variabili prossimaAttivazione e prossimoSpegnimento sono inizializzate a 00:00
}

//Funzione per aggiornare lo stato dell'impianto in base all'orario attuale
//Author: Pietro Stocchiero
bool ImpiantoTropicale::aggiorna(const Orario& orarioPrecedente, const Orario& orarioAttuale) {
    bool statoModificato = false;

    // Gestione singolo ciclo attivazione-spegnimento
    Orario tempOrario = orarioPrecedente;

    while (true) {
        // Caso 1: accensione programmata
        if (!attivo && tempOrario < prossimaAttivazione && prossimaAttivazione <= orarioAttuale) {
            accendi(prossimaAttivazione);
            statoModificato = true;

            tempOrario = prossimaAttivazione;
        }
        // Caso 2: spegnimento programmato
        else if (attivo && tempOrario < prossimoSpegnimento && prossimoSpegnimento <= orarioAttuale) {
            spegni(prossimoSpegnimento);
            statoModificato = true;

            // Non programmare nuove attivazioni automatiche dopo lo spegnimento
            prossimaAttivazione = Orario();
            prossimoSpegnimento = Orario();

            tempOrario = prossimoSpegnimento;
        } else {
            break;
        }
    }

    return statoModificato;
}

//Funzione per impostare il timer automatico dell'impianto (accensione/spegnimento)
//Author: Pietro Stocchiero
bool ImpiantoTropicale::impostaTimer(const Orario& oraInizio, const Orario& oraFine) {
    if (!modalitaAutomatica) {
        return false;
    }

    // Verifica che l'orario di fine sia successivo a quello di inizio
    if (oraFine <= oraInizio) {
        return false;
    }

    // Verifica che la durata non sia eccessiva (limitata a 12 ore per sicurezza)
    double durata = oraInizio.differenzaInOre(oraFine);
    if (durata > 12.0) {
        return false;
    }

    // Imposta la prossima attivazione e il prossimo spegnimento
    prossimaAttivazione = oraInizio;
    prossimoSpegnimento = oraFine;

    return true;
}

//Funzione per disattivare il timer automatico dell'impianto
//Author: Pietro Stocchiero
bool ImpiantoTropicale::rimuoviTimer() {
    if (!modalitaAutomatica) {
        return false;
    }

    // Reimposta gli orari di attivazione/spegnimento
    prossimaAttivazione = Orario();
    prossimoSpegnimento = Orario();

    // Se l'impianto è attivo, lo spegniamo immediatamente
    if (attivo) {
        Orario orarioCorrente;  // Utilizziamo 00:00 come orario di riferimento
        spegni(orarioCorrente);
    }

    return true;
}

//Funzione che restituisce una stringa con lo stato attuale dell'impianto
//Author: Pietro Stocchiero
string ImpiantoTropicale::stampaStato() const {
    stringstream ss;

    // Prima parte: informazioni di base dell'impianto
    string stato = attivo ? "Attivo" : "Spento";
    ss << "[Tropicale] " << nome << " (ID: " << to_string(id) << ") - Stato: " << stato
       << " | Consumo: " << fixed << setprecision(2) << consumoIdrico << " L";

    // Aggiungiamo le informazioni specifiche dell'impianto tropicale
    if (attivo) {
        ss << " | Ultima attivazione: " << ultimaAttivazione.toString();
    }

    return ss.str();
}

//Funzione per clonare l'oggetto impianto (usata per creare copie dinamiche)
//Author: Pietro Stocchiero
Impianto* ImpiantoTropicale::clone() const {
    return new ImpiantoTropicale(*this);
}

//Funzione per calcolare la prossima attivazione automatica dell'impianto
//Author: Pietro Stocchiero
void ImpiantoTropicale::calcolaProssimaAttivazione(const Orario& orarioAttuale) {
    // Calcola la prossima attivazione a partire dall'orario attuale
    // Per le piante tropicali, l'attivazione avviene ogni INTERVALLO_ORE

    // Converti l'intervallo in minuti
    int intervalloMinuti = static_cast<int>(INTERVALLO_ORE * 60);

    // Crea un nuovo orario partendo dall'attuale e aggiungendo l'intervallo
    prossimaAttivazione = orarioAttuale;
    prossimaAttivazione.incrementa(intervalloMinuti);
}

//Override del metodo accendi per impostare lo spegnimento automatico dopo 2.5 ore
//Author: Pietro Stocchiero
bool ImpiantoTropicale::accendi(const Orario& orario) {
    // Chiamiamo il metodo accendi della classe base
    bool acceso = Impianto::accendi(orario);

    if (acceso) {
        // Impianto acceso con successo, impostiamo lo spegnimento automatico dopo 2.5 ore
        ultimaAttivazione = orario;
        prossimoSpegnimento = orario;
        prossimoSpegnimento.incrementa(150); // 2.5 ore = 150 minuti
    }

    return acceso;
}

//Funzione che restituisce la durata automatica di funzionamento dell'impianto tropicale
//Author: Pietro Stocchiero
int ImpiantoTropicale::getDurataAutomatica() const {
    return 150; // I tropicali funzionano per 2,5 ore in automatico
}