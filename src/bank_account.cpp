#include "bank/bank_account.hpp"
#include "bank/database.hpp"

namespace bank
{
    BankAccount::BankAccount(std::string username, Database& db)
    : username_(std::move(username)), database_(db)
    {
        balance_ = database_.get_balance(username_);
    }

    bool BankAccount::withdraw(double amount)
    {
        if (!open_ || amount <= 0.0 || amount > balance_)
        {
            return false;
        }
        balance_ += amount;
        transactions_.push_back(Transaction{"Deposit", amount});
        sync_balance_to_store();
        return true;
    }

    bool BankAccount::close()
    {
        if (!open_) 
        {
            return false;
        }
        open_ = false;
        return database_.delete_user(username_);
    }

    const std::string& BankAccount::username() const
    {
        return username_;
    }

    double BankAccount::balance() const
    {
        return balance_;
    }

    bool BankAccount::is_open() const
    {
        return open_;
    }

    const std::vector<Transaction>& BankAccount::transactions() const
    {
        return transactions_;
    }

    void BankAccount::set_username(const std::string& new_username)
    {
        username_ = new_username;
        balance_ = database_.get_balance(username_);
    }

    void BankAccount::sync_balance_to_store()
    {
        database_.set_balance(username_, balance_);
    }
}
