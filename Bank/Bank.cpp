#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

const string FileNameClints = "Clints.txt";

void ShowMainScreen();
void ShoTransactionsMenueScreen();

struct sClint 
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	bool MarkForDelete = false;
	double AccountBalance = 0;
};

vector <string> SplitString(string S, string Delim)
{
	size_t pos = 0;
	string sWord;
	vector <string> vString;
	while ((pos = S.find(Delim)) != std::string::npos)
	{
		sWord = S.substr(0, pos);
		if (sWord != "")
		{
			vString.push_back(sWord);
		}
		S.erase(0, pos + Delim.length());
	}
	if (S != "")
	{
		vString.push_back(S);
	}
	return vString;
}

sClint ConvertLineToRecord(string stLine, string Parameter = "/##/")
{
	sClint Clint;
	vector <string> vClint = SplitString(stLine, Parameter);

	Clint.AccountNumber = vClint[0];
	Clint.PinCode = vClint[1];
	Clint.Name = vClint[2];
	Clint.Phone = vClint[3];
	Clint.AccountBalance = stod(vClint[4]);

	return Clint;
}

string ConvertRecordToLine(sClint Clint , string Seprator = "/##/")
{
	string DataLine;

	DataLine = Clint.AccountNumber + Seprator;
	DataLine += Clint.PinCode + Seprator;
	DataLine += Clint.Name + Seprator;
	DataLine += Clint.Phone + Seprator;
	DataLine += to_string(Clint.AccountBalance);

	return DataLine;
}

vector <sClint> LoadFromFileClints(string FileName)
{
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	vector <sClint> vClint;

	if (MyFile.is_open())
	{
		sClint Clint;
		string Line;
		while (getline(MyFile, Line))
		{
			Clint = ConvertLineToRecord(Line);
			vClint.push_back(Clint);
		}
		MyFile.close();
	}
	return vClint;
}

void PrintHeaderShowClints()
{
	cout << " | " << left << setw(14) << "Account Number";
	cout << " | " << left << setw(10) << "Pin Code ";
	cout << " | " << left << setw(30) << "Client Name ";
	cout << " | " << left << setw(11) << "Phone";
	cout << " | " << left << setw(15) << "Balance ";
	cout << "\n\n-------------------------------------------------------------------";
	cout << "------------------------------------------------\n\n";

}

void PrinClint(sClint Clint)
{
	cout << " | " << left << setw(14) << Clint.AccountNumber;
	cout << " | " << left << setw(10) << Clint.PinCode;
	cout << " | " << left << setw(30) << Clint.Name;
	cout << " | " << left << setw(11) << Clint.Phone;
	cout << " | " << left << setw(15) << Clint.AccountBalance;
}

