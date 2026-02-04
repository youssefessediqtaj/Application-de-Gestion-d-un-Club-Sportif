-- Schema SQL pour l'application de gestion d'un club sportif
-- Base de données SQLite

-- Table des utilisateurs (authentification)
CREATE TABLE utilisateurs (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT NOT NULL UNIQUE,
    password TEXT NOT NULL,
    role TEXT NOT NULL CHECK (role IN ('ADMIN', 'ENTRAINEUR', 'MEMBRE'))
);

-- Table des membres
CREATE TABLE membres (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    nom TEXT NOT NULL,
    prenom TEXT NOT NULL,
    date_naissance TEXT,
    email TEXT,
    telephone TEXT,
    numero_membre TEXT UNIQUE,
    date_inscription TEXT NOT NULL,
    statut TEXT NOT NULL DEFAULT 'Actif' CHECK (statut IN ('Actif', 'Inactif', 'Suspendu')),
    cotisation REAL DEFAULT 0.0
);

-- Table des entraineurs
CREATE TABLE entraineurs (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    nom TEXT NOT NULL,
    prenom TEXT NOT NULL,
    date_naissance TEXT,
    email TEXT,
    telephone TEXT,
    specialite TEXT,
    salaire REAL DEFAULT 0.0
);

-- Table des équipes
CREATE TABLE equipes (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    nom TEXT NOT NULL,
    sport TEXT NOT NULL,
    coach_id INTEGER,
    FOREIGN KEY (coach_id) REFERENCES entraineurs(id)
);

-- Table des séances
CREATE TABLE seances (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    equipe_id INTEGER NOT NULL,
    date TEXT NOT NULL,
    heure_debut TEXT NOT NULL,
    duree INTEGER NOT NULL, -- durée en minutes
    type TEXT NOT NULL DEFAULT 'Entraînement' CHECK (type IN ('Entraînement', 'Match', 'Compétition')),
    coach TEXT,
    FOREIGN KEY (equipe_id) REFERENCES equipes(id)
);

-- Table des paiements
CREATE TABLE paiements (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    membre_id INTEGER NOT NULL,
    montant REAL NOT NULL,
    date TEXT NOT NULL,
    statut TEXT NOT NULL DEFAULT 'Non payé' CHECK (statut IN ('Payé', 'Non payé', 'En attente')),
    type TEXT NOT NULL DEFAULT 'Cotisation',
    FOREIGN KEY (membre_id) REFERENCES membres(id)
);

-- Table d'association pour lier les membres aux équipes
CREATE TABLE membres_equipes (
    membre_id INTEGER,
    equipe_id INTEGER,
    FOREIGN KEY (membre_id) REFERENCES membres(id),
    FOREIGN KEY (equipe_id) REFERENCES equipes(id),
    PRIMARY KEY (membre_id, equipe_id)
);

-- Insertion des utilisateurs par défaut
INSERT INTO utilisateurs (username, password, role) VALUES 
('admin', 'admin123', 'ADMIN'),
('coach1', 'coach123', 'ENTRAINEUR'),
('membre1', 'membre123', 'MEMBRE');

-- Insertion de quelques données de test
INSERT INTO membres (nom, prenom, date_naissance, email, telephone, numero_membre, date_inscription, statut, cotisation) VALUES
('Dupont', 'Jean', '1990-05-15', 'jean.dupont@email.com', '0123456789', 'M001', '2023-01-15', 'Actif', 150.00),
('Martin', 'Sophie', '1985-08-22', 'sophie.martin@email.com', '0987654321', 'M002', '2023-02-10', 'Actif', 150.00);

INSERT INTO entraineurs (nom, prenom, date_naissance, email, telephone, specialite, salaire) VALUES
('Leroy', 'Pierre', '1975-03-10', 'pierre.leroy@email.com', '0111111111', 'Football', 2500.00),
('Moreau', 'Claire', '1980-12-05', 'claire.moreau@email.com', '0222222222', 'Basketball', 2300.00);

INSERT INTO equipes (nom, sport, coach_id) VALUES
('Équipe A', 'Football', 1),
('Équipe B', 'Basketball', 2);

INSERT INTO seances (equipe_id, date, heure_debut, duree, type, coach) VALUES
(1, '2025-01-15', '18:00', 90, 'Entraînement', 'Pierre Leroy'),
(2, '2025-01-16', '19:00', 60, 'Match', 'Claire Moreau');

INSERT INTO paiements (membre_id, montant, date, statut, type) VALUES
(1, 150.00, '2025-01-01', 'Payé', 'Cotisation'),
(2, 150.00, '2025-01-05', 'Payé', 'Cotisation');