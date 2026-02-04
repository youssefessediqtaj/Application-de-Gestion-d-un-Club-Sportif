import React, { useState, useEffect } from 'react';
import { equipesAPI } from '../services/api';

const TeamsPage = () => {
  const [teams, setTeams] = useState([]);
  const [formData, setFormData] = useState({
    id: '',
    nom: '',
    sport: '',
    coach: ''
  });
  const [editingId, setEditingId] = useState(null);
  const [isSubmitting, setIsSubmitting] = useState(false);

  // Fetch teams from the API
  useEffect(() => {
    const fetchTeams = async () => {
      try {
        const data = await equipesAPI.getAll();
        setTeams(data);
      } catch (error) {
        console.error('Error fetching teams:', error);
      }
    };
    
    fetchTeams();
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
        // Update existing team
        const updatedTeam = await equipesAPI.update(editingId, {
          ...formData,
          coach: parseInt(formData.coach) || 0
        });
        
        setTeams(prev => prev.map(t => 
          t.id === editingId ? updatedTeam : t
        ));
        setEditingId(null);
      } else {
        // Add new team
        const newTeam = await equipesAPI.create({
          ...formData,
          coach: parseInt(formData.coach) || 0
        });
        
        setTeams(prev => [...prev, newTeam]);
      }
      
      setFormData({
        id: '',
        nom: '',
        sport: '',
        coach: ''
      });
    } catch (error) {
      console.error('Error saving team:', error);
    } finally {
      setIsSubmitting(false);
    }
  };

  const handleEdit = (team) => {
    setFormData({
      id: team.id,
      nom: team.nom,
      sport: team.sport,
      coach: team.coach
    });
    setEditingId(team.id);
  };

  const handleDelete = async (id) => {
    if (window.confirm('Êtes-vous sûr de vouloir supprimer cette équipe ?')) {
      try {
        const success = await equipesAPI.delete(id);
        if (success) {
          setTeams(prev => prev.filter(t => t.id !== id));
        }
      } catch (error) {
        console.error('Error deleting team:', error);
      }
    }
  };

  const handleCancel = () => {
    setFormData({
      id: '',
      nom: '',
      sport: '',
      coach: ''
    });
    setEditingId(null);
  };

  return (
    <div className="teams-page">
      <h1>Gestion des Équipes</h1>
      
      <div className="card">
        <h2>{editingId ? 'Modifier une équipe' : 'Ajouter une équipe'}</h2>
        <form onSubmit={handleSubmit}>
          <div style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fit, minmax(300px, 1fr))', gap: '15px' }}>
            <div className="form-group">
              <label htmlFor="nom">Nom de l'équipe</label>
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
              <label htmlFor="sport">Sport</label>
              <input
                type="text"
                id="sport"
                name="sport"
                value={formData.sport}
                onChange={handleInputChange}
                required
              />
            </div>
            
            <div className="form-group">
              <label htmlFor="coach">ID du coach</label>
              <input
                type="number"
                id="coach"
                name="coach"
                value={formData.coach}
                onChange={handleInputChange}
                min="0"
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
        <h2>Liste des équipes</h2>
        
        <div style={{ overflowX: 'auto' }}>
          <table className="table">
            <thead>
              <tr>
                <th>ID</th>
                <th>Nom</th>
                <th>Sport</th>
                <th>Coach</th>
                <th>Actions</th>
              </tr>
            </thead>
            <tbody>
              {teams.map(team => (
                <tr key={team.id}>
                  <td>{team.id}</td>
                  <td>{team.nom}</td>
                  <td>{team.sport}</td>
                  <td>{team.coach}</td>
                  <td>
                    <button 
                      className="btn btn-primary" 
                      onClick={() => handleEdit(team)}
                      style={{ marginRight: '5px' }}
                    >
                      Modifier
                    </button>
                    <button 
                      className="btn btn-danger" 
                      onClick={() => handleDelete(team.id)}
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

export default TeamsPage;