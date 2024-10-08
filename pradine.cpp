#include "Lib.h"

//struktura studento duomenims saugoti
struct Studentas {
   string vardas;
   string pavarde;
   vector<int> namuDarbai;
   int egzaminas;
   double galutinis;
};


//funkcija galutiniam balui apskaiciuoti naudojant vidurki
double skaiciuotiGalutiniVidurkiu(const vector<int> namuDarbai, int egzaminas) {
   double vidurkis = 0.0;
   for (int nd: namuDarbai) {
      vidurkis += nd;
   }

   vidurkis /= namuDarbai.size();

   return vidurkis * 0.4 + egzaminas * 0.6;
}

//funkcija galutiniam balui apskaiciuoti naudojant mediana
double skaiciuotiGalutiniMediana(vector<int> namuDarbai, int egzaminas) {
    sort(namuDarbai.begin(), namuDarbai.end()) ;
    double mediana;
    int size = namuDarbai.size();

    if (size % 2 == 0) {
        mediana = (namuDarbai[size / 2 - 1] + namuDarbai[size / 2]) / 2.0; 
    } else {
        mediana = namuDarbai[size / 2];
    }

    return mediana * 0.4 + egzaminas * 0.6;

}

//funkcija generuoti atsitiktinius skaicius
int generuotiSkaiciu() {
    return rand() % 10 + 1;
}

//funckija sugeneruoti atsitiktinius namu darbu ir egzamino pazymius
void generuotiDuomenis(Studentas& Lok, int ndSkaicius) {
   Lok.namuDarbai.clear();
   cout << "Sugeneruoti namų darbų pažymiai: ";
   for (int i = 0; i < ndSkaicius; i++) {
      int pazymys = generuotiSkaiciu();
      Lok.namuDarbai.push_back(pazymys);
      cout << pazymys << " ";
   }
   Lok.egzaminas = generuotiSkaiciu();
   cout << "\nSugeneruotas egzamino rezultatas: " << Lok.egzaminas << endl;
}

//funkcija ivesti studento varda ir pavarde
void ivestiVardaPavarde(Studentas& Lok) {
   cout << "Įveskite studento vardą: " << endl;
   cin >> Lok.vardas;
   cout << "Įveskite studento pavardę: " << endl;
   cin >> Lok.pavarde;
   cin.ignore();
}

//funkcija ivesti pazymius ranka
void ivedimas(Studentas &Lok) {
   ivestiVardaPavarde(Lok);

   cout << "Įveskite namų darbų pažymius (Kai baigsite įvedimą, spauskite dukart Enter klavišą):" << endl;

   Lok.namuDarbai.clear();
   string input;
   int pazymys;

   while (true) {
      getline(cin, input);
      if (input.empty()) {
         break;
      }
      stringstream ss(input);
      if (ss >> pazymys) {
         Lok.namuDarbai.push_back(pazymys);
      } else {
         cout << "Netinkama įvestis, bandykite dar kartą." << endl;
      }

   }

   cout << "Įveskite egzamino rezultatą: " << endl;
   cin >> Lok.egzaminas;

};

//funkcija pasirinkti duomenu ivedimo buda
void pasirinktiDuomenuIvedimoBuda(Studentas& Lok) {
   char pasirinkimas;
   cout << "Ar norite įvesti pažymius rankiniu būdu (R) ar generuoti automatiškai (A)? ";
   cin >> pasirinkimas;
   pasirinkimas = toupper(pasirinkimas);

   if (pasirinkimas == 'R') {
      ivedimas(Lok);
   } else if (pasirinkimas == 'A') {
      ivestiVardaPavarde(Lok);
      int ndSkaicius;
      cout << "Įveskite, kiek namų darbų pažymių sugeneruoti: ";
      cin >> ndSkaicius;
      generuotiDuomenis(Lok, ndSkaicius);
   }

}


//funkcija apskaiciuoti galutini bala
void skaiciuotiGalutini(Studentas& Lok, char pasirinkimas) {
     if (pasirinkimas == 'V') {
      Lok.galutinis = skaiciuotiGalutiniVidurkiu(Lok.namuDarbai, Lok.egzaminas);
   } else if (pasirinkimas == 'M') {
      Lok.galutinis = skaiciuotiGalutiniMediana(Lok.namuDarbai, Lok.egzaminas);
   }
}

