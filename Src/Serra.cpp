#include "Serra.h"
#include <sstream>
#include <iomanip>
#include "Logger.h"

using namespace std;

//Costruttore di default
//Author: Davide Gastaldello
Serra::Serra() : orarioAttuale(), prossimoId(1) {
    //Inizializzo l'orario a 00:00 e il prossimo ID a 1
}

//Distruttore
//Author: Davide Gastaldello
Serra::~Serra() {
    for (auto ptr : impianti) { //Itero su tutti i puntatori a impianto
        delete ptr; //Dealloco la memoria di ciascun impianto
    }
}

//Funzione per cercare l'indice di un impianto specifico dato l'ID
//Author: Davide Gastaldello
size_t Serra::trovaIndiceImpianto(int id) const {
    for (size_t i = 0; i < impianti.size(); ++i) {  //Itero tutti gli impianti della serra
        if (impianti[i]->getId() == id) {   //Se l'ID dell'impianto corrisponde all'ID parametro
            return i; //Ritorno l'indice
        }
    }
    return impianti.size(); //Ritorno un valore non valido se non trovato
}

//Funzione per cercare l'indice di un impianto specifico dato il nome
//Author: Davide Gastaldello
size_t Serra::trovaIndiceImpiantoDaNome(const string& nome) const {
    for (size_t i = 0; i < impianti.size(); ++i) {  //Itero tutti gli impianti della serra
        if (impianti[i]->getNome() == nome) {   //Se il nome dell'impianto corrisponde al nome parametro
            return i; //Ritorno l'indice
        }
    }
    return impianti.size(); //Ritorno valore non valido se non trovato
}

//Funzione per aggiungere un impianto alla serra
//Author: Davide Gastaldello
int Serra::aggiungiImpianto(Impianto* impianto) {
    if (!impianto) {    //Controllo se l'impianto esiste
        log(orarioAttuale, "Errore: Impossibile aggiungere un impianto nullo", 1);
        return -1;
    }

    if (esisteImpianto(impianto->getNome())) {  //Controllo se esiste già un impianto con lo stesso nome
        log(orarioAttuale, "Errore: Impianto con nome '" + impianto->getNome() + "' già esistente", 1);
        return -1;
    }

    int id = prossimoId++; //Assegno un ID univoco e incremento l'ID per un eventuale prossimo impianto
    impianti.push_back(impianto); //Inserisco l'impianto nel vettore

    stringstream ss;
    ss << "Aggiunto impianto '" << impianto->getNome() << "' con ID " << id;
    log(orarioAttuale, ss.str());

    return id; //Ritorno l'ID assegnato al nuovo impianto
}

//Funzione per rimuovere un impianto usando l'ID
//Author: Davide Gastaldello
bool Serra::rimuoviImpianto(int id) {
    size_t indice = trovaIndiceImpianto(id); //Cerco l'indice dell'impianto
    if (indice < impianti.size()) { //Se l'indice è valido nel vettore di impianti
        string nome = impianti[indice]->getNome();
        delete impianti[indice]; //Dealloco la memoria
        impianti.erase(impianti.begin() + indice); //Rimuovo l'impianto dal vettore

        log(orarioAttuale, "Rimosso impianto '" + nome + "' con ID " + to_string(id));
        return true;    //Ritorno true perché sono riuscito a rimuovere l'impianto correttamente
    }

    log(orarioAttuale, "Errore: Impossibile rimuovere impianto con ID " + to_string(id) + ", non trovato", 1);
    return false; //Altrimenti ritorno false se non trovo l'impianto
}

