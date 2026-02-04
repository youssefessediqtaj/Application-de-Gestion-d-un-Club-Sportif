#ifndef CLUBSPORTIF_H
#define CLUBSPORTIF_H

#include "Database.h"
#include "Entraineur.h"
#include "Equipe.h"
#include "Membre.h"
#include "Paiement.h"
#include "Seance.h"
#include "Utilisateur.h"
#include <fstream>
#include <string>
#include <vector>

class ClubSportif {
private:
  std::vector<Membre> membres;
  std::vector<Equipe> equipes;
  std::vector<Entraineur> entraineurs;
  std::vector<Paiement> paiements;
  std::vector<Seance> sessions;
  std::vector<Utilisateur> utilisateurs;
  int idCounter;
  Database *db; // Pointeur vers la base de données

  // Méthodes utilitaires
  int genererId();
  Membre *trouverMembre(int id);
  Equipe *trouverEquipe(int id);
  Entraineur *trouverEntraineur(int id);
  Seance *trouverSeance(int id);
  Paiement *trouverPaiement(int id);

public:
  // Constructeur
  ClubSportif();
  ~ClubSportif();

  // Méthodes pour la gestion des membres
  void ajouterMembre(const Membre &membre);
  void modifierMembre(int id, const Membre &nouveauMembre);
  void supprimerMembre(int id);
  Membre *chercherMembre(int id);
  std::vector<Membre> listerMembres() const;
  std::vector<Membre> rechercherMembresParNom(const std::string &nom) const;

  // Méthodes de base de données pour les membres
  int ajouterMembreDB(const Membre &membre);
  bool modifierMembreDB(int id, const Membre &nouveauMembre);
  bool supprimerMembreDB(int id);
  std::vector<Membre> listerMembresDB() const;

  // Méthodes pour la gestion des équipes
  void ajouterEquipe(const Equipe &equipe);
  void modifierEquipe(int id, const Equipe &nouvelleEquipe);
  void supprimerEquipe(int id);
  Equipe *chercherEquipe(int id);
  std::vector<Equipe> listerEquipes() const;
  void assignerMembreAEquipe(int idMembre, int idEquipe);
  void assignerCoachAEquipe(int idCoach, int idEquipe);

  // Méthodes de base de données pour les équipes
  int ajouterEquipeDB(const Equipe &equipe);
  bool modifierEquipeDB(int id, const Equipe &nouvelleEquipe);
  bool supprimerEquipeDB(int id);
  std::vector<Equipe> listerEquipesDB() const;

  // Méthodes pour la gestion des entraineurs
  void ajouterEntraineur(const Entraineur &entraineur);
  void modifierEntraineur(int id, const Entraineur &nouvelEntraineur);
  void supprimerEntraineur(int id);
  Entraineur *chercherEntraineur(int id);
  std::vector<Entraineur> listerEntraineurs() const;

  // Méthodes de base de données pour les entraineurs
  int ajouterEntraineurDB(const Entraineur &entraineur);
  bool modifierEntraineurDB(int id, const Entraineur &nouvelEntraineur);
  bool supprimerEntraineurDB(int id);
  std::vector<Entraineur> listerEntraineursDB() const;

  // Méthodes pour la gestion des séances
  void creerSeance(const Seance &seance);
  void modifierSeance(int id, const Seance &nouvelleSeance);
  void supprimerSeance(int id);
  Seance *chercherSeance(int id);
  std::vector<Seance> listerSeances() const;
  std::vector<Seance> getSeancesParEquipe(int idEquipe) const;

  // Méthodes de base de données pour les séances
  int creerSeanceDB(const Seance &seance);
  bool modifierSeanceDB(int id, const Seance &nouvelleSeance);
  bool supprimerSeanceDB(int id);
  std::vector<Seance> listerSeancesDB() const;

  // Méthodes pour la gestion des paiements
  void enregistrerPaiement(const Paiement &paiement);
  void modifierPaiement(int id, const Paiement &nouveauPaiement);
  void supprimerPaiement(int id);
  Paiement *chercherPaiement(int id);
  std::vector<Paiement> getPaiementsParMembre(int idMembre) const;
  bool membreEstEnRegle(int idMembre) const;

  // Méthodes de base de données pour les paiements
  int enregistrerPaiementDB(const Paiement &paiement);
  bool modifierPaiementDB(int id, const Paiement &nouveauPaiement);
  bool supprimerPaiementDB(int id);
  std::vector<Paiement> getPaiementsParMembreDB(int idMembre) const;

  // Méthodes pour la gestion des utilisateurs
  void ajouterUtilisateur(const Utilisateur &utilisateur);
  void modifierUtilisateur(int id, const Utilisateur &nouvelUtilisateur);
  void supprimerUtilisateur(int id);
  Utilisateur *chercherUtilisateur(int id);
  std::vector<Utilisateur> listerUtilisateurs() const;
  bool authentifierUtilisateur(const std::string &username,
                               const std::string &password,
                               Utilisateur &utilisateurConnecte);

  // Méthodes de base de données pour les utilisateurs
  int ajouterUtilisateurDB(const Utilisateur &utilisateur);
  bool modifierUtilisateurDB(int id, const Utilisateur &nouvelUtilisateur);
  bool supprimerUtilisateurDB(int id);
  std::vector<Utilisateur> listerUtilisateursDB() const;

  // Méthodes utilitaires
  void afficherStatistiques() const;
  int getNombreMembres() const;
  int getNombreEquipes() const;
  int getNombreEntraineurs() const;
  int getNombreSeances() const;
  int getNombrePaiements() const;

  // Méthodes pour les statistiques avec la base de données
  int getTotalMembresDB() const;
  int getTotalEquipesDB() const;
  int getTotalEntraineursDB() const;
  int getTotalSeancesDB() const;
  int getTotalPaiementsDB() const;
  int getMembresPayesDB() const;
  int getMembresImpayesDB() const;

  // Méthodes pour la persistance des données (migration vers SQL)
  void chargerDonnees();
  void sauvegarderDonnees();
  void chargerDonneesDB();
  void sauvegarderDonneesDB();
};

#endif // CLUBSPORTIF_H