#include "../include/ClubSportif.h"
#include "../include/config.h"
#include <algorithm>
#include <cctype>
#include <iostream>

#include <iomanip>
#include <sstream>

// Constructeur
ClubSportif::ClubSportif() : idCounter(1) {
  db = new Database(Config::MYSQL_HOST, Config::MYSQL_USER,
                    Config::MYSQL_PASSWORD, Config::MYSQL_DATABASE,
                    Config::DB_PORT);
  if (db->connect()) {
    // Charger les données depuis la base de données
    chargerDonneesDB();
  } else {
    std::cerr << "Erreur critique: Impossible de connecter ClubSportif à MySQL"
              << std::endl;
  }
}

// Destructeur
ClubSportif::~ClubSportif() {
  if (db) {
    db->disconnect();
    delete db;
    db = nullptr;
  }
}

// Méthodes utilitaires privées
int ClubSportif::genererId() { return idCounter++; }

Membre *ClubSportif::trouverMembre(int id) {
  for (auto &membre : membres) {
    if (membre.getId() == id) {
      return &membre;
    }
  }
  return nullptr;
}

Equipe *ClubSportif::trouverEquipe(int id) {
  for (auto &equipe : equipes) {
    if (equipe.getId() == id) {
      return &equipe;
    }
  }
  return nullptr;
}

Entraineur *ClubSportif::trouverEntraineur(int id) {
  for (auto &entraineur : entraineurs) {
    if (entraineur.getId() == id) {
      return &entraineur;
    }
  }
  return nullptr;
}

Seance *ClubSportif::trouverSeance(int id) {
  for (auto &seance : sessions) {
    if (seance.getId() == id) {
      return &seance;
    }
  }
  return nullptr;
}

Paiement *ClubSportif::trouverPaiement(int id) {
  for (auto &paiement : paiements) {
    if (paiement.getId() == id) {
      return &paiement;
    }
  }
  return nullptr;
}

// Méthodes pour la gestion des membres
void ClubSportif::ajouterMembre(const Membre &membre) {
  Membre nouveauMembre = membre;
  nouveauMembre.afficher(); // Pour vérifier l'ID
  membres.push_back(nouveauMembre);
  ajouterMembreDB(membre); // Sauvegarder dans la base de données
  std::cout << "Membre ajouté avec succès!" << std::endl;
}

int ClubSportif::ajouterMembreDB(const Membre &membre) {
  if (db) {
    std::string query =
        "INSERT INTO membres (nom, prenom, date_naissance, email, telephone, "
        "numero_membre, date_inscription, statut, cotisation) VALUES ('" +
        membre.getNom() + "', '" + membre.getPrenom() + "', '" +
        membre.getDateNaissance() + "', '" + membre.getEmail() + "', '" +
        membre.getTelephone() + "', '" + membre.getNumeroMembre() + "', '" +
        membre.getDateInscription() + "', '" + membre.getStatut() + "', " +
        std::to_string(membre.getCotisation()) + ")";
    if (db->executeQuery(query)) {
      return (int)db->getLastInsertId();
    }
  }
  return -1;
}

void ClubSportif::modifierMembre(int id, const Membre &nouveauMembre) {
  for (auto &membre : membres) {
    if (membre.getId() == id) {
      membre = nouveauMembre;
      modifierMembreDB(id,
                       nouveauMembre); // Mettre à jour dans la base de données
      std::cout << "Membre modifié avec succès!" << std::endl;
      return;
    }
  }
  std::cout << "Membre non trouvé!" << std::endl;
}

bool ClubSportif::modifierMembreDB(int id, const Membre &nouveauMembre) {
  if (db) {
    std::string query =
        "UPDATE membres SET nom='" + nouveauMembre.getNom() + "', prenom='" +
        nouveauMembre.getPrenom() + "', date_naissance='" +
        nouveauMembre.getDateNaissance() + "', email='" +
        nouveauMembre.getEmail() + "', telephone='" +
        nouveauMembre.getTelephone() + "', numero_membre='" +
        nouveauMembre.getNumeroMembre() + "', date_inscription='" +
        nouveauMembre.getDateInscription() + "', statut='" +
        nouveauMembre.getStatut() +
        "', cotisation=" + std::to_string(nouveauMembre.getCotisation()) +
        " WHERE id=" + std::to_string(id);
    return db->executeQuery(query);
  }
  return false;
}

