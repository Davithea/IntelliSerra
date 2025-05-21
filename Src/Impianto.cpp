#include "Impianto.h"
#include "UserInterface.h"
#include <sstream>
#include <iomanip>

using namespace std;

//SISTEMATO FINO A RIGA 160

//Costruttore per l'impianto
//Author: Davide Gastaldello
Impianto::Impianto(int _id, const string& _nome, double _tassoConsumo, bool _modalitaAutomatica)
    : id(_id),
      nome(_nome),
      attivo(false),    //All'inizio l'impianto è spento
      consumoIdrico(0.0),   //Il consumo idrico iniziale è 0
      tassoConsumo(_tassoConsumo),  //Il tasso di consumo dipende dal tipo di impianto
      modalitaAutomatica(_modalitaAutomatica) {
}

//Funzione per accendere l'impianto
//Author: Davide Gastaldello
bool Impianto::accendi(const Orario& orarioCorrente) {
    if (attivo) {   //Se l'impianto è già attivo
        return false;   //Non è possibile accenderlo
    }

    attivo = true;  //Altrimenti lo accendo
    ultimaAttivazione = orarioCorrente; //L'orario corrente diventa l'orario dell'ultima attivazione
    return true;    //Ritorno true perché l'azione di accensione è stata completata con successo
}

//Funzione per spegnere l'impianto
//Author: Davide Gastaldello
bool Impianto::spegni(const Orario& orarioCorrente) {
    if (!attivo) {  //Se l'impianto è spento
        return false;   //Non è possibile spegnerlo
    }

    double oreAttivita = ultimaAttivazione.differenzaInOre(orarioCorrente); //Calcolo il consumo d'acqua in base al tempo in cui è rimasto acceso
    consumoIdrico += calcolaConsumo(oreAttivita);   //Incremento il consumo idrico dell'impianto

    attivo = false; //Spengo l'impianto
    return true;    //Ritorno true perché l'azione di spegnimento è stata completata con successo
}

//Funzione per impostare un timer
//Author: Davide Gastaldello
bool Impianto::impostaTimer(const Orario& oraInizio, const Orario& oraFine) {
    //Questa implementazione dovrà essere sovrascritta dalle classi derivate per supportare il corretto funzionamento
    return modalitaAutomatica;
}

//Funzione per rimuovere un timer
//Author: Davide Gastaldello
bool Impianto::rimuoviTimer() {
    //Questa implementazione dovrà essere sovrascritta dalle classi derivate per supportare il corretto funzionamento
    return modalitaAutomatica;
}

//Funzione per stamapre lo stato di un impianto usando il metodo logMessage di UserInterface
//Author: Davide Gastaldello
string Impianto::stampaStato() const {
    stringstream ss;    //Dichiaro una stringa

    //Compongo la stringa con le informazioni da stampare
    ss << "Impianto " << id << " (" << nome << "): ";
    ss << (attivo ? "ATTIVO" : "SPENTO");
    ss << " - Consumo totale: " << std::fixed << std::setprecision(2) << consumoIdrico << " litri";

    if (attivo) {
        ss << " - Ultima attivazione: " << ultimaAttivazione.toString();
    }

    string messaggio = ss.str();   //Converto tutto in una stringa
    return messaggio;   //Ritorno il messaggio stampato
}

//Fuznione per calcolare il consumo di un impianto in base alle ore di attività
//Author: Davide Gastaldello
double Impianto::calcolaConsumo(double oreDiAttivita) const {
    return tassoConsumo * oreDiAttivita;
}

bool Impianto::isAutomatico() const {
    return modalitaAutomatica;
}

int Impianto::getDurataAutomatica() const {
    return 60; // Default: 60 minuti (può essere sovrascritto nelle sottoclassi)
}
