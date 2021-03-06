// RidersOfFortune.cpp : Defines the entry point for the console application.
/*
* Peter Sauter
* 11/07/17
* COP2335 (Obejct Oriented Programming) Fall Term: 2017
* Professor: Alex Roque M.S.
* Assignment 2
*/

#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#undef max
using namespace std;

//Global Constant Variables:
const unsigned short int x = 9; // Horizontal Space defined by this constant (set as constant as it isn't supposed to change during game). Minimum functional parameters 5x5, any larger works OK
const unsigned short int y = 9; // Vertical Space defined by this constant.

//Global Variables:
unsigned short int map[x][y]; //currently set to 9x9 for symmetry (the one in the page shows a 8x8)
int playerMoveDice;
int weaponName;
bool newOrLoad; //True = New Game; False = Load;
bool does_file_exist(const char *fileName);
bool playerTurn = false;
bool dragonfight = false;
bool pickingSomethingUp = false;
bool singleDigitModifier = true;
bool equippingWeapon = false;

short int monster_CurHP;
short unsigned int monster_MaxHP;
short unsigned int monster_Vit;
short unsigned int monster_Str;
short unsigned int monster_End;
short unsigned int monster_Spd;
short int monster_processedDamage;

//To save the things with multiple properties in a vector. Save them as 0-5 (6 total), first number 'n' weapon type, second and third numbers 'NN' attack modifier(from 1-20), fourth and fifth 'NN' (durability)
//to load them simply read the line by char, rather than a line encompassing string.


//attempts at making sense of vectors and dynamic arrays:
vector<int> wCollect;
int *iCollect = nullptr;
int iCount;
int usedItemIndex = 0;
int pageN = 0;
int options[5];
int weaponType;

//attempt at enumeration:
enum itemName { Small_Potion = 9000, Medium_Potion = 9100, Elixir = 9200, Small_Explosive = 9300, Curse_Jar = 9400, Relic = 9500 };
//Function Declarations:
void startGame();
void defaultStats();
void chooseAllign();
void displayMap();
void mapGen();
void errorDialogue();
void rideOrExplore();
void Ride();
void Explore();
void playerHPCheck();
void Menu();
void checkCharacter();
void wellOfReflectionChoice();
void emptySpaceRNG();
void checkItemInventory();
void itemRNG();
void findEmptyInventorySlot();
void useItemSwitch();
void ExpandInv();
void displayInventory();
void flagInvalidPlace();
void inventorySortProcess();
void GenerateMonster();
void displayMonster();
void randMonsterVit();
void randMonsterStr();
void randMonsterEnd();
void randMonsterSpd();
void monsterattack();
void weaponModifierSpawnRNG();
void weaponTypeRNG();
void SelectionSort(vector<int> &vec);
int binSearch(vector<int> &vecT, int value);
void checkWeaponCollection();
void displayWeaponCollection();
void WeaponNameSwitch();
void weaponNameFinder(int index);
void battleMenu();
void shopDisplay();
void shopThing();

//player object and class
class Player {
private:
	const short unsigned int iSize = 5;

