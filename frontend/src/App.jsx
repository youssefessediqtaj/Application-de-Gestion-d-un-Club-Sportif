import React from 'react';
import { Routes, Route, Navigate } from 'react-router-dom';
import { AuthProvider } from './services/AuthProvider';
import LoginPage from './pages/LoginPage';
import Dashboard from './pages/Dashboard';
import MembersPage from './pages/MembersPage';
import TeamsPage from './pages/TeamsPage';
import CoachesPage from './pages/CoachesPage';
import SessionsPage from './pages/SessionsPage';
import PaymentsPage from './pages/PaymentsPage';
import Navbar from './components/Navbar';
import ProtectedRoute from './components/ProtectedRoute';
import './App.css';

function App() {
  return (
    <AuthProvider>
      <div className="App">
        <Routes>
          <Route path="/login" element={<LoginPage />} />
          <Route
            path="/"
            element={
              <ProtectedRoute>
                <div className="app-layout">
                  <Navbar />
                  <div className="main-content">
                    <div className="container">
                      <Dashboard />
                    </div>
                  </div>
                </div>
              </ProtectedRoute>
            }
          />
          <Route
            path="/dashboard"
            element={
              <ProtectedRoute>
                <div className="app-layout">
                  <Navbar />
                  <div className="main-content">
                    <div className="container">
                      <Dashboard />
                    </div>
                  </div>
                </div>
              </ProtectedRoute>
            }
          />
          <Route
            path="/membres"
            element={
              <ProtectedRoute allowedRoles={['ADMIN']}>
                <div className="app-layout">
                  <Navbar />
                  <div className="main-content">
                    <div className="container">
                      <MembersPage />
                    </div>
                  </div>
                </div>
              </ProtectedRoute>
            }
          />
          <Route
            path="/equipes"
            element={
              <ProtectedRoute allowedRoles={['ADMIN']}>
                <div className="app-layout">
                  <Navbar />
                  <div className="main-content">
                    <div className="container">
                      <TeamsPage />
                    </div>
                  </div>
                </div>
              </ProtectedRoute>
            }
          />
          <Route
            path="/entraineurs"
            element={
              <ProtectedRoute allowedRoles={['ADMIN']}>
                <div className="app-layout">
                  <Navbar />
                  <div className="main-content">
                    <div className="container">
                      <CoachesPage />
                    </div>
                  </div>
                </div>
              </ProtectedRoute>
            }
          />
          <Route
            path="/seances"
            element={
              <ProtectedRoute allowedRoles={['ADMIN', 'ENTRAINEUR', 'MEMBRE']}>
                <div className="app-layout">
                  <Navbar />
                  <div className="main-content">
                    <div className="container">
                      <SessionsPage />
                    </div>
                  </div>
                </div>
              </ProtectedRoute>
            }
          />
          <Route
            path="/paiements"
            element={
              <ProtectedRoute allowedRoles={['ADMIN']}>
                <div className="app-layout">
                  <Navbar />
                  <div className="main-content">
                    <div className="container">
                      <PaymentsPage />
                    </div>
                  </div>
                </div>
              </ProtectedRoute>
            }
          />
          <Route path="*" element={<Navigate to="/dashboard" replace />} />
        </Routes>
      </div>
    </AuthProvider>
  );
}

export default App;