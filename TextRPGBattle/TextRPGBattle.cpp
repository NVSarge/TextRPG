// TextRPGBattle.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <time.h>   
#include <stdlib.h>

using namespace std;


struct Item
{
	string name;
	int Att;
	int Def;
	int Dext;
	Item()
	{
		Att = Def = Dext = 0;
		name = "dust";
	}
	Item(int a, int d, int x)
	{
		Att = a;
		Def = d;
		Dext = x;
		name = "smth";
	}
	Item(int a, int d, int x,string s)
	{
		Att = a;
		Def = d;
		Dext = x;
		name = s;
	}
};

struct Unit
{
	Item Base;
	int HP;
	int Stamina;
	string name;
	Unit(int a,int d, int x, string s)
	{
		Base.Att = a;
		Base.Def = d;
		Base.Dext = x;
		HP = Base.Def*1000;
		Stamina = Base.Dext;
		name = s;
	}
	Unit()
	{
		HP = 1;
		Stamina = 1;
		name = "";
	}
	Unit(string s,int CR)
	{

		Base.Att = rand() % CR + 1;
		Base.Def = rand() % CR + 1;
		Base.Dext = rand() % CR + 1;
		HP = Base.Def * 1000;
		Stamina = Base.Dext;
		name = s;
	}
};

struct Option
{
	string Text;
	Item AddDamage;
	Option(string T, Item *i)
	{
		Text = T;
		AddDamage.Att = i->Att;
		AddDamage.Def = i->Def;
		AddDamage.Dext = i->Dext;
	}
};

int DoDamage(Unit *p, Unit *f,Option *o,Option *l)
{
	int retval = 10;
	int sum = 0;
	for (int i = 0; i < p->Base.Dext; i++)
	{
		if (rand() % 6 + 1 > f->Base.Def)
		{
			sum++;
		}
	}
	if(sum>0)	
	{
		cout <<p->name<<" hit";
		int Dmg = (p->Base.Att + o->AddDamage.Att);		
		Dmg = rand() % Dmg+1;
		retval = Dmg*100;		
	}
	else
	{
		cout << p->name << " miss";
	}
	if(retval>=0)
	{
		f->HP -= retval;
	}
	else
	{
		retval = 0;
	}
	p->Stamina += o->AddDamage.Dext;
	
	return retval;
}

int main()
{
	srand(time(NULL));
	vector<Item> Inventory;
	vector<vector<Option>> PlayerOptions;
	
	

	vector<Option> FoeOptions;
	vector<Unit> Enemies;
	Enemies.push_back(Unit("Goblin",2));
	Enemies.push_back(Unit("HobGoblin",3));
	Enemies.push_back(Unit("Rat",1));
	Enemies.push_back(Unit("Owlbear", 4));


	vector<Option> T;
	T.push_back(Option("Attack", new Item(0, 0, 0)));
	T.push_back(Option("Move", new Item(0, 0, 0)));
	T.push_back(Option("Use", new Item(0, 0, 0)));
	T.push_back(Option("wait", new Item(0, 0, 0)));
	PlayerOptions.push_back(T);

	T.clear();
	T.push_back(Option("Cut", new Item(1, 0, -1)));
	T.push_back(Option("Block", new Item(0, 1, -1)));
	T.push_back(Option("Slash", new Item(2, 0, -2)));
	PlayerOptions.push_back(T);

	T.clear();
	T.push_back(Option("Lunge", new Item(1, 0, -2)));
	T.push_back(Option("Volte", new Item(0, 1, -1)));
	T.push_back(Option("Retreat", new Item(0, 2, -2)));
	PlayerOptions.push_back(T);

	T.clear();
	T.push_back(Option("Apple", new Item(-10, 0, 1)));
	T.push_back(Option("Potion", new Item(-10,0,2)));
	T.push_back(Option("Spell", new Item(-10, 5,0)));
	PlayerOptions.push_back(T);

	T.clear();
	T.push_back(Option("and wait", new Item(0, 0, 1)));
	PlayerOptions.push_back(T);

	FoeOptions.push_back(Option("Bite", new Item(1, 0, -1)));
	FoeOptions.push_back(Option("Slash", new Item(2, 0, -2)));
	FoeOptions.push_back(Option("Stud", new Item(0, 1, -2)));
	FoeOptions.push_back(Option("Heal", new Item(-10, 2, 2)));
	Unit Foe=Enemies[rand()%Enemies.size()];
	Unit Player;	

	cout<< "Hello stranger, state your name: ";
	cin>> Player.name;
	cout << "Tell me,"<<Player.name<<" your level: ";
	int cr;
	cin >> cr;
	Player = Unit(Player.name, cr);
	cout << Player.name << " you meet an ugly "<<Foe.name<<"! Time to fight!" << endl;
	while (Foe.HP > 0 && Player.HP > 0)
	{
		
		cout << "\n==============================" << endl;
		cout << Player.name << " is " << Player.HP<<" s:"<<Player.Stamina<<" ["<<Player.Base.Att<<", " << Player.Base.Def<< ", " << Player.Base.Dext<<"]"<<endl;
		cout << Foe.name << " is " << Foe.HP << " [" << Foe.Base.Att << ", " << Foe.Base.Def << ", " << Foe.Base.Dext << "]"<< endl;
		cout << "==============================" << endl;
		int i = 1;
		int j = 1;
		int PSel = 1 ;
		int FSel = 0;
		if (Player.Stamina > 0)
		{
			for (Option o : PlayerOptions[0])
			{
				cout << i++ << ". " << o.Text << endl;
			}
			cin >> i;
			if (i >= 1 && i <= PlayerOptions.size())
			{
				PSel = i;
				i = 1;
				for (Option o : PlayerOptions[PSel])
				{

					cout << " *" << i++ << ". " << o.Text <<"["<<o.AddDamage.Dext<<"]"<<endl;
				}
				cin >> j;				
				

			
			}
			else
			{
				
				PSel = 4;
				j = 1;
			}
			
		}
		else
		{
			PSel = 4;
			j = 1;
		}
		system("CLS");
		
		int d = DoDamage(&Player, &Foe, &((PlayerOptions[PSel]).at(j - 1)), &FoeOptions[FSel]);
		cout << "\n\nYou " << (PlayerOptions[0]).at(PSel - 1).Text << " " << PlayerOptions[PSel].at(j - 1).Text << " for " << d << " damage." << endl;
		FSel = rand() % FoeOptions.size();
		d = DoDamage(&Foe, &Player, &FoeOptions[FSel], &((PlayerOptions[PSel]).at(j - 1)));
		cout << "\n\n" << Foe.name << " " << FoeOptions[i - 1].Text << "s for " << d << " damage." << endl;

		if (Player.Stamina > Player.Base.Dext)
		{
			Player.Stamina = Player.Base.Dext;
		}
		
		
	}
	cout << "Battle is over " << endl;
	system("PAUSE");
    return 0;
}