//funkcija isvesti duomenis
void isvedimas(const vector<Studentas>& studentai, char pasirinkimas) {
   if (pasirinkimas == 'V') {
      cout << left << setw(15) << "Pavarde" << setw(15) << "Vardas" << setw(15) << "Galutinis (Vid.)" << endl;
   } else if (pasirinkimas == 'M') {
      cout << left << setw(15) << "Pavarde" << setw(15) << "Vardas" << setw(15) << "Galutinis (Med.)" << endl;
   }

   cout << "-----------------------------------------------" << endl;
   for (const auto& Lok: studentai) {
      cout << left << setw(15) << Lok.pavarde << setw(15) << Lok.vardas << fixed << setprecision(2) << Lok.galutinis  << endl;
   }
}

//funkcija skaityti duomenis is failo
void nuskaitytiIsFailo(vector<Studentas>& studentai, const string& failoPavadinimas) {
   ifstream failas(failoPavadinimas);
   if (!failas) {
      cerr << "Nepavyko atidaryti failo: " << failoPavadinimas << endl;
      return;
   }

   string eilute;
   getline(failas, eilute);

   while (getline(failas, eilute)) {
      stringstream ss(eilute);
      Studentas tempStudentas;
      ss >> tempStudentas.pavarde >> tempStudentas.vardas; //pirmi du elementai eiluteje yra pavarde ir vardas, todel jie yra priskiriami pavardei ir vardui

      int pazymys;
      vector<int> namuDarbai;

      while (ss >> pazymys) {
         namuDarbai.push_back(pazymys);
      }

      //paskutinis ivestas pazymys yra egzamino, todel ji pasalinsime is namu darbu saraso
      tempStudentas.egzaminas = namuDarbai.back();
      namuDarbai.pop_back();

      tempStudentas.namuDarbai = namuDarbai;
      studentai.push_back(tempStudentas);

   }
   failas.close();

}

void isvedimasIFaila(const vector<Studentas>& studentai, char pasirinkimas, const string& failoPavadinimas) {
   ofstream failas(failoPavadinimas);
   if (!failas) {
      cerr << "Nepavyko sukurti failo: " << failoPavadinimas << endl;
      return;
   }

   if (pasirinkimas == 'V') {
      failas << left << setw(15) << "Pavardė" << setw(15) << "Vardas" << setw(15) << "Galutinis (Vid.)" << endl;
   } else if (pasirinkimas == 'M') {
      failas << left << setw(15) << "Pavardė" << setw(15) << "Vardas" << setw(15) << "Galutinis (Med.)" << endl;
   }

   failas << "----------------------------------------------" << endl;
   for (const auto& Lok : studentai)  {
      failas << left << setw(15) << Lok.pavarde << setw(15) << Lok.vardas << fixed << setprecision(2) << Lok.galutinis << endl;
   }

   failas.close();
   cout << "Rezultatai sėkmingai išsaugoti faile: " << failoPavadinimas << endl;
}


int main() {
   srand(time(0));

   vector<Studentas> studentai;
    char duomenuIvedimoBudas;
    
    cout << "Ar norite duomenis įvesti (I) ar nuskaityti iš failo (F)? ";
    cin >> duomenuIvedimoBudas;
    duomenuIvedimoBudas = toupper(duomenuIvedimoBudas);
    
    if (duomenuIvedimoBudas == 'I') {
        cout << "Įveskite studentų skaičių: ";
        int studentuSk;
        cin >> studentuSk;
        studentai.resize(studentuSk);

        for (int i = 0; i < studentuSk; i++) {
            pasirinktiDuomenuIvedimoBuda(studentai[i]);
        }

    } else if (duomenuIvedimoBudas == 'F') {
        string failoPavadinimas;
        cout << "Įveskite failo pavadinimą: ";
        cin >> failoPavadinimas;
        nuskaitytiIsFailo(studentai, failoPavadinimas);
    }

    char pasirinkimas;
    do {
      cout << "Pasirinkite galutinio balo skaičiavimo metodą (V- vidurkiu, M- mediana): ";
      cin >> pasirinkimas;
      pasirinkimas = toupper(pasirinkimas);
    } while (pasirinkimas != 'V' && pasirinkimas != 'M');

    for (Studentas& studentas : studentai) {
        skaiciuotiGalutini(studentas, pasirinkimas);
    }

    char isvedimoBudas;
    cout << "Ar norite išvesti rezultatus į ekraną (E) ar į failą (F)? ";
    cin >> isvedimoBudas;
    isvedimoBudas = toupper(isvedimoBudas);

    if (isvedimoBudas == 'E') {
      isvedimas(studentai, pasirinkimas);
    } else if (isvedimoBudas = 'F') {
      string failoPavadinimas;
      cout << "Įveskite failo pavadinimą rezultatams išsaugoti: ";
      cin >> failoPavadinimas;
      isvedimasIFaila(studentai, pasirinkimas, failoPavadinimas);
    }

    return 0;

}