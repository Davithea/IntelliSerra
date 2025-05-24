#include "./Mediterraneo.h"
#include <sstream>
#include <iomanip>

//Costruttore dell'impianto Mediterraneo
//Author: Davide Gastaldello
ImpiantoMediterraneo::ImpiantoMediterraneo(int id, const string& nome, double tassoConsumo)
    : Impianto(id, nome, tassoConsumo, true, "mediterraneo"), generatore(random_device{}()) {}

//Funzione per aggiornare lo stato dell’impianto in base all’orario attuale
//Author: Davide Gastaldello
bool ImpiantoMediterraneo::aggiorna(const Orario& orarioPrecedente, const Orario& orarioAttuale) {
    bool statoModificato = false;

    int minutiTrascorsi = orarioPrecedente.differenzaInMinuti(orarioAttuale); //Calcolo minuti trascorsi tra i due orari
    if (minutiTrascorsi <= 0) return false;

    if (attivo) {
        //Se l’impianto è attivo aumento la temperatura con una variazione casuale
        uniform_real_distribution aumentoTemp(0.75, 1.0);
        double aumentoTotale = (minutiTrascorsi / 60.0) * aumentoTemp(generatore);
        temperatura += aumentoTotale;

        //Se la temperatura supera i 28°C spengo l’impianto
        if (temperatura >= 28.0) {
            temperatura = 28.0;
            spegni(orarioAttuale);
            statoModificato = true;
        }
    } else {
        //Se l’impianto è spento diminuisco lentamente la temperatura
        uniform_real_distribution<double> decrementoTemp(0.01, 0.05);
        for (int i = 0; i < minutiTrascorsi; ++i) {
            temperatura -= decrementoTemp(generatore);
        }
        if (temperatura < 0) temperatura = 0;

        //Se la temperatura scende sotto i 25°C riattivo l’impianto
        if (temperatura < 25.0) {
            attivo = true;
            ultimaAttivazione = orarioAttuale;
            statoModificato = true;
        }
    }

    return statoModificato;
}

//Funzione per impostare il timer automatico dell’impianto (accensione/spegnimento)
//Author: Davide Gastaldello
bool ImpiantoMediterraneo::impostaTimer(const Orario& oraInizio, const Orario& oraFine) {
    if (!modalitaAutomatica) return false;
    if (oraFine <= oraInizio) return false;

    prossimaAttivazione = oraInizio;
    prossimoSpegnimento = oraFine;
    return true;
}

//Funzione per disattivare il timer automatico dell’impianto
//Author: Davide Gastaldello
bool ImpiantoMediterraneo::rimuoviTimer() {
    if (!modalitaAutomatica) return false;

    prossimaAttivazione = Orario();
    prossimoSpegnimento = Orario();
    return true;
}

//Funzione che restituisce una stringa con lo stato attuale dell’impianto
//Author: Davide Gastaldello
string ImpiantoMediterraneo::stampaStato() const {
    stringstream ss;
    string stato = attivo ? "Attivo" : "Spento";

    ss << "[Mediterraneo] " << nome << " (ID: " << id << ") - Stato: " << stato
       << " | Consumo: " << fixed << setprecision(2) << consumoIdrico << " L"
       << " | Temperatura: " << fixed << setprecision(2) << temperatura << " C";

    return ss.str();
}

//Funzione per clonare l’oggetto impianto (usata per creare copie dinamiche)
//Author: Davide Gastaldello
Impianto* ImpiantoMediterraneo::clone() const {
    return new ImpiantoMediterraneo(*this);
}
