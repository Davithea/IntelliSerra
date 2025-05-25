//=== ORARIO.CPP ===

#include "Orario.h"
#include <sstream>
#include <iomanip>

//Costruttore di default che inizializza l'orario a 00:00
//Author: Pietro Stocchiero
Orario::Orario() : ore(0), minuti(0) {
    // Costruttore di default che inizializza l'orario a 00:00
}

//Costruttore parametrizzato per creare un orario con ore e minuti specifici
//Author: Pietro Stocchiero
Orario::Orario(int _ore, int _minuti) {
    setOre(_ore);
    setMinuti(_minuti);
}

//Costruttore che crea un orario a partire da una stringa nel formato "HH:MM"
//Author: Pietro Stocchiero
Orario::Orario(const string& orarioStr) {
    if (!fromString(orarioStr)) {
        // Se il formato non è valido, inizializza a 00:00
        ore = 0;
        minuti = 0;
    }
}

//Funzione per impostare le ore con controllo di validità (0-23)
//Author: Pietro Stocchiero
void Orario::setOre(int _ore) {
    // Assicura che le ore siano nel range 0-23
    if (_ore >= 0 && _ore < 24) {
        ore = _ore;
    } else {
        ore = _ore % 24;
        if (ore < 0) ore += 24;  // Gestisce il caso di valori negativi
    }
}

//Funzione per impostare i minuti con controllo di validità (0-59) e gestione overflow
//Author: Pietro Stocchiero
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

//Funzione per parsare una stringa nel formato "HH:MM" e impostare l'orario
//Author: Pietro Stocchiero
bool Orario::fromString(const string& orarioStr) {
    // Formato atteso: "HH:MM"
    istringstream ss(orarioStr);
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

//Funzione che converte l'orario in una stringa formattata "HH:MM"
//Author: Pietro Stocchiero
string Orario::toString() const {
    stringstream ss;
    ss << setfill('0') << setw(2) << ore << ":"
       << setfill('0') << setw(2) << minuti;
    return ss.str();
}

//Funzione per calcolare la differenza in ore tra due orari
//Author: Pietro Stocchiero
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

//Funzione per calcolare la differenza in minuti tra due orari
//Author: Pietro Stocchiero
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

//Funzione per verificare se questo orario è precedente a un altro
//Author: Pietro Stocchiero
bool Orario::isPrimaDi(const Orario& altro) const {
    // Utilizziamo l'operatore < sovraccaricato
    return *this < altro;
}

//Funzione per verificare se questo orario è successivo a un altro
//Author: Pietro Stocchiero
bool Orario::isDopoD(const Orario& altro) const {
    // Utilizziamo l'operatore > sovraccaricato
    return *this > altro;
}

//Funzione per verificare se questo orario è uguale a un altro
//Author: Pietro Stocchiero
bool Orario::isUgualeA(const Orario& altro) const {
    // Utilizziamo l'operatore == sovraccaricato
    return *this == altro;
}

//Funzione per incrementare l'orario di un numero specificato di minuti
//Author: Pietro Stocchiero
void Orario::incrementa(int _minuti) {
    // Incrementa l'orario del numero di minuti specificato
    int totalMinuti = ore * 60 + minuti + _minuti;

    // Gestisci il caso in cui i minuti totali superino un giorno
    totalMinuti = totalMinuti % (24 * 60);
    if (totalMinuti < 0) totalMinuti += 24 * 60;

    ore = totalMinuti / 60;
    minuti = totalMinuti % 60;
}

//Funzione per reimpostare l'orario a 00:00
//Author: Pietro Stocchiero
void Orario::reset() {
    // Reimposta l'orario a 00:00
    ore = 0;
    minuti = 0;
}

//Operatore di uguaglianza per confrontare due orari
//Author: Pietro Stocchiero
bool Orario::operator==(const Orario& altro) const {
    return ore == altro.ore && minuti == altro.minuti;
}

//Operatore di diversità per confrontare due orari
//Author: Pietro Stocchiero
bool Orario::operator!=(const Orario& altro) const {
    return !(*this == altro);
}

//Operatore di minore per confrontare due orari
//Author: Pietro Stocchiero
bool Orario::operator<(const Orario& altro) const {
    if (ore < altro.ore) return true;
    if (ore > altro.ore) return false;
    return minuti < altro.minuti;
}

//Operatore di minore o uguale per confrontare due orari
//Author: Pietro Stocchiero
bool Orario::operator<=(const Orario& altro) const {
    return *this < altro || *this == altro;
}

//Operatore di maggiore per confrontare due orari
//Author: Pietro Stocchiero
bool Orario::operator>(const Orario& altro) const {
    return !(*this <= altro);
}

//Operatore di maggiore o uguale per confrontare due orari
//Author: Pietro Stocchiero
bool Orario::operator>=(const Orario& altro) const {
    return !(*this<altro);
}