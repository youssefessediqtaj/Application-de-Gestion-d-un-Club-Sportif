#include "../include/Utilisateur.h"
#include <iostream>

// Constructeurs
Utilisateur::Utilisateur() : id(0), username(""), password(""), role("") {}

Utilisateur::Utilisateur(int id, const std::string& username, const std::string& password, const std::string& role)
    : id(id), username(username), password(password), role(role) {}

// Getters
int Utilisateur::getId() const {
    return id;
}

std::string Utilisateur::getUsername() const {
    return username;
}

std::string Utilisateur::getPassword() const {
    return password;
}

std::string Utilisateur::getRole() const {
    return role;
}

// Setters
void Utilisateur::setId(int id) {
    this->id = id;
}

void Utilisateur::setUsername(const std::string& username) {
    this->username = username;
}

void Utilisateur::setPassword(const std::string& password) {
    this->password = password;
}

void Utilisateur::setRole(const std::string& role) {
    this->role = role;
}

// Méthodes d'authentification
bool Utilisateur::verifyPassword(const std::string& inputPassword) const {
    return this->password == inputPassword;
}

bool Utilisateur::login(const std::string& inputUsername, const std::string& inputPassword) {
    if (this->username == inputUsername && verifyPassword(inputPassword)) {
        std::cout << "Connexion réussie pour l'utilisateur: " << username << std::endl;
        return true;
    }
    std::cout << "Échec de la connexion - identifiants incorrects" << std::endl;
    return false;
}

void Utilisateur::logout() {
    std::cout << "Déconnexion de l'utilisateur: " << username << std::endl;
    // Réinitialiser les informations d'authentification si nécessaire
}

// Méthodes utilitaires
bool Utilisateur::isAdmin() const {
    return role == "ADMIN";
}

bool Utilisateur::isCoach() const {
    return role == "ENTRAINEUR";
}

bool Utilisateur::isMember() const {
    return role == "MEMBRE";
}