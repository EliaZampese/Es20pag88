#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>

using namespace std;

class Computer {
private:
    int codice;
    string marca;
    string modello;
    double velocitaProcessore;
    int memoriaRAM;
    int dimensioneDisco;
    double dimensioneMonitor;
    int annoAcquisto;
    static int prossimoCodice;

public:
    Computer(string mar, string mod, double vel, int ram, int disco,
             double monitor, int anno) {
        codice = prossimoCodice++;
        marca = mar;
        modello = mod;
        velocitaProcessore = vel;
        memoriaRAM = ram;
        dimensioneDisco = disco;
        dimensioneMonitor = monitor;
        annoAcquisto = anno;
    }

    Computer(int cod, string mar, string mod, double vel, int ram,
             int disco, double monitor, int anno) {
        codice = cod;
        marca = mar;
        modello = mod;
        velocitaProcessore = vel;
        memoriaRAM = ram;
        dimensioneDisco = disco;
        dimensioneMonitor = monitor;
        annoAcquisto = anno;

        if (cod >= prossimoCodice) {
            prossimoCodice = cod + 1;
        }
    }

    int getCodice() const {
        return codice;
    }

    string getMarca() const {
        return marca;
    }

    string getModello() const {
        return modello;
    }

    double getVelocitaProcessore() const {
        return velocitaProcessore;
    }

    int getMemoriaRAM() const {
        return memoriaRAM;
    }

    int getDimensioneDisco() const {
        return dimensioneDisco;
    }

    double getDimensioneMonitor() const {
        return dimensioneMonitor;
    }

    int getAnnoAcquisto() const {
        return annoAcquisto;
    }

    void visualizza() const {
        cout << "Codice: " << codice
             << " | Marca: " << marca
             << " | Modello: " << modello
             << " | CPU: " << velocitaProcessore << " GHz"
             << " | RAM: " << memoriaRAM << " GB"
             << " | Disco: " << dimensioneDisco << " GB"
             << " | Monitor: " << dimensioneMonitor << "\""
             << " | Anno: " << annoAcquisto << endl;
    }
};

int Computer::prossimoCodice = 1;

class CatalogoComputer {
private:
    vector<Computer> computerList;

public:
    void aggiungiComputer(string marca, string modello, double vel,
                          int ram, int disco, double monitor, int anno) {
        Computer c(marca, modello, vel, ram, disco, monitor, anno);
        computerList.push_back(c);

        cout << "Computer aggiunto con successo! Codice assegnato: "
             << c.getCodice() << endl;
    }

    bool rimuoviPerCodice(int codice) {
        for (auto it = computerList.begin(); it != computerList.end(); ++it) {
            if (it->getCodice() == codice) {
                computerList.erase(it);
                cout << "Computer con codice " << codice
                     << " eliminato con successo." << endl;
                return true;
            }
        }

        cout << "Nessun computer trovato con il codice specificato."
             << endl;
        return false;
    }

    void cercaPerCodice(int codice) const {
        for (const auto& c : computerList) {
            if (c.getCodice() == codice) {
                cout << "Computer trovato:" << endl;
                c.visualizza();
                return;
            }
        }

        cout << "Nessun computer trovato con il codice "
             << codice << "." << endl;
    }

    void cercaMigliori(double minVel, int minRam, int minDisco) const {
        bool trovato = false;

        cout << "--- Computer con CPU > " << minVel
             << " GHz, RAM >= " << minRam
             << " GB, Disco >= " << minDisco << " GB ---" << endl;

        for (const auto& c : computerList) {
            if (c.getVelocitaProcessore() > minVel &&
                c.getMemoriaRAM() >= minRam &&
                c.getDimensioneDisco() >= minDisco) {
                c.visualizza();
                trovato = true;
            }
        }

        if (!trovato) {
            cout << "Nessun computer soddisfa i criteri di ricerca."
                 << endl;
        }
    }

    void salvaSuFile(const string& nomeFile) const {
        ofstream file(nomeFile);

        if (!file.is_open()) {
            cout << "Errore nell'apertura del file per la scrittura."
                 << endl;
            return;
        }

        for (const auto& c : computerList) {
            file << c.getCodice() << ","
                 << c.getMarca() << ","
                 << c.getModello() << ","
                 << c.getVelocitaProcessore() << ","
                 << c.getMemoriaRAM() << ","
                 << c.getDimensioneDisco() << ","
                 << c.getDimensioneMonitor() << ","
                 << c.getAnnoAcquisto() << "\n";
        }

        if (!file) {
            cout << "Errore durante il salvataggio del catalogo."
                 << endl;
            return;
        }

        file.close();
        cout << "Catalogo salvato correttamente sul file "
             << nomeFile << "." << endl;
    }

