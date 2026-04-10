#include <iostream>
#include <string>
#include "bank/bank_service.hpp"
#include "bank/database.hpp"
#include "bank/security_config.hpp"

static bank::VulnProfile parse_profile(int argc, char** argv)
{
    if (argc < 2) return bank::VulnProfile::Easy;

    std::string p = argv[1];
    if (p == "medium") return bank::VulnProfile::Medium;
    if (p == "hard") return bank::VulnProfile::Hard;
    return bank::VulnProfile::Easy;
}

int main(int argc, char** argv)
{
    bank::Database db;
    bank::SecurityConfig cfg = bank::make_profile(parse_profile(argc, argv));
    bank::BankService service(db, cfg);

    bool running = true;
    while (running)
    {
        std::cout << "\n=== CLI Menu ===\n";
        std::cout << "1) Register\n";
        std::cout << "2) Login user\n";
        std::cout << "3) Deposit\n";
        std::cout << "4) Withdraw\n";
        std::cout << "5) Transfer\n";
        std::cout << "6) Balance\n";
        std::cout << "7) Exit\n";
        std::cout << "Choice: ";

        int choice = 0;
        std::cin >> choice;

        std::string user, pass, to;
        double amt = 0.0;

        switch(choice)
        {
            case 1:
            {
                std::cout << "Username: ";
                std::cin >> user;
                std::cout << "Password: ";
                std::cin >> pass;
                auto r = service.register_user(user, pass);
                std::cout << r.message << "\n";
                break;
            }
            case 2:
            {
                std::cout << "Username: ";
                std::cin >> user;
                std::cout << "Passsword: ";
                std::cin >> pass;
                auto r = service.login_user(user, pass);
                std::cout << r.message << "\n";
                break;
            }
            case 3:
            {
                std::cout << "Username: ";
                std::cin >> user;
                std::cout << "Amount: ";
                std::cin >> amt;
                auto r = service.deposit(user, amt);
                std::cout << r.message << "\n";
                break;
            }
            case 4:
            {
                std::cout << "Username: ";
                std::cin >> user;
                std::cout << "Amount: ";
                std::cin >> amt;
                auto r = service.withdraw(user, amt);
                std::cout << r.message << "\n";
                break;
            }
            case 5:
            {
                std::cout << "From User: ";
                std::cin >> user;
                std::cout << "To User: ";
                std::cin >> to;
                std::cout << "Amount: ";
                std::cin >> amt;
                auto r = service.transfer(user, to, amt);
                std::cout << r.message << "\n";
                break;
            }
            case 6:
            {
                std::cout << "Username: ";
                std::cin >> user;
                std::cout << "Balance: " << service.balance_of(user) << "\n";
                break;
            }
            case 7:
            {
                running = false;
                break;
            }
            default:
            {
                std::cout << "Invalid choice. Please try again.\n";
                break;
            }
    }
    return 0;
}