//Funzione per rimuovere un impianto usando il nome
//Author: Davide Gastaldello
bool Serra::rimuoviImpiantoDaNome(const string& nome) {
    size_t indice = trovaIndiceImpiantoDaNome(nome);    //Trovo l'indice dell'impianto dal nome
    if (indice < impianti.size()) { //Se l'indice è valido nel vettore
        delete impianti[indice]; //Dealloco la memoria
        impianti.erase(impianti.begin() + indice); //Rimuovo l'impianto dal vettore

        log(orarioAttuale, "Rimosso impianto '" + nome + "'");
        return true;    //Ritorno true perché sono riuscito a rimuovere l'impianto correttamente
    }

    log(orarioAttuale, "Errore: Impossibile rimuovere impianto '" + nome + "', non trovato", 1);
    return false;   //Altrimenti ritorno false se non trovo l'impianto
}

//Funzione per accendere un impianto specifico dato il nome
//Author: Davide Gastaldello
bool Serra::accendiImpianto(const string& nome) {
    size_t indice = trovaIndiceImpiantoDaNome(nome);    //Trovo l'indice dell'impianto in base al nome
    if (indice < impianti.size()) { //Se l'indice è valido nel vettore
        bool result = impianti[indice]->accendi(orarioAttuale); //Accendo l'impianto in base all'orario attuale

        if (result) {
            log(orarioAttuale, "L'impianto '" + nome + "' si è acceso");
        } else {
            log(orarioAttuale, "Errore: Impossibile accendere l'impianto '" + nome + "'", 1);
        }

        return result;
    }

    log(orarioAttuale, "Errore: Impossibile trovare l'impianto '" + nome + "'", 1);
    return false;   //Altrimenti ritorno false se non ho trovato l'impianto
}

//Funzione per spegnere un impianto specifico dato il nome
//Author: Davide Gastaldello
bool Serra::spegniImpianto(const string& nome) {
    size_t indice = trovaIndiceImpiantoDaNome(nome);    //Trovo l'indice dell'impianto in base al nome
    if (indice < impianti.size()) { //Se l'indice è valido nel vettore
        bool result = impianti[indice]->spegni(orarioAttuale); //Spengo l'impianto in base all'orario attuale

        if (result) {
            log(orarioAttuale, "L'impianto '" + nome + "' si è spento");
        } else {
            log(orarioAttuale, "Errore: Impossibile spegnere l'impianto '" + nome + "'", 1);
        }

        return result;
    }

    log(orarioAttuale, "Errore: Impossibile trovare l'impianto '" + nome + "'", 1);
    return false;   //Altrimenti ritorno false se non ho trovato l'impianto
}

//Funzione per impostare un timer ad un impianto in base al nome
//Author: Davide Gastaldello
bool Serra::impostaTimer(const string& nome, const Orario& oraInizio, const Orario& oraFine) {
    size_t indice = trovaIndiceImpiantoDaNome(nome);    //Trovo l'indice dell'impianto in base al nome
    if (indice < impianti.size()) { //Se l'indice è valido nel vettore
        bool result = impianti[indice]->impostaTimer(oraInizio, oraFine); //Imposto il timer con gli orari parametri

        if (result) {
            stringstream ss;
            ss << "Timer impostato per '" << nome << "' dalle " << oraInizio.toString() << " alle " << oraFine.toString();
            log(orarioAttuale, ss.str());
        } else {
            log(orarioAttuale, "Errore: Impossibile impostare timer per l'impianto '" + nome + "'", 1);
        }

        return result;
    }

    log(orarioAttuale, "Errore: Impossibile trovare l'impianto '" + nome + "'", 1);
    return false;   //Altrimenti ritorno false se non ho trovato l'impianto
}

//Funzione per rimuovere un timer impostato precedentemente da un impianto
//Author: Davide Gastaldello
bool Serra::rimuoviTimer(const string& nome) {
    size_t indice = trovaIndiceImpiantoDaNome(nome);    //Trovo l'indice dell'impianto in base al nome
    if (indice < impianti.size()) { //Se l'indice è valido nel vettore
        bool result = impianti[indice]->rimuoviTimer(); //Rimuovo il timer

        if (result) {
            log(orarioAttuale, "Timer rimosso per l'impianto '" + nome + "'");
        } else {
            log(orarioAttuale, "Errore: L'impianto '" + nome + "' non ha timer da rimuovere", 1);
        }

        return result;
    }

    log(orarioAttuale, "Errore: Impossibile trovare l'impianto '" + nome + "'", 1);
    return false;   //Altrimenti ritorno false se non ho trovato l'impianto
}

