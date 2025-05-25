#include "UserInterface.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <memory>
#include "Orario.h"
#include "Serra.h"
#include "Tropicale.h"
#include "Mediterraneo.h"
#include "Desertico.h"
#include "Alpino.h"
#include "Carnivoro.h"
#include "Logger.h"

//Costruttore per UserInterface
//Author: Davide Gastaldello
UserInterface::UserInterface(Serra& serra) : serra(serra), continua(true) {
}

//Distruttore per UserInterface
//Author: Davide Gastaldello
UserInterface::~UserInterface() {
}

//Funzione per verificare che l'interfaccia utente stia effettivamente funzionando
//Author: Davide Gastaldello
bool UserInterface::isRunning() const {
    return continua;
}

//Funzione per fermare l'esecuzione dell'interfaccia utente
//Author: Davide Gastaldello
void UserInterface::stop() {
    continua = false;
}

//Funzione per far partire l'interaccia utente
//Author: Davide Gastaldello
void UserInterface::run() {
    string input;

    cout << "=== Sistema di Gestione Serra Intelligente ===" << endl;
    cout << "Digita 'help' per visualizzare i comandi disponibili." << endl;
    while (continua) {
        cout << "\n> "; //Stampo il simbolo del terminale che indica che posso inserire un comando
        getline(cin, input);    //Funzione che legge una riga del programma

        string inputLower = input;
        transform(inputLower.begin(), inputLower.end(), inputLower.begin(),
                      [](unsigned char c){ return tolower(c); });   //Trasformo la stringa letta in un formato
        //necessario al riconoscimento di un comando

        //Gestione dei comandi exit, quit e help
        if (inputLower == "exit" || inputLower == "quit") { //Se il comando è exit o quit
            continua = false;   //Dico al programma che deve fermarsi...
            cout << "Chiusura del sistema di gestione serra." << endl; //Scrivo che ho chiuso il programma
            continue;
        } if (inputLower == "help") {   //Se il comando è help
            stampaHelp();   //Stampo il messaggio che mostra quello che fanno i vari comandi
            continue;
        }

        processCommand(input);  //Chiamata alla funzione necessaria per processare tutti gli altri comandi
    }
}

//Funzione per dividere il comando in base alle parole di cui è composto
//Author: codice fornito dal professore
vector<string> UserInterface::commandParser(const string& command) {
    vector<string> tokens;
    istringstream ss(command);
    string token;

    bool isDeviceName = false;
    string deviceName;

    while (ss >> token) {
        if (isDeviceName) {
            if (token == "on" || token == "off" || token.find(':') != string::npos) {
                if (!deviceName.empty()) {
                    tokens.push_back(deviceName);
                    deviceName.clear();
                }
                isDeviceName = false;
            }
        }

        if (token == "set" || token == "rm" || token == "show" || token == "add") {
            tokens.push_back(token);
            isDeviceName = true;
            continue;
        }

        if (isDeviceName) {
            if (!deviceName.empty()) {
                deviceName += " ";
            }
            deviceName += token;
        } else {
            tokens.push_back(token);
        }
    }

    if (!deviceName.empty()) {
        tokens.push_back(deviceName);
    }

    return tokens;
}

//Funzione per convertire una stringa in un oggetto Orario
//Author: Davide Gastaldello
Orario UserInterface::stringToOrario(const string& str) {
    int ore = 0, minuti = 0;

    //Verifico che il formato della stringa sia quello corretto
    if (str.size() == 5 && str[2] == ':') {
        try {
            ore = stoi(str.substr(0, 2));   //Converto la stringa formata dai primi due caratteri in intero
            minuti = stoi(str.substr(3, 2));     //Converto la stringa formata dagli ultimi due caratteri in intero
        } catch (const exception& e) {
            cerr << "Formato orario non valido: " << str << endl;   //Se non riesco a convertire significa che l'orario è stato scritto male
            return Orario();
        }
    }

    //Verifico che l'orario inserito sia corretto (non ci siano ore > 23 o < 0 e minuti > 60 o < 0)
    if (ore < 0 || ore > 23 || minuti < 0 || minuti > 59) {
        cerr << "Orario non valido: " << str << endl;
        return Orario();
    }

    return Orario(ore, minuti); //Se non ci sono stati errori ritorno una variabile Orario con ore e minuti scritti
}

