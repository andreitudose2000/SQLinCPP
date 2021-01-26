#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <sstream>
#include <algorithm>
#include <fstream>
using namespace std;


class Tabel
{
	string nume_tabel;
	vector<tuple<string, string, int>> coloane; // tuplul contine: <nume, tip, dimensiune>
	vector<vector<string>> date;
public:
	friend class BazaDeDate;
	friend class Create;
	friend class Drop;
	friend class Display;
	friend class Insert;
	friend class Select;
	friend class Delete;
	friend class Update;
	friend class Import;
	friend class Incarcare;
	friend class Stocare;
	friend class SalvareDate;
};


class Create
{
public:
	void create(vector<Tabel>& tabele, vector<string> impartire)
	{
		Tabel tabel;

		bool daca_nu_exista = false;

		if (impartire[0] != "CREATE"
			|| impartire[1] != "TABLE"
			|| impartire.size() < 7)
		{
			cout << "Comanda a fost scrisa gresit.";
			return;
		}

		tabel.nume_tabel = impartire[2];
		for (int i = 0; i < tabele.size(); i++)
		{
			if (tabele[i].nume_tabel == tabel.nume_tabel)
			{
				cout << "Tabelul pe care incercati sa-l adaugati exista deja in baza de date.\n";
				return;
			}
		}

		int index_parcurgere;
		int numar_coloane;

		if (impartire[3] == "[DACA"
			&& impartire[4] == "NU"
			&& impartire[5] == "EXISTA]")
		{
			daca_nu_exista = true;
			index_parcurgere = 6;
			numar_coloane = (impartire.size() - 6) / 4;
		}
		else if (impartire[3].substr(0, 2) == "((")
		{
			daca_nu_exista = false;
			index_parcurgere = 3;
			numar_coloane = (impartire.size() - 3) / 4;
		}
		else
		{
			cout << "Comanda a fost scrisa gresit.";
			return;
		}

		if (daca_nu_exista)
		{
			if (impartire.size() < 10)
			{
				cout << "Comanda a fost scrisa gresit.";
				return;
			}
		}
		else
		{
			if (impartire.size() < 7)
			{
				cout << "Comanda a fost scrisa gresit.";
				return;
			}
		}

		int temp = 1;
		for (int i = 0; i < numar_coloane; i++)
		{
			string nume_col, tip, dimensiune, valoare_implicita;
			int dimensiune_int;

			nume_col = impartire[index_parcurgere].substr(1 + temp, impartire[index_parcurgere].size() - 2 - temp);
			tip = impartire[index_parcurgere + 1].substr(0, impartire[index_parcurgere + 1].size() - 1);
			if (tip != "text" && tip != "float" && tip != "integer")
			{
				cout << "Tipul datelor trebuie sa fie text / float / integer";
				return;
			}
			dimensiune = impartire[index_parcurgere + 2].substr(0, impartire[index_parcurgere + 2].size() - 1);
			valoare_implicita = impartire[index_parcurgere + 3].substr(0, impartire[index_parcurgere + 3].size() - 2);

			try
			{
				dimensiune_int = stoi(dimensiune);
			}
			catch (...)
			{
				cout << "Dimensiunea trebuie sa fie un numar natural.";
				return;
			}

			tabel.coloane.push_back(make_tuple(nume_col, tip, dimensiune_int));
			index_parcurgere += 4;
			temp = 0;
		}
		tabele.push_back(tabel);
		cout << "S-a adaugat tabelul " << tabel.nume_tabel << '.';
	}
};

class Drop
{
public:
	void drop(vector<Tabel>& tabele, vector<string> impartire)
	{
		if (impartire[0] != "DROP"
			|| impartire[1] != "TABLE")
		{
			cout << "Comanda a fost scrisa gresit.";
			return;
		}

		bool gasit = false;
		int index = -1;
		for (int i = 0; i < tabele.size(); i++)
		{
			if (tabele[i].nume_tabel == impartire[2])
			{
				gasit = true;
				index = i;
				break;
			}
		}
		if (gasit)
		{
			string nume_tabel_sters = tabele[index].nume_tabel;
			tabele.erase(tabele.begin() + index);
			cout << "S-a sters tabelul " << nume_tabel_sters << '.';
		}
		else
		{
			cout << "Tabelul pe care incercati sa il stergeti nu exista.";
			return;
		}
	}
};

