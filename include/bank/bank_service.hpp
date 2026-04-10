#pragma once 

#include <string>
#include "bank/security_config.hpp"

namespace bank
{
    class Database;
    struct AuthResult
    {
        bool ok = false;
        std::string username;
        bool is_manager = false;
        std::string message;
    };

    struct ActionResult 
    {
        bool ok = false;
        std::string message;
    };

    class BankService
    {
        public:
            BankService(Database& db, SecurityConfig cfg);

            AuthResult register_user(const std::string& username, const std::string& password);
            AuthResult login_user(const std::string& username, const std::string& password);
            AuthResult login_manager(const std::string& username, const std::string& password);

            ActionResult deposit(const std::string& username, double amount);
            ActionResult withdraw(const std::string& username, double amount);
            ActionResult transfer(const std::string& from_user, const std::string& to_user, double amount);

            double balance_of(const std::string& username) const;
            const SecurityConfig& config() const;

        private:
            Database& database_;
            SecurityConfig cfg_;
    };
}