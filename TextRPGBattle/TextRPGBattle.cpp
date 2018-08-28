// TextRPGBattle.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <time.h>   
#include <stdlib.h>
#include <chrono>
#include<thread>

using namespace std;
using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;

int xd6(int x)
{
	int retval = 0;
	for (int i = 0; i < x; i++)
	{
		int R = rand()%6;
		for (int j = 0; j < rand() % 5 + 5; j++)
		{
			cout << R+1;
			sleep_until(system_clock::now() + 100ms);
			R = rand() % 6;
			cout << "\r    \r";
		}
		cout << R+1 << endl;
		retval += R + 1;
	}
	return retval;
}

int xdy(int x, int y)
{
	int retval = 0;
	for (int i = 0; i < x; i++)
	{
		retval += rand() % y + 1;
	}
	return retval;
}

struct Item
{
	string name;
	int Att;
	int Def;
	int BDY;
	Item()
	{
		Att = Def = BDY = 0;
		name = "dust";
	}
	Item(int a, int d, int x)
	{
		Att = a;
		Def = d;
		BDY = x;
		name = "smth";
	}
	Item(int a, int d, int x, string s)
	{
		Att = a;
		Def = d;
		BDY = x;
		name = s;
	}
};

struct RageDicePool
{
	int amount;
	RageDicePool()
	{
		amount = 0;
	}
	void Push()
	{
		amount++;
	}
	void Pop()
	{
		amount--;
	}
	
	bool Acquire()
	{
		bool retval = true;
		if (amount > 0)
		{
			if (xd6(1) <= amount)
			{
				retval = false;
				amount = 0;
			}
			else
			{
				amount--;
			}
		}
		return retval;
	}
};

struct DNA
{
	DNA()
	{
		isBDYReroll = 0;
	}
	int isBDYReroll;//0-10 amount of RDP use for reroll BDY
	bool CheckBDY()
	{

		return xdy(1, 10) <= isBDYReroll;
	}
};

struct Unit
{
	Item Base;
	int HP;
	int Stamina;
	RageDicePool RDP;
	int SubDicePool;
	DNA dna;
	string name;
	Unit(int a, int d, int x, string s)
	{
		SubDicePool = 0;
		Base.Att = a;
		Base.Def = d;
		Base.BDY = x;
		HP = 4 + xdy(Base.BDY * 2 - 2, 3);
		Stamina = Base.BDY;
		name = s;
		dna.isBDYReroll = 5;
	}
	Unit()
	{
		SubDicePool = 0;
		HP = 1;
		Stamina = 1;
		name = "";
		dna.isBDYReroll = 5;
	}
	Unit(string s, int CR)
	{
		SubDicePool = 0;
		Base.Att = rand() % CR + 1;
		Base.Def = rand() % CR + 1;
		Base.BDY = CR;
		HP = 4 + 2 * xdy((CR - 1) * 2, 3);
		Stamina = Base.BDY;
		name = s;
		dna.isBDYReroll = 5;
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
		AddDamage.BDY = i->BDY;
	}
};

