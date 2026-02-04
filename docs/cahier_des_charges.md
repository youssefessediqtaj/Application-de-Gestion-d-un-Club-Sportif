# Cahier des Charges - Application de Gestion d'un Club Sportif

## 1. Présentation du Projet

### 1.1 Contexte
Dans le cadre de la gestion efficace d'un club sportif, il est nécessaire de disposer d'une application informatisée permettant de gérer les différentes composantes du club : membres, équipes, entraineurs, séances d'entraînement et paiements.

### 1.2 Objectif
Développer une application complète de gestion d'un club sportif permettant d'automatiser et d'optimiser la gestion administrative et organisationnelle du club.

### 1.3 Cible
Cette application s'adresse principalement aux gestionnaires de clubs sportifs, aux administrateurs, aux entraineurs et aux membres du club.

## 2. Analyse des Besoins

### 2.1 Système d'authentification
- Connexion utilisateur avec nom d'utilisateur et mot de passe
- Gestion des rôles (Administrateur, Entraineur, Membre)
- Contrôle d'accès basé sur les rôles

### 2.2 Acteurs du Système
- **Administrateur** : Accès complet au système, gestion de toutes les fonctionnalités
- **Entraineur** : Accès en lecture aux informations de ses équipes et séances, gestion des séances
- **Membre** : Accès en lecture à ses informations personnelles

### 2.2 Fonctionnalités Principales

#### 2.2.1 Gestion des Membres
- Ajouter un nouveau membre (Administrateurs uniquement)
- Modifier les informations d'un membre (Administrateurs uniquement)
- Supprimer un membre (Administrateurs uniquement)
- Lister tous les membres
- Rechercher un membre par nom ou numéro
- Gérer le statut des membres (actif, inactif, suspendu)

#### 2.2.2 Gestion des Équipes
- Créer une nouvelle équipe (Administrateurs uniquement)
- Modifier les informations d'une équipe (Administrateurs uniquement)
- Supprimer une équipe (Administrateurs uniquement)
- Lister toutes les équipes
- Assigner des membres à une équipe (Administrateurs uniquement)
- Assigner un entraineur à une équipe (Administrateurs uniquement)

#### 2.2.3 Gestion des Entraineurs
- Ajouter un nouvel entraineur (Administrateurs uniquement)
- Modifier les informations d'un entraineur (Administrateurs uniquement)
- Supprimer un entraineur (Administrateurs uniquement)
- Lister tous les entraineurs
- Gérer les spécialités et salaires

#### 2.2.4 Gestion des Séances
- Créer une nouvelle séance (entraînement, match) (Administrateurs et Entraineurs)
- Modifier les informations d'une séance (Administrateurs et Entraineurs)
- Supprimer une séance (Administrateurs et Entraineurs)
- Lister toutes les séances
- Affecter une séance à une équipe
- Gérer les horaires et types de séances

#### 2.2.5 Gestion des Paiements
- Enregistrer un nouveau paiement (Administrateurs uniquement)
- Modifier les informations d'un paiement (Administrateurs uniquement)
- Lister les paiements d'un membre
- Gérer le statut des paiements (payé, non payé, en attente)
- Suivi des cotisations

## 3. Spécifications Techniques

### 3.1 Architecture du Système
- **Backend** : C++ orienté objet avec classes et héritage
- **Serveur HTTP** : Implémentation d'un serveur HTTP en C++ pour l'API
- **Frontend** : React avec composants fonctionnels et hooks
- **Stockage** : Base de données MySQL / MariaDB pour la persistance des données
- **Communication** : Communication via API REST entre le frontend React et le backend C++

### 3.2 Structure des Classes

#### 3.2.1 Classes de Base
- **Personne** : Classe abstraite de base
  - Attributs : id, nom, prénom, date de naissance, email, téléphone
  - Méthodes : getters, setters, afficher()

- **Membre** : Hérite de Personne
  - Attributs supplémentaires : numéro de membre, date d'inscription, statut, cotisation
  - Méthodes : estEnRegle(), modifierStatut()

