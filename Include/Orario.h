//=== ORARIO.H ===

#ifndef ORARIO_H
#define ORARIO_H

#include <string>

using namespace std;

//Definizione della classe Orario per la gestione degli orari nel formato HH:MM
//Author: Pietro Stocchiero
class Orario {
    int ore;      //Ore (0-23)
    int minuti;   //Minuti (0-59)

public:
    //Per commenti relativi alle funzioni della classe Orario vedi Orario.cpp
    Orario();
    Orario(int _ore, int _minuti);
    Orario(const string& orarioStr);

    int getOre() const { return ore; }
    int getMinuti() const { return minuti; }
    void setOre(int _ore);
    void setMinuti(int _minuti);

    bool fromString(const string& orarioStr);

    std::string toString() const;

    double differenzaInOre(const Orario& altro) const;

    int differenzaInMinuti(const Orario& altro) const;

    bool isPrimaDi(const Orario& altro) const;

    bool isDopoD(const Orario& altro) const;

    bool isUgualeA(const Orario& altro) const;

    void incrementa(int minuti);

    void reset();

    // Operatori di confronto
    bool operator==(const Orario& altro) const;
    bool operator!=(const Orario& altro) const;
    bool operator<(const Orario& altro) const;
    bool operator<=(const Orario& altro) const;
    bool operator>(const Orario& altro) const;
    bool operator>=(const Orario& altro) const;
};

#endif //ORARIO_H