//Funzione per stampare l'help con i comandi disponibili
//Author: Davide Gastaldello
void UserInterface::stampaHelp() {
    cout << "\n=== Sistema di Gestione Serra Intelligente - Guida Comandi ===" << endl;
    cout << "add tropicale <nome>       - Aggiunge un impianto tropicale" << endl;
    cout << "add mediterraneo <nome>    - Aggiunge un impianto mediterraneo" << endl;
    cout << "add desertico <nome>       - Aggiunge un impianto desertico" << endl;
    cout << "add alpino <nome>          - Aggiunge un impianto alpino" << endl;
    cout << "add carnivoro <nome>       - Aggiunge un impianto carnivoro" << endl;
    cout << "set <nome> on              - Accende l'impianto manualmente" << endl;
    cout << "set <nome> off             - Spegne l'impianto manualmente" << endl;
    cout << "set <nome> <inizio> <fine> - Imposta timer per l'impianto" << endl;
    cout << "rm <nome>                  - Rimuove il timer dell'impianto" << endl;
    cout << "show                       - Mostra tutti gli impianti" << endl;
    cout << "show <nome>                - Mostra un impianto specifico" << endl;
    cout << "set time <HH:MM>           - Imposta l'orario del sistema" << endl;
    cout << "reset time                 - Reimposta l'orario a 00:00" << endl;
    cout << "reset timers               - Rimuove tutti i timer" << endl;
    cout << "reset all                  - Reimposta tutto il sistema" << endl;
    cout << "exit                       - Esci dal programma" << endl;
    cout << "help                       - Mostra questo messaggio di aiuto" << endl;
    cout << "================================================================\n" << endl;
}

