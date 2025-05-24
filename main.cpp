#include "Serra.h"
#include "UserInterface.h"
#include "Logger.h"

//main per l'avvio del programma e dell'interfaccia utente
//Author: Davide Gastaldello
int main() {
    setupConsoleAndFile("serra_log.txt");   //Funzione per aprire il file di logger

    Serra serra;    //Cero la serra

    UserInterface InterfacciaUtente(serra); //Creo l'interfaccia utente collegata alla serra

    InterfacciaUtente.run();    //Avvio l'interfaccia utente

    shutdownLogger();   //Richiamo la funzione che distrugge il logger

    return 0;
}