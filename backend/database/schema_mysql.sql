-- Schema MySQL pour Application de Gestion d'un Club Sportif
-- Migration depuis SQLite vers MySQL 8.x
-- Date: 2025-12-27

-- Créer la base de données si elle n'existe pas
CREATE DATABASE IF NOT EXISTS club_sportif CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

USE club_sportif;

-- Table des utilisateurs (authentification)
CREATE TABLE IF NOT EXISTS utilisateurs (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL,
    role ENUM('ADMIN', 'ENTRAINEUR', 'MEMBRE') NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    INDEX idx_username (username)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- Table des membres
CREATE TABLE IF NOT EXISTS membres (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nom VARCHAR(100) NOT NULL,
    prenom VARCHAR(100) NOT NULL,
    date_naissance VARCHAR(20),
    email VARCHAR(100),
    telephone VARCHAR(20),
    numero_membre VARCHAR(50) UNIQUE,
    date_inscription VARCHAR(20) NOT NULL,
    statut ENUM('Actif', 'Inactif', 'Suspendu') NOT NULL DEFAULT 'Actif',
    cotisation DECIMAL(10,2) DEFAULT 0.00,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    INDEX idx_nom (nom),
    INDEX idx_numero_membre (numero_membre),
    INDEX idx_statut (statut)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- Table des entraîneurs
CREATE TABLE IF NOT EXISTS entraineurs (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nom VARCHAR(100) NOT NULL,
    prenom VARCHAR(100) NOT NULL,
    date_naissance VARCHAR(20),
    email VARCHAR(100),
    telephone VARCHAR(20),
    specialite VARCHAR(100),
    salaire DECIMAL(10,2) DEFAULT 0.00,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    INDEX idx_nom (nom),
    INDEX idx_specialite (specialite)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- Table des équipes
CREATE TABLE IF NOT EXISTS equipes (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nom VARCHAR(100) NOT NULL,
    sport VARCHAR(50) NOT NULL,
    coach_id INT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    FOREIGN KEY (coach_id) REFERENCES entraineurs(id) ON DELETE SET NULL,
    INDEX idx_nom (nom),
    INDEX idx_sport (sport),
    INDEX idx_coach (coach_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- Table des séances
CREATE TABLE IF NOT EXISTS seances (
    id INT AUTO_INCREMENT PRIMARY KEY,
    equipe_id INT NOT NULL,
    date VARCHAR(20) NOT NULL,
    heure_debut VARCHAR(10) NOT NULL,
    duree INT NOT NULL,
    type ENUM('Entraînement', 'Match', 'Compétition') NOT NULL DEFAULT 'Entraînement',
    coach VARCHAR(100),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    FOREIGN KEY (equipe_id) REFERENCES equipes(id) ON DELETE CASCADE,
    INDEX idx_equipe (equipe_id),
    INDEX idx_date (date),
    INDEX idx_type (type)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- Table des paiements
CREATE TABLE IF NOT EXISTS paiements (
    id INT AUTO_INCREMENT PRIMARY KEY,
    membre_id INT NOT NULL,
    montant DECIMAL(10,2) NOT NULL,
    date VARCHAR(20) NOT NULL,
    statut ENUM('Payé', 'Non payé', 'En attente') NOT NULL DEFAULT 'Non payé',
    type VARCHAR(50) NOT NULL DEFAULT 'Cotisation',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    FOREIGN KEY (membre_id) REFERENCES membres(id) ON DELETE CASCADE,
    INDEX idx_membre (membre_id),
    INDEX idx_statut (statut),
    INDEX idx_date (date)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- Table de liaison membres-équipes (relation many-to-many)
CREATE TABLE IF NOT EXISTS membres_equipes (
    membre_id INT NOT NULL,
    equipe_id INT NOT NULL,
    date_assignation TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (membre_id, equipe_id),
    FOREIGN KEY (membre_id) REFERENCES membres(id) ON DELETE CASCADE,
    FOREIGN KEY (equipe_id) REFERENCES equipes(id) ON DELETE CASCADE,
    INDEX idx_membre (membre_id),
    INDEX idx_equipe (equipe_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- Insertion des utilisateurs par défaut
INSERT INTO utilisateurs (username, password, role) VALUES
    ('admin', 'admin123', 'ADMIN'),
    ('coach1', 'coach123', 'ENTRAINEUR'),
    ('membre1', 'membre123', 'MEMBRE')
ON DUPLICATE KEY UPDATE username=username;

-- Insertion de données de test (optionnel pour la démonstration)
INSERT INTO membres (nom, prenom, numero_membre, date_inscription, statut, cotisation) VALUES
    ('El Amrani', 'Amine', 'M001', '2025-01-10', 'Actif', 500.00),
    ('Bennani', 'Sara', 'M002', '2025-02-15', 'Actif', 500.00),
    ('Mansouri', 'Yassine', 'M003', '2025-03-01', 'Inactif', 500.00);

INSERT INTO entraineurs (nom, prenom, specialite, salaire) VALUES
    ('Zaki', 'Badou', 'Football', 15000.00),
    ('Naybet', 'Noureddine', 'Football', 12000.00);

INSERT INTO equipes (nom, sport, coach_id) VALUES
    ('Aigles de l''Atlas', 'Football', 1),
    ('Lions de la RF', 'Football', 2);

INSERT INTO seances (equipe_id, date, heure_debut, duree, type, coach) VALUES
    (1, '2025-12-28', '10:00', 90, 'Entraînement', 'Zaki'),
    (2, '2025-12-29', '15:00', 60, 'Match', 'Naybet');

INSERT INTO paiements (membre_id, montant, date, statut, type) VALUES
    (1, 500.00, '2025-12-20', 'Payé', 'Cotisation'),
    (2, 500.00, '2025-12-22', 'Non payé', 'Cotisation');

-- Afficher les tables créées
SHOW TABLES;
SELECT 'Utilisateurs' as TableName, COUNT(*) FROM utilisateurs
UNION ALL SELECT 'Membres', COUNT(*) FROM membres
UNION ALL SELECT 'Entraineurs', COUNT(*) FROM entraineurs
UNION ALL SELECT 'Equipes', COUNT(*) FROM equipes;