	struct playerStats
	{
		short unsigned int Px; //Player's X coordinate
		short unsigned int Py; //Player's Y coordinate
		short unsigned int Plvl; //Keeps track of how many times have you leveled up.
		int XP; //Works as both Currency and level UP requirement
		unsigned int requiredXP; // Formula for reuqiredXP = (2 * (Plvl + 1)) ^ 2;
		unsigned int MaxHP; //Formula for MaxHP = (10*Vit);
		int CurHP; //CurrentHP
		short unsigned int Strength; // Affects the Minimums and Maximums when calculating Damage
		short unsigned int Vitality; // Determines Max Health
		short unsigned int Speed; //Determines who takes the first turn in combat
		short unsigned int Endurance; //Substracts from enemy strength when calculating incoming damage
		short unsigned int AllignGood; //0 = Evil; 1 = Good; (used to be a bool but Load only brought back ints)
		short unsigned int moveHorizontal; //0 = Vertical; 1 = Horizontal; (used to be a bool but Load only brought back ints)
		short unsigned int moveIncrement; // 0 = Left or Up; 1 = Right or Down; (used to be a bool but Load only brought back ints)
		short unsigned int relicPicked;
		unsigned int iMaxInv;
		short unsigned int nOfExpansions;
		short unsigned int inBattle; //0 = False; 1 = True;
		short unsigned int pickedSomthingUp; //0 = False; 1 = True;
		int itemInventory[25];
	}p;


public:
	int itemindx = 0;
	short int Player_ProcessedDamage;
	short int weaponMod = 0;
	short int equipped = 0;
	//Load Game (currently only loads the Player's stats):
	void Load() {
		ifstream myfile;
		myfile.open("RoFsave.txt");
		string tmpLine;
		vector<playerStats> statTable;
		if (myfile.is_open()) {
			int counter = 0;
			int something;
			while (getline(myfile, tmpLine)) {
				istringstream inputLine(tmpLine);
				playerStats pStat;
				if (!(inputLine >> pStat.Plvl >> pStat.XP >> pStat.CurHP >> pStat.Vitality >> pStat.Strength >> pStat.Endurance >> pStat.Speed >> pStat.AllignGood >>
					pStat.Px >> pStat.Py >> pStat.moveHorizontal >> pStat.moveIncrement >> pStat.relicPicked >> pStat.inBattle >> pStat.pickedSomthingUp) >> pStat.itemInventory[0]
					>> pStat.itemInventory[1] >> pStat.itemInventory[2] >> pStat.itemInventory[3] >> pStat.itemInventory[4] >> pStat.itemInventory[5] >> pStat.itemInventory[6] >> pStat.itemInventory[7] >> pStat.itemInventory[8]
					>> pStat.itemInventory[9] >> pStat.itemInventory[10] >> pStat.itemInventory[11] >> pStat.itemInventory[12] >> pStat.itemInventory[13] >> pStat.itemInventory[14] >> pStat.itemInventory[15] >> pStat.itemInventory[16]
					>> pStat.itemInventory[17] >> pStat.itemInventory[18] >> pStat.itemInventory[19] >> pStat.itemInventory[20] >> pStat.itemInventory[21] >> pStat.itemInventory[22] >> pStat.itemInventory[23] >> pStat.itemInventory[24]) {
					something = stoi(tmpLine);
					switch (counter) {
					case 0: {
						P1.set_PlayerLevel(something);
						break;
					}
					case 1: {
						P1.set_PlayerRequiredXP((2 * (P1.get_PlayerLevel() + 1))); //you can add a ^2 at the end of it just to make it progressively harder
						P1.set_PlayerXP(something);
						break;
					}
					case 2: {
						P1.set_PlayerCurHP(something);
						break;
					}
					case 3: {
						P1.set_PlayerVitality(something);
						break;
					}
					case 4: {
						P1.set_PlayerMaxHP(10 * P1.get_PlayerVitality());
						P1.set_PlayerStrength(something);
						break;
					}
					case 5: {
						P1.set_PlayerEndurance(something);
						break;
					}
					case 6: {
						P1.set_PlayerSpeed(something);
						break;
					}
					case 7: {
						P1.set_PlayerAllignment(something);
						break;
					}
					case 8: {
						P1.set_PlayerX(something);
						break;
					}
					case 9: {
						P1.set_PlayerY(something);
						break;
					}
					case 10: {
						P1.set_HorizMove(something);
						break;
					}
					case 11: {
						P1.set_IncreMove(something);
						break;
					}
					case 12: {
						map[P1.get_PlayerX()][P1.get_PlayerY()] = map[P1.get_PlayerX()][P1.get_PlayerY()] + 1;
						P1.set_relicPick(something);
						break;
					}
					case 13: {
						P1.set_iMaxInv(iSize*(P1.get_nOfExpand() + 1));
						P1.set_inBattle(something);
						break;
					}
					case 14: {
						P1.set_pickedSomethingUp(something);
						break;
					}
					case 15: {
						if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
							iCollect[itemindx] = something;
							itemindx++;
						}
						break;
					}
					case 16: {
						if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
							iCollect[itemindx] = something;
							itemindx++;
						}
						break;
					}
					case 17: {
						if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
							iCollect[itemindx] = something;
							itemindx++;
						}
						break;
					}
					case 18: {
						if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar)) {
							iCollect[itemindx] = something;
							itemindx++;
						}
						break;
					}
					case 19: {
						if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
							iCollect[itemindx] = something;
							itemindx++;
						}
						break;
					}
					case 20: {
						if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
							iCollect[itemindx] = something;
							itemindx++;
						}
						break;
					}
					case 21: {
						if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
							iCollect[itemindx] = something;
							itemindx++;
						}
						break;
					}
					case 22: {
						if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
							iCollect[itemindx] = something;
							itemindx++;
						}
						break;
					}
					case 23: {
						if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
							iCollect[itemindx] = something;
							itemindx++;
						}
						break;
					}
					case 24: {
						if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
							iCollect[itemindx] = something;
							itemindx++;
						}
						break;
					}
					case 25: {
						if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
							iCollect[itemindx] = something;
							itemindx++;
						}
						break;
					}
					case 26: {
						if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
							iCollect[itemindx] = something;
							itemindx++;
						}
						break;
					}
					case 27: {
						if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
							iCollect[itemindx] = something;
							itemindx++;
						}
						break;
					}
					case 28: {
						if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
							iCollect[itemindx] = something;
							itemindx++;
						}
						break;
					}
					case 29: {
						if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
							iCollect[itemindx] = something;
							itemindx++;
						}
						break;
					}
					case 30: {
						if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
							iCollect[itemindx] = something;
							itemindx++;
						}
						break;
					}
					case 31: {
						if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
							iCollect[itemindx] = something;
							itemindx++;
						}
						break;
					}
					case 32: {
						if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
							iCollect[itemindx] = something;
							itemindx++;
						}
						break;
					}
					case 33: {
						if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
							iCollect[itemindx] = something;
							itemindx++;
						}
						break;
					}
					case 34: {
						if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
							iCollect[itemindx] = something;
							itemindx++;
						}
						break;
					}
					case 35: {
						if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
							iCollect[itemindx] = something;
							itemindx++;
						}
						break;
					}
					case 36: {
						if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
							iCollect[itemindx] = something;
							itemindx++;
						}
						break;
					}
					case 37: {
						if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
							iCollect[itemindx] = something;
							itemindx++;
						}
						break;
					}
					case 38: {
						if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
							iCollect[itemindx] = something;
							itemindx++;
						}
						break;
					}
					case 39: {
						if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
							iCollect[itemindx] = something;
							itemindx++;
						}
						break;
					}
					default: {
						std::cout << "WTF!" << endl;
					}
					}
					counter++;
					continue;
				}
				something = stoi(tmpLine);
				switch (counter) {
				case 0: {
					P1.set_PlayerLevel(something);
					break;
				}
				case 1: {
					P1.set_PlayerRequiredXP((2 * (P1.get_PlayerLevel() + 1))); //you can add a ^2 at the end of it just to make it progressively harder
					P1.set_PlayerXP(something);
					break;
				}
				case 2: {
					P1.set_PlayerCurHP(something);
					break;
				}
				case 3: {
					P1.set_PlayerVitality(something);
					break;
				}
				case 4: {
					P1.set_PlayerMaxHP(10 * P1.get_PlayerVitality());
					P1.set_PlayerStrength(something);
					break;
				}
				case 5: {
					P1.set_PlayerEndurance(something);
					break;
				}
				case 6: {
					P1.set_PlayerSpeed(something);
					break;
				}
				case 7: {
					P1.set_PlayerAllignment(something);
					break;
				}
				case 8: {
					P1.set_PlayerX(something);
					break;
				}
				case 9: {
					P1.set_PlayerY(something);
					break;
				}
				case 10: {
					P1.set_HorizMove(something);
					break;
				}
				case 11: {
					P1.set_IncreMove(something);
					break;
				}
				case 12: {
					map[P1.get_PlayerX()][P1.get_PlayerY()] = map[P1.get_PlayerX()][P1.get_PlayerY()] + 1;
					P1.set_relicPick(something);
					break;
				}
				case 13: {
					P1.set_iMaxInv(iSize*(P1.get_nOfExpand() + 1));
					P1.set_inBattle(something);
					break;
				}
				case 14: {
					P1.set_pickedSomethingUp(something);
					break;
				}
				case 15: {
					if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
						iCollect[itemindx] = something;
						itemindx++;
					}
					break;
				}
				case 16: {
					if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
						iCollect[itemindx] = something;
						itemindx++;
					}
					break;
				}
				case 17: {
					if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
						iCollect[itemindx] = something;
						itemindx++;
					}
					break;
				}
				case 18: {
					if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar)) {
						iCollect[itemindx] = something;
						itemindx++;
					}
					break;
				}
				case 19: {
					if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
						iCollect[itemindx] = something;
						itemindx++;
					}
					break;
				}
				case 20: {
					if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
						iCollect[itemindx] = something;
						itemindx++;
					}
					break;
				}
				case 21: {
					if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
						iCollect[itemindx] = something;
						itemindx++;
					}
					break;
				}
				case 22: {
					if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
						iCollect[itemindx] = something;
						itemindx++;
					}
					break;
				}
				case 23: {
					if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
						iCollect[itemindx] = something;
						itemindx++;
					}
					break;
				}
				case 24: {
					if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
						iCollect[itemindx] = something;
						itemindx++;
					}
					break;
				}
				case 25: {
					if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
						iCollect[itemindx] = something;
						itemindx++;
					}
					break;
				}
				case 26: {
					if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
						iCollect[itemindx] = something;
						itemindx++;
					}
					break;
				}
				case 27: {
					if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
						iCollect[itemindx] = something;
						itemindx++;
					}
					break;
				}
				case 28: {
					if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
						iCollect[itemindx] = something;
						itemindx++;
					}
					break;
				}
				case 29: {
					if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
						iCollect[itemindx] = something;
						itemindx++;
					}
					break;
				}
				case 30: {
					if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
						iCollect[itemindx] = something;
						itemindx++;
					}
					break;
				}
				case 31: {
					if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
						iCollect[itemindx] = something;
						itemindx++;
					}
					break;
				}
				case 32: {
					if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
						iCollect[itemindx] = something;
						itemindx++;
					}
					break;
				}
				case 33: {
					if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
						iCollect[itemindx] = something;
						itemindx++;
					}
					break;
				}
				case 34: {
					if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
						iCollect[itemindx] = something;
						itemindx++;
					}
					break;
				}
				case 35: {
					if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
						iCollect[itemindx] = something;
						itemindx++;
					}
					break;
				}
				case 36: {
					if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
						iCollect[itemindx] = something;
						itemindx++;
					}
					break;
				}
				case 37: {
					if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
						iCollect[itemindx] = something;
						itemindx++;
					}
					break;
				}
				case 38: {
					if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
						iCollect[itemindx] = something;
						itemindx++;
					}
					break;
				}
				case 39: {
					if ((something == Small_Explosive) || (something == Small_Potion) || (something == Medium_Potion) || (something == Elixir) || (something == Curse_Jar) || (something == Relic)) {
						iCollect[itemindx] = something;
						itemindx++;
					}
					break;
				}
				default: {
					if (counter > 39) {
						wCollect.push_back(something);
					}
					else {
						std::cout << "wtf";
					}
				}
				}
				counter++;
				statTable.push_back(pStat);
			}
		}
		myfile.close();
	}

	//Save Game (currently only saves the Player's stats, except for MaxHP and requiredXP, and maxInventory):
	void Save() {
		ofstream myfile;
		myfile.open("RoFsave.txt");
		if (myfile.is_open()) {
			myfile << p.Plvl << "\n";
			myfile << p.XP << "\n";
			myfile << p.CurHP << "\n";
			myfile << p.Vitality << "\n";
			myfile << p.Strength << "\n";
			myfile << p.Endurance << "\n";
			myfile << p.Speed << "\n";
			myfile << p.AllignGood << "\n";
			myfile << p.Px << "\n";
			myfile << p.Py << "\n";
			myfile << p.moveHorizontal << "\n";
			myfile << p.moveIncrement << "\n";
			myfile << p.relicPicked << "\n";
			//myfile << p.nOfExpansions << "\n";
			myfile << p.inBattle << "\n";
			myfile << p.pickedSomthingUp << "\n";
			for (int q = 0; q < 25; q++) {
				myfile << p.itemInventory[q] << "\n";
			}
			for (int q = 0; q < wCollect.size(); q++) {
				myfile << wCollect[q] << "\n";
			}

			myfile.close();
		}
	}

	//Sets:{
	void set_PlayerX(short unsigned int PlayerX) {
		p.Px = PlayerX;
	}
	void set_PlayerY(short unsigned int PlayerY) {
		p.Py = PlayerY;
	}
	void set_HorizMove(short unsigned int HorizMovement) {
		p.moveHorizontal = HorizMovement;
	}
	void set_IncreMove(short unsigned int IncreMove) {
		p.moveIncrement = IncreMove;
	}
	void set_relicPick(short unsigned int rPick) {
		p.relicPicked = rPick;
	}
	void set_PlayerLevel(short unsigned int PlayerLevel) {
		p.Plvl = PlayerLevel;
	}
	void set_PlayerXP(int PlayerXP) {
		p.XP = PlayerXP;
	}
	void set_PlayerRequiredXP(unsigned int PlayerRequiredXP) {
		p.requiredXP = PlayerRequiredXP;
	}
	void set_PlayerMaxHP(int PlayerMaxHP) {
		p.MaxHP = PlayerMaxHP;
	}
	void set_PlayerCurHP(int PlayerCurHP) {
		p.CurHP = PlayerCurHP;
	}
	void set_PlayerStrength(short unsigned int PlayerStrength) {
		p.Strength = PlayerStrength;
	}
	void set_PlayerSpeed(short unsigned int PlayerSpeed) {
		p.Speed = PlayerSpeed;
	}
	void set_PlayerVitality(short unsigned int PlayerVitality) {
		p.Vitality = PlayerVitality;
	}
	void set_PlayerEndurance(short unsigned int PlayerEndurance) {
		p.Endurance = PlayerEndurance;
	}
	void set_PlayerAllignment(short unsigned int PlayerAllignment) {
		p.AllignGood = PlayerAllignment;
	}
	void set_iMaxInv(unsigned int MaxInventorySize) {
		p.iMaxInv = MaxInventorySize;
	}
	void set_nOfExpand(unsigned int NumberExpo) {
		p.nOfExpansions = NumberExpo;
	}
	void set_inBattle(short unsigned int Battle) {
		p.inBattle = Battle;
	}
	void set_pickedSomethingUp(short unsigned int pickUp) {
		p.pickedSomthingUp = pickUp;
	}
	//Gets:
	short unsigned int get_PlayerX() {
		return p.Px;
	}
	short unsigned int get_PlayerY() {
		return p.Py;
	}
	short unsigned int get_HorizMove() {
		return p.moveHorizontal;
	}
	short unsigned int get_IncreMove() {
		return p.moveIncrement;
	}
	short unsigned int get_PlayerAllign() {
		return p.AllignGood;
	}
	short unsigned int get_relicPick() {
		return p.relicPicked;
	}
	short unsigned int get_PlayerLevel() {
		return p.Plvl;
	}
	int get_PlayerXP() {
		return p.XP;
	}
	unsigned int get_PlayerRequiredXP() {
		return p.requiredXP;
	}
	int get_PlayerMaxHP() {
		return p.MaxHP;
	}
	int get_PlayerCurHP() {
		return p.CurHP;
	}
	short unsigned int get_PlayerStrength() {
		return p.Strength;
	}
	short unsigned int get_PlayerSpeed() {
		return p.Speed;
	}
	short unsigned int get_PlayerVitality() {
		return p.Vitality;
	}
	short unsigned int get_PlayerEndurance() {
		return p.Endurance;
	}
	short unsigned int get_iSize() {
		return iSize;
	}
	unsigned int get_iMaxInv() {
		return p.iMaxInv;
	}
	unsigned int get_nOfExpand() {
		return p.nOfExpansions;
	}
	short unsigned int get_inBattle() {
		return p.inBattle;
	}
	short unsigned int get_pickedSomethingUp() {
		return p.pickedSomthingUp;
	}

	//Misc Functions:
	//used for saving data into the array that eventually gets written into the text file, THIS IS NOT the function that writes the data into the text file
	void inventorySaveFileProcess() {
		short int wht = 0;
		for (int u = 0; u < P1.get_iMaxInv(); u++) {
			if ((iCollect[u] == Small_Explosive) || (iCollect[u] == Small_Potion) || (iCollect[u] == Medium_Potion) || (iCollect[u] == Elixir) || (iCollect[u] == Curse_Jar) || (iCollect[u] == Relic)) {
				p.itemInventory[wht] = iCollect[u];
				wht++;
			}
		}
	}

	void displayStats() {
		std::cout << "Yarra: \n";
		std::cout << "Level: " << p.Plvl << endl;
		std::cout << "XP: " << p.XP << endl;
		std::cout << "HP: " << p.CurHP << " / " << p.MaxHP << endl;
		std::cout << "Vitality: " << p.Vitality << "						*Vitality defines your Maximum HP" << endl;
		std::cout << "Strength: " << p.Strength << "						*Strength defines how much damage you can deal" << endl;
		std::cout << "Endurance: " << p.Endurance << "			*Endurance reduces the amount of HP lost when hit by an enemy" << endl;
		std::cout << "Speed: " << p.Speed << "							*Speed determines who acts first in combat" << endl;
		if (p.AllignGood == 1) {
			std::cout << "Allignment: Good" << endl;
		}
		else if (p.AllignGood == 0) {
			std::cout << "Allignment: Evil" << endl;
		}
		/*
		std::cout << "Location:\n x: " << p.Px << ", y: " << p.Py << endl;
		
		std::cout << "Increment?: ";
		if (p.moveIncrement == 1) {
		std::cout << "T" << endl;
		}
		else if (p.moveIncrement == 0) {
		std::cout << "F" << endl;
		}
		std::cout << "Horizontal?: ";
		if (p.moveHorizontal == 1) {
		std::cout << "T" << endl;
		}
		else if (p.moveHorizontal == 0) {
		std::cout << "F" << endl;
		}
		
		std::cout << "Relic?: ";
		if (p.relicPicked == 1) {
			std::cout << "T" << endl;
		}
		else if (p.relicPicked == 0) {
			std::cout << "F" << endl;
		}
		*/
	}

	void turnOrder() {
		if (P1.get_PlayerSpeed() < monster_Spd) {
			playerTurn = false;
		}
		else if (P1.get_PlayerSpeed() > monster_Spd) {
			playerTurn = true;
		}
		else if (P1.get_PlayerSpeed() == monster_Spd) {
			short int iturn = 0;
			iturn = rand() % 4 + 1;
			if (iturn <= 2) {
				playerTurn = false;
			}
			else if (iturn > 2) {
				playerTurn = true;
			}
		}
		if (!playerTurn) {
			system("cls");
			std::cout << "The monster outspeeds you!" << endl;
			Sleep(750);
			monsterattack();
		}
	}

	void battleWin() {
		if ((!dragonfight) && (monster_CurHP <= 0)) {
			int tempRand = rand() % (10 * P1.get_PlayerLevel()) + (3 * P1.get_PlayerLevel());
			std::cout << "You defeated the Monster!\nYou gain: " << tempRand << "XP" << endl;
			P1.set_PlayerXP(P1.get_PlayerXP() + tempRand);
			P1.set_inBattle(0);
			Sleep(2000);
			system("cls");
			Ride();
		}
		else if ((dragonfight) && (monster_CurHP <= 0)) {
			std::cout << "CONGLATURATIONS YOUR THE WINNER";
			Sleep(5000);
			if (remove("RoFsave.txt") != 0) {
				perror("Error deleting file");
				std::cout << "poop"; //poop
			}
			exit(0);
		}
	}

	void battle() {
		displayMonster();
		std::cout << "\nPlayer: \nPlayer HP: " << P1.get_PlayerCurHP() << " / " << P1.get_PlayerMaxHP() << endl;
		std::cout << "Weapon Equipped: ";
		if ((wCollect.size() > 0) && ((P1.equipped >-1) && (P1.equipped <= wCollect.size()))) {
			weaponNameFinder(P1.equipped);
		}
		else {
			std::cout << "Knife" << endl;
		}
		std::cout << "Strength: " << P1.get_PlayerStrength() << "		 Endurance: " << P1.get_PlayerEndurance() << "			Speed: " << P1.get_PlayerSpeed() << endl;
		std::cout << "===================================================================================================\n";
		std::cout << "\nWhat will you do?\n";
		std::cout << "1:Fight		2:Inventory			3:Run\n" << endl;
		unsigned short int userInput;
		cin >> userInput;
		while (((userInput != 1) || (userInput != 2) || (userInput != 3))) {
			if (cin.fail()) {
				Sleep(150);
				system("cls");
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				errorDialogue();
				displayMonster();
				std::cout << "Player: \nPlayer HP: " << P1.get_PlayerCurHP() << " / " << P1.get_PlayerMaxHP() << endl;
				std::cout << "Weapon Equipped: ";
				if ((wCollect.size() > 0) && ((P1.equipped >-1) && (P1.equipped <= wCollect.size()))) {
					weaponNameFinder(P1.equipped);
				}
				else {
					std::cout << "Knife" << endl;
				}
				std::cout << "Strength: " << P1.get_PlayerStrength() << "		 Endurance: " << P1.get_PlayerEndurance() << "			Speed: " << P1.get_PlayerSpeed() << endl;
				std::cout << "\nWhat will you do?\n";
				std::cout << "1:Fight		2:Inventory			3:Run";
				cin >> userInput;
			}
			else if (userInput == 1) {
				system("cls");
				turnOrder();
				playerHPCheck();
				Fight();
				if (playerTurn) {
					monsterattack();
					playerHPCheck();
				}
				if (monster_CurHP > 0) {
					battle();
				}
			}
			else if (userInput == 2) {
				system("cls");
				battleMenu();
			}
			else if (userInput == 3) {
				if (!dragonfight) {
					system("cls");
					turnOrder();
					playerHPCheck();
					P1.set_inBattle(0);
					Flee();
					Ride();
				}
				else {
					std::cout << "The cave is magically sealed preventing any form of escape" << endl;
					Sleep(1500);
					system("cls");
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					battle();
				}

			}
			else if ((userInput > 3) || (userInput < 1)) {
				Sleep(150);
				system("cls");
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				errorDialogue();
				displayMonster();
				std::cout << "Player: \nPlayer HP: " << P1.get_PlayerCurHP() << " / " << P1.get_PlayerMaxHP() << endl;
				std::cout << "Weapon Equipped: ";
				if ((wCollect.size() > 0) && ((P1.equipped >-1) && (P1.equipped <= wCollect.size()))) {
					weaponNameFinder(P1.equipped);
				}
				else {
					std::cout << "Knife" << endl;
				}
				std::cout << "Strength: " << P1.get_PlayerStrength() << "		 Endurance: " << P1.get_PlayerEndurance() << "			Speed: " << P1.get_PlayerSpeed() << endl;
				std::cout << "\nWhat will you do?\n";
				std::cout << "1:Fight		2:Inventory			3:Run";
				cin >> userInput;
			}
		}
	}

	void Fight() {
		short unsigned int RNG;
		RNG = rand() % 100 + 1;
		if (RNG == 1) {
			Player_ProcessedDamage = 0;
			std::cout << "Your attack misses the Monster";
		}
		else if ((RNG > 1) && (RNG <= 10)) {
			Player_ProcessedDamage = ((((P1.get_PlayerStrength()*0.6) + weaponMod) - (monster_End)));
			std::cout << "Your attack barely hits the Monster";
		}
		else if ((RNG > 10) && (RNG <= 20)) {
			Player_ProcessedDamage = ((((P1.get_PlayerStrength()*0.7) + weaponMod) - (monster_End)));
			std::cout << "You land a grazing hit on the Monster";
		}
		else if ((RNG > 20) && (RNG <= 30)) {
			Player_ProcessedDamage = ((((P1.get_PlayerStrength()*0.8) + weaponMod) - (monster_End)));
			std::cout << "You land a weak hit on the Monster";
		}
		else if ((RNG > 30) && (RNG <= 40)) {
			Player_ProcessedDamage = ((((P1.get_PlayerStrength()*0.9) + weaponMod) - (monster_End)));
			std::cout << "You land a hit on the Monster";
		}
		else if ((RNG > 40) && (RNG <= 50)) {
			Player_ProcessedDamage = (((P1.get_PlayerStrength() + weaponMod) - (monster_End)));
			std::cout << "You attack the Monster";
		}
		else if ((RNG > 50) && (RNG <= 60)) {
			Player_ProcessedDamage = ((((P1.get_PlayerStrength()*1.1) + weaponMod) - ((monster_End)*0.95)));
			std::cout << "You skillfully attack the Monster";
		}
		else if ((RNG > 60) && (RNG <= 70)) {
			Player_ProcessedDamage = ((((P1.get_PlayerStrength()*1.2) + weaponMod) - ((monster_End)*0.9)));
			std::cout << "You land a solid hit on the Monster";
		}
		else if ((RNG > 70) && (RNG <= 80)) {
			Player_ProcessedDamage = ((((P1.get_PlayerStrength()*1.3) + weaponMod) - ((monster_End)*0.85)));
			std::cout << "You land a strong attack on the Monster";
		}
		else if ((RNG > 80) && (RNG <= 99)) {
			Player_ProcessedDamage = ((((P1.get_PlayerStrength()*1.4) + weaponMod) - ((monster_End)*0.8)));
			std::cout << "You land a devastating attack on the Monster";
		}
		else if ((RNG > 99) && (RNG <= 100)) {
			Player_ProcessedDamage = ((((P1.get_PlayerStrength()*1.5) + weaponMod) - ((monster_End)*0.75)) * 2);
			std::cout << "You land a devastating critical hit on the Monster";
		}
		if (Player_ProcessedDamage <= 0) {
			Player_ProcessedDamage = 0;
		}
		monster_CurHP = monster_CurHP - P1.Player_ProcessedDamage;
		std::cout << " dealing:\n" << P1.Player_ProcessedDamage << " damage points! " << endl;
		Sleep(1500);
		system("cls");
		battleWin();
	}

	void Flee() {
		int tempNum = 0;
		int tempRand = 0;
		int emptySpot = 0;
		for (int i = 0; i < P1.get_iMaxInv(); i++) {
			if ((iCollect[i] == Small_Explosive) || (iCollect[i] == Small_Potion) || (iCollect[i] == Medium_Potion) || (iCollect[i] == Elixir) || (iCollect[i] == Curse_Jar) || (iCollect[i] == Relic)) {
				tempNum++;
			}
		}
		if (P1.get_pickedSomethingUp() == 1) {
			if (tempNum <= 1) {
				tempRand = 0;
				P1.set_pickedSomethingUp(0);
			}
			else {
				tempRand = rand() % tempNum;
			}
			std::cout << "You run away like a coward\n You dropped ";

			switch (iCollect[tempRand]) {
			case Small_Potion: {
				std::cout << "a 'Small Potion'";
				break;
			}
			case Small_Explosive: {
				std::cout << "a 'Small Explosive'";
				break;
			}
			case Medium_Potion: {
				std::cout << "a 'Medium Potion'";
				break;
			}
			case Elixir: {
				std::cout << "an 'Elixir'";
				break;
			}
			case Curse_Jar: {
				std::cout << "a 'Curse Jar'";
				break;
			}
			case Relic: {
				std::cout << "the 'Relic'";
				P1.set_relicPick(0);
				break;
			}
			}
			std::cout << " while escaping from the monster\n";
			iCollect[tempRand] = 0;
			flagInvalidPlace();
			Sleep(2000);
			system("cls");
			pageN = 0;
		}
		else {
			std::cout << "You run away like a coward";
			Sleep(2000);
			system("cls");
		}
	}
}P1;

