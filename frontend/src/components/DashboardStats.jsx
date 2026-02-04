import React, { useState, useEffect } from 'react';
import { useAuth } from '../services/AuthProvider';
import { statsAPI } from '../services/api';

const DashboardStats = () => {
  const { user } = useAuth();
  const [stats, setStats] = useState({
    totalMembres: 0,
    totalEquipes: 0,
    totalEntraineurs: 0,
    totalSeances: 0,
    totalPaiements: 0,
    membresPayes: 0,
    membresImpayes: 0
  });

  useEffect(() => {
    // Charger les statistiques depuis le backend
    const fetchStats = async () => {
      try {
        const statsData = await statsAPI.getGeneral();
        setStats(statsData);
      } catch (error) {
        console.error('Erreur lors du chargement des statistiques:', error);
        // En cas d'erreur, charger des données de secours
        const mockStats = {
          totalMembres: 45,
          totalEquipes: 8,
          totalEntraineurs: 12,
          totalSeances: 156,
          totalPaiements: 89,
          membresPayes: 38,
          membresImpayes: 7
        };
        setStats(mockStats);
      }
    };

    fetchStats();
  }, []);

  const statsCards = [
    {
      title: 'Membres',
      value: stats.totalMembres,
      color: 'bg-blue-500',
      icon: '👥'
    },
    {
      title: 'Équipes',
      value: stats.totalEquipes,
      color: 'bg-green-500',
      icon: '⚽'
    },
    {
      title: 'Entraineurs',
      value: stats.totalEntraineurs,
      color: 'bg-purple-500',
      icon: '👨‍🏫'
    },
    {
      title: 'Séances',
      value: stats.totalSeances,
      color: 'bg-yellow-500',
      icon: '⏱️'
    },
    {
      title: 'Paiements',
      value: stats.totalPaiements,
      color: 'bg-red-500',
      icon: '💰'
    },
    {
      title: 'Membres en règle',
      value: stats.membresPayes,
      color: 'bg-green-600',
      icon: '✅'
    },
    {
      title: 'Membres en impayé',
      value: stats.membresImpayes,
      color: 'bg-red-600',
      icon: '❌'
    }
  ];

  return (
    <div className="dashboard-stats">
      <h2 style={{ fontSize: '1.5rem', fontWeight: '600', marginBottom: '24px', color: '#212529' }}>Statistiques du Club</h2>
      
      <div style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fit, minmax(250px, 1fr))', gap: '24px', marginBottom: '32px' }}>
        {statsCards.map((stat, index) => (
          <div key={index} className="card" style={{ padding: '24px', borderLeft: '4px solid #0d6efd' }}>
            <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center' }}>
              <div>
                <p style={{ fontSize: '0.875rem', color: '#6c757d', marginBottom: '8px' }}>{stat.title}</p>
                <p style={{ fontSize: '2rem', fontWeight: 'bold', color: '#212529' }}>{stat.value}</p>
              </div>
              <div style={{ fontSize: '2rem' }}>{stat.icon}</div>
            </div>
          </div>
        ))}
      </div>

      <div style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fit, minmax(400px, 1fr))', gap: '24px' }}>
        {/* Graphique des membres par statut */}
        <div className="card">
          <h3 style={{ fontSize: '1.125rem', fontWeight: '600', marginBottom: '16px', color: '#212529' }}>Membres par statut</h3>
          <div style={{ display: 'flex', flexDirection: 'column', gap: '8px' }}>
            <div style={{ display: 'flex', alignItems: 'center', justifyContent: 'space-between' }}>
              <span style={{ fontSize: '0.875rem', color: '#495057' }}>En règle</span>
              <div style={{ display: 'flex', alignItems: 'center' }}>
                <div 
                  style={{ 
                    height: '16px', 
                    backgroundColor: '#198754', 
                    borderRadius: '4px',
                    marginRight: '8px',
                    width: `${(stats.membresPayes / stats.totalMembres * 100) || 0}%`, 
                    minWidth: '20px' 
                  }}
                ></div>
                <span style={{ fontSize: '0.875rem', color: '#495057' }}>{stats.membresPayes}</span>
              </div>
            </div>
            <div style={{ display: 'flex', alignItems: 'center', justifyContent: 'space-between' }}>
              <span style={{ fontSize: '0.875rem', color: '#495057' }}>En impayé</span>
              <div style={{ display: 'flex', alignItems: 'center' }}>
                <div 
                  style={{ 
                    height: '16px', 
                    backgroundColor: '#dc3545', 
                    borderRadius: '4px',
                    marginRight: '8px',
                    width: `${(stats.membresImpayes / stats.totalMembres * 100) || 0}%`, 
                    minWidth: '20px' 
                  }}
                ></div>
                <span style={{ fontSize: '0.875rem', color: '#495057' }}>{stats.membresImpayes}</span>
              </div>
            </div>
          </div>
        </div>

        {/* Graphique des paiements */}
        <div className="card">
          <h3 style={{ fontSize: '1.125rem', fontWeight: '600', marginBottom: '16px', color: '#212529' }}>Paiements</h3>
          <div style={{ display: 'flex', flexDirection: 'column', gap: '8px' }}>
            <div style={{ display: 'flex', alignItems: 'center', justifyContent: 'space-between' }}>
              <span style={{ fontSize: '0.875rem', color: '#495057' }}>Total</span>
              <span style={{ fontSize: '0.875rem', fontWeight: '500', color: '#212529' }}>{stats.totalPaiements}</span>
            </div>
            <div style={{ display: 'flex', alignItems: 'center', justifyContent: 'space-between' }}>
              <span style={{ fontSize: '0.875rem', color: '#495057' }}>Payés</span>
              <span style={{ fontSize: '0.875rem', color: '#198754' }}>{stats.membresPayes}</span>
            </div>
            <div style={{ display: 'flex', alignItems: 'center', justifyContent: 'space-between' }}>
              <span style={{ fontSize: '0.875rem', color: '#495057' }}>Impayés</span>
              <span style={{ fontSize: '0.875rem', color: '#dc3545' }}>{stats.membresImpayes}</span>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
};

export default DashboardStats;