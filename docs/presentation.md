# Présentation - Application de Gestion d'un Club Sportif

## 1. Introduction

### Objectif du Projet
Développer une application complète de gestion d'un club sportif utilisant:
- **Backend**: C++ orienté objet
- **Frontend**: HTML/CSS/JavaScript
- **Stockage**: Fichiers textes pour la persistance

### Contexte
- Projet académique pour démontrer la POO en C++
- Application destinée à automatiser la gestion d'un club sportif
- Solution complète avec interface utilisateur

## 2. Architecture du Système

### Structure des Dossiers
```
Application_Club_Sportif/
├── backend/
│   ├── include/          # Fichiers d'en-tête
│   ├── src/              # Fichiers sources
│   └── main.cpp          # Point d'entrée
├── frontend/
│   ├── index.html        # Interface utilisateur
│   ├── style.css         # Styles
│   └── app.js            # Logique frontend
└── docs/
    ├── cahier_des_charges.md
    ├── rapport_projet.md
    └── presentation.md
```

### Technologies Utilisées
- **Langages**: C++ (backend), HTML/CSS/JavaScript (frontend)
- **Paradigme**: Programmation Orientée Objet
- **Stockage**: Fichiers textes (CSV)
- **Plateforme**: Multiplateforme

## 3. Structure des Classes

### Hiérarchie des Classes
```
Personne (Classe de Base Abstraite)
├── Membre (Hérite de Personne)
└── Entraineur (Hérite de Personne)

Autres Classes:
├── Equipe
├── Seance
├── Paiement
└── ClubSportif (Classe Contrôleur)
```

### Détail des Classes

#### Personne (Classe de Base)
```cpp
class Personne {
protected:
    int id;
    string nom, prenom, dateNaissance, email, telephone;
public:
    // Getters, setters, méthodes virtuelles pures
};
```

#### Membre (Hérite de Personne)
```cpp
class Membre : public Personne {
private:
    string numeroMembre, dateInscription, statut;
    double cotisation;
public:
    // Méthodes spécifiques aux membres
};
```

#### ClubSportif (Classe Contrôleur)
- Gère toutes les collections d'entités
- Méthodes CRUD pour chaque type
- Gestion de la persistance

## 4. Fonctionnalités Principales

### Gestion des Membres
- ✅ Ajouter/modifier/supprimer des membres
- ✅ Lister et rechercher des membres
- ✅ Gérer les statuts (actif/inactif/suspendu)

### Gestion des Équipes
- ✅ Créer et gérer des équipes
- ✅ Assigner membres et entraineurs aux équipes
- ✅ Lister les équipes

### Gestion des Entraineurs
- ✅ Ajouter/modifier/supprimer des entraineurs
- ✅ Gérer les spécialités et salaires

### Gestion des Séances
- ✅ Planifier des séances d'entraînement
- ✅ Gérer les horaires et types de séances

### Gestion des Paiements
- ✅ Enregistrer les paiements
- ✅ Suivre les statuts de paiement
- ✅ Gestion des cotisations

## 5. Interface Utilisateur

### Design
- Interface web responsive
- Navigation intuitive
- Design moderne et épuré
- Compatible mobile et desktop

### Modules Disponibles
1. **Tableau de bord** - Vue d'ensemble
2. **Membres** - Gestion des membres
3. **Équipes** - Gestion des équipes
4. **Entraineurs** - Gestion du personnel
5. **Séances** - Planning des activités
6. **Paiements** - Suivi financier

## 6. Persistance des Données

### Système de Stockage
- Base de données SQLite
- Persistance des données dans un fichier unique club_sportif.db
- Chargement et sauvegarde automatiques via API HTTP

## 7. Avantages de la Solution

### Techniques
- Architecture modulaire et extensible
- Respect des principes de POO
- Code bien structuré et commenté
- Facilité de maintenance

### Fonctionnels
- Interface utilisateur intuitive
- Gestion complète des aspects du club
- Recherche et filtrage des données
- Statistiques et tableaux de bord

## 8. Conclusion

### Résultats Atteints
- Application complète répondant aux besoins spécifiés
- Démonstration pratique des concepts de POO
- Intégration réussie frontend/backend
- Solution prête pour une utilisation académique

### Perspectives
- Intégration d'une base de données
- Ajout d'un système d'authentification
- Développement d'une version mobile
- Amélioration de l'interface utilisateur

### Compétences Développées
- Programmation orientée objet en C++
- Développement d'interfaces web
- Gestion de projet logiciel
- Documentation technique
- Tests et validation