//The game keeps looping in and out of the rideOrExplore() function so the 'main' is merely there to initiate things
int main()
{
	Player P1;

	srand(time(NULL));
	std::cout << "Welcome to Riders of Fortune 2.0!\n" << endl;
	std::cout << "1: New Game\n2: Load Game\n" << endl;
	mapGen();
	startGame();
	if (newOrLoad) {
		system("cls");
		defaultStats();
		iCollect = new (nothrow) int[P1.get_iSize()];
		if (iCollect == nullptr) {
			std::cout << "oh so this is what's ruining my fun!";
		}
		else {
			for (iCount = 0; iCount < P1.get_iSize(); iCount++) {
				*(iCollect + iCount) = 0;
			}
			iCount = 0;
		}
		std::cout << "What are you? \n1: Good\n2: Evil\n" << endl;
		chooseAllign();
		displayMap();
		std::cout << "\nWhat will you do? \n" << endl;
		std::cout << "1:Ride horse and travel" << endl;
		std::cout << "2:Dismount horse and carefully explore the surroundings" << endl;
		std::cout << "3:'Menu'" << endl;
		rideOrExplore();
	}
	else {
		if (does_file_exist("RoFsave.txt")) {
			iCollect = new (nothrow) int[P1.get_iSize() * 5];
			P1.Load();
			flagInvalidPlace();
			inventorySortProcess();
			flagInvalidPlace();
			displayMap();
			std::cout << "\nWhat will you do? \n" << endl;
			std::cout << "1:Ride horse and travel" << endl;
			std::cout << "2:Dismount horse and carefully explore the surroundings" << endl;
			std::cout << "3:'Menu'" << endl;
			rideOrExplore();
		}
		else {
			std::cout << "File not found... \nProceeding with 'new game'" << endl;
			Sleep(2000);
			system("cls");
			defaultStats();
			iCollect = new (nothrow) int[P1.get_iSize()];
			if (iCollect == nullptr) {
				std::cout << "oh so this is what's ruining my fun!";
			}
			else {
				for (iCount = 0; iCount < P1.get_iSize(); iCount++) {
					*(iCollect + iCount) = 0;
				}
				iCount = 0;
			}
			std::cout << "What are you? \n1: Good\n2: Evil\n" << endl;
			chooseAllign();
			displayMap();
			std::cout << "\nWhat will you do? \n" << endl;
			std::cout << "1:Ride horse and travel" << endl;
			std::cout << "2:Dismount horse and carefully explore the surroundings" << endl;
			std::cout << "3:'Menu'" << endl;
			rideOrExplore();
		}

	}
	return 0;
}

//defines and sets the values for the player's stats for 'new game':
void defaultStats() {
	P1.set_PlayerLevel(1); //default value 1
	P1.set_PlayerXP(0); //default value 0
	P1.set_PlayerRequiredXP((2 * (P1.get_PlayerLevel() + 1))); //you can add a ^2 to make it progressively harder
	P1.set_PlayerVitality(10); //default value 10
	P1.set_PlayerStrength(10); //default value 14
	P1.set_PlayerEndurance(5); //default value 6
	P1.set_PlayerSpeed(10); //default value 10
	P1.set_PlayerMaxHP(P1.get_PlayerVitality() * 10);
	P1.set_PlayerCurHP(P1.get_PlayerMaxHP());
	P1.set_PlayerX(0);
	P1.set_PlayerY(0); //default value 0
	P1.set_HorizMove(0); //default value 1
	P1.set_IncreMove(0); //dafault value 1
	P1.set_relicPick(0); //default value 0
	P1.set_nOfExpand(0);
	P1.set_iMaxInv(P1.get_iSize());
	P1.set_inBattle(0);
	P1.set_pickedSomethingUp(0);
	map[P1.get_PlayerX()][P1.get_PlayerY()] = map[P1.get_PlayerX()][P1.get_PlayerY()] + 1;
	return;
}

//allows the player to either start new game or load a previous one
void startGame() {
	short int userInput;
	cin >> userInput;
	while ((userInput != 1) || (userInput != 2)) {
		if (cin.fail()) {
			Sleep(150);
			system("cls"); //clears log
			cin.clear(); // clears error flags
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			errorDialogue();
			std::cout << "What do you wish to do? \n1: New Game \n2: Load Game" << endl;
			cin >> userInput;
		}
		else if (userInput == 1) {
			newOrLoad = true;
			cin.clear(); // clears error flags
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			system("cls");
			return;
		}
		else if (userInput == 2) {
			newOrLoad = false;
			cin.clear(); // clears error flags
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			system("cls");
			return;
		}
		else if ((userInput > 2) || (userInput < 1)) {
			Sleep(150);
			system("cls"); //clears log
			cin.clear(); // clears error flags
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			errorDialogue();
			std::cout << "What do you wish to do? \n1: New Game \n2: Load Game" << endl;
			cin >> userInput;
		}
	}
}

//allows the player to choose their allignment at the beginning of the game
void chooseAllign() {
	short int userInput;
	cin >> userInput;
	while ((userInput != 1) || (userInput != 2)) {
		if (cin.fail()) {
			Sleep(150);
			system("cls");
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			errorDialogue();
			std::cout << "What are you? \n1: Good \n2: Evil" << endl;
			cin >> userInput;
		}
		else if (userInput == 1) {
			P1.set_PlayerAllignment(1);
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			system("cls");
			return;
		}
		else if (userInput == 2) {
			P1.set_PlayerAllignment(0);
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			system("cls");
			return;
		}
		else if ((userInput > 2) || (userInput < 1)) {
			Sleep(150);
			system("cls");
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			errorDialogue();
			std::cout << "What are you? \n1: Good \n2: Evil" << endl;
			cin >> userInput;
		}
	}
}

//generates the code for the map fixtures to be put in place and the spacial distribution
void mapGen() {
	//definitions of the fixtures in the map
	map[0][0] = 12; //Starting Point (Upper Left Corner)
	map[(x - 1) / 2][0] = 8; //Well of Reflection (Center Up)
	map[x - 1][0] = 4; //Cathedral (Upper Right Corner)
	map[0][(y - 1) / 2] = 10; //Dragon Entrance (Middle Left)
	map[(x - 1) / 2][(y - 1) / 2] = 14; //Dragon Cave (Center Middle)
	map[x - 1][(y - 1) / 2] = 6; //Graveyard (Middle Right)
	map[0][y - 1] = 6; //Graveyard (Lower Left Corner)
	map[(x - 1) / 2][y - 1] = 8; // Well of Reflection (Center Down)
	map[x - 1][y - 1] = 4; // Cathedral (Lower Right Corner)

						   //set as 4 different for loops to allow for flexibility in X or Y cooridnates.

						   //X First half of top and bottom row
	for (short int i = 1; ((i < ((x - 1) / 2)) && (i > 0)); i++) {
		map[i][0] = 2;
		map[i][y - 1] = 2;
	}

	//X Second half of top and bottom row
	for (short int i = (1 + ((x - 1) / 2)); ((i < (x - 1)) && (i >((x - 1) / 2))); i++) {
		map[i][0] = 2;
		map[i][y - 1] = 2;
	}

	//Y First half of the left and right collumns
	for (short int i = 1; ((i < ((y - 1) / 2)) && (i > 0)); i++) {
		map[0][i] = 2;
		map[x - 1][i] = 2;
	}

	//Y Second half of the Left and Right collumns
	for (short int i = (1 + ((y - 1) / 2)); ((i < (y - 1)) && (i >((y - 1) / 2))); i++) {
		map[0][i] = 2;
		map[x - 1][i] = 2;
	}

}