class Display
{
	static int nr_comenzi_display;
public:
	static int get_nr_comenzi_display() { return nr_comenzi_display; }
	static void set_nr_comenzi_display(int val) { nr_comenzi_display = val; }
	void display(vector<Tabel>& tabele, vector<string> impartire)
	{
		if (impartire[0] != "DISPLAY"
			|| impartire[1] != "TABLE")
		{
			cout << "Comanda a fost scrisa gresit.";
			return;
		}

		bool gasit = false;
		int index = -1;
		for (int i = 0; i < tabele.size(); i++)
		{
			if (tabele[i].nume_tabel == impartire[2])
			{
				gasit = true;
				index = i;
				break;
			}
		}
		if (gasit)
		{
			cout << endl;
			display(tabele[index]);
		}
		else
		{
			cout << "Tabelul pe care incercati sa il afisati nu exista.";
			return;
		}
	}

	void display(Tabel tabel)
	{
		nr_comenzi_display++;
		string nume_fisier = "DISPLAY_" + to_string(nr_comenzi_display);
		ofstream out_display(nume_fisier);

		cout << "--------------" << tabel.nume_tabel << "--------------" << endl;
		out_display << "--------------" << tabel.nume_tabel << "--------------" << endl;
		for (tuple<string, string, int> t : tabel.coloane)
		{
			cout << get<0>(t) << ' ' << get<1>(t) << '(' << get<2>(t) << ')' << endl;
			out_display << get<0>(t) << ' ' << get<1>(t) << '(' << get<2>(t) << ')' << endl;
		}
		cout << "--------------------------------------------" << endl;
		out_display << "--------------------------------------------" << endl;

		vector<string> coloane_cautare;
		for (int i = 0; i < tabel.coloane.size(); i++)
		{
			coloane_cautare.push_back(get<0>(tabel.coloane[i]));
		}

		cout << endl;
		out_display << endl;
		for (int j = 0; j < coloane_cautare.size(); j++)
		{
			cout << coloane_cautare[j] << ' ';
			out_display << coloane_cautare[j] << ' ';
		}
		cout << endl;
		out_display << endl;

		vector<int> pozitii_coloane_cautare;
		for (int i = 0; i < coloane_cautare.size(); i++)
		{
			for (int j = 0; j < tabel.coloane.size(); j++)
			{
				if (get<0>(tabel.coloane[j]) == coloane_cautare[i])
				{
					pozitii_coloane_cautare.push_back(j);
					break;
				}
			}
		}

		for (int j = 0; j < tabel.date.size(); j++)
		{
			for (int k = 0; k < pozitii_coloane_cautare.size(); k++)
			{
				cout << tabel.date[j][pozitii_coloane_cautare[k]] << ' ';
				out_display << tabel.date[j][pozitii_coloane_cautare[k]] << ' ';
			}
			cout << endl;
			out_display << endl;
		}
		out_display.close();
	}
};

class Insert
{
public:
	void insert(vector<Tabel>& tabele, vector<string> impartire)
	{
		if (impartire[0] != "INSERT"
			|| impartire[1] != "INTO"
			|| impartire.size() < 4)
		{
			cout << "Comanda a fost scrisa gresit.";
			return;
		}

		bool gasit = false;
		int index = -1;
		for (int i = 0; i < tabele.size(); i++)
		{
			if (tabele[i].nume_tabel == impartire[2])
			{
				gasit = true;
				index = i;
				break;
			}
		}
		if (gasit)
		{
			cout << endl;
			vector<string> valori;
			int nr_valori = impartire.size() - 4;
			if (nr_valori != tabele[index].coloane.size())
			{
				cout << "Numarul de valori nu corespunde structurii tabelului.";
				return;
			}
			for (int i = 4; i < 4 + nr_valori; i++)
			{
				valori.push_back(impartire[i]);
			}
			for (int i = 0; i < valori.size(); i++)
			{
				valori[i].erase(remove(valori[i].begin(), valori[i].end(), ','), valori[i].end());
				valori[i].erase(remove(valori[i].begin(), valori[i].end(), '('), valori[i].end());
				valori[i].erase(remove(valori[i].begin(), valori[i].end(), ')'), valori[i].end());
			}
			insert(tabele[index], valori);
		}
		else
		{
			cout << "Tabelul in care incercati sa inserati nu exista.";
			return;
		}
	}

