#ifndef SEANCE_H
#define SEANCE_H

#include <string>

class Seance {
private:
    int id;
    int idEquipe; // ID de l'équipe concernée
    std::string date; // Format: JJ/MM/AAAA
    std::string heureDebut; // Format: HH:MM
    int duree; // Durée en minutes
    std::string type; // Entraînement, Match, etc.
    std::string coach; // Nom ou ID du coach responsable

public:
    // Constructeurs
    Seance();
    Seance(int id, int idEquipe, const std::string& date, const std::string& heureDebut, 
           int duree, const std::string& type, const std::string& coach);

    // Getters
    int getId() const;
    int getIdEquipe() const;
    std::string getDate() const;
    std::string getHeureDebut() const;
    int getDuree() const;
    std::string getType() const;
    std::string getCoach() const;

    // Setters
    void setIdEquipe(int idEquipe);
    void setDate(const std::string& date);
    void setHeureDebut(const std::string& heureDebut);
    void setDuree(int duree);
    void setType(const std::string& type);
    void setCoach(const std::string& coach);

    // Méthodes
    void afficher() const;
    std::string getHoraire() const; // Renvoie l'horaire complet (date + heure)
};

#endif // SEANCE_H