void ClubSportif::supprimerMembre(int id) {
  membres.erase(
      std::remove_if(membres.begin(), membres.end(),
                     [id](const Membre &m) { return m.getId() == id; }),
      membres.end());
  // Supprimer aussi les paiements associés à ce membre
  paiements.erase(
      std::remove_if(paiements.begin(), paiements.end(),
                     [id](const Paiement &p) { return p.getIdMembre() == id; }),
      paiements.end());
  supprimerMembreDB(id); // Supprimer de la base de données
  std::cout << "Membre supprimé avec succès!" << std::endl;
}

bool ClubSportif::supprimerMembreDB(int id) {
  if (db) {
    // Supprimer d'abord les paiements associés
    std::string deletePaiements =
        "DELETE FROM paiements WHERE membre_id=" + std::to_string(id);
    db->executeQuery(deletePaiements);

    // Puis supprimer le membre
    std::string deleteMembre =
        "DELETE FROM membres WHERE id=" + std::to_string(id);
    return db->executeQuery(deleteMembre);
  }
  return false;
}

Membre *ClubSportif::chercherMembre(int id) { return trouverMembre(id); }

std::vector<Membre> ClubSportif::listerMembres() const { return membres; }

std::vector<Membre> ClubSportif::listerMembresDB() const {
  std::vector<Membre> membresDB;
  if (db) {
    auto results = db->executeSelect(
        "SELECT id, nom, prenom, date_naissance, email, telephone, "
        "numero_membre, date_inscription, statut, cotisation FROM membres;");
    for (const auto &row : results) {
      int id = std::stoi(row[0]);
      Membre membre(id, row[1], row[2], row[3], row[4], row[5], row[6], row[7],
                    row[8], std::stod(row[9]));
      membresDB.push_back(membre);
    }
  }
  return membresDB;
}

std::vector<Membre>
ClubSportif::rechercherMembresParNom(const std::string &nom) const {
  std::vector<Membre> resultats;
  for (const auto &membre : membres) {
    std::string nomMembre = membre.getNom();
    // Convertir en minuscules pour la comparaison
    std::transform(nomMembre.begin(), nomMembre.end(), nomMembre.begin(),
                   ::tolower);
    std::string recherche = nom;
    std::transform(recherche.begin(), recherche.end(), recherche.begin(),
                   ::tolower);

    if (nomMembre.find(recherche) != std::string::npos) {
      resultats.push_back(membre);
    }
  }
  return resultats;
}

// Méthodes pour la gestion des équipes
void ClubSportif::ajouterEquipe(const Equipe &equipe) {
  equipes.push_back(equipe);
  ajouterEquipeDB(equipe); // Sauvegarder dans la base de données
  std::cout << "Équipe ajoutée avec succès!" << std::endl;
}

int ClubSportif::ajouterEquipeDB(const Equipe &equipe) {
  if (db) {
    std::string query = "INSERT INTO equipes (nom, sport, coach_id) VALUES ('" +
                        equipe.getNom() + "', '" + equipe.getSport() + "', " +
                        std::to_string(equipe.getCoach()) + ")";
    if (db->executeQuery(query)) {
      return (int)db->getLastInsertId();
    }
  }
  return -1;
}

void ClubSportif::modifierEquipe(int id, const Equipe &nouvelleEquipe) {
  for (auto &equipe : equipes) {
    if (equipe.getId() == id) {
      equipe = nouvelleEquipe;
      modifierEquipeDB(id,
                       nouvelleEquipe); // Mettre à jour dans la base de données
      std::cout << "Équipe modifiée avec succès!" << std::endl;
      return;
    }
  }
  std::cout << "Équipe non trouvée!" << std::endl;
}

bool ClubSportif::modifierEquipeDB(int id, const Equipe &nouvelleEquipe) {
  if (db) {
    std::string query =
        "UPDATE equipes SET nom='" + nouvelleEquipe.getNom() + "', sport='" +
        nouvelleEquipe.getSport() +
        "', coach_id=" + std::to_string(nouvelleEquipe.getCoach()) +
        " WHERE id=" + std::to_string(id);
    return db->executeQuery(query);
  }
  return false;
}

