#include "../include/Membre.h"
#include <iostream>

// Constructeur par défaut
Membre::Membre() : Personne(), numeroMembre(""), dateInscription(""), statut("Actif"), cotisation(0.0) {}

// Constructeur avec paramètres
Membre::Membre(int id, const std::string& nom, const std::string& prenom, 
               const std::string& dateNaissance, const std::string& email, 
               const std::string& telephone, const std::string& numeroMembre, 
               const std::string& dateInscription, const std::string& statut, 
               double cotisation)
    : Personne(id, nom, prenom, dateNaissance, email, telephone),
      numeroMembre(numeroMembre), dateInscription(dateInscription), 
      statut(statut), cotisation(cotisation) {}

// Getters
std::string Membre::getNumeroMembre() const {
    return numeroMembre;
}

std::string Membre::getDateInscription() const {
    return dateInscription;
}

std::string Membre::getStatut() const {
    return statut;
}

double Membre::getCotisation() const {
    return cotisation;
}

// Setters
void Membre::setNumeroMembre(const std::string& numeroMembre) {
    this->numeroMembre = numeroMembre;
}

void Membre::setDateInscription(const std::string& dateInscription) {
    this->dateInscription = dateInscription;
}

void Membre::setStatut(const std::string& statut) {
    this->statut = statut;
}

void Membre::setCotisation(double cotisation) {
    this->cotisation = cotisation;
}

// Méthodes virtuelles
void Membre::afficher() const {
    std::cout << "ID: " << getId() << std::endl;
    std::cout << "Nom: " << getNom() << std::endl;
    std::cout << "Prénom: " << getPrenom() << std::endl;
    std::cout << "Date de naissance: " << getDateNaissance() << std::endl;
    std::cout << "Email: " << getEmail() << std::endl;
    std::cout << "Téléphone: " << getTelephone() << std::endl;
    std::cout << "Numéro de membre: " << numeroMembre << std::endl;
    std::cout << "Date d'inscription: " << dateInscription << std::endl;
    std::cout << "Statut: " << statut << std::endl;
    std::cout << "Cotisation: " << cotisation << " €" << std::endl;
}

std::string Membre::getType() const {
    return "Membre";
}

// Méthodes spécifiques
bool Membre::estEnRegle() const {
    return cotisation > 0; // Simplifié : considère qu'un membre est en règle s'il a une cotisation > 0
}

void Membre::modifierStatut(const std::string& nouveauStatut) {
    this->statut = nouveauStatut;
}