//generates and displays the visuals for the map
void displayMap() {
	/*
	P = +1 to current location (player is located using odd numbers)
	' ' = nothing. Space value of 0. This space is unavailable even for movement.
	* = Monster/Item/empty space, space value of 2 (monster/item/empty determined at random)
	C = Cathedral, space value of 4
	G = Graveyard, Space value of 6
	W = Well of reflection, space value 8
	E = Dragon Entrance, space value 10
	S = start point (not an empty space), space value 12
	D = Dragon cave (an inescapable spot in the center of the map) = 14
	*/

	//Nested 'for' loops to generate the actual visuals of the map.
	for (short int V = 0; V <= (y - 1); V++) {
		for (short int H = 0; H <= (x - 1); H++) {
			switch (map[H][V]) {
			case 0: {
				std::cout << "  ";
				break;
			}
			case 2: {
				std::cout << "* ";
				break;
			}
			case 4: {
				std::cout << "C ";
				break;
			}
			case 6: {
				std::cout << "G ";
				break;
			}
			case 8: {
				std::cout << "W ";
				break;
			}
			case 10: {
				std::cout << "E ";
				break;
			}
			case 12: {
				std::cout << "S ";
				break;
			}
			case 14: {
				std::cout << "D ";
				break;
			}
			default:
				std::cout << "P ";
				P1.set_PlayerX(H);
				P1.set_PlayerY(V);
			}
		}
		std::cout << "\n";
	}
}

//essentially allows the game to happen. Ride(), Explore(), and Menu() are accessed through here
void rideOrExplore() {
	unsigned short int userInput;
	cin >> userInput;
	while ((userInput != 1) || (userInput != 2) || (userInput != 3)) {
		if (userInput == 1) {
			system("cls");
			Ride();
		}
		else if (userInput == 2) {
			system("cls");
			Explore();
		}
		else if (userInput == 3) {
			system("cls");
			Menu();
		}
		if (cin.fail()) {
			Sleep(150);
			system("cls");
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			errorDialogue();
			std::cout << "What do you wish to do? \n 1:Ride horse and travel \n 2:Dismount and Explore \n 3: Menu'" << endl;
			cin >> userInput;
		}
		else if ((userInput > 3) || (userInput < 1)) {
			Sleep(150);
			system("cls");
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			errorDialogue();
			std::cout << "What do you wish to do? \n 1:Ride horse and travel \n 2:Dismount and Explore \n 3:'Menu'" << endl;
			cin >> userInput;
		}
	}
}

//allows the player to move 1d6 in the map. It also saves the game once the player has stopped moving
void Ride() {

	playerMoveDice = rand() % 6 + 1;

	for (short int i = playerMoveDice; i > 0; i--) {
		map[P1.get_PlayerX()][P1.get_PlayerY()] = (map[P1.get_PlayerX()][P1.get_PlayerY()] - 1); //removing the +1 player modifier from the current Space but the Player hasn't moved yet.
		std::cout << "You rolled a " << playerMoveDice << "!\n" << endl;
		if ((P1.get_PlayerY() == 0) && (P1.get_PlayerX() == 0)) {
			P1.set_IncreMove(1);
			P1.set_HorizMove(1);
			P1.set_PlayerX(P1.get_PlayerX() + 1);
			map[P1.get_PlayerX()][P1.get_PlayerY()] = map[P1.get_PlayerX()][P1.get_PlayerY()] + 1;
			displayMap();
			Sleep(250);
			if (i > 1) {
				system("cls");
			}
		}
		else if (((P1.get_IncreMove() == 1) && (P1.get_HorizMove() == 1)) && (P1.get_PlayerX() < (x - 1))) {
			P1.set_PlayerX(P1.get_PlayerX() + 1);
			map[P1.get_PlayerX()][P1.get_PlayerY()] = map[P1.get_PlayerX()][P1.get_PlayerY()] + 1;
			displayMap();
			Sleep(250);
			if (i > 1) {
				system("cls");
			}
		}
		else if ((P1.get_PlayerY() == 0) && (P1.get_PlayerX() == (x - 1))) {
			P1.set_IncreMove(1);
			P1.set_HorizMove(0);
			P1.set_PlayerY(P1.get_PlayerY() + 1);
			map[P1.get_PlayerX()][P1.get_PlayerY()] = map[P1.get_PlayerX()][P1.get_PlayerY()] + 1;
			displayMap();
			Sleep(250);
			if (i > 1) {
				system("cls");
			}
		}
		else if (((P1.get_IncreMove() == 1) && (P1.get_HorizMove() == 0)) && (P1.get_PlayerY() < (y - 1))) {
			P1.set_PlayerY(P1.get_PlayerY() + 1);
			map[P1.get_PlayerX()][P1.get_PlayerY()] = map[P1.get_PlayerX()][P1.get_PlayerY()] + 1;
			displayMap();
			Sleep(250);
			if (i > 1) {
				system("cls");
			}
		}
		else if ((P1.get_PlayerY() == (y - 1)) && (P1.get_PlayerX() == (x - 1))) {
			P1.set_IncreMove(0);
			P1.set_HorizMove(1);
			P1.set_PlayerX(P1.get_PlayerX() - 1);
			map[P1.get_PlayerX()][P1.get_PlayerY()] = map[P1.get_PlayerX()][P1.get_PlayerY()] + 1;
			displayMap();
			Sleep(250);
			if (i > 1) {
				system("cls");
			}
		}
		else if (((P1.get_IncreMove() == 0) && (P1.get_HorizMove() == 1)) && (P1.get_PlayerX() > 0)) {
			P1.set_PlayerX(P1.get_PlayerX() - 1);
			map[P1.get_PlayerX()][P1.get_PlayerY()] = map[P1.get_PlayerX()][P1.get_PlayerY()] + 1;
			displayMap();
			Sleep(250);
			if (i > 1) {
				system("cls");
			}
		}
		else if ((P1.get_PlayerY() == (y - 1)) && (P1.get_PlayerX() == 0)) {
			P1.set_IncreMove(0);
			P1.set_HorizMove(0);
			P1.set_PlayerY(P1.get_PlayerY() - 1);
			map[P1.get_PlayerX()][P1.get_PlayerY()] = map[P1.get_PlayerX()][P1.get_PlayerY()] + 1;
			displayMap();
			Sleep(250);
			if (i > 1) {
				system("cls");
			}
		}
		else if (((P1.get_IncreMove() == 0) && (P1.get_HorizMove() == 0)) && (P1.get_PlayerY() > 0)) {
			P1.set_PlayerY(P1.get_PlayerY() - 1);
			map[P1.get_PlayerX()][P1.get_PlayerY()] = map[P1.get_PlayerX()][P1.get_PlayerY()] + 1;
			displayMap();
			Sleep(250);
			if (i > 1) {
				system("cls");
			}
		}
	}
	if ((P1.get_PlayerX() == ((x - 1) / 2)) && (P1.get_PlayerY() == ((y - 1) / 2))) {
		Explore();
	}
	else {
		P1.inventorySaveFileProcess();
		Sleep(250);
		P1.Save();
		Sleep(500);
		std::cout << "\nWhat will you do? \n" << endl;
		std::cout << "1: Ride horse and travel" << endl;
		std::cout << "2: Dismount horse and carefully explore the surroundings" << endl;
		std::cout << "3: 'Menu'" << endl;
		rideOrExplore();
	}
}

//allows the player to interact with the space he/she is currently in, Explore() always ends with a Ride() or a rideOrExplore()
void Explore() {
	switch (map[P1.get_PlayerX()][P1.get_PlayerY()]) {

		//space with * (monster/weapon/item/nothing)
	case 3: {
		emptySpaceRNG();
		break;
	}

			//Cathedral
	case 5: {
		playerHPCheck();
		if (P1.get_PlayerAllign() == 1) {
			P1.set_PlayerCurHP(P1.get_PlayerCurHP() + (P1.get_PlayerMaxHP()*0.2));
			playerHPCheck();
			std::cout << "The holy light heals you by 20% of your MaxHP";
		}
		else if (P1.get_PlayerAllign() == 0) {
			std::cout << "The holy light burns you dealing 20% of your MaxHP in damage";
			P1.set_PlayerCurHP(P1.get_PlayerCurHP() - (P1.get_PlayerMaxHP()*0.2));
			playerHPCheck();
		}
		Sleep(1500);
		system("cls");
		Ride();
		break;
	}

			//Graveyard
	case 7: {
		playerHPCheck();
		if (P1.get_PlayerAllign() == 1) {
			P1.set_PlayerCurHP(P1.get_PlayerCurHP() - (P1.get_PlayerMaxHP()*0.2));
			playerHPCheck();
			std::cout << "The unholy aura corrodes you dealing 20% of your MaxHP in damage";
		}
		else if (P1.get_PlayerAllign() == 0) {
			std::cout << "The unholy aura reconstructs your flesh healing you for 20% of your MaxHP";
			P1.set_PlayerCurHP(P1.get_PlayerCurHP() + (P1.get_PlayerMaxHP()*0.2));
			playerHPCheck();
		}
		Sleep(1500);
		system("cls");
		Ride();
		break;
	}

			//Well Of Reflection
	case 9: {
		wellOfReflectionChoice();
		break;
	}

			//Dragon Entrance (warps to Dragon Cave)
	case 11: {
		if ((P1.get_relicPick() == 1) && (P1.get_PlayerXP() >= 20)) {
			map[P1.get_PlayerX()][P1.get_PlayerY()] = map[P1.get_PlayerX()][P1.get_PlayerY()] - 1;
			P1.set_PlayerX((x - 1) / 2);
			P1.set_PlayerY((y - 1) / 2);
			map[P1.get_PlayerX()][P1.get_PlayerY()] = map[P1.get_PlayerX()][P1.get_PlayerY()] + 1;
		}
		else {
			std::cout << "A magical force field repels you. Perhaps a relic, or more experience in the matter is required";
			Sleep(1500);
			system("cls");
			Ride();
		}
		break;
	}

			 //Start Point and also shop
	case 13: {
		shopThing();
		break;
	}

			 //Dragon Cave (currently waps back to start)
	case 15: {
		dragonfight = true;
		P1.set_inBattle(1);
		GenerateMonster();
		P1.battle();
		break;
	}

			 //since there are no more defined things, and a space with value of 1 or greater than 15 is not a normally allowed, I'm assuming it's a gltich, and warping you back
	default: {
		std::cout << "Invalid place location, warping to start point";
		map[P1.get_PlayerX()][P1.get_PlayerY()] = map[P1.get_PlayerX()][P1.get_PlayerY()] - 1;
		P1.set_PlayerX(0);
		P1.set_PlayerY(0);
		P1.set_HorizMove(1);
		P1.set_IncreMove(1);
		map[P1.get_PlayerX()][P1.get_PlayerY()] = map[P1.get_PlayerX()][P1.get_PlayerY()] + 1;
		system("cls");
		displayMap();
		std::cout << "\nWhat will you do? \n" << endl;
		std::cout << "1:Ride horse and travel" << endl;
		std::cout << "2:Dismount horse and carefully explore the surroundings" << endl;
		std::cout << "3:'Menu'" << endl;
		rideOrExplore();
	}
	}
}

//checks the player's Vitality to define MaxHP then checks if CurHP is within valid parameters. Also deletes the save file upon death
void playerHPCheck() {
	P1.set_PlayerMaxHP(10 * P1.get_PlayerVitality());
	if (P1.get_PlayerCurHP() > P1.get_PlayerMaxHP()) {
		P1.set_PlayerCurHP(P1.get_PlayerMaxHP());
		return;
	}
	if (P1.get_PlayerCurHP() <= 0) {
		std::cout << "You died...";
		if (remove("RoFsave.txt") != 0) {
			perror("Error deleting file");
			std::cout << "poop"; //poop
		}
		Sleep(5000);
		exit(0);
	}
}

//checks the player's XP to see if it's within valid parameters, sets it to 0 if not
void playerXPCheck() {
	if (P1.get_PlayerLevel() == 396) {
		P1.set_PlayerRequiredXP(999999999);
	}
	if (P1.get_PlayerXP() < 0) {
		P1.set_PlayerXP(0);
	}
}

//used to check a variety of information related utilities, such as, character stats, weapons, inventory, and save and exit
void Menu() {
	system("cls");
	std::cout << "What do you wish to do? \n1:Resume Game\n2:Check 'Character'\n3:Check 'Weapons' inventory\n4:Check 'Items' inventory\n5:Save and Exit\n" << endl;
	short int userInput;
	cin >> userInput;
	while ((userInput != 1) || (userInput != 2) || (userInput != 3) || (userInput != 4) || (userInput != 5)) {
		if (cin.fail()) { //Input not of type or size 
			Sleep(150);
			system("cls"); //clears log
			cin.clear(); // clears error flags
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			errorDialogue();
			std::cout << "What do you wish to do? \n1:Resume Game\n2:Check 'Character'\n3:Check 'Weapons' inventory\n4:Check 'Items' inventory\n5:Save and Exit\n" << endl;
			cin >> userInput;
		}
		else if (userInput == 1) { //return to game
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			system("cls");
			displayMap();
			std::cout << "\nWhat will you do? \n" << endl;
			std::cout << "1:Ride horse and travel" << endl;
			std::cout << "2:Dismount horse and carefully explore the surroundings" << endl;
			std::cout << "3:'Menu'" << endl;
			rideOrExplore();
		}
		else if (userInput == 2) { //check character
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			system("cls");
			checkCharacter();
		}
		else if (userInput == 3) { //check weapons (currently set to 'return to game')
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			system("cls");
			checkWeaponCollection();
		}
		else if (userInput == 4) {
			cin.clear(); // 
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			system("cls");
			checkItemInventory();
		}
		else if (userInput == 5) { //save and exit function
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			system("cls");
			P1.inventorySaveFileProcess();
			P1.Save();
			exit(0);
		}
		else if ((userInput > 5) || (userInput < 1)) { //Numeric input out of bounds
			Sleep(150);
			system("cls");
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			errorDialogue();
			std::cout << "What do you wish to do? \n1:Resume Game\n2:Check 'Character'\n3:Check 'Weapons' inventory\n4:Check 'Items' inventory\n5:Save and Exit\n" << endl;
			cin >> userInput;
		}
	}
}