bool ClintExitByAccountNumber(string AccountNumber, string FileName)
{
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	vector <sClint> vClint;

	if (MyFile.is_open())
	{
		string Line;
		sClint Clint;
		
		while (getline(MyFile, Line))
		{
			Clint = ConvertLineToRecord(Line);
			if (Clint.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
			vClint.push_back(Clint);
		}
		MyFile.close();
	}
	return false;

}

sClint ReadNewClint()
{
	sClint Client;
	cout << "Enter Your Account Number : ";
	getline(cin >> ws, Client.AccountNumber);
	while (ClintExitByAccountNumber(Client.AccountNumber, FileNameClints))
	{
		cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
		getline(cin >> ws, Client.AccountNumber);
	}
	cout << "\nPin Code : ";
	getline(cin, Client.PinCode);
	cout << "\nName : ";
	getline(cin, Client.Name);
	cout << "\nPhone : ";
	getline(cin, Client.Phone);
	cout << "\nAccount Balance : ";
	cin >> Client.AccountBalance;
	return Client;
}

void SaveClientToFile(string FileName, string stLine)
{
	fstream MyFile;
	MyFile.open(FileName, ios::app | ios::out);
	if (MyFile.is_open())
	{
		MyFile << stLine << endl;
		MyFile.close();
	}
}

void AddNewClient()
{
	sClint Client;
	Client = ReadNewClint();
	SaveClientToFile(FileNameClints, ConvertRecordToLine(Client));
}

void AddNewClints()
{
	cout << "\n\n\t\t\tAdd New Clints\n\n";
	cout << "------------------------------------------------------------\n"; 
	char AddMore = 'Y';
	do {       
		cout <<"Adding New Client:\n\n";         
		AddNewClient();         
		cout <<"\nClient Added Successfully, do you want to add more clients? Y/N? ";         
		cin >> AddMore;
	} while (toupper(AddMore) == 'Y'); 
}

void ShowClintsList()
{
	vector <sClint> vClint = LoadFromFileClints(FileNameClints);
	cout << "\n\n\t\t\t\tClint List (" << vClint.size() << ") Clint(s). \n";
	cout << "\n\n-------------------------------------------------------------------";
	cout << "------------------------------------------------\n\n";
	
	if (vClint.size() == 0)
	{
		cout << "\t\t\tClients Available In the System!\n";
	}
	else
	{
		PrintHeaderShowClints();
		for (sClint C : vClint)
		{
			PrinClint(C);
			cout << endl;
		}
		cout << "\n\n-------------------------------------------------------------------";
		cout << "------------------------------------------------\n\n";
	}
}

enum enTransactionsMenueOptions
{
	enDeposit = 1, enWithdraw =2,
	enTotalBalance = 3, enMainMenue = 4
};

enum enMainMenueOptions { 
	enShowListClints = 1, enAddNewClints = 2,
	enDleleteClint = 3, enUpdateClint = 4,
	enFindClint = 5, enTransactions = 6,
	enExit = 7
};

short ReadMainOptions()
{
	cout << "\nChoose what do you want to do ? [1 to 7] ? ";
	short Choose;
	cin >> Choose;
	return Choose;
}

void GoToBack()
{
	cout << "\nPress any key to go back to Main Menue .....";
	system("pause>0");
	ShowMainScreen();
}

bool FindClientByAccountNumber(string AccountNumber, vector <sClint> & vClients, sClint & Client)
{
	for (sClint C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

void PrintClientCard(sClint Client)
{
	cout << "the Following are the Client details :\n";
	cout << "----------------------------------------\n";
	cout << "Account Number  : " << Client.AccountNumber << endl;
	cout << "Pin Code        : " << Client.PinCode << endl;
	cout << "Name            : " << Client.Name << endl;
	cout << "Phone           : " << Client.Phone << endl;
	cout << "Account Balance : " << Client.AccountBalance << endl;
	cout << "----------------------------------------\n";
}

string ReadAccountNumber()
{
	cout << "Enter Account Number : ";
	string AccountNumber;
	cin >> AccountNumber;
	cout << endl;
	return AccountNumber;
}

bool MarkForDeleteByAccountNumber(string AccountNumber, vector <sClint>& vClints)
{
	for (sClint& C : vClints)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

void SaveToFile(string FileName, vector <sClint> vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);

	string Line;
	if (MyFile.is_open())
	{
		for (sClint C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				Line = ConvertRecordToLine(C);
				MyFile << Line << endl;
			}
		}
		MyFile.close();
	}
}

bool DeleteClintByAccountNumber()
{
	cout << "\n\n\t\t\tDelete Client \n\n";
	cout << "-------------------------------------------------------\n\n";
	cout << "the folowing Client details \n\n";
	sClint Client;
	vector <sClint> vClients = LoadFromFileClints(FileNameClints);
	string AccountNumber = ReadAccountNumber();
	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);

		char Answer = 'Y';
		cout << "\n\nDo You Want to Delete  Thie Client ? y / n ? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			MarkForDeleteByAccountNumber(AccountNumber, vClients);
			SaveToFile(FileNameClints, vClients);

			//Refresh File

			vClients = LoadFromFileClints(FileNameClints);
			return true;
		}

	}
	else
	{
		cout << "\n\nNot Found Clinet !\n\n";
	}
		return false;
}

sClint UpdateClintByAccountNumber(string AccountNumber)
{
	sClint Client;
	Client.AccountNumber = AccountNumber;

	cout << "\nPin Code : ";
	getline(cin >> ws, Client.PinCode);
	cout << "\nName : ";
	getline(cin, Client.Name);
	cout << "\nPhone : ";
	getline(cin, Client.Phone);
	cout << "\nAccount Balance : ";
	cin >> Client.AccountBalance;
	return Client;
}

bool FindClintByAccountNumber(string AccountNumber, vector <sClint> & vClients, sClint & Client)
{
	
	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);
		return true;
	}
	else
	{
		cout << "\n\nNot Found Clinet !\n\n";
		return false;
	}
}

bool UpdateClintByAccountNumber(string AccountNumber, vector <sClint>& vClients, sClint& Client)
{

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);

		char Answer = 'Y';
		cout << "\n\nDo You Want to Update  Thie Client ? y / n ? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			MarkForDeleteByAccountNumber(AccountNumber, vClients);

			for (sClint& C : vClients)
			{
				if (AccountNumber == C.AccountNumber)
				{
					C = UpdateClintByAccountNumber(AccountNumber);
					break;
				}
			}
			SaveToFile(FileNameClints, vClients);

			//Refresh File

			vClients = LoadFromFileClints(FileNameClints);
			return true;
		}

	}
	else
	{
		cout << "\n\nNot Found Clinet !\n\n";
	}
		return false;
}

void ShowUpdateClintByAccountNumber()
{
	cout << "\n\n\t\t\tUpdate Client \n\n";
	cout << "-------------------------------------------------------\n\n";
	cout << "the folowing Client details \n\n";
	sClint Client;
	vector <sClint> vClients = LoadFromFileClints(FileNameClints);
	string AccountNumber = ReadAccountNumber();
	UpdateClintByAccountNumber(AccountNumber, vClients,Client);
}

void ShowFindClintByAccountNumber()
{
	cout << "\n\n\t\tFind Client \n\n";
	cout << "-------------------------------------------------------\n\n";
	cout << "the folowing Client details \n\n";
	sClint Client;
	vector <sClint> vClients = LoadFromFileClints(FileNameClints);
	string AccountNumber = ReadAccountNumber();
	FindClintByAccountNumber(AccountNumber, vClients, Client);
}

