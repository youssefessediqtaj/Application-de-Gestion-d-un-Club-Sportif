import React, { useState, useEffect } from 'react';
import { paiementsAPI } from '../services/api';

const PaymentsPage = () => {
  const [payments, setPayments] = useState([]);
  const [formData, setFormData] = useState({
    id: '',
    membre: '',
    montant: '',
    date: '',
    statut: 'Payé',
    type: 'Cotisation'
  });
  const [editingId, setEditingId] = useState(null);
  const [searchTerm, setSearchTerm] = useState('');
  const [isSubmitting, setIsSubmitting] = useState(false);

  // Fetch payments from the API
  useEffect(() => {
    const fetchPayments = async () => {
      try {
        const data = await paiementsAPI.getAll();
        setPayments(data);
      } catch (error) {
        console.error('Error fetching payments:', error);
      }
    };
    
    fetchPayments();
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
        // Update existing payment
        const updatedPayment = await paiementsAPI.update(editingId, {
          ...formData,
          membre: parseInt(formData.membre),
          montant: parseFloat(formData.montant)
        });
        
        setPayments(prev => prev.map(p => 
          p.id === editingId ? updatedPayment : p
        ));
        setEditingId(null);
      } else {
        // Add new payment
        const newPayment = await paiementsAPI.create({
          ...formData,
          membre: parseInt(formData.membre),
          montant: parseFloat(formData.montant)
        });
        
        setPayments(prev => [...prev, newPayment]);
      }
      
      setFormData({
        id: '',
        membre: '',
        montant: '',
        date: '',
        statut: 'Payé',
        type: 'Cotisation'
      });
    } catch (error) {
      console.error('Error saving payment:', error);
    } finally {
      setIsSubmitting(false);
    }
  };

  const handleEdit = (payment) => {
    setFormData({
      id: payment.id,
      membre: payment.membre,
      montant: payment.montant,
      date: payment.date,
      statut: payment.statut,
      type: payment.type
    });
    setEditingId(payment.id);
  };

  const handleDelete = async (id) => {
    if (window.confirm('Êtes-vous sûr de vouloir supprimer ce paiement ?')) {
      try {
        const success = await paiementsAPI.delete(id);
        if (success) {
          setPayments(prev => prev.filter(p => p.id !== id));
        }
      } catch (error) {
        console.error('Error deleting payment:', error);
      }
    }
  };

  const handleCancel = () => {
    setFormData({
      id: '',
      membre: '',
      montant: '',
      date: '',
      statut: 'Payé',
      type: 'Cotisation'
    });
    setEditingId(null);
  };

  // Filter payments based on search term (membre ID)
  const filteredPayments = payments.filter(payment =>
    payment.membre.toString().includes(searchTerm)
  );

  return (
    <div className="payments-page">
      <h1>Gestion des Paiements</h1>
      
      <div className="card">
        <h2>{editingId ? 'Modifier un paiement' : 'Enregistrer un paiement'}</h2>
        <form onSubmit={handleSubmit}>
          <div style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fit, minmax(300px, 1fr))', gap: '15px' }}>
            <div className="form-group">
              <label htmlFor="membre">ID du membre</label>
              <input
                type="number"
                id="membre"
                name="membre"
                value={formData.membre}
                onChange={handleInputChange}
                min="0"
                required
              />
            </div>
            
            <div className="form-group">
              <label htmlFor="montant">Montant (€)</label>
              <input
                type="number"
                id="montant"
                name="montant"
                value={formData.montant}
                onChange={handleInputChange}
                step="0.01"
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
              <label htmlFor="statut">Statut</label>
              <select
                id="statut"
                name="statut"
                value={formData.statut}
                onChange={handleInputChange}
                required
              >
                <option value="Payé">Payé</option>
                <option value="Non payé">Non payé</option>
                <option value="En attente">En attente</option>
              </select>
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
                <option value="Cotisation">Cotisation</option>
                <option value="Frais d'inscription">Frais d'inscription</option>
                <option value="Autre">Autre</option>
              </select>
            </div>
          </div>
          
          <div style={{ marginTop: '20px' }}>
            <button type="submit" className="btn btn-primary" disabled={isSubmitting}>
              {isSubmitting ? 'En cours...' : (editingId ? 'Modifier' : 'Enregistrer')}
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
          <h2>Liste des paiements</h2>
          <div className="form-group" style={{ marginBottom: 0, width: '300px' }}>
            <input
              type="number"
              placeholder="Rechercher par ID membre..."
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
                <th>Membre</th>
                <th>Montant (€)</th>
                <th>Date</th>
                <th>Statut</th>
                <th>Type</th>
                <th>Actions</th>
              </tr>
            </thead>
            <tbody>
              {filteredPayments.map(payment => (
                <tr key={payment.id}>
                  <td>{payment.id}</td>
                  <td>{payment.membre}</td>
                  <td>{payment.montant}</td>
                  <td>{payment.date}</td>
                  <td>
                    <span style={{
                      padding: '4px 8px',
                      borderRadius: '12px',
                      backgroundColor: payment.statut === 'Payé' ? '#d4edda' : payment.statut === 'En attente' ? '#fff3cd' : '#f8d7da',
                      color: payment.statut === 'Payé' ? '#155724' : payment.statut === 'En attente' ? '#856404' : '#721c24'
                    }}>
                      {payment.statut}
                    </span>
                  </td>
                  <td>{payment.type}</td>
                  <td>
                    <button 
                      className="btn btn-primary" 
                      onClick={() => handleEdit(payment)}
                      style={{ marginRight: '5px' }}
                    >
                      Modifier
                    </button>
                    <button 
                      className="btn btn-danger" 
                      onClick={() => handleDelete(payment.id)}
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

export default PaymentsPage;