void ClubSportif::supprimerEquipe(int id) {
  equipes.erase(
      std::remove_if(equipes.begin(), equipes.end(),
                     [id](const Equipe &e) { return e.getId() == id; }),
      equipes.end());
  // Supprimer aussi les séances associées à cette équipe
  sessions.erase(
      std::remove_if(sessions.begin(), sessions.end(),
                     [id](const Seance &s) { return s.getIdEquipe() == id; }),
      sessions.end());
  supprimerEquipeDB(id); // Supprimer de la base de données
  std::cout << "Équipe supprimée avec succès!" << std::endl;
}

bool ClubSportif::supprimerEquipeDB(int id) {
  if (db) {
    // Supprimer d'abord les séances associées
    std::string deleteSeances =
        "DELETE FROM seances WHERE equipe_id=" + std::to_string(id);
    db->executeQuery(deleteSeances);

    // Puis supprimer l'équipe
    std::string deleteEquipe =
        "DELETE FROM equipes WHERE id=" + std::to_string(id);
    return db->executeQuery(deleteEquipe);
  }
  return false;
}

Equipe *ClubSportif::chercherEquipe(int id) { return trouverEquipe(id); }

std::vector<Equipe> ClubSportif::listerEquipes() const { return equipes; }

std::vector<Equipe> ClubSportif::listerEquipesDB() const {
  std::vector<Equipe> equipesDB;
  if (db) {
    auto results =
        db->executeSelect("SELECT id, nom, sport, coach_id FROM equipes;");
    for (const auto &row : results) {
      int id = std::stoi(row[0]);
      int coachId = row[3].empty() ? 0 : std::stoi(row[3]);
      Equipe equipe(id, row[1], row[2], coachId);
      equipesDB.push_back(equipe);
    }
  }
  return equipesDB;
}

void ClubSportif::assignerMembreAEquipe(int idMembre, int idEquipe) {
  Equipe *equipe = trouverEquipe(idEquipe);
  if (equipe != nullptr) {
    equipe->addMembre(idMembre);
    std::cout << "Membre assigné à l'équipe avec succès!" << std::endl;
  } else {
    std::cout << "Équipe non trouvée!" << std::endl;
  }
}

void ClubSportif::assignerCoachAEquipe(int idCoach, int idEquipe) {
  Equipe *equipe = trouverEquipe(idEquipe);
  if (equipe != nullptr) {
    equipe->setCoach(idCoach);
    std::cout << "Coach assigné à l'équipe avec succès!" << std::endl;
  } else {
    std::cout << "Équipe non trouvée!" << std::endl;
  }
}

// Méthodes pour la gestion des entraineurs
void ClubSportif::ajouterEntraineur(const Entraineur &entraineur) {
  entraineurs.push_back(entraineur);
  ajouterEntraineurDB(entraineur); // Sauvegarder dans la base de données
  std::cout << "Entraineur ajouté avec succès!" << std::endl;
}

int ClubSportif::ajouterEntraineurDB(const Entraineur &entraineur) {
  if (db) {
    std::string query = "INSERT INTO entraineurs (nom, prenom, date_naissance, "
                        "email, telephone, specialite, salaire) VALUES ('" +
                        entraineur.getNom() + "', '" + entraineur.getPrenom() +
                        "', '" + entraineur.getDateNaissance() + "', '" +
                        entraineur.getEmail() + "', '" +
                        entraineur.getTelephone() + "', '" +
                        entraineur.getSpecialite() + "', " +
                        std::to_string(entraineur.getSalaire()) + ")";
    if (db->executeQuery(query)) {
      return (int)db->getLastInsertId();
    }
  }
  return -1;
}

void ClubSportif::modifierEntraineur(int id,
                                     const Entraineur &nouvelEntraineur) {
  for (auto &entraineur : entraineurs) {
    if (entraineur.getId() == id) {
      entraineur = nouvelEntraineur;
      modifierEntraineurDB(
          id, nouvelEntraineur); // Mettre à jour dans la base de données
      std::cout << "Entraineur modifié avec succès!" << std::endl;
      return;
    }
  }
  std::cout << "Entraineur non trouvé!" << std::endl;
}

