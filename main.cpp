#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <cctype>
#include <iomanip>

#include "Include/Serra.h"
#include "Include/Impianto.h"
#include "Include/Orario.h"
#include "Include/Tropicale.h"
#include "Include/Mediterraneo.h"
//#include "Include/Desertico.h"
//#include "Include/Alpino.h"
//#include "Include/Carnivoro.h"

// Funzione ausiliaria per dividere una stringa in base a delimitatori
std::vector<std::string> dividiStringa(const std::string& input, char delimitatore = ' ') {
    std::vector<std::string> parti;
    std::istringstream stream(input);
    std::string parte;

    while (std::getline(stream, parte, delimitatore)) {
        if (!parte.empty()) {
            parti.push_back(parte);
        }
    }

    return parti;
}

// Funzione per convertire una stringa in un oggetto Orario
Orario stringToOrario(const std::string& str) {
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
void stampaHelp() {
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

// Funzione principale
int main() {
    Serra serra;
    std::string input;
    bool continua = true;

    std::cout << "=== Sistema di Gestione Serra Intelligente ===" << std::endl;
    std::cout << "Digita 'help' per visualizzare i comandi disponibili." << std::endl;

    // Aggiungiamo alcuni impianti di default per iniziare
    serra.aggiungiImpianto(std::make_unique<ImpiantoTropicale>(1, "Palma"));
    serra.aggiungiImpianto(std::make_unique<ImpiantoMediterraneo>(2, "Ulivo"));

    // Stampa lo stato iniziale
    std::cout << "Orario iniziale: " << serra.getOrarioAttuale().toString() << std::endl;

    while (continua) {
        std::cout << "\n> ";
        std::getline(std::cin, input);

        // Converti la stringa in minuscolo per i comandi (ma preserva i nomi degli impianti)
        std::string inputLower = input;
        std::transform(inputLower.begin(), inputLower.end(), inputLower.begin(),
                      [](unsigned char c){ return std::tolower(c); });

        // Dividi l'input in parti
        auto parti = dividiStringa(input);
        if (parti.empty()) continue;

        // Comandi base: exit e help
        if (inputLower == "exit" || inputLower == "quit") {
            continua = false;
            continue;
        } else if (inputLower == "help") {
            stampaHelp();
            continue;
        }

        // Elabora i comandi
        try {
            // Gestione comandi add
            if (parti[0] == "add" && parti.size() >= 3) {
                std::string tipo = parti[1];
                std::string nome = parti[2];

                std::transform(tipo.begin(), tipo.end(), tipo.begin(),
                              [](unsigned char c){ return std::tolower(c); });

                if (serra.esisteImpianto(nome)) {
                    std::cout << "Errore: Esiste già un impianto con il nome '" << nome << "'" << std::endl;
                    continue;
                }

                int nuovoId = serra.getNumeroImpianti() + 1;

                if (tipo == "tropicale") {
                    serra.aggiungiImpianto(std::make_unique<ImpiantoTropicale>(nuovoId, nome));
                    std::cout << "Aggiunto impianto tropicale: " << nome << std::endl;
                } else if (tipo == "mediterraneo") {
                    serra.aggiungiImpianto(std::make_unique<ImpiantoMediterraneo>(nuovoId, nome));
                    std::cout << "Aggiunto impianto mediterraneo: " << nome << std::endl;
                } else if (tipo == "desertico") {
                    serra.aggiungiImpianto(std::make_unique<ImpiantoDesertico>(nuovoId, nome));
                    std::cout << "Aggiunto impianto desertico: " << nome << std::endl;
                } else if (tipo == "alpino") {
                    serra.aggiungiImpianto(std::make_unique<ImpiantoAlpino>(nuovoId, nome));
                    std::cout << "Aggiunto impianto alpino: " << nome << std::endl;
                } else if (tipo == "carnivoro") {
                    serra.aggiungiImpianto(std::make_unique<ImpiantoCarnivoro>(nuovoId, nome));
                    std::cout << "Aggiunto impianto carnivoro: " << nome << std::endl;
                } else {
                    std::cout << "Tipo di impianto non riconosciuto: " << tipo << std::endl;
                }
                continue;
            }

            // Comando set (accensione/spegnimento/timer)
            if (parti[0] == "set" && parti.size() >= 3) {
                if (parti[1] == "time" && parti.size() == 3) {
                    // Imposta orario
                    Orario nuovoOrario = stringToOrario(parti[2]);
                    if (nuovoOrario == Orario()) {
                        std::cout << "Formato orario non valido. Usa HH:MM." << std::endl;
                        continue;
                    }

                    std::vector<std::string> eventi = serra.impostaOrario(nuovoOrario);
                    for (const auto& evento : eventi) {
                        std::cout << evento << std::endl;
                    }
                    std::cout << "[" << nuovoOrario.toString() << "] L'orario attuale è " << nuovoOrario.toString() << std::endl;
                    continue;
                }

                // Accensione/spegnimento manuale
                std::string nomeImpianto = parti[1];

                if (parti[2] == "on") {
                    // Accendi impianto
                    if (serra.accendiImpianto(nomeImpianto)) {
                        std::cout << "[" << serra.getOrarioAttuale().toString() << "] L'impianto '"
                                  << nomeImpianto << "' si è acceso" << std::endl;
                    } else {
                        std::cout << "Impossibile accendere l'impianto '" << nomeImpianto << "'" << std::endl;
                    }
                } else if (parti[2] == "off") {
                    // Spegni impianto
                    if (serra.spegniImpianto(nomeImpianto)) {
                        std::cout << "[" << serra.getOrarioAttuale().toString() << "] L'impianto '"
                                  << nomeImpianto << "' si è spento" << std::endl;
                    } else {
                        std::cout << "Impossibile spegnere l'impianto '" << nomeImpianto << "'" << std::endl;
                    }
                } else if (parti.size() == 4) {
                    // Imposta timer
                    Orario oraInizio = stringToOrario(parti[2]);
                    Orario oraFine = stringToOrario(parti[3]);

                    if (oraInizio == Orario() || oraFine == Orario()) {
                        std::cout << "Formato orario non valido. Usa HH:MM." << std::endl;
                        continue;
                    }

                    if (serra.impostaTimer(nomeImpianto, oraInizio, oraFine)) {
                        std::cout << "[" << serra.getOrarioAttuale().toString() << "] Timer impostato per '"
                                  << nomeImpianto << "' dalle " << oraInizio.toString()
                                  << " alle " << oraFine.toString() << std::endl;
                    } else {
                        std::cout << "Impossibile impostare il timer per l'impianto '" << nomeImpianto << "'" << std::endl;
                    }
                }
                continue;
            }

            // Comando rm (rimozione timer)
            if (parti[0] == "rm" && parti.size() == 2) {
                std::string nomeImpianto = parti[1];

                if (serra.rimuoviTimer(nomeImpianto)) {
                    std::cout << "[" << serra.getOrarioAttuale().toString() << "] Timer rimosso per '"
                              << nomeImpianto << "'" << std::endl;
                } else {
                    std::cout << "Impossibile rimuovere il timer per l'impianto '" << nomeImpianto << "'" << std::endl;
                }
                continue;
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
                continue;
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
                        std::cout << "[00:00] L'orario attuale è 00:00" << std::endl;
                    } else if (parti[1] == "timers") {
                        // Reset timer
                        std::vector<std::string> eventi = serra.resetTimer();
                        for (const auto& evento : eventi) {
                            std::cout << evento << std::endl;
                        }
                        std::cout << "[" << serra.getOrarioAttuale().toString()
                                  << "] Tutti i timer sono stati rimossi" << std::endl;
                    } else if (parti[1] == "all") {
                        // Reset completo
                        std::vector<std::string> eventi = serra.resetAll();
                        for (const auto& evento : eventi) {
                            std::cout << evento << std::endl;
                        }
                        std::cout << "[00:00] Sistema completamente resettato" << std::endl;
                    }
                    continue;
                }
            }

            // Se arriviamo qui, il comando non è stato riconosciuto
            std::cout << "Comando non riconosciuto. Digita 'help' per la lista dei comandi." << std::endl;

        } catch (const std::exception& e) {
            std::cerr << "Errore nell'esecuzione del comando: " << e.what() << std::endl;
        }
    }

    std::cout << "Chiusura del sistema di gestione serra." << std::endl;
    return 0;
}