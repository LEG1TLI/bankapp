#pragma once

#include <string>

namespace bank
{
    class Database;
    class BankManager
    {
        public:
            explicit BankManager(Database& db);

            bool login(const std::string& username, const std::string& password) const;

        private:
            Database& database_;
    };
}