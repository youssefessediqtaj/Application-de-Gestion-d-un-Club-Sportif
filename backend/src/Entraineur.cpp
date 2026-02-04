#include "../include/Entraineur.h"
#include <iostream>

// Constructeur par défaut
Entraineur::Entraineur() : Personne(), specialite(""), salaire(0.0) {}

// Constructeur avec paramètres
Entraineur::Entraineur(int id, const std::string& nom, const std::string& prenom, 
                       const std::string& dateNaissance, const std::string& email, 
                       const std::string& telephone, const std::string& specialite, 
                       double salaire)
    : Personne(id, nom, prenom, dateNaissance, email, telephone),
      specialite(specialite), salaire(salaire) {}

// Getters
std::string Entraineur::getSpecialite() const {
    return specialite;
}

double Entraineur::getSalaire() const {
    return salaire;
}

// Setters
void Entraineur::setSpecialite(const std::string& specialite) {
    this->specialite = specialite;
}

void Entraineur::setSalaire(double salaire) {
    this->salaire = salaire;
}

// Méthodes virtuelles
void Entraineur::afficher() const {
    std::cout << "ID: " << getId() << std::endl;
    std::cout << "Nom: " << getNom() << std::endl;
    std::cout << "Prénom: " << getPrenom() << std::endl;
    std::cout << "Date de naissance: " << getDateNaissance() << std::endl;
    std::cout << "Email: " << getEmail() << std::endl;
    std::cout << "Téléphone: " << getTelephone() << std::endl;
    std::cout << "Spécialité: " << specialite << std::endl;
    std::cout << "Salaire: " << salaire << " €" << std::endl;
}

std::string Entraineur::getType() const {
    return "Entraineur";
}

// Méthodes spécifiques
void Entraineur::augmenterSalaire(double montant) {
    if (montant > 0) {
        salaire += montant;
    }
}

void Entraineur::changerSpecialite(const std::string& nouvelleSpecialite) {
    this->specialite = nouvelleSpecialite;
}