	void insert(Tabel& tabel, vector<string> valori, bool afisare = true)
	{
		for (int i = 0; i < tabel.coloane.size(); i++)
		{
			if (get<1>(tabel.coloane[i]) == "integer")
			{
				int valoare_integer;
				try
				{
					valoare_integer = stoi(valori[i]);
				}
				catch (...)
				{
					cout << "Una sau mai multe valori nu sunt de tipul corespunzator.";
					return;
				}
			}
			else if (get<1>(tabel.coloane[i]) == "float")
			{
				float valoare_float;
				try
				{
					valoare_float = stof(valori[i]);
				}
				catch (...)
				{
					cout << "Una sau mai multe valori nu sunt de tipul corespunzator.";
					return;
				}
			}
		}
		tabel.date.push_back(valori);
		if (afisare == true)
		{
			cout << "S-a inserat cu succes in tabelul " << tabel.nume_tabel << ".\n";
		}

	}
};

class Select
{
	static int nr_comenzi_select;
public:
	static int get_nr_comenzi_select() { return nr_comenzi_select; }
	static void set_nr_comenzi_select(int val) { nr_comenzi_select = val; }
	void select(vector<Tabel>& tabele, vector<string> impartire)
	{
		int indexFrom = -1;
		bool gasitMaiMulteFrom = false;
		for (int i = 0; i < impartire.size(); i++)
		{
			if (impartire[i] == "FROM")
			{
				if (indexFrom != -1)
					gasitMaiMulteFrom = true;
				indexFrom = i;
			}
		}
		if (indexFrom == -1
			|| gasitMaiMulteFrom)
		{
			cout << "Comanda a fost scrisa gresit.";
			return;
		}
		vector<string> coloane_cautare;

		bool selectAll = false;

		if (impartire[1] == "ALL")
			selectAll = true;

		for (int i = 1; i < indexFrom; i++)
		{
			coloane_cautare.push_back(impartire[i]);
		}
		for (int i = 0; i < coloane_cautare.size(); i++)
		{
			coloane_cautare[i].erase(remove(coloane_cautare[i].begin(), coloane_cautare[i].end(), ','), coloane_cautare[i].end());
			coloane_cautare[i].erase(remove(coloane_cautare[i].begin(), coloane_cautare[i].end(), '('), coloane_cautare[i].end());
			coloane_cautare[i].erase(remove(coloane_cautare[i].begin(), coloane_cautare[i].end(), ')'), coloane_cautare[i].end());
		}

		bool gasit = false;
		int index = -1;
		for (int i = 0; i < tabele.size(); i++)
		{
			if (tabele[i].nume_tabel == impartire[indexFrom + 1])
			{
				gasit = true;
				index = i;
				break;
			}
		}
		if (gasit)
		{
			if (selectAll)
			{
				coloane_cautare.clear();
				for (int i = 0; i < tabele[index].coloane.size(); i++)
				{
					coloane_cautare.push_back(get<0>(tabele[index].coloane[i]));
				}
			}

			if (impartire.size() > indexFrom + 2)
			{
				if (impartire[indexFrom + 2] != "WHERE"
					|| impartire[indexFrom + 4] != "=")
				{
					cout << "Comanda a fost scrisa gresit.";
					return;
				}
				string coloana_where, valoare_where;
				coloana_where = impartire[indexFrom + 3];
				valoare_where = impartire[indexFrom + 5];

				select(tabele[index], coloane_cautare, coloana_where, valoare_where);
			}
			else
			{
				select(tabele[index], coloane_cautare);
			}
		}
		else
		{
			cout << "Tabelul din care incercati sa selectati nu exista.";
			return;
		}
	}

