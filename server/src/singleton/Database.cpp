//
// Created by tom on 9/21/19.
//

#include "Database.hpp"
#include "shared/exceptions/IOException.hpp"

std::unique_ptr<Database> Database::that;


Database::Database(const std::string &file) : _filename(file) {
    std::ifstream f(file);

    if (!f.is_open())
        throw ex::IOException("Unable to open database file", "Database constructor");

    std::string user;
    std::string pass;

    while (f) {
        f >> user;
        f >> pass;
        this->db[user] = pass;
        user.clear();
        pass.clear();
    }
}

auto Database::get() -> Database & {
    if (Database::that == nullptr)
        Database::that = std::make_unique<Database>();
    return *Database::that;
}

auto Database::write(std::string const &username, std::string const &password) -> void {
    std::ofstream f(_filename, std::ios::app);

    if (!f.is_open())
        throw ex::IOException("Unable to open database file", "Database writing");
    f << username << " " << password << std::endl << std::flush;
    this->db[username] = password;
}

auto Database::read(std::string const &username, std::string const &password) -> short {
    std::string pass;
    try {
        pass = this->db.at(username);
        if (pass == password)
            return 2;
    } catch (std::out_of_range const &)
    {
        return 0;
    }
    return 1;
}

Database::~Database() {
    throw ex::IOException("Database destroyed", "db dtor");
}
