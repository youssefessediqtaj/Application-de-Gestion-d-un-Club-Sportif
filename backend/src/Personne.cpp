#include "../include/Personne.h"

// Constructeur par défaut
Personne::Personne() : id(0), nom(""), prenom(""), dateNaissance(""), email(""), telephone("") {}

// Constructeur avec paramètres
Personne::Personne(int id, const std::string& nom, const std::string& prenom, 
                   const std::string& dateNaissance, const std::string& email, 
                   const std::string& telephone) 
    : id(id), nom(nom), prenom(prenom), dateNaissance(dateNaissance), 
      email(email), telephone(telephone) {}
      
// Getters
int Personne::getId() const {
    return id;
}

std::string Personne::getNom() const {
    return nom;
}

std::string Personne::getPrenom() const {
    return prenom;
}

std::string Personne::getDateNaissance() const {
    return dateNaissance;
}

std::string Personne::getEmail() const {
    return email;
}

std::string Personne::getTelephone() const {
    return telephone;
}

// Setters
void Personne::setNom(const std::string& nom) {
    this->nom = nom;
}

void Personne::setPrenom(const std::string& prenom) {
    this->prenom = prenom;
}

void Personne::setDateNaissance(const std::string& dateNaissance) {
    this->dateNaissance = dateNaissance;
}

void Personne::setEmail(const std::string& email) {
    this->email = email;
}

void Personne::setTelephone(const std::string& telephone) {
    this->telephone = telephone;
}