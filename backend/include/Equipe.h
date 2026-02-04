#ifndef EQUIPE_H
#define EQUIPE_H

#include <string>
#include <vector>

class Equipe {
private:
    int id;
    std::string nom;
    std::string sport;
    int coach; // ID de l'entraineur
    std::vector<int> membres; // IDs des membres

public:
    // Constructeurs
    Equipe();
    Equipe(int id, const std::string& nom, const std::string& sport, int coach);

    // Getters
    int getId() const;
    std::string getNom() const;
    std::string getSport() const;
    int getCoach() const;
    std::vector<int> getMembres() const;

    // Setters
    void setNom(const std::string& nom);
    void setSport(const std::string& sport);
    void setCoach(int coach);

    // Méthodes spécifiques
    void addMembre(int idMembre);
    void removeMembre(int idMembre);
    int getNombreMembres() const;
    void afficher() const;
};

#endif // EQUIPE_H