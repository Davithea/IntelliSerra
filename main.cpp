#include "Serra.h"
#include "UserInterface.h"

//main per l'avvio del programma e dell'interfaccia utente
//Author: Davide Gastaldello
int main() {
    Serra serra;    //Cero la serra

    UserInterface InterfacciaUtente(serra); //Creo l'interfaccia utente collegata alla serra

    InterfacciaUtente.run();    //Avvio l'interfaccia utente

    return 0;
}