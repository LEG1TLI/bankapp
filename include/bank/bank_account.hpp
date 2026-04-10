#pragma once 

#include <string>
#include <vector>
#include "bank/transaction.hpp"
namespace bank
{
    class Database;

    class BankAccount
    {
        public:
        BankAccount(std::string username, Database& db);

        bool withdraw(double amount);
        bool deposit(double amount);
        bool close();

        const std::string& username() const;
        double balance() const;
        bool is_open() const;
        const std::vector<Transaction>& transactions() const;

        void set_username(const std::string& new_username);
        void sync_balance_to_store();

        private:
            std::string username_;
            bool open_ = true;
            double balance_ = 0.0;
            std::vector<Transaction> transactions_;
            Database& database_;
    };
}