//Funzione per aggiornare l'orario della serra e lo stato degli impianti
//Author: Davide Gastaldello
vector<string> Serra::impostaOrario(const Orario& nuovoOrario) {
    vector<string> eventi;

    if (nuovoOrario < orarioAttuale) {  //Se il nuovo orario è precedente a quello attuale, significa che è errato
        stringstream ss;
        ss << "Errore: Impossibile impostare un orario precedente a quello attuale (" << orarioAttuale.toString() << ")";
        log(orarioAttuale, ss.str(), 1);
        eventi.push_back(ss.str());
        return eventi;
    }

    Orario iteratore = orarioAttuale;
    while (iteratore < nuovoOrario) {   //Finché l'orario è minore di quello a cui devo arrivare
        Orario prossimoStep = iteratore;
        prossimoStep.incrementa(30);    //Incremento di 30 minuti

        if (prossimoStep > nuovoOrario) {   //Mi fermo all'orario esatto se il prossimo step è successivo alla mezz'ora
            prossimoStep = nuovoOrario;
        }

        for (Impianto* impianto : impianti) {   //Per ogni impianto nella serra
            bool statoModificato = impianto->aggiorna(iteratore, prossimoStep);
            if (statoModificato) {  //Se lo stato è modificato
                stringstream ss;
                if (impianto->isAttivo()) { //se l'impianto si è attivato, lo comunico
                    ss << "L'impianto \"" << impianto->getNome() << "\" si è acceso";
                    log(prossimoStep, ss.str());
                } else {    //Se l'impianto si è spento, lo comunico
                    ss << "L'impianto \"" << impianto->getNome() << "\" si è spento";
                    log(prossimoStep, ss.str());
                }
                eventi.push_back(ss.str());
            }
        }

        iteratore = prossimoStep;

        if (iteratore.getMinuti() % 30 == 0) {  //Se ho iterato di mezz'ora
            stringstream ssOrario;
            ssOrario << "L'orario attuale e' " << iteratore.toString(); //Lo scrivo
            log(iteratore, ssOrario.str());
            eventi.push_back(ssOrario.str());
        }
    }

    orarioAttuale = nuovoOrario;    //L'orario attuale è ora il nuovo orario
    return eventi;
}

//Funzione per ritornare l'orario attuale della serra
//Author: Davide Gastaldello
Orario Serra::getOrarioAttuale() const {
    return orarioAttuale;
}

//Funzione per resettare l'orario a 00:00 e spegnere tutti gli impianti
//Author: Davide Gastaldello
vector<string> Serra::resetOrario() {
    vector<string> eventi;  //Creo un vettore di stringhe che conterrà gli eventi

    for (Impianto* impianto : impianti) {   //Per ogni impianto nella serra
        if (impianto->isAttivo()) { //Se è acceso
            impianto->spegni(orarioAttuale);    //Lo spengo
            stringstream ss;
            ss << "L'impianto \"" << impianto->getNome() << "\" si è spento";
            log(orarioAttuale, ss.str());
            eventi.push_back(ss.str()); //Evento spegnimento
        }

        impianto->rimuoviTimer(); //Rimuovo tutti timer da ogni impianto
    }

    orarioAttuale.reset(); //Resetto l'orario attuale a 00:00

    log(orarioAttuale, "L'orario è stato reimpostato a 00:00");
    eventi.push_back("L'orario è stato reimpostato a 00:00");

    return eventi;  //Ritorno gli eventi
}

