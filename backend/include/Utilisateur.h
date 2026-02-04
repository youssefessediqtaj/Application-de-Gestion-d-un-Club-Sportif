#ifndef UTILISATEUR_H
#define UTILISATEUR_H

#include <string>

class Utilisateur {
private:
    int id;
    std::string username;
    std::string password;
    std::string role; // ADMIN, ENTRAINEUR, MEMBRE

public:
    // Constructeurs
    Utilisateur();
    Utilisateur(int id, const std::string& username, const std::string& password, const std::string& role);

    // Getters
    int getId() const;
    std::string getUsername() const;
    std::string getPassword() const;
    std::string getRole() const;

    // Setters
    void setId(int id);
    void setUsername(const std::string& username);
    void setPassword(const std::string& password);
    void setRole(const std::string& role);

    // Méthodes d'authentification
    bool verifyPassword(const std::string& inputPassword) const;
    bool login(const std::string& inputUsername, const std::string& inputPassword);
    void logout();

    // Méthodes utilitaires
    bool isAdmin() const;
    bool isCoach() const;
    bool isMember() const;
};

#endif // UTILISATEUR_H