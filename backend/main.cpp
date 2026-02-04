#include "include/ClubSportif.h"
#include "include/Database.h"
#include "include/config.h"
#include "include/server/HttpServer.h"
#include <iostream>
#include <limits>
#include <string>

using namespace std;

// Fonction pour nettoyer le buffer d'entrée
void clearInputBuffer() {
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Fonction pour afficher le menu principal
void afficherMenuPrincipal() {
  cout << "\n===========================================" << endl;
  cout << "      APPLICATION DE GESTION D'UN CLUB SPORTIF" << endl;
  cout << "===========================================" << endl;
  cout << "1. Gestion des Membres" << endl;
  cout << "2. Gestion des Équipes" << endl;
  cout << "3. Gestion des Entraineurs" << endl;
  cout << "4. Gestion des Séances" << endl;
  cout << "5. Gestion des Paiements" << endl;
  cout << "6. Afficher les Statistiques" << endl;
  cout << "7. Charger les Données" << endl;
  cout << "8. Sauvegarder les Données" << endl;
  cout << "0. Quitter" << endl;
  cout << "===========================================" << endl;
  cout << "Choisissez une option: ";
}

// Fonction pour afficher le menu des membres
void afficherMenuMembres() {
  cout << "\n--- Gestion des Membres ---" << endl;
  cout << "1. Ajouter un membre" << endl;
  cout << "2. Modifier un membre" << endl;
  cout << "3. Supprimer un membre" << endl;
  cout << "4. Lister les membres" << endl;
  cout << "5. Rechercher un membre" << endl;
  cout << "0. Retour au menu principal" << endl;
  cout << "Choisissez une option: ";
}

// Fonction pour afficher le menu des équipes
void afficherMenuEquipes() {
  cout << "\n--- Gestion des Équipes ---" << endl;
  cout << "1. Ajouter une équipe" << endl;
  cout << "2. Modifier une équipe" << endl;
  cout << "3. Supprimer une équipe" << endl;
  cout << "4. Lister les équipes" << endl;
  cout << "5. Assigner un membre à une équipe" << endl;
  cout << "6. Assigner un coach à une équipe" << endl;
  cout << "0. Retour au menu principal" << endl;
  cout << "Choisissez une option: ";
}

// Fonction pour afficher le menu des entraineurs
void afficherMenuEntraineurs() {
  cout << "\n--- Gestion des Entraineurs ---" << endl;
  cout << "1. Ajouter un entraineur" << endl;
  cout << "2. Modifier un entraineur" << endl;
  cout << "3. Supprimer un entraineur" << endl;
  cout << "4. Lister les entraineurs" << endl;
  cout << "0. Retour au menu principal" << endl;
  cout << "Choisissez une option: ";
}

// Fonction pour afficher le menu des séances
void afficherMenuSeances() {
  cout << "\n--- Gestion des Séances ---" << endl;
  cout << "1. Créer une séance" << endl;
  cout << "2. Modifier une séance" << endl;
  cout << "3. Supprimer une séance" << endl;
  cout << "4. Lister les séances" << endl;
  cout << "0. Retour au menu principal" << endl;
  cout << "Choisissez une option: ";
}

// Fonction pour afficher le menu des paiements
void afficherMenuPaiements() {
  cout << "\n--- Gestion des Paiements ---" << endl;
  cout << "1. Enregistrer un paiement" << endl;
  cout << "2. Lister les paiements d'un membre" << endl;
  cout << "0. Retour au menu principal" << endl;
  cout << "Choisissez une option: ";
}

int main() {
  ClubSportif club;

  // Charger les données au démarrage
  club.chargerDonnees();

  // Initialiser la base de données MySQL
  Database db(Config::MYSQL_HOST, Config::MYSQL_USER, Config::MYSQL_PASSWORD,
              Config::MYSQL_DATABASE, Config::DB_PORT);
  if (db.connect()) {
    // Les tables existent déjà dans MySQL (créées via schema_mysql.sql)
    std::cout << "Base de données MySQL initialisée avec succès" << std::endl;
    db.disconnect();
  } else {
    std::cerr << "Erreur: Impossible de se connecter à MySQL" << std::endl;
  }

  // Démarrer le serveur HTTP
  HttpServer server(8080, &club);
  server.start();

  std::cout << "Serveur HTTP démarré sur le port 8080" << std::endl;
  std::cout << "L'application continue à fonctionner en arrière-plan..."
            << std::endl;

  // Le serveur continuera à fonctionner en arrière-plan
  // On peut ajouter une boucle ici si on veut garder l'application interactive
  std::string input;
  std::cout << "Tapez 'quit' pour arrêter le serveur: ";
  while (std::getline(std::cin, input)) {
    if (input == "quit") {
      break;
    }
    std::cout << "Tapez 'quit' pour arrêter le serveur: ";
  }

  server.stop();
  std::cout << "Serveur arrêté." << std::endl;

  return 0;
}