	void select(Tabel& tabel, vector<string> coloane_cautare, string coloana_where, string valoare_where)
	{
		nr_comenzi_select++;
		string nume_fisier = "SELECT_" + to_string(nr_comenzi_select);
		ofstream out_select(nume_fisier);
		bool gasit_coloana = false;
		int index_coloana = -1;
		for (int i = tabel.coloane.size() - 1; i >= 0; i--)
		{
			if (get<0>(tabel.coloane[i]) == coloana_where)
			{
				gasit_coloana = true;
				index_coloana = i;
				break;
			}
		}
		if (!gasit_coloana)
		{
			cout << "Coloana introdusa nu exista in tabelul dat.";
			out_select << "Coloana introdusa nu exista in tabelul dat.";
			return;
		}


		vector<int> pozitii_coloane_cautare;
		for (int i = 0; i < coloane_cautare.size(); i++)
		{
			for (int j = 0; j < tabel.coloane.size(); j++)
			{
				if (get<0>(tabel.coloane[j]) == coloane_cautare[i])
				{
					pozitii_coloane_cautare.push_back(j);
					break;
				}
			}
		}

		cout << endl;
		out_select << endl;
		for (int j = 0; j < coloane_cautare.size(); j++)
		{
			cout << coloane_cautare[j] << ' ';
			out_select << coloane_cautare[j] << ' ';
		}

		cout << endl;
		out_select << endl;
		for (int j = 0; j < tabel.date.size(); j++)
		{
			if (tabel.date[j][index_coloana] == valoare_where)
			{
				for (int k = 0; k < pozitii_coloane_cautare.size(); k++)
				{
					cout << tabel.date[j][pozitii_coloane_cautare[k]] << ' ';
					out_select << tabel.date[j][pozitii_coloane_cautare[k]] << ' ';
				}
			}
			cout << endl;
			out_select << endl;
		}
		out_select.close();
	}

	void select(Tabel& tabel, vector<string> coloane_cautare)
	{
		nr_comenzi_select++;
		string nume_fisier = "SELECT_" + to_string(nr_comenzi_select);
		ofstream out_select(nume_fisier);
		cout << endl;
		for (int j = 0; j < coloane_cautare.size(); j++)
		{
			cout << coloane_cautare[j] << ' ';
			out_select << coloane_cautare[j] << ' ';
		}
		cout << endl;
		out_select << endl;

		vector<int> pozitii_coloane_cautare;
		for (int i = 0; i < coloane_cautare.size(); i++)
		{
			for (int j = 0; j < tabel.coloane.size(); j++)
			{
				if (get<0>(tabel.coloane[j]) == coloane_cautare[i])
				{
					pozitii_coloane_cautare.push_back(j);
					break;
				}
			}
		}

		for (int j = 0; j < tabel.date.size(); j++)
		{
			for (int k = 0; k < pozitii_coloane_cautare.size(); k++)
			{
				cout << tabel.date[j][pozitii_coloane_cautare[k]] << ' ';
				out_select << tabel.date[j][pozitii_coloane_cautare[k]] << ' ';
			}
			cout << endl;
			out_select << endl;
		}
		out_select.close();
	}
};

class Delete
{
public:
	void delete1(vector<Tabel>& tabele, vector<string> impartire)
	{
		if (impartire[0] != "DELETE"
			|| impartire[1] != "FROM"
			|| impartire[3] != "WHERE"
			|| impartire[5] != "=")
		{
			cout << "Comanda a fost scrisa gresit.";
			return;
		}
		bool gasit = false;
		int index = -1;
		for (int i = 0; i < tabele.size(); i++)
		{
			if (tabele[i].nume_tabel == impartire[2])
			{
				gasit = true;
				index = i;
				break;
			}
		}
		if (gasit)
		{
			string coloana_where = impartire[4];
			string valoare_where = impartire[6];

			delete1(tabele[index], coloana_where, valoare_where);
		}
		else
		{
			cout << "Tabelul din care incercati sa stergeti nu exista.";
			return;
		}
	}

