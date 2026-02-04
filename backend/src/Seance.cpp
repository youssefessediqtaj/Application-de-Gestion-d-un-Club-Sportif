#include "../include/Seance.h"
#include <iostream>

// Constructeur par défaut
Seance::Seance() : id(0), idEquipe(0), date(""), heureDebut(""), duree(0), type(""), coach("") {}

// Constructeur avec paramètres
Seance::Seance(int id, int idEquipe, const std::string& date, const std::string& heureDebut, 
               int duree, const std::string& type, const std::string& coach)
    : id(id), idEquipe(idEquipe), date(date), heureDebut(heureDebut), 
      duree(duree), type(type), coach(coach) {}

// Getters
int Seance::getId() const {
    return id;
}

int Seance::getIdEquipe() const {
    return idEquipe;
}

std::string Seance::getDate() const {
    return date;
}

std::string Seance::getHeureDebut() const {
    return heureDebut;
}

int Seance::getDuree() const {
    return duree;
}

std::string Seance::getType() const {
    return type;
}

std::string Seance::getCoach() const {
    return coach;
}

// Setters
void Seance::setIdEquipe(int idEquipe) {
    this->idEquipe = idEquipe;
}

void Seance::setDate(const std::string& date) {
    this->date = date;
}

void Seance::setHeureDebut(const std::string& heureDebut) {
    this->heureDebut = heureDebut;
}

void Seance::setDuree(int duree) {
    if (duree > 0) {
        this->duree = duree;
    }
}

void Seance::setType(const std::string& type) {
    this->type = type;
}

void Seance::setCoach(const std::string& coach) {
    this->coach = coach;
}

// Méthodes
void Seance::afficher() const {
    std::cout << "ID: " << id << std::endl;
    std::cout << "ID Équipe: " << idEquipe << std::endl;
    std::cout << "Date: " << date << std::endl;
    std::cout << "Heure de début: " << heureDebut << std::endl;
    std::cout << "Durée: " << duree << " minutes" << std::endl;
    std::cout << "Type: " << type << std::endl;
    std::cout << "Coach: " << coach << std::endl;
}

std::string Seance::getHoraire() const {
    return date + " à " + heureDebut;
}