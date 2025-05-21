#ifndef IMPIANTO_H
#define IMPIANTO_H

#include <string>
#include "Orario.h"

using namespace std;

//Classe base impianto per la gestione di di un impianto
//Author: Davide Gastaldello
class Impianto {
protected:
    int id; //Identificatore per riconoscere in modo univoco un impianto
    string nome;    //Stringa per contenere il nome dell'impianto
    bool attivo;    //Variabile booleana per indicare se un impianto è attivo o meno
    Orario ultimaAttivazione;   //Orario per tenere traccia dell'ultima attivazione dell'impianto
    double consumoIdrico;       //Consumo totale di acqua
    double tassoConsumo;        //Tasso di consumo di acqua (litri/ora)
    bool modalitaAutomatica;    //Variabile booleana per indicare se l'impianto funziona in modalità automatica

public:
    Impianto(int _id, const string& _nome, double _tassoConsumo, bool _modalitaAutomatica); //Costruttore per l'impianto
    virtual ~Impianto() = default;  //Costruttore di default che può essere sovrascritto dalle classi secondarie
    virtual bool accendi(const Orario& orarioCorrente); //Funzione per accendere l'impianto
    virtual bool spegni(const Orario& orarioCorrente);  //Funzione per spegnere l'impianto
    virtual bool impostaTimer(const Orario& oraInizio, const Orario& oraFine);  //Funzione per impostare un timer
    virtual bool rimuoviTimer();    //Funzione per rimuovere un timer precedentemente impostato
    virtual bool aggiorna(const Orario& orarioPrecedente, const Orario& orarioAttuale) = 0; //Funzione per aggiornare lo stato di un impianto
    virtual std::string stampaStato() const;    //Funzione per stampare lo stato di un impianto
    double calcolaConsumo(double oreDiAttivita) const;  //Funzione per calcolare il consumo di un impianto
    virtual Impianto* clone() const = 0;

    //Propetry pubbliche per accedere agli attributi privati della classe
    int getId() const { return id; }
    std::string getNome() const { return nome; }
    bool isAttivo() const { return attivo; }
    Orario getUltimaAttivazione() const { return ultimaAttivazione; }
    double getConsumoIdrico() const { return consumoIdrico; }
    double getTassoConsumo() const { return tassoConsumo; }
    bool isModalitaAutomatica() const { return modalitaAutomatica; }
    bool isAutomatico() const;
    virtual int getDurataAutomatica() const; // virtual in caso sia ridefinito nelle sottoclassi

};

#endif //IMPIANTO_H