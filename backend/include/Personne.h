#ifndef PERSONNE_H
#define PERSONNE_H

#include <string>

class Personne {
protected:
    int id;
    std::string nom;
    std::string prenom;
    std::string dateNaissance;
    std::string email;
    std::string telephone;

public:
    // Constructeur
    Personne();
    Personne(int id, const std::string& nom, const std::string& prenom, 
             const std::string& dateNaissance, const std::string& email, 
             const std::string& telephone);

    // Getters
    int getId() const;
    std::string getNom() const;
    std::string getPrenom() const;
    std::string getDateNaissance() const;
    std::string getEmail() const;
    std::string getTelephone() const;

    // Setters
    void setNom(const std::string& nom);
    void setPrenom(const std::string& prenom);
    void setDateNaissance(const std::string& dateNaissance);
    void setEmail(const std::string& email);
    void setTelephone(const std::string& telephone);

    // Méthodes virtuelles
    virtual void afficher() const = 0;
    virtual std::string getType() const = 0;
};

#endif // PERSONNE_H