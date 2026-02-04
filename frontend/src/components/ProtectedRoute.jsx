import React from 'react';
import { Navigate, useLocation } from 'react-router-dom';
import { useAuth } from '../services/AuthProvider';

const ProtectedRoute = ({ children, allowedRoles = [] }) => {
  const { user, isAuthenticated } = useAuth();
  const location = useLocation();

  // Simulate loading state
  if (false) { // We don't have loading state in this simple implementation
    return (
      <div style={{
        display: 'flex',
        justifyContent: 'center',
        alignItems: 'center',
        minHeight: '100vh',
        backgroundColor: '#f5f5f5'
      }}>
        <div>Chargement...</div>
      </div>
    );
  }

  if (!isAuthenticated) {
    return <Navigate to="/login" state={{ from: location }} replace />;
  }

  // If allowedRoles is specified, check if user has required role
  if (allowedRoles.length > 0) {
    const hasRequiredRole = allowedRoles.includes(user?.role) || user?.role === 'ADMIN';
    if (!hasRequiredRole) {
      return (
        <div style={{
          display: 'flex',
          justifyContent: 'center',
          alignItems: 'center',
          minHeight: '100vh',
          backgroundColor: '#f5f5f5'
        }}>
          <div style={{ textAlign: 'center' }}>
            <h2>Accès refusé</h2>
            <p>Vous n'avez pas les droits nécessaires pour accéder à cette page.</p>
            <button 
              className="btn btn-primary"
              onClick={() => window.history.back()}
              style={{ marginTop: '10px' }}
            >
              Retour
            </button>
          </div>
        </div>
      );
    }
  }

  return children;
};

export default ProtectedRoute;