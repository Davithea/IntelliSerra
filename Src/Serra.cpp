#include "Serra.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

Serra::Serra() : orarioAttuale(), prossimoId(1) {
    // Inizializza la serra con l'orario attuale a 00:00
    // e nessun impianto
}

size_t Serra::trovaIndiceImpianto(int id) const {
    for (size_t i = 0; i < impianti.size(); ++i) {
        if (impianti[i]->getId() == id) {
            return i;
        }
    }
    return impianti.size(); // Indice non valido -> impianto non trovato
}

size_t Serra::trovaIndiceImpiantoDaNome(const std::string& nome) const {
    for (size_t i = 0; i < impianti.size(); ++i) {
        if (impianti[i]->getNome() == nome) {
            return i;
        }
    }
    return impianti.size(); // Indice non valido -> impianto non trovato
}

int Serra::aggiungiImpianto(std::unique_ptr<Impianto> impianto) {
    if (!impianto) {
        return -1; // Impianto non valido
    }

    // Verifica che non esista già un impianto con lo stesso nome
    if (esisteImpianto(impianto->getNome())) {
        return -1; // Nome già esistente
    }

    int id = prossimoId++;
    // Poiché il puntatore viene trasferito, dobbiamo memorizzare il nome prima
    std::string nome = impianto->getNome();

    impianti.push_back(std::move(impianto));
    return id;
}

bool Serra::rimuoviImpianto(int id) {
    size_t indice = trovaIndiceImpianto(id);
    if (indice < impianti.size()) {
        impianti.erase(impianti.begin() + indice);
        return true;
    }
    return false;
}

bool Serra::rimuoviImpiantoDaNome(const std::string& nome) {
    size_t indice = trovaIndiceImpiantoDaNome(nome);
    if (indice < impianti.size()) {
        impianti.erase(impianti.begin() + indice);
        return true;
    }
    return false;
}

bool Serra::accendiImpianto(const std::string& nome) {
    size_t indice = trovaIndiceImpiantoDaNome(nome);
    if (indice < impianti.size()) {
        return impianti[indice]->accendi(orarioAttuale);
    }
    return false;
}

bool Serra::spegniImpianto(const std::string& nome) {
    size_t indice = trovaIndiceImpiantoDaNome(nome);
    if (indice < impianti.size()) {
        return impianti[indice]->spegni(orarioAttuale);
    }
    return false;
}

bool Serra::impostaTimer(const std::string& nome, const Orario& oraInizio, const Orario& oraFine) {
    size_t indice = trovaIndiceImpiantoDaNome(nome);
    if (indice < impianti.size()) {
        return impianti[indice]->impostaTimer(oraInizio, oraFine);
    }
    return false;
}

bool Serra::rimuoviTimer(const std::string& nome) {
    size_t indice = trovaIndiceImpiantoDaNome(nome);
    if (indice < impianti.size()) {
        return impianti[indice]->rimuoviTimer();
    }
    return false;
}

std::vector<std::string> Serra::impostaOrario(const Orario& nuovoOrario) {
    std::vector<std::string> eventi;

    // Verifica che il nuovo orario sia valido (non nel passato)
    if (nuovoOrario < orarioAttuale) {
        std::stringstream ss;
        ss << "[ERRORE] Impossibile impostare un orario precedente a quello attuale ("
           << orarioAttuale.toString() << ")";
        eventi.push_back(ss.str());
        return eventi;
    }

    // Costruisci un evento per l'aggiornamento dell'orario
    std::stringstream ssOrario;
    ssOrario << "[" << nuovoOrario.toString() << "] L'orario attuale è " << nuovoOrario.toString();

    // Per ogni impianto, aggiorna lo stato e raccogli gli eventi generati
    for (auto& impianto : impianti) {
        bool statoModificato = impianto->aggiorna(orarioAttuale, nuovoOrario);

        if (statoModificato) {
            std::stringstream ss;
            if (impianto->isAttivo()) {
                ss << "[" << nuovoOrario.toString() << "] L'impianto \""
                   << impianto->getNome() << "\" si è acceso";
            } else {
                ss << "[" << nuovoOrario.toString() << "] L'impianto \""
                   << impianto->getNome() << "\" si è spento";
            }
            eventi.push_back(ss.str());
        }
    }

    // Aggiorna l'orario della serra
    orarioAttuale = nuovoOrario;

    // Aggiungi l'evento dell'orario alla fine dell'elenco
    eventi.push_back(ssOrario.str());

    return eventi;
}

