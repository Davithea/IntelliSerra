#ifndef IMPIANTO_H
#define IMPIANTO_H

#include <string>
#include "Orario.h"

class Impianto {
protected:
    int id;                     // Identificatore univoco dell'impianto
    std::string nome;           // Nome dell'impianto/tipo di pianta
    bool attivo;                // Stato corrente (true = acceso, false = spento)
    //Orario ultimaAttivazione;   // Orario dell'ultima attivazione
    double consumoIdrico;       // Consumo idrico totale (in litri)
    double tassoConsumo;        // Consumo idrico per unità di tempo (litri/ora)
    bool modalitaAutomatica;    // Indica se l'impianto è in modalità automatica

public:
    Impianto(int _id, const std::string& _nome, double _tassoConsumo, bool _modalitaAutomatica);

    virtual ~Impianto() = default;

    virtual bool accendi(const Orario& orarioCorrente);

    virtual bool spegni(const Orario& orarioCorrente);

    virtual bool impostaTimer(const Orario& oraInizio, const Orario& oraFine);

    virtual bool rimuoviTimer();

    virtual bool aggiorna(const Orario& orarioPrecedente, const Orario& orarioAttuale) = 0;

    virtual std::string stampaStato() const;

    // Metodi getter
    int getId() const { return id; }
    std::string getNome() const { return nome; }
    bool isAttivo() const { return attivo; }
    //Orario getUltimaAttivazione() const { return ultimaAttivazione; }
    double getConsumoIdrico() const { return consumoIdrico; }
    double getTassoConsumo() const { return tassoConsumo; }
    bool isModalitaAutomatica() const { return modalitaAutomatica; }

    double calcolaConsumo(double oreDiAttivita) const;

    virtual Impianto* clone() const = 0;
};

#endif // IMPIANTO_H