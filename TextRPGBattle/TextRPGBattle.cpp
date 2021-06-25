// TextRPGBattle.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <time.h>   
#include <stdlib.h>
#include <chrono>
#include<thread>
#include <Windows.h>

using namespace std;
using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;

#define ENEMY_DICES_HIDE false
#define ENEMY_DICES_SHOW true
enum tcolor
{
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};

void textcolor(int color)
{
	HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsoleHandle, color);
}

void wait(int mils)
{
	sleep_until(system_clock::now() + chrono::milliseconds(mils));
}

int xd6(int x, bool show = ENEMY_DICES_SHOW)
{
	int retval = 0;
	for (int i = 0; i < x; i++)
	{
		int R = rand() % 6;
		for (int j = 0; j < rand() % 8 + 8; j++)
		{

			if (show)
			{
				cout << R + 1;
			}
			wait(30);
			R = rand() % 6;
			if (show)
			{
				cout << "\b";
			}

		}
		if (show)
		{
			cout << R + 1;
		}
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
	int SubDice;
	int HP;
	int RC;
	void Use()
	{
		SubDice = 0;
		HP = 0;
		RC = 0;
		Att = Def = BDY = 0;
	}
	Item()
	{
		SubDice = 0;
		HP = 0;
		RC = 0;
		Att = Def = BDY = 0;
		name = "dust";
	}
	Item(int a, int d, int x)
	{
		Att = a;
		Def = d;
		BDY = x;
		SubDice = 0;
		HP = 0;
		RC = 0;
		name = "smth";
	}
	Item(int att, int def, int bd, string nm, int hp = 0, int rt = 0, int sd = 0)
	{
		Att = att;
		Def = def;
		BDY = bd;
		name = nm;
		SubDice = sd;
		HP = hp;
		RC = rt;
	}
};

struct RageDicePool
{
	int amount;
	RageDicePool()
	{
		amount = 0;
	}
	void Push(int a = 1)
	{
		amount += a;
		if (amount > 6)
			amount = 6;
	}
	void Pop(int a = 1)
	{
		amount -= a;
		if (amount < 0)
			amount = 0;
	}

	bool Acquire()
	{
		bool retval = true;
		if (amount > 0)
		{
			textcolor(Red);
			cout << "\nRed dice:";
			wait(600);
			if (xd6(1) <= amount)
			{
				retval = false;
				amount = 0;
				cout << endl;
				textcolor(Magenta);
				for (int i = 0; i < 5; i++)
				{

					cout << "RAAAAAGE!";
					wait(200);
					cout << "\r         \r";
					wait(200);
				}
			}
			else
			{
				cout << endl;
				textcolor(Green);
				amount--;
				for (int i = 0; i < 5; i++)
				{

					cout << "Cool!";
					wait(200);
					cout << "\r         \r";
					wait(200);
				}
			}
		}
		cout << endl;
		textcolor(0x0F);
		return retval;
	}
};

string GetClashText()
{
	string Texts[7] = {
		" none",
		" clang!",
		" bang! ",
		" fhoosh!",
		" swish!",
		" pat!  ",
		" whooze!"
	};
	int R = xd6(1, false);
	cout << Texts[R];
	return Texts[R];
}

string GetWoundText()
{
	string Texts[7] = {
		" none",
		" lung pierced",
		" leg cut",
		" arm wound",
		" head wound",
		" eye pierce",
		" teeth broken"
	};
	int R = xd6(1, false);
	cout << Texts[R];
	return Texts[R];
}

string GetBrokeText()
{
	string Texts[7] = {
		" none",
		" stepped on the dog's shit",
		" stepped on the mud",
		" sneezed",
		" heavy farted",
		" hicced loud",
		" coughed with blood"
	};
	int R = xd6(1, false);
	cout << Texts[R];
	return Texts[R];

}


string GetFriendRoomText()
{
	string Texts[7] = {
		" none",
		" a shady dormitory, all beds are almost broken, \nbut one still has a bedroll and a hay pillow. ",
		" a small marbled room with a small basin in the\n middle, lights are dancing and flickering on the walls and ceilng",
		" an abandoned tavern,but fire pit is still hot, \nthere are some smoked meat and a barrel of ale under the counter",
		" a library, books are almost everythere, even laying\n heaps on the floor, candels are standing dangerously near dried scrolls",
		" a dusty ceremonial hall, huge throne is empty, some\n armors and swords are rusting on the walls, banners with lions and griffons silently hangs from poles",
		" an abandoned armory, weapon racks are empty, \nguards lost their rations on the table and a ring with keys."
	};
	int R = xd6(1, false);
	cout << Texts[R];
	return Texts[R];

}

string GetFoeRoomText()
{
	string Texts[7] = {
		" none",
		" dusty webs and gossamers are hanging around, \nthere is somthing dangerous lurking in the corner",
		" a room full of bleached bones, dried blood \nand tattered rags",
		" an abandoned tavern, broken chairs, dusty \ntables, kitchen utensils and tableware laying around",
		" a torture room, embers glowing in the fire\n pit, chains, racks, and a big skeleton in the rusty cage",
		" a mostly destroyed by magic explosion ceremonial hall,\n human-like shadows on the floor, burned banners hanging from ceilng, rusted and melted weapons laying on the floor.",
		" a former prison hall, cells are opened, \nflooded mud soaking on the floor, moss and dipping water covering walls"
	};
	int R = xd6(1, false);
	cout << Texts[R];
	return Texts[R];

}
enum DNAType
{
	Chaotic,
	Attacker,
	Defender,
	Strafer,
	Charger,
	Rager,
	Sneaker,
	Waller,
};

enum Perk
{
	None,
	Cold,
	Fiechter,
	Dancer,
	Tactitian,
	Sneak,
	Wall,
	Berserker
};

string Perks[8]
{
	"None",
	"Cold",
	"Fiechter",
	"Dancer",
	"Tactitian",
	"Sneaker",
	"Waller",
	"Berserker"
};

struct DNA
{
	DNA(DNAType d = DNAType::Attacker)
	{
		dt = d;
	}
	DNAType dt;
	bool Rerollprob(int RDPamnt)
	{
		if (RDPamnt == 0)
		{
			return false;
		}
		if (dt == Rager || dt == Attacker)
		{
			return true;
		}
		if (RDPamnt > 3)
		{
			return dt == Rager;
		}
		return false;
	}
	int GetAction(int RDPamnt, int HP, int MaxHP, int Stamina)
	{
		int retval = 1;
		switch (dt)
		{
		case Chaotic:
			retval = rand() % 5 + 1;
			break;
		case Attacker:
			if (HP > MaxHP / 3)
			{
				retval = 1;
			}
			else
			{
				retval = rand() % 5 + 1;
			}
			break;
		case Defender:
			if (HP > MaxHP / 3)
			{
				retval = 2;
				if (xd6(1, false) > 4)
				{
					retval = 1;
				}
			}
			else
			{
				retval = rand() % 5 + 1;
			}
			break;
		case Strafer:
			if (HP > MaxHP / 3)
			{
				retval = 3;
				if (xd6(1, false) > 4)
				{
					retval = 1;
				}
			}
			else
			{
				retval = rand() % 5 + 1;
			}
			break;
		case Charger:
			if (HP > MaxHP / 3 && Stamina > 1)
			{
				retval = 4;
				if (xd6(1, false) > 4)
				{
					retval = 1;
				}
			}
			else
			{
				retval = rand() % 5 + 1;
			}
			break;
		case Rager:
			if (RDPamnt > 2)
			{
				retval = 1;
			}
			else
			{
				retval = rand() % 2 + 1;
			}			
			break;
		case Sneaker:
			if (HP > MaxHP / 3)
			{
				retval = 2;
				if (xd6(1, false) > 4)
				{
					retval = 1;
				}
			}
			else
			{
				retval = rand() % 5 + 1;
			}
			break;
		case Waller:
			if (HP > MaxHP / 3)
			{
				retval = 2;
				if (xd6(1, false) > 4)
				{
					retval = 1;
				}
			}
			else
			{
				retval = rand() % 5 + 1;
			}
			break;
		}
				
		return retval;
	}

};

struct ColoredDice
{
	ColoredDice(tcolor dc)
	{
		color = dc;
	}
	ColoredDice(tcolor dc, int n)
	{
		color = dc;
		NUM = n;
	}
	tcolor color;
	int NUM;
	void Throw(bool isShow = true)
	{
		NUM = xd6(1, isShow);
	}
};


bool CheckDices(ColoredDice CD1, ColoredDice CD2)
{
	return (CD1.NUM < CD2.NUM);
}

struct DicePool
{
	vector<ColoredDice> Dices;
	void Clear()
	{
		Dices.clear();
	}
	void MakeThrow(int n, tcolor c, bool isHidden = false)
	{
		for (int i = 0; i < n; i++)
		{
			textcolor(c << 4);
			ColoredDice D(c);
			D.Throw();
			Dices.push_back(D);
		}
		textcolor(White);
	}

	ColoredDice GetMaxColoredSeq()
	{
		int ActionsNum = 0;
		sort(Dices.begin(), Dices.end(), CheckDices);
		tcolor c = Dices.back().color;
		int lastDice = Dices.back().NUM;
		int lastChunk = 0;
		for (int i = Dices.size() - 1; i >= 0; i--)
		{

			if (Dices[i].color != c)
			{
				if (Dices[i].NUM != lastDice)
				{
					lastChunk = 0;
				}
				break;
			}
			else
			{
				if (Dices[i].NUM == lastDice)
				{
					lastChunk++;
				}
				else
				{
					lastChunk = 1;
				}
				if (i == 0)
				{
					lastChunk = 0;
				}
				lastDice = Dices[i].NUM;
				ActionsNum++;
			}

		}

		ActionsNum -= lastChunk;
		return ColoredDice(Dices.back().color, ActionsNum);
	}
	void ShowPool(bool isShaded = false, int s = 0)
	{
		sort(Dices.begin(), Dices.end(), CheckDices);
		textcolor(0x0F);
		cout << "\n=========DICE POOL==================" << endl;
		cout << endl;
		if (!isShaded)
		{
			for (int i = 0; i < Dices.size(); i++)
			{

				textcolor(Dices[i].color << 4);
				cout << i + 1 << ":" << Dices[i].NUM;
				textcolor(0x0F);
				cout << "  ";
			}
		}
		else
		{
			for (int i = 0; i < Dices.size(); i++)
			{

				textcolor(Dices[i].color << 4);
				cout << i + 1 << ":" << Dices[i].NUM;
				textcolor(0x0F);
				cout << "  ";
			}
			cout << "\r";
			s = min(Dices.size() - s, Dices.size());
			for (int i = 0; i < s; i++)
			{
				wait(300);
				textcolor(DarkGray << 4 | DarkGray);
				cout << i + 1 << ":" << Dices[i].NUM;
				textcolor(0x0F);
				cout << "  ";
			}
			for (int i = s; i < Dices.size(); i++)
			{
				textcolor(Dices[i].color << 4);
				cout << i + 1 << ":" << Dices[i].NUM;
				textcolor(0x0F);
				cout << "  ";
			}
		}
		cout << endl;
		textcolor(0x0F);
		cout << "\n==================================" << endl;
	}

};


struct Unit
{
	vector<string> wounds;
	Item Base;
	Item Used;
	Perk myPerk;
	vector<Item> Inventory;
	int HP;
	int MaxHP;
	int Stamina;
	RageDicePool RDP;
	bool isRaged;
	int SubDicePool;
	DNA dna;
	string name;

	void PushRt(int a)
	{
		if (myPerk != Cold)
		{
			RDP.Push(a);
		}
		else {
			if (rand() % 3 != 0)
			{
				cout << " cold perk saves!";
			}
			else
			{
				RDP.Push(a);
			}
		}
	}

	int AskForReroll(int oldR, bool isAI, bool isRageEnable, DicePool* DP = NULL)
	{
		char c = 'y';
		vector<int> rerolldices;
		int R = oldR;
		if (!isAI)
		{

			while (c != 'n' && RDP.amount > 0)
			{

				cout << " reroll(y/n)?" << endl;
				cin >> c;

				if (c != 'n')
				{
					if (DP != NULL)
					{
						cout << RDP.amount << "RT, input numbers 1.."<<rerolldices.size()<<":";
						string rr;
						cin >> rr;
						stringstream ss(rr);

						int i;
						while (ss >> i && rerolldices.size() < RDP.amount)
						{
							if (i > 0)
							{
								rerolldices.push_back(i - 1);
							}
							if (ss.peek() == ',')
							{
								ss.ignore();
							}
						}
						for (int rt = 0; rt < rerolldices.size(); rt++)
						{
							R = DP->Dices[rerolldices[rt]].NUM;
							if (RDP.Acquire())
							{
								int F = xd6(1);
								if (myPerk == Tactitian)
								{
									textcolor(Green);
									if (DP->Dices[rerolldices[rt]].color == White)
									{
										R = max(R, F);
									}
									else
									{
										R = min(R, F);
									}
									cout << " tactical desicion (" << R << "<->" << F << ") :" << R << endl;
								}
								else {
									R = F;
								}
								DP->Dices[rerolldices[rt]].NUM = R;
							}
							else if (isRageEnable)
							{
								isRaged = true;
								textcolor(Magenta);
								GetBrokeText();
								Stamina = 0;
								R = 0;
								break;
							}
							else
							{
								if (rand() % 3 != 0)
								{
									R = 0;
									textcolor(Green);
									cout << " cold perk saves!";
								}	
								else
								{
									isRaged = true;
									textcolor(Magenta);
									GetBrokeText();
									Stamina = 0;
									R = 0;
								}
								break;
							}
						}
					}
					else
					{
						if (RDP.Acquire())
						{
							int F = xd6(1);
							if (myPerk == Tactitian)
							{
								textcolor(Green);
								R = max(R, F);
								cout << " tactical desicion (" << R << "<->" << F << ") :" << R << endl;
							}
							else {
								R = F;
							}
						}
						else if (isRageEnable)
						{
							isRaged = true;
							textcolor(Magenta);
							GetBrokeText();
							Stamina = 0;
							R = 0;
							break;
						}
						else
						{
							if (rand() % 3 != 0)
							{
								R = 0;
								textcolor(Green);
								cout << " cold perk saves!";
							}
							else
							{
								isRaged = true;
								textcolor(Magenta);
								GetBrokeText();
								Stamina = 0;
								R = 0;
							}
							break;
						}
					}

				}

			}

		}
		else
		{
			if (DP != NULL)
			{

				int numtoreroll = rand() % RDP.amount;
				if (numtoreroll > 0)
				{
					cout << name << " choose to reroll: ";
				}
				for (int i = 0; i < numtoreroll; i++)
				{
					rerolldices.push_back(rand() % DP->Dices.size());
					cout << rerolldices.back() + 1 << ",  ";
				}


				for (int rt = 0; rt < rerolldices.size(); rt++)
				{
					R = DP->Dices[rerolldices[rt]].NUM;
					if (RDP.Acquire())
					{
						int F = xd6(1);
						if (myPerk == Tactitian)
						{
							textcolor(Green);
							if (DP->Dices[rerolldices[rt]].color != White)
							{
								R = max(R, F);
							}
							else
							{
								R = min(R, F);
							}
							cout << " tactical desicion (" << R << "<->" << F << ") :" << R << endl;
						}
						else {
							R = F;
						}
						DP->Dices[rerolldices[rt]].NUM = R;
						DP->ShowPool();
					}
					else if (isRageEnable)
					{
						isRaged = true;
						textcolor(Magenta);
						GetBrokeText();
						Stamina = 0;
						R = 0;
						break;
					}
					else
					{
						if (rand() % 3 != 0)
						{
							R = 0;
							textcolor(Green);
							cout << " cold perk saves!";
						}
						else
						{
							isRaged = true;
							textcolor(Magenta);
							GetBrokeText();
							Stamina = 0;
							R = 0;
						}
						break;
					}
				}
			}
			else
			{
				if (RDP.Acquire())
				{
					int F = xd6(1);
					if (myPerk == Tactitian)
					{
						textcolor(Green);
						R = max(R, F);
						cout << " tactical desicion (" << R << "<->" << F << ") :" << R << endl;
					}
					else {
						R = F;
					}
				}
				else if (isRageEnable)
				{
					isRaged = true;
					textcolor(Magenta);
					GetBrokeText();
					Stamina = 0;
					R = 0;
				}
				else
				{
					if (rand() % 3 != 0)
					{
						R = 0;
						textcolor(Green);
						cout << " cold perk saves!";
					}
					else
					{
						isRaged = true;
						textcolor(Magenta);
						GetBrokeText();
						Stamina = 0;
						R = 0;
					}

				}
			}

		}
		textcolor(White);

		return R;

	}

	void DamageMe(int d, bool isCritical = false)
	{
		if (!isCritical)
		{
			HP -= d;
			cout << endl << name << " wounded by " << d;
			wounds.push_back(GetWoundText());
		}
		else
		{
			HP -= d * 2;
			Base.BDY -= rand() % 2;
			cout << endl << name << " heavy wounded by " << d * 2;
			wounds.push_back("heavy " + GetWoundText());
		}
		if (Base.BDY == 0)
		{
			HP = 0;
		}
	}

	void UseItem()
	{
		HP = min(HP + Used.HP, MaxHP);
		SubDicePool += Used.SubDice;
		RDP.amount += Used.RC;
		Base.Att += Used.Att;
		Base.Def += Used.Def;
		Base.BDY += Used.BDY;
		Used.Use();
	}
	bool useRage()
	{
		bool ret = RDP.Acquire();
		if (!ret)
		{
			Stamina = 0;
			isRaged = true;
		}
		return ret;
	}

	int CollectDamageMod()
	{
		int R = Base.BDY / 3 + (myPerk == Fiechter ? 1 : 0);
		return R;
	}

	Unit(int a, int d, int x, string s)
	{
		SubDicePool = 0;
		Base.Att = a;
		Base.Def = d;
		Base.BDY = x;
		HP = 4 + xdy((Base.BDY - 1) * 2, 3);
		MaxHP = 4 + (Base.BDY - 1) * 6;
		Stamina = Base.BDY;
		name = s;
	}
	Unit()
	{
		Base = Item(1, 0, 1);
		SubDicePool = 0;
		HP = 4;
		MaxHP = 4;
		Stamina = 1;
		name = "";
	}
	Unit(string s, int CR)
	{
		SubDicePool = 0;
		Base.Att = 1 + rand() % 3 + CR / 3;
		Base.Def = 1 + rand() % 3 - 1 + CR / 3;
		Base.BDY = CR;
		HP = 4 + xdy((CR - 1) * 2, 3);
		MaxHP = 4 + (Base.BDY - 1) * 6;
		Stamina = Base.BDY;
		name = s;

	}
};



struct Option
{
	string Text;
	Item AddDamage;
	Option(string T, Item* i)
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
		//cout <<p->name<<" hit";
		int Dmg = (p->Base.Att + o->AddDamage.Att);
		Dmg = rand() % Dmg+1;
		retval = Dmg*100;
	}
	else
	{
		//cout << p->name << " miss";
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

		//cout << "\n==============================" << endl;
		//cout << Player->name << " is " << Player->HP << " s:" << Player->Stamina << " [" << Player->Base.Att << ", " << Player->Base.Def << ", " << Player->Base.BDY << "]" << endl;
		//cout << Foe->name << " is " << Foe->HP << " [" << Foe->Base.Att << ", " << Foe->Base.Def << ", " << Foe->Base.BDY << "]" << endl;
		//cout << "==============================" << endl;
		int i = 1;
		int j = 1;
		int PSel = 1;
		int FSel = 0;
		if (Player->Stamina > 0)
		{
			for (Option o : PlayerOptions->at(0))
			{
				//cout << i++ << ". " << o.Text << endl;
			}
			cin >> i;
			if (i >= 1 && i <= PlayerOptions->size())
			{
				PSel = i;
				i = 1;
				for (Option o : PlayerOptions->at(PSel))
				{

					//cout << " *" << i++ << ". " << o.Text << "[" << o.AddDamage.BDY << "]" << endl;
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

		//cout << "\n\nYou " << PlayerOptions->at(0).at(PSel - 1).Text << " " << (PlayerOptions->at(PSel)).at(j - 1).Text << " for " << d << " damage." << endl;
		FSel = rand() % FoeOptions->size();
		d = DoDamageV1(Foe, Player, &(FoeOptions->at(FSel)), &((PlayerOptions->at(PSel)).at(j - 1)));
		//cout << "\n\n" << Foe->name << " " << (FoeOptions->at(i - 1)).Text << "s for " << d << " damage." << endl;

		if (Player->Stamina > Player->Base.BDY)
		{
			Player->Stamina = Player->Base.BDY;
		}


	}
	//cout << "Battle is over " << endl;
	system("PAUSE");
}
/**/






void ShowStats(Unit Player, Unit Foe)
{
	cout << "\n==============================" << endl;

	cout << Player.name;
	cout << "      [BDY:" << Player.Base.BDY << ",  W " << Player.Base.Att << ", A " << Player.Base.Def << "]" << " ad:" << Player.SubDicePool << endl;
	textcolor(Magenta);
	cout << " RT: " << Player.RDP.amount << endl;
	cout << " Perk:" << Perks[Player.myPerk] << endl;
	textcolor(Cyan);
	cout << " Items: ";
	for (Item S : Player.Inventory)
	{

		cout << S.name << ", ";
	}
	cout << endl;
	cout << " Last used:" << Player.Used.name << endl;
	textcolor(Red);
	cout << " Wounds: ";
	for (string S : Player.wounds)
	{

		cout << S << ", ";
	}
	textcolor(White);
	cout << endl << " HP :";
	textcolor(0x02);
	cout << Player.HP << " of " << Player.MaxHP << endl;
	textcolor(0x0F);
	cout << "===============================================" << endl;
	cout << Foe.name;
	/*cout << "      [BDY:" << Foe.Base.BDY << ", weapon " << Foe.Base.Att << ", armor " << Foe.Base.Def << "]" << endl << " RageT: " << Foe.RDP.amount << " Perk:" << Perks[Foe.myPerk];
	cout << "  ad:" << Foe.SubDicePool ;
	*/

	textcolor(Red);
	cout << endl << " Wounds: ";
	for (string S : Foe.wounds)
	{

		cout << S << ", ";
	}
	textcolor(White);
	cout << endl << " HP :";
	textcolor(0x02);
	cout << Foe.HP << " of " << Foe.MaxHP << endl;
	textcolor(Magenta);
	cout << " RT: " << Foe.RDP.amount << endl;
	textcolor(0x0F);
	cout << "===============================================" << endl;
	wait(500);
}


void ShowStatsMin(Unit Player, Unit Foe)
{
	textcolor(White);
	cout << "\n===============================================" << endl;
	cout << Player.name << " HP :";
	textcolor(0x02);
	cout << Player.HP << " of " << Player.MaxHP;
	textcolor(White);
	cout << "          " << Foe.name << " HP :";
	textcolor(0x02);
	cout << Foe.HP << " of " << Foe.MaxHP << endl;
	textcolor(Magenta);
	cout << " RT:" << Player.RDP.amount << "                         RT:" << Foe.RDP.amount << endl;
	textcolor(0x0F);
	cout << "\n===============================================" << endl;
	wait(500);
}


void UseItem(Unit* Player, bool isRested = true, bool isAI = false, bool isShowDices = true)
{
	if (isRested)
	{
		if (!isAI)
		{
			int R = xd6(1, isShowDices);
			int itemnum = -1;
			while (itemnum < 0 && itemnum >= Player->Inventory.size())
			{
				cout << "Inventory:" << endl;
				for (int i = 0; i < Player->Inventory.size(); i++)
				{
					Item ii = Player->Inventory[i];
					cout << i + 1 << ": " << ii.name << "[" << ii.BDY << ", " << ii.Att << ", " << ii.Def << ", " << ii.HP << ", " << ii.RC << ", " << ii.SubDice << "]" << endl;
				}
				cin >> itemnum;
				itemnum--;
			}
			Item SR = Player->Inventory[itemnum];
			Player->Used = SR;
			Player->Inventory[itemnum] = Player->Inventory.back();
			Player->Inventory.pop_back();
			cout << "uses: " << Player->Used.name << " (vs " << Player->RDP.amount - Player->Stamina << "AP)" << endl;
			if (Player->RDP.amount > 0)
			{
				R = Player->AskForReroll(R, isAI, Player->myPerk != Cold);
			}

			if (R > 0)
			{
				R = R + (Player->myPerk == Cold ? 1 : 0);
				if (R > Player->RDP.amount - Player->Stamina)
				{
					textcolor(Green);
					cout << " success!" << endl;
					Player->UseItem();

				}
				else
				{
					textcolor(Magenta);
					cout << " fails!" << endl;
					cout << endl << Player->name << " opens bag and";
					GetBrokeText();
					cout << endl;
				}
				textcolor(White);

			}
			else
			{
				cout << endl << Player->name << " opens bag and";
				GetBrokeText();
				cout << endl;
				Player->Used.Use();
			}
		}
	}
}


void MakeActions(Unit* Player, Unit* Foe, bool isAI, bool isShowDices = true)
{
	cout << endl << Player->name << " wins " << Player->Stamina << " actions," << Foe->name << " get 1 RT" << endl;
	Foe->PushRt(1);
	while (Player->Stamina > 0)
	{
		int n = 0;
		int R = 0;
		if (!isAI)
		{
			while (n <= 0 || n >6&&n!=99)
			{
				cout << "\nSelect(" << Player->Stamina << " ap left):" << endl;
				cout << "1: Attack(1 ap)" << endl;
				cout << "2: Retreat(1 ap)" << endl;
				cout << "3: Strafe(1 ap)" << endl;
				cout << "4: Charge(2 ap)" << endl;
				cout << "5: Defend(" << Player->Stamina << " ap)" << endl;
				cout << "6: Use(" << Player->Stamina << " ap)" << endl;
				cout << "99:debug_win" << endl;
				wait(500);
				cin >> n;
				cout << "\r             \r";
			}
		}
		else
		{
			n = Player->dna.GetAction(Player->RDP.amount, Player->HP, Player->MaxHP, Player->Stamina);
		}

		cout << endl << Player->name << " rolls:";
		R = xd6(1, isShowDices);
		switch (n)
		{
		case 99:
			Foe->HP = 0;
			Player->Stamina = 0;
			break;
		case 1:
			Player->Stamina--;
			cout << " attacks (vs " << Foe->name << "'s " << Foe->Base.Def << "DEF)" << endl;
			if (Player->RDP.amount > 0)
			{
				R = Player->AskForReroll(R, isAI, !(Player->myPerk == Fiechter));
			}
			if (R != 0)
			{
				R = R + (Player->myPerk == Fiechter ? 1 : 0);
				if (R >= Foe->Base.Def)
				{
					textcolor(Green);
					cout << " success!" << endl;
					Foe->DamageMe(Player->Base.Att-(Player->Base.Att/2)*(Foe->myPerk != Wall?0:1), R == Foe->Base.Def);
					cout << endl << Foe->name << "  HP:" << Foe->HP << endl;

				}
				else
				{
					textcolor(Magenta);
					cout << " fails!" << endl;
					cout << endl << Player->name << " attacks and";
					GetBrokeText();
					cout << endl;
				}
				textcolor(White);
			}
			else
			{
				Player->DamageMe(3); //Player->att-- ?
				Foe->DamageMe(1);
				Player->Stamina = 0;
			}
			break;
		case 2:
			Player->Stamina--;
			cout << " retreats (vs " << Player->name << "'s " << Player->RDP.amount << "RT)" << endl;
			if (Player->RDP.amount > 0)
			{
				R = Player->AskForReroll(R, isAI, !(Player->myPerk == Dancer));
			}

			if (R != 0)
			{
				R = R + (Player->myPerk == Dancer ? 1 : 0);
				if (R > Player->RDP.amount)
				{
					textcolor(Green);
					cout << " success!" << endl;
					Player->RDP.Pop();
					cout << endl << Player->name << "  RT:" << Player->RDP.amount << endl;
					if (Foe->dna.dt == DNAType::Sneaker)
					{
						Foe->Stamina += 1;
						cout << endl <<Foe->name << " Sneaks around and gain initiative!"  << endl;
					}
				}
				else
				{
					textcolor(Magenta);
					cout << " fails!" << endl;
					cout << endl << Player->name << " retreats and";
					GetBrokeText();
					cout << endl;
				}
				textcolor(White);
			}
			else
			{
				if (Foe->dna.dt == DNAType::Sneaker)
				{
					Foe->Stamina +=1;
					cout << endl << Foe->name << " Sneaks around and gain initiative!" << endl;
				}
				Player->SubDicePool--;
				Player->Stamina = 0;
			}
			break;
		case 3:
			Player->Stamina--;
			cout << " strafes (vs " << Foe->name << "'s " << Foe->RDP.amount << "RT)" << endl;
			if (Player->RDP.amount > 0)
			{
				R = Player->AskForReroll(R, isAI, !(Player->myPerk == Dancer));
			}

			if (R != 0)
			{
				R = R + (Player->myPerk == Dancer ? 1 : 0);
				if (R > Foe->RDP.amount)
				{
					textcolor(Green);
					cout << " success!" << endl;
					Foe->PushRt(1);
					cout << endl << Foe->name << "  RT:" << Foe->RDP.amount << endl;

				}
				else
				{
					textcolor(Magenta);
					cout << " fails!" << endl;
					cout << endl << Player->name << " retreats and";
					GetBrokeText();
					cout << endl;
				}
				textcolor(White);
			}
			else
			{
				Player->SubDicePool--;
				Player->Stamina = 0;
			}
			break;
		case 4:
			if (Player->Stamina >= 2)
			{
				Player->Stamina -= 2;
				cout << " charges (vs " << Foe->name << "'s " << Foe->Base.Def << "DEF)" << endl;
				if (Player->RDP.amount > 0)
				{
					R = Player->AskForReroll(R, isAI, Player->myPerk != Dancer && Player->myPerk != Fiechter);
				}

				if (R != 0)
				{
					R = R + (Player->myPerk == Dancer ? 1 : 0) + (Player->myPerk == Fiechter ? 1 : 0);
					if (R > Foe->Base.Def)
					{
						textcolor(Green);
						cout << " success!" << endl;
						
						if (Foe->dna.dt == DNAType::Sneaker)
						{
							Foe->DamageMe(Player->Base.Att, false);
							Foe->Stamina += 1;
							cout << endl << Foe->name << " Sneaks around and gain initiative!" << endl;
						}
						else
						{
							Foe->DamageMe(Player->Base.Att - (Player->Base.Att / 2) * (Foe->myPerk != Wall ? 0 : 1), true);
						}
					
						cout << endl << Foe->name << "  HP:" << Foe->HP << endl;
					}
					else
					{
						textcolor(Magenta);
						cout << " fails!" << endl;
						cout << endl << Player->name << " charges and";
						GetBrokeText();
						cout << endl;
					}
					textcolor(White);
				}
				else
				{
					Player->DamageMe(3); //Player->att-- ?
					Player->SubDicePool--;
					Foe->DamageMe(1);
					Player->Stamina = 0;
				}
			}
			break;
		case 5:
			Player->SubDicePool += Player->Stamina + (Player->myPerk == Cold ? 1 : 0);
			cout << " and stands ground!" << endl;
			Player->Stamina = 0;
			break;
		case 6:
		{
			int itemnum = -1;
			while (itemnum < 0 && itemnum >= Player->Inventory.size())
			{
				cout << "Inventory:" << endl;
				for (int i = 0; i < Player->Inventory.size(); i++)
				{
					Item ii = Player->Inventory[i];
					cout << i + 1 << ": " << ii.name << "[" << ii.BDY << ", " << ii.Att << ", " << ii.Def << ", " << ii.HP << ", " << ii.RC << ", " << ii.SubDice << "]" << endl;
				}
				cin >> itemnum;
				itemnum--;
			}
			Item SR = Player->Inventory[itemnum];
			Player->Used = SR;
			Player->Inventory[itemnum] = Player->Inventory.back();
			Player->Inventory.pop_back();

			cout << "uses: " << Player->Used.name << " (vs " << Player->RDP.amount - Player->Stamina << "AP)" << endl;
			if (Player->RDP.amount > 0)
			{
				R = Player->AskForReroll(R, isAI, Player->myPerk != Cold);
			}

			if (R > 0)
			{
				R = R + (Player->myPerk == Cold ? 1 : 0);
				if (R > Player->RDP.amount - Player->Stamina)
				{
					textcolor(Green);
					cout << " success!" << endl;
					Player->UseItem();

				}
				else
				{
					textcolor(Magenta);
					cout << " fails!" << endl;
					cout << endl << Player->name << " opens bag and";
					GetBrokeText();
					cout << endl;
				}
				textcolor(White);

			}
			else
			{
				cout << endl << Player->name << " opens bag and";
				GetBrokeText();
				cout << endl;
				Player->Used.Use();
			}
			Player->Stamina = 0;
		}
		break;
		default:
			break;
		}

	}
}


bool Battle(Unit* Player, Unit* Foe)
{
	bool isPlayerWin = true;
	while (Player->HP > 0 && Foe->HP > 0)
	{
		system("CLS");
		ShowStats(*Player, *Foe);
		system("PAUSE");
		system("CLS");
		Player->isRaged = false;
		Foe->isRaged = false;
		Player->Stamina = 0;
		Foe->Stamina = 0;
		ShowStatsMin(*Player, *Foe);
		cout << "Fighting for initiative:" << endl;

		int zP = Player->Base.BDY + Player->SubDicePool;
		int zF = Foe->Base.BDY + Foe->SubDicePool;
		if (Foe->RDP.amount >= 6)
		{
			cout << Foe->name << " RAGED!" << endl;
			Foe->RDP.amount = 0;
			zF = 0;
			zP = Player->Base.BDY / 3;
			Foe->isRaged = true;
		}
		if (Player->RDP.amount >= 6)
		{
			Player->isRaged = true;
			cout << Player->name << " RAGED!" << endl;
			Player->RDP.amount = 0;
			zP = 0;
			zF = Player->Base.BDY / 3;
		}

		int Z = zP + zF;
		DicePool BDYCheck;
		for (int i = 0; i < Z; i++)
		{
			int side = rand() % 2;
			if (zP <= 0)
			{
				side = 0;
			}
			if (zF <= 0)
			{
				side = 1;
			}
			cout << endl;
			if (side == 1)
			{
				GetClashText();
				BDYCheck.MakeThrow(1, White);
				zP--;
			}
			else
			{
				cout << "                             ";
				GetClashText();
				BDYCheck.MakeThrow(1, DarkGray);
				zF--;
			}
			textcolor(0x0F);
			cout << "    ";
		}

		Player->SubDicePool = 0;
		Foe->SubDicePool = 0;
		BDYCheck.ShowPool();
		wait(300);
		if (Player->RDP.amount > 0)
		{
			if (Player->AskForReroll(-1, false, Player->myPerk != Cold, &BDYCheck) == 0)
			{
				Player->isRaged = true;
			}
		}
		if (Foe->RDP.amount > 0 && Foe->dna.Rerollprob(Foe->RDP.amount))
		{
			if (Foe->AskForReroll(-1, true, Foe->myPerk != Cold, &BDYCheck) == 0)
			{
				Foe->isRaged = true;
			}
		}
		wait(300);
		ColoredDice Actions = BDYCheck.GetMaxColoredSeq();

		if (Player->isRaged)
		{
			Actions.NUM = 1;
			Actions.color = DarkGray;
		}
		else if (Foe->isRaged)
		{
			Actions.NUM = 1;
			Actions.color = White;
		}
		else
		{
			BDYCheck.ShowPool(true, Actions.NUM);
		}

		if (Actions.color == White)
		{
			Foe->Stamina = 0;
			Player->Stamina = Actions.NUM;
		}
		else {
			Player->Stamina = 0;
			Foe->Stamina = Actions.NUM;
		}
		cout << endl;

		system("PAUSE");
		system("CLS");
		while (Player->Stamina > 0 || Foe->Stamina > 0)
		{


			if (Player->Stamina > 0)
			{
				ShowStats(*Player, *Foe);
				MakeActions(Player, Foe, false, true);

			}
			else if (Foe->Stamina > 0)
			{
				ShowStatsMin(*Player, *Foe);
				MakeActions(Foe, Player, true, true);
			}
			else
			{
				cout << "Draw! Retreat! Tuche!" << endl;

				Player->PushRt(1);
				Foe->PushRt(1);

			}
		}

		system("PAUSE");
	}

	if (Player->HP <= 0)
	{
		isPlayerWin = false;
	}
	cout << "Battle is over " << endl;
	system("PAUSE");
	return isPlayerWin;
}



Unit MakeFoe(vector<Unit> Enemies, int PCR = 0)
{

	int FoeLevel = rand() % Enemies.size();
	if (PCR != 0)
	{
		FoeLevel = rand() % PCR;
	}
	Unit Foe = Enemies[FoeLevel];
	Foe.myPerk = Perk(rand() % 5);
	Foe.dna.dt = DNAType(rand() % 6);
	return Foe;
}

int main()
{

	textcolor(White);
	srand(time(NULL));
	vector<Item> Inventory;
	vector<Item> Loot;
	vector<Item> Treasure;
	vector<vector<Option>> PlayerOptions;
	vector<Option> FoeOptions;
	vector<Unit> Enemies;
	Enemies.push_back(Unit("Rat", 1));
	Enemies.push_back(Unit("Goblin", 2));
	Enemies.push_back(Unit("HobGoblin", 3));
	Enemies.push_back(Unit("Orc", 4));
	Enemies.push_back(Unit("Orc Chief", 5));
	Enemies.push_back(Unit("Troll", 6));
	Enemies.push_back(Unit("Owlbear", 7));
	Enemies.push_back(Unit("Sabertooth", 8));
	Enemies.push_back(Unit("Dragon", 9));

	int FoeLevel = -1;



	Unit Player;
	cout << unitbuf;
	int pprk = -1;
	int cr = 1;
	/*while (FoeLevel < 0 && FoeLevel > Enemies.size())
	{
		cout << "Select Foe:" << endl;
		cout << "0 - Random" << endl;
		for (int f = 0; f < Enemies.size(); f++)
		{
			cout << f + 1 << " - " << Enemies[f].name << endl;
		}
		cout << "Enter 0-" << Enemies.size() << ":";
		cin >> FoeLevel;
	}
	if (FoeLevel == 0)
	{
		FoeLevel = rand() % Enemies.size();
	}
	else
	{
		FoeLevel--;
	}
	Unit Foe = Enemies[FoeLevel];


	cout << "Hello stranger, i am " << Foe.name << endl;
	cout << "What is my Perk?" << endl;
	cout << "0 - None" << endl;
	cout << "1 - Cold (no ragetoken gain)" << endl;
	cout << "2 - Fiechter (no rage exploding in attacks)" << endl;
	cout << "3 - Dancer(no rage exploding in moves)" << endl;
	cout << "4 - Tactitian(best reroll dice)" << endl;
	cout << "5 - Random" << endl;





	while (pprk < 0 || pprk>5)
	{
		cout << "Enter 0 - 5:";
		cin >> pprk;
	}
	if (pprk != 5)
	{
		Foe.myPerk = Perk(pprk);
	}
	else
	{
		Foe.myPerk = Perk(rand() % 5);
	}


	cout << "What is my Behevior?" << endl;
	cout << "0 - Chaotic" << endl;
	cout << "1 - Attacker" << endl;
	cout << "2 - Defender" << endl;
	cout << "3 - Strafer" << endl;
	cout << "4 - Charger" << endl;
	cout << "5 - Rager" << endl;
	pprk = -1;
	while (pprk < 0 || pprk>5)
	{
		cout << "Enter 0 - 5:";
		cin >> pprk;
	}
	Foe.dna.dt = DNAType(pprk);

	cout << "[" << Foe.Base.BDY << "," << Foe.Base.Att << "," << Foe.Base.Def << "]" << endl;
	*/
	cout << "State your name, stranger!: ";
	cin >> Player.name;
	//cout << "Tell me," << Player.name << " your level: ";	
	//cin >> cr;


	cout << "What is your Perk?" << endl;
	cout << "0 - None" << endl;
	cout << "1 - Cold (no ragetoken gain)" << endl;
	cout << "2 - Fiechter (no rage exploding in attacks)" << endl;
	cout << "3 - Dancer(no rage exploding in moves)" << endl;
	cout << "4 - Tactitian(best reroll dice)" << endl;
	cout << "5 - Sneaker(instant base attack then enemy retreats)" << endl;
	cout << "6 - Wall(reduce enemy attacks to one step)" << endl;
	pprk = -1;
while (pprk < 0 || pprk>6)
	{
		cout << "Enter 0 - 6:";
		cin >> pprk;
	}
	Player.myPerk = Perk(pprk);

	for (int i = 0; i < 8; i++)
	{

		textcolor(rand() % 14);
		cout << Player.name << " you must save my daughter from the deeps of this dungeon! Help her!\r";
		wait(300);
	}
	textcolor(White);
	///Item(int att, int def, int bd, string nm, int hp = 0, int rt = 0, int sd = 0)
	Inventory.push_back(Item(0, 0, 0, "apple", 10, 0, 0));
	Inventory.push_back(Item(0, 0, 0, "potion", 30, 0, 0));
	Inventory.push_back(Item(0, 0, 0, "potion of ice cold", 0, -5, 0));
	Inventory.push_back(Item(0, 0, 0, "potion of blood burn", 0, 2, 2));
	Inventory.push_back(Item(0, -1, 1, "potion of fury", 0, 0, 0));
	Inventory.push_back(Item(1, 0, 0, "sword", 0, 0, 0));
	Inventory.push_back(Item(0, 1, 0, "shield", 0, 0, 0));

	Loot.push_back(Item(1, 0, 0, "dagger", 0, 0, 0));
	Loot.push_back(Item(0, 1, 0, "buckler", 0, 0, 0));
	Loot.push_back(Item(0, 2, 0, "shield", 0, 0, 0));
	Loot.push_back(Item(2, 0, 0, "sword", 0, 0, 0));
	Loot.push_back(Item(1, 1, 0, "pike", 0, 0, 0));
	Loot.push_back(Item(2, 1, 0, "halebard", 0, 0, 0));
	Loot.push_back(Item(0, 1, 0, "chainmale", 0, 0, 0));
	Loot.push_back(Item(0, 2, 0, "cuirass", 0, 0, 0));
	Loot.push_back(Item(3, 1, 0, "katana", 0, 0, 0));
	Loot.push_back(Item(0, 3, 0, "fullplate", 0, 0, 0));





	Player.Inventory.push_back(Item(0, 0, 0, "apple", 10, 0, 0));
	Unit Foe = MakeFoe(Enemies, 2);
	Player.RDP.amount = 1;
	Foe.RDP.amount = 1;
	system("PAUSE");

	textcolor(White);
	cout << "You opened door and see:" << endl;
	textcolor(LightMagenta);
	GetFoeRoomText();
	cout << endl;
	system("PAUSE");
	system("CLS");
	textcolor(Magenta);
	cout << endl << "Watch out, there is a ";
	textcolor(Yellow);
	cout << Foe.name;
	textcolor(Magenta);
	cout << " ready to attack! To arms!" << endl;
	textcolor(White);
	system("PAUSE");
	while (Battle(&Player, &Foe))
	{
		system("CLS");
		textcolor(White);
		cout << endl << "You win this time. Time to move forward" << endl;
		int i = rand() % Loot.size();
		cout << "You found: " << Loot[i].name << endl;
		Player.Inventory.push_back(Loot[i]);
		if (rand() % 2)
		{
			Player.Base.BDY++;
			cout << "You feel much stronger now!" << endl;
		}
		Player.HP += xdy(2, 3);
		Player.MaxHP = Player.Base.BDY * 6;
		Foe = MakeFoe(Enemies, max(1, Player.Base.BDY + rand() % 3 - 1));
		textcolor(White);
		system("PAUSE");
		system("CLS");
		if (rand() % 3 == 2)
		{
			cout << endl << "You opened door and see:" << endl;
			textcolor(LightGreen);
			GetFriendRoomText();
			system("PAUSE");
			int i = rand() % Inventory.size();
			cout << "You found: " << Inventory[i].name << endl;
			Player.Inventory.push_back(Inventory[i]);
			char yn = 'y';
			while (yn != 'n')
			{
				cout << "Use any item(y/n)?";
				cin >> yn;
				if (yn == 'y')
				{
					UseItem(&Player);
				}
				cout << "Rest more(y/n)?";
				cin >> yn;
				textcolor(Yellow);
				cout << Player.name << ", you rest well and ready to fight more!" << endl;
				textcolor(White);
				Player.wounds.clear();
				system("PAUSE");
				system("CLS");
			}


		}

		cout << endl << "You opened next door and see:" << endl;
		textcolor(LightMagenta);
		GetFoeRoomText();
		system("PAUSE");
		system("CLS");
		textcolor(Magenta);
		cout << endl << "Watch out, there is a ";
		textcolor(Yellow);
		cout << Foe.name;
		textcolor(Magenta);
		cout << " ready to attack! To arms!" << endl;
		textcolor(White);
		system("PAUSE");

	}
	textcolor(Magenta);
	cout << "Quest is over. Your bones lonely whites in shades of dungeon.";

	return 0;
}

