#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <vector>
using namespace std;

class Database
{
    public:
        Database()
        {
            loadusrfrmfile();
            loadmngrfrmfile();
        }
    
    bool userexists(const string &username)    
    {
        return users.find(username) != users.end();
    }

    double getBalance(const string &username)
    {
        auto it = users.find(username);
        return (it != users.end()) ? it->second.second : 0.0;
    }

    void setBalance(const string &username, double newBalance)
    {
        auto it = users.find(username);
        if (it != users.end())
        {
            it ->second.second = newBalance;
            saveusrtofile();
        }
    }

        
    void saveusrtofile()
    {
        ofstream userFile("users.txt");

        for (const auto &user : users)
        {
            userFile << user.first << " " << user.second.first << " " << user.second.second << '\n';
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
            double balance;
            if (iss >> username >> password >> balance)
            {
                users[username] = make_pair(password, balance);
            }
        }
        userFile.close();
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
        saveusrtofile();
        cout << "User successfully deleted." << endl;
    }

    bool login(string & outUsername)
    {
        string username, password;
        cout << "Please enter your username: ";
        cin >> username;
        auto it = users.find(username);
        if (it != users.end())
        {
            cout << "Please enter your password: ";
            cin >> password;
            if (it ->second.first == password)
            {
                outUsername = username;
                cout << "login successful. Welcome, " << username << endl;
                return true;
            }
        }
        cout << "Invalid username or password. Please try again." << endl;
        return false;
    }

    void dumpUsers(ostream &os) const
    {
        for (const auto &u : users)
        {
            os << u.first << " " << u.second.first << " " << u.second.second << endl;
        }
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
            balance = database.getBalance(username);
        }

        void withdraw(double amount)
        {
            if (open && amount <= balance && amount > 0)
            {
                balance -= amount;
                transactions.emplace_back("Withdrawal", amount);
                cout << "Successfully withdrew $" << amount << "from your account." << endl;
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

        void close()
        {
            open = false;
            database.deleteusr(username);
            cout << "Account closed successfully. Goodbye!" << endl;
        }

        void setUsername(const string &newUsername)
        {
            username = newUsername;
        }

        void updateBalanceInFile()
        {
            database.setBalance(username, balance);
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
            switch (choice)
            {
                case 1:
                    if (database.login(username))
                    {
                        loggedIn = true;
                        acc.setUsername(username);
                    }
                    break;
                case 2:
                    {
                    string newUsername, newPassword;
                    cout << "Enter new username: ";
                    cin >> newUsername;
                    cout << "Enter new password: ";
                    cin >> newPassword;
                    database.createusr(newUsername, newPassword);
                    username = newUsername;
                    acc.setUsername(username);
                    break;
                    }
                case 3:
                    if (manager.login())
                    {
                        loggedIn = true;
                        managerLoggedIn = true;
                        cout << "manager options\n";
                        cout << "1. view all accounts\n2. logout\n";
                        cin >> bankingOption;
                        switch(bankingOption)
                        {
                            case 1:
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
                                database.dumpUsers(cout);
                                break;
                                }
                            case 2:
                                loggedIn = false;
                                managerLoggedIn = false;
                                break;
                        }
                    }
                    break;

                case 4: 
                    database.saveusrtofile();
                    return 0;
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
                    database.dumpUsers(cout);
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
                switch(bankingOption)
                {
                    case 1:
                        cout << "Please enter a withdrawal amount: ";
                        cin >> withdrawal;
                        acc.withdraw(withdrawal);
                        break;
                    case 2:
                        cout << "Please enter a deposit amount: ";
                        cin >> depositAmt;
                        acc.deposit(depositAmt);
                        break;
                    case 3:
                        acc.accountsum();
                        break;
                    case 4:
                        acc.close();
                        loggedIn = false;
                        break;
                    case 5:
                        loggedIn = false;
                        break;
                }
            }
        }
    }
    return 0;
}