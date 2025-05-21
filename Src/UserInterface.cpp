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

UserInterface::UserInterface(Serra& serra) : serra(serra), continua(true) {
    initializeLogger(new ConsoleLogger());
}

UserInterface::~UserInterface() {
    initializeLogger(nullptr);
}

bool UserInterface::isRunning() const {
    return continua;
}

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
            cout << "Chiusura del sistema di gestione serra." << std::endl; //Scrivo che ho chiuso il programma
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
        std::cerr << "Orario non valido: " << str << std::endl;
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
void UserInterface::processCommand(const std::string& command) {
    try {
        //Gestisco in moso speciale il comando add per aggiungere un impianto alla serra
        if (command.substr(0, 4) == "add ") {   //Controllo se il comando contiene la sottostringa "add"
            istringstream ss(command);
            string cmd, tipo, nome;

            ss >> cmd; //Legge la prima parola del comando che dovrebbe essere "add"
            ss >> tipo; //Legge il tipo (tropicale, mediterraneo, ecc.)
            getline(ss, nome); //Dopo comando e tipo resta solo da leggere il nome, che sarà la parte finale del comando
            nome = nome.substr(1); //Dal nome devo però rimuovere lo spazio iniziale che avevo letto con getLine()

            transform(tipo.begin(), tipo.end(), tipo.begin(),
                          [](unsigned char c){ return tolower(c); });   //Trasformo la stringa del tipo in modo da avere una lettura corretta

            if (serra.esisteImpianto(nome)) {   //Controllo che nella serra non esista già un impianto con il nome scelto
                cerr << "Errore: Esiste già un impianto con il nome '" << nome << "'" << endl;
                return;
            }

            int nuovoId = serra.getNumeroImpianti() + 1;    //Il nuovo ID sarà il numero successivo al numero di impianti

            //Aggiungo un nuovo impianto in base al tipo
            if (tipo == "tropicale") {
                serra.aggiungiImpianto(new ImpiantoTropicale(nuovoId, nome));
                cout << "Aggiunto impianto tropicale: " << nome << endl;
            } else if (tipo == "mediterraneo") {
                serra.aggiungiImpianto(new ImpiantoMediterraneo(nuovoId, nome));
                cout << "Aggiunto impianto mediterraneo: " << nome << endl;
            } else if (tipo == "desertico") {
                serra.aggiungiImpianto(new ImpiantoDesertico(nuovoId, nome));
                cout << "Aggiunto impianto desertico: " << nome << endl;
            } else if (tipo == "alpino") {
                serra.aggiungiImpianto(new ImpiantoAlpino(nuovoId, nome));
                cout << "Aggiunto impianto alpino: " << nome << endl;
            } else if (tipo == "carnivoro") {
                serra.aggiungiImpianto(new ImpiantoCarnivoro(nuovoId, nome));
                cout << "Aggiunto impianto carnivoro: " << nome << endl;
            } else {
                cerr << "Tipo di impianto non riconosciuto: " << tipo << endl;
            }
            return;
        }

        vector<string> parti = commandParser(command);  //Creo un vettore che conterrà le parti del comando che mi ritorna la funzione commandParser
        if (parti.empty()) return;  //Controllo se il vettore è vuoto

        if (parti[0] == "set" && parti.size() >= 3) {
            //Se la prima parola del comando è "set" e il comando ha 3 o più parti
            if (parti[1] == "time" && parti.size() == 3) {  //Se la seconda parola è "time"
                Orario nuovoOrario = stringToOrario(parti[2]);  //La terza parte del comando sarà l'orario e quindi lo salvo in una variabile orario dopoa verlo convertito
                if (nuovoOrario == Orario()) {  //Controllo che il formato dell'orario sia corretto (Orario vuoto errore)
                    cerr << "Formato orario non valido. Usa HH:MM." << endl;
                    return;
                }

                Orario attuale = serra.getOrarioAttuale();  //Prendo l'orario attuale della serra

                if (nuovoOrario < attuale) {    //Se il nuovo orario è precedente a quello attuale --> errore
                    cerr << "Non è possibile impostare un orario precedente all'attuale." << endl;
                    return;
                }

                Orario corrente = attuale;

                while (corrente < nuovoOrario) {    //Scorro nel tempo fino al nuovo orario
                    Orario prossimo = corrente;
                    prossimo.incrementa(1); //Incremento di minuto in minuto
                    vector<string> eventi = serra.impostaOrario(prossimo);  //Imposto l'orario della serra come prossimo
                    corrente = prossimo;
                }
                return;
            }


            //Gestione dell'accensione di un impianto (comando "set nome on/off")
            string nomeImpianto = parti[1];

            if (parti[2] == "on") {
                // Accendi impianto
                if (serra.accendiImpianto(nomeImpianto)) {
                    log(serra.getOrarioAttuale(), "L'impianto '" + nomeImpianto + "' si e' acceso", 0);
                } else {
                    std::cout << "Impossibile accendere l'impianto '" << nomeImpianto << "'" << std::endl;
                }
            } else if (parti[2] == "off") {
                // Spegni impianto
                if (serra.spegniImpianto(nomeImpianto)) {
                    log(serra.getOrarioAttuale(), "L'impianto '" + nomeImpianto + "' si e' spento", 0);
                } else {
                    cerr << "Impossibile spegnere l'impianto '" << nomeImpianto << "'" << std::endl;
                }
            } else if (parti.size() == 3 && parti[2].find(':') != string::npos) {
                // Comando: set nome HH:MM
                Orario oraInizio = stringToOrario(parti[2]);
                if (oraInizio == Orario()) {
                    std::cout << "Formato orario non valido. Usa HH:MM." << std::endl;
                    return;
                }

                Impianto* imp = serra.getImpianto(nomeImpianto);
                if (!imp) {
                    std::cout << "Impianto '" << nomeImpianto << "' non trovato." << std::endl;
                    return;
                }

                Orario oraFine;
                if (imp->isAutomatico()) {
                    int durata = imp->getDurataAutomatica();
                    oraFine = oraInizio;
                    oraFine.incrementa(durata);
                } else {
                    oraFine = Orario(23, 59); // Manuale: acceso fino a fine giornata
                }

                if (serra.impostaTimer(nomeImpianto, oraInizio, oraFine)) {
                    log(serra.getOrarioAttuale(), "Timer impostato per '" + nomeImpianto +
                                                  "' dalle " + oraInizio.toString() +
                                                  " alle " + oraFine.toString(), 0);
                } else {
                    std::cout << "Impossibile impostare il timer per l'impianto '" << nomeImpianto << "'" << std::endl;
                }
                return;
            }
            else if (parti.size() == 4 && parti[2].find(':') != string::npos && parti[3].find(':') != string::npos) {
                // Comando: set nome HH:MM HH:MM
                Orario oraInizio = stringToOrario(parti[2]);
                Orario oraFine = stringToOrario(parti[3]);

                if (oraInizio == Orario() || oraFine == Orario()) {
                    std::cout << "Formato orario non valido. Usa HH:MM." << std::endl;
                    return;
                }

                Impianto* imp = serra.getImpianto(nomeImpianto);
                if (!imp) {
                    std::cout << "Impianto '" << nomeImpianto << "' non trovato." << std::endl;
                    return;
                }

                if (imp->isAutomatico()) {
                    std::cout << "Errore: Non è possibile impostare un intervallo temporale per un impianto automatico." << std::endl;
                    return;
                }

                // Impianto manuale
                if (serra.impostaTimer(nomeImpianto, oraInizio, oraFine)) {
                    log(serra.getOrarioAttuale(), "Timer impostato per '" + nomeImpianto +
                                                  "' dalle " + oraInizio.toString() +
                                                  " alle " + oraFine.toString(), 0);
                } else {
                    std::cout << "Impossibile impostare il timer per l'impianto '" << nomeImpianto << "'" << std::endl;
                }
                return;
            }
        }
        // Comando rm (rimozione timer)
        if (parti[0] == "rm" && parti.size() == 2) {
            std::string nomeImpianto = parti[1];

            if (serra.rimuoviTimer(nomeImpianto)) {
                log(serra.getOrarioAttuale(), "Timer rimosso per '" + nomeImpianto + "'", 0);
            } else {
                std::cout << "Impossibile rimuovere il timer per l'impianto '" << nomeImpianto << "'" << std::endl;
            }
            return;
        }

        // Comando show (mostra stato)
        if (parti[0] == "show") {
            if (parti.size() == 1) {
                // Mostra tutti gli impianti
                std::vector<std::string> stati = serra.mostraStatoImpianti();

                if (stati.empty()) {
                    std::cout << "Nessun impianto presente nella serra." << std::endl;
                } else {
                    std::cout << "=== Stato Impianti ===" << std::endl;
                    for (const auto& stato : stati) {
                        std::cout << stato << std::endl;
                    }
                    std::cout << "Consumo idrico totale: " << std::fixed << std::setprecision(2)
                              << serra.getConsumoIdricoTotale() << " litri" << std::endl;
                }
            } else if (parti.size() == 2) {
                // Mostra un impianto specifico
                std::string nomeImpianto = parti[1];
                std::string stato = serra.mostraStatoImpianto(nomeImpianto);

                if (stato.empty()) {
                    std::cout << "Impianto '" << nomeImpianto << "' non trovato." << std::endl;
                } else {
                    std::cout << stato << std::endl;
                }
            }
            return;
        }

        // Comandi reset
        if (parti[0] == "reset") {
            if (parti.size() == 2) {
                if (parti[1] == "time") {
                    // Reset orario
                    std::vector<std::string> eventi = serra.resetOrario();
                    for (const auto& evento : eventi) {
                        std::cout << evento << std::endl;
                    }
                    log(Orario(), "L'orario attuale e' 00:00", 0);
                    return;
                } if (parti[1] == "timers") {
                    // Reset timer
                    std::vector<std::string> eventi = serra.resetTimer();
                    for (const auto& evento : eventi) {
                        std::cout << evento << std::endl;
                    }
                    log(serra.getOrarioAttuale(), "Tutti i timer sono stati rimossi", 0);
                    return;
                } if (parti[1] == "all") {
                    // Reset completo
                    std::vector<std::string> eventi = serra.resetAll();
                    for (const auto& evento : eventi) {
                        std::cout << evento << std::endl;
                    }
                    log(Orario(), "Sistema completamente resettato", 0);
                    return;
                }
            }
        }

        // Se arriviamo qui, il comando non è stato riconosciuto
        std::cout << "Comando non riconosciuto. Digita 'help' per la lista dei comandi." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Errore nell'esecuzione del comando: " << e.what() << std::endl;
    }
}