//used to display a screen with all the player's stats 
void checkCharacter() {
	P1.displayStats();
	std::cout << "============================================================================================================\n" << endl;
	std::cout << "\nWhat will you do?\n1:Return to 'Menu'\n2:Return to Game\n" << endl;
	short int userInput;
	cin >> userInput;
	while ((userInput != 1) || (userInput != 2)) {
		if (cin.fail()) {
			Sleep(150);
			system("cls"); //clears log
			cin.clear(); // clears error flags
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			errorDialogue();
			std::cout << "What will you do?\n1:Return to 'Menu'\n2:Return to Game" << endl;
			cin >> userInput;
		}
		else if (userInput == 1) {
			cin.clear(); // clears error flags
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			system("cls");
			Menu();
		}
		else if (userInput == 2) {
			cin.clear(); // clears error flags
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			system("cls");
			displayMap();
			std::cout << "\nWhat will you do? \n" << endl;
			std::cout << "1:Ride horse and travel" << endl;
			std::cout << "2:Dismount horse and carefully explore the surroundings" << endl;
			std::cout << "3:'Menu'" << endl;
			rideOrExplore();
		}
		else if ((userInput > 2) || (userInput < 1)) {
			Sleep(150);
			system("cls");
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			errorDialogue();
			std::cout << "What will you do?\n1:Return to Menu\n2:Return to Game" << endl;
			cin >> userInput;
		}
	}
}

//was too long to keep in Explore() so I made it's own function
void wellOfReflectionChoice() {
	playerXPCheck();
	std::cout << "Do you wish to look into the Well of Reflection?" << endl;
	std::cout << "1:Yes\n2:No\n" << endl;
	short int userInput;
	cin >> userInput;
	while ((userInput != 1) || (userInput != 2)) {
		if (cin.fail()) {
			Sleep(150);
			system("cls");
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			errorDialogue();
			std::cout << "Look into the Well of Reflection?\n1:Yes\n2:No\n" << endl;
			cin >> userInput;
		}
		else if (userInput == 1) {
			system("cls");
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			std::cout << "You look into the mirror-like well, and in it you see the errors of your path." << endl;
			unsigned int WellXP = P1.get_PlayerXP();
			if (P1.get_PlayerAllign() == 1) {
				P1.set_PlayerAllignment(0);
				if (WellXP <= 20) {
					P1.set_PlayerXP(P1.get_PlayerXP() + 2);
					std::cout << "You gain 2XP!";
				}
				else if (WellXP > 20) {
					P1.set_PlayerXP(P1.get_PlayerXP() + (P1.get_PlayerXP()*0.1));
					std::cout << "You gain " << (P1.get_PlayerXP()*0.1) << "XP!";
				}
			}
			else if (P1.get_PlayerAllign() == 0) {
				P1.set_PlayerAllignment(1);
				if (WellXP <= 20) {
					P1.set_PlayerXP(P1.get_PlayerXP() + 2);
					std::cout << "You gain 2XP!";
				}
				else if (WellXP > 20) {
					P1.set_PlayerXP(P1.get_PlayerXP() + (P1.get_PlayerXP()*0.1));
					std::cout << "You gain " << (P1.get_PlayerXP()*0.1) << "XP!";
				}
			}
			Sleep(3000);
			WellXP = 0;
			system("cls");
			Ride();
			return;
		}
		else if (userInput == 2) {
			std::cout << "You choose to avoid troubling your mind with magical introspection wells" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Sleep(1500);
			system("cls");
			Ride();
			return;
		}
		else if ((userInput > 2) || (userInput < 1)) {
			Sleep(150);
			system("cls");
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			errorDialogue();
			std::cout << "Look into the Well of Reflection?\n1:Yes\n2:No\n" << endl;
			cin >> userInput;
		}
	}
}

//generic message display
void errorDialogue() {
	std::cout << "INVALID INPUT. Please use only the numbers provided. \n" << endl;
	return;
}

//used to determine if the file I'm trying to load exists
bool does_file_exist(const char *fileName) {
	ifstream myfile(fileName);
	return myfile.good();
}

//determines what happens when the player explores an empty spot
void emptySpaceRNG() {
	short unsigned int rng;
	rng = rand() % 100 + 1;
	if (rng <= 30) {//(rng <= 30)
		itemRNG();
	}
	else if ((rng > 30) && (rng <= 70)) {//(rng > 30) && (rng <= 80)
		std::cout << "You encounter a Monster!" << endl;
		GenerateMonster();
		P1.set_inBattle(1);
		P1.battle();
	}
	else if ((rng > 70) && (rng <= 100)) { //(rng > 70) && (rng <= 100)
		weaponModifierSpawnRNG();
	}
}

//determines what item the player gets from an item check in the empty spot
void itemRNG() {
	unsigned int rng;
	rng = rand() % 100 + 1;
	if ((rng > 0) && (rng <= 30)) {
		std::cout << "You find a 'Small_Potion'" << endl;
		iCount = 0;
		if (P1.get_pickedSomethingUp() == 0) {
			iCollect[iCount] = Small_Potion;
			P1.set_pickedSomethingUp(1);
		}
		else {
			pickingSomethingUp = true;
			findEmptyInventorySlot();
			iCollect[iCount] = Small_Potion;
		}
	}
	else if ((rng > 50) && (rng <= 60)) {
		std::cout << "You find a 'Medium_Potion'" << endl;
		iCount = 0;
		if (P1.get_pickedSomethingUp() == 0) {
			iCollect[iCount] = Medium_Potion;
			P1.set_pickedSomethingUp(1);
		}
		else {
			pickingSomethingUp = true;
			findEmptyInventorySlot();
			iCollect[iCount] = Medium_Potion;
		}
	}
	else if ((rng > 30) && (rng <= 50)) {
		std::cout << "You find a 'Small_Explosive'" << endl;
		iCount = 0;
		if (P1.get_pickedSomethingUp() == 0) {
			iCollect[iCount] = Small_Explosive;
			P1.set_pickedSomethingUp(1);
		}
		else {
			pickingSomethingUp = true;
			findEmptyInventorySlot();
			iCollect[iCount] = Small_Explosive;
		}
	}
	else if ((rng > 60) && (rng <= 90)) {
		if (P1.get_relicPick() == 1) {
			std::cout << "You thought you found an item. But it was just rubble" << endl;
			Sleep(1500);
			system("cls");
			Ride();
		}
		else if (P1.get_relicPick() == 0) {
			std::cout << "You find a 'Relic'" << endl;
			iCount = 0;
			if ((P1.get_pickedSomethingUp() == 0)) {
				iCollect[iCount] = Relic;
				P1.set_relicPick(1);
				P1.set_pickedSomethingUp(1);
			}
			else {
				pickingSomethingUp = true;
				findEmptyInventorySlot();
				iCollect[iCount] = Relic;
				P1.set_relicPick(1);
			}
		}
	}
	else if ((rng > 90) && (rng <= 95)) {
		std::cout << "You find a 'Curse_Jar'" << endl;
		iCount = 0;
		if (P1.get_pickedSomethingUp() == 0) {
			iCollect[iCount] = Curse_Jar;
			P1.set_pickedSomethingUp(1);
		}
		else {
			pickingSomethingUp = true;
			findEmptyInventorySlot();
			iCollect[iCount] = Curse_Jar;
		}
	}
	else if ((rng > 95) && (rng <= 100)) {
		std::cout << "You find an 'Elixir' " << endl;
		iCount = 0;
		if (P1.get_pickedSomethingUp() == 0) {
			iCollect[iCount] = Elixir;
			P1.set_pickedSomethingUp(1);
		}
		else {
			pickingSomethingUp = true;
			findEmptyInventorySlot();
			iCollect[iCount] = Elixir;
		}
	}
	iCount = 0;
	P1.inventorySaveFileProcess();
	Sleep(1500);
	system("cls");
	Ride();
}

//dialogue function for battle or outside of battle items
void invInOutofbattle() {
	if (P1.get_inBattle() == 0) {
		std::cout << "\n6:Previous Page\n7:Next Page\n8:Return to 'Menu'\n" << endl;
	}
	else {
		std::cout << "\n6:Previous Page\n7:Next Page\n8:Return to Inventory Select\n" << endl;
	}
}

//allows one to use items (calls on the display function)
void checkItemInventory() {
	inventorySortProcess();
	flagInvalidPlace();
	displayInventory();
	invInOutofbattle();
	short int userInput;
	cin >> userInput;
	while ((userInput != 1) || (userInput != 2) || (userInput != 3) || (userInput != 4) || (userInput != 5) || (userInput != 6) || (userInput != 7) || (userInput != 8)) {
		if (cin.fail()) {
			Sleep(150);
			system("cls"); //clears log
			cin.clear(); // clears error flags
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			errorDialogue();
			pageN = 0;
			displayInventory();
			invInOutofbattle();
		}
		else if (userInput == 1) {
			if (options[0] == 0) {
				std::cout << "option not available at the moment \nReturning to Previous screen";
				Sleep(1500);
				system("cls"); //clears log
				cin.clear(); // clears error flags
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				pageN = 0;
				if (P1.get_inBattle() == 0) {
					Menu();
				}
				else {
					battleMenu();
				}

			}
			else {
				usedItemIndex = (0 + (P1.get_iSize()*pageN));//make it a 0+ variable (that grows in value by 5 every time the array exapnds)
				useItemSwitch();
			}
		}
		else if (userInput == 2) {
			if (options[1] == 0) {
				std::cout << "option not available at the moment \nReturning to Previous screen";
				Sleep(1500);
				system("cls"); //clears log
				cin.clear(); // clears error flags
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				pageN = 0;
				if (P1.get_inBattle() == 0) {
					Menu();
				}
				else {
					battleMenu();
				}
			}
			else {
				usedItemIndex = (1 + (P1.get_iSize()*pageN));//make it a 0+ variable (that grows in value by 5 every time the array exapnds)
				useItemSwitch();
			}
		}
		else if (userInput == 3) {
			if (options[2] == 0) {
				std::cout << "option not available at the moment \nReturning to Previous screen";
				Sleep(1500);
				system("cls"); //clears log
				cin.clear(); // clears error flags
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				pageN = 0;
				if (P1.get_inBattle() == 0) {
					Menu();
				}
				else {
					battleMenu();
				}
			}
			else {
				usedItemIndex = (2 + (P1.get_iSize()*pageN));//make it a 0+ variable (that grows in value by 5 every time the array exapnds)
				useItemSwitch();
			}
		}
		else if (userInput == 4) {
			if (options[3] == 0) {
				std::cout << "option not available at the moment \nReturning to Previous screen";
				Sleep(1500);
				system("cls"); //clears log
				cin.clear(); // clears error flags
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				pageN = 0;
				if (P1.get_inBattle() == 0) {
					Menu();
				}
				else {
					battleMenu();
				}
			}
			else {
				usedItemIndex = (3 + (P1.get_iSize()*pageN));//make it a 0+ variable (that grows in value by 5 every time the array exapnds)
				useItemSwitch();
			}
		}
		else if (userInput == 5) {
			if (options[4] == 0) {
				std::cout << "option not available at the moment \nReturning to Previous screen";
				Sleep(1500);
				system("cls"); //clears log
				cin.clear(); // clears error flags
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				pageN = 0;
				if (P1.get_inBattle() == 0) {
					Menu();
				}
				else {
					battleMenu();
				}
			}
			else {
				usedItemIndex = (4 + (P1.get_iSize()*pageN));//make it a 0+ variable (that grows in value by 5 every time the array exapnds)
				useItemSwitch();
			}
		}
		else if (userInput == 6) {
			if ((P1.get_nOfExpand() > 0) && ((pageN <= P1.get_nOfExpand()) && (pageN>0))) {
				pageN = pageN - 1;
				system("cls"); //clears log
				cin.clear(); // clears error flags
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				checkItemInventory();
			}
			else if (pageN == 0) {
				std::cout << "Previous Page not available at the moment.";
				Sleep(1500);
				system("cls"); //clears log
				cin.clear(); // clears error flags
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				checkItemInventory();
			}
		}
		else if (userInput == 7) {
			if ((P1.get_nOfExpand() > 0) && ((pageN >= 0) && (pageN<P1.get_nOfExpand()))) {
				pageN = pageN + 1;
				system("cls"); //clears log
				cin.clear(); // clears error flags
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				checkItemInventory();
			}
			else if ((pageN == P1.get_nOfExpand())) {
				std::cout << "Next Page not available at the moment.";
				Sleep(1500);
				system("cls"); //clears log
				cin.clear(); // clears error flags
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				checkItemInventory();
			}
		}
		else if (userInput == 8) { //if not combat; else Combat menu;
			cin.clear(); // clears error flags
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			pageN = 0;
			system("cls");
			if (P1.get_inBattle() == 0) {
				Menu();
			}
			else {
				battleMenu();
			}

		}
		else if ((userInput > 8) || (userInput < 1)) {
			Sleep(150);
			system("cls");
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			errorDialogue();
			Sleep(750);
			system("cls");
			checkItemInventory();
			invInOutofbattle();
		}
	}
}

