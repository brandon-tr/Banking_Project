// ConsoleApplication5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <chrono>
#include <tuple>
#include <list>

#define LOG(x)  std::cout << x << std::endl;
using namespace std;

struct AccountStruct
{
	int serial;
	int account;
	string firstName;
	string lastName;
	double balance;
	bool flag;
};

const int rowSize = 20;
const int columnSize = 6;

int getArrayIndexOfAccount(int accountNumber, struct AccountStruct accounts[rowSize][columnSize]) {
	for (int i = 0; i < 20; i++)
	{
		if (accountNumber == accounts[i]->account)
		{
			LOG(i)
			return i;
		}
	}
	return -1;
}

tuple<int, int, int, int, int, int>current_datetime()
{
	auto start = chrono::system_clock::now();
	struct tm t;
	time_t now = time(0);
	localtime_s(&t, &now);
	int month = 1 + t.tm_mon;
	int year = 1900 + t.tm_year;
	int day = t.tm_mday;
	int hour = t.tm_hour;
	int minute = t.tm_min;
	int second = t.tm_sec;
	return make_tuple(month, day, year, hour, minute, second);
}


void depositMoney(int index, int amount, struct AccountStruct accounts[rowSize][columnSize]) {
	ofstream outFile;
	ifstream inFile;
	string line;
	list<string> oldFileList;
	for (size_t i = 0; i < 6; i++)
	{
		if (index == i)
		{
			string fileName = to_string(accounts[i]->account);
			int month, day, year, hours, minutes, seconds;
			tie(month, day, year, hours, minutes, seconds) = current_datetime();
			string date = to_string(month) + "/" + to_string(day) + "/" + to_string(year) + "     " + to_string(hours) + ":" +to_string(minutes) + ":" + to_string(seconds);
			

			inFile.open(fileName + ".txt");

			while (getline(inFile, line))
			{
				if (line.length() > 0)
				{
					oldFileList.emplace_back(line);
				}
			}
			inFile.close();
			outFile.open(fileName + ".txt");
			if (oldFileList.size() > 0)
			{
				for (auto const& i : oldFileList)
				{
					outFile << i;
					outFile << "\n";
				}
			}		
			outFile << "You deposited " + to_string(amount) + "      " + date;
			outFile.close();


			outFile.open("list.txt");
			accounts[i]->balance += amount;
			for (size_t i = 0; i < 6; i++)
			{
				outFile << accounts[i]->serial;
				outFile << " ";
				outFile << accounts[i]->account;
				outFile << " ";
				outFile << accounts[i]->firstName;
				outFile << " ";
				outFile << accounts[i]->lastName;
				outFile << " ";
				outFile << accounts[i]->balance;
				outFile << " ";
				outFile << accounts[i]->flag;
				outFile << "\n";
			}
			outFile.close();
		}
	}
}

void withdrawMoney(int index, int amount, struct AccountStruct accounts[rowSize][columnSize]) {
	ofstream outFile;
	ifstream inFile;
	string line;
	list<string> oldFileList;
	if (accounts[index]->balance >= amount)
	{
		string fileName = to_string(accounts[index]->account);
		int month, day, year, hours, minutes, seconds;
		tie(month, day, year, hours, minutes, seconds) = current_datetime();
		string date = to_string(month) + "/" + to_string(day) + "/" + to_string(year) + "     " + to_string(hours) + ":" + to_string(minutes) + ":" + to_string(seconds);


		inFile.open(fileName + ".txt");

		while (getline(inFile, line))
		{
			if (line.length() > 0)
			{
				oldFileList.emplace_back(line);
			}
		}
		inFile.close();
		outFile.open(fileName + ".txt");
		if (oldFileList.size() > 0)
		{
			for (auto const& i : oldFileList)
			{
				outFile << i;
				outFile << "\n";
			}
		}
		outFile << "You Withdrew " + to_string(amount) + "      " + date;
		outFile.close();
		outFile.open("list.txt");
		accounts[index]->balance -= amount;
		for (size_t i = 0; i < rowSize; i++)
		{
			outFile << accounts[i]->serial;
			outFile << " ";
			outFile << accounts[i]->account;
			outFile << " ";
			outFile << accounts[i]->firstName;
			outFile << " ";
			outFile << accounts[i]->lastName;
			outFile << " ";
			outFile << accounts[i]->balance;
			outFile << " ";
			outFile << accounts[i]->flag;
			outFile << "\n";
		}
		outFile.close();
	}
	else
	{
		LOG("Not enough Money")
	}
}