void ShowEnd()
{
	cout << "\n-------------------------------------------\n";
	cout << "\tEnd Program !\n";
	cout << "\n-------------------------------------------\n";

}

void GoToBackTransactions()
{
	cout << "\nPress any key to go back to Main Menue .....";
	system("pause>0");
	ShoTransactionsMenueScreen();
}

double ReadDepositAmount()
{
	double DepositAmount;
	cout << "Please Enter Deposit amount ? ";
	cin >> DepositAmount;
	return DepositAmount;
}

vector <sClint> DepositAmountClint(string AccountNumber, vector <sClint> &vClients, double DepositAmount)
{
	for (sClint &C : vClients)
	{
		if(C.AccountNumber == AccountNumber)
		{
			C.AccountBalance = C.AccountBalance + DepositAmount;
			break;
		}
	}
	return vClients;
}

void ProccesTransactiosDeposit(string AccountNumber, vector <sClint> &vClients, sClint &Client)
{
	if(FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);
		double DepositAmount = ReadDepositAmount();

		cout << "\nAre you sure want perfrom this transaction? y / n ? ";
		char Answer;
		cin >> Answer;
		if(Answer == 'y' || Answer == 'Y')
		{
			DepositAmountClint(AccountNumber, vClients, DepositAmount);
			SaveToFile(FileNameClints, vClients);
			vClients = LoadFromFileClints(FileNameClints);
		}
	}
	else 
	{
		cout << "\nNot Found Client !\n\n";
	}
}

void ShowTransactiosDeposit()
{
	cout << "\n----------------------------------------------------------\n";
	cout << "\n\t\t\tDeposit Screen\n";
	cout << "\n----------------------------------------------------------\n";
	string AccountNumber = ReadAccountNumber();
	sClint Client;
	vector <sClint> vClients = LoadFromFileClints(FileNameClints);
	ProccesTransactiosDeposit(AccountNumber, vClients, Client);
}

void PerForTransactionsMenueOptions(enTransactionsMenueOptions TransactionsMenueOption)
{
	switch(TransactionsMenueOption)
	{
		case enTransactionsMenueOptions::enDeposit:
			system("cls");
			ShowTransactiosDeposit();
			GoToBackTransactions();
			break;

		case enTransactionsMenueOptions::enWithdraw:
			system("cls");
			GoToBackTransactions();
			break;

		case enTransactionsMenueOptions::enTotalBalance:
			system("cls");
			GoToBackTransactions();
			break;

		case enTransactionsMenueOptions::enMainMenue:
			ShowMainScreen();
			break;
	}

}

void PerForMainMenueOptions(enMainMenueOptions MainMenueOptions)
{
	switch (MainMenueOptions)
	{
		case enMainMenueOptions::enShowListClints:
			system("cls");
			ShowClintsList();
			GoToBack();
			break;
		case enMainMenueOptions::enAddNewClints:
			system("cls");
			AddNewClints();
			GoToBack();
			break;
		case enMainMenueOptions::enDleleteClint:
			system("cls");
			DeleteClintByAccountNumber();
			GoToBack();
			break;
		case enMainMenueOptions::enUpdateClint:
			system("cls");
			ShowUpdateClintByAccountNumber();
			GoToBack();
			break;
		case enMainMenueOptions::enFindClint :
			system("cls");
			ShowFindClintByAccountNumber();
			GoToBack();
			break;
		case enMainMenueOptions :: enTransactions:
			system("cls");
			ShoTransactionsMenueScreen();
			break;
		case enMainMenueOptions::enExit:
			system("cls");
			ShowEnd();
			break;


	}
}

short ReadChoose()
{
	cout << "Choose What do You Want to do ? [1 To 4 ] ? ";
	short ChooseNumber;
	cin >> ChooseNumber;
	return ChooseNumber;
}

void ShoTransactionsMenueScreen()
{
	system("cls");
	cout << "=====================================================";
	cout << "\n\n\t\tTransActions Menue Screen\n\n";
	cout << "=====================================================\n\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] Total Balannce.\n";
	cout << "\t[4] Main Menue.\n";
	cout << "=====================================================\n";
	PerForTransactionsMenueOptions((enTransactionsMenueOptions) ReadChoose());
}

void ShowMainScreen()
{
	system("cls");
	cout << "\n==================================================";
	cout << "==================================================\n\n";
	cout << "\t\t\t\tMain Sreen\n\n";
	cout << "==================================================";
	cout << "==================================================\n\n";
	cout << "\t[1] Show All Clints !\n";
	cout << "\t[2] Add New Clints !\n";
	cout << "\t[3] Dlete Clint !\n";
	cout << "\t[4] Update Clint !\n";
	cout << "\t[5] Find Clint !\n";
	cout << "\t[6] Tansactions !\n";
	cout << "\t[7] Exit !\n";

	cout << "==================================================";
	cout << "==================================================\n";
	PerForMainMenueOptions((enMainMenueOptions)ReadMainOptions());
}

int main()
{

	ShowMainScreen();
	system("pause>0");
}