//allows one to see items
void displayInventory() {
	int displayC = 0;
	std::cout << "What will you do?\n" << endl;
	if (P1.get_pickedSomethingUp() == 0) {
		std::cout << "Inventory is empty" << endl;
		P1.set_relicPick(0);
	}
	else {
		if (P1.get_nOfExpand() > 0) {
			std::cout << "Page: " << (pageN + 1) << "/" << (P1.get_nOfExpand() + 1) << endl;
		}
		else {
			std::cout << "Page: " << 1 << "/" << 1 << endl;
		}
		for (iCount = (0 + (P1.get_iSize()* pageN)); iCount < (5 + (P1.get_iSize()* pageN)); iCount++) {
			std::cout << (displayC + 1);
			switch (iCollect[iCount]) {
			case Small_Explosive: {
				options[displayC] = 1;
				std::cout << ": Use 'Small Explosive'";
				break;
			}
			case Small_Potion: {
				options[displayC] = 1;
				std::cout << ": Use 'Small Potion'";
				break;
			}
			case Medium_Potion: {
				options[displayC] = 1;
				std::cout << ": Use 'Medium Potion'";
				break;
			}
			case Elixir: {
				options[displayC] = 1;
				std::cout << ": Use 'Elixir'";
				break;
			}
			case Curse_Jar: {
				options[displayC] = 1;
				std::cout << ": Use 'Curse Jar'";
				break;
			}
			case Relic: {
				options[displayC] = 1;
				P1.set_relicPick(1);
				std::cout << ": Relic";
				break;
			}
			default: {
				options[displayC] = 0;
				std::cout << ": --------------------";
			}
			}
			displayC = displayC + 1;
			std::cout << "\n" << endl;
		}
	}
	displayC = 0;
	iCount = 0;
}

//function outlining the actions and behaviors of used items in and out of combat
void useItemSwitch() {
	if (P1.get_inBattle() == 0) {
		switch (iCollect[usedItemIndex]) {
		case Small_Potion: {
			P1.set_PlayerCurHP(P1.get_PlayerCurHP() + (P1.get_PlayerMaxHP()*0.15));
			std::cout << "You drink the Small Potion.\n It heals your HP by " << (P1.get_PlayerMaxHP()*0.15) << " points";
			playerHPCheck();
			iCollect[usedItemIndex] = 0;
			flagInvalidPlace();
			break;
		}
		case Medium_Potion: {
			P1.set_PlayerCurHP(P1.get_PlayerCurHP() + (P1.get_PlayerMaxHP()*0.4));
			std::cout << "You drink the Medium Potion.\n It heals your HP by " << (P1.get_PlayerMaxHP()*0.4) << " points";
			playerHPCheck();
			iCollect[usedItemIndex] = 0;
			flagInvalidPlace();
			break;
		}
		case Elixir: {
			P1.set_PlayerCurHP(P1.get_PlayerMaxHP());
			std::cout << "You drink the Elixir.\n It heals your HP completely";
			playerHPCheck();
			iCollect[usedItemIndex] = 0;
			flagInvalidPlace();
			break;
		}
		case Small_Explosive: {
			std::cout << "You see no reason why to use this item out of combat";
			break;
		}
		case Curse_Jar: {
			std::cout << "You see no reason why to use this item out of combat";
			break;
		}
		default: {
			std::cout << "You see no reason why to use this item";
		}
		}
		Sleep(1500);
		system("cls");
		pageN = 0;
		displayMap();
		std::cout << "\nWhat do you wish to do? \n 1:Ride horse and travel \n 2:Dismount and Explore \n 3: Menu'\n" << endl;
		rideOrExplore();
	}
	else if ((P1.get_inBattle() == 1)) {
		P1.turnOrder();
		playerHPCheck();
		switch (iCollect[usedItemIndex]) {
		case Small_Potion: {
			P1.set_PlayerCurHP(P1.get_PlayerCurHP() + (P1.get_PlayerMaxHP()*0.15));
			std::cout << "You drink the Small_Potion.\n It heals your HP by " << (P1.get_PlayerMaxHP()*0.15) << " points";
			playerHPCheck();
			iCollect[usedItemIndex] = 0;
			break;
		}
		case Medium_Potion: {
			P1.set_PlayerCurHP(P1.get_PlayerCurHP() + (P1.get_PlayerMaxHP()*0.4));
			std::cout << "You drink the Medium_Potion.\n It heals your HP by " << (P1.get_PlayerMaxHP()*0.4) << " points";
			playerHPCheck();
			iCollect[usedItemIndex] = 0;
			break;
		}
		case Elixir: {
			P1.set_PlayerCurHP(P1.get_PlayerMaxHP());
			std::cout << "You drink the Healing Elixir.\n It heals your HP completely";
			playerHPCheck();
			iCollect[usedItemIndex] = 0;
			break;
		}
		case Small_Explosive: {
			system("cls");
			std::cout << "You throw the vial of explosive liquid at the monster!\nThe ensuing explosion deals the monster ";
			monster_CurHP = (monster_CurHP - (monster_MaxHP*0.25));
			std::cout << (monster_MaxHP*0.25) << " points of damage";
			iCollect[usedItemIndex] = 0;
			break;
		}
		case Curse_Jar: {
			system("cls");
			std::cout << "You throw the Curse jar at the monster!\n The evil spirits within the jar curse the monster dealing ";
			monster_CurHP = (monster_CurHP - (monster_MaxHP*0.5));
			std::cout << (monster_MaxHP*0.5) << " points of damage!";
			iCollect[usedItemIndex] = 0;
			break;
		}
		default: {
			std::cout << "You see no reason to use this item";
		}
		}
		usedItemIndex = 0;
		pageN = 0;
		flagInvalidPlace();
		Sleep(2000);
		system("cls");
		P1.battleWin();
		if (playerTurn) {
			monsterattack();
			playerHPCheck();
		}
		if (monster_CurHP > 0) {
			P1.battle();
		}
	}
}

//find all spaces with gibberish values and set them to 0, also finds Relic and items in inventory.
void flagInvalidPlace() {
	//being that it is one of the first things to load, I wanted it to be able to fill/flag the unused adresses with '0'
	if (!newOrLoad) {
		P1.set_nOfExpand(4);
		P1.set_iMaxInv(P1.get_iSize() * (P1.get_nOfExpand() + 1));
		newOrLoad = false;
	}

	for (int U = 0; U < P1.get_iMaxInv(); U++) {
		if ((iCollect[U] == Small_Explosive) || (iCollect[U] == Small_Potion) || (iCollect[U] == Medium_Potion) || (iCollect[U] == Elixir) || (iCollect[U] == Curse_Jar)) {
			P1.set_pickedSomethingUp(1);
		}
		else if (iCollect[U] == Relic) {
			P1.set_pickedSomethingUp(1);
			P1.set_relicPick(1);
		}
		else {
			iCollect[U] = 0;
		}
	}
}

//finds the index of a space with a value of 0 in which to allocate an item. it also finds out when you've run out of space so as to call the expand function
void findEmptyInventorySlot() {
	iCount = 0;
	flagInvalidPlace();
	if (pickingSomethingUp) {
		int tC = 0; //stands for Thing counter
					//find all spaces that have the forcible 0 mark on them and use them as valid slots for item allocation
		while ((tC < P1.get_iMaxInv())) {
			if (iCollect[tC] == 0) {
				iCount = tC; //item allocation
				pickingSomethingUp = false;
				break;
			}
			else {
				tC++;
			}
		}
		if ((tC == P1.get_iMaxInv()) && (P1.get_nOfExpand() < 4)) {
			ExpandInv();
		}
		else if ((tC == P1.get_iMaxInv()) && (P1.get_nOfExpand() == 4)) {
			std::cout << "item not picked up";
			Sleep(1500);
			system("cls");
			Ride();
		}
	}
}

//Expands the inventory by 5 upon obtaining an item that would push the threshold into the next 5 numbers. 
void ExpandInv() {
	P1.set_iMaxInv((P1.get_iSize()) + (P1.get_iMaxInv()));
	int *tempInventory = nullptr;
	tempInventory = new (nothrow) int[P1.get_iMaxInv()];
	for (int i = 0; i < P1.get_iMaxInv(); i++) {
		tempInventory[i] = iCollect[i];
	}
	delete[] iCollect;
	iCollect = nullptr;
	iCollect = new (nothrow) int[P1.get_iMaxInv()];
	for (int i = 0; i < P1.get_iMaxInv(); i++) {
		iCollect[i] = tempInventory[i];
	}
	delete[] tempInventory;
	P1.set_nOfExpand(P1.get_nOfExpand() + 1);
	findEmptyInventorySlot();
}

//calls all the random generators in the Monster
void GenerateMonster() {
	randMonsterVit();
	randMonsterStr();
	randMonsterEnd();
	randMonsterSpd();
	monster_MaxHP = monster_Vit * 10;
	monster_CurHP = monster_MaxHP;
}

//Randomizes the monster's Vitality
void randMonsterVit() { //396/12 = 33; 33*4 (because it doesn't discriminate what stat was leveled up) = 132. + the RNG base stat means that the maximum possible is 143 Vit = = 1430 MaxHP.
	short unsigned int RNG;

	if (!dragonfight) {
		if (P1.get_PlayerLevel()<10) {
			RNG = rand() % 50 + 1;
		}
		else if (P1.get_PlayerLevel()<20) {
			RNG = rand() % 70 + 1;
		}
		else if (P1.get_PlayerLevel()<30) {
			RNG = rand() % 80 + 3;
		}
		else {
			RNG = rand() % 100 + 1;
		}
		
	}
	else {
		RNG = 100;
	}

	if (RNG <=10) {
		monster_Vit = (((P1.get_PlayerLevel() / 4) + 1));
	}
	else if ((RNG > 10) && (RNG <= 25)) {
		monster_Vit = (((P1.get_PlayerLevel() / 4) + 2));
	}
	else if ((RNG > 25) && (RNG <= 50)) {
		monster_Vit = (((P1.get_PlayerLevel() / 4) + 3));
	}
	else if ((RNG > 50) && (RNG <= 69)) {
		monster_Vit = (((P1.get_PlayerLevel() / 4) + 4));
	}
	else if ((RNG > 69) && (RNG <= 74)) {
		monster_Vit = (((P1.get_PlayerLevel() / 4) + 5));
	}
	else if ((RNG > 74) && (RNG <= 79)) {
		monster_Vit = (((P1.get_PlayerLevel() / 4) + 6));
	}
	else if ((RNG > 79) && (RNG <= 84)) {
		monster_Vit = (((P1.get_PlayerLevel() / 4) + 7));
	}
	else if ((RNG > 84) && (RNG <= 89)) {
		monster_Vit = (((P1.get_PlayerLevel() / 4) + 8));
	}
	else if ((RNG > 89) && (RNG <= 94)) {
		monster_Vit = (((P1.get_PlayerLevel() / 4) + 9));
	}
	else if ((RNG > 94) && (RNG <= 99)) {
		monster_Vit = (((P1.get_PlayerLevel() / 4) + 10));
	}
	else if ((RNG > 99) && (RNG <= 100)) {
		monster_Vit = (((P1.get_PlayerLevel() / 4) + 11));
	}
}

//Randomizes the monster's Strength
void randMonsterStr() { //I don't mind the player taking A TON of damage, so the str stat is as abundant as the VIT
	short unsigned int RNG;
	if (!dragonfight) {
		
		if (P1.get_PlayerLevel()<10) {
			RNG = rand() % 50 + 1;
		}
		else if (P1.get_PlayerLevel()<20) {
			RNG = rand() % 70 + 1;
		}
		else if (P1.get_PlayerLevel()<30) {
			RNG = rand() % 80 + 3;
		}
		else {
			RNG = rand() % 100 + 1;
		}
	}
	else {
		RNG = 100;
	}
	if (RNG <= 10) {
		monster_Str = (((P1.get_PlayerLevel() / 7) + 1));
	}
	else if ((RNG > 10) && (RNG <= 25)) {
		monster_Str = (((P1.get_PlayerLevel() / 7) + 2));
	}
	else if ((RNG > 25) && (RNG <= 50)) {
		monster_Str = (((P1.get_PlayerLevel() / 7) + 3));
	}
	else if ((RNG > 50) && (RNG <= 69)) {
		monster_Str = (((P1.get_PlayerLevel() / 7) + 4));
	}
	else if ((RNG > 69) && (RNG <= 74)) {
		monster_Str = (((P1.get_PlayerLevel() / 7) + 5));
	}
	else if ((RNG > 74) && (RNG <= 79)) {
		monster_Str = (((P1.get_PlayerLevel() / 7) + 6));
	}
	else if ((RNG > 79) && (RNG <= 84)) {
		monster_Str = (((P1.get_PlayerLevel() / 7) + 7));
	}
	else if ((RNG > 84) && (RNG <= 89)) {
		monster_Str = (((P1.get_PlayerLevel() / 7) + 8));
	}
	else if ((RNG > 89) && (RNG <= 94)) {
		monster_Str = (((P1.get_PlayerLevel() / 7) + 9));
	}
	else if ((RNG > 94) && (RNG <= 99)) {
		monster_Str = (((P1.get_PlayerLevel() / 7) + 10));
	}
	else if ((RNG > 99) && (RNG <= 100)) {
		monster_Str = (((P1.get_PlayerLevel() / 7) + 11));
	}
}