/*
int DoDamageV1(Unit *p, Unit *f,Option *o,Option *l)
{
	int retval = 10;
	int sum = 0;
	for (int i = 0; i < p->Base.BDY; i++)
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
	p->Stamina += o->AddDamage.BDY;

	return retval;
}


void Version1_0()
{
	Unit *Foe;
	Unit *Player;
	vector<Item> *Inventory;
	vector<vector<Option>> *PlayerOptions;
	vector<Option> *FoeOptions;
	while (Foe->HP > 0 && Player->HP > 0)
	{

		cout << "\n==============================" << endl;
		cout << Player->name << " is " << Player->HP << " s:" << Player->Stamina << " [" << Player->Base.Att << ", " << Player->Base.Def << ", " << Player->Base.BDY << "]" << endl;
		cout << Foe->name << " is " << Foe->HP << " [" << Foe->Base.Att << ", " << Foe->Base.Def << ", " << Foe->Base.BDY << "]" << endl;
		cout << "==============================" << endl;
		int i = 1;
		int j = 1;
		int PSel = 1;
		int FSel = 0;
		if (Player->Stamina > 0)
		{
			for (Option o : PlayerOptions->at(0))
			{
				cout << i++ << ". " << o.Text << endl;
			}
			cin >> i;
			if (i >= 1 && i <= PlayerOptions->size())
			{
				PSel = i;
				i = 1;
				for (Option o : PlayerOptions->at(PSel))
				{

					cout << " *" << i++ << ". " << o.Text << "[" << o.AddDamage.BDY << "]" << endl;
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

		int d = DoDamageV1(Player,Foe,&((PlayerOptions->at(PSel)).at(j - 1)), &FoeOptions->at(FSel));

		cout << "\n\nYou " << PlayerOptions->at(0).at(PSel - 1).Text << " " << (PlayerOptions->at(PSel)).at(j - 1).Text << " for " << d << " damage." << endl;
		FSel = rand() % FoeOptions->size();
		d = DoDamageV1(Foe, Player, &(FoeOptions->at(FSel)), &((PlayerOptions->at(PSel)).at(j - 1)));
		cout << "\n\n" << Foe->name << " " << (FoeOptions->at(i - 1)).Text << "s for " << d << " damage." << endl;

		if (Player->Stamina > Player->Base.BDY)
		{
			Player->Stamina = Player->Base.BDY;
		}


	}
	cout << "Battle is over " << endl;
	system("PAUSE");
}
/**/

enum DColor
{
	WHITE,
	BLACK,
	RED
};



struct ColoredDice
{
	ColoredDice(DColor dc)
	{
		color = dc;
	}
	DColor color;
	int NUM;
	void Throw()
	{		
		NUM = xd6(1);
	}
};


bool CheckDices(ColoredDice CD1, ColoredDice CD2)
{
	return (CD1.NUM < CD2.NUM);
}