//Funzione per rimuovere tutti i timer dagli impianti
//Author: Davide Gastaldello
vector<string> Serra::resetTimer() {
    vector<string> eventi;  //Creo un vettore di stringhe che conterrà gli eventi

    for (Impianto* impianto : impianti) {   //Per ogni impianto nella serra
        if (impianto->rimuoviTimer()) { //Rimuovo il timer
            stringstream ss;
            ss << "Timer rimosso per l'impianto \"" << impianto->getNome() << "\"";
            log(orarioAttuale, ss.str());
            eventi.push_back(ss.str());
        }
    }

    log(orarioAttuale, "Tutti i timer sono stati rimossi");
    eventi.push_back("Tutti i timer sono stati rimossi");

    return eventi;
}

//Funzione per resettare del tutto la serra
//Author: Davide Gastaldello
vector<string> Serra::resetAll() {
    vector<string> eventi;  //Creo un vettore di stringhe che conterrà gli eventi
    vector<string> eventiTimer = resetTimer(); //Resetto i timer
    eventi.insert(eventi.end(), eventiTimer.begin(), eventiTimer.end());    //In eventi, copio gli eventi del timer

    vector<string> eventiOrario = resetOrario(); //Reset l'orario
    eventi.insert(eventi.end(), eventiOrario.begin(), eventiOrario.end());  //In eventi, copio gli eventi dell'orario

    log(orarioAttuale, "Il sistema è stato ripristinato alle condizioni iniziali");
    eventi.push_back("Il sistema è stato ripristinato alle condizioni iniziali");

    return eventi;
}

//Funzione per mostrare lo stato di tutti gli impianti
//Author: Davide Gastaldello
vector<string> Serra::mostraStatoImpianti() const {
    vector<string> stati;   //Creo un vettore di stringhe per ottenere gli stati

    if (impianti.empty()) { //Se non è presente nessun impianto, lo segnalo
        stati.push_back("Nessun impianto presente nella serra");
        return stati;
    }

    double consumoTotale = getConsumoIdricoTotale();    //Calcolo il consumo idrico totale della serra

    stringstream ssIntestazione;    //Creo una stringa per l'intestazione
    ssIntestazione << "===== STATO SERRA - " << orarioAttuale.toString() << " =====";
    stati.push_back(ssIntestazione.str());

    for (const Impianto* impianto : impianti) {
        stati.push_back(impianto->stampaStato()); //Aggiungo lo stato di ogni impianto
    }

    stringstream ssFooter;
    ssFooter << "Consumo idrico totale: " << fixed << setprecision(2)
             << consumoTotale << " litri";
    stati.push_back(ssFooter.str());
    stati.push_back("========================================");

    return stati;
}

//Funzione per mostrare lo stato di un singolo impianto
//Author: Davide Gastaldello
string Serra::mostraStatoImpianto(const string& nome) const {
    size_t indice = trovaIndiceImpiantoDaNome(nome);    //Prelevo l'indice dell'impianto dal nome
    if (indice < impianti.size()) { //Se l'indice è presente nel vettore
        return impianti[indice]->stampaStato(); //Stampo lo stato
    }
    return "Impianto \"" + nome + "\" non trovato"; //Altrimenti non ho trovato l'impianto
}

//Funzione per calcolare il consumo idrico totale della serra
//Author: Davide Gastaldello
double Serra::getConsumoIdricoTotale() const {
    double consumoTotale = 0.0;
    for (const Impianto* impianto : impianti) { //Per ogni impianto nella serra
        consumoTotale += impianto->getConsumoIdrico();  //Il consumo totale è incrementato in base al consumo dell'impianto
    }
    return consumoTotale;
}

//Funzione che ritorna il numero di impianti presneti all'interno della serra
//Author: Davide Gastaldello
size_t Serra::getNumeroImpianti() const {
    return impianti.size();
}

//Funzione per verificare se un determinato impianto esiste, dato il nome
//Author: Davide Gastaldello
bool Serra::esisteImpianto(const string& nome) const {
    return trovaIndiceImpiantoDaNome(nome) < impianti.size();
}