// API service to communicate with C++ backend

const API_BASE_URL = 'http://localhost:8080'; // Default URL for C++ backend

// Helper to get token
const getToken = () => localStorage.getItem('authToken');

// Function to handle API responses
const handleResponse = async (response) => {
  if (response.status === 401) {
    // Session expired or unauthorized
    localStorage.removeItem('authToken');
    localStorage.removeItem('currentUser');
    window.location.href = '/login';
    throw new Error('Session expirée. Veuillez vous reconnecter.');
  }

  if (!response.ok) {
    try {
      const errorData = await response.json();
      throw new Error(errorData.error || errorData.message || `HTTP error! status: ${response.status}`);
    } catch (e) {
      if (e instanceof SyntaxError) {
        const errorText = await response.text();
        throw new Error(errorText || `HTTP error! status: ${response.status}`);
      }
      throw e;
    }
  }
  // Some endpoints might not return JSON, so handle accordingly
  const contentType = response.headers.get('content-type');
  if (contentType && contentType.includes('application/json')) {
    return response.json();
  } else {
    return response.text();
  }
};

// Generic fetch wrapper with auth header
const authFetch = async (url, options = {}) => {
  const token = getToken();
  const headers = {
    ...options.headers,
    'Content-Type': 'application/json'
  };

  if (token) {
    headers['Authorization'] = `Bearer ${token}`;
  }

  const response = await fetch(url, {
    ...options,
    headers
  });

  return handleResponse(response);
};

// Authentication API
export const authAPI = {
  login: async (username, password) => {
    try {
      const response = await fetch(`${API_BASE_URL}/login`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json'
        },
        body: JSON.stringify({ username, password })
      });

      const result = await handleResponse(response);
      if (result.success && result.token) {
        localStorage.setItem('authToken', result.token);
      }
      return result;
    } catch (error) {
      console.error('Login error:', error);
      throw error;
    }
  },

  logout: async () => {
    try {
      const result = await authFetch(`${API_BASE_URL}/logout`, {
        method: 'POST'
      });
      localStorage.removeItem('authToken');
      localStorage.removeItem('currentUser');
      return result;
    } catch (error) {
      console.error('Logout error:', error);
      localStorage.removeItem('authToken');
      localStorage.removeItem('currentUser');
      throw error;
    }
  },

  checkAuth: async () => {
    try {
      return await authFetch(`${API_BASE_URL}/check-auth`);
    } catch (error) {
      console.error('Check auth error:', error);
      throw error;
    }
  }
};

// Members API
export const membresAPI = {
  getAll: async () => {
    return authFetch(`${API_BASE_URL}/membres`);
  },

  getById: async (id) => {
    return authFetch(`${API_BASE_URL}/membres/${id}`);
  },

  create: async (membre) => {
    return authFetch(`${API_BASE_URL}/membres`, {
      method: 'POST',
      body: JSON.stringify(membre)
    });
  },

  update: async (id, membre) => {
    return authFetch(`${API_BASE_URL}/membres/${id}`, {
      method: 'PUT',
      body: JSON.stringify(membre)
    });
  },

  delete: async (id) => {
    return authFetch(`${API_BASE_URL}/membres/${id}`, {
      method: 'DELETE'
    });
  }
};

// Teams API
export const equipesAPI = {
  getAll: async () => {
    return authFetch(`${API_BASE_URL}/equipes`);
  },

  getById: async (id) => {
    return authFetch(`${API_BASE_URL}/equipes/${id}`);
  },

  create: async (equipe) => {
    return authFetch(`${API_BASE_URL}/equipes`, {
      method: 'POST',
      body: JSON.stringify(equipe)
    });
  },

  update: async (id, equipe) => {
    return authFetch(`${API_BASE_URL}/equipes/${id}`, {
      method: 'PUT',
      body: JSON.stringify(equipe)
    });
  },

  delete: async (id) => {
    return authFetch(`${API_BASE_URL}/equipes/${id}`, {
      method: 'DELETE'
    });
  }
};

// Coaches API
export const entraineursAPI = {
  getAll: async () => {
    return authFetch(`${API_BASE_URL}/entraineurs`);
  },

  getById: async (id) => {
    return authFetch(`${API_BASE_URL}/entraineurs/${id}`);
  },

  create: async (entraineur) => {
    return authFetch(`${API_BASE_URL}/entraineurs`, {
      method: 'POST',
      body: JSON.stringify(entraineur)
    });
  },

  update: async (id, entraineur) => {
    return authFetch(`${API_BASE_URL}/entraineurs/${id}`, {
      method: 'PUT',
      body: JSON.stringify(entraineur)
    });
  },

  delete: async (id) => {
    return authFetch(`${API_BASE_URL}/entraineurs/${id}`, {
      method: 'DELETE'
    });
  }
};

// Sessions API
export const seancesAPI = {
  getAll: async () => {
    return authFetch(`${API_BASE_URL}/seances`);
  },

  getById: async (id) => {
    return authFetch(`${API_BASE_URL}/seances/${id}`);
  },

  create: async (seance) => {
    return authFetch(`${API_BASE_URL}/seances`, {
      method: 'POST',
      body: JSON.stringify(seance)
    });
  },

  update: async (id, seance) => {
    return authFetch(`${API_BASE_URL}/seances/${id}`, {
      method: 'PUT',
      body: JSON.stringify(seance)
    });
  },

  delete: async (id) => {
    return authFetch(`${API_BASE_URL}/seances/${id}`, {
      method: 'DELETE'
    });
  }
};

// Payments API
export const paiementsAPI = {
  getAll: async () => {
    return authFetch(`${API_BASE_URL}/paiements`);
  },

  getById: async (id) => {
    return authFetch(`${API_BASE_URL}/paiements/${id}`);
  },

  create: async (paiement) => {
    return authFetch(`${API_BASE_URL}/paiements`, {
      method: 'POST',
      body: JSON.stringify(paiement)
    });
  },

  update: async (id, paiement) => {
    return authFetch(`${API_BASE_URL}/paiements/${id}`, {
      method: 'PUT',
      body: JSON.stringify(paiement)
    });
  },

  delete: async (id) => {
    return authFetch(`${API_BASE_URL}/paiements/${id}`, {
      method: 'DELETE'
    });
  }
};

// Statistics API
export const statsAPI = {
  getGeneral: async () => {
    // General stats can be public for the landing page
    try {
      const response = await fetch(`${API_BASE_URL}/stats/general`);
      return await handleResponse(response);
    } catch (error) {
      console.error('Get stats error:', error);
      throw error;
    }
  },

  getMembres: async () => {
    return authFetch(`${API_BASE_URL}/stats/membres`);
  },

  getPaiements: async () => {
    return authFetch(`${API_BASE_URL}/stats/paiements`);
  },

  getEquipes: async () => {
    return authFetch(`${API_BASE_URL}/stats/equipes`);
  },

  getEntraineurs: async () => {
    return authFetch(`${API_BASE_URL}/stats/entraineurs`);
  },

  getSeances: async () => {
    return authFetch(`${API_BASE_URL}/stats/seances`);
  }
};