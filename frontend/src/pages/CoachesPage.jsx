import React, { useState, useEffect } from 'react';
import { entraineursAPI } from '../services/api';

const CoachesPage = () => {
  const [coaches, setCoaches] = useState([]);
  const [formData, setFormData] = useState({
    id: '',
    nom: '',
    prenom: '',
    dateNaissance: '',
    email: '',
    telephone: '',
    specialite: '',
    salaire: ''
  });
  const [editingId, setEditingId] = useState(null);
  const [isSubmitting, setIsSubmitting] = useState(false);

  // Fetch coaches from the API
  useEffect(() => {
    const fetchCoaches = async () => {
      try {
        const data = await entraineursAPI.getAll();
        setCoaches(data);
      } catch (error) {
        console.error('Error fetching coaches:', error);
      }
    };
    
    fetchCoaches();
  }, []);

  const handleInputChange = (e) => {
    const { name, value } = e.target;
    setFormData(prev => ({
      ...prev,
      [name]: value
    }));
  };

  const handleSubmit = async (e) => {
    e.preventDefault();
    
    // Prevent duplicate submissions
    if (isSubmitting) return;
    
    setIsSubmitting(true);
    
    try {
      if (editingId) {
        // Update existing coach
        const updatedCoach = await entraineursAPI.update(editingId, {
          ...formData,
          salaire: parseFloat(formData.salaire)
        });
        
        setCoaches(prev => prev.map(c => 
          c.id === editingId ? updatedCoach : c
        ));
        setEditingId(null);
      } else {
        // Add new coach
        const newCoach = await entraineursAPI.create({
          ...formData,
          salaire: parseFloat(formData.salaire)
        });
        
        setCoaches(prev => [...prev, newCoach]);
      }
      
      setFormData({
        id: '',
        nom: '',
        prenom: '',
        dateNaissance: '',
        email: '',
        telephone: '',
        specialite: '',
        salaire: ''
      });
    } catch (error) {
      console.error('Error saving coach:', error);
    } finally {
      setIsSubmitting(false);
    }
  };

  const handleEdit = (coach) => {
    setFormData({
      id: coach.id,
      nom: coach.nom,
      prenom: coach.prenom,
      dateNaissance: coach.dateNaissance,
      email: coach.email,
      telephone: coach.telephone,
      specialite: coach.specialite,
      salaire: coach.salaire
    });
    setEditingId(coach.id);
  };

  const handleDelete = async (id) => {
    if (window.confirm('Êtes-vous sûr de vouloir supprimer cet entraineur ?')) {
      try {
        const success = await entraineursAPI.delete(id);
        if (success) {
          setCoaches(prev => prev.filter(c => c.id !== id));
        }
      } catch (error) {
        console.error('Error deleting coach:', error);
      }
    }
  };

  const handleCancel = () => {
    setFormData({
      id: '',
      nom: '',
      prenom: '',
      dateNaissance: '',
      email: '',
      telephone: '',
      specialite: '',
      salaire: ''
    });
    setEditingId(null);
  };

  return (
    <div className="coaches-page">
      <h1>Gestion des Entraineurs</h1>
      
      <div className="card">
        <h2>{editingId ? 'Modifier un entraineur' : 'Ajouter un entraineur'}</h2>
        <form onSubmit={handleSubmit}>
          <div style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fit, minmax(300px, 1fr))', gap: '15px' }}>
            <div className="form-group">
              <label htmlFor="nom">Nom</label>
              <input
                type="text"
                id="nom"
                name="nom"
                value={formData.nom}
                onChange={handleInputChange}
                required
              />
            </div>
            
            <div className="form-group">
              <label htmlFor="prenom">Prénom</label>
              <input
                type="text"
                id="prenom"
                name="prenom"
                value={formData.prenom}
                onChange={handleInputChange}
                required
              />
            </div>
            
            <div className="form-group">
              <label htmlFor="dateNaissance">Date de naissance</label>
              <input
                type="date"
                id="dateNaissance"
                name="dateNaissance"
                value={formData.dateNaissance}
                onChange={handleInputChange}
              />
            </div>
            
            <div className="form-group">
              <label htmlFor="email">Email</label>
              <input
                type="email"
                id="email"
                name="email"
                value={formData.email}
                onChange={handleInputChange}
              />
            </div>
            
            <div className="form-group">
              <label htmlFor="telephone">Téléphone</label>
              <input
                type="tel"
                id="telephone"
                name="telephone"
                value={formData.telephone}
                onChange={handleInputChange}
              />
            </div>
            
            <div className="form-group">
              <label htmlFor="specialite">Spécialité</label>
              <input
                type="text"
                id="specialite"
                name="specialite"
                value={formData.specialite}
                onChange={handleInputChange}
              />
            </div>
            
            <div className="form-group">
              <label htmlFor="salaire">Salaire (€)</label>
              <input
                type="number"
                id="salaire"
                name="salaire"
                value={formData.salaire}
                onChange={handleInputChange}
                step="0.01"
              />
            </div>
          </div>
          
          <div style={{ marginTop: '20px' }}>
            <button type="submit" className="btn btn-primary" disabled={isSubmitting}>
              {isSubmitting ? 'En cours...' : (editingId ? 'Modifier' : 'Ajouter')}
            </button>
            {editingId && (
              <button type="button" className="btn btn-secondary" onClick={handleCancel} style={{ marginLeft: '10px' }} disabled={isSubmitting}>
                Annuler
              </button>
            )}
          </div>
        </form>
      </div>
      
      <div className="card">
        <h2>Liste des entraineurs</h2>
        
        <div style={{ overflowX: 'auto' }}>
          <table className="table">
            <thead>
              <tr>
                <th>ID</th>
                <th>Nom</th>
                <th>Prénom</th>
                <th>Email</th>
                <th>Spécialité</th>
                <th>Salaire (€)</th>
                <th>Actions</th>
              </tr>
            </thead>
            <tbody>
              {coaches.map(coach => (
                <tr key={coach.id}>
                  <td>{coach.id}</td>
                  <td>{coach.nom}</td>
                  <td>{coach.prenom}</td>
                  <td>{coach.email}</td>
                  <td>{coach.specialite}</td>
                  <td>{coach.salaire}</td>
                  <td>
                    <button 
                      className="btn btn-primary" 
                      onClick={() => handleEdit(coach)}
                      style={{ marginRight: '5px' }}
                    >
                      Modifier
                    </button>
                    <button 
                      className="btn btn-danger" 
                      onClick={() => handleDelete(coach.id)}
                    >
                      Supprimer
                    </button>
                  </td>
                </tr>
              ))}
            </tbody>
          </table>
        </div>
      </div>
    </div>
  );
};

export default CoachesPage;