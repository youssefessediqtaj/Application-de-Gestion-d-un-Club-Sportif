# Guide de Migration SQLite vers MySQL

## Vue d'Ensemble

Ce guide vous accompagne dans la migration complète de la base de données SQLite vers MySQL pour l'application de gestion du club sportif. La migration a été préparée mais nécessite quelques étapes manuelles pour être complétée.

## Prérequis

### 1. Installation de MySQL

**Sur macOS** :
```bash
# Installer MySQL via Homebrew
brew install mysql

# Démarrer le service MySQL
brew services start mysql

# Sécuriser l'installation (optionnel mais recommandé)
mysql_secure_installation
```

**Vérifier l'installation** :
```bash
mysql --version
# Devrait afficher: mysql  Ver 8.x.x
```

### 2. Installation des Bibliothèques MySQL C++

```bash
# Sur macOS
brew install mysql-client

# Vérifier l'emplacement des bibliothèques
ls /usr/local/opt/mysql-client/lib
ls /usr/local/opt/mysql-client/include
```

---

## Étape 1: Création de la Base de Données MySQL

### 1.1 Connexion à MySQL

```bash
mysql -u root -p
# Entrez votre mot de passe MySQL
```

### 1.2 Création de la Base

```sql
-- Créer la base de données
CREATE DATABASE club_sportif CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

-- Vérifier
SHOW DATABASES;

-- Quitter
exit;
```

### 1.3 Import du Schéma

```bash
cd /Users/mac/Desktop/c++\ douae\ emsi/backend/database
mysql -u root -p club_sportif < schema_mysql.sql
```

### 1.4 Vérification

```bash
mysql -u root -p club_sportif
```

```sql
-- Lister les tables
SHOW TABLES;

-- Vérifier les utilisateurs par défaut
SELECT * FROM utilisateurs;

-- Devrait afficher:
-- +----+----------+-----------+------------+
-- | id | username | password  | role       |
-- +----+----------+-----------+------------+
-- |  1 | admin    | admin123  | ADMIN      |
-- |  2 | coach1   | coach123  | ENTRAINEUR |
-- |  3 | membre1  | membre123 | MEMBRE     |
-- +----+----------+-----------+------------+

exit;
```

---

## Étape 2: Modification du Code Backend

### 2.1 Créer le Fichier de Configuration

**Fichier**: `/Users/mac/Desktop/c++ douae emsi/backend/include/config.h`

```cpp
#ifndef CONFIG_H
#define CONFIG_H

#include <string>

namespace Config {
    // Configuration MySQL
    const std::string MYSQL_HOST = "localhost";
    const std::string MYSQL_USER = "root";
    const std::string MYSQL_PASSWORD = ""; // Mettez votre mot de passe MySQL ici
    const std::string MYSQL_DATABASE = "club_sportif";
    const int MYSQL_PORT = 3306;
}

#endif // CONFIG_H
```

> [!IMPORTANT]
> **Sécurité**: Ne commitez jamais ce fichier avec un vrai mot de passe dans un dépôt Git. Ajoutez `config.h` au `.gitignore`.

### 2.2 Modifier Database.h

**Fichier**: `/Users/mac/Desktop/c++ douae emsi/backend/include/Database.h`

**Avant** (SQLite):
```cpp
#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>
#include <vector>

class Database {
private:
    sqlite3* db;
    std::string dbPath;

public:
    Database(const std::string& path = "club_sportif.db");
    ~Database();

    bool connect();
    void disconnect();
    
    bool executeQuery(const std::string& query);
    std::vector<std::vector<std::string>> executeSelect(const std::string& query);
    
    // ... méthodes statistiques
};

#endif
```

**Après** (MySQL):
```cpp
#ifndef DATABASE_H
#define DATABASE_H

#include <mysql/mysql.h>
#include <string>
#include <vector>

class Database {
private:
    MYSQL* db;
    std::string host;
    std::string user;
    std::string password;
    std::string database;
    int port;

public:
    Database(const std::string& host = "localhost",
             const std::string& user = "root",
             const std::string& password = "",
             const std::string& database = "club_sportif",
             int port = 3306);
    ~Database();

    bool connect();
    void disconnect();
    
    bool executeQuery(const std::string& query);
    std::vector<std::vector<std::string>> executeSelect(const std::string& query);
    
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
```

### 2.3 Modifier Database.cpp

**Fichier**: `/Users/mac/Desktop/c++ douae emsi/backend/src/Database.cpp`

**Remplacer tout le contenu par**:

```cpp
#include "../include/Database.h"
#include "../include/config.h"
#include <iostream>
#include <sstream>

Database::Database(const std::string& h, const std::string& u, 
                   const std::string& p, const std::string& d, int pt)
    : db(nullptr), host(h), user(u), password(p), database(d), port(pt) {
}

Database::~Database() {
    disconnect();
}

bool Database::connect() {
    // Initialiser la structure MySQL
    db = mysql_init(nullptr);
    
    if (db == nullptr) {
        std::cerr << "Erreur d'initialisation MySQL: " << mysql_error(db) << std::endl;
        return false;
    }
    
    // Se connecter à MySQL
    if (mysql_real_connect(db, host.c_str(), user.c_str(), password.c_str(),
                          database.c_str(), port, nullptr, 0) == nullptr) {
        std::cerr << "Erreur de connexion à MySQL: " << mysql_error(db) << std::endl;
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

bool Database::executeQuery(const std::string& query) {
    if (!db) {
        std::cerr << "Erreur: Pas de connexion à la base de données" << std::endl;
        return false;
    }
    
    if (mysql_query(db, query.c_str()) != 0) {
        std::cerr << "Erreur SQL: " << mysql_error(db) << std::endl;
        std::cerr << "Requête: " << query << std::endl;
        return false;
    }
    
    return true;
}

std::vector<std::vector<std::string>> Database::executeSelect(const std::string& query) {
    std::vector<std::vector<std::string>> results;
    
    if (!db) {
        std::cerr << "Erreur: Pas de connexion à la base de données" << std::endl;
        return results;
    }
    
    if (mysql_query(db, query.c_str()) != 0) {
        std::cerr << "Erreur de requête: " << mysql_error(db) << std::endl;
        return results;
    }
    
    MYSQL_RES* result = mysql_store_result(db);
    if (result == nullptr) {
        std::cerr << "Erreur de récupération des résultats: " << mysql_error(db) << std::endl;
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

// Méthodes pour les statistiques
int Database::getTotalMembres() {
    auto results = executeSelect("SELECT COUNT(*) FROM membres;");
    if (!results.empty() && !results[0].empty()) {
        try {
            return std::stoi(results[0][0]);
        } catch (const std::exception& e) {
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
        } catch (const std::exception& e) {
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
        } catch (const std::exception& e) {
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
        } catch (const std::exception& e) {
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
        } catch (const std::exception& e) {
            std::cerr << "Erreur de conversion: " << e.what() << std::endl;
            return 0;
        }
    }
    return 0;
}

int Database::getMembresPayes() {
    auto results = executeSelect("SELECT COUNT(DISTINCT p.membre_id) FROM paiements p WHERE p.statut = 'Payé';");
    if (!results.empty() && !results[0].empty()) {
        try {
            return std::stoi(results[0][0]);
        } catch (const std::exception& e) {
            std::cerr << "Erreur de conversion: " << e.what() << std::endl;
            return 0;
        }
    }
    return 0;
}

int Database::getMembresImpayes() {
    std::string query = "SELECT COUNT(*) FROM membres m WHERE m.id NOT IN (SELECT DISTINCT p.membre_id FROM paiements p WHERE p.statut = 'Payé');";
    auto results = executeSelect(query);
    if (!results.empty() && !results[0].empty()) {
        try {
            return std::stoi(results[0][0]);
        } catch (const std::exception& e) {
            std::cerr << "Erreur de conversion: " << e.what() << std::endl;
            return 0;
        }
    }
    return 0;
}
```

### 2.4 Modifier main.cpp

**Fichier**: `/Users/mac/Desktop/c++ douae emsi/backend/main.cpp`

**Changements à apporter**:

1. Ajouter l'include du config:
```cpp
#include "include/config.h"
```

2. Modifier l'initialisation de la base de données (ligne ~98):

**Avant**:
```cpp
Database db;
if (db.connect()) {
    // Exécuter le schéma SQL pour s'assurer que les tables existent
    db.executeQuery("CREATE TABLE IF NOT EXISTS utilisateurs ...");
    // ...
}
```

**Après**:
```cpp
Database db(Config::MYSQL_HOST, Config::MYSQL_USER, 
            Config::MYSQL_PASSWORD, Config::MYSQL_DATABASE, Config::MYSQL_PORT);
if (db.connect()) {
    // Les tables existent déjà dans MySQL, pas besoin de CREATE TABLE
    // Vérifier s'il y a des utilisateurs
    auto utilisateurs = db.executeSelect("SELECT COUNT(*) FROM utilisateurs;");
    if (!utilisateurs.empty() && utilisateurs[0][0] == "0") {
        // Ajouter des utilisateurs par défaut
        db.executeQuery("INSERT INTO utilisateurs (username, password, role) VALUES ('admin', 'admin123', 'ADMIN'), ('coach1', 'coach123', 'ENTRAINEUR'), ('membre1', 'membre123', 'MEMBRE');");
    }
    
    db.disconnect();
}
```

---

## Étape 3: Compilation avec MySQL

### 3.1 Arrêter l'Application Actuelle

```bash
# Dans le terminal du backend, tapez:
quit

# Dans le terminal du frontend:
Ctrl+C
```

### 3.2 Compiler avec MySQL

```bash
cd /Users/mac/Desktop/c++\ douae\ emsi/backend

# Compilation avec MySQL
g++ -o club_sportif_mysql main.cpp src/*.cpp src/server/*.cpp \
    -I/usr/local/opt/mysql-client/include \
    -L/usr/local/opt/mysql-client/lib \
    -lmysqlclient \
    -std=c++17
```

> [!NOTE]
> Si vous obtenez une erreur "mysql/mysql.h not found", ajustez le chemin `-I` selon votre installation MySQL.