void transferMoney(int indexFirstAccount, int indexSecondAccount, int amount, struct AccountStruct accounts[rowSize][columnSize]) {
	if (accounts[indexFirstAccount]->balance >= amount)
	{
		withdrawMoney(indexFirstAccount, amount, accounts);
		depositMoney(indexSecondAccount, amount, accounts);
	}
	else
	{
		cout << "Error not enough money" << endl;
	}
}

void showTransactions(int index, struct AccountStruct account[rowSize][columnSize]) {
	ifstream inFile;
	list<string> oldFileList;
	string line;
	string fileName = to_string(account[index]->account);
	inFile.open(fileName + ".txt");
	if (inFile.fail())
	{
		cerr << "Failed to open text file" << endl;
	}
	else
	{
		while (getline(inFile, line))
		{
			oldFileList.emplace_back(line);
		}
		for (auto const& i : oldFileList)
		{
			cout << i << endl;
		}
		inFile.close();
	}
}

void openAccount(struct AccountStruct account[rowSize][columnSize]) {
	ofstream outFile;
	ifstream inFile;
	string line;

	int accountNumber;
	string firstName;
	string lastName;
	double balance;
	int flag = 0;

	cin >> accountNumber;
	cin >> firstName;
	cin >> lastName;
	cin >> balance;

	outFile.open("list.txt");
	for (int i = 0; i < rowSize; i++)
	{
		if (account[i]->account != 0)
		{
			outFile << account[i]->serial;
			outFile << " ";
			outFile << account[i]->account;
			outFile << " ";
			outFile << account[i]->firstName;
			outFile << " ";
			outFile << account[i]->lastName;
			outFile << " ";
			outFile << account[i]->balance;
			outFile << " ";
			outFile << account[i]->flag;
			outFile << "\n";
			if (!account[i + 1]->account)
			{
				account[i + 1]->account = accountNumber;
				account[i + 1]->balance = balance;
				account[i + 1]->firstName = firstName;
				account[i + 1]->lastName = lastName;
				account[i + 1]->flag = flag;
				account[i + 1]->serial = i + 2;
				outFile << account[i + 1]->serial;
				outFile << " ";
				outFile << account[i + 1]->account;
				outFile << " ";
				outFile << account[i + 1]->firstName;
				outFile << " ";
				outFile << account[i + 1]->lastName;
				outFile << " ";
				outFile << account[i + 1]->balance;
				outFile << " ";
				outFile << account[i + 1]->flag;
				outFile << "\n";
				break;
			}
		}
	}
	outFile.close();

	string fileName = to_string(accountNumber);
	int month, day, year, hours, minutes, seconds;
	tie(month, day, year, hours, minutes, seconds) = current_datetime();
	string date = to_string(month) + "/" + to_string(day) + "/" + to_string(year) + "     " + to_string(hours) + ":" + to_string(minutes) + ":" + to_string(seconds);
	
	outFile.open(fileName + ".txt");
	outFile << "You opened a new account with the balance of  " + to_string(balance) + " " + date;
	outFile << "\n";

	outFile.close();
	/*outFile.open("list.txt");
	for (size_t i = 0; i < rowSize; i++)
	{
		outFile << account[i]->serial;
		outFile << " ";
		outFile << account[i]->account;
		outFile << " ";
		outFile << account[i]->firstName;
		outFile << " ";
		outFile << account[i]->lastName;
		outFile << " ";
		outFile << account[i]->balance;
		outFile << " ";
		outFile << account[i]->flag;
		outFile << "\n";
	}
	outFile.close();*/
}

int main()
{
	/* Initial Setup */
	ifstream inFile;
	inFile.open("list.txt");
	string line;
	string test;
	AccountStruct account[rowSize][columnSize] = { 0 };
	if (inFile.fail())
	{
		cout << "Failed" << endl;
	}
	else
	{
		int i = 0;
		while (getline(inFile, line))
		{
			stringstream stream(line);
			stream >> account[i]->serial;
			stream >> account[i]->account;
			stream >> account[i]->firstName;
			stream >> account[i]->lastName;
			stream >> account[i]->balance;
			stream >> account[i]->flag;
			i++;
		}
	}
	inFile.close();
	/* End Initial Setup */


	//getArrayIndexOfAccount(38531, account);

	//depositMoney(3, 5000, account);
	//withdrawMoney(3, 5000, account);
	//transferMoney(3, 1, 5000, account);
	//showTransactions(2, account);
	openAccount(account);
	system("pause");
 }