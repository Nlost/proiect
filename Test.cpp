#include<iostream>
#include<fstream>
#include<Windows.h>
#include<list>
#include<String.h>

using namespace std;

class reteta {
private:
	string name;
	double time;
public:
	reteta(){};
	reteta(string name, double time)
	{
		this->name = name;
		this->time = time;
	}
	double time_()
	{
		return this->time;
	}
	string name_()
	{
		return this->name;
	}
	friend istream& operator >>(istream& in, reteta& r);
};

istream& operator>>(istream& in, reteta& r)
{
	in >> r.name >> r.time;
	return in;
}

class prajitura {
private:
	string name;
	reteta ret;
public:
	prajitura(string name, reteta r)
	{
		this->name = name;
		this->ret = r;
	}
	string name_()
	{
		return this->name;
	}
	reteta reteta_()
	{
		return this->ret;
	}
	bool operator ==(prajitura& cake)
	{
		return this->name == cake.name;
	}
	virtual void afisare()
	{
		cout << this->name << " " << ret.name_() << " " << ret.time_();
	}
};

class diabetic :public prajitura {
private:
	double zahar;
public:
	diabetic(string name, reteta r, double z) :prajitura(name, r)
	{
		this->zahar = z;
	}
	double zahar_()
	{
		return this->zahar;
	}
	bool operator ==(diabetic& cake)
	{
		return this->zahar == cake.zahar && prajitura::operator==(cake);
	}
	void afisare()
	{
		prajitura::afisare();
		cout << " " << this->zahar;
	}
};


class preparator {
public:
	prajitura* pregatire(prajitura* cake)
	{
		Sleep(cake->reteta_().time_() * 60000);
		return cake;
	}
};

class panou_comanda {
private:
	int max;
	list<prajitura*>carusel;
	list<prajitura*>lista_prajituri_existente;
public:
	panou_comanda(int max)
	{
		this->max = max;
	}
	int max_()
	{
		return max;
	}
	list<prajitura*> carusel_()
	{
		return carusel;
	}
	list<prajitura*>lista_prajituri_existente_()
	{
		return lista_prajituri_existente;
	}
	void citire_fisier()
	{
		string name;
		reteta ret;
		int type;
		double zahar;
		ifstream input("retete.txt");
		while (!input.eof())
		{
			input >> name >> ret >> type;
			if (type == 0)
			{
				prajitura* temp = new prajitura(name, ret);

				lista_prajituri_existente.push_back(temp);
			}
			else if (type == 1)
			{
				input >> zahar;
				prajitura* temp = new diabetic(name, ret, zahar);
				lista_prajituri_existente.push_back(temp);
			}
		}
	}

	void takeOrder(prajitura* pr, int cantitate)
	{
		for (int i = 0; i < cantitate; i++)
		{
			bool found = false;
			for (auto temp : carusel)
			{
				if (temp->name_() == pr->name_())
				{
					found = true;
					cout << "<Client>Prajitura comandata se livreaza! Pofta buna" << endl;
					carusel.remove(pr);
					break;
				}

			}
			if (found == false)
			{
				cout << "<Client>Se prepara prajitura: " << pr->name_() << endl;
				Sleep(pr->reteta_().time_() * 60000);
				cout << "<Client>Prajitura a fost preparata! Pofta buna!" << endl;
				while (carusel.size() < max)
				{
					cout << "<Internal command>Umplere depozit cu " << pr->name_() << endl;
					carusel.push_back(pr);
				}
			}
		}
	}

};



void order(panou_comanda& panou)
{
	int i = 1;
	int j, nr_comanda, cantitate;
	for (auto t : panou.lista_prajituri_existente_())
	{
		cout << i << ")";
		t->afisare();
		i++;
		cout << endl;
	}
	cout << "Comanda: ";
	cin >> nr_comanda;
	if (nr_comanda <1 || nr_comanda>panou.lista_prajituri_existente_().size())
	{
		try {
			throw - 1;
		}
		catch (int a)
		{
			ofstream file("log.txt", ios::app);
			file << "Numar gresit" << endl;
			file.close();
		}
	}
	else
	{
		cout << "Nr bucati: ";
		cin >> cantitate;
		j = 1;
		for (auto t : panou.lista_prajituri_existente_())
		{
			if (j == nr_comanda)
			{
				panou.takeOrder(t, cantitate);
				break;
			}
			j++;
		}
	}
}

int main()
{
	panou_comanda panou(4);
	int opt;
	panou.citire_fisier();
	while (true)
	{
		cout << "Aparat" << endl;
		cout << "1. Lista prajiturilor disponibile" << endl;
		cout << "2. Comdanda" << endl;
		cout << "3.Afisare carusel" << endl;
		cout << "0. Iesire" << endl;
		cin >> opt;
		switch (opt)
		{
		case 1:
			for (auto t : panou.lista_prajituri_existente_())
			{
				t->afisare();
				cout << endl;
			}
			break;
		case 2:
			order(panou);
			break;
		case 3:
			for (auto t : panou.carusel_())
			{
				t->afisare();
				cout << endl;
			}
			break;
		default:
			cout << endl;
			break;
		}
	}
	return 0;
}


