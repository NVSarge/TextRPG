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
	Item(int a, int d, int x, string s, int h = 0, int rc = 0, int sd = 0)
	{
		Att = a;
		Def = d;
		BDY = x;
		name = s;
		SubDice = sd;
		HP = h;
		RC = rc;
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
		amount += 1;
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
		" hoosh!",
		" swish!",
		" pat!  ",
		" wooze!"
	};
	int R = xd6(1, false);
	cout << Texts[R];
	return Texts[R];
}

string GetWoundText()
{
	string Texts[7] = {
		" none",
		" pierce lung",
		" cut leg",
		" cut arm",
		" wound head",
		" scratched eye",
		" broke teeth"
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
		" stepped in the mud",
		" sneezed",
		" heavy farted",
		" hicced up fluorishly",
		" coughed with blood"
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
	Rager
};

enum Perk
{
	None,
	Cold,
	Fiechter,
	Dancer,
	Tactitian
};

string Perks[5]
{
	"None",
	"Cold",
	"Fiechter",
	"Dancer",
	"Tactitian"
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
	void ShowPool()
	{
		sort(Dices.begin(), Dices.end(), CheckDices);
		textcolor(0x0F);
		cout << "\n=========DICE POOL==================" << endl;
		cout << endl;
		for (int i = 0; i < Dices.size(); i++)
		{

			textcolor(Dices[i].color << 4);
			cout << i + 1 << ":" << Dices[i].NUM;
			textcolor(0x0F);
			cout << "  ";
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
			cout << " cold perk saves!";
		}
	}

	int AskForReroll(int oldR, bool isAI, bool isRageEnable, DicePool *DP = NULL)
	{
		char c = 'y';
		vector<int> rerolldices;
		int R = oldR;
		if (!isAI)
		{

			while (c != 'n'&&RDP.amount > 0)
			{

				cout << " reroll(y/n)?" << endl;
				cin >> c;

				if (c != 'n')
				{
					if (DP != NULL)
					{
						cout << RDP.amount << "RT, input numbers e.g. 1,4,7: ";
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
									cout << " tactical desicion: " << R << endl;
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
								R = 0;
								textcolor(Green);
								cout << " perks saves!" << endl;
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
								cout << " tactical desicion: " << R << endl;
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
							R = 0;
							textcolor(Green);
							cout << " perks saves!" << endl;
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
							cout << " tactical desicion: " << R << endl;
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
						R = 0;
						textcolor(Green);
						cout << " perks saves!" << endl;
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
						cout << " tactical desicion: " << R << endl;
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
					R = 0;
					textcolor(Green);
					cout << " perks saves!" << endl;

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
		HP = 4 + xdy(Base.BDY * 2 - 2, 3);
		MaxHP = Base.BDY * 6;
		Stamina = Base.BDY;
		name = s;
	}
	Unit()
	{
		SubDicePool = 0;
		HP = 1;
		Stamina = 1;
		name = "";
	}
	Unit(string s, int CR)
	{
		SubDicePool = 0;
		Base.Att = 1 + rand() % 3 + CR / 3;
		Base.Def = 1 + rand() % 3 - 1 + CR / 3;
		Base.BDY = CR;
		HP = 4 + 2 * xdy((CR - 1) * 2, 3);
		MaxHP = Base.BDY * 6;
		Stamina = Base.BDY;
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
	cout << "\n==============================" << endl << endl;

	cout << Player.name << endl;
	cout << " [BDY:" << Player.Base.BDY << ",  weapon " << Player.Base.Att << ", armor " << Player.Base.Def << "]" << endl << " RageT: " << Player.RDP.amount << " Perk:" << Perks[Player.myPerk] << endl;
	cout << "ad:" << Player.SubDicePool << endl << endl;
	textcolor(Cyan);
	cout << " Last used:" << Player.Used.name << endl;
	cout << "Items: ";
	for (Item S : Player.Inventory)
	{

		cout << S.name << ", ";
	}
	cout << endl;
	textcolor(Red);
	cout << "Wounds: ";
	for (string S : Player.wounds)
	{

		cout << S << ", ";
	}
	textcolor(White);
	cout << endl << " HP :";
	textcolor(0x02);
	cout << Player.HP << " of " << Player.MaxHP << endl;
	textcolor(0x0F);
	cout << "\n==============================" << endl << endl;
	cout << Foe.name << endl;
	cout << " [BDY:" << Foe.Base.BDY << ", weapon " << Foe.Base.Att << ", armor " << Foe.Base.Def << "]" << endl << " RageT: " << Foe.RDP.amount << " Perk:" << Perks[Foe.myPerk] << endl;
	cout << "ad:" << Foe.SubDicePool << endl << endl;
	textcolor(Red);
	cout << "Wounds: ";
	for (string S : Foe.wounds)
	{

		cout << S << ", ";
	}
	textcolor(White);
	cout << endl << " HP :";
	textcolor(0x02);
	cout << Foe.HP << " of " << Foe.MaxHP << endl;
	textcolor(0x0F);
	cout << "\n==============================" << endl;
	wait(500);
}





void MakeActions(Unit *Player, Unit *Foe, bool isAI, bool isShowDices = true)
{
	cout << endl << Player->name << " wins " << Player->Stamina << " actions," << Foe->name << " get 1 RT" << endl;
	Foe->RDP.Push();
	while (Player->Stamina > 0)
	{
		int n = 0;
		int R = 0;
		if (!isAI)
		{
			while (n <= 0 || n > 6)
			{
				cout << "\nSelect(" << Player->Stamina << " ap left):" << endl;
				cout << "1: Attack(1 ap)" << endl;
				cout << "2: Retreat(1 ap)" << endl;
				cout << "3: Strafe(1 ap)" << endl;
				cout << "4: Charge(2 ap)" << endl;
				cout << "5: Defend(" << Player->Stamina << " ap)" << endl;
				cout << "6: Use(" << Player->Stamina << " ap)" << endl;
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
		case 1:
			Player->Stamina--;
			cout << " attacks (vs " << Foe->name << "'s " << Foe->Base.Def << "DEF)" << endl;
			if (Player->RDP.amount > 0)
			{
				R = Player->AskForReroll(R, isAI, !(Player->myPerk == Fiechter));
			}
			R = R + (Player->myPerk == Fiechter ? 1 : 0);
			if (R != 0)
			{
				if (R >= Foe->Base.Def)
				{
					textcolor(Green);
					cout << " success!" << endl;
					Foe->DamageMe(Player->Base.Att, R == Foe->Base.Def);
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
			R = R + (Player->myPerk == Dancer ? 1 : 0);
			if (R != 0)
			{
				if (R > Player->RDP.amount)
				{
					textcolor(Green);
					cout << " success!" << endl;
					Player->RDP.Pop();
					cout << endl << Player->name << "  RT:" << Player->RDP.amount << endl;

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
		case 3:
			Player->Stamina--;
			cout << " strafes (vs " << Foe->name << "'s " << Foe->RDP.amount << "RT)" << endl;
			if (Player->RDP.amount > 0)
			{
				R = Player->AskForReroll(R, isAI, !(Player->myPerk == Dancer));
			}
			R = R + (Player->myPerk == Dancer ? 1 : 0);
			if (R != 0)
			{
				if (R > Player->RDP.amount)
				{
					textcolor(Green);
					cout << " success!" << endl;
					Foe->RDP.Push();
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
				R = R + (Player->myPerk == Dancer ? 1 : 0) + (Player->myPerk == Fiechter ? 1 : 0);
				if (R != 0)
				{
					if (R > Foe->Base.Def)
					{
						textcolor(Green);
						cout << " success!" << endl;
						Foe->DamageMe(Player->Base.Att, true);
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
			while (itemnum == -1)
			{
				cout << "Inventory:" << endl;
				for (int i = 0; i < Player->Inventory.size(); i++)
				{
					Item ii = Player->Inventory[i];
					cout << i + 1 << ": " << ii.name << "[" << ii.BDY << ", " << ii.Att << ", " << ii.Def << ", " << ii.HP << ", " << ii.RC << ", " << ii.SubDice << "]" << endl;
				}
				cin >> itemnum;
				itemnum--;
				if (R >= 0 && R < Player->Inventory.size())
				{
					Item SR = Player->Inventory[itemnum];
					Player->Used = SR;
					Player->Inventory[itemnum] = Player->Inventory.back();
					Player->Inventory.pop_back();
				}
				else
				{
					itemnum = -1;
				}
			}
		}
		cout << "uses: " << Player->Used.name << " (vs " << Player->RDP.amount - Player->Stamina << "AP)" << endl;
		if (Player->RDP.amount > 0)
		{
			R = Player->AskForReroll(R, isAI, Player->myPerk != Cold);
		}
		R = R + (Player->myPerk == Cold ? 1 : 0);

		if (R > 0)
		{
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
				cout << endl << Player->name << " open sbag and";
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
		break;
		default:
			break;
		}

	}
}


/*

case 2:
Player->Stamina--;
cout << " moves: ";
R = xd6(1, isShowDices);
if (Player->RDP.amount > 0)
{
while (c != 'n'&&Player->RDP.amount > 0)
{
if (!isAI)
{
cout << " reroll(y/n," << Player->RDP.amount << "rt)?" << endl;
cin >> c;
}
else
{
if (Player->dna.Rerollprob(Player->RDP.amount))
{
c = 'y';
}
}
if (c != 'n')
{
if (Player->RDP.Acquire())
{
int F = xd6(1);
if (Player->myPerk == Tactitian)
{
textcolor(Green);
R = max(R, F);
cout << " tactical desicion: " << R << endl;
}
}
else if (Player->myPerk != Dancer)
{
isRaged = true;
textcolor(Magenta);
GetBrokeText();
Player->SubDicePool--;
textcolor(White);
}
else
{
textcolor(Green);
cout << " dancer perks saves!" << endl;
}
textcolor(White);
}
}
}
if (!isRaged)
{
R = R + (Player->myPerk == Dancer ? 1 : 0);
cout << " ->" << R << " vs " << Player->RDP.amount << endl;
cout << endl << Player->name << " moves back and ";
if (Player->RDP.amount < R&&Player->RDP.amount>0)
{
Player->PushRt(-1);
cout << Player->name << " loses one rage token." << endl;
}
else
{
GetBrokeText();
cout << endl;
}
}
break;
case 3:
Player->Stamina--;
cout << " strafes: ";
R = xd6(1, isShowDices);
if (Player->RDP.amount > 0)
{
while (c != 'n'&&Player->RDP.amount > 0)
{
if (!isAI)
{
cout << " reroll(y/n," << Player->RDP.amount << "rt)?" << endl;
cin >> c;
}
else
{
if (Player->dna.Rerollprob(Player->RDP.amount))
{
c = 'y';
}
}
if (c != 'n')
{
if (Player->RDP.Acquire())
{
int F = xd6(1);
if (Player->myPerk == Tactitian)
{
textcolor(Green);
R = max(R, F);
cout << " tactical desicion: " << R << endl;
}
}
else if (Player->myPerk != Dancer)
{
isRaged = true;
textcolor(Magenta);
GetBrokeText();
Player->SubDicePool--;
textcolor(White);
}
else
{
textcolor(Green);
cout << " attacker perks saves!" << endl;
}
textcolor(White);
}
}
}
if (!isRaged)
{
R = R + (Player->myPerk == Dancer ? 1 : 0);
cout << " ->" << R << " vs " << Foe->RDP.amount << endl;
cout << endl << Player->name << " strafes back and ";
if (Foe->RDP.amount < R)
{
Foe->PushRt(1);
cout << Foe->name << " gets one rage token." << endl;
}
else
{
GetBrokeText();
cout << endl;
}
break;
}
case 4:
if (Player->Stamina > 1)
{
Player->Stamina -= 2;
cout << " charges: ";
R = xd6(1, isShowDices);
if (Player->RDP.amount > 0)
{
while (c != 'n'&&Player->RDP.amount > 0)
{
if (!isAI)
{
cout << " reroll(y/n," << Player->RDP.amount << "rt)?" << endl;
cin >> c;
}
else
{
if (Player->dna.Rerollprob(Player->RDP.amount))
{
c = 'y';
}
}
if (c != 'n')
{
if (Player->RDP.Acquire())
{
int F = xd6(1);
if (Player->myPerk == Tactitian)
{
textcolor(Green);
R = max(R, F);
cout << " tactical desicion: " << R << endl;
}
}
else if (Player->myPerk != Fiechter && Player->myPerk != Dancer)
{
isRaged = true;
textcolor(Magenta);
GetBrokeText();
Player->SubDicePool--;
textcolor(White);
}
else
{
textcolor(Green);
cout << " attacker or dancer perks saves!" << endl;
}
textcolor(White);
}
}
}
}
if (!isRaged)
{
R = R + Player->Base.Att / 2 + (Player->myPerk == Dancer ? 1 : 0) + (Player->myPerk == Fiechter ? 1 : 0);
cout << " ->" << R << " vs " << Foe->Base.Def << endl;
if (R > Foe->Base.Def)
{
Foe->HP -= Player->Base.Att;
Foe->PushRt(1);
cout << endl << Player->name << " charges for " << Player->Base.Att << " and";
Foe->wounds.push_back(GetWoundText());
cout << " of " << Foe->name << ",  HP:" << Foe->HP << endl;

}
else if (R == Foe->Base.Def)
{
Foe->Base.BDY--;
Foe->PushRt(1);
Foe->HP -= Player->Base.Att * 2;
cout << endl << Player->name << " heavy charges for " << Player->Base.Att * 2 << " and";
Foe->wounds.push_back("heavy " + GetWoundText());
cout << " of " << Foe->name << ",  HP:" << Foe->HP << endl;
}
else
{
cout << endl << Player->name << " charges and";
GetBrokeText();
cout << endl;
}
}
break;
case 5:
cout << endl << " stands ground!" << endl;
Player->SubDicePool += Player->Stamina;
Player->Stamina = 0;
break;
case 6:
R = -1;
while (R == -1)
{
cout << "Inventory:" << endl;
for (int i = 0; i < Player->Inventory.size(); i++)
{
Item ii = Player->Inventory[i];
cout << i + 1 << ": " << ii.name << "[" << ii.BDY << ", " << ii.Att << ", " << ii.Def << ", " << ii.HP << ", " << ii.RC << ", " << ii.SubDice << "]" << endl;
}
cin >> R;
R--;
if (R >= 0 && R < Player->Inventory.size())
{
Item SR = Player->Inventory[R];
Player->Used = SR;
Player->Inventory[R] = Player->Inventory.back();
Player->Inventory.pop_back();
}
else
{
R = -1;
}
}
cout << "uses: " << Player->Used.name << endl;
R = xd6(1, isShowDices);
if (Player->RDP.amount > 0)
{
while (c != 'n'&&Player->RDP.amount > 0)
{
if (!isAI)
{
cout << " reroll(y/n," << Player->RDP.amount << "rt)?" << endl;
cin >> c;
}
else
{
if (Player->dna.Rerollprob(Player->RDP.amount))
{
c = 'y';
}
}
if (c != 'n')
{
if (Player->RDP.Acquire())
{
int F = xd6(1);
if (Player->myPerk == Tactitian)
{
textcolor(Green);
R = max(R, F);
cout << " tactical desicion: " << R << endl;
}
}
else if (Player->myPerk != Cold)
{
isRaged = true;
textcolor(Magenta);
GetBrokeText();
Player->Used.Use();
Player->SubDicePool--;
textcolor(White);
}
else
{
textcolor(Green);
cout << " cold perks saves!" << endl;
}
textcolor(White);

}
}
}
if (!isRaged)
{
R = R + (Player->myPerk == Cold ? 1 : 0);
cout << " ->" << R << " vs " << Player->RDP.amount - Player->Stamina << endl;
if (R > Player->RDP.amount - Player->Stamina)
{
Player->UseItem();
cout << endl << " succesful!" << endl;
}
else
{
cout << endl << Player->name << " opens bag and";
GetBrokeText();
cout << endl;
Player->Used.Use();
}

}
Player->Stamina = 0;
break;

/**/
int main()
{


	srand(time(NULL));
	vector<Item> Inventory;
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

	int FoeLevel = 4;// rand() % Enemies.size();
	Unit Foe = Enemies[FoeLevel];
	Foe.dna.dt = Attacker;// DNAType(xd6(1, false) - 1);
	Foe.myPerk = Fiechter;// Perk(rand() % 5);
	Unit Player;
	cout << unitbuf;
	cout << "Hello stranger, i am " << Foe.name << "[" << Foe.Base.BDY << "," << Foe.dna.dt << "," << Foe.myPerk << "], state your name!: ";
	cin >> Player.name;
	cout << "Tell me," << Player.name << " your level: ";
	int cr;
	cin >> cr;



	Player = Unit(Player.name, cr);

	cout << "What is your Perk?" << endl;
	cout << "0-None" << endl;
	cout << "1 - Cold (no ragetoken gain)" << endl;
	cout << "2 - Fiechter (no rage exploding in attacks)" << endl;
	cout << "3 - Dancer(no rage exploding in moves)" << endl;
	cout << "4 - Tactitian(best reroll dice)" << endl;
	int pprk = -1;
	while (pprk < 0 || pprk>4)
	{
		cout << "Enter 0 - 4:";
		cin >> pprk;
	}
	Player.myPerk = Perk(pprk);

	cout << Player.name << " you ows me money! Time to fight!" << endl;

	Player.Inventory.push_back(Item(0, 0, 0, "apple", 10, 0, 0));
	Player.Inventory.push_back(Item(0, 0, 0, "potion", 30, 0, 0));
	Player.Inventory.push_back(Item(0, 0, 0, "potion of ice cold", 0, -5, 0));
	Player.Inventory.push_back(Item(0, 0, 0, "potion of burn", 0, 2, 2));
	Player.Inventory.push_back(Item(0, -1, 1, "potion of fury", 0, 0, 0));

	Player.RDP.amount = 1;
	Foe.RDP.amount = 1;

	ShowStats(Player, Foe);
	system("PAUSE");

	while (Player.HP > 0 && Foe.HP > 0)
	{
		system("CLS");
		ShowStats(Player, Foe);
		Player.isRaged = false;
		Foe.isRaged = false;
		Player.Stamina = 0;
		Foe.Stamina = 0;
		cout << "Fighting for initiative:" << endl;

		int zP = Player.Base.BDY + Player.SubDicePool;
		int zF = Foe.Base.BDY + Foe.SubDicePool;
		if (Foe.RDP.amount >= 6)
		{
			cout << Foe.name << " RAGED!" << endl;
			Foe.RDP.amount = 0;
			zF = 0;
			zP = Player.Base.BDY / 3;
			Foe.isRaged = true;
		}
		if (Player.RDP.amount >= 6)
		{
			Player.isRaged = true;
			cout << Player.name << " RAGED!" << endl;
			Player.RDP.amount = 0;
			zP = 0;
			zF = Player.Base.BDY / 3;
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
			GetClashText();
			if (side == 1)
			{

				BDYCheck.MakeThrow(1, White);
				zP--;
			}
			else
			{
				BDYCheck.MakeThrow(1, DarkGray);
				zF--;
			}
			textcolor(0x0F);
			cout << "    ";
		}

		Player.SubDicePool = 0;
		Foe.SubDicePool = 0;

		BDYCheck.ShowPool();
		wait(300);
		if (Player.RDP.amount > 0)
		{
			if (Player.AskForReroll(-1, false, Player.myPerk != Cold, &BDYCheck) == 0)
			{
				Player.isRaged = true;
			}
		}
		if (Foe.RDP.amount > 0 && Foe.dna.Rerollprob(Foe.RDP.amount))
		{
			if (Foe.AskForReroll(-1, true, Foe.myPerk != Cold, &BDYCheck) == 0)
			{
				Foe.isRaged = true;
			}
		}
		wait(300);
		BDYCheck.ShowPool();
		ColoredDice Actions = BDYCheck.GetMaxColoredSeq();

		if (Actions.color == White)
		{
			Foe.Stamina = 0;
			Player.Stamina = Actions.NUM;
		}
		else {
			Player.Stamina = 0;
			Foe.Stamina = Actions.NUM;
		}
		cout << endl;

		system("PAUSE");
		system("CLS");
		if (Player.Stamina > 0)
		{
			ShowStats(Player, Foe);
			MakeActions(&Player, &Foe, false, true);

		}
		else if (Foe.Stamina > 0)
		{
			ShowStats(Player, Foe);
			MakeActions(&Foe, &Player, true, false);
		}
		else
		{
			cout << "Draw! Retreat! Tuche!" << endl;

			Player.PushRt(1);
			Foe.PushRt(1);

		}

		system("PAUSE");
	}


	cout << "Battle is over " << endl;
	system("PAUSE");

	return 0;
}

