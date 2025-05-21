#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <string>
#include <vector>
#include "Orario.h"
#include "Serra.h"
#include "Logger.h"

using namespace std;

class UserInterface {
    Serra& serra;
    bool continua;

public:
    UserInterface(Serra& serra);    //Costruttore per l'interfacciaUtente
    ~UserInterface();   //Distruttore per l'interfaccia utente

    //Per commenti relativi alle funzioni della classe UserInterface vedi UserInterface.cpp
    bool isRunning() const;
    void stop();
    void run();
    vector<string> commandParser(const string& command);
    static Orario stringToOrario(const string& str);
    void stampaHelp();
    void processCommand(const string& command);
};

#endif //USERINTERFACE_H