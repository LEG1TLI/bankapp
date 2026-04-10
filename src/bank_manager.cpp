#include "bank/bank_manager.hpp"
#include "bank/database.hpp"

namespace bank
{
    BankManager::BankManager(Database& db) : database_(db)
    {

    }
    bool BankManager::login(const std::string& username, const std::string& password) const
    {
        return database_.validate_mngr_password(username, password);
    }
}