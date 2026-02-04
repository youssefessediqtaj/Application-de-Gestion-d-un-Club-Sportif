#ifndef PAIEMENT_H
#define PAIEMENT_H

#include <string>

class Paiement {
private:
    int id;
    int idMembre; // ID du membre concerné
    double montant;
    std::string date; // Format: JJ/MM/AAAA
    std::string statut; // Payé, Non payé, En attente
    std::string type; // Cotisation, Frais d'inscription, etc.

public:
    // Constructeurs
    Paiement();
    Paiement(int id, int idMembre, double montant, const std::string& date, 
             const std::string& statut, const std::string& type);

    // Getters
    int getId() const;
    int getIdMembre() const;
    double getMontant() const;
    std::string getDate() const;
    std::string getStatut() const;
    std::string getType() const;

    // Setters
    void setIdMembre(int idMembre);
    void setMontant(double montant);
    void setDate(const std::string& date);
    void setStatut(const std::string& statut);
    void setType(const std::string& type);

    // Méthodes
    void afficher() const;
    bool estPaye() const;
    void marquerCommePaye();
    void marquerCommeNonPaye();
};

#endif // PAIEMENT_H