//Randomizes the monster's Endurance
void randMonsterEnd() { //MAX PLAYER LEVEL = 396. Divided by 18 = 22; x4 (because it doesn't discriminate what stat was leveled up) 88. + the RNG base stat means that the maximum possible is 99 end.
	short unsigned int RNG;
	if (!dragonfight) {
		if (P1.get_PlayerLevel()<10) {
			RNG = rand() % 50 + 1;
		}
		else if (P1.get_PlayerLevel()<20) {
			RNG = rand() % 70 + 1;
		}
		else if (P1.get_PlayerLevel()<30) {
			RNG = rand() % 80 + 3;
		}
		else {
			RNG = rand() % 100 + 1;
		}
	}
	else {
		RNG = 100;
	}
	if (RNG == 10) {
		monster_End = ((((P1.get_PlayerLevel() / 10) + 1)));
	}
	else if ((RNG > 10) && (RNG <= 25)) {
		monster_End = ((((P1.get_PlayerLevel() / 10) + 2)));
	}
	else if ((RNG > 25) && (RNG <= 50)) {
		monster_End = ((((P1.get_PlayerLevel() / 10) + 3)));
	}
	else if ((RNG > 50) && (RNG <= 69)) {
		monster_End = ((((P1.get_PlayerLevel() / 10) + 4)));
	}
	else if ((RNG > 69) && (RNG <= 74)) {
		monster_End = ((((P1.get_PlayerLevel() / 10) + 5)));
	}
	else if ((RNG > 74) && (RNG <= 79)) {
		monster_End = ((((P1.get_PlayerLevel() / 10) + 6)));
	}
	else if ((RNG > 79) && (RNG <=  84)) {
		monster_End = ((((P1.get_PlayerLevel() / 10) + 7)));
	}
	else if ((RNG > 84) && (RNG <= 89)) {
		monster_End = ((((P1.get_PlayerLevel() / 10) + 8)));
	}
	else if ((RNG > 89) && (RNG <= 94)) {
		monster_End = ((((P1.get_PlayerLevel() / 10) + 9)));
	}
	else if ((RNG > 94) && (RNG <= 99)) {
		monster_End = ((((P1.get_PlayerLevel() / 10) + 10)));
	}
	else if ((RNG > 99) && (RNG <= 100)) {
		monster_End = ((((P1.get_PlayerLevel() / 10) + 11)));
	}
}

//Randomizes the monster's Speed
void randMonsterSpd() {
	short unsigned int RNG;
	if (!dragonfight) {
		if (P1.get_PlayerLevel()<10) {
			RNG = rand() % 50 + 1;
		}
		else if (P1.get_PlayerLevel()<20) {
			RNG = rand() % 70 + 1;
		}
		else if (P1.get_PlayerLevel()<30) {
			RNG = rand() % 80 + 3;
		}
		else {
			RNG = rand() % 100 + 1;
		}
	}
	else {
		RNG = 100;
	}

	if (RNG <= 10) {
		monster_Spd = ((((P1.get_PlayerLevel() / 10) + 4)));
	}
	else if ((RNG > 10) && (RNG <= 25)) {
		monster_Spd = ((((P1.get_PlayerLevel() / 10) + 5)));
	}
	else if ((RNG > 25) && (RNG <= 45)) {
		monster_Spd = ((((P1.get_PlayerLevel() / 10) + 6)));
	}
	else if ((RNG > 45) && (RNG <= 55)) {
		monster_Spd = ((((P1.get_PlayerLevel() / 10) + 7)));
	}
	else if ((RNG > 55) && (RNG <= 69)) {
		monster_Spd = ((((P1.get_PlayerLevel() / 10) + 8)));
	}
	else if ((RNG > 69) && (RNG <= 74)) {
		monster_Spd = ((((P1.get_PlayerLevel() / 10) + 9)));
	}
	else if ((RNG > 74) && (RNG <= 79)) {
		monster_Spd = ((((P1.get_PlayerLevel() / 10) + 10)));
	}
	else if ((RNG > 79) && (RNG <= 84)) {
		monster_Spd = ((((P1.get_PlayerLevel() / 10) + 11)));
	}
	else if ((RNG > 84) && (RNG <= 89)) {
		monster_Spd = ((((P1.get_PlayerLevel() / 10) + 12)));
	}
	else if ((RNG > 89) && (RNG <= 94)) {
		monster_Spd = ((((P1.get_PlayerLevel() / 10) + 13)));
	}
	else if ((RNG > 94) && (RNG <= 100)) {
		monster_Spd = ((((P1.get_PlayerLevel() / 10) + 14)));
	}
}

//Randomizes the damage value a monster is capable of dealing
void monsterattack() {
	short unsigned int RNG;
	if (!dragonfight) {
		RNG = rand() % 100 + 1;
	}
	else {
		RNG = rand() % 100 + 41;
	}
	if (RNG <= 10) {
		monster_processedDamage = 0;
		std::cout << "The monster's attack misses you";
	}
	else if ((RNG > 10) && (RNG <= 20)) {
		monster_processedDamage = ((((monster_Str * 0.5) + 10) - (P1.get_PlayerEndurance())));
		std::cout << "The monster's attack barely hits you";
	}
	else if ((RNG > 20) && (RNG <= 30)) {
		monster_processedDamage = ((((monster_Str * 0.6) + 10) - (P1.get_PlayerEndurance())));
		std::cout << "The monster's attack grazes you";
	}
	else if ((RNG > 30) && (RNG <= 40)) {
		monster_processedDamage = ((((monster_Str * 0.7) + 10) - (P1.get_PlayerEndurance())));
		std::cout << "The monster lands a weak hit on you";
	}
	else if ((RNG > 40) && (RNG <= 50)) {
		monster_processedDamage = ((((monster_Str * 0.8) + 10) - (P1.get_PlayerEndurance())));
		std::cout << "The monster lands a hit on you";
	}
	else if ((RNG > 50) && (RNG <= 60)) {
		monster_processedDamage = ((((monster_Str*0.9)+10) - (P1.get_PlayerEndurance())));
		std::cout << "The monster attacks you";
	}
	else if ((RNG > 60) && (RNG <= 70)) {
		monster_processedDamage = ((((monster_Str) + 10) - (P1.get_PlayerEndurance()*0.95)));
		std::cout << "The monster skillfully attacks you";
	}
	else if ((RNG > 70) && (RNG <= 80)) {
		monster_processedDamage = ((((monster_Str * 1.15) + 10) - (P1.get_PlayerEndurance()*0.9)));
		std::cout << "The monster gets a solid hit on you";
	}
	else if ((RNG > 80) && (RNG <= 90)) {
		monster_processedDamage = ((((monster_Str * 1.3) + 10) - (P1.get_PlayerEndurance()*0.85)));
		std::cout << "The monster attacks you with great power";
	}
	else if ((RNG > 90) && (RNG <= 95)) {
		monster_processedDamage = ((((monster_Str * 1.35) + 10) - (P1.get_PlayerEndurance()*0.8)));
		std::cout << "The monster lands a devastating blow on you";
	}
	else if ((RNG > 95) && (RNG <= 100)) {
		monster_processedDamage = ((((monster_Str * 1.5) + 10) - (P1.get_PlayerEndurance()*0.75)) * 2);
		std::cout << "The monster lands a devastating critical hit on you";
	}
	if (monster_processedDamage <= 0) {
		monster_processedDamage = 0;
	}
	P1.set_PlayerCurHP(P1.get_PlayerCurHP() - monster_processedDamage);
	std::cout << " dealing:\n" << monster_processedDamage << " damage!";
	playerHPCheck;
	Sleep(1500);
	system("cls");
}

//displays message that matches the monster
void displayMonster() {
	if (!dragonfight) {
		std::cout << "Monster HP: ";
	}
	else {
		std::cout << "DRAGON HP: ";
	}
	std::cout << monster_CurHP << " / " << monster_MaxHP << endl;
	std::cout << "Strength: " << monster_Str << "		 Endurance: " << monster_End << "			Speed: " << monster_Spd << endl;
	std::cout << "=====================================================================\n" << endl;
}

//it turns out that making an inventory that shrinks is impossible... so I'll keep it as a sorting mechanism
void inventorySortProcess() {
	flagInvalidPlace();
	int lackOfAbetterNum = 0;
	int *tempInventory = nullptr;
	tempInventory = new (nothrow) int[P1.get_iMaxInv()];
	for (int i = 0; i < P1.get_iMaxInv(); i++) {
		if ((iCollect[i] == Small_Explosive) || (iCollect[i] == Small_Potion) || (iCollect[i] == Medium_Potion) || (iCollect[i] == Elixir) || (iCollect[i] == Curse_Jar) || (iCollect[i] == Relic)) {
			tempInventory[lackOfAbetterNum] = iCollect[i];
			lackOfAbetterNum++;
		}
		else {
			tempInventory[i] = 0;
		}
	}
	lackOfAbetterNum = 0;
	delete[] iCollect;
	iCollect = nullptr;
	iCollect = new (nothrow) int[P1.get_iMaxInv()];

	for (int W = 0; W < P1.get_iMaxInv(); W++) {
		if ((tempInventory[W] == Small_Explosive) || (tempInventory[W] == Small_Potion) || (tempInventory[W] == Medium_Potion) || (tempInventory[W] == Elixir) || (tempInventory[W] == Curse_Jar) || (tempInventory[W] == Relic)) {
			iCollect[lackOfAbetterNum] = tempInventory[W];
			lackOfAbetterNum++;
		}
		else {
			iCollect[W] = 0;
		}
	}
	delete[] tempInventory;
}

//Randomizes the weapon type from 1-6 all weapon types are equally probable as they have no effect.
void weaponTypeRNG() {
	unsigned int rng;
	rng = rand() % 6 + 1;
	switch (rng) {
	case 1: {
		std::cout << " Crossbow" << endl;
		weaponType = weaponType + 1;
		wCollect.push_back(weaponType);
		break;
	}
	case 2: {
		std::cout << " Flail" << endl;
		weaponType = weaponType + 2;
		wCollect.push_back(weaponType);
		break;
	}
	case 3: {
		std::cout << " Whip" << endl;
		weaponType = weaponType + 3;
		wCollect.push_back(weaponType);
		break;
	}
	case 4: {
		std::cout << " Mace" << endl;
		weaponType = weaponType + 4;
		wCollect.push_back(weaponType);
		break;
	}
	case 5: {
		std::cout << " Staff" << endl;
		weaponType = weaponType + 5;
		wCollect.push_back(weaponType);
		break;
	}
	case 6: {
		std::cout << " Broadsword" << endl;
		weaponType = weaponType + 6;
		wCollect.push_back(weaponType);
		break;
	}
	default:
		std::cout << " Knife" << endl;
		weaponType = weaponType + 0;
		wCollect.push_back(weaponType);
		break;
	}
	Sleep(1500);
	system("cls");
	Ride();
}

//determines the probabilities of a weapon to spawn with a modifier from 1-20. 20 being the least likely.
void weaponModifierSpawnRNG() {
	unsigned int rng = 0;
	rng = rand() % 100 + 1;
	if ((rng > 0) && (rng <= 12)) {
		std::cout << "You got a +1";
		weaponType = 10;
		weaponTypeRNG();
	}
	else if ((rng > 12) && (rng <= 24)) {
		std::cout << "You got a +2";
		weaponType = 20;
		weaponTypeRNG();
	}
	else if ((rng > 24) && (rng <= 36)) {
		std::cout << "You got a +3";
		weaponType = 30;
		weaponTypeRNG();
	}
	else if ((rng > 36) && (rng <= 46)) {
		std::cout << "You got a +4";
		weaponType = 40;
		weaponTypeRNG();
	}
	else if ((rng > 46) && (rng <= 56)) {
		std::cout << "You got a +5";
		weaponType = 50;
		weaponTypeRNG();
	}
	else if ((rng > 56) && (rng <= 66)) {
		std::cout << "You got a +6";
		weaponType = 60;
		weaponTypeRNG();
	}
	else if ((rng > 66) && (rng <= 70)) {
		std::cout << "You got a +7";
		weaponType = 70;
		weaponTypeRNG();
	}
	else if ((rng > 70) && (rng <= 74)) {
		std::cout << "You got a +8";
		weaponType = 80;
		weaponTypeRNG();
	}
	else if ((rng > 74) && (rng <= 78)) {
		std::cout << "You got a +9";
		weaponType = 90;
		weaponTypeRNG();
	}
	else if ((rng > 78) && (rng <= 81)) {
		std::cout << "You got a +10";
		weaponType = 100;
		weaponTypeRNG();
	}
	else if ((rng > 81) && (rng <= 84)) {
		std::cout << "You got a +11";
		weaponType = 110;
		weaponTypeRNG();
	}
	else if ((rng > 84) && (rng <= 87)) {
		std::cout << "You got a +12";
		weaponType = 120;
		weaponTypeRNG();
	}
	else if ((rng > 87) && (rng <= 90)) {
		std::cout << "You got a +13";
		weaponType = 130;
		weaponTypeRNG();
	}
	else if ((rng > 90) && (rng <= 92)) {
		std::cout << "You got a +14";
		weaponType = 140;
		weaponTypeRNG();
	}
	else if ((rng > 92) && (rng <= 94)) {
		std::cout << "You got a +15";
		weaponType = 150;
		weaponTypeRNG();
	}
	else if ((rng > 94) && (rng <= 96)) {
		std::cout << "You got a +16";
		weaponType = 160;
		weaponTypeRNG();
	}
	else if ((rng > 96) && (rng <= 97)) {
		std::cout << "You got a +17";
		weaponType = 170;
		weaponTypeRNG();
	}
	else if ((rng > 97) && (rng <= 98)) {
		std::cout << "You got a +18";
		weaponType = 180;
		weaponTypeRNG();
	}
	else if ((rng > 98) && (rng <= 99)) {
		std::cout << "You got a +19";
		weaponType = 190;
		weaponTypeRNG();
	}
	else if ((rng > 99) && (rng <= 100)) {
		std::cout << "You got a +20";
		weaponType = 200;
		weaponTypeRNG();
	}
}

