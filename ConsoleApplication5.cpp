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
#include <vector>
#include <algorithm>

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

void printOptionsMenu() //display menu
{
	cout << "-----------------------------------------------------------------------------------\n";
	cout << "1. Deposit Money\n";
	cout << "2. Withdraw Money\n";
	cout << "3. Transfer Money\n";
	cout << "4. Show Transactions\n";
	cout << "5. Open an Account\n";
	cout << "6. Close an Account\n";
	cout << "7. Sort Accounts from largest balance to smallest\n";
	cout << "8. Sort Accounts in alphabetical order using the account name\n";
	cout << "9. Quit Program\n";
	cout << "-----------------------------------------------------------------------------------\n";
}

int getArrayIndexOfAccount(int accountNumber, struct AccountStruct accounts[rowSize][columnSize]) {
	for (int i = 0; i < 20; i++)
	{
		if (accountNumber == accounts[i]->account)
		{
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
				if (accounts[i]->account != 0)
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
				else
				{
					break;
				}
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
			if (accounts[i]->account != 0)
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
			else
			{
				break;
			}
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
			LOG(line)
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
}

void closeAccount(int index, struct AccountStruct account[rowSize][columnSize]) {
	
	if (account[index]->balance > 0)
	{
		account[index]->flag = 1;
		ofstream outFile;
		string line;
		{
			outFile.open("list.txt");
			for (size_t i = 0; i < rowSize; i++)
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
				}
				else
				{
					break;
				}
			}
			outFile.close();
		}
	}
	else
	{
		cerr << "Account has a balance greater than 0" << endl;
	}
}


void sortBalances(struct AccountStruct accounts[rowSize][columnSize]) {
	vector<AccountStruct>sortedAccount;
	for (size_t i = 0; i < rowSize; i++)
	{
		if (accounts[i]->account != 0)
		{
			for (size_t j = 0; j < 1; j++)
			{
				sortedAccount.push_back(accounts[i][j]);
			}
		}
	}
	sort(sortedAccount.begin(), sortedAccount.end(), [](const auto& lhs, const auto& rhs)
	{
		return	rhs.balance < lhs.balance;
	});
	for (auto i : sortedAccount)
	{
		cout << i.serial << " " << i.account << " " << i.firstName << " " << i.lastName << " " << i.balance << " " << i.flag << endl;
	}
}

void sortNames(struct AccountStruct accounts[rowSize][columnSize]) {
	vector<AccountStruct>sortedAccount;
	for (size_t i = 0; i < rowSize; i++)
	{
		if (accounts[i]->account != 0)
		{
			for (size_t j = 0; j < 1; j++)
			{
				sortedAccount.push_back(accounts[i][j]);
			}
		}
	}
	sort(sortedAccount.begin(), sortedAccount.end(), [](const auto& lhs, const auto& rhs)
	{
		return lhs.firstName < rhs.firstName;
	});
	for (auto i : sortedAccount)
	{
		cout << i.serial << " " << i.account << " " << i.firstName << " " << i.lastName << " " << i.balance << " " << i.flag << endl;
	}
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

	printOptionsMenu();
	int x, index, accountNumber, amount;
	while (true)
	{
		cin >> x;
		switch (x)
		{
		case 1:
			cin >> accountNumber;
			index = getArrayIndexOfAccount(accountNumber, account);
			cin >> amount;
			depositMoney(index, amount, account);
			break;
		case 2:
			cin >> accountNumber;
			index = getArrayIndexOfAccount(accountNumber, account);
			cin >> amount;
			withdrawMoney(index, amount, account);
			break;
		case 3:
			cin >> accountNumber;
			int secondAccount;
			cin >> secondAccount;
			index = getArrayIndexOfAccount(accountNumber, account);
			cin >> amount;
			transferMoney(accountNumber, secondAccount, amount, account);
			break;
		case 4:
			cin >> accountNumber;
			index = getArrayIndexOfAccount(accountNumber, account);
			showTransactions(index, account);
			break;
		case 5:
			openAccount(account);
			break;
		case 6:
			cin >> accountNumber;
			index = getArrayIndexOfAccount(accountNumber, account);
			closeAccount(accountNumber, account);
			break;
		case 7:
			sortBalances(account);
			break;
		case 8:
			sortNames(account);
			break;
		default:
			system("cls");
			break;
		}
	}
	
	//getArrayIndexOfAccount(38531, account);

	//depositMoney(3, 5000, account);
	//withdrawMoney(3, 5000, account);
	//transferMoney(3, 1, 5000, account);
	//showTransactions(3, account);
	//openAccount(account);
	//closeAccount(3, account);
	//sortBalances(account);
	//sortNames(account);
	system("pause");
 }