bool ClubSportif::modifierEntraineurDB(int id,
                                       const Entraineur &nouvelEntraineur) {
  if (db) {
    std::string query =
        "UPDATE entraineurs SET nom='" + nouvelEntraineur.getNom() +
        "', prenom='" + nouvelEntraineur.getPrenom() + "', date_naissance='" +
        nouvelEntraineur.getDateNaissance() + "', email='" +
        nouvelEntraineur.getEmail() + "', telephone='" +
        nouvelEntraineur.getTelephone() + "', specialite='" +
        nouvelEntraineur.getSpecialite() +
        "', salaire=" + std::to_string(nouvelEntraineur.getSalaire()) +
        " WHERE id=" + std::to_string(id);
    return db->executeQuery(query);
  }
  return false;
}

void ClubSportif::supprimerEntraineur(int id) {
  entraineurs.erase(
      std::remove_if(entraineurs.begin(), entraineurs.end(),
                     [id](const Entraineur &e) { return e.getId() == id; }),
      entraineurs.end());
  supprimerEntraineurDB(id); // Supprimer de la base de données
  std::cout << "Entraineur supprimé avec succès!" << std::endl;
}

bool ClubSportif::supprimerEntraineurDB(int id) {
  if (db) {
    // Supprimer d'abord les équipes associées à ce coach
    std::string updateEquipes =
        "UPDATE equipes SET coach_id=NULL WHERE coach_id=" + std::to_string(id);
    db->executeQuery(updateEquipes);

    // Puis supprimer l'entraineur
    std::string deleteEntraineur =
        "DELETE FROM entraineurs WHERE id=" + std::to_string(id);
    return db->executeQuery(deleteEntraineur);
  }
  return false;
}

Entraineur *ClubSportif::chercherEntraineur(int id) {
  return trouverEntraineur(id);
}

std::vector<Entraineur> ClubSportif::listerEntraineurs() const {
  return entraineurs;
}

std::vector<Entraineur> ClubSportif::listerEntraineursDB() const {
  std::vector<Entraineur> entraineursDB;
  if (db) {
    auto results =
        db->executeSelect("SELECT id, nom, prenom, date_naissance, email, "
                          "telephone, specialite, salaire FROM entraineurs;");
    for (const auto &row : results) {
      int id = std::stoi(row[0]);
      Entraineur entraineur(id, row[1], row[2], row[3], row[4], row[5], row[6],
                            std::stod(row[7]));
      entraineursDB.push_back(entraineur);
    }
  }
  return entraineursDB;
}

// Méthodes pour la gestion des séances
void ClubSportif::creerSeance(const Seance &seance) {
  sessions.push_back(seance);
  creerSeanceDB(seance); // Sauvegarder dans la base de données
  std::cout << "Séance créée avec succès!" << std::endl;
}

int ClubSportif::creerSeanceDB(const Seance &seance) {
  if (db) {
    std::string query = "INSERT INTO seances (equipe_id, date, heure_debut, "
                        "duree, type, coach) VALUES (" +
                        std::to_string(seance.getIdEquipe()) + ", '" +
                        seance.getDate() + "', '" + seance.getHeureDebut() +
                        "', " + std::to_string(seance.getDuree()) + ", '" +
                        seance.getType() + "', '" + seance.getCoach() + "')";
    if (db->executeQuery(query)) {
      return (int)db->getLastInsertId();
    }
  }
  return -1;
}

void ClubSportif::modifierSeance(int id, const Seance &nouvelleSeance) {
  for (auto &seance : sessions) {
    if (seance.getId() == id) {
      seance = nouvelleSeance;
      modifierSeanceDB(id,
                       nouvelleSeance); // Mettre à jour dans la base de données
      std::cout << "Séance modifiée avec succès!" << std::endl;
      return;
    }
  }
  std::cout << "Séance non trouvée!" << std::endl;
}

bool ClubSportif::modifierSeanceDB(int id, const Seance &nouvelleSeance) {
  if (db) {
    std::string query =
        "UPDATE seances SET equipe_id=" +
        std::to_string(nouvelleSeance.getIdEquipe()) + ", date='" +
        nouvelleSeance.getDate() + "', heure_debut='" +
        nouvelleSeance.getHeureDebut() +
        "', duree=" + std::to_string(nouvelleSeance.getDuree()) + ", type='" +
        nouvelleSeance.getType() + "', coach='" + nouvelleSeance.getCoach() +
        "' WHERE id=" + std::to_string(id);
    return db->executeQuery(query);
  }
  return false;
}

