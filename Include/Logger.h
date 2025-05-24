#ifndef LOGGER_H
#define LOGGER_H

#include <string> //Includo la libreria per usare le stringhe
#include <fstream> //Includo la libreria per gestire file di output (ofstream)
#include "Orario.h" //Includo la definizione della classe Orario

using namespace std;

//Definisco una classe generale Logger che verrà sovrascritta dai possibili logger (logger che scrive solo su Console, Console e File o solo File)
//Author: Davide Gastaldello
class Logger {
public:
    virtual ~Logger() = default; //Creo un distruttore virtuale per permettere la cancellazione corretta del logger
    virtual void logMessage(const Orario& time, const string& message, int errorLevel = 0) const = 0; //Definisco un metodo virtuale puro che ogni logger deve implementare
};

//Definisco la classe DualLogger che permette di scrivere contemporaneamente su file e su Console
//Author: Davide Gastaldello
class DualLogger : public Logger {
    string filename;    //Memorizzo il nome del file per logging su file
    mutable ofstream logFile;   //Uso mutable per scrivere su file anche in un metodo const

public:
    explicit DualLogger(const string& nomeFile); //Costruttore che riceve il nome del file
    ~DualLogger(); //Distruttore per chiudere il file se necessario
    void logMessage(const Orario& time, const string& message, int errorLevel = 0) const override; //Scrivo su console e file
    bool isFileOpen() const; //Controllo se il file è aperto correttamente
};

void setupConsoleAndFile(const string& nomeFile); //Imposto il logger globale per scrivere sia su console che su file
void shutdownLogger(); //Distruggo il logger globale e rilascio risorse

//Funzione di logging
void log(const Orario& time, const string& message, int errorLevel = 0);  //Funzione globale per la scrittura nel logger (usato solo per i messaggi più importanti che voglio venagno salvati nel file)

#endif //LOGGER_H
