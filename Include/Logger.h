#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include "Orario.h"

// Interfaccia per il logging
class Logger {
public:
    virtual ~Logger() = default;
    virtual void logMessage(const Orario& time, const std::string& message, int errorLevel = 0) const = 0;
};

// Implementazione concreta del logger che scrive su console
class ConsoleLogger : public Logger {
public:
    void logMessage(const Orario& time, const std::string& message, int errorLevel = 0) const override;
};

// Implementazione globale del logger
extern Logger* globalLogger;

// Funzione di inizializzazione del logger globale
void initializeLogger(Logger* logger);

// Funzione di utility per il logging
void log(const Orario& time, const std::string& message, int errorLevel = 0);

#endif // LOGGER_H