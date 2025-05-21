//
// Created by cracc on 21/05/2025.
//
#include "Carnivoro.h"

Carnivora::Carnivora(int id, const std::string& nome, double tassoConsumo)
    : Impianto(id, nome, tassoConsumo, true) {}

bool Carnivora::aggiorna(const Orario& orarioPrecedente, const Orario& orarioAttuale) {
    if (modalitaAutomatica) {
        if (attivo && orarioAttuale >= prossimoSpegnimento) {
            spegni(orarioAttuale);
            calcolaProssimaAttivazione(orarioAttuale);
        } else if (!attivo && orarioAttuale >= prossimaAttivazione) {
            accendi(orarioAttuale);
            prossimoSpegnimento = orarioAttuale;
            prossimoSpegnimento.incrementa(static_cast<int>(DURATA_ATTIVAZIONE_ORE * 60));
        }
    }
    return true;
}

bool Carnivora::impostaTimer(const Orario& oraInizio, const Orario& oraFine) {
    prossimaAttivazione = oraInizio;
    prossimoSpegnimento = oraFine;
    return true;
}

bool Carnivora::rimuoviTimer() {
    prossimaAttivazione.reset();
    prossimoSpegnimento.reset();
    return true;
}

std::string Carnivora::stampaStato() const {
    return "Impianto Carnivora [" + nome + "] - Stato: " + (attivo ? "Attivo" : "Spento");
}

Impianto* Carnivora::clone() const {
    return new Carnivora(*this);
}

void Carnivora::calcolaProssimaAttivazione(const Orario& orarioAttuale) {
    prossimaAttivazione = orarioAttuale;
    prossimaAttivazione.incrementa(static_cast<int>(INTERVALLO_ORE*60));
}