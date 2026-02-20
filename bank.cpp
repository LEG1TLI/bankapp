#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <random>
#include <set>
using namespace std;

class Database
{
    public:
        Database()
        {
            loadusrfrmfile();
            loadmngrfrmfile();
        }
    void saveusrtofile()
    {
        ofstream userFile("users.txt");

        for (const auto &user : users)
        {
            userFile << user.first << " " << user.second.first << " " << user.second.second << endl;
        }
        userFile.close();
    }

    bool loadusrfrmfile()
    {
        ifstream userFile("users.txt");
        if (!userFile)
        {
            return false;
        }
        string line;
        while (getline(userFile, line))
        {
            istringstream iss(line);
            string username, password;
            if (iss >> username >> password)
            {
                users[username].first = password;
            }
        }
        return true;
    }

    bool loadmngrfrmfile()
    {
        ifstream mngrFile("managers.txt");
        if (!mngrFile)
        {
            return false;
        }
        string line;
        while (getline(mngrFile, line))
        {
            istringstream iss(line);
            string username, password;
            if (iss >> username >> password)
            {
                managers[username] = password;
            }
        }
        mngrFile.close();
        return true;
    }

    void createusr(const string &username, const string &password)
    {
        users[username] = make_pair(password, 0.0);
        saveusrtofile();
        cout << "User successfully created." << endl;
    }

    void deleteusr(const string &username)
    {
        users.erase(username);
        cout << "user deleted successfully." << endl;
    }

    bool login()
    {
        string username, password;
        cout << "Please enter your username: ";
        cin >> username;
        if (users.find(username) != users.end())
        {
            cout << "Please enter your password: ";
            cin >> password;
            if (users[username].first == password)
            {
                cout << "Login successful." << endl;
                return true;
            }
        }
        cout << "Invalid username or password. Please try again." << endl;
        return false;
    }
    private:
        unordered_map<string, pair<string, double>> users;
        unordered_map<string, string> managers;
};

class Transaction
{
    public:
        string type;
        double amount;
        
        Transaction(const string &t, double a) : type(t), amount(a) {}
};

class BankAccount
{
    public:
        BankAccount(const string &username, Database &db) : username(username), open(true), balance(0), database(db)
        {
            ifstream file("users.txt");
            string line;
            while(getline(file, line))
            {
                istringstream iss(line);
                string storedUsr, storedPwd;
                double storedBlnc;
                if (iss >> storedUsr >> storedPwd)
                {
                    if (storedUsr == username)
                    {
                        balance = storedBlnc;
                        break;
                    }
                }
            }

        }

        void withdraw(double amount)
        {
            if (open && amount <= balance && amount > 0)
            {
                balance -= amount;
                transactions.emplace_back("Withdrawal", amount);
                cout << "Successfully witrdrew $" << amount << "from your account." << endl;
                cout << "Your current balance is $" << balance << "." << endl;
                updateBalanceInFile();
            }
            else
            {
                cout << "Invalid withdrawal amount or insufficient balance." << endl;
            }
        }

        void deposit(double amount)
        {
            if (open && amount > 0)
            {
                balance += amount;
                transactions.emplace_back("Deposit", amount);
                cout << "successfully deposited $" << amount << " to your account." << endl;
                cout << "your current balance is $" << balance << "." << endl;
                updateBalanceInFile();
            }
            else
            {
                cout << "Invalid amount provided or account is closed." << endl;
            }
        }

        void accountsum()
        {
            cout << "Account holder: " << username << endl;
            cout << "Balance: $" << balance << endl;
            cout << "Transactions:" << endl;
            for (const Transaction &transaction : transactions)
            {
                cout << transaction.type << ", amount: $" << transaction.amount << endl;
            }
            cout << " " << endl;
        }

        void close(Database &datbase)
        {
            open = false;
            database.deleteusr(username);
            database.saveusrtofile();
            cout << "Account closed successfully." << endl;
        }

        void setUsername(const string &newUsername)
        {
            username = newUsername;
        }

