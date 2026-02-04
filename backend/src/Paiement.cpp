#include "../include/Paiement.h"
#include <iostream>

// Constructeur par défaut
Paiement::Paiement() : id(0), idMembre(0), montant(0.0), date(""), statut("Non payé"), type("Cotisation") {}

// Constructeur avec paramètres
Paiement::Paiement(int id, int idMembre, double montant, const std::string& date, 
                   const std::string& statut, const std::string& type)
    : id(id), idMembre(idMembre), montant(montant), date(date), 
      statut(statut), type(type) {}

// Getters
int Paiement::getId() const {
    return id;
}

int Paiement::getIdMembre() const {
    return idMembre;
}

double Paiement::getMontant() const {
    return montant;
}

std::string Paiement::getDate() const {
    return date;
}

std::string Paiement::getStatut() const {
    return statut;
}

std::string Paiement::getType() const {
    return type;
}

// Setters
void Paiement::setIdMembre(int idMembre) {
    this->idMembre = idMembre;
}

void Paiement::setMontant(double montant) {
    if (montant >= 0) {
        this->montant = montant;
    }
}

void Paiement::setDate(const std::string& date) {
    this->date = date;
}

void Paiement::setStatut(const std::string& statut) {
    this->statut = statut;
}

void Paiement::setType(const std::string& type) {
    this->type = type;
}

// Méthodes
void Paiement::afficher() const {
    std::cout << "ID: " << id << std::endl;
    std::cout << "ID Membre: " << idMembre << std::endl;
    std::cout << "Montant: " << montant << " €" << std::endl;
    std::cout << "Date: " << date << std::endl;
    std::cout << "Statut: " << statut << std::endl;
    std::cout << "Type: " << type << std::endl;
}

bool Paiement::estPaye() const {
    return statut == "Payé";
}

void Paiement::marquerCommePaye() {
    statut = "Payé";
}

void Paiement::marquerCommeNonPaye() {
    statut = "Non payé";
}