void ClubSportif::supprimerSeance(int id) {
  sessions.erase(
      std::remove_if(sessions.begin(), sessions.end(),
                     [id](const Seance &s) { return s.getId() == id; }),
      sessions.end());
  supprimerSeanceDB(id); // Supprimer de la base de données
  std::cout << "Séance supprimée avec succès!" << std::endl;
}

bool ClubSportif::supprimerSeanceDB(int id) {
  if (db) {
    // Supprimer la séance
    std::string deleteSeance =
        "DELETE FROM seances WHERE id=" + std::to_string(id);
    return db->executeQuery(deleteSeance);
  }
  return false;
}

Seance *ClubSportif::chercherSeance(int id) { return trouverSeance(id); }

std::vector<Seance> ClubSportif::listerSeances() const { return sessions; }

std::vector<Seance> ClubSportif::listerSeancesDB() const {
  std::vector<Seance> seancesDB;
  if (db) {
    auto results = db->executeSelect("SELECT id, equipe_id, date, heure_debut, "
                                     "duree, type, coach FROM seances;");
    for (const auto &row : results) {
      int id = std::stoi(row[0]);
      int equipeId = std::stoi(row[1]);
      int duree = std::stoi(row[4]);
      Seance seance(id, equipeId, row[2], row[3], duree, row[5], row[6]);
      seancesDB.push_back(seance);
    }
  }
  return seancesDB;
}

std::vector<Seance> ClubSportif::getSeancesParEquipe(int idEquipe) const {
  std::vector<Seance> resultats;
  for (const auto &seance : sessions) {
    if (seance.getIdEquipe() == idEquipe) {
      resultats.push_back(seance);
    }
  }
  return resultats;
}

// Méthodes pour la gestion des paiements
void ClubSportif::enregistrerPaiement(const Paiement &paiement) {
  paiements.push_back(paiement);
  enregistrerPaiementDB(paiement); // Sauvegarder dans la base de données
  std::cout << "Paiement enregistré avec succès!" << std::endl;
}

int ClubSportif::enregistrerPaiementDB(const Paiement &paiement) {
  if (db) {
    std::string query = "INSERT INTO paiements (membre_id, montant, date, "
                        "statut, type) VALUES (" +
                        std::to_string(paiement.getIdMembre()) + ", " +
                        std::to_string(paiement.getMontant()) + ", '" +
                        paiement.getDate() + "', '" + paiement.getStatut() +
                        "', '" + paiement.getType() + "')";
    if (db->executeQuery(query)) {
      return (int)db->getLastInsertId();
    }
  }
  return -1;
}

void ClubSportif::modifierPaiement(int id, const Paiement &nouveauPaiement) {
  for (auto &paiement : paiements) {
    if (paiement.getId() == id) {
      paiement = nouveauPaiement;
      modifierPaiementDB(
          id, nouveauPaiement); // Mettre à jour dans la base de données
      std::cout << "Paiement modifié avec succès!" << std::endl;
      return;
    }
  }
  std::cout << "Paiement non trouvé!" << std::endl;
}

bool ClubSportif::modifierPaiementDB(int id, const Paiement &nouveauPaiement) {
  if (db) {
    std::string query =
        "UPDATE paiements SET membre_id=" +
        std::to_string(nouveauPaiement.getIdMembre()) +
        ", montant=" + std::to_string(nouveauPaiement.getMontant()) +
        ", date='" + nouveauPaiement.getDate() + "', statut='" +
        nouveauPaiement.getStatut() + "', type='" + nouveauPaiement.getType() +
        "' WHERE id=" + std::to_string(id);
    return db->executeQuery(query);
  }
  return false;
}

void ClubSportif::supprimerPaiement(int id) {
  paiements.erase(
      std::remove_if(paiements.begin(), paiements.end(),
                     [id](const Paiement &p) { return p.getId() == id; }),
      paiements.end());
  supprimerPaiementDB(id); // Supprimer de la base de données
  std::cout << "Paiement supprimé avec succès!" << std::endl;
}

