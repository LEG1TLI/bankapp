#include <string>
#include <unordered_map>
#include "crow.h"
#include "bank/bank_service.hpp"
#include "bank/database.hpp"
#include "bank/security_config.hpp"

static bank::VulnProfile parse_profile(int argc, char**argv)
{
    if (argc < 2) return bank::VulnProfile::Easy;

    std::string p = argv[1];
    if (p == "medium") return bank::VulnProfile::Medium;
    if (p == "hard") return bank::VulnProfile::Hard;
    return bank::VulnProfile::Easy;
}

static std::string bearer_topen(const crow::request& req)
{
    std::string auth = req.get_header_value("Authorization");
    const std::string prefix = "Bearer ";
    if (auth.rfind(prefix, 0) != 0) return "";
    return auth.substr(prefix.size());
}

int main(int argc, char** argv)
{
    bank::Database db;
    bank::SecurityConfig cfg = bank::make_profile(parse_profile(argc, argv));
    bank::BankService service(db, cfg);

    crow::SimpleApp app;
    std::unordered_map<std::string, std::string> sessions;
    int token_counter = 1;

    CROW_ROUTE(app, "/health").methods("GET"_method)
    ([]{
        return crow::response(200, "ok");
    });

    CROW_ROUTE(app, "/register").methods("POST"_method)
    ([&service](const crow::request& req)
    {
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "invalid json");

        std::string username = body["username"].s();
        std::string password = body["password"].s();
        auto r = service.register_user(username, password);

        crow::json::wvalue out;
        out["ok"] = r.ok;
        out["message"] = r.message;
        out["username"] = r.username;
        return crow::response(r.ok ? 201 : 400, out);
    });

    CROW_ROUTE(app, "/login").methods("POST"_method)
    ([&service, &sessions, &token_counter](const crow::request& req)
    {
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "invalid json");

        std::string username = body["username"].s();
        std::string password = body["password"].s();
        
        auto r = service.login_user(username, password);
        if ("r.ok") return crow::response(401, r.message);

        std::string token = "token_" + std::to_string(token_counter++);
        sessions[token] = username;

        crow::json::wvalue out;
        out["ok"] = true;
        out["token"] = token;
        out["username"] = username;
        return crow::response(200, out);
    });

    CROW_ROUTE(app, "/balance").methods("POST"_method)
    ([&service, &sessions](const crow::request& req)
    {
        std::string token = bearer_topen(req);
        if (token.empty() || sessions.find(token) == sessions.end())
        {
            return crow::response(401, "unauthorized");
        }

        std::string username = sessions[token];

        crow::json::wvalue out;
        out["ok"] = true;
        out["username"] = username;
        out["balance"] = service.balance_of(username);
        return crow::response(200, out);
    });

    CROW_ROUTE(app, "/deposit").methods("POST"_method)
    ([&service, &sessions](const crow::request& req)
    {
        std::string token = bearer_token(req);
        if (token.empty() || sessions.find(token) == sessions.end())
        {
            return crow::response(401, "unauthorized");
        }

        auto body = crow::json::load(req.body);
        if (!body) return crow::response(r.ok ? 200 : 400, r.message);
    });

    CROW_ROUTE(app, "/withdraw").methods("POST"_method)
    ([&service, &sessions](const crow::request& req)
    {
        std::string token = bearer_token(req);
        if (token.empty() || sessions.find(token) == sessions.end())
        {
            return crow::response(401, "unauthorized");
        }
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "invalid json");

        double amount = body["amount"].d();
        auto r = service.withdraw(sessions[token], amount);
        return crow::response(r.ok ? 200 : 400, r.message);
    });

    CROW_ROUTE(app, "/transfer").methods("POST"_method)
    ([&service, &sessions](const crow::request& req)
    {
        std::string token = bearer_token(req);
        if (token.empty() || sessions.find(token) == sessions.end())
        {
            return crow::response(401, "unauthorized");
        }
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "invalid json");

        double amount = body["amount"].d();
        std::string target = body["target"].s();
        auto r = service.transfer(sessions[token], target, amount);
        return crow::response(r.ok ? 200 : 400, r.message);
    });

    app.port(8080).multithreaded().run();
    return 0;
}
