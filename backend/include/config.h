#ifndef CONFIG_H
#define CONFIG_H

#include <string>

namespace Config {
// Configuration MySQL
const std::string MYSQL_HOST = "localhost";
const std::string MYSQL_USER = "root";
const std::string MYSQL_PASSWORD = "12345678";
const std::string MYSQL_DATABASE = "club_sportif";
const int DB_PORT = 3306;
} // namespace Config

#endif // CONFIG_H
