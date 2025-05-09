#include "Orario.h"
#include <sstream>
#include <iomanip>

Orario::Orario() : ore(0), minuti(0) {
    // Costruttore di default che inizializza l'orario a 00:00
}

Orario::Orario(int _ore, int _minuti) {
    setOre(_ore);
    setMinuti(_minuti);
}

Orario::Orario(const std::string& orarioStr) {
    if (!fromString(orarioStr)) {
        // Se il formato non è valido, inizializza a 00:00
        ore = 0;
        minuti = 0;
    }
}

void Orario::setOre(int _ore) {
    // Assicura che le ore siano nel range 0-23
    if (_ore >= 0 && _ore < 24) {
        ore = _ore;
    } else {
        ore = _ore % 24;
        if (ore < 0) ore += 24;  // Gestisce il caso di valori negativi
    }
}

void Orario::setMinuti(int _minuti) {
    // Assicura che i minuti siano nel range 0-59
    if (_minuti >= 0 && _minuti < 60) {
        minuti = _minuti;
    } else {
        int oreExtra = _minuti / 60;
        minuti = _minuti % 60;
        if (minuti < 0) {
            minuti += 60;
            oreExtra--;
        }
        setOre(ore + oreExtra);
    }
}

bool Orario::fromString(const std::string& orarioStr) {
    // Formato atteso: "HH:MM"
    std::istringstream ss(orarioStr);
    int h, m;
    char delim;
    
    ss >> h >> delim >> m;
    
    if (ss.fail() || delim != ':' || h < 0 || h >= 24 || m < 0 || m >= 60) {
        return false;
    }
    
    setOre(h);
    setMinuti(m);
    return true;
}

std::string Orario::toString() const {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << ore << ":" 
       << std::setfill('0') << std::setw(2) << minuti;
    return ss.str();
}

double Orario::differenzaInOre(const Orario& altro) const {
    // Calcola la differenza in ore tra i due orari
    int minThis = ore * 60 + minuti;
    int minAltro = altro.ore * 60 + altro.minuti;
    int diffMin = minAltro - minThis;
    
    // Se la differenza è negativa, assumiamo che l'altro orario sia il giorno successivo
    if (diffMin < 0) {
        diffMin += 24 * 60;  // Aggiungi un giorno in minuti
    }
    
    return diffMin / 60.0;  // Converti in ore (con decimali)
}

int Orario::differenzaInMinuti(const Orario& altro) const {
    // Calcola la differenza in minuti tra i due orari
    int minThis = ore * 60 + minuti;
    int minAltro = altro.ore * 60 + altro.minuti;
    int diffMin = minAltro - minThis;
    
    // Se la differenza è negativa, assumiamo che l'altro orario sia il giorno successivo
    if (diffMin < 0) {
        diffMin += 24 * 60;  // Aggiungi un giorno in minuti
    }
    
    return diffMin;
}

bool Orario::isPrimaDi(const Orario& altro) const {
    return *this < altro;
}

bool Orario::isDopoD(const Orario& altro) const {
    return *this > altro;
}

bool Orario::isUgualeA(const Orario& altro) const {
    return *this == altro;
}

void Orario::incrementa(int _minuti) {
    // Incrementa l'orario del numero di minuti specificato
    int totalMinuti = ore * 60 + minuti + _minuti;
    
    // Gestisci il caso in cui i minuti totali superino un giorno
    totalMinuti = totalMinuti % (24 * 60);
    if (totalMinuti < 0) totalMinuti += 24 * 60;
    
    ore = totalMinuti / 60;
    minuti = totalMinuti % 60;
}

void Orario::reset() {
    // Reimposta l'orario a 00:00
    ore = 0;
    minuti = 0;
}

// Implementazione degli operatori di confronto
bool Orario::operator==(const Orario& altro) const {
    return ore == altro.ore && minuti == altro.minuti;
}

bool Orario::operator!=(const Orario& altro) const {
    return !(*this == altro);
}

bool Orario::operator<(const Orario& altro) const {
    if (ore < altro.ore) return true;
    if (ore > altro.ore) return false;
    return minuti < altro.minuti;
}

bool Orario::operator<=(const Orario& altro) const {
    return *this < altro || *this == altro;
}

bool Orario::operator>(const Orario& altro) const {
    return !(*this <= altro);
}

bool Orario::operator>=(const Orario& altro) const {
    return !(*this < altro);
}