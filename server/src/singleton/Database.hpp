//
// Created by tom on 9/21/19.
//

#ifndef BABEL_DATABASE_HPP
#define BABEL_DATABASE_HPP

#include <string>
#include <memory>
#include <unordered_map>
#include <fstream>


class Database {
public:
    static auto get() -> Database&;
    explicit Database();
    ~Database();

    auto write(std::string const &username, std::string const &password) -> void;
    auto read(std::string const &username, std::string const &password) -> short; // 0 = KO; 1 = good username; 2 = good username/password

    static std::string file_name;
private:
    static std::shared_ptr<Database> that;
    std::unordered_map<std::string, std::string> db;
};


#endif //BABEL_DATABASE_HPP
