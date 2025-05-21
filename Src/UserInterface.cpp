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
        cout << "\n> ";
        getline(cin, input);

        string inputLower = input;
        transform(inputLower.begin(), inputLower.end(), inputLower.begin(),
                      [](unsigned char c){ return std::tolower(c); });

        //Gestione dei comandi exit, quit e help
        if (inputLower == "exit" || inputLower == "quit") {
            continua = false;
            cout << "Chiusura del sistema di gestione serra." << std::endl;
            continue;
        } if (inputLower == "help") {
            stampaHelp();
            continue;
        }

        processCommand(input);  //Chiamata alla funzione necessaria per processare tutti gli altri comandi
    }
}

std::vector<std::string> UserInterface::commandParser(const std::string& command) {
    std::vector<std::string> tokens;
    std::istringstream ss(command);
    std::string token;

    bool isDeviceName = false;
    std::string deviceName;

    while (ss >> token) {
        if (isDeviceName) {
            if (token == "on" || token == "off" || token.find(':') != std::string::npos) {
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

    for (const auto& t : tokens) {
        std::cout << "[" << t << "] ";
    }
    std::cout << std::endl;

    return tokens;
}

// Funzione per convertire una stringa in un oggetto Orario
Orario UserInterface::stringToOrario(const std::string& str) {
    int ore = 0, minuti = 0;

    // Formato atteso: HH:MM
    if (str.size() == 5 && str[2] == ':') {
        try {
            ore = std::stoi(str.substr(0, 2));
            minuti = std::stoi(str.substr(3, 2));
        } catch (const std::exception& e) {
            // Gestione errore di conversione
            std::cerr << "Formato orario non valido: " << str << std::endl;
            return Orario();
        }
    }

    // Verifica validità
    if (ore < 0 || ore > 23 || minuti < 0 || minuti > 59) {
        std::cerr << "Orario non valido: " << str << std::endl;
        return Orario();
    }

    return Orario(ore, minuti);
}

// Stampa l'help con i comandi disponibili
void UserInterface::stampaHelp() {
    std::cout << "\n=== Sistema di Gestione Serra Intelligente - Guida Comandi ===" << std::endl;
    std::cout << "add tropicale <nome>       - Aggiunge un impianto tropicale" << std::endl;
    std::cout << "add mediterraneo <nome>    - Aggiunge un impianto mediterraneo" << std::endl;
    std::cout << "add desertico <nome>       - Aggiunge un impianto desertico" << std::endl;
    std::cout << "add alpino <nome>          - Aggiunge un impianto alpino" << std::endl;
    std::cout << "add carnivoro <nome>       - Aggiunge un impianto carnivoro" << std::endl;
    std::cout << "set <nome> on              - Accende l'impianto manualmente" << std::endl;
    std::cout << "set <nome> off             - Spegne l'impianto manualmente" << std::endl;
    std::cout << "set <nome> <inizio> <fine> - Imposta timer per l'impianto" << std::endl;
    std::cout << "rm <nome>                  - Rimuove il timer dell'impianto" << std::endl;
    std::cout << "show                       - Mostra tutti gli impianti" << std::endl;
    std::cout << "show <nome>                - Mostra un impianto specifico" << std::endl;
    std::cout << "set time <HH:MM>           - Imposta l'orario del sistema" << std::endl;
    std::cout << "reset time                 - Reimposta l'orario a 00:00" << std::endl;
    std::cout << "reset timers               - Rimuove tutti i timer" << std::endl;
    std::cout << "reset all                  - Reimposta tutto il sistema" << std::endl;
    std::cout << "exit                       - Esci dal programma" << std::endl;
    std::cout << "help                       - Mostra questo messaggio di aiuto" << std::endl;
    std::cout << "================================================================\n" << std::endl;
}

// Processo di un singolo comando
void UserInterface::processCommand(const std::string& command) {
    try {
        // Gestione speciale per il comando add
        if (command.substr(0, 4) == "add ") {
            std::istringstream ss(command);
            std::string cmd, tipo, nome;

            ss >> cmd; // Legge "add"
            ss >> tipo; // Legge il tipo (tropicale, mediterraneo, ecc.)

            // Legge il resto come nome dell'impianto
            std::getline(ss, nome);
            nome = nome.substr(1); // Rimuove lo spazio iniziale

            std::transform(tipo.begin(), tipo.end(), tipo.begin(),
                          [](unsigned char c){ return std::tolower(c); });

            if (serra.esisteImpianto(nome)) {
                std::cout << "Errore: Esiste già un impianto con il nome '" << nome << "'" << std::endl;
                return;
            }

            int nuovoId = serra.getNumeroImpianti() + 1;

            if (tipo == "tropicale") {
                serra.aggiungiImpianto(new ImpiantoTropicale(nuovoId, nome));
                std::cout << "Aggiunto impianto tropicale: " << nome << std::endl;
            } else if (tipo == "mediterraneo") {
                serra.aggiungiImpianto(new ImpiantoMediterraneo(nuovoId, nome));
                std::cout << "Aggiunto impianto mediterraneo: " << nome << std::endl;
            } else if (tipo == "desertico") {
                serra.aggiungiImpianto(new ImpiantoDesertico(nuovoId, nome));
                std::cout << "Aggiunto impianto desertico: " << nome << std::endl;
            } else if (tipo == "alpino") {
                serra.aggiungiImpianto(new Alpina(nuovoId, nome));
                std::cout << "Aggiunto impianto alpino: " << nome << std::endl;
            } else if (tipo == "carnivoro") {
                serra.aggiungiImpianto(new Carnivora(nuovoId, nome));
                std::cout << "Aggiunto impianto carnivoro: " << nome << std::endl;
            } else {
                std::cout << "Tipo di impianto non riconosciuto: " << tipo << std::endl;
            }
            return;
        }

        auto parti = commandParser(command);
        if (parti.empty()) return;

        // Comando set (accensione/spegnimento/timer)
        if (parti[0] == "set" && parti.size() >= 3) {
            if (parti[1] == "time" && parti.size() == 3) {
                Orario nuovoOrario = stringToOrario(parti[2]);
                if (nuovoOrario == Orario()) {
                    std::cout << "Formato orario non valido. Usa HH:MM." << std::endl;
                    return;
                }

                Orario attuale = serra.getOrarioAttuale();

                if (nuovoOrario < attuale) {
                    std::cout << "Non è possibile impostare un orario precedente all'attuale." << std::endl;
                    return;
                }

                Orario corrente = attuale;

                while (corrente < nuovoOrario) {
                    Orario prossimo = corrente;
                    prossimo.incrementa(1); // incrementa di 1 minuto
                    std::vector<std::string> eventi = serra.impostaOrario(prossimo);

                    for (const auto& evento : eventi) {
                        std::cout << evento << std::endl;
                    }

                    corrente = prossimo;
                }
                return;
            }


            // Accensione/spegnimento manuale
            std::string nomeImpianto = parti[1];

            if (parti[2] == "on") {
                // Accendi impianto
                if (serra.accendiImpianto(nomeImpianto)) {
                    log(serra.getOrarioAttuale(), "L'impianto '" + nomeImpianto + "' si è acceso", 0);
                } else {
                    std::cout << "Impossibile accendere l'impianto '" << nomeImpianto << "'" << std::endl;
                }
            } else if (parti[2] == "off") {
                // Spegni impianto
                if (serra.spegniImpianto(nomeImpianto)) {
                    log(serra.getOrarioAttuale(), "L'impianto '" + nomeImpianto + "' si è spento", 0);
                } else {
                    std::cout << "Impossibile spegnere l'impianto '" << nomeImpianto << "'" << std::endl;
                }
            } else if (parti.size() == 4) {
                // Imposta timer
                Orario oraInizio = stringToOrario(parti[2]);
                Orario oraFine = stringToOrario(parti[3]);

                if (oraInizio == Orario() || oraFine == Orario()) {
                    std::cout << "Formato orario non valido. Usa HH:MM." << std::endl;
                    return;
                }

                if (serra.impostaTimer(nomeImpianto, oraInizio, oraFine)) {
                    log(serra.getOrarioAttuale(), "Timer impostato per '" + nomeImpianto +
                                                  "' dalle " + oraInizio.toString() +
                                                  " alle " + oraFine.toString(), 0);
                } else {
                    std::cout << "Impossibile impostare il timer per l'impianto '" << nomeImpianto << "'" << std::endl;
                }
            }
            return;
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
                } else if (parti[1] == "timers") {
                    // Reset timer
                    std::vector<std::string> eventi = serra.resetTimer();
                    for (const auto& evento : eventi) {
                        std::cout << evento << std::endl;
                    }
                    log(serra.getOrarioAttuale(), "Tutti i timer sono stati rimossi", 0);
                    return;
                } else if (parti[1] == "all") {
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