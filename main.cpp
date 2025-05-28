#include "BankDatabase.h" 
#include "crow.h"
#include <string>
#include <mutex>
#include <random>
#include <chrono>
#include <sstream>
#include <unordered_map>

// Global instances
BankDatabase bank_database;
std::unordered_map<std::string, int> sessionMap;
std::mutex mtx;

// Helper: Generate a random session token
std::string generateSessionToken() {
    static std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    static std::uniform_int_distribution<uint64_t> dist;
    std::stringstream ss;
    for (int i = 0; i < 4; ++i) ss << std::hex << dist(rng);
    return ss.str();
}

// Helper: Get account number from session token
int getAccountFromToken(const crow::request& req) {
    auto it = req.get_header_value("X-Session-Token");
    if (it.empty()) return 0;
    std::lock_guard<std::mutex> guard(mtx);
    auto found = sessionMap.find(it);
    if (found != sessionMap.end()) return found->second;
    return 0;
}

int main() {
    crow::SimpleApp app;

    // Serve the HTML file
    CROW_ROUTE(app, "/")([]() {
        std::ifstream file("atm_interface.html");
        if (file.is_open()) {
            std::stringstream buffer;
            buffer << file.rdbuf();
            return crow::response(buffer.str());
        }
        return crow::response(404, "ATM interface not found");
    });

    // Login API endpoint
    CROW_ROUTE(app, "/api/login").methods("POST"_method)([](const crow::request& req) {
        auto json = crow::json::load(req.body);
        if (!json || !json.has("accountNumber") || !json.has("pin")) {
            return crow::response(400, "{\"error\":\"Invalid JSON or missing credentials.\"}");
        }
        
        std::string accNum = json["accountNumber"].s();
        std::string pin = json["pin"].s();
        
        if (bank_database.authenticateUser(accNum, pin)) {
            int accNumber = 0;
            try { 
                accNumber = std::stoi(accNum); 
            } catch (...) { 
                return crow::response(400, "{\"error\":\"Invalid account number format.\"}"); 
            }
            
            Account* acc = bank_database.getAccount(accNumber);
            if (!acc) {
                return crow::response(500, "{\"error\":\"Internal server error.\"}");
            }
            
            std::string token = generateSessionToken();
            {
                std::lock_guard<std::mutex> guard(mtx);
                sessionMap[token] = accNumber;
            }
            
            crow::json::wvalue result;
            result["success"] = true;
            result["userName"] = acc->getUserName();
            result["accountNumber"] = accNum;
            result["token"] = token;
            
            return crow::response(200, result);
        }
        return crow::response(401, "{\"error\":\"Invalid account number or PIN.\"}");
    });

    // Logout API endpoint
    CROW_ROUTE(app, "/api/logout").methods("POST"_method)([](const crow::request& req) {
        auto token = req.get_header_value("X-Session-Token");
        if (!token.empty()) {
            std::lock_guard<std::mutex> guard(mtx);
            sessionMap.erase(token);
        }
        return crow::response(200, "{\"success\":true}");
    });

    // Get Balance API endpoint
    CROW_ROUTE(app, "/api/balance").methods("GET"_method)([](const crow::request& req) {
        int accNumber = getAccountFromToken(req);
        if (accNumber == 0) return crow::response(401, "{\"error\":\"Not authenticated.\"}");
        double balance = bank_database.getAvailableBalance(accNumber);
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << balance;
        crow::json::wvalue result;
        result["success"] = true;
        result["balance"] = ss.str();
        return crow::response(200, result);
    });

    // Withdrawal API endpoint
    CROW_ROUTE(app, "/api/withdraw").methods("POST"_method)([](const crow::request& req) {
        int accNumber = getAccountFromToken(req);
        if (accNumber == 0) return crow::response(401, "{\"error\":\"Not authenticated.\"}");
        auto json = crow::json::load(req.body);
        if (!json || !json.has("amount")) return crow::response(400, "{\"error\":\"Invalid JSON or missing amount.\"}");
        double amount = json["amount"].d();
        if (amount <= 0) return crow::response(400, "{\"error\":\"Withdrawal amount must be positive.\"}");
        double availableBalance = bank_database.getAvailableBalance(accNumber);
        if (availableBalance < amount) return crow::response(400, "{\"error\":\"Insufficient funds in your account.\"}");
        bank_database.debit(accNumber, amount);
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << bank_database.getAvailableBalance(accNumber);
        crow::json::wvalue result;
        result["success"] = true;
        result["newBalance"] = ss.str();
        return crow::response(200, result);
    });

    // Deposit API endpoint
    CROW_ROUTE(app, "/api/deposit").methods("POST"_method)([](const crow::request& req) {
        int accNumber = getAccountFromToken(req);
        if (accNumber == 0) return crow::response(401, "{\"error\":\"Not authenticated.\"}");
        auto json = crow::json::load(req.body);
        if (!json || !json.has("amount")) return crow::response(400, "{\"error\":\"Invalid JSON or missing amount.\"}");
        double amount = json["amount"].d();
        if (amount <= 0) return crow::response(400, "{\"error\":\"Deposit amount must be positive.\"}");
        bank_database.credit(accNumber, amount);
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << bank_database.getAvailableBalance(accNumber);
        crow::json::wvalue result;
        result["success"] = true;
        result["newBalance"] = ss.str();
        return crow::response(200, result);
    });

    // History API endpoint
    CROW_ROUTE(app, "/api/history").methods("GET"_method)([](const crow::request& req) {
        int accNumber = getAccountFromToken(req);
        if (accNumber == 0) return crow::response(401, "{\"error\":\"Not authenticated.\"}");
        
        // For now, just return the current balance since we don't have transaction history storage
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << bank_database.getAvailableBalance(accNumber);
        crow::json::wvalue result;
        result["success"] = true;
        result["balance"] = ss.str();
        return crow::response(200, result);
    });

    std::cout << "ATM server starting on port 18080..." << std::endl;
    app.port(18080).multithreaded().run();
    return 0;
}