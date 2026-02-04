#ifndef MEMBRE_H
#define MEMBRE_H

#include "Personne.h"
#include <string>

class Membre : public Personne {
private:
    std::string numeroMembre;
    std::string dateInscription;
    std::string statut; // Actif, Inactif, Suspendu
    double cotisation; // Montant de la cotisation annuelle

public:
    // Constructeurs
    Membre();
    Membre(int id, const std::string& nom, const std::string& prenom, 
           const std::string& dateNaissance, const std::string& email, 
           const std::string& telephone, const std::string& numeroMembre, 
           const std::string& dateInscription, const std::string& statut, 
           double cotisation);

    // Getters
    std::string getNumeroMembre() const;
    std::string getDateInscription() const;
    std::string getStatut() const;
    double getCotisation() const;

    // Setters
    void setNumeroMembre(const std::string& numeroMembre);
    void setDateInscription(const std::string& dateInscription);
    void setStatut(const std::string& statut);
    void setCotisation(double cotisation);

    // Méthodes virtuelles
    void afficher() const override;
    std::string getType() const override;

    // Méthodes spécifiques
    bool estEnRegle() const; // Vérifie si la cotisation est payée
    void modifierStatut(const std::string& nouveauStatut);
};

#endif // MEMBRE_H