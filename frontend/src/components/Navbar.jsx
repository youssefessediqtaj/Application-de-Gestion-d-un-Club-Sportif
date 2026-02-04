import React from 'react';
import { Link, useLocation, useNavigate } from 'react-router-dom';
import { getCurrentUser } from '../services/auth';
import { useAuth } from '../services/AuthProvider';

const Navbar = () => {
  const user = getCurrentUser();
  const { logout } = useAuth();
  const navigate = useNavigate();
  const location = useLocation();

  const handleLogout = () => {
    logout();
    navigate('/login', { replace: true });
  };

  // Navigation items based on user role
  const getNavItems = () => {
    if (!user) return [];

    const commonItems = [
      { path: '/dashboard', label: 'Tableau de bord' }
    ];

    const adminItems = [
      { path: '/membres', label: 'Membres' },
      { path: '/equipes', label: 'Équipes' },
      { path: '/entraineurs', label: 'Entraineurs' },
      { path: '/paiements', label: 'Paiements' }
    ];

    const coachItems = [
      { path: '/seances', label: 'Séances' }
    ];

    let items = [...commonItems];

    if (user.role === 'ADMIN') {
      items = [...items, ...adminItems, ...coachItems];
    } else if (user.role === 'ENTRAINEUR' || user.role === 'MEMBRE') {
      items = [...items, ...coachItems];
    }
    // MEMBRE role has only dashboard access

    return items;
  };

  const navItems = getNavItems();

  return (
    <nav style={{
      position: 'fixed',
      top: 0,
      left: 0,
      bottom: 0,
      width: '260px',
      backgroundColor: '#ffffff',
      color: '#212529',
      zIndex: 1000,
      boxShadow: '2px 0 10px rgba(0,0,0,0.08)',
      borderRight: '1px solid #e9ecef',
      display: 'flex',
      flexDirection: 'column'
    }}>
      <div style={{
        padding: '24px',
        borderBottom: '1px solid #e9ecef',
        display: 'flex',
        flexDirection: 'column',
        alignItems: 'center'
      }}>
        <h2 style={{
          margin: 0,
          fontSize: '20px',
          textAlign: 'center',
          fontWeight: '600',
          color: '#212529'
        }}>Club Sportif</h2>
        <div style={{
          marginTop: '16px',
          textAlign: 'center',
          padding: '12px',
          backgroundColor: '#f8f9fa',
          borderRadius: '8px',
          width: '100%'
        }}>
          <p style={{
            margin: '0 0 8px 0',
            fontSize: '14px',
            fontWeight: '500',
            color: '#495057'
          }}>
            <strong>{user?.username}</strong>
          </p>
          <span style={{
            fontSize: '12px',
            padding: '4px 10px',
            backgroundColor: user?.role === 'ADMIN' ? '#0d6efd' : user?.role === 'ENTRAINEUR' ? '#fd7e14' : '#198754',
            borderRadius: '20px',
            color: 'white',
            fontWeight: '500'
          }}>
            {user?.role === 'ADMIN' ? 'Administrateur' : user?.role === 'ENTRAINEUR' ? 'Entraîneur' : 'Membre'}
          </span>
        </div>
      </div>

      <ul style={{
        listStyle: 'none',
        padding: '16px',
        margin: 0,
        flex: 1,
        overflowY: 'auto'
      }}>
        {navItems.map((item, index) => (
          <li key={index} style={{ marginBottom: '4px' }}>
            <Link
              to={item.path}
              style={{
                display: 'block',
                padding: '12px 16px',
                color: location.pathname === item.path ? '#0d6efd' : '#495057',
                textDecoration: 'none',
                backgroundColor: location.pathname === item.path ? '#e7f1ff' : 'transparent',
                borderRadius: '6px',
                transition: 'all 0.2s ease',
                fontWeight: location.pathname === item.path ? '500' : '400',
                borderLeft: location.pathname === item.path ? '3px solid #0d6efd' : '3px solid transparent'
              }}
              onMouseEnter={(e) => {
                if (location.pathname !== item.path) {
                  e.target.style.backgroundColor = '#f8f9fa';
                }
              }}
              onMouseLeave={(e) => {
                if (location.pathname !== item.path) {
                  e.target.style.backgroundColor = 'transparent';
                }
              }}
            >
              {item.label}
            </Link>
          </li>
        ))}
      </ul>

      <div style={{
        padding: '20px',
        borderTop: '1px solid #e9ecef',
        marginTop: 'auto',
        backgroundColor: '#f8f9fa'
      }}>
        <button
          onClick={handleLogout}
          className="btn btn-danger"
          style={{
            width: '100%',
            padding: '12px',
            backgroundColor: '#dc3545',
            border: 'none',
            borderRadius: '6px',
            color: 'white',
            cursor: 'pointer',
            fontSize: '14px',
            fontWeight: '500',
            transition: 'all 0.2s ease'
          }}
          onMouseEnter={(e) => {
            e.target.style.backgroundColor = '#c82333';
            e.target.style.transform = 'translateY(-1px)';
          }}
          onMouseLeave={(e) => {
            e.target.style.backgroundColor = '#dc3545';
            e.target.style.transform = 'translateY(0)';
          }}
        >
          <i className="fas fa-sign-out-alt" style={{ marginRight: '8px' }}></i>
          Déconnexion
        </button>
      </div>
    </nav>
  );
};

export default Navbar;