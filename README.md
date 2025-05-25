
# INTELLI-SERRA

Questo progetto implementa una libreria in C++ per la gestione automatizzata di una serra intelligente. Il sistema simula il controllo di impianti di irrigazione specifici per diverse tipologie di piante, con meccanismi sia automatici che manuali, e tiene traccia del consumo idrico e dello stato di ogni impianto.


## 🎯 Funzionalità principali
- Gestione di diversi tipi di impianti tramite classi derivate.
- Attivazione automatica/manuale in base all'orario simulato.
- Logica di irrigazione personalizzata per tipo di pianta.
- Controllo da terminale con comandi (vedi sezione 'Comandi disponibili')
- Simulazione dalla durata di 24 ore (00:00 → 23:59).
- Visualizzazione dello stato degli impianti e del consumo idrico.
## 🌱 Tipi di impianti supportati

- **Tropicale** – Irrigazione automatica ogni 2,5 ore.
- **Desertico** – Irrigazione solo manuale.
- **Mediterraneo** – Sistema adattivo basato sulla temperatura.
- **Alpino** – Irrigazione automatica ogni 1 ora.
- **Carnivoro** – Irrigazione automatica ogni 1,5 ore.
## 📂 Struttura del codice

- `main.cpp` – Entry point e gestione input da terminale.
- `Serra.h/cpp` – Classe centrale che gestisce gli impianti.
- `Orario.h/cpp` – Gestione e simulazione del tempo.
- `Impianto.h/cpp` – Classe base dalla quale ereditano tutti gli impianti.
- `ImpiantoTropicale.h/cpp`, `ImpiantoDesertico.h/cpp`, ecc. – Implementazioni concrete.
- `Logger.h/cpp` – Funzioni di supporto e logging.
- `CMakeLists.txt` – File per la compilazione del progetto.
## 🧪 Comandi disponibili

- `add {PLANTTYPE} {PLANTNAME}` - Aggiunge un impianto con nome e tipo specificati.
- `set {PLANTNAME} on/off` – Accende o spegne manualmente un impianto.
- `set {PLANTNAME} HH:MM [HH:MM]` – Imposta un timer automatico (facoltativo spegnimento).
- `rm {PLANTNAME}` – Rimuove un timer da un impianto.
- `show` – Mostra lo stato di tutti gli impianti.
- `show {PLANTNAME}` – Mostra lo stato dettagliato di un impianto.
- `set time HH:MM` – Imposta l’orario attuale.
- `reset time` – Reimposta l'orario a 00:00 e spegne gli impianti.
- `reset timers` – Rimuove tutti i timer ma mantiene lo stato.
- `reset all` – Reimposta tutto (timer, orario, impianti).
## 📌 Funzionalità aggiuntive
- È stato implementato un avanzamento dinamico del tempo: 30 minuti per impianti normali, 1 minuto per il range critico (23:30–23:59).
- Il codice è corredato da un file di log, chiamato `serra_log.txt`, che salva i dati di ogni esecuzione del programma. Come richiesto, a scopo esemplificativo, al suo interno sono già presenti i log di due sessioni.

## 📷 Rappresentazione grafica delle classi

![Immagine UML](https://files.catbox.moe/0bvibv.png)
## Authors

- [@davithea](https://www.github.com/davithea)
- [@LeonardoCracco](https://github.com/LeonardoCracco)
- [@PietroStoc](https://github.com/PietroStoc)

