import React, { useState, useEffect } from 'react';
import { seancesAPI } from '../services/api';

const SessionsPage = () => {
  const [sessions, setSessions] = useState([]);
  const [formData, setFormData] = useState({
    id: '',
    equipe: '',
    date: '',
    heure: '',
    duree: '',
    type: 'Entraînement',
    coach: ''
  });
  const [editingId, setEditingId] = useState(null);
  const [isSubmitting, setIsSubmitting] = useState(false);

  // Fetch sessions from the API
  useEffect(() => {
    const fetchSessions = async () => {
      try {
        const data = await seancesAPI.getAll();
        setSessions(data);
      } catch (error) {
        console.error('Error fetching sessions:', error);
      }
    };
    
    fetchSessions();
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
        // Update existing session
        const updatedSession = await seancesAPI.update(editingId, {
          ...formData,
          equipe: parseInt(formData.equipe),
          duree: parseInt(formData.duree)
        });
        
        setSessions(prev => prev.map(s => 
          s.id === editingId ? updatedSession : s
        ));
        setEditingId(null);
      } else {
        // Add new session
        const newSession = await seancesAPI.create({
          ...formData,
          equipe: parseInt(formData.equipe),
          duree: parseInt(formData.duree)
        });
        
        setSessions(prev => [...prev, newSession]);
      }
      
      setFormData({
        id: '',
        equipe: '',
        date: '',
        heure: '',
        duree: '',
        type: 'Entraînement',
        coach: ''
      });
    } catch (error) {
      console.error('Error saving session:', error);
    } finally {
      setIsSubmitting(false);
    }
  };

  const handleEdit = (session) => {
    setFormData({
      id: session.id,
      equipe: session.equipe,
      date: session.date,
      heure: session.heure,
      duree: session.duree,
      type: session.type,
      coach: session.coach
    });
    setEditingId(session.id);
  };

  const handleDelete = async (id) => {
    if (window.confirm('Êtes-vous sûr de vouloir supprimer cette séance ?')) {
      try {
        const success = await seancesAPI.delete(id);
        if (success) {
          setSessions(prev => prev.filter(s => s.id !== id));
        }
      } catch (error) {
        console.error('Error deleting session:', error);
      }
    }
  };

  const handleCancel = () => {
    setFormData({
      id: '',
      equipe: '',
      date: '',
      heure: '',
      duree: '',
      type: 'Entraînement',
      coach: ''
    });
    setEditingId(null);
  };

  return (
    <div className="sessions-page">
      <h1>Gestion des Séances</h1>
      
      <div className="card">
        <h2>{editingId ? 'Modifier une séance' : 'Créer une séance'}</h2>
        <form onSubmit={handleSubmit}>
          <div style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fit, minmax(300px, 1fr))', gap: '15px' }}>
            <div className="form-group">
              <label htmlFor="equipe">ID de l'équipe</label>
              <input
                type="number"
                id="equipe"
                name="equipe"
                value={formData.equipe}
                onChange={handleInputChange}
                min="0"
                required
              />
            </div>
            
            <div className="form-group">
              <label htmlFor="date">Date</label>
              <input
                type="date"
                id="date"
                name="date"
                value={formData.date}
                onChange={handleInputChange}
                required
              />
            </div>
            
            <div className="form-group">
              <label htmlFor="heure">Heure de début</label>
              <input
                type="time"
                id="heure"
                name="heure"
                value={formData.heure}
                onChange={handleInputChange}
                required
              />
            </div>
            
            <div className="form-group">
              <label htmlFor="duree">Durée (minutes)</label>
              <input
                type="number"
                id="duree"
                name="duree"
                value={formData.duree}
                onChange={handleInputChange}
                min="1"
                required
              />
            </div>
            
            <div className="form-group">
              <label htmlFor="type">Type</label>
              <select
                id="type"
                name="type"
                value={formData.type}
                onChange={handleInputChange}
                required
              >
                <option value="Entraînement">Entraînement</option>
                <option value="Match">Match</option>
                <option value="Compétition">Compétition</option>
              </select>
            </div>
            
            <div className="form-group">
              <label htmlFor="coach">Coach</label>
              <input
                type="text"
                id="coach"
                name="coach"
                value={formData.coach}
                onChange={handleInputChange}
              />
            </div>
          </div>
          
          <div style={{ marginTop: '20px' }}>
            <button type="submit" className="btn btn-primary" disabled={isSubmitting}>
              {isSubmitting ? 'En cours...' : (editingId ? 'Modifier' : 'Créer')}
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
        <h2>Liste des séances</h2>
        
        <div style={{ overflowX: 'auto' }}>
          <table className="table">
            <thead>
              <tr>
                <th>ID</th>
                <th>Équipe</th>
                <th>Date</th>
                <th>Heure</th>
                <th>Type</th>
                <th>Actions</th>
              </tr>
            </thead>
            <tbody>
              {sessions.map(session => (
                <tr key={session.id}>
                  <td>{session.id}</td>
                  <td>{session.equipe}</td>
                  <td>{session.date}</td>
                  <td>{session.heure}</td>
                  <td>
                    <span style={{
                      padding: '4px 8px',
                      borderRadius: '12px',
                      backgroundColor: session.type === 'Entraînement' ? '#d4edda' : session.type === 'Match' ? '#fff3cd' : '#cce5ff',
                      color: session.type === 'Entraînement' ? '#155724' : session.type === 'Match' ? '#856404' : '#004085'
                    }}>
                      {session.type}
                    </span>
                  </td>
                  <td>
                    <button 
                      className="btn btn-primary" 
                      onClick={() => handleEdit(session)}
                      style={{ marginRight: '5px' }}
                    >
                      Modifier
                    </button>
                    <button 
                      className="btn btn-danger" 
                      onClick={() => handleDelete(session.id)}
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

export default SessionsPage;