- **Entraineur** : Hérite de Personne
  - Attributs supplémentaires : spécialité, salaire
  - Méthodes : augmenterSalaire(), changerSpecialite()

#### 3.2.2 Classes de Gestion
- **Equipe** : Gestion des équipes
  - Attributs : id, nom, sport, coach, membres
  - Méthodes : addMembre(), removeMembre(), getNombreMembres()

- **Seance** : Gestion des séances
  - Attributs : id, idEquipe, date, heureDebut, duree, type, coach
  - Méthodes : getHoraire(), afficher()

- **Paiement** : Gestion des paiements
  - Attributs : id, idMembre, montant, date, statut, type
  - Méthodes : estPaye(), marquerCommePaye()

- **Utilisateur** : Gestion des utilisateurs et authentification
  - Attributs : id, username, password, role
  - Méthodes : login(), logout(), verifyPassword(), isAdmin(), isCoach(), isMember()

- **ClubSportif** : Contrôleur principal
  - Attributs : collections de toutes les entités
  - Méthodes : toutes les opérations CRUD pour chaque entité

### 3.3 Interface Utilisateur
- Interface web responsive développée avec React
- Navigation via React Router avec barre latérale
- Composants réutilisables pour la saisie et l'affichage des données
- Tableaux dynamiques avec recherche et filtres
- Design moderne avec une expérience utilisateur améliorée

## 4. Contraintes

### 4.1 Techniques
- Utilisation exclusive de C++ orienté objet pour le backend
- Implémentation d'un serveur HTTP en C++ pour l'API
- Utilisation de React avec Vite pour le frontend
- Communication API REST entre le frontend et le backend
- Stockage des données dans une base de données MySQL
- Application multiplateforme (macOS, Linux, Windows)

### 4.2 Fonctionnelles
- Respect des principes de programmation orientée objet
- Interface utilisateur conviviale
- Gestion des erreurs et validations
- Persistance des données

### 4.3 Non-Fonctionnelles
- Performance acceptable pour un petit à moyen club
- Facilité de maintenance
- Documentation complète
- Code lisible et commenté

### 4.4 Considérations de sécurité
- Stockage des mots de passe en texte clair (limitation acceptée pour un projet académique)
- Contrôle d'accès basé sur les rôles
- Validation des entrées utilisateur

## 5. Cas d'Utilisation

### 5.1 Cas d'Utilisation Principaux
1. **Inscription d'un nouveau membre**
   - L'administrateur saisit les informations du membre
   - Le système enregistre le membre dans la base de données

2. **Création d'une séance d'entraînement**
   - L'administrateur sélectionne l'équipe
   - Le système crée la séance avec les détails fournis

3. **Enregistrement d'un paiement**
   - L'administrateur sélectionne le membre et le type de paiement
   - Le système enregistre le paiement et met à jour le statut

### 5.2 Scénarios d'Utilisation
- Gestion quotidienne des membres
- Planification des séances d'entraînement
- Suivi des paiements et cotisations
- Génération de rapports simples

## 6. Tests et Validation

### 6.1 Tests Unitaires
- Validation des méthodes de chaque classe
- Tests de persistance des données
- Tests des validations d'entrée

### 6.2 Tests d'Intégration
- Test de la communication entre les classes
- Test de l'interface utilisateur
- Test de la cohérence des données

## 7. Maintenance et Évolution

### 7.1 Maintenance Prévue
- Correction de bugs
- Amélioration de l'interface utilisateur
- Ajout de fonctionnalités mineures

### 7.2 Évolutions Possibles
- Application mobile
- Fonctionnalités de reporting avancées
- Amélioration de la sécurité (hachage des mots de passe)
- Intégration de graphiques statistiques avancés

## 8. Conclusion

Ce cahier des charges définit les besoins et spécifications pour le développement d'une application de gestion d'un club sportif. L'application devra être robuste, conviviale et répondre aux besoins réels de gestion d'un club sportif.