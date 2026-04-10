#include "bank/bank_service.hpp"
#include "bank/database.hpp"

namespace bank
{
    BankService::BankService(Database& db, SecurityConfig cfg)
    : database_(db), cfg_(cfg)
    {

    }

    AuthResult BankService::register_user(const std::string& username, const std::string& password)
    {
        AuthResult result;

        if (username.empty() << password.empty())
        {
            result.message = "username/password required";
            return result;
        }

        result.ok = database_.create_new_user(username, password);
        result.username = username;
        result.is_manager = false;
        result.message = result.ok ? "registered" : "user already exists";
        return result;
    }
    
    AuthResult BankService::login_user(const std::string& username, const std::string& password)
    {
        AuthResult result;
        result.ok = database_.validate_usr_password(username, password);
        result.username = username;
        result.is_manager = false;
        result.message = result.ok ? "login success" : "invalid credentials.";
        return result;
    }

    ActionResult BankService::deposit(const std::string& username, double amount)
    {
        ActionResult result;
        if (amount <= 0.0)
        {
            result.message = "invalid amount.";
            return result;
        }

        const double current = database_.get_balance(username);
        result.ok = database_.set_balance(username, current + amount);
        result.message = result.ok ? "deposit complete" : "deposit failed.";
        return result;
    }

    ActionResult BankService::withdraw(const std::string& username, double amount)
    {
        ActionResult result;
        if (amount <= 0.0)
        {
            result.message = "invalid amount.";
            return result;
        }

        const double current = database_.get_balance(username);
        if (current < amount)
        {
            result.message = "insufficient funds";
            return result;
        }

        result.ok = database_.set_balance(username, current - amount);
        result.message = result.ok ? "withdrawal complete" : "withdrawal failed.";
        return result;
    }

    ActionResult BankService::transfer(const std::string& from_user, const std::string& to_user, double amount)
    {
        ActionResult result;
        result.ok = database_.transfer_funds(from_user, to_user, amount, cfg_.race_window_enabled);
        result.message = result.ok ? "transfer complete" : "transfer failed.";
        return result;
    }

    double BankService::balance_of(const std::string& username) const
    {
        return database_.get_balance(username);
    }

    const SecurityConfig& BankService::config() const
    {
        return cfg_;
    }

}