	void delete1(Tabel& tabel, string coloana_where, string valoare_where)
	{
		bool gasit_coloana = false;
		int index_coloana = -1;
		for (int i = tabel.coloane.size() - 1; i >= 0; i--)
		{
			if (get<0>(tabel.coloane[i]) == coloana_where)
			{
				gasit_coloana = true;
				index_coloana = i;
				break;
			}
		}
		if (!gasit_coloana)
		{
			cout << "Coloana introdusa nu exista in tabelul dat.";
			return;
		}
		bool gasit_linie = false;

		int nr_linii_sterse = 0;

		for (int i = tabel.date.size() - 1; i >= 0; i--)
		{
			if (tabel.date[i][index_coloana] == valoare_where)
			{
				tabel.date.erase(tabel.date.begin() + i);
				nr_linii_sterse++;
			}
		}

		cout << "S-au sters " << nr_linii_sterse << " linii.";
	}
};

class Update
{
public:
	void update(vector<Tabel>& tabele, vector<string> impartire)
	{
		if (impartire[0] != "UPDATE"
			|| impartire[2] != "SET"
			|| impartire[4] != "="
			|| impartire[6] != "WHERE"
			|| impartire[8] != "=")
		{
			cout << "Comanda a fost scrisa gresit.";
			return;
		}

		bool gasit = false;
		int index = -1;
		for (int i = 0; i < tabele.size(); i++)
		{
			if (tabele[i].nume_tabel == impartire[1])
			{
				gasit = true;
				index = i;
				break;
			}
		}
		if (gasit)
		{
			string coloana_set = impartire[3],
				valoare_set = impartire[5],
				coloana_where = impartire[7],
				valoare_where = impartire[9];

			update(tabele[index], coloana_set, valoare_set, coloana_where, valoare_where);
		}
		else
		{
			cout << "Tabelul in care incercati sa updatati nu exista.";
			return;
		}
	}

	void update(Tabel& tabel, string coloana_set, string valoare_set, string coloana_where, string valoare_where)
	{
		bool gasit_coloana_where = false;
		int index_coloana_where = -1;
		for (int i = tabel.coloane.size() - 1; i >= 0; i--)
		{
			if (get<0>(tabel.coloane[i]) == coloana_where)
			{
				gasit_coloana_where = true;
				index_coloana_where = i;
				break;
			}
		}
		if (!gasit_coloana_where)
		{
			cout << "Coloana introdusa in WHERE nu exista in tabelul dat.";
			return;
		}

		bool gasit_coloana_set = false;
		int index_coloana_set = -1;

		for (int i = tabel.coloane.size() - 1; i >= 0; i--)
		{
			if (get<0>(tabel.coloane[i]) == coloana_set)
			{
				gasit_coloana_set = true;
				index_coloana_set = i;
				break;
			}
		}
		if (!gasit_coloana_set)
		{
			cout << "Coloana introdusa in SET nu exista in tabelul dat.";
			return;
		}

		int nr_linii_updatate = 0;

		for (int i = 0; i < tabel.date.size(); i++)
		{
			if (tabel.date[i][index_coloana_where] == valoare_where)
			{
				tabel.date[i][index_coloana_set] = valoare_set;
				nr_linii_updatate++;
			}
		}

		cout << "S-au updatat " << nr_linii_updatate << " linii.";
	}
};

class Import : public Insert
{
public:
	void import1(vector<Tabel>& tabele, vector<string> impartire)
	{
		if (impartire.size() != 3)
		{
			cout << "Comanda a fost scrisa gresit.";
			return;
		}
		string nume_tabel_nou = impartire[1];
		string nume_fisier_input = impartire[2];
		if (nume_fisier_input.substr(nume_fisier_input.size() - 4, 4) != ".csv"
			&& nume_fisier_input.substr(nume_fisier_input.size() - 4, 4) != ".CSV")
		{
			cout << "Va rog incarcati un fisier .CSV.\n";
			return;
		}
		bool gasit = false;
		bool index = -1;
		for (int i = 0; i < tabele.size(); i++)
		{
			if (tabele[i].nume_tabel == nume_tabel_nou)
			{
				gasit = true;
				index = i;
				break;
			}
		}
		if (gasit)
		{
			string linie;
			vector<string> linii;
			string valoare;
			vector<string> valori;
			vector<vector<string>> toate_valorile;
			ifstream importare(nume_fisier_input);

			while (getline(importare, linie))
			{
				linii.push_back(linie);
			}

			for (string linie : linii)
			{
				for (int i = 0; i < linie.size(); i++)
				{
					if (linie[i] < 'A' && linie[i] > 'Z'
						&& linie[i] < 'a' && linie[i] > 'z'
						&& linie[i] < '0' && linie[i] > '9'
						&& linie[i] != ',')
					{
						cout << "Fisierul .csv nu este formatat cum trebuie sau contine alt simbol de delimitare in afara de virgula.";
						return;
					}
				}
				valori.clear();
				stringstream ss(linie);
				while (getline(ss, valoare, ','))
				{
					valori.push_back(valoare);
				}
				if (valori.size() != tabele[index].coloane.size())
				{
					cout << "Fisierul .csv nu are structura aferenta tabelei indicate.";
					return;
				}
				toate_valorile.push_back(valori);
			}

			for (int i = 0; i < toate_valorile.size(); i++)
			{
				insert(tabele[index], toate_valorile[i], false);
			}

			importare.close();
		}
		else
		{
			cout << "Tabelul nu exista in baza de date.\n";
			return;
		}
	}
};

