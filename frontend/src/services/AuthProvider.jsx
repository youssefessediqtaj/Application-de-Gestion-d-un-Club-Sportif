import React, { createContext, useContext, useReducer, useEffect } from 'react';
import { authAPI } from './api';

// Initial state for authentication
const initialState = {
  user: null,
  isAuthenticated: false,
  isLoading: true
};

// Auth reducer
const authReducer = (state, action) => {
  switch (action.type) {
    case 'LOGIN_START':
      return {
        ...state,
        isLoading: true
      };
    case 'LOGIN_SUCCESS':
      return {
        ...state,
        user: action.payload,
        isAuthenticated: true,
        isLoading: false
      };
    case 'LOGIN_FAILURE':
      return {
        ...state,
        user: null,
        isAuthenticated: false,
        isLoading: false
      };
    case 'LOGOUT':
      return {
        ...state,
        user: null,
        isAuthenticated: false,
        isLoading: false
      };
    case 'SET_USER':
      return {
        ...state,
        user: action.payload,
        isAuthenticated: !!action.payload,
        isLoading: false
      };
    default:
      return state;
  }
};

// Auth context
const AuthContext = createContext();

// Auth provider component
export const AuthProvider = ({ children }) => {
  const [state, dispatch] = useReducer(authReducer, initialState);

  // Check if user is already logged in on app start
  useEffect(() => {
    const checkSession = async () => {
      const token = localStorage.getItem('authToken');
      if (!token) {
        dispatch({ type: 'SET_USER', payload: null });
        return;
      }

      try {
        const result = await authAPI.checkAuth();
        if (result.success) {
          // If valid, restore from localStorage (or re-fetch full user data)
          const storedUser = localStorage.getItem('currentUser');
          if (storedUser) {
            dispatch({ type: 'SET_USER', payload: JSON.parse(storedUser) });
          } else {
            // Fallback user object if currentUser was missing
            dispatch({ type: 'SET_USER', payload: { username: result.username, role: 'USER' } });
          }
        } else {
          // Invalid token
          localStorage.removeItem('authToken');
          localStorage.removeItem('currentUser');
          dispatch({ type: 'SET_USER', payload: null });
        }
      } catch (error) {
        console.error('Session check failed:', error);
        // On network error, we might want to keep the local state or clear it. 
        // Clearing it is safer for "No Auth" by default.
        dispatch({ type: 'SET_USER', payload: null });
      }
    };

    checkSession();
  }, []);

  // Login function - calls the API service
  const login = async (username, password) => {
    dispatch({ type: 'LOGIN_START' });

    try {
      const result = await authAPI.login(username, password);

      if (result.success) {
        // Store user without password
        const userToStore = { id: result.user.id, username: result.user.username, role: result.user.role };
        localStorage.setItem('currentUser', JSON.stringify(userToStore));
        // Token is stored inside authAPI.login already
        dispatch({ type: 'LOGIN_SUCCESS', payload: userToStore });
        return { success: true, user: userToStore };
      } else {
        dispatch({ type: 'LOGIN_FAILURE' });
        return { success: false, message: result.message };
      }
    } catch (error) {
      dispatch({ type: 'LOGIN_FAILURE' });
      return { success: false, message: 'Erreur de connexion. Veuillez réessayer.' };
    }
  };

  const logout = async () => {
    try {
      await authAPI.logout();
    } catch (error) {
      console.error('Backend logout failed:', error);
    } finally {
      localStorage.removeItem('authToken');
      localStorage.removeItem('currentUser');
      dispatch({ type: 'LOGOUT' });
    }
  };

  const value = {
    ...state,
    login,
    logout,
    hasRole: (requiredRole) => {
      if (!state.user) return false;
      return state.user.role === requiredRole || state.user.role === 'ADMIN';
    }
  };

  return (
    <AuthContext.Provider value={value}>
      {children}
    </AuthContext.Provider>
  );
};

// Custom hook to use auth context
export const useAuth = () => {
  const context = useContext(AuthContext);
  if (!context) {
    throw new Error('useAuth must be used within an AuthProvider');
  }
  return context;
};