bool ClubSportif::supprimerPaiementDB(int id) {
  if (db) {
    // Supprimer le paiement
    std::string deletePaiement =
        "DELETE FROM paiements WHERE id=" + std::to_string(id);
    return db->executeQuery(deletePaiement);
  }
  return false;
}

Paiement *ClubSportif::chercherPaiement(int id) { return trouverPaiement(id); }

std::vector<Paiement> ClubSportif::getPaiementsParMembre(int idMembre) const {
  std::vector<Paiement> resultats;
  for (const auto &paiement : paiements) {
    if (paiement.getIdMembre() == idMembre) {
      resultats.push_back(paiement);
    }
  }
  return resultats;
}

std::vector<Paiement> ClubSportif::getPaiementsParMembreDB(int idMembre) const {
  std::vector<Paiement> paiementsDB;
  if (db) {
    auto results =
        db->executeSelect("SELECT id, membre_id, montant, date, statut, type "
                          "FROM paiements WHERE membre_id=" +
                          std::to_string(idMembre));
    for (const auto &row : results) {
      int id = std::stoi(row[0]);
      Paiement paiement(id, std::stoi(row[1]), std::stod(row[2]), row[3],
                        row[4], row[5]);
      paiementsDB.push_back(paiement);
    }
  }
  return paiementsDB;
}

bool ClubSportif::membreEstEnRegle(int idMembre) const {
  std::vector<Paiement> paiementsMembre = getPaiementsParMembre(idMembre);
  for (const auto &paiement : paiementsMembre) {
    if (!paiement.estPaye()) {
      return false; // Si un seul paiement n'est pas payé, le membre n'est pas
                    // en règle
    }
  }
  return true; // Tous les paiements sont payés
}

// Méthodes utilitaires
void ClubSportif::afficherStatistiques() const {
  std::cout << "\n=== Statistiques du Club ===" << std::endl;
  std::cout << "Nombre de membres: " << getNombreMembres() << std::endl;
  std::cout << "Nombre d'équipes: " << getNombreEquipes() << std::endl;
  std::cout << "Nombre d'entraineurs: " << getNombreEntraineurs() << std::endl;
  std::cout << "Nombre de séances: " << getNombreSeances() << std::endl;
  std::cout << "Nombre de paiements: " << getNombrePaiements() << std::endl;

  // Afficher les statistiques depuis la base de données
  std::cout << "\n=== Statistiques depuis la base de données ===" << std::endl;
  std::cout << "Total membres (DB): " << getTotalMembresDB() << std::endl;
  std::cout << "Total équipes (DB): " << getTotalEquipesDB() << std::endl;
  std::cout << "Total entraineurs (DB): " << getTotalEntraineursDB()
            << std::endl;
  std::cout << "Total séances (DB): " << getTotalSeancesDB() << std::endl;
  std::cout << "Total paiements (DB): " << getTotalPaiementsDB() << std::endl;
  std::cout << "Membres en règle (payés): " << getMembresPayesDB() << std::endl;
  std::cout << "Membres en impayé: " << getMembresImpayesDB() << std::endl;
}

int ClubSportif::getNombreMembres() const { return membres.size(); }

int ClubSportif::getNombreEquipes() const { return equipes.size(); }

int ClubSportif::getNombreEntraineurs() const { return entraineurs.size(); }

int ClubSportif::getNombreSeances() const { return sessions.size(); }

int ClubSportif::getNombrePaiements() const { return paiements.size(); }

// Méthodes pour la persistance des données
void ClubSportif::chargerDonnees() {
  chargerDonneesDB();
  std::cout << "Données chargées avec succès!" << std::endl;
}

void ClubSportif::sauvegarderDonnees() {
  sauvegarderDonneesDB();
  std::cout << "Données sauvegardées avec succès!" << std::endl;
}

