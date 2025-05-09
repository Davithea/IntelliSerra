#include "Impianto.h"
#include <sstream>
#include <iomanip>

Impianto::Impianto(int _id, const std::string& _nome, double _tassoConsumo, bool _modalitaAutomatica)
    : id(_id),
      nome(_nome),
      attivo(false),
      consumoIdrico(0.0),
      tassoConsumo(_tassoConsumo),
      modalitaAutomatica(_modalitaAutomatica) {
    // Il costruttore inizializza l'impianto come spento
    // L'orario di ultima attivazione è inizializzato di default
}

bool Impianto::accendi(const Orario& orarioCorrente) {
    if (attivo) {
        // L'impianto è già acceso
        return false;
    }

    attivo = true;
    ultimaAttivazione = orarioCorrente;
    return true;
}

bool Impianto::spegni(const Orario& orarioCorrente) {
    if (!attivo) {
        // L'impianto è già spento
        return false;
    }

    // Calcoliamo il consumo d'acqua per il periodo di attività
    double oreAttivita = ultimaAttivazione.differenzaInOre(orarioCorrente);
    consumoIdrico += calcolaConsumo(oreAttivita);

    attivo = false;
    return true;
}

bool Impianto::impostaTimer(const Orario& oraInizio, const Orario& oraFine) {
    // Implementazione di base che sarà sovrascritta dalle classi derivate
    // che supportano timer
    return modalitaAutomatica;
}

bool Impianto::rimuoviTimer() {
    // Implementazione di base che sarà sovrascritta dalle classi derivate
    // che supportano timer
    return modalitaAutomatica;
}

std::string Impianto::stampaStato() const {
    std::stringstream ss;

    ss << "Impianto " << id << " (" << nome << "): ";
    ss << (attivo ? "ATTIVO" : "SPENTO");
    ss << " - Consumo totale: " << std::fixed << std::setprecision(2) << consumoIdrico << " litri";

    if (attivo) {
        ss << " - Ultima attivazione: " << ultimaAttivazione.toString();
    }

    return ss.str();
}

double Impianto::calcolaConsumo(double oreDiAttivita) const {
    return tassoConsumo * oreDiAttivita;
}