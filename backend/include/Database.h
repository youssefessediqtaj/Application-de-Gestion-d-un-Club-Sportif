#ifndef DATABASE_H
#define DATABASE_H

#include <mysql.h>
#include <string>
#include <vector>

class Database {
private:
  MYSQL *db;
  std::string host;
  std::string user;
  std::string password;
  std::string database;
  int port;

public:
  Database(const std::string &host = "localhost",
           const std::string &user = "root", const std::string &password = "",
           const std::string &database = "club_sportif", int port = 3306);
  ~Database();

  bool connect();
  void disconnect();
  bool ensureConnected();

  bool executeQuery(const std::string &query);
  std::vector<std::vector<std::string>> executeSelect(const std::string &query);
  uint64_t getLastInsertId();

  // Méthodes pour les statistiques
  int getTotalMembres();
  int getTotalEquipes();
  int getTotalEntraineurs();
  int getTotalSeances();
  int getTotalPaiements();
  int getMembresPayes();
  int getMembresImpayes();
};

#endif // DATABASE_H