void ClubSportif::chargerDonneesDB() {
  // Charger les membres depuis la base de données
  membres.clear();
  auto membresData = db->executeSelect(
      "SELECT id, nom, prenom, date_naissance, email, telephone, "
      "numero_membre, date_inscription, statut, cotisation FROM membres;");
  for (const auto &row : membresData) {
    int id = std::stoi(row[0]);
    Membre membre(id, row[1], row[2], row[3], row[4], row[5], row[6], row[7],
                  row[8], std::stod(row[9]));
    membres.push_back(membre);
    if (id >= idCounter) {
      idCounter = id + 1;
    }
  }

  // Charger les équipes depuis la base de données
  equipes.clear();
  auto equipesData =
      db->executeSelect("SELECT id, nom, sport, coach_id FROM equipes;");
  for (const auto &row : equipesData) {
    int id = std::stoi(row[0]);
    int coachId = row[3].empty() ? 0 : std::stoi(row[3]);
    Equipe equipe(id, row[1], row[2], coachId);
    equipes.push_back(equipe);
    if (id >= idCounter) {
      idCounter = id + 1;
    }
  }

  // Charger les entraineurs depuis la base de données
  entraineurs.clear();
  auto entraineursData =
      db->executeSelect("SELECT id, nom, prenom, date_naissance, email, "
                        "telephone, specialite, salaire FROM entraineurs;");
  for (const auto &row : entraineursData) {
    int id = std::stoi(row[0]);
    Entraineur entraineur(id, row[1], row[2], row[3], row[4], row[5], row[6],
                          std::stod(row[7]));
    entraineurs.push_back(entraineur);
    if (id >= idCounter) {
      idCounter = id + 1;
    }
  }

  // Charger les séances depuis la base de données
  sessions.clear();
  auto seancesData =
      db->executeSelect("SELECT id, equipe_id, date, heure_debut, duree, type, "
                        "coach FROM seances;");
  for (const auto &row : seancesData) {
    int id = std::stoi(row[0]);
    int equipeId = std::stoi(row[1]);
    int duree = std::stoi(row[4]);
    Seance seance(id, equipeId, row[2], row[3], duree, row[5], row[6]);
    sessions.push_back(seance);
    if (id >= idCounter) {
      idCounter = id + 1;
    }
  }

  // Charger les paiements depuis la base de données
  paiements.clear();
  auto paiementsData = db->executeSelect(
      "SELECT id, membre_id, montant, date, statut, type FROM paiements;");
  for (const auto &row : paiementsData) {
    int id = std::stoi(row[0]);
    int membreId = std::stoi(row[1]);
    Paiement paiement(id, membreId, std::stod(row[2]), row[3], row[4], row[5]);
    paiements.push_back(paiement);
    if (id >= idCounter) {
      idCounter = id + 1;
    }
  }

  // Charger les utilisateurs depuis la base de données
  utilisateurs.clear();
  auto utilisateursData = db->executeSelect(
      "SELECT id, username, password, role FROM utilisateurs;");
  for (const auto &row : utilisateursData) {
    int id = std::stoi(row[0]);
    Utilisateur utilisateur(id, row[1], row[2], row[3]);
    utilisateurs.push_back(utilisateur);
    if (id >= idCounter) {
      idCounter = id + 1;
    }
  }
}

void ClubSportif::sauvegarderDonneesDB() {
  // Les données sont maintenant automatiquement sauvegardées dans la base de
  // données via les opérations CRUD, donc cette méthode est principalement pour
  // la compatibilité
  std::cout << "Données sauvegardées dans la base de données" << std::endl;
}

// Méthodes pour les statistiques avec la base de données
int ClubSportif::getTotalMembresDB() const { return db->getTotalMembres(); }

int ClubSportif::getTotalEquipesDB() const { return db->getTotalEquipes(); }

int ClubSportif::getTotalEntraineursDB() const {
  return db->getTotalEntraineurs();
}

int ClubSportif::getTotalSeancesDB() const { return db->getTotalSeances(); }

int ClubSportif::getTotalPaiementsDB() const { return db->getTotalPaiements(); }

int ClubSportif::getMembresPayesDB() const { return db->getMembresPayes(); }

int ClubSportif::getMembresImpayesDB() const { return db->getMembresImpayes(); }

// Méthodes pour la gestion des utilisateurs
void ClubSportif::ajouterUtilisateur(const Utilisateur &utilisateur) {
  utilisateurs.push_back(utilisateur);
  ajouterUtilisateurDB(utilisateur); // Sauvegarder dans la base de données
  std::cout << "Utilisateur ajouté avec succès!" << std::endl;
}

