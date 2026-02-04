import React, { useState } from 'react';
import { useNavigate, useLocation } from 'react-router-dom';
import { useAuth } from '../services/AuthProvider';

const LoginPage = () => {
  const [username, setUsername] = useState('');
  const [password, setPassword] = useState('');
  const [error, setError] = useState('');
  const [success, setSuccess] = useState('');
  const [isSubmitting, setIsSubmitting] = useState(false);

  const { login } = useAuth();
  const navigate = useNavigate();
  const location = useLocation();

  const from = location.state?.from?.pathname || '/dashboard';

  const handleSubmit = async (e) => {
    e.preventDefault();
    setError('');
    setSuccess('');
    setIsSubmitting(true);

    try {
      const result = await login(username, password);
      if (result.success) {
        setSuccess('Connexion réussie ! Redirection...');
        // Small delay to show success message
        setTimeout(() => {
          navigate(from, { replace: true });
        }, 1500);
      } else {
        setError(result.message);
        setIsSubmitting(false);
      }
    } catch (err) {
      setError('Erreur de connexion. Veuillez réessayer.');
      setIsSubmitting(false);
    }
  };

  return (
    <div className="login-container" style={{
      display: 'flex',
      justifyContent: 'center',
      alignItems: 'center',
      minHeight: '100vh',
      backgroundColor: '#f8f9fa',
      padding: '20px'
    }}>
      <div className="login-form" style={{
        backgroundColor: 'white',
        padding: '32px',
        borderRadius: '12px',
        boxShadow: '0 10px 30px rgba(0, 0, 0, 0.1)',
        width: '100%',
        maxWidth: '420px'
      }}>
        <div style={{ textAlign: 'center', marginBottom: '24px' }}>
          <div style={{
            width: '60px',
            height: '60px',
            backgroundColor: '#0d6efd',
            borderRadius: '50%',
            display: 'flex',
            alignItems: 'center',
            justifyContent: 'center',
            margin: '0 auto 16px'
          }}>
            <span style={{ color: 'white', fontSize: '24px', fontWeight: 'bold' }}>S</span>
          </div>
          <h2 style={{ margin: 0, fontSize: '24px', fontWeight: '600', color: '#212529' }}>Connexion</h2>
          <p style={{ margin: '8px 0 0', color: '#6c757d', fontSize: '14px' }}>Connectez-vous à votre compte</p>
        </div>

        {error && (
          <div className="error-message notification error" style={{
            padding: '12px',
            marginBottom: '16px',
            borderRadius: '6px',
            fontSize: '14px',
            backgroundColor: '#fee2e2',
            color: '#dc2626',
            border: '1px solid #fecaca'
          }}>
            {error}
          </div>
        )}

        {success && (
          <div className="success-message notification success" style={{
            padding: '12px',
            marginBottom: '16px',
            borderRadius: '6px',
            fontSize: '14px',
            backgroundColor: '#dcfce7',
            color: '#16a34a',
            border: '1px solid #bbf7d0'
          }}>
            {success}
          </div>
        )}

        <form onSubmit={handleSubmit}>
          <div className="form-group" style={{ marginBottom: '16px' }}>
            <label htmlFor="username" style={{ display: 'block', marginBottom: '8px', fontSize: '14px', fontWeight: '500' }}>Nom d'utilisateur</label>
            <input
              type="text"
              id="username"
              value={username}
              onChange={(e) => setUsername(e.target.value)}
              required
              disabled={isSubmitting}
              placeholder="Entrez votre nom d'utilisateur"
              style={{
                width: '100%',
                padding: '10px 12px',
                borderRadius: '6px',
                border: '1px solid #dee2e6',
                boxSizing: 'border-box'
              }}
            />
          </div>

          <div className="form-group" style={{ marginBottom: '24px' }}>
            <label htmlFor="password" style={{ display: 'block', marginBottom: '8px', fontSize: '14px', fontWeight: '500' }}>Mot de passe</label>
            <input
              type="password"
              id="password"
              value={password}
              onChange={(e) => setPassword(e.target.value)}
              required
              disabled={isSubmitting}
              placeholder="Entrez votre mot de passe"
              style={{
                width: '100%',
                padding: '10px 12px',
                borderRadius: '6px',
                border: '1px solid #dee2e6',
                boxSizing: 'border-box'
              }}
            />
          </div>

          <button
            type="submit"
            className="btn btn-primary"
            disabled={isSubmitting}
            style={{
              width: '100%',
              padding: '12px',
              fontSize: '16px',
              marginTop: '8px',
              fontWeight: '500',
              backgroundColor: '#0d6efd',
              color: 'white',
              border: 'none',
              borderRadius: '6px',
              cursor: isSubmitting ? 'not-allowed' : 'pointer',
              opacity: isSubmitting ? 0.7 : 1,
              transition: 'all 0.2s'
            }}
          >
            {isSubmitting ? 'Connexion...' : 'Se connecter'}
          </button>
        </form>

        <div style={{
          marginTop: '24px',
          padding: '16px',
          backgroundColor: '#f8f9fa',
          borderRadius: '8px',
          textAlign: 'center',
          fontSize: '13px',
          color: '#6c757d',
          border: '1px solid #e9ecef'
        }}>
          <p style={{ margin: '0 0 8px 0', fontWeight: '500' }}>Comptes de test:</p>
          <div style={{ display: 'flex', flexDirection: 'column', gap: '4px', fontSize: '12px' }}>
            <span>Admin: admin / admin123</span>
            <span>Coach: coach1 / coach123</span>
            <span>Membre: membre1 / membre123</span>
          </div>
        </div>
      </div>
    </div>
  );
};

export default LoginPage;