    void caricaDaFile(const string& nomeFile) {
        ifstream file(nomeFile);

        if (!file.is_open()) {
            cout << "Errore nell'apertura del file per la lettura."
                 << endl;
            return;
        }

        vector<Computer> nuoviComputer;
        string linea;
        int numeroRiga = 0;

        try {
            while (getline(file, linea)) {
                numeroRiga++;

                if (linea.empty()) {
                    continue;
                }

                stringstream ss(linea);
                string campi[8];

                for (int i = 0; i < 8; i++) {
                    if (!getline(ss, campi[i], ',')) {
                        throw runtime_error("Numero di campi non valido");
                    }
                }

                string extra;
                if (getline(ss, extra, ',')) {
                    throw runtime_error("Numero di campi non valido");
                }

                int codice = stoi(campi[0]);
                string marca = campi[1];
                string modello = campi[2];
                double vel = stod(campi[3]);
                int ram = stoi(campi[4]);
                int disco = stoi(campi[5]);
                double monitor = stod(campi[6]);
                int anno = stoi(campi[7]);

                if (codice <= 0 || vel <= 0 || ram <= 0 ||
                    disco <= 0 || monitor <= 0 || anno <= 0 ||
                    marca.empty() || modello.empty()) {
                    throw runtime_error("Valori non validi");
                }

                nuoviComputer.emplace_back(
                    codice, marca, modello, vel, ram, disco, monitor, anno
                );
            }
        } catch (const exception&) {
            cout << "Errore nel formato dei dati alla riga "
                 << numeroRiga << ". Catalogo non caricato." << endl;
            return;
        }

        if (file.bad()) {
            cout << "Errore durante la lettura del file." << endl;
            return;
        }

        computerList = nuoviComputer;

        cout << "Catalogo ripristinato correttamente dal file "
             << nomeFile << "." << endl;
    }

    void visualizzaTutti() const {
        if (computerList.empty()) {
            cout << "Il catalogo dei computer e' attualmente vuoto."
                 << endl;
            return;
        }

        cout << "=== ELENCO COMPLETO COMPUTER ("
             << computerList.size() << ") ===" << endl;

        for (const auto& c : computerList) {
            c.visualizza();
        }
    }
};

int main() {
    CatalogoComputer catalogo;
    int scelta;

    do {
        cout << "\n=====================================" << endl;
        cout << "    GESTIONE CATALOGO COMPUTER" << endl;
        cout << "=====================================" << endl;
        cout << "1. Aggiungi nuovo computer" << endl;
        cout << "2. Elimina computer per codice" << endl;
        cout << "3. Cerca computer per codice" << endl;
        cout << "4. Cerca computer con specifiche superiori" << endl;
        cout << "5. Visualizza tutti i computer" << endl;
        cout << "6. Salva catalogo su file" << endl;
        cout << "7. Ripristina catalogo da file" << endl;
        cout << "0. Esci" << endl;
        cout << "Seleziona un'opzione: ";

        if (!(cin >> scelta)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Inserimento non valido." << endl;
            continue;
        }

        switch (scelta) {
            case 1: {
                string marca, modello;
                double vel, monitor;
                int ram, disco, anno;

                cout << "Inserisci marca: ";
                cin >> marca;

                cout << "Inserisci modello: ";
                cin >> modello;

                cout << "Inserisci velocita processore in GHz: ";
                cin >> vel;

                cout << "Inserisci memoria RAM in GB: ";
                cin >> ram;

                cout << "Inserisci dimensione disco in GB: ";
                cin >> disco;

                cout << "Inserisci dimensione monitor in pollici: ";
                cin >> monitor;

                cout << "Inserisci anno di acquisto: ";
                cin >> anno;

                if (!cin) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Dati inseriti non validi." << endl;
                    break;
                }

                if (vel <= 0 || ram <= 0 || disco <= 0 ||
                    monitor <= 0 || anno <= 0) {
                    cout << "I valori numerici devono essere positivi."
                         << endl;
                    break;
                }

                catalogo.aggiungiComputer(
                    marca, modello, vel, ram, disco, monitor, anno
                );
                break;
            }

            case 2: {
                int codice;

                cout << "Inserisci il codice del computer da rimuovere: ";
                cin >> codice;

                if (!cin) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Codice non valido." << endl;
                    break;
                }

                catalogo.rimuoviPerCodice(codice);
                break;
            }

            case 3: {
                int codice;

                cout << "Inserisci il codice da cercare: ";
                cin >> codice;

                if (!cin) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Codice non valido." << endl;
                    break;
                }

                catalogo.cercaPerCodice(codice);
                break;
            }

            case 4: {
                double minVel;
                int minRam, minDisco;

                cout << "Inserisci velocita minima CPU (GHz): ";
                cin >> minVel;

                cout << "Inserisci RAM minima (GB): ";
                cin >> minRam;

                cout << "Inserisci disco minimo (GB): ";
                cin >> minDisco;

                if (!cin) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Valori inseriti non validi." << endl;
                    break;
                }

                catalogo.cercaMigliori(minVel, minRam, minDisco);
                break;
            }

            case 5:
                catalogo.visualizzaTutti();
                break;

            case 6:
                catalogo.salvaSuFile("catalogo_computer.txt");
                break;

            case 7:
                catalogo.caricaDaFile("catalogo_computer.txt");
                break;

            case 0:
                cout << "Chiusura del programma in corso..." << endl;
                break;

            default:
                cout << "Opzione non valida. Riprova." << endl;
        }

    } while (scelta != 0);

    return 0;
}