int ClubSportif::ajouterUtilisateurDB(const Utilisateur &utilisateur) {
  if (db) {
    std::string query =
        "INSERT INTO utilisateurs (username, password, role) VALUES ('" +
        utilisateur.getUsername() + "', '" + utilisateur.getPassword() +
        "', '" + utilisateur.getRole() + "')";
    if (db->executeQuery(query)) {
      return (int)db->getLastInsertId();
    }
  }
  return -1;
}

void ClubSportif::modifierUtilisateur(int id,
                                      const Utilisateur &nouvelUtilisateur) {
  for (auto &utilisateur : utilisateurs) {
    if (utilisateur.getId() == id) {
      utilisateur = nouvelUtilisateur;
      modifierUtilisateurDB(
          id, nouvelUtilisateur); // Mettre à jour dans la base de données
      std::cout << "Utilisateur modifié avec succès!" << std::endl;
      return;
    }
  }
  std::cout << "Utilisateur non trouvé!" << std::endl;
}

bool ClubSportif::modifierUtilisateurDB(int id,
                                        const Utilisateur &nouvelUtilisateur) {
  if (db) {
    std::string query =
        "UPDATE utilisateurs SET username='" + nouvelUtilisateur.getUsername() +
        "', password='" + nouvelUtilisateur.getPassword() + "', role='" +
        nouvelUtilisateur.getRole() + "' WHERE id=" + std::to_string(id);
    return db->executeQuery(query);
  }
  return false;
}

void ClubSportif::supprimerUtilisateur(int id) {
  utilisateurs.erase(
      std::remove_if(utilisateurs.begin(), utilisateurs.end(),
                     [id](const Utilisateur &u) { return u.getId() == id; }),
      utilisateurs.end());
  supprimerUtilisateurDB(id); // Supprimer de la base de données
  std::cout << "Utilisateur supprimé avec succès!" << std::endl;
}

bool ClubSportif::supprimerUtilisateurDB(int id) {
  if (db) {
    // Supprimer l'utilisateur
    std::string deleteUtilisateur =
        "DELETE FROM utilisateurs WHERE id=" + std::to_string(id);
    return db->executeQuery(deleteUtilisateur);
  }
  return false;
}

Utilisateur *ClubSportif::chercherUtilisateur(int id) {
  for (auto &utilisateur : utilisateurs) {
    if (utilisateur.getId() == id) {
      return &utilisateur;
    }
  }
  return nullptr;
}

std::vector<Utilisateur> ClubSportif::listerUtilisateurs() const {
  return utilisateurs;
}

std::vector<Utilisateur> ClubSportif::listerUtilisateursDB() const {
  std::vector<Utilisateur> utilisateursDB;
  if (db) {
    auto results = db->executeSelect(
        "SELECT id, username, password, role FROM utilisateurs;");
    for (const auto &row : results) {
      int id = std::stoi(row[0]);
      Utilisateur utilisateur(id, row[1], row[2], row[3]);
      utilisateursDB.push_back(utilisateur);
    }
  }
  return utilisateursDB;
}

bool ClubSportif::authentifierUtilisateur(const std::string &username,
                                          const std::string &password,
                                          Utilisateur &utilisateurConnecte) {
  if (db) {
    // Rechercher l'utilisateur dans la base de données
    std::string query = "SELECT id, username, password, role FROM utilisateurs "
                        "WHERE username='" +
                        username + "';";
    auto results = db->executeSelect(query);

    if (!results.empty()) {
      // Trouvé dans la base de données
      int id = std::stoi(results[0][0]);
      std::string dbUsername = results[0][1];
      std::string dbPassword = results[0][2];
      std::string role = results[0][3];

      // Créer un utilisateur temporaire pour vérifier le mot de passe
      Utilisateur tempUser(id, dbUsername, dbPassword, role);
      if (tempUser.verifyPassword(password)) {
        utilisateurConnecte = tempUser; // Copie de l'utilisateur trouvé
        return true;
      }
    }
  }

  // Sinon, chercher dans la liste locale (compatibilité descendante)
  for (const auto &utilisateur : utilisateurs) {
    if (utilisateur.getUsername() == username &&
        utilisateur.verifyPassword(password)) {
      utilisateurConnecte = utilisateur; // Copie de l'utilisateur trouvé
      return true;
    }
  }
  return false; // Identifiants incorrects
}