class RulareArgumenteMain
{
public:
	vector<string> rulare_comenzi(int argc, char* argv[])
	{
		char comanda_char[256];
		string comanda;
		vector<string> comenzi;

		for (int i = 1; i < argc; i++)
		{
			const char* p = argv[i];
			ifstream intrare(argv[i]);
			if (intrare)
			{
				while (getline(intrare, comanda))
				{
					comenzi.push_back(comanda);
				}
			}
			intrare.close();
		}
		return comenzi;
	}
};

class Incarcare
{
public:
	void incarcare(vector<Tabel>& tabele, string nume_fisier)
	{
		ifstream i_incarcare(nume_fisier);
		int nr_tabele;

		i_incarcare >> nr_tabele;
		for (int i = 0; i < nr_tabele; i++)
		{
			Tabel tabel;
			i_incarcare >> tabel.nume_tabel;

			int nr_coloane;
			i_incarcare >> nr_coloane;
			for (int i = 0; i < nr_coloane; i++)
			{
				string nume_col, tip; int dimensiune;
				i_incarcare >> nume_col >> tip >> dimensiune;
				tabel.coloane.push_back(make_tuple(nume_col, tip, dimensiune));
			}
			tabele.push_back(tabel);
		}


		i_incarcare.close();
	}
};

class Stocare
{
public:
	void stocare(vector<Tabel> tabele, string nume_fisier)
	{
		ofstream o_stocare(nume_fisier);

		o_stocare << tabele.size() << endl;
		for (int i = 0; i < tabele.size(); i++)
		{
			o_stocare << tabele[i].nume_tabel << endl;
			o_stocare << tabele[i].coloane.size() << endl;
			for (int j = 0; j < tabele[i].coloane.size(); j++)
			{
				o_stocare << get<0>(tabele[i].coloane[j]) << ' '
					<< get<1>(tabele[i].coloane[j]) << ' '
					<< get<2>(tabele[i].coloane[j]) << endl;
			}
		}

		o_stocare.close();
	}
};