        void updateBalanceInFile()
        {
            ifstream inputFile("users.txt");
            ofstream outputFile("temp.txt");

            string line;
            while(getline(inputFile, line))
            {
                istringstream iss(line);
                string storedUsr, storedPwd;
                double storedBlnc;

                if(iss >>storedUsr >> storedPwd)
                {
                    if (storedUsr == username)
                    {
                        outputFile << username << " " << storedPwd << " " << balance << endl;
                    }
                    else 
                    {
                        outputFile << line << endl;
                    }
                }
            }
            inputFile.close();
            outputFile.close();
            remove("users.txt");
            rename("temp.txt", "users.txt");
        }
    private:
        string username;
        bool open;
        double balance;
        vector<Transaction> transactions;
        Database &database;
};

class BankManager
{
    public:
        BankManager()
        {
            loadmngrfrmfile();
        }

        bool login()
        {
            string username, password;
            cout << "Please enter your username: ";
            cin >> username;
            if (managers.find(username) != managers.end())
            {
                cout << "Enter password: ";
                cin >> password;

                if(managers[username] == password)
                {
                    return true;
                }
            }
            cout << "Incorrect login. Please try again." << endl;
            return false;
        }

        bool loadmngrfrmfile()
        {
            ifstream mngrFile("managers.txt");
            if (!mngrFile)
            {
                return false;
            }

            string line;
            while(getline(mngrFile, line))
            {
                istringstream iss(line);
                string username, password;
                if (iss >> username >> password)
                {
                    managers[username] = password;
                }
            }
            mngrFile.close();
            return true;
        }
    private:
        unordered_map<string, string> managers;
};

int main()
{
    Database database;
    BankManager manager;
    string username, password;
    BankAccount acc("", database);
    int choice;
    int bankingOption;
    double withdrawal;
    double depositAmt;
    bool loggedIn = false;
    bool managerLoggedIn = false;
    while (true)
    {
        if (!loggedIn)
        {
            cout << "1. login\n2. create account\n3. manager login\n4. exit" << endl;
            cin >> choice;

            if (choice == 1)
            {
                if (database.login())
                {
                    loggedIn = true;
                    username = username;
                    acc.setUsername(username);
                }
            } 
            else if (choice == 2)
            {
                string newUsername, newPassword;
                cout << "Enter new username: ";
                cin >> newUsername;
                cout << "Enter new password: ";
                cin >> newPassword;
                database.createusr(newUsername, newPassword);
                username = newUsername;
                acc.setUsername(username);
            }
            else if (choice == 3)
            {
                if (manager.login())
                {
                    loggedIn = true;
                    managerLoggedIn = true;
                    cout << "Manager Options\n";
                    cout << "1. view all accounts\n2. logout\n";
                    cin >> bankingOption;
                    if (bankingOption == 1)
                    {
                        ifstream userFile("users.txt");
                        if (userFile.is_open())
                        {
                            string line;
                            while (getline(userFile, line))
                            {
                                cout << line << endl;
                            }
                            userFile.close();
                        }
                    }
                    if (bankingOption == 2)
                    {
                        loggedIn = false;
                        managerLoggedIn = false;
                    }
                }
            } 
            else if (choice == 4)
            {
                break;
            }
        }
        else 
        {
            if (managerLoggedIn)
            {
                cout << "Manager options\n";
                cout << "1. Get list of users and account details\n2. Logout\n";
                cin >> bankingOption;
                if (bankingOption == 1);
                {
                    ifstream userFile("users.txt");
                    if (userFile.is_open())
                    {
                        string line;
                        while (getline(userFile, line))
                        {
                            cout << line << endl;
                        }
                        userFile.close();
                    }
                }
                if (bankingOption == 2)
                {
                    loggedIn = false;
                }
            }
            else 
            {
                cout << "User Options:\n";
                cout << "1. withdraw\n2. Deposit\n3. Get account Summary\n4. Close Account \n5. Logout\n";
                cin >> bankingOption;
                if (bankingOption == 1)
                {
                    cout << "Please enter a withdrawal amount: ";
                    cin >> withdrawal;
                    acc.withdraw(withdrawal);
                }
                else if (bankingOption == 2)
                {
                    cout << "Please enter a deposit amount: ";
                    cin >> depositAmt;
                    acc.deposit(depositAmt);
                }
                else if (bankingOption == 3)
                {
                    acc.accountsum();
                }
                else if (bankingOption == 4)
                {
                    acc.close(database);
                    loggedIn = false;
                }
                else if (bankingOption == 5)
                {
                    loggedIn = false;
                }
            }
        }
    }
    return 0;
}