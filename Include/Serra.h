#ifndef SERRA_H
#define SERRA_H

#include <vector>
#include <memory>
#include <string>
#include "Impianto.h"
#include "Orario.h"

class Serra {
private:
    std::vector<std::unique_ptr<Impianto>> impianti;    // Collezione di impianti
    Orario orarioAttuale;                               // Orario corrente del sistema
    int prossimoId;                                     // ID incrementale per nuovi impianti
    size_t trovaIndiceImpianto(int id) const;
    size_t trovaIndiceImpiantoDaNome(const std::string& nome) const;

public:
    Serra();
    int aggiungiImpianto(std::unique_ptr<Impianto> impianto);
    bool rimuoviImpianto(int id);
    bool rimuoviImpiantoDaNome(const std::string& nome);
    bool accendiImpianto(const std::string& nome);
    bool spegniImpianto(const std::string& nome);
    bool impostaTimer(const std::string& nome, const Orario& oraInizio, const Orario& oraFine);
    bool rimuoviTimer(const std::string& nome);
    std::vector<std::string> impostaOrario(const Orario& nuovoOrario);
    Orario getOrarioAttuale() const;
    std::vector<std::string> resetOrario();
    std::vector<std::string> resetTimer();
    std::vector<std::string> resetAll();
    std::vector<std::string> mostraStatoImpianti() const;
    std::string mostraStatoImpianto(const std::string& nome) const;
    double getConsumoIdricoTotale() const;
    size_t getNumeroImpianti() const;
    bool esisteImpianto(const std::string& nome) const;
};

#endif // SERRA_H