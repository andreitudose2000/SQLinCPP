#include "ProiectPOO.h"

int Select::nr_comenzi_select = 0;
int Display::nr_comenzi_display = 0;
int main(int argc, char* argv[])
{
	BazaDeDate baza_de_date;
	string comanda;

	baza_de_date.get_incarcare().incarcare(baza_de_date.get_tabele(), "Tabele.txt");
	baza_de_date.get_salvare().incarcare(baza_de_date.get_tabele());

	ifstream fin("indici.txt");
	int x, y;
	fin >> x >> y;
	Select::set_nr_comenzi_select(x);
	Display::set_nr_comenzi_display(y);
	fin.close();

	if (argc > 6)
	{
		cout << "Maxim 5 argumente";
		return -1;
	}

	if (argc > 1)
	{
		vector<string> comenzi = baza_de_date.get_argumente().rulare_comenzi(argc, argv);
		for (int i = 0; i < comenzi.size(); i++)
		{
			baza_de_date.interpretare_comanda(comenzi[i]);
		}
	}

	afisareComenzi();

	do
	{
		getline(cin, comanda);
		baza_de_date.interpretare_comanda(comanda);
		cout << endl << endl;
	} while (comanda != "EXIT");

	baza_de_date.get_stocare().stocare(baza_de_date.get_tabele(), "Tabele.txt");
	baza_de_date.get_salvare().stocare(baza_de_date.get_tabele());

	ofstream fout("indici.txt");
	fout << Select::get_nr_comenzi_select() << ' ' << Display::get_nr_comenzi_display();
	fout.close();
	return 0;
}