Orario Serra::getOrarioAttuale() const {
    return orarioAttuale;
}

std::vector<std::string> Serra::resetOrario() {
    std::vector<std::string> eventi;

    // Spegni tutti gli impianti
    for (auto& impianto : impianti) {
        if (impianto->isAttivo()) {
            impianto->spegni(orarioAttuale);
            std::stringstream ss;
            ss << "[" << orarioAttuale.toString() << "] L'impianto \""
               << impianto->getNome() << "\" si è spento";
            eventi.push_back(ss.str());
        }

        // Rimuovi anche i timer
        impianto->rimuoviTimer();
    }

    // Reimposta l'orario a 00:00
    orarioAttuale.reset();

    std::stringstream ss;
    ss << "[00:00] L'orario è stato reimpostato a 00:00";
    eventi.push_back(ss.str());

    return eventi;
}

std::vector<std::string> Serra::resetTimer() {
    std::vector<std::string> eventi;

    // Rimuovi tutti i timer lasciando invariato lo stato degli impianti
    for (auto& impianto : impianti) {
        if (impianto->rimuoviTimer()) {
            std::stringstream ss;
            ss << "[" << orarioAttuale.toString() << "] Timer rimosso per l'impianto \""
               << impianto->getNome() << "\"";
            eventi.push_back(ss.str());
        }
    }

    std::stringstream ss;
    ss << "[" << orarioAttuale.toString() << "] Tutti i timer sono stati rimossi";
    eventi.push_back(ss.str());

    return eventi;
}

std::vector<std::string> Serra::resetAll() {
    std::vector<std::string> eventi;

    // Prima reset dei timer
    std::vector<std::string> eventiTimer = resetTimer();
    eventi.insert(eventi.end(), eventiTimer.begin(), eventiTimer.end());

    // Poi reset dell'orario
    std::vector<std::string> eventiOrario = resetOrario();
    eventi.insert(eventi.end(), eventiOrario.begin(), eventiOrario.end());

    // Messaggio finale
    std::stringstream ss;
    ss << "[00:00] Il sistema è stato ripristinato alle condizioni iniziali";
    eventi.push_back(ss.str());

    return eventi;
}

std::vector<std::string> Serra::mostraStatoImpianti() const {
    std::vector<std::string> stati;

    if (impianti.empty()) {
        stati.push_back("Nessun impianto presente nella serra");
        return stati;
    }

    // Calcola il consumo totale
    double consumoTotale = getConsumoIdricoTotale();

    // Intestazione
    std::stringstream ssIntestazione;
    ssIntestazione << "===== STATO SERRA - " << orarioAttuale.toString() << " =====";
    stati.push_back(ssIntestazione.str());

    // Aggiungi lo stato di ciascun impianto
    for (const auto& impianto : impianti) {
        stati.push_back(impianto->stampaStato());
    }

    // Piè di pagina con consumo totale
    std::stringstream ssFooter;
    ssFooter << "Consumo idrico totale: " << std::fixed << std::setprecision(2)
             << consumoTotale << " litri";
    stati.push_back(ssFooter.str());
    stati.push_back("========================================");

    return stati;
}

std::string Serra::mostraStatoImpianto(const std::string& nome) const {
    size_t indice = trovaIndiceImpiantoDaNome(nome);
    if (indice < impianti.size()) {
        return impianti[indice]->stampaStato();
    }
    return "Impianto \"" + nome + "\" non trovato";
}

double Serra::getConsumoIdricoTotale() const {
    double consumoTotale = 0.0;
    for (const auto& impianto : impianti) {
        consumoTotale += impianto->getConsumoIdrico();
    }
    return consumoTotale;
}

size_t Serra::getNumeroImpianti() const {
    return impianti.size();
}

bool Serra::esisteImpianto(const std::string& nome) const {
    return trovaIndiceImpiantoDaNome(nome) < impianti.size();
}