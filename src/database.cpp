#include "bank/database.hpp"
#include <fstream>
#include <sstream>

namespace bank
{
    Database::Database(const std::string& user_file, const std::string& manager_file)
    : user_file_(user_file), manager_file_(manager_file)
    {
        load_users_from_file();
        load_managers_from_file();
    }

    bool Database::user_exists(const std::string& username) const
    {
        return users_.find(username) != users_.end();
    }

    bool Database::create_new_user(const std::string& username, const std::string& password)
    {
        if (user_exists(username) || manager_exists(username))
        {
            return false;
        }
        users_[username] = {password, 0.0};
        return save_users_to_file();
    }

    bool Database::delete_user(const std::string& username)
    {
        auto it = users_.find(username);
        if (it == users_.end())
        {
            return false;
        }
        users_.erase(it);
        return save_users_to_file();
    }

    bool Database::validate_usr_password(const std::string& username, const std::string& password) const
    {
        auto it = users_.find(username);
        return it != users_.end() && it -> second.first == password;
    }

    double Database::get_balance(const std::string& username) const
    {
        auto it = users_.find(username);
        return it != users_.end() ? it-> second.second : 0.0;
    }

    bool Database::set_balance(const std::string& username, double new_balance)
    {
        auto it = users_.find(username);
        if (it == users_.end())
        {
            return false;
        }
        it -> second.second = new_balance;
        return save_users_to_file();
    }

    bool Database::transfer_funds(const std::string& from_user,
    const std::string& to_user, double amount, bool race_window_enabled)
    {
    (void) race_window_enabled;
    if (amount <= 0)
    {
        return false;
    }

    auto from_it = users_.find(from_user);
    auto to_it = users_.find(to_user);
    if (from_it == users_.end() || to_it == users_.end())
    {
        return false;
    }

    if (from_it->second.second < amount)
    {
        return false;
    }

    from_it->second.second -= amount;
    to_it->second.second += amount;
    return save_users_to_file();
    }

    void Database::dump_usrs(std::ostream& os) const
    {
        for (const auto& user : users_)
        {
            os << user.first << " "  << user.second.first << " " << user.second.second << "\n"; 
        }
    }

    bool Database::load_users_from_file()
    {
        std::ifstream user_file(user_file_);
        if (!user_file)
        {
            return false;
        }

        users_.clear();
        std::string line;
        while (std::getline(user_file, line))
        {
            std::istringstream iss(line);
            std::string username, password;
            double balance = 0.0;
            if (iss >> username >> password >> balance)
            {
                users_[username] = {password, balance};
            }
        }
        return true;
    }

    bool Database::load_managers_from_file() 
    {
        std::ifstream manager_file(manager_file_);
        if (!manager_file)
        {
            return false;
        }
        managers_.clear();
        std::string line;
        while (std::getline(manager_file, line))
        {
            std::istringstream iss(line);
            std::string username, password;
            if (iss >> username >> password)
            {
                managers_[username] = password;
            }
        }
        return true;
    }
    

    bool Database::save_users_to_file() const
    {
        std::ofstream user_file(user_file_);
        if (!user_file)
        {
            return false;
        }
        for (const auto& user : users_)
        {
            user_file << user.first << " " << user.second.first << " " << user.second.second << "\n";
        }
        return true;
    }
}    