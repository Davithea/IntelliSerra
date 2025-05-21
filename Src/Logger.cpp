#include "Logger.h"
#include <iostream>

// Variabile globale per il logger
Logger* globalLogger = nullptr;

// Inizializzazione del logger globale
void initializeLogger(Logger* logger) {
    if (globalLogger) {
        delete globalLogger; // Elimina il logger precedente se esiste
    }
    globalLogger = logger;
}

// Funzione di utility per il logging
void log(const Orario& time, const std::string& message, int errorLevel) {
    if (globalLogger) {
        globalLogger->logMessage(time, message, errorLevel);
    }
}

// Implementazione di ConsoleLogger::logMessage
void ConsoleLogger::logMessage(const Orario& time, const std::string& message, int errorLevel) const {
    if (errorLevel == 0) {
        std::cout << "[" << time.toString() << "]\t" << message << std::endl;
    } else if (errorLevel == 1) {
        std::cerr << "[" << time.toString() << "]\t" << message << std::endl;
    }
}