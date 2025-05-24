#include "./Mediterraneo.h"
#include <sstream>
#include <iomanip>
#include <cmath>

ImpiantoMediterraneo::ImpiantoMediterraneo(int id, const std::string& nome, bool /*_isWeekend*/, double tassoConsumo)
    : Impianto(id, nome, tassoConsumo, true, "mediterraneo"), generatore(std::random_device{}()) {}

bool ImpiantoMediterraneo::aggiorna(const Orario& orarioPrecedente, const Orario& orarioAttuale) {
    bool statoModificato = false;

    int minutiTrascorsi = orarioPrecedente.differenzaInMinuti(orarioAttuale);
    if (minutiTrascorsi <= 0) return false;

    if (attivo) {
        uniform_real_distribution aumentoTemp(0.75, 1.0);
        double aumentoTotale = (minutiTrascorsi / 60.0) * aumentoTemp(generatore);
        temperatura += aumentoTotale;

        if (temperatura >= 28.0) {
            temperatura = 28.0;
            spegni(orarioAttuale);
            statoModificato = true;
        }
    } else {
        std::uniform_real_distribution<double> decrementoTemp(0.01, 0.05);
        for (int i = 0; i < minutiTrascorsi; ++i) {
            temperatura -= decrementoTemp(generatore);
        }
        if (temperatura < 0) temperatura = 0;

        if (temperatura < 25.0) {
            attivo = true;
            ultimaAttivazione = orarioAttuale;
            statoModificato = true;
        }
    }

    if (!attivo && orarioPrecedente < prossimaAttivazione && prossimaAttivazione <= orarioAttuale) {
        attivo = true;
        ultimaAttivazione = prossimaAttivazione;
        statoModificato = true;

        prossimoSpegnimento = prossimaAttivazione;
        prossimoSpegnimento.incrementa(static_cast<int>(DURATA_ATTIVAZIONE_ORE * 60));
    }

    if (attivo && orarioPrecedente < prossimoSpegnimento && prossimoSpegnimento <= orarioAttuale) {
        spegni(prossimoSpegnimento);
        statoModificato = true;
    }

    return statoModificato;
}

bool ImpiantoMediterraneo::impostaTimer(const Orario& oraInizio, const Orario& oraFine) {
    if (!modalitaAutomatica) return false;
    if (oraFine <= oraInizio) return false;

    prossimaAttivazione = oraInizio;
    prossimoSpegnimento = oraFine;
    return true;
}

bool ImpiantoMediterraneo::rimuoviTimer() {
    if (!modalitaAutomatica) return false;

    prossimaAttivazione = Orario();
    prossimoSpegnimento = Orario();
    return true;
}

std::string ImpiantoMediterraneo::stampaStato() const {
    std::stringstream ss;
    ss << Impianto::stampaStato();

    ss << " - Tipo: Mediterraneo";
    ss << " - Temperatura: " << std::fixed << std::setprecision(2) << temperatura << "°C";

    return ss.str();
}

Impianto* ImpiantoMediterraneo::clone() const {
    return new ImpiantoMediterraneo(*this);
}