//Funzione per processare un comando
//Author: Davide Gastaldello (utilizzando struttura fornita dal professore)
void UserInterface::processCommand(const string& command) {
    try {
        //Gestisco in moso speciale il comando add per aggiungere un impianto alla serra
        if (command.substr(0, 4) == "add ") {   //Controllo se il comando contiene la sottostringa "add"
            istringstream ss(command);
            string cmd, tipo, nome;

            ss >> cmd; //Legge la prima parola del comando che dovrebbe essere "add"
            ss >> tipo; //Legge il tipo (tropicale, mediterraneo, ecc.)
            getline(ss, nome); //Dopo comando e tipo resta solo da leggere il nome, che sarà la parte finale del comando

            // Rimuovi spazi iniziali e finali manualmente:
            size_t start = nome.find_first_not_of(" \t\r\n");
            if (start == string::npos) {
                cerr << "Errore: Nome impianto non specificato." << endl;
                return;
            }

            size_t end = nome.find_last_not_of(" \t\r\n");
            nome = nome.substr(start, end - start + 1);

            if (nome.empty()) {
                cerr << "Errore: Nome impianto non specificato." << endl;
                return;
            }

            transform(tipo.begin(), tipo.end(), tipo.begin(),
                      [](unsigned char c){ return tolower(c); });

            if (serra.esisteImpianto(nome)) {   //Controllo che nella serra non esista già un impianto con il nome scelto
                cerr << "Errore: Esiste già un impianto con il nome '" << nome << "'" << endl;
                return;
            }

            int nuovoId = serra.getNumeroImpianti() + 1;    //Il nuovo ID sarà il numero successivo al numero di impianti

            //Aggiungo un nuovo impianto in base al tipo
            if (tipo == "tropicale") {
                serra.aggiungiImpianto(new ImpiantoTropicale(nuovoId, nome));
            } else if (tipo == "mediterraneo") {
                serra.aggiungiImpianto(new ImpiantoMediterraneo(nuovoId, nome));
            } else if (tipo == "desertico") {
                serra.aggiungiImpianto(new ImpiantoDesertico(nuovoId, nome));
            } else if (tipo == "alpino") {
                serra.aggiungiImpianto(new ImpiantoAlpino(nuovoId, nome));
            } else if (tipo == "carnivoro") {
                serra.aggiungiImpianto(new ImpiantoCarnivoro(nuovoId, nome));
            } else {
                cerr << "Tipo di impianto non riconosciuto: " << tipo << endl;
            }
            return;
        }

        vector<string> parti = commandParser(command);  //Creo un vettore che conterrà le parti del comando che mi ritorna la funzione commandParser
        if (parti.empty()) return;  //Controllo se il vettore è vuoto

        if (parti[0] == "set" && parti.size() >= 3) {
            //Se la prima parola del comando è "set" e il comando ha 3 o più parti
            if (parti[1] == "time" && parti.size() == 3) {  //Gestione del comando "set time HH:MM"
                Orario nuovoOrario = stringToOrario(parti[2]);
                if (nuovoOrario == Orario()) {
                    cerr << "Formato orario non valido. Usa HH:MM." << endl;
                    return;
                }

                Orario attuale = serra.getOrarioAttuale();

                if (nuovoOrario < attuale) {    //Controllo se si sta cercando di andare indietro nel tempo
                    cerr << "Non e' possibile impostare un orario precedente all'attuale." << endl;
                    return;
                }

                if (nuovoOrario == attuale) {   //Se il nuovo orario è uguale all'attuale, non devo fare nulla
                    cout << "L'orario e' gia' impostato a " << nuovoOrario.toString() << endl;
                    return;
                }

                try {
                    serra.impostaOrario(nuovoOrario);   //Gestione normale dell'impostazione del nuovo orario
                    if(nuovoOrario.toString() == "23:59") {
                        cout << "=== FINE GIORNATA ===" << endl;
                        cout << "Ora se vuoi puoi terminare il codice con il comando 'exit' oppure resettare il sistema con uno dei comandi di 'reset'" << endl;
                    }
                } catch (const exception& e) {
                    cerr << "Errore durante l'impostazione dell'orario: " << e.what() << endl;
                }

                return;
            }
            string nomeImpianto = parti[1];  //Negli altri casi il secondo campo del è il nome dell'impianto

            //Controllo se l'impianto esiste effettivamente
            Impianto* imp = serra.getImpianto(nomeImpianto);
            if (!imp) {
                cerr << "Impianto '" << nomeImpianto << "' non trovato." << endl;
                return;
            }

            if (parti[2] == "on") { //Gestione comando "set nome on"
                serra.accendiImpianto(nomeImpianto);
            } else if (parti[2] == "off") { //Gestione comando "set nome off"
                serra.spegniImpianto(nomeImpianto);
            } else if (serra.getImpianto(nomeImpianto)->getTipo() == "mediterraneo") {  //Gestione nel caso di impianti mediterranei --> non posso impostare timer perchè conta la temperatura
                cerr << "I comandi 'set nome HH:MM' e set nome HH:MM HH:MM' non sono accessibili per gli impianti di tipo mediterraneo" << endl;
                return;
            } else if (parti.size() == 3 && parti[2].find(':') != string::npos) {   //Gestisco il comando è del tipo "set nome HH:MM"
                Orario oraInizio = stringToOrario(parti[2]);    //Il parametro che ho come terzo campo del comando è l'ora di inizio
                if (oraInizio == Orario()) {    //Gestisco il caso in cui l'ora di inizio non sia valida
                    cerr << "Formato orario non valido. Usa HH:MM." << endl;
                    return;
                }

                Impianto* imp = serra.getImpianto(nomeImpianto);    //Prendo l'impianto dalla serra dato il nome
                if (!imp) { //Gestisco il caso in cui non lo trovo
                    cerr << "Impianto '" << nomeImpianto << "' non trovato." << endl;
                    return;
                }

                Orario oraFine; //Variabile per gestire l'orario di fine
                if (imp->isAutomatico()) {  //Se l'impianto è automatico
                    int durata = imp->getDurataAutomatica();    //Prendo il valore della durataAutomatica
                    oraFine = oraInizio;
                    oraFine.incrementa(durata); //Incremento l'ora di fine fino al valore della durataAutomatica
                } else {
                    oraFine = Orario(23, 59); //Manuale: acceso fino a fine giornata
                }

                serra.impostaTimer(nomeImpianto, oraInizio, oraFine);   //Imposto un timer nella serra

                return;
            }
            else if (parti.size() == 4 && parti[2].find(':') != string::npos && parti[3].find(':') != string::npos) {
                //Gestisco il comando "set nome HH:MM HH:MM"
                Orario oraInizio = stringToOrario(parti[2]);    //Prelevo l'ora di inizio
                Orario oraFine = stringToOrario(parti[3]);  //Prelevo l'ora di fine

                if (oraInizio == Orario() || oraFine == Orario()) { //Se uno dei formati dell'ora non è corretto: errore
                    cerr << "Formato orario non valido. Usa HH:MM." << endl;
                    return;
                }

                Impianto* imp = serra.getImpianto(nomeImpianto);    //Prelevo l'impianto dalla serra
                if (!imp) { //Se non lo trovo: errore
                    cerr << "Impianto '" << nomeImpianto << "' non trovato." << endl;
                    return;
                }

                if (imp->isAutomatico()) {  //Se l'impianto è automatico, non posso impostare una durata manualmente
                    cerr << "Errore: Non e' possibile impostare un intervallo temporale per un impianto automatico." << endl;
                    return;
                }

                serra.impostaTimer(nomeImpianto, oraInizio, oraFine);   //Se l'impianto è manuale imposto un timer con gli orari inseriti

                return;
            }
        }

        if (parti[0] == "rm" && parti.size() == 2) {    //Gestione comando "rm nome"
            string nomeImpianto = parti[1];     //La seconda parola del comando rappresenta il nome dell'impianto

            serra.rimuoviTimer(nomeImpianto);

            return;
        }

        if (parti[0] == "show") {   //Gestione del comandi "show"
            if (parti.size() == 1) {    //Se questo comando non ha altre parti --> Gestione comando "show"
                vector<string> stati = serra.mostraStatoImpianti(); //Salvo in un vettore le informazioni su tutti gli impianti della serra

                if (stati.empty()) {    //Se non ci sono informazioni --> non ci sono impianti
                    cout << "Nessun impianto presente nella serra." << endl;
                } else {    //Stampo lo stato di tutti gli impianti
                    for (const string& stato : stati) {
                        cout << stato << endl;
                    }
                }
            } else if (parti.size() == 2) { //Gestione comando "show nome"
                string nomeImpianto = parti[1]; //Prelevo il nome dell'impianto dal comando
                string stato = serra.mostraStatoImpianto(nomeImpianto); //Salvo in una stringa lo stato dell'impianto

                if (stato.empty()) {    //Se non ci sono informazioni --> l'impianto richiesto non è presente
                    cout << "Impianto '" << nomeImpianto << "' non trovato." << endl;
                } else {
                    cout << stato << endl;
                }
            }
            return;
        }

        //Gestione comandi "reset"
        if (parti[0] == "reset") {
            if (parti.size() == 2) {
                if (parti[1] == "time") {   //Gestione comando "reset time"
                    serra.resetOrario();    //Resetto l'orario
                    return;
                } if (parti[1] == "timers") {   //Gestione comando "reset timers"
                    serra.resetTimer(); //Resetto tutti i timer presenti nella serra
                    return;
                } if (parti[1] == "all") {  //Gestione comando "reset all"
                    serra.resetAll();   //Resetto tutto il sistema (timer, orario, impianti)
                    return;
                }
            }
        }
       cout << "Comando non riconosciuto. Digita 'help' per la lista dei comandi." << endl; //Se nessun comando è stato riconosciuto, lo scrivo
    } catch (const exception& e) {
        cerr << "Errore nell'esecuzione del comando: " << e.what() << endl;
    }
}