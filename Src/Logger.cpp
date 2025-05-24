#include "Logger.h"
#include <iostream>
#include <memory>
unique_ptr<Logger> globalLogger = nullptr;  //Dichiaro un puntatore globale al Logger

//Funzione per inizializzare il file di logging
//Author: Davide Gastaldello
DualLogger::DualLogger(const string& filename) : filename(filename) {
    logFile.open(filename, ios::app);   //Apro il file di logging in modalità 'append'
    if (logFile.is_open()) {    //Se il file è stato aperto correttamente scrivo che una nuova sezione è cominciata
        logFile << "\n========== NUOVA SESSIONE ==========\n";
        logFile.flush();
    }
}

//Funzione per chiudere una sessione di logging nel file
//Author: Davide Gastaldello
DualLogger::~DualLogger() { //Quando tremino l'esecuzione del codice, scrivo nel file di log che la sessione è terminata
    if (logFile.is_open()) {
        logFile << "========== FINE SESSIONE ==========\n\n";
        logFile.close();
    }
}

//Funzione per scrivere contemporaneamente in Console e nel file di testo
//Author: Davide Gastaldello (utilizzata struttura del codice fornito dal professore)
void DualLogger::logMessage(const Orario& time, const string& message, int errorLevel) const {
    //Stampa sulla console
    if (errorLevel == 0) {
        cout << "[" << time.toString() << "]\t" << message << endl;
    } else {
        cerr << "[" << time.toString() << "]\tERRORE: " << message << endl;
    }

    //Stampa sul file di testo
    if (logFile.is_open()) {
        logFile << "[" << time.toString() << "]\t";
        if (errorLevel > 0) {
            logFile << "ERRORE: ";
        }
        logFile << message << endl;
        logFile.flush();
    }
}

//Funzione booleana per controllare che il file di testo sia effettivamente aperto
//Author: Davide Gastaldello
bool DualLogger::isFileOpen() const {
    return logFile.is_open();
}

//Funzione per inizializzare il sistema di logging
//Author: Davide Gastaldello
void setupConsoleAndFile(const string& filename) {
    globalLogger = make_unique<DualLogger>(filename);  //Creo un nuovo oggetto DualLogger e lo assegno al puntatore globale
}

//Funzione per terminare il sistema di logging e liberare la memoria
//Author: Davide Gastaldello
void shutdownLogger() {
    globalLogger.reset();  //Resetto il puntatore unico per distruggere l'oggetto DualLogger e chiudere il file
}

//Funzione log che verrà chiamata nel codice e che richiama logMessage di DualLogger
//Author: Davide Gastaldello
void log(const Orario& time, const string& message, int errorLevel) {
    if (globalLogger) {
        globalLogger->logMessage(time, message, errorLevel);
    }
}