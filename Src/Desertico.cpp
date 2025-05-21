#include "Desertico.h"
#include <string>

ImpiantoDesertico::ImpiantoDesertico(int id, const std::string& nome, double tassoConsumo)
    : Impianto(id, nome, tassoConsumo, false) {  // Modalità automatica = false
    // L'impianto desertico inizia spento e non usa l'automatismo
}

bool ImpiantoDesertico::aggiorna(const Orario& orarioPrecedente, const Orario& orarioAttuale) {
    // L'impianto desertico non ha comportamento automatico
    return false;
}

bool ImpiantoDesertico::impostaTimer(const Orario& oraInizio, const Orario& oraFine) {
    // L'impianto desertico non supporta timer
    return false;
}

bool ImpiantoDesertico::rimuoviTimer() {
    // Non ci sono timer da rimuovere
    return false;
}

std::string ImpiantoDesertico::stampaStato() const {
    std::string stato = attivo ? "Attivo" : "Spento";
    return "[Desertico] " + nome + " (ID: " + std::to_string(id) + ") - Stato: " + stato +
           " | Consumo: " + std::to_string(consumoIdrico) + " L";
}

Impianto* ImpiantoDesertico::clone() const {
    return new ImpiantoDesertico(*this);
}