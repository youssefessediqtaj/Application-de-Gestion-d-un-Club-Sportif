#include "../include/Equipe.h"
#include <iostream>

// Constructeur par défaut
Equipe::Equipe() : id(0), nom(""), sport(""), coach(0) {}

// Constructeur avec paramètres
Equipe::Equipe(int id, const std::string& nom, const std::string& sport, int coach)
    : id(id), nom(nom), sport(sport), coach(coach) {}

// Getters
int Equipe::getId() const {
    return id;
}

std::string Equipe::getNom() const {
    return nom;
}

std::string Equipe::getSport() const {
    return sport;
}

int Equipe::getCoach() const {
    return coach;
}

std::vector<int> Equipe::getMembres() const {
    return membres;
}

// Setters
void Equipe::setNom(const std::string& nom) {
    this->nom = nom;
}

void Equipe::setSport(const std::string& sport) {
    this->sport = sport;
}

void Equipe::setCoach(int coach) {
    this->coach = coach;
}

// Méthodes spécifiques
void Equipe::addMembre(int idMembre) {
    // Vérifier si le membre n'est pas déjà dans l'équipe
    for (int membreId : membres) {
        if (membreId == idMembre) {
            return; // Le membre est déjà dans l'équipe
        }
    }
    membres.push_back(idMembre);
}

void Equipe::removeMembre(int idMembre) {
    for (auto it = membres.begin(); it != membres.end(); ++it) {
        if (*it == idMembre) {
            membres.erase(it);
            break;
        }
    }
}

int Equipe::getNombreMembres() const {
    return membres.size();
}

void Equipe::afficher() const {
    std::cout << "ID: " << id << std::endl;
    std::cout << "Nom: " << nom << std::endl;
    std::cout << "Sport: " << sport << std::endl;
    std::cout << "Coach ID: " << coach << std::endl;
    std::cout << "Nombre de membres: " << getNombreMembres() << std::endl;
}