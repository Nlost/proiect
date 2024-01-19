#include<iostream>
#include<Windows.h>
#include<string.h>
#include<vector>
#include<fstream>
//uuid guid

#define MINUTES 60000
using namespace std;



class Reteta {
private:
	string nume;
	float timp;
public:
	Reteta(){}
	Reteta(string n, float t) : nume(n), timp(t) {}
	string getname() { return nume; }
	float gettime() { return timp; }
	friend istream& operator >>(istream& in, Reteta& r);
	Reteta* operator =(Reteta c);

};



Reteta* Reteta::operator=(Reteta c)
{
	Reteta* r = new Reteta();
	r->nume = c.getname();
	r->timp = c.gettime();
	return r;
}

istream& operator>> (istream& in, Reteta& r)
{
	in >> r.nume >> r.timp;
	return in;
}

class Prajitura {
	string nume;
	Reteta receipt;
public:
	Prajitura(string n, const Reteta r): nume(n), receipt(r) {}
	string getnume() { return nume; }
	void afisarereteta() 
	{
		cout << "Nume reteta: " << receipt.getname() << " Timp pregatire: " << receipt.gettime();
	}
	virtual void info()
	{
		cout <<"Nume: "<< nume<<" ";
		afisarereteta();

	}
	bool operator == (Prajitura& comp) {return nume == comp.getnume();}
	virtual void cumparare() {cout << "Nume: " << nume;}
	Reteta getreteta() { return receipt; }

};

class Prajitura_diabetica : public Prajitura {
	float cantitate_zahar;
public:
	Prajitura_diabetica(string n, const Reteta r, float cz):Prajitura(n, r), cantitate_zahar(cz){}
	float get_c_zahar() { return cantitate_zahar; }
	bool operator ==(Prajitura_diabetica& comp)
	{
		return cantitate_zahar == comp.get_c_zahar() && Prajitura::operator==(comp);
	}
	 void info()
	{
		Prajitura::info();
		cout << " Zahar: " << cantitate_zahar;
	}
	 void cumparare()
	 {
		 Prajitura::cumparare();
		 cout << "(diabetica). Cantitate zahar: " << cantitate_zahar;
	 }
};




class Automat {
	int capacitate;
	vector<Prajitura*>deposit;
	vector<Prajitura*>acces;
public:
	Automat(int max): capacitate(max), deposit(), acces() {}
	void initializare()
	{
		string nume_prajitura;
		Reteta reteta;
		int tip;
		float zahar;
		ifstream file("retete.txt");
		while (!file.eof()) {
			file >> nume_prajitura >> reteta >> tip;
			if (tip == 1)
			{
				file >> zahar;
				Prajitura *p = new Prajitura_diabetica(nume_prajitura, reteta, zahar);
				acces.push_back(p);
			}
			else
			{
				Prajitura *p = new Prajitura(nume_prajitura, reteta);
				acces.push_back(p);
			}
		}
	}

	void comanda(Prajitura* cake, int amount)
	{
		for (int i = 0; i < amount; i++)
		{
			bool found = false;
			int j = 0;
			for (auto temp : deposit)
			{
				if (temp->getnume() == cake->getnume())
				{
					found = true;
					cout << "<Client>Prajitura comandata se livreaza! Pofta buna" << endl;
					deposit.erase(deposit.begin()+j);
					break;
				}
				j++;
			}
			if (found == false)
			{
				cout <<"<Client>Se prepara prajitura: " << cake->getnume()<< endl;
				Sleep(cake->getreteta().gettime() * MINUTES);
				cout << "<Client>Prajitura a fost preparata! Pofta buna!" << endl;
				while (deposit.size() < capacitate)
				{
					cout << "<Internal command>Umplere depozit cu " << cake->getnume() << endl;
					Sleep(cake->getreteta().gettime() * MINUTES);
					deposit.push_back(cake);
				}
			}
		}
		
	}



	vector<Prajitura*> returnare_acces(){ return acces; }

	vector<Prajitura*> returnare_deposit(){ return deposit; }
};

void acces(vector<Prajitura*> p)
{
	for (auto temp : p)
	{
		temp->info();
		cout << endl;
	}
}

void deposit(vector<Prajitura*> p)
{
	for (auto temp : p)
	{
		temp->info();
		cout << endl;
	}
}

void cumparare(Automat &a)
{
	int i = 1;
	int j, option, amount;
	for (auto temp : a.returnare_acces())
	{
		cout << i << ")";
		temp->cumparare();
		i++;
		cout << endl;
	}
	cout << "Comanda dorita: ";
	cin >> option;
	if (option<1 || option >a.returnare_acces().size())
	{
		try {
			cout << "<Error>Check log.txt for info" << endl;
			throw option;
		}
		catch (int a)
		{
			ofstream error("log.txt", ios::app);
			if (error.is_open())
			{
				error << "S-a introdus o comanda inexistenta, si anume "<< a << endl;
			}
			error.close();
		}
	}
	else {
		cout << "Numarul de bucati: ";
		cin >> amount;
		j = 1;
		for (auto temp : a.returnare_acces())
		{
			if (j == option)
			{
				a.comanda(temp, amount);
				break;
			}
			j++;
		}
	}
}



int main()
{ 
	Reteta* r = new Reteta("nune", 31232);
	Automat aparat(4);
	int opt;
	aparat.initializare();
	
	while (true)
	{
		cout << "=======================" << endl << "=======================" << endl;
		cout << "Meniu" << endl;
		cout << "1. Afisare prajituri existente" << endl;
		cout << "2. Comandarea unui produs" << endl;
		cout << "3. Afisarea prajituri in aparat" << endl;
		cout << "0. Iesire aparat" << endl;
		cout << "=======================" << endl;
		cout << "Optiune: ";
		cin >> opt;
		cout << "=======================" << endl;
		switch (opt)
		{
		case 1:
			acces(aparat.returnare_acces());
			break;
		case 2:
			cumparare(aparat);
			break;
		case 3:
			acces(aparat.returnare_deposit());
			break;
		case 0:
			exit(0);
			break;
		default:
			try {
				cout << "<Error>Check log.txt for info" << endl;
				throw opt;
			}
			catch (int a) {
				ofstream error("log.txt", ios::app);
				if (error.is_open())
				{
					error << "S-a introdus optiune gresita: " << a << endl;
				}
				error.close();
			}
			break;
		}
	}
	return 0;
}