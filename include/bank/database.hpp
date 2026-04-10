#pragma once

#include <ostream>
#include <string>
#include <unordered_map>
#include <utility>

namespace bank
{
    class Database
    {
        public:
        Database(const std::string& user_file = "data/users.txt",
            const std::string& manager_file = "data/managers.txt");

        bool user_exists(const std::string& username) const;
        bool manager_exists(const std::string& username) const;

        bool create_new_user(const std::string& username, const std::string& password);
        bool delete_user(const std::string& username);

        bool validate_usr_password(const std::string& username, const std::string& password) const;
        bool validate_mngr_password(const std::string& username, const std::string& password) const;

        double get_balance(const std::string& username) const;
        bool set_balance(const std::string& username, double new_balance);

        bool transfer_funds(const std::string& from_user,
            const std::string& to_user, double amount, 
            bool race_window_enabled);

        void dump_usrs(std::ostream& os)  const;

        bool load_users_from_file();
        bool load_managers_from_file();
        bool save_users_to_file() const;

        private:
        std::string user_file_;
        std::string manager_file_;

        std::unordered_map<std::string, std::pair<std::string, double>> users_;
        std::unordered_map<std::string, std::string> managers_;
    };
}