int main()
{
	srand(time(NULL));
	vector<Item> Inventory;
	vector<vector<Option>> PlayerOptions;
	vector<Option> FoeOptions;
	vector<Unit> Enemies;
	Enemies.push_back(Unit("Goblin", 2));
	Enemies.push_back(Unit("HobGoblin", 3));
	Enemies.push_back(Unit("Rat", 1));
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
	T.push_back(Option("Potion", new Item(-10, 0, 2)));
	T.push_back(Option("Spell", new Item(-10, 5, 0)));
	PlayerOptions.push_back(T);

	T.clear();
	T.push_back(Option("and wait", new Item(0, 0, 1)));
	PlayerOptions.push_back(T);

	FoeOptions.push_back(Option("Bite", new Item(1, 0, -1)));
	FoeOptions.push_back(Option("Slash", new Item(2, 0, -2)));
	FoeOptions.push_back(Option("Stud", new Item(0, 1, -2)));
	FoeOptions.push_back(Option("Heal", new Item(-10, 2, 2)));
	Unit Foe = Enemies[rand() % Enemies.size()];
	Unit Player;

	cout << "Hello stranger, state your name: ";
	cin >> Player.name;
	cout << "Tell me," << Player.name << " your level: ";
	int cr;
	cin >> cr;
	Player = Unit(Player.name, cr);
	cout << Player.name << " you meet an ugly " << Foe.name << "! Time to fight!" << endl;
	system("PAUSE");


	while (Player.HP > 0 && Foe.HP > 0)
	{
		system("CLS");
		cout << "\n==============================" << endl;
		cout << Player.name << " HP is " << Player.HP << " [" << Player.Base.BDY << ", " << Player.Base.Att << ", " << Player.Base.Def << "]" << " RageT: " << Player.RDP.amount << endl;
		cout << Foe.name << " HP is " << Foe.HP << " [" << Foe.Base.BDY << ", " << Foe.Base.Att << ", " << Foe.Base.Def << "]" << " RageT: " << Foe.RDP.amount << endl;
		cout << "==============================" << endl;
		cout << "Make BDY throw:" << endl;
		vector<ColoredDice>BDYDices;
		cout << "Player:" << endl;
		for (int i = 0; i < Player.Base.BDY+Player.SubDicePool; i++)
		{
			ColoredDice white(WHITE);
			white.Throw();
			BDYDices.push_back(white);
		}
		cout << "Foe:" << endl;
		for (int i = 0; i < Foe.Base.BDY+Foe.SubDicePool; i++)
		{
			ColoredDice black(BLACK);
			black.Throw();
			BDYDices.push_back(black);

		}

		sort(BDYDices.begin(), BDYDices.end(), CheckDices);
		bool isPlayerRaged = false;
		bool isFoeRaged = false;
		int ActionsNum = 0;
		cout << "\n====BDY===" << endl;
		for (int i = 0; i < BDYDices.size(); i++)
		{
			cout << i + 1 << ((BDYDices[i].color == WHITE) ? (" w") : (" b")) << ":" << BDYDices[i].NUM << "   ";
		}
		if (Player.RDP.amount > 0)
		{
			cout << "\nReroll how many(0-none): ";
			int rr = 0;
			cin >> rr;
			if (rr > 0 && Player.RDP.amount >= rr)
			{

				int num;
				for (int i = 0; i < rr; i++)
				{					
					cout << "Which one?:";
					cin >> num;
					if (Player.RDP.Acquire())
					{
						BDYDices[num - 1].Throw();
						cout << "new value: " << BDYDices[num - 1].NUM << endl;
					}
					else
					{
						isPlayerRaged = true;
						ActionsNum = 1;
						cout << "RAAAAAGE EXPLODE! " << endl;
						rr = -10;
					}
				}
			}
		}
		if (Foe.dna.CheckBDY() && Foe.RDP.amount > 0)
		{
			cout << "\nFoe rerolls BDY:";
			if (Foe.RDP.Acquire())
			{
				int r = rand() % BDYDices.size();
				BDYDices[r].Throw();
				cout << r + 1 << "for " << BDYDices[r].NUM;
			}
			else
			{
				isFoeRaged = true;
				ActionsNum = 1;
				cout << "RAAAAAGE EXPLODE! " << endl;
			}
		}

		sort(BDYDices.begin(), BDYDices.end(), CheckDices);

		cout << "\n====BDY  FINAL===" << endl;
		if (!isPlayerRaged && !isFoeRaged)
		{
			for (int i = 0; i < BDYDices.size(); i++)
			{
				cout << i + 1 << ((BDYDices[i].color == WHITE) ? (" w") : (" b")) << ":" << BDYDices[i].NUM << "   ";
			}
			DColor c;
			int lastDice;
			int lastChunk = 1;

			c = BDYDices.back().color;
			lastDice = BDYDices.back().NUM;
			for (int i = BDYDices.size() - 2; i >= 0; i--)
			{

				if (BDYDices[i].color != c)
				{
					if (BDYDices[i].NUM != lastDice)
					{
						lastChunk = 0;
					}
					i = -1;
				}
				else
				{
					if (BDYDices[i].NUM == lastDice)
					{
						lastChunk++;
					}
					else
					{
						lastChunk = 0;
					}

					lastDice = BDYDices[i].NUM;
					ActionsNum++;
				}

			}
			
			ActionsNum -= lastChunk - 1;
			if (BDYDices.back().color == WHITE)
			{
				isFoeRaged = true;
			}
			else {
				isPlayerRaged = true;
			}

			if (ActionsNum == 0)
			{
				isPlayerRaged = isFoeRaged = true;
			}
		}

		cout << "\n====" << endl;

		if (isFoeRaged && !isPlayerRaged)
		{
			cout << "Player wins " << ActionsNum << " actions" << endl;
			while (ActionsNum > 0)
			{
				cout << "\n\n1: Attack"<<endl;
				cout << "2: Move" << endl;
				cout << "3: Defend" << endl;
				int n;
				int R = 0;
				int c = 1;
				cin >> n;
				cout << "\r             \r";
				switch (n)
				{
				case 1:
					ActionsNum--;
					R = xd6(1);
					cout << "You throw: " << R;
					if (Player.RDP.amount > 0)
					{
						while (c > 0)
						{
							cout << " reroll(0 -none, 1-RageToken)?" << endl;
							cin >> c;
							if (c != 0)
							{
								if (Player.RDP.Acquire())
								{
									R = xd6(1);
								}
								else
								{
									cout << "RAAAAAGE EXPLODE! " << endl;
									ActionsNum = 0;
									Player.HP -= 1;
									Foe.HP -= 1;
								}
							}
						}
					}
					R=R+Player.Base.Att / 2;
					if (R > Foe.Base.Def)
					{
						cout << " it's a Hit" << endl;
						Foe.HP -= Player.Base.Att;
					}
					else if (R == Foe.Base.Def)
					{
						cout << " it's a Critical hit!" << endl;
						Foe.HP -= Player.Base.Att * 2;
					}
					else
					{
						cout << " and Missed!" << endl;
					}
					break;
				case 2:
					ActionsNum--;
					R = xd6(1);
					cout << "You throw: " << R;
					if (Player.RDP.amount > 0)
					{
						while (c > 0)
						{
							cout <<" reroll(0 -none, 1-RageToken)?" << endl;
							cin >> c;
							if (c != 0)
							{
								if (Player.RDP.Acquire())
								{
									R = xd6(1);
								}
								else
								{
									cout << "RAAAAAGE EXPLODE! " << endl;
									ActionsNum = 0;
									Player.SubDicePool--;									
								}
							}
						}
					}
					if (Player.RDP.amount<R)
					{
						Player.RDP.amount--;
						cout << " and strafed";
					}
					else
					{
						cout << " and missteped";
					}
				case 3:
					cout << "Stand Ground!" << endl;
					Player.SubDicePool += ActionsNum;
					ActionsNum = 0;
					break;
				default:
					break;
				}
			}
			Foe.RDP.Push();
		}
		else if (isPlayerRaged && !isFoeRaged)
		{
			cout << "Foe wins " << ActionsNum << " actions" << endl;
			Player.RDP.Push();
			while (ActionsNum > 0)
			{
				int n;
				int R = 0;
				int c = 1;
				n = rand()%3 + 1;
				switch (n)
				{
				case 1:
					ActionsNum--;
					R = xd6(1);
					cout << "Foe attack throw: " << R;
					if (Foe.RDP.amount > 0)
					{
					
								if (Player.RDP.Acquire())
								{
									R = xd6(1);
								}
								else
								{
									cout << "RAAAAAGE EXPLODE! " << endl;
									ActionsNum = 0;
									Player.HP -= 1;
									Foe.HP -= 1;
								}
							
						
					}
					R = R + Player.Base.Att / 2;
					if (R > Player.Base.Def)
					{
						cout << " it's a Hit" << endl;
						Player.HP -= Foe.Base.Att;
					}
					else if (R == Player.Base.Def)
					{
						cout << " it's a Critical hit!" << endl;
						Player.HP -= Foe.Base.Att * 2;
					}
					else
					{
						cout << " and Missed!" << endl;
					}
					break;
				case 2:
					ActionsNum--;
					R = xd6(1);
					cout << "Foe move throw: " << R;
					if (Foe.RDP.amount > 0)
					{
						
								if (Player.RDP.Acquire())
								{
									R = xd6(1);
								}
								else
								{
									cout << "RAAAAAGE EXPLODE! " << endl;
									ActionsNum = 0;
									Foe.SubDicePool--;
								}
							
						
					}
					if (Foe.RDP.amount<R)
					{
						Foe.RDP.amount--;
						cout << " and strafed";
					}
					else
					{
						cout << " and missteped";
					}
				case 3:
					cout << "\nFoe Stand Ground!" << endl;
					Foe.SubDicePool += ActionsNum;
					ActionsNum = 0;
					break;
				default:
					break;
				}
			}
		}
		else
		{
			cout << "Draw! Sides retreat! Tuche!" << endl;
		}

		isFoeRaged = isPlayerRaged = false;
		


		system("PAUSE");

	}


	cout << "Battle is over " << endl;
	system("PAUSE");

	return 0;
}

