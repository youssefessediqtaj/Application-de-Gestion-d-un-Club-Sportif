# Application de Gestion d'un Club Sportif

Application de gestion complète d'un club sportif développée en C++ pour le backend et React pour l'interface utilisateur. Cette version intègre une base de données MySQL pour une meilleure robustesse et scalabilité.

## Technologies utilisées

- **Backend** : C++ avec programmation orientée objet (POO)
- **Frontend** : React avec composants fonctionnels et hooks
- **Base de données** : MySQL / MariaDB pour la persistance des données
- **Communication** : API REST via un serveur HTTP personnalisé en C++

## Structure du projet

```
.
├── backend/
│   ├── include/
│   │   ├── ClubSportif.h
│   │   ├── Database.h
│   │   ├── config.h
│   │   └── ... (autres headers)
│   ├── src/
│   │   ├── ClubSportif.cpp
│   │   ├── Database.cpp
│   │   └── ... (autres implémentations)
│   ├── database/
│   │   └── schema_mysql.sql
│   ├── main.cpp
│   └── club_sportif_mysql (exécutable)
├── frontend/ (projet React/Vite)
└── docs/
    ├── migration_mysql.md
    ├── rapport_projet.md
    └── ...
```

## Installation et exécution

### 1. Base de données (MySQL)

1. Assurez-vous que MySQL ou MariaDB est installé et démarré.
2. Créez la base de données : `CREATE DATABASE club_sportif;`
3. Importez le schéma : `mysql -u root -p club_sportif < backend/database/schema_mysql.sql`
4. Vérifiez la configuration dans [config.h](file:///Users/mac/Desktop/c++%20douae%20emsi/backend/include/config.h) (Host, User, Password).

### 2. Backend (C++)

#### Sur macOS
1. Installez le connecteur MariaDB via Homebrew :
   ```bash
   brew install mariadb-connector-c
   ```
2. Compilez le projet avec les chemins Homebrew :
   ```bash
   cd backend
   g++ -o club_sportif main.cpp src/*.cpp src/server/*.cpp -I/opt/homebrew/opt/mariadb/include/mysql -L/opt/homebrew/opt/mariadb/lib -lmariadb -std=c++17
   ```
3. Exécutez :
   ```bash
   ./club_sportif
   ```

#### Sur Windows (MinGW)
1. Installez MinGW (via MSYS2) et le [MySQL Connector/C](https://dev.mysql.com/downloads/connector/c/).
2. Ajoutez le répertoire `lib` de MySQL à votre variable d'environnement `PATH`.
3. Compilez en incluant la bibliothèque réseau `ws2_32` :
   ```bash
   cd backend
   g++ -o club_sportif.exe main.cpp src/*.cpp src/server/*.cpp -I"C:\Chemin\Vers\MySQL\include" -L"C:\Chemin\Vers\MySQL\lib" -lmysql -lws2_32 -std=c++17
   ```
4. Exécutez :
   ```bash
   club_sportif.exe
   ```

### 3. Frontend (React)

1. Accédez au répertoire frontend : `cd frontend`
2. Installez les dépendances : `npm install`
3. Démarrez l'application : `npm run dev`

L'application sera accessible sur `http://localhost:5173` (ou le port affiché par Vite).
Le backend écoute sur `http://localhost:8080`.

## Comptes de test

- **Administrateur** : `admin` / `admin123`
- **Entraîneur** : `coach1` / `coach123`
- **Membre** : `membre1` / `membre123`

## Fonctionnalités clés

- Authentification sécurisée et gestion des rôles (RBAC)
- Gestion complète des membres, équipes et entraîneurs
- Suivi des séances d'entraînement et des paiements
- Tableau de bord avec statistiques en temps réel extraites de MySQL
- Interface réactive et fluide avec React

## Architecture

Le projet suit une architecture client-serveur :
- **Backend** : Core business logic en C++, serveur HTTP multi-threadé, couche d'accès aux données (DAO) pour MySQL.
- **Frontend** : Application Single Page (SPA) moderne, gestion d'état avec Hooks, services API dédiés.

## Contribution

Les contributions sont les bienvenues ! Veuillez consulter le fichier CONTRIBUTING.md pour plus de détails.

## Licence

Ce projet n'a pas encore de licence spécifiée. Veuillez contacter les mainteneurs pour plus d'informations.
