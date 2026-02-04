#ifndef ENTRAINEUR_H
#define ENTRAINEUR_H

#include "Personne.h"
#include <string>

class Entraineur : public Personne {
private:
    std::string specialite; // Sport ou discipline spécifique
    double salaire;

public:
    // Constructeurs
    Entraineur();
    Entraineur(int id, const std::string& nom, const std::string& prenom, 
               const std::string& dateNaissance, const std::string& email, 
               const std::string& telephone, const std::string& specialite, 
               double salaire);

    // Getters
    std::string getSpecialite() const;
    double getSalaire() const;

    // Setters
    void setSpecialite(const std::string& specialite);
    void setSalaire(double salaire);

    // Méthodes virtuelles
    void afficher() const override;
    std::string getType() const override;

    // Méthodes spécifiques
    void augmenterSalaire(double montant);
    void changerSpecialite(const std::string& nouvelleSpecialite);
};

#endif // ENTRAINEUR_H