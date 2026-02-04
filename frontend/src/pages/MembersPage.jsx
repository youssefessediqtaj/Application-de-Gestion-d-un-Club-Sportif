import React, { useState, useEffect } from 'react';
import { membresAPI } from '../services/api';

const MembersPage = () => {
  const [members, setMembers] = useState([]);
  const [formData, setFormData] = useState({
    id: '',
    nom: '',
    prenom: '',
    dateNaissance: '',
    email: '',
    telephone: '',
    numeroMembre: '',
    dateInscription: '',
    statut: 'Actif',
    cotisation: ''
  });
  const [editingId, setEditingId] = useState(null);
  const [searchTerm, setSearchTerm] = useState('');
  const [isSubmitting, setIsSubmitting] = useState(false);
  const [message, setMessage] = useState({ type: '', text: '' });

  // Fetch members from the API
  useEffect(() => {
    const fetchMembers = async () => {
      try {
        const data = await membresAPI.getAll();
        setMembers(data);
      } catch (error) {
        console.error('Error fetching members:', error);
      }
    };

    fetchMembers();
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
        // Update existing member
        const updatedMember = await membresAPI.update(editingId, {
          ...formData,
          cotisation: parseFloat(formData.cotisation)
        });

        setMembers(prev => prev.map(m =>
          m.id === editingId ? updatedMember : m
        ));
        setEditingId(null);
      } else {
        // Add new member
        const newMember = await membresAPI.create({
          ...formData,
          cotisation: parseFloat(formData.cotisation)
        });

        setMembers(prev => [...prev, newMember]);
      }

      setFormData({
        id: '',
        nom: '',
        prenom: '',
        dateNaissance: '',
        email: '',
        telephone: '',
        numeroMembre: '',
        dateInscription: '',
        statut: 'Actif',
        cotisation: ''
      });
    } catch (error) {
      console.error('Error saving member:', error);
    } finally {
      setIsSubmitting(false);
    }
  };

  const handleEdit = (member) => {
    setFormData({
      id: member.id,
      nom: member.nom,
      prenom: member.prenom,
      dateNaissance: member.dateNaissance,
      email: member.email,
      telephone: member.telephone,
      numeroMembre: member.numeroMembre,
      dateInscription: member.dateInscription,
      statut: member.statut,
      cotisation: member.cotisation
    });
    setEditingId(member.id);
  };

  const handleDelete = async (id) => {
    if (window.confirm('Êtes-vous sûr de vouloir supprimer ce membre ?')) {
      try {
        console.log('Attempting to delete member with ID:', id);
        const success = await membresAPI.delete(id);
        console.log('Delete status from API:', success);
        if (success) {
          console.log('Deletion confirmed, updating state...');
          setMembers(prev => prev.filter(m => m.id !== id));
          setMessage({ type: 'success', text: 'Membre supprimé avec succès !' });
          setTimeout(() => setMessage({ type: '', text: '' }), 3000);
        }
      } catch (error) {
        console.error('Error deleting member:', error);
        setMessage({ type: 'error', text: 'Erreur lors de la suppression du membre.' });
        setTimeout(() => setMessage({ type: '', text: '' }), 3000);
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
      numeroMembre: '',
      dateInscription: '',
      statut: 'Actif',
      cotisation: ''
    });
    setEditingId(null);
  };

  // Filter members based on search term
  const filteredMembers = members.filter(member =>
    (member.nom || '').toLowerCase().includes(searchTerm.toLowerCase()) ||
    (member.prenom || '').toLowerCase().includes(searchTerm.toLowerCase()) ||
    (member.numeroMembre || '').toLowerCase().includes(searchTerm.toLowerCase())
  );

  return (
    <div className="members-page">
      <h1>Gestion des Membres</h1>

      {message.text && (
        <div style={{
          padding: '10px',
          marginBottom: '20px',
          borderRadius: '4px',
          backgroundColor: message.type === 'success' ? '#d4edda' : '#f8d7da',
          color: message.type === 'success' ? '#155724' : '#721c24',
          border: `1px solid ${message.type === 'success' ? '#c3e6cb' : '#f5c6cb'}`
        }}>
          {message.text}
        </div>
      )}

      <div className="card">
        <h2>{editingId ? 'Modifier un membre' : 'Ajouter un membre'}</h2>
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
              <label htmlFor="numeroMembre">Numéro de membre</label>
              <input
                type="text"
                id="numeroMembre"
                name="numeroMembre"
                value={formData.numeroMembre}
                onChange={handleInputChange}
              />
            </div>

            <div className="form-group">
              <label htmlFor="dateInscription">Date d'inscription</label>
              <input
                type="date"
                id="dateInscription"
                name="dateInscription"
                value={formData.dateInscription}
                onChange={handleInputChange}
              />
            </div>

            <div className="form-group">
              <label htmlFor="statut">Statut</label>
              <select
                id="statut"
                name="statut"
                value={formData.statut}
                onChange={handleInputChange}
              >
                <option value="Actif">Actif</option>
                <option value="Inactif">Inactif</option>
                <option value="Suspendu">Suspendu</option>
              </select>
            </div>

            <div className="form-group">
              <label htmlFor="cotisation">Cotisation (€)</label>
              <input
                type="number"
                id="cotisation"
                name="cotisation"
                value={formData.cotisation}
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
        <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: '20px' }}>
          <h2>Liste des membres</h2>
          <div className="form-group" style={{ marginBottom: 0, width: '300px' }}>
            <input
              type="text"
              placeholder="Rechercher un membre..."
              value={searchTerm}
              onChange={(e) => setSearchTerm(e.target.value)}
              style={{ width: '100%' }}
            />
          </div>
        </div>

        <div style={{ overflowX: 'auto' }}>
          <table className="table">
            <thead>
              <tr>
                <th>ID</th>
                <th>Nom</th>
                <th>Prénom</th>
                <th>Email</th>
                <th>Téléphone</th>
                <th>Statut</th>
                <th>Actions</th>
              </tr>
            </thead>
            <tbody>
              {filteredMembers.map(member => (
                <tr key={member.id}>
                  <td>{member.id}</td>
                  <td>{member.nom}</td>
                  <td>{member.prenom}</td>
                  <td>{member.email}</td>
                  <td>{member.telephone}</td>
                  <td>
                    <span style={{
                      padding: '4px 8px',
                      borderRadius: '12px',
                      backgroundColor: member.statut === 'Actif' ? '#d4edda' : member.statut === 'Inactif' ? '#fff3cd' : '#f8d7da',
                      color: member.statut === 'Actif' ? '#155724' : member.statut === 'Inactif' ? '#856404' : '#721c24'
                    }}>
                      {member.statut}
                    </span>
                  </td>
                  <td>
                    <button
                      className="btn btn-primary"
                      onClick={() => handleEdit(member)}
                      style={{ marginRight: '5px' }}
                    >
                      Modifier
                    </button>
                    <button
                      className="btn btn-danger"
                      onClick={() => handleDelete(member.id)}
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

export default MembersPage;