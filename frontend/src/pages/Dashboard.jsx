import React, { useState, useEffect } from 'react';
import { useAuth } from '../services/AuthProvider';
import DashboardStats from '../components/DashboardStats';

const Dashboard = () => {
  const { user } = useAuth();
  
  return (
    <div className="dashboard">
      <div className="card">
        <div className="card-header">
          <h1 className="card-title">Tableau de bord</h1>
        </div>
        <div className="card-body">
          <p>Bienvenue <strong>{user?.username}</strong> ({user?.role === 'ADMIN' ? 'Administrateur' : user?.role === 'ENTRAINEUR' ? 'Entraîneur' : 'Membre'})!</p>
          <p>Sélectionnez une option dans le menu pour gérer le club sportif.</p>
        </div>
      </div>
      
      <DashboardStats />
    </div>
  );
};

export default Dashboard;