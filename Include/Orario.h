#ifndef ORARIO_H
#define ORARIO_H

#include <string>

class Orario {
    int ore;
    int minuti;

public:
    Orario();
    Orario(int _ore, int _minuti);
    Orario(const std::string& orarioStr);

    int getOre() const { return ore; }
    int getMinuti() const { return minuti; }
    void setOre(int _ore);
    void setMinuti(int _minuti);

    bool fromString(const std::string& orarioStr);

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

#endif // ORARIO_H
