#include "Stud.h"

//funkcija galutiniam balui apskaiciuoti naudojant vidurki
double skaiciuotiGalutiniVidurkiu(const vector<int> namuDarbai, int egzaminas) {
   if (namuDarbai.empty()) {
      return egzaminas * 0.6;
   }

   double vidurkis = 0.0;
   for (int nd: namuDarbai) {
      vidurkis += nd;
   }

   vidurkis /= namuDarbai.size();

   return vidurkis * 0.4 + egzaminas * 0.6;
}

//funkcija galutiniam balui apskaiciuoti naudojant mediana
double skaiciuotiGalutiniMediana(vector<int> namuDarbai, int egzaminas) {
   if (namuDarbai.empty()) {
      return egzaminas * 0.6;
   }

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

random_device rd_generator;
uniform_int_distribution<int> Results_interval(1, 10);

//funckija sugeneruoti atsitiktinius namu darbu ir egzamino pazymius
void generuotiDuomenis(Studentas& Lok, int ndSkaicius) {
   Lok.namuDarbai.clear();
   cout << "Sugeneruoti namų darbų pažymiai: ";
   for (int i = 0; i < ndSkaicius; i++) {
      int pazymys = Results_interval(rd_generator);
      Lok.namuDarbai.push_back(pazymys);
      cout << pazymys << " ";
   }
   Lok.egzaminas = Results_interval(rd_generator);
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
      getline(cin, input);   //naudojama perskaityti visa eilute input, iskaiciuojant tarpus
      if (input.empty()) {
         break;
      }

      try {
         stringstream ss(input);
         if (!(ss >> pazymys)) {  //bandoma extractint skaiciu
            throw invalid_argument("Netinkama įvestis");
         }

         if (pazymys < 1 || pazymys > 10) {
            throw out_of_range("Pažymys turi būti tarp 1 ir 10");
         }

         Lok.namuDarbai.push_back(pazymys);

      } catch (const invalid_argument& e) {
         cout << "Klaida: įvesta ne skaičius. Bandykite dar kartą." << endl;
      } catch (const out_of_range& e) {
         cout << "Klaida: pažymys turi būti tarp 1 ir 10. Bandykite dar kartą." << endl;
      }

   }

   cout << "Įveskite egzamino rezultatą: " << endl;
   while (true) {
      cin >> input;

      try {
         stringstream ss(input);
         if (!(ss >> pazymys)) {
            throw invalid_argument("Netinkama įvestis");
         }

         if (pazymys < 1 || pazymys > 10) {
            throw out_of_range("Egzamino pažymys turi būti tarp 1 ir 10");
         }

         Lok.egzaminas = pazymys;
         break;

      } catch (const invalid_argument& e) {
         cout << "Klaida: įvesta ne skaičius. Bandykite dar kartą." << endl;
      } catch (const out_of_range& e) {
         cout << "Klaida: pažymys turi būti tarp 1 ir 10. Bandykite dar kartą." << endl;
      }
   }

};

//funkcija pasirinkti duomenu ivedimo buda
void pasirinktiDuomenuIvedimoBuda(Studentas& Lok) {
   char pasirinkimas;
   while (true) {
      cout << "Ar norite įvesti pažymius rankiniu būdu (R) ar generuoti automatiškai (A)? ";
      cin >> pasirinkimas;
      pasirinkimas = toupper(pasirinkimas);
      if (pasirinkimas == 'R' || pasirinkimas == 'A') {
            break;
        } else {
            cout << "Neteisinga įvestis, bandykite dar kartą.\n";
        }
   }

   if (pasirinkimas == 'R') {
      ivedimas(Lok);
   } else if (pasirinkimas == 'A') {
      ivestiVardaPavarde(Lok);
      cout << "Įveskite, kiek namų darbų pažymių sugeneruoti: ";
      //cin.ignore();
      
      int ndSkaicius;
      string input;

        while(true) {
            getline(cin, input);

            try{
                stringstream ss(input);
                if (!(ss >> ndSkaicius)) {
                    throw invalid_argument("įvestis nėra skaičius. ");
                }
                break;
            } catch (const invalid_argument &e){
                cout << "Klaida: " << e.what() << "Bandykite dar kartą\n";
            }
        }

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
   ifstream failas(failoPavadinimas);  //bandoma atidaryti faila
   if (!failas) {
      cerr << "Nepavyko atidaryti failo: " << failoPavadinimas << endl;
      return;
   }

   //perskaitoma pirma eilute(header) ir praleidziama
   string eilute;
   getline(failas, eilute);

   while (getline(failas, eilute)) {
      stringstream ss(eilute);
      Studentas tempStudentas;
      ss >> tempStudentas.pavarde >> tempStudentas.vardas; //pirmi du elementai eiluteje yra pavarde ir vardas, todel jie yra priskiriami pavardei ir vardui

      string reiksme;
      vector<int> namuDarbai;

      while (ss >> reiksme) {
         try {
            int pazymys = stoi(reiksme);

            if (pazymys >= 1 && pazymys <= 10) {
               namuDarbai.push_back(pazymys);
            } else {
               cerr << "Netinkamas pažymys (" << pazymys << "), praleidžiama reikšmė" << endl;
            }
         } catch (invalid_argument&) {
            cerr << "Netinkamas simbolis (" << reiksme << "), praleidžiama reikšmė." << endl;
         } catch (out_of_range&) {
            cerr << "Pažymys už intervalo ribų, praleidžiama reikšmė." << endl;
         }
      }

      if (!namuDarbai.empty()) {
            //Paskutinis skaičius yra egzamino pažymys
            tempStudentas.egzaminas = namuDarbai.back();
            namuDarbai.pop_back();
            tempStudentas.namuDarbai = namuDarbai;

            studentai.push_back(tempStudentas);
        }

   }
   failas.close();

}

//funckija irasyti rezultatus i faila
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