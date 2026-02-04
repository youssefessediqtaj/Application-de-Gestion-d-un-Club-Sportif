// API service functions for authentication
import { authAPI } from './api';

// Function to handle login
export const handleLogin = async (username, password) => {
  try {
    const result = await authAPI.login(username, password);
    if (result.success) {
      // Store user without password
      const userToStore = { id: result.user.id, username: result.user.username, role: result.user.role };
      localStorage.setItem('currentUser', JSON.stringify(userToStore));
      return { success: true, user: userToStore };
    } else {
      return { success: false, message: result.message };
    }
  } catch (error) {
    return { success: false, message: 'Erreur de connexion. Veuillez réessayer.' };
  }
};

// Function to handle logout
export const handleLogout = () => {
  localStorage.removeItem('currentUser');
};

// Enhanced logout function that also updates auth state
export const performLogout = (logoutFn) => {
  localStorage.removeItem('currentUser');
  if (logoutFn) {
    logoutFn();
  }
};

// Function to get current user
export const getCurrentUser = () => {
  const storedUser = localStorage.getItem('currentUser');
  if (storedUser) {
    try {
      return JSON.parse(storedUser);
    } catch (error) {
      console.error('Error parsing stored user:', error);
      localStorage.removeItem('currentUser');
      return null;
    }
  }
  return null;
};

// Function to check if user is authenticated
export const isAuthenticated = () => {
  return !!getCurrentUser();
};

// Function to check user role
export const hasRole = (requiredRole) => {
  const user = getCurrentUser();
  if (!user) return false;
  return user.role === requiredRole || user.role === 'ADMIN';
};