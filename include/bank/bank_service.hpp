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
        bool is_manager = flase;
        std::string message;
    };

    sruct ActionResult 
    {
        bool ok = flase;
        std::string message;
    };

    class BankService
    {
        public:
            BankService(database& db, SecurityConfig cfg);

            AuthResult register_user(const std::string& username, const std::string& password);
            AuthResult login_user(const std::string& username, const std::string& password);
            AuthResult login_manager(const std::string& username, const std::string& password);

            ActionResult deposit(const std::string& username, double amount):
            ActionResult withdraw(const std::string& username, double amount);
            ActionResult transfer(const std::string& from_user, const std::string& to_use,r double amount);

            double balance_op(const std::string& username) const;
            const Securityonfig& config() const;

        private:
            Database& database_;
            SecurityConfig cfg_;
    };
}