class SalvareDate : public Insert
{
public:
	void incarcare(vector<Tabel>& tabele)
	{
		for (int i = 0; i < tabele.size(); i++)
		{

			string nume_fisier = tabele[i].nume_tabel + ".dat";
			ifstream i_incarcare(nume_fisier, ios::in | ios::binary);

			if (!i_incarcare) {
				cout << "Pentru tabelul " << tabele[i].nume_tabel << " nu s-au gasit date!\n";
				continue;
			}

			i_incarcare.seekg(0, i_incarcare.end);
			int lungime_fisier = i_incarcare.tellg();
			i_incarcare.seekg(0, i_incarcare.beg);


			char* input;
			if (lungime_fisier > 10000 || lungime_fisier <= 0)
			{
				cout << "Pentru tabelul " << tabele[i].nume_tabel << " nu s-au gasit date!\n";
				continue;
			}

			input = new char[lungime_fisier + 1];
			i_incarcare.read(input, lungime_fisier);
			input[lungime_fisier] = '\0';

			string data = input;

			vector<vector<string>> toate_valorile;
			string temp_string = "";
			vector<string> temp_vector;
			for (int j = 0; j < data.size(); j++)
			{
				if (data[j] == '\n')
				{
					temp_vector.push_back(temp_string);
					temp_string = "";
					toate_valorile.push_back(temp_vector);
					temp_vector.clear();
				}
				else if (data[j] == ',')
				{
					temp_vector.push_back(temp_string);
					temp_string = "";
				}
				else
				{
					temp_string += data[j];
				}
			}
			temp_vector.push_back(temp_string);
			toate_valorile.push_back(temp_vector);

			for (int k = 0; k < toate_valorile.size(); k++)
			{
				insert(tabele[i], toate_valorile[k], false);
			}

			i_incarcare.close();
		}

	}
	void stocare(vector<Tabel> tabele)
	{
		for (int i = 0; i < tabele.size(); i++)
		{
			if (tabele[i].date.size() > 0)
			{
				string nume_fisier = tabele[i].nume_tabel + ".dat";
				ofstream o_salvare(nume_fisier, ios::out | ios::binary);

				string data = "";

				for (int j = 0; j < tabele[i].date.size(); j++)
				{
					for (int k = 0; k < tabele[i].date[j].size(); k++)
					{
						data += tabele[i].date[j][k];
						if (k < tabele[i].date[j].size() - 1)
							data += ',';
					}
					if (j < tabele[i].date.size() - 1)
						data += '\n';
				}

				o_salvare.write(data.c_str(), data.size());
				o_salvare.close();
			}
		}
	}
};

class BazaDeDate : public Create, public Drop, public Display, public Select, public Delete, public Update, public Import
{
	vector<Tabel> tabele;
	RulareArgumenteMain argumente;
	Incarcare incarcare;
	Stocare stocare;
	SalvareDate salvare;
public:
	vector<Tabel>& get_tabele() { return tabele; }

	RulareArgumenteMain get_argumente() { return argumente; }
	Incarcare get_incarcare() { return incarcare; }
	Stocare get_stocare() { return stocare; }
	SalvareDate get_salvare() { return salvare; }

	void interpretare_comanda(string comanda)
	{
		string s = comanda;
		string delimiter = " ";
		vector<string> impartire;

		size_t pos = 0;
		string token;
		while ((pos = s.find(delimiter)) != string::npos) {
			token = s.substr(0, pos);
			impartire.push_back(token);
			s.erase(0, pos + delimiter.length());
		}
		impartire.push_back(s);

		if (impartire[0] == "CREATE")
		{
			create(tabele, impartire);
		}
		else if (impartire[0] == "DROP")
		{
			drop(tabele, impartire);
		}
		else if (impartire[0] == "DISPLAY")
		{
			display(tabele, impartire);
		}
		else if (impartire[0] == "INSERT")
		{
			insert(tabele, impartire);
		}
		else if (impartire[0] == "DELETE")
		{
			delete1(tabele, impartire);
		}
		else if (impartire[0] == "SELECT")
		{
			select(tabele, impartire);
		}
		else if (impartire[0] == "UPDATE")
		{
			update(tabele, impartire);
		}
		else if (impartire[0] == "IMPORT")
		{
			import1(tabele, impartire);
		}
		else if (impartire[0] != "EXIT")
		{
			cout << "Comanda scrisa gresit.\n";
		}
	}
};

void afisareComenzi()
{
	cout << "================" << endl;
	cout << "Puteti folosi urmatoarele comenzi:" << endl;
	cout << "- CREATE TABLE table_name [DACA NU EXISTA] ((nume_coloana_1, tip, dimensiune, valoare_implicita), (nume_coloana_2, tip, dimensiune, valoare_implicita), ... )" << endl;
	cout << "- DROP TABLE table_name" << endl;
	cout << "- DISPLAY TABLE table_name" << endl;
	cout << "- INSERT INTO nume_tabela VALUES (...)" << endl;
	cout << "- DELETE FROM nume_tabela WHERE nume_coloana = valoare" << endl;
	cout << "- SELECT (cel_putin_o_coloana, ...) | ALL FROM nume_tabela [WHERE nume_coloana = valoare]" << endl;
	cout << "- UPDATE nume_tabela SET nume_coloana = valoare WHERE nume_coloana = valoare" << endl;
	cout << "Introduceti EXIT pentru a inchide programul." << endl;
	cout << "================" << endl;
}