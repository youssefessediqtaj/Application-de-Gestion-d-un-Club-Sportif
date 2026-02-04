#include "../include/Database.h"
#include "../include/config.h"
#include <iostream>
#include <sstream>

Database::Database(const std::string &h, const std::string &u,
                   const std::string &p, const std::string &d, int pt)
    : db(nullptr), host(h), user(u), password(p), database(d), port(pt) {}

Database::~Database() { disconnect(); }

bool Database::connect() {
  // Initialiser la structure MySQL
  db = mysql_init(nullptr);

  if (db == nullptr) {
    std::cerr << "Erreur d'initialisation MySQL: " << mysql_error(db)
              << std::endl;
    return false;
  }

  // Activer la reconnexion automatique
  bool reconnect = true;
  mysql_options(db, MYSQL_OPT_RECONNECT, &reconnect);

  // Se connecter à MySQL
  if (mysql_real_connect(db, host.c_str(), user.c_str(), password.c_str(),
                         database.c_str(), port, nullptr, 0) == nullptr) {
    std::cerr << "Erreur de connexion à MySQL: " << mysql_error(db)
              << std::endl;
    mysql_close(db);
    db = nullptr;
    return false;
  }

  // Définir l'encodage UTF-8
  mysql_set_character_set(db, "utf8mb4");

  std::cout << "Connecté à la base de données MySQL: " << database << std::endl;
  return true;
}

void Database::disconnect() {
  if (db) {
    mysql_close(db);
    db = nullptr;
  }
}

bool Database::ensureConnected() {
  if (!db || mysql_ping(db) != 0) {
    std::cout << "Tentative de reconnexion à MySQL..." << std::endl;
    return connect();
  }
  return true;
}

bool Database::executeQuery(const std::string &query) {
  if (!ensureConnected()) {
    return false;
  }

  if (mysql_query(db, query.c_str()) != 0) {
    std::cerr << "Erreur SQL: " << mysql_error(db) << std::endl;
    std::cerr << "Requête: " << query << std::endl;
    return false;
  }

  return true;
}

std::vector<std::vector<std::string>>
Database::executeSelect(const std::string &query) {
  std::vector<std::vector<std::string>> results;

  if (!ensureConnected()) {
    return results;
  }

  if (mysql_query(db, query.c_str()) != 0) {
    std::cerr << "Erreur de requête: " << mysql_error(db) << std::endl;
    return results;
  }

  MYSQL_RES *result = mysql_store_result(db);
  if (result == nullptr) {
    std::cerr << "Erreur de récupération des résultats: " << mysql_error(db)
              << std::endl;
    return results;
  }

  int num_fields = mysql_num_fields(result);
  MYSQL_ROW row;

  while ((row = mysql_fetch_row(result))) {
    std::vector<std::string> rowData;
    for (int i = 0; i < num_fields; i++) {
      rowData.push_back(row[i] ? std::string(row[i]) : "");
    }
    results.push_back(rowData);
  }

  mysql_free_result(result);
  return results;
}

uint64_t Database::getLastInsertId() {
  if (db) {
    return mysql_insert_id(db);
  }
  return 0;
}

// Méthodes pour les statistiques
int Database::getTotalMembres() {
  auto results = executeSelect("SELECT COUNT(*) FROM membres;");
  if (!results.empty() && !results[0].empty()) {
    try {
      return std::stoi(results[0][0]);
    } catch (const std::exception &e) {
      std::cerr << "Erreur de conversion: " << e.what() << std::endl;
      return 0;
    }
  }
  return 0;
}

int Database::getTotalEquipes() {
  auto results = executeSelect("SELECT COUNT(*) FROM equipes;");
  if (!results.empty() && !results[0].empty()) {
    try {
      return std::stoi(results[0][0]);
    } catch (const std::exception &e) {
      std::cerr << "Erreur de conversion: " << e.what() << std::endl;
      return 0;
    }
  }
  return 0;
}

int Database::getTotalEntraineurs() {
  auto results = executeSelect("SELECT COUNT(*) FROM entraineurs;");
  if (!results.empty() && !results[0].empty()) {
    try {
      return std::stoi(results[0][0]);
    } catch (const std::exception &e) {
      std::cerr << "Erreur de conversion: " << e.what() << std::endl;
      return 0;
    }
  }
  return 0;
}

int Database::getTotalSeances() {
  auto results = executeSelect("SELECT COUNT(*) FROM seances;");
  if (!results.empty() && !results[0].empty()) {
    try {
      return std::stoi(results[0][0]);
    } catch (const std::exception &e) {
      std::cerr << "Erreur de conversion: " << e.what() << std::endl;
      return 0;
    }
  }
  return 0;
}

int Database::getTotalPaiements() {
  auto results = executeSelect("SELECT COUNT(*) FROM paiements;");
  if (!results.empty() && !results[0].empty()) {
    try {
      return std::stoi(results[0][0]);
    } catch (const std::exception &e) {
      std::cerr << "Erreur de conversion: " << e.what() << std::endl;
      return 0;
    }
  }
  return 0;
}

int Database::getMembresPayes() {
  auto results = executeSelect("SELECT COUNT(DISTINCT p.membre_id) FROM "
                               "paiements p WHERE p.statut = 'Payé';");
  if (!results.empty() && !results[0].empty()) {
    try {
      return std::stoi(results[0][0]);
    } catch (const std::exception &e) {
      std::cerr << "Erreur de conversion: " << e.what() << std::endl;
      return 0;
    }
  }
  return 0;
}

int Database::getMembresImpayes() {
  std::string query =
      "SELECT COUNT(*) FROM membres m WHERE m.id NOT IN (SELECT DISTINCT "
      "p.membre_id FROM paiements p WHERE p.statut = 'Payé');";
  auto results = executeSelect(query);
  if (!results.empty() && !results[0].empty()) {
    try {
      return std::stoi(results[0][0]);
    } catch (const std::exception &e) {
      std::cerr << "Erreur de conversion: " << e.what() << std::endl;
      return 0;
    }
  }
  return 0;
}