//Selection sort algorithm from book, sorts things by value
void SelectionSort(vector<int> &vec) {
	int startScan, minIndex, minValue;

	for (startScan = 0; startScan < (vec.size() - 1); startScan++) {
		minIndex = startScan;
		minValue = vec[startScan];
		for (int inDEX = startScan + 1; inDEX < vec.size(); inDEX++) {
			if (vec[inDEX] < minValue) {
				minValue = vec[inDEX];
				minIndex = inDEX;
			}
		}
		vec[minIndex] = vec[startScan];
		vec[startScan] = minValue;
	}
}

//binary search (yet to implement)
int binSearch(vector<int> &vecT, int value) {
	int first = 0,
		last = vecT.size() - 1,
		middle,
		position = -1;
	bool found = false;

	while ((!(found)) && (first <= last)) {
		middle = (first + last) / 2;
		if (vecT[middle] == value) {
			found = true;
			position = middle;
		}
		else if (vecT[middle] > value) {
			last = middle - 1;
		}
		else {
			first = middle + 1;
		}
	}
	return position;
}

//lists the weapons in your collection
void displayWeaponCollection() {
	if (wCollect.size() > 0) {
		SelectionSort(wCollect);
		std::cout << "Weapon inventory: " << endl;
		for (int i = 0; i < wCollect.size(); i++) {
			std::cout << "Weapon index [" << i << "]		Weapon: ";
			weaponNameFinder(i);
			if (wCollect[i] == P1.equipped) {
				std::cout << " (equipped)" << endl;
			}
		}
		std::cout << "\n==========================================================================================================\n";
	}
	else {
		std::cout << "Weapon inventory is empty";
	}

}

//Allows one to return to menu or return to game
void checkWeaponCollection() {
	displayWeaponCollection();
	std::cout << "\n\nWhat do you wish to do? \n1:Return to Menu\n2:Return to Game\n3:Equip a Weapon" << endl;
	unsigned short int userInput;
	cin >> userInput;
	while ((userInput != 1) || (userInput != 2) || (userInput != 3)) {
		if (cin.fail()) {
			Sleep(150);
			system("cls");
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			errorDialogue();
			Sleep(1000);
			system("cls");
			displayWeaponCollection();
			std::cout << "What do you wish to do? \n1:Return to Menu\n2:Return to Game\n3:Equip a Weapon" << endl;
			cin >> userInput;
		}
		else if (userInput == 1) {
			if (P1.get_inBattle() == 0) {
				system("cls");
				Menu();
			}
			else {
				system("cls");
				battleMenu();
			}
		}
		else if (userInput == 2) {
			if (P1.get_inBattle() == 0) {
				system("cls");
				displayMap();
				std::cout << "\nWhat will you do? \n" << endl;
				std::cout << "1:Ride horse and travel" << endl;
				std::cout << "2:Dismount horse and carefully explore the surroundings" << endl;
				std::cout << "3:'Menu'" << endl;
				rideOrExplore();
			}
			else {
				system("cls");
				P1.battle();
			}
		}
		else if (userInput == 3) {
			if (wCollect.size() > 0) {
				system("cls");
				displayWeaponCollection();
				std::cout << "\nType the index of the weapon you wish to equip: " << endl;
				int val;
				cin >> val;
				while ((val >= wCollect.size()) || (val < 0)) {
					if (cin.fail()) {
						system("cls"); //clears log
						cin.clear(); // clears error flags
						errorDialogue();
						checkWeaponCollection();
					}
					else if ((val >= wCollect.size()) || (val < 0)) {
						system("cls");
						cin.clear();
						errorDialogue();
						checkWeaponCollection();
					}
				}
				P1.turnOrder();
				playerHPCheck();
				binSearch(wCollect, val);
				system("cls");
				std::cout << "You equipped the ";
				equippingWeapon = true;
				weaponNameFinder(val);
				equippingWeapon = false;
				Sleep(1000);
				system("cls");
				if (P1.get_inBattle() == 0) {
					displayMap();
					std::cout << "What will you do? \n" << endl;
					std::cout << "1:Ride horse and travel" << endl;
					std::cout << "2:Dismount horse and carefully explore the surroundings" << endl;
					std::cout << "3:'Menu'" << endl;
					rideOrExplore();
				}
				else {
					if (playerTurn) {
						monsterattack();
						playerHPCheck();
					}
					P1.battle();
				}
			}
			else {
				system("cls"); //clears log
				std::cout << "Option not available at the moment. Returning to Previous screen";
				Sleep(1500);
				system("cls"); //clears log
				cin.clear(); // clears error flags
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				if (P1.get_inBattle() == 0) {
					Menu();
				}
				else {
					battleMenu();
				}
			}
		}
		else if ((userInput > 3) || (userInput < 1)) {
			Sleep(150);
			system("cls");
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			errorDialogue();
			Sleep(1000);
			system("cls");
			displayWeaponCollection();
			std::cout << "What do you wish to do? \n1:Return to Menu\n2:Return to Game\n3:Equip a Weapon" << endl;
			cin >> userInput;
		}
	}
}

void WeaponNameSwitch() {
	switch (weaponName)
	{
	case 1: {
		std::cout << "Crossbow ";
		break;
	}
	case 2: {
		std::cout << "Flail ";
		break;
	}
	case 3: {
		std::cout << "Whip ";
		break;
	}
	case 4: {
		std::cout << "Mace ";
		break;
	}
	case 5: {
		std::cout << "Staff ";
		break;
	}
	case 6: {
		std::cout << "Broadsword ";
		break;
	}
	default:
		std::cout << "Knife ";
		break;
	}
}

//finds the weapon's modifier and type and outputs a message corresponding to the type and mod.
void weaponNameFinder(int index) {
	if (equippingWeapon) {
		P1.equipped = index;
	}
	int digits = 0;
	int temp = wCollect[index];
	while (temp > 0) {
		temp = temp / 10;
		if (temp > 0) {
			digits++;
		}
	}
	if (digits == 2) {
		singleDigitModifier = false;
	}
	else if (digits == 1) {
		singleDigitModifier = true;
	}

	char firstVal;
	char secondVal;
	char thirdVal;
	int weaponMod;
	if (singleDigitModifier) {
		firstVal = to_string(wCollect[index]).at(0);
		secondVal = to_string(wCollect[index]).at(1);
		weaponMod = firstVal - '0';
		if (equippingWeapon) {
			P1.weaponMod = weaponMod;
		}
		weaponName = secondVal - '0';
	}
	else {
		firstVal = to_string(wCollect[index]).at(0);
		secondVal = to_string(wCollect[index]).at(1);
		thirdVal = to_string(wCollect[index]).at(2);
		weaponMod = firstVal - '0';
		weaponMod = (weaponMod * 10) + (secondVal - '0'); //I multiply the first value by 10 because if it's a one then 1*10 = 10 + second val = whatever the actual modifier is.
		if (equippingWeapon) {
			P1.weaponMod = weaponMod;
		}
		weaponName = thirdVal - '0';
	}
	WeaponNameSwitch();
	std::cout << "+ " << weaponMod << endl;
}

//A battle version of the Menu
void battleMenu() {
	std::cout << "Which Inventory would you wish to access?" << endl;
	std::cout << "1:Weapons Inventory\n2:Items Inventory\n3:Return to battle" << endl;
	short int userInput;
	cin >> userInput;
	while ((userInput != 1) || (userInput != 2) || (userInput != 3)) {
		if (cin.fail()) {
			Sleep(150);
			system("cls"); //clears log
			cin.clear(); // clears error flags
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			errorDialogue();
			std::cout << "Which Inventory would you wish to access?" << endl;
			std::cout << "1:Weapons Inventory\n2:Items Inventory\n3:Return to battle" << endl;
			cin >> userInput;
		}
		else if (userInput == 1) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			system("cls");
			checkWeaponCollection();
		}
		else if (userInput == 2) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			system("cls");
			checkItemInventory();
		}
		else if (userInput == 3) {
			system("cls");
			P1.battle();
		}
		else if ((userInput > 3) || (userInput < 1)) {
			Sleep(150);
			system("cls");
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			errorDialogue();
			std::cout << "Which Inventory would you wish to access?" << endl;
			std::cout << "1:Weapons Inventory\n2:Items Inventory\n3:Return to battle" << endl;
			cin >> userInput;
		}
	}
}

void shopThing() {
	playerXPCheck();
	int bonfireXP = P1.get_PlayerXP();
	shopDisplay();
	short int userInput;
	cin >> userInput;
	while (userInput != 5) {
		if (cin.fail()) {
			Sleep(150);
			system("cls");
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			errorDialogue();
			Sleep(1500);
			system("cls");
			shopDisplay();
			cin >> userInput;
		}
		else if (userInput == 1) {
			system("cls");
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			if (((P1.get_PlayerXP() - P1.get_PlayerRequiredXP()) > 0) && (P1.get_PlayerXP() >= P1.get_PlayerRequiredXP())) {
				P1.set_PlayerXP(P1.get_PlayerXP() - P1.get_PlayerRequiredXP());
				P1.set_PlayerRequiredXP(5 * (P1.get_PlayerLevel() + 1));
				P1.set_PlayerVitality(P1.get_PlayerVitality() + 1);
				P1.set_PlayerLevel(P1.get_PlayerLevel() + 1);
			}
			else {
				system("cls");
				std::cout << "you don't have the XP to level up";
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				Sleep(2000);
				system("cls");
				break;
			}
			shopThing();
		}
		else if (userInput == 2) {
			system("cls");
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			if (((P1.get_PlayerXP() - P1.get_PlayerRequiredXP()) > 0) && (P1.get_PlayerXP() >= P1.get_PlayerRequiredXP())) {
				P1.set_PlayerXP(P1.get_PlayerXP() - P1.get_PlayerRequiredXP());
				P1.set_PlayerRequiredXP(5 * (P1.get_PlayerLevel() + 1));
				P1.set_PlayerStrength(P1.get_PlayerStrength() + 1);
				P1.set_PlayerLevel(P1.get_PlayerLevel() + 1);
			}
			else {
				system("cls");
				std::cout << "you don't have the XP to level up";
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				Sleep(2000);
				system("cls");
				break;
			}
			shopThing();
		}
		else if (userInput == 3) {
			system("cls");
			if (((P1.get_PlayerXP() - P1.get_PlayerRequiredXP()) > 0) && (P1.get_PlayerXP() >= P1.get_PlayerRequiredXP())) {
				P1.set_PlayerXP(P1.get_PlayerXP() - P1.get_PlayerRequiredXP());
				P1.set_PlayerRequiredXP(5 * (P1.get_PlayerLevel() + 1));
				P1.set_PlayerEndurance(P1.get_PlayerEndurance() + 1);
				P1.set_PlayerLevel(P1.get_PlayerLevel() + 1);
			}
			else {
				system("cls");
				std::cout << "you don't have the XP to level up";
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				Sleep(2000);
				system("cls");
				break;
			}
			shopThing();
		}
		else if (userInput == 4) {
			system("cls");
			if (((P1.get_PlayerXP() - P1.get_PlayerRequiredXP()) > 0) && (P1.get_PlayerXP() >= P1.get_PlayerRequiredXP())) {
				P1.set_PlayerXP(P1.get_PlayerXP() - P1.get_PlayerRequiredXP());
				P1.set_PlayerRequiredXP(5 * (P1.get_PlayerLevel() + 1));
				P1.set_PlayerSpeed(P1.get_PlayerSpeed() + 1);
				P1.set_PlayerLevel(P1.get_PlayerLevel() + 1);
			}
			else {
				system("cls");
				std::cout << "you don't have the XP to level up";
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				Sleep(2000);
				system("cls");
				break;
			}
			shopThing();
		}
		else if (userInput == 5) {
			break;
		}
		else if ((userInput > 5) || (userInput < 1)) {
			Sleep(150);
			system("cls");
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			errorDialogue();
			Sleep(1500);
			system("cls");
			shopDisplay();
			cin >> userInput;
		}
	}
	std::cout << "Well rested and with new determination you return to your quest" << endl;
	Sleep(1500);
	system("cls");
	Ride();
}
void shopDisplay() {
	std::cout << "You find yourself where your journey started" << endl;
	std::cout << "You rest by the bonfire, allowing you to level up" << endl;
	std::cout << "Player Level: " << P1.get_PlayerLevel() << endl;
	std::cout << "XP required to Level Up: " << P1.get_PlayerRequiredXP() << endl;
	std::cout << "Current XP " << P1.get_PlayerXP() << endl;
	std::cout << "Level up:\n1: Vitality " << P1.get_PlayerVitality() << endl;
	std::cout << "2: Strength " << P1.get_PlayerStrength() << endl;
	std::cout << "3: Endurance " << P1.get_PlayerEndurance() << endl;
	std::cout << "4: Speed " << P1.get_PlayerSpeed() << endl;
	std::cout << "=============================================================================================\n";
	std::cout << "5: Return to Journey" << endl;
}
//to implement durability, make sure to change the weapon name and type to something like 99206 -> 99 being current durability -> 20 being weapon modifier -> 6 being weapon Name;
//why add durability as first two digits? because every hit can be weaponType = weaponType -1000; and add an if(weaponType<1000){ destroy weapon };