### 3.3 Tester la Compilation

```bash
./club_sportif_mysql
```

**Résultat attendu**:
```
Connecté à la base de données MySQL: club_sportif
Données chargées avec succès!
Serveur HTTP démarré sur le port 8080
L'application continue à fonctionner en arrière-plan...
Tapez 'quit' pour arrêter le serveur:
```

---

## Étape 4: Tests de Vérification

### 4.1 Test de Connexion API

```bash
curl -X POST http://localhost:8080/login \
  -H "Content-Type: application/json" \
  -d '{"username":"admin","password":"admin123"}'
```

**Résultat attendu**:
```json
{
  "success": true,
  "user": {
    "id": 1,
    "username": "admin",
    "role": "ADMIN"
  }
}
```

### 4.2 Test des Statistiques

```bash
curl http://localhost:8080/stats/general
```

**Résultat attendu**:
```json
{
  "totalMembres": 0,
  "totalEquipes": 0,
  "totalEntraineurs": 0,
  "totalSeances": 0,
  "totalPaiements": 0,
  "membresPayes": 0,
  "membresImpayes": 0
}
```

### 4.3 Test Frontend

```bash
cd /Users/mac/Desktop/c++\ douae\ emsi/frontend
npm run dev
```

Ouvrir http://localhost:3000 et tester:
- ✅ Connexion avec admin/admin123
- ✅ Navigation dans le dashboard
- ✅ Création d'un membre
- ✅ Vérification dans MySQL

---

## Étape 5: Vérification dans MySQL

```bash
mysql -u root -p club_sportif
```

```sql
-- Vérifier les membres créés via l'interface
SELECT * FROM membres;

-- Vérifier les équipes
SELECT * FROM equipes;

-- Vérifier les statistiques
SELECT 
    (SELECT COUNT(*) FROM membres) as total_membres,
    (SELECT COUNT(*) FROM equipes) as total_equipes,
    (SELECT COUNT(*) FROM entraineurs) as total_entraineurs;

exit;
```

---

## Étape 6: Migration des Données (Optionnel)

Si vous avez des données dans SQLite que vous souhaitez migrer vers MySQL:

### 6.1 Exporter depuis SQLite

```bash
cd /Users/mac/Desktop/c++\ douae\ emsi/backend

# Exporter les données
sqlite3 club_sportif.db .dump > sqlite_dump.sql
```

### 6.2 Convertir pour MySQL

Éditer `sqlite_dump.sql` et:
1. Supprimer les lignes `BEGIN TRANSACTION;` et `COMMIT;`
2. Remplacer `AUTOINCREMENT` par `AUTO_INCREMENT`
3. Ajuster les types de données si nécessaire

### 6.3 Importer dans MySQL

```bash
mysql -u root -p club_sportif < sqlite_dump.sql
```

---

## Dépannage

### Erreur: "Can't connect to MySQL server"

**Solution**:
```bash
# Vérifier que MySQL est démarré
brew services list

# Démarrer MySQL si nécessaire
brew services start mysql
```

### Erreur: "Access denied for user 'root'@'localhost'"

**Solution**:
```bash
# Réinitialiser le mot de passe MySQL
mysql -u root

# Dans MySQL:
ALTER USER 'root'@'localhost' IDENTIFIED BY 'nouveau_mot_de_passe';
FLUSH PRIVILEGES;
exit;

# Mettre à jour config.h avec le nouveau mot de passe
```

### Erreur: "mysql/mysql.h: No such file or directory"

**Solution**:
```bash
# Trouver l'emplacement des headers MySQL
brew --prefix mysql-client

# Utiliser ce chemin dans la compilation:
g++ ... -I$(brew --prefix mysql-client)/include ...
```

---

## Checklist de Migration

- [ ] MySQL installé et démarré
- [ ] Base de données `club_sportif` créée
- [ ] Schéma importé (schema_mysql.sql)
- [ ] Utilisateurs par défaut présents
- [ ] config.h créé avec les bonnes informations
- [ ] Database.h modifié pour MySQL
- [ ] Database.cpp modifié pour MySQL
- [ ] main.cpp modifié pour MySQL
- [ ] Compilation réussie avec `-lmysqlclient`
- [ ] Backend démarre et se connecte à MySQL
- [ ] API de login fonctionne
- [ ] Frontend se connecte au backend
- [ ] Opérations CRUD fonctionnent
- [ ] Données persistées dans MySQL

---

## Conclusion

Une fois toutes ces étapes complétées, votre application utilisera MySQL au lieu de SQLite. Les avantages incluent:

✅ **Performance**: Meilleure gestion des connexions concurrentes  
✅ **Scalabilité**: Prêt pour un environnement de production  
✅ **Fonctionnalités**: Transactions ACID complètes, triggers, procédures stockées  
✅ **Sécurité**: Gestion avancée des utilisateurs et permissions  
✅ **Professionnalisme**: Base de données utilisée en entreprise  

**Note pour le rapport**: Documentez cette migration comme une amélioration majeure du projet, expliquant le choix de MySQL pour se rapprocher d'un environnement professionnel.
