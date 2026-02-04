#include "../../include/server/HttpServer.h"
#include "../../include/ClubSportif.h"
#include <cstring>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <regex>
#include <sstream>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <vector>

HttpServer::HttpServer(int port, ClubSportif *club)
    : port(port), club(club), running(false), server_fd(0) {}

HttpServer::~HttpServer() { stop(); }

void HttpServer::start() {
  if (running)
    return;

  // Create socket
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == 0) {
    std::cerr << "Erreur de création du socket" << std::endl;
    return;
  }

  // Set socket options
  int opt = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    std::cerr << "Erreur de configuration du socket" << std::endl;
    close(server_fd);
    return;
  }

  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    std::cerr << "Erreur de liaison du socket" << std::endl;
    close(server_fd);
    return;
  }

  if (listen(server_fd, 10) < 0) {
    std::cerr << "Erreur d'écoute du socket" << std::endl;
    close(server_fd);
    return;
  }

  running = true;
  setupRoutes();

  std::cout << "Serveur HTTP démarré sur le port " << port << std::endl;

  // Start server thread
  server_thread = std::thread([this]() {
    while (running) {
      struct sockaddr_in client_addr;
      socklen_t client_len = sizeof(client_addr);

      int client_fd =
          accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
      if (client_fd < 0)
        continue;

      // Handle client request in a separate thread
      std::thread([this, client_fd]() {
        char buffer[4096] = {0};
        int bytes_read =
            read(client_fd, buffer, 4095); // Leave space for null terminator

        if (bytes_read > 0) {
          std::string request(
              buffer, bytes_read); // Use actual bytes read to avoid garbage
          std::istringstream iss(request);
          std::string method, path, version;

          iss >> method >> path >> version;

          // Validate that we have a proper HTTP request
          if (method.empty() || path.empty()) {
            std::string error_response =
                "HTTP/1.1 400 Bad Request\r\nContent-Type: "
                "text/plain\r\n\r\nBad Request";
            send(client_fd, error_response.c_str(), error_response.length(), 0);
          } else {
            // Extract body if present
            std::string body;
            size_t body_pos = request.find("\r\n\r\n");
            if (body_pos != std::string::npos) {
              body = request.substr(body_pos + 4);
            }

            std::string response = handleRequest(method, path, body, request);

            send(client_fd, response.c_str(), response.length(), 0);
          }
        } else {
          // Send error response if read failed
          std::string error_response =
              "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\n\r\nBad "
              "Request";
          send(client_fd, error_response.c_str(), error_response.length(), 0);
        }

        close(client_fd);
      }).detach();
    }
  });
}

void HttpServer::stop() {
  if (!running)
    return;

  running = false;

  if (server_fd > 0) {
    close(server_fd);
  }

  if (server_thread.joinable()) {
    server_thread.join();
  }
}

void HttpServer::setupRoutes() {
  routes["GET /stats/general"] = [this](const std::string &body,
                                        const std::string &req) {
    return this->getStatsGeneral(body);
  };
  routes["GET /stats/membres"] = [this](const std::string &body,
                                        const std::string &req) {
    return this->getStatsMembres(body);
  };
  routes["GET /stats/paiements"] = [this](const std::string &body,
                                          const std::string &req) {
    return this->getStatsPaiements(body);
  };
  routes["GET /stats/equipes"] = [this](const std::string &body,
                                        const std::string &req) {
    return this->getStatsEquipes(body);
  };
  routes["GET /stats/entraineurs"] = [this](const std::string &body,
                                            const std::string &req) {
    return this->getStatsEntraineurs(body);
  };
  routes["GET /stats/seances"] = [this](const std::string &body,
                                        const std::string &req) {
    return this->getStatsSeances(body);
  };

  routes["GET /membres"] = [this](const std::string &body,
                                  const std::string &req) {
    return this->getMembres(body);
  };
  routes["POST /membres"] = [this](const std::string &body,
                                   const std::string &req) {
    return this->postMembres(body);
  };

  routes["GET /equipes"] = [this](const std::string &body,
                                  const std::string &req) {
    return this->getEquipes(body);
  };
  routes["POST /equipes"] = [this](const std::string &body,
                                   const std::string &req) {
    return this->postEquipes(body);
  };

  routes["GET /entraineurs"] = [this](const std::string &body,
                                      const std::string &req) {
    return this->getEntraineurs(body);
  };
  routes["POST /entraineurs"] = [this](const std::string &body,
                                       const std::string &req) {
    return this->postEntraineurs(body);
  };

  routes["GET /seances"] = [this](const std::string &body,
                                  const std::string &req) {
    return this->getSeances(body);
  };
  routes["POST /seances"] = [this](const std::string &body,
                                   const std::string &req) {
    return this->postSeances(body);
  };

  routes["GET /paiements"] = [this](const std::string &body,
                                    const std::string &req) {
    return this->getPaiements(body);
  };
  routes["POST /paiements"] = [this](const std::string &body,
                                     const std::string &req) {
    return this->postPaiements(body);
  };

  routes["POST /login"] = [this](const std::string &body,
                                 const std::string &req) {
    return this->handleLogin(body);
  };
  routes["POST /logout"] = [this](const std::string &body,
                                  const std::string &req) {
    return this->handleLogout(req);
  };
  routes["GET /check-auth"] = [this](const std::string &body,
                                     const std::string &req) {
    return this->handleCheckAuth(req);
  };
}

// Helper function to add CORS headers
std::string addCorsHeaders(const std::string &response) {
  // Find the end of the status line
  size_t headerStart = response.find("\r\n");
  if (headerStart == std::string::npos) {
    return response;
  }

  // Insert CORS headers after the status line
  std::string corsHeaders =
      "\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Methods: "
      "GET, POST, PUT, DELETE, OPTIONS\r\nAccess-Control-Allow-Headers: "
      "Content-Type, Authorization, authorization";

  return response.substr(0, headerStart) + corsHeaders +
         response.substr(headerStart);
}

std::string HttpServer::handleRequest(const std::string &method,
                                      const std::string &path,
                                      const std::string &body,
                                      const std::string &full_request) {
  std::cout << "[Server] Request: " << method << " [" << path
            << "] (len: " << path.length() << ")" << std::endl;

  // Handle OPTIONS requests for CORS preflight
  if (method == "OPTIONS") {
    std::string response =
        "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
    return addCorsHeaders(response);
  }

  // Security: Check authorization for protected routes
  // Non-protected routes: /login, /stats/general (public dashboard)
  std::string username;
  if (path != "/login" && path != "/stats/general" &&
      !isAuthorized(full_request, username)) {
    return addCorsHeaders(
        "HTTP/1.1 401 Unauthorized\r\nContent-Type: "
        "application/json\r\n\r\n{\"error\":\"Non autorisé. Veuillez vous "
        "connecter.\"}");
  }

  std::string route_key = method + " " + path;

  // Check for specific ID routes
  if (method == "GET" &&
      std::regex_match(path, std::regex("/membres/[0-9]+"))) {
    std::regex id_regex("/membres/([0-9]+)");
    std::smatch match;
    if (std::regex_search(path, match, id_regex) && match.size() > 1) {
      std::string id = match[1].str();
      return addCorsHeaders(getMembreById(id, body));
    } else {
      return addCorsHeaders(
          "HTTP/1.1 400 Bad Request\r\nContent-Type: "
          "application/json\r\n\r\n{\"error\":\"Invalid ID format\"}");
    }
  } else if (method == "PUT" &&
             std::regex_match(path, std::regex("/membres/[0-9]+"))) {
    std::regex id_regex("/membres/([0-9]+)");
    std::smatch match;
    if (std::regex_search(path, match, id_regex) && match.size() > 1) {
      std::string id = match[1].str();
      return addCorsHeaders(putMembres(id, body));
    } else {
      return addCorsHeaders(
          "HTTP/1.1 400 Bad Request\r\nContent-Type: "
          "application/json\r\n\r\n{\"error\":\"Invalid ID format\"}");
    }
  } else if (method == "DELETE" &&
             std::regex_match(path, std::regex("/membres/[0-9]+"))) {
    std::regex id_regex("/membres/([0-9]+)");
    std::smatch match;
    if (std::regex_search(path, match, id_regex) && match.size() > 1) {
      std::string id = match[1].str();
      std::cout << "[Server] Deleting member ID: " << id << std::endl;
      return addCorsHeaders(deleteMembres(id, body));
    } else {
      return addCorsHeaders(
          "HTTP/1.1 400 Bad Request\r\nContent-Type: "
          "application/json\r\n\r\n{\"error\":\"Invalid ID format\"}");
    }
  } else if (method == "GET" &&
             std::regex_match(path, std::regex("/equipes/[0-9]+"))) {
    std::regex id_regex("/equipes/([0-9]+)");
    std::smatch match;
    if (std::regex_search(path, match, id_regex) && match.size() > 1) {
      std::string id = match[1].str();
      return addCorsHeaders(getEquipeById(id, body));
    } else {
      return addCorsHeaders(
          "HTTP/1.1 400 Bad Request\r\nContent-Type: "
          "application/json\r\n\r\n{\"error\":\"Invalid ID format\"}");
    }
  } else if (method == "PUT" &&
             std::regex_match(path, std::regex("/equipes/[0-9]+"))) {
    std::regex id_regex("/equipes/([0-9]+)");
    std::smatch match;
    if (std::regex_search(path, match, id_regex) && match.size() > 1) {
      std::string id = match[1].str();
      return addCorsHeaders(putEquipes(id, body));
    } else {
      return addCorsHeaders(
          "HTTP/1.1 400 Bad Request\r\nContent-Type: "
          "application/json\r\n\r\n{\"error\":\"Invalid ID format\"}");
    }
  } else if (method == "DELETE" &&
             std::regex_match(path, std::regex("/equipes/[0-9]+"))) {
    std::regex id_regex("/equipes/([0-9]+)");
    std::smatch match;
    if (std::regex_search(path, match, id_regex) && match.size() > 1) {
      std::string id = match[1].str();
      return addCorsHeaders(deleteEquipes(id, body));
    } else {
      return addCorsHeaders(
          "HTTP/1.1 400 Bad Request\r\nContent-Type: "
          "application/json\r\n\r\n{\"error\":\"Invalid ID format\"}");
    }
  } else if (method == "GET" &&
             std::regex_match(path, std::regex("/entraineurs/[0-9]+"))) {
    std::regex id_regex("/entraineurs/([0-9]+)");
    std::smatch match;
    if (std::regex_search(path, match, id_regex) && match.size() > 1) {
      std::string id = match[1].str();
      return addCorsHeaders(getEntraineurById(id, body));
    } else {
      return addCorsHeaders(
          "HTTP/1.1 400 Bad Request\r\nContent-Type: "
          "application/json\r\n\r\n{\"error\":\"Invalid ID format\"}");
    }
  } else if (method == "PUT" &&
             std::regex_match(path, std::regex("/entraineurs/[0-9]+"))) {
    std::regex id_regex("/entraineurs/([0-9]+)");
    std::smatch match;
    if (std::regex_search(path, match, id_regex) && match.size() > 1) {
      std::string id = match[1].str();
      return addCorsHeaders(putEntraineurs(id, body));
    } else {
      return addCorsHeaders(
          "HTTP/1.1 400 Bad Request\r\nContent-Type: "
          "application/json\r\n\r\n{\"error\":\"Invalid ID format\"}");
    }
  } else if (method == "DELETE" &&
             std::regex_match(path, std::regex("/entraineurs/[0-9]+"))) {
    std::regex id_regex("/entraineurs/([0-9]+)");
    std::smatch match;
    if (std::regex_search(path, match, id_regex) && match.size() > 1) {
      std::string id = match[1].str();
      return addCorsHeaders(deleteEntraineurs(id, body));
    } else {
      return addCorsHeaders(
          "HTTP/1.1 400 Bad Request\r\nContent-Type: "
          "application/json\r\n\r\n{\"error\":\"Invalid ID format\"}");
    }
  } else if (method == "GET" &&
             std::regex_match(path, std::regex("/seances/[0-9]+"))) {
    std::regex id_regex("/seances/([0-9]+)");
    std::smatch match;
    if (std::regex_search(path, match, id_regex) && match.size() > 1) {
      std::string id = match[1].str();
      return addCorsHeaders(getSeanceById(id, body));
    } else {
      return addCorsHeaders(
          "HTTP/1.1 400 Bad Request\r\nContent-Type: "
          "application/json\r\n\r\n{\"error\":\"Invalid ID format\"}");
    }
  } else if (method == "PUT" &&
             std::regex_match(path, std::regex("/seances/[0-9]+"))) {
    std::regex id_regex("/seances/([0-9]+)");
    std::smatch match;
    if (std::regex_search(path, match, id_regex) && match.size() > 1) {
      std::string id = match[1].str();
      return addCorsHeaders(putSeances(id, body));
    } else {
      return addCorsHeaders(
          "HTTP/1.1 400 Bad Request\r\nContent-Type: "
          "application/json\r\n\r\n{\"error\":\"Invalid ID format\"}");
    }
  } else if (method == "DELETE" &&
             std::regex_match(path, std::regex("/seances/[0-9]+"))) {
    std::regex id_regex("/seances/([0-9]+)");
    std::smatch match;
    if (std::regex_search(path, match, id_regex) && match.size() > 1) {
      std::string id = match[1].str();
      return addCorsHeaders(deleteSeances(id, body));
    } else {
      return addCorsHeaders(
          "HTTP/1.1 400 Bad Request\r\nContent-Type: "
          "application/json\r\n\r\n{\"error\":\"Invalid ID format\"}");
    }
  } else if (method == "GET" &&
             std::regex_match(path, std::regex("/paiements/[0-9]+"))) {
    std::regex id_regex("/paiements/([0-9]+)");
    std::smatch match;
    if (std::regex_search(path, match, id_regex) && match.size() > 1) {
      std::string id = match[1].str();
      return addCorsHeaders(getPaiementById(id, body));
    } else {
      return addCorsHeaders(
          "HTTP/1.1 400 Bad Request\r\nContent-Type: "
          "application/json\r\n\r\n{\"error\":\"Invalid ID format\"}");
    }
  } else if (method == "PUT" &&
             std::regex_match(path, std::regex("/paiements/[0-9]+"))) {
    std::regex id_regex("/paiements/([0-9]+)");
    std::smatch match;
    if (std::regex_search(path, match, id_regex) && match.size() > 1) {
      std::string id = match[1].str();
      return addCorsHeaders(putPaiements(id, body));
    } else {
      return addCorsHeaders(
          "HTTP/1.1 400 Bad Request\r\nContent-Type: "
          "application/json\r\n\r\n{\"error\":\"Invalid ID format\"}");
    }
  } else if (method == "DELETE" &&
             std::regex_match(path, std::regex("/paiements/[0-9]+"))) {
    std::regex id_regex("/paiements/([0-9]+)");
    std::smatch match;
    if (std::regex_search(path, match, id_regex) && match.size() > 1) {
      std::string id = match[1].str();
      return addCorsHeaders(deletePaiements(id, body));
    } else {
      return addCorsHeaders(
          "HTTP/1.1 400 Bad Request\r\nContent-Type: "
          "application/json\r\n\r\n{\"error\":\"Invalid ID format\"}");
    }
  } else if (routes.find(route_key) != routes.end()) {
    return addCorsHeaders(routes[route_key](body, full_request));
  } else {
    return addCorsHeaders(
        "HTTP/1.1 404 Not Found\r\nContent-Type: "
        "application/json\r\n\r\n{\"error\":\"Route not found\"}");
  }
  // Default return in case all conditions fail
  return addCorsHeaders(
      "HTTP/1.1 500 Internal Server Error\r\nContent-Type: "
      "application/json\r\n\r\n{\"error\":\"Internal server error\"}");
}

// Statistics endpoints
std::string HttpServer::getStatsGeneral(const std::string &body) {
  std::ostringstream oss;
  oss << "{\"totalMembres\":" << club->getTotalMembresDB()
      << ",\"totalEquipes\":" << club->getTotalEquipesDB()
      << ",\"totalEntraineurs\":" << club->getTotalEntraineursDB()
      << ",\"totalSeances\":" << club->getTotalSeancesDB()
      << ",\"totalPaiements\":" << club->getTotalPaiementsDB()
      << ",\"membresPayes\":" << club->getMembresPayesDB()
      << ",\"membresImpayes\":" << club->getMembresImpayesDB() << "}";

  std::string json_str = oss.str();
  std::string response_str =
      "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n" + json_str;
  return response_str;
}

std::string HttpServer::getStatsMembres(const std::string &body) {
  std::ostringstream oss;
  oss << "{\"total\":" << club->getTotalMembresDB()
      << ",\"actifs\":" << club->getTotalMembresDB()
      << "}"; // This would need to be refined to get active members
              // specifically

  std::string json_str = oss.str();
  std::string response_str =
      "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n" + json_str;
  return response_str;
}

std::string HttpServer::getStatsPaiements(const std::string &body) {
  std::ostringstream oss;
  oss << "{\"total\":" << club->getTotalPaiementsDB()
      << ",\"payes\":" << club->getMembresPayesDB()
      << ",\"impayes\":" << club->getMembresImpayesDB() << "}";

  std::string json_str = oss.str();
  std::string response_str =
      "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n" + json_str;
  return response_str;
}

std::string HttpServer::getStatsEquipes(const std::string &body) {
  std::ostringstream oss;
  oss << "{\"total\":" << club->getTotalEquipesDB() << "}";

  std::string json_str = oss.str();
  std::string response_str =
      "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n" + json_str;
  return response_str;
}

std::string HttpServer::getStatsEntraineurs(const std::string &body) {
  std::ostringstream oss;
  oss << "{\"total\":" << club->getTotalEntraineursDB() << "}";

  std::string json_str = oss.str();
  std::string response_str =
      "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n" + json_str;
  return response_str;
}

std::string HttpServer::getStatsSeances(const std::string &body) {
  std::ostringstream oss;
  oss << "{\"total\":" << club->getTotalSeancesDB() << "}";

  std::string json_str = oss.str();
  std::string response_str =
      "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n" + json_str;
  return response_str;
}

// Helper function to parse JSON body
std::map<std::string, std::string> parseJsonBody(const std::string &body) {
  std::map<std::string, std::string> result;
  // Regex improved to handle: "key": "value", "key": 123, "key": 123.45, "key":
  // ""
  std::regex pattern(
      "\"([^\"]+)\"\\s*:\\s*(?:\"([^\"]*)\"|([0-9]+\\.?[0-9]*)|null)");
  std::sregex_iterator iter(body.begin(), body.end(), pattern);
  std::sregex_iterator end;

  for (; iter != end; ++iter) {
    std::smatch match = *iter;
    std::string key = match[1].str();
    if (match[2].matched) {
      result[key] = match[2].str();
    } else if (match[3].matched) {
      result[key] = match[3].str();
    } else {
      result[key] = ""; // null or other
    }
  }

  return result;
}

// CRUD endpoints - proper implementations using database
std::string HttpServer::getMembres(const std::string &body) {
  std::vector<Membre> membres = club->listerMembresDB();
  std::ostringstream oss;
  oss << "[";
  for (size_t i = 0; i < membres.size(); ++i) {
    const Membre &m = membres[i];
    if (i > 0)
      oss << ",";
    oss << "{"
        << "\"id\":" << m.getId() << ","
        << "\"nom\":\"" << m.getNom() << "\","
        << "\"prenom\":\"" << m.getPrenom() << "\","
        << "\"dateNaissance\":\"" << m.getDateNaissance() << "\","
        << "\"email\":\"" << m.getEmail() << "\","
        << "\"telephone\":\"" << m.getTelephone() << "\","
        << "\"numeroMembre\":\"" << m.getNumeroMembre() << "\","
        << "\"dateInscription\":\"" << m.getDateInscription() << "\","
        << "\"statut\":\"" << m.getStatut() << "\","
        << "\"cotisation\":" << m.getCotisation() << "}";
  }
  oss << "]";

  std::string json_str = oss.str();
  std::string response_str =
      "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n" + json_str;
  return response_str;
}

std::string HttpServer::getMembreById(const std::string &id,
                                      const std::string &body) {
  // In a real implementation, you would fetch the specific member from the
  // database For now, returning a placeholder response
  std::string response_str =
      "HTTP/1.1 404 Not Found\r\nContent-Type: "
      "application/json\r\n\r\n{\"error\":\"Member not implemented yet\"}";
  return response_str;
}

std::string HttpServer::postMembres(const std::string &body) {
  // Parse the JSON body to extract member data
  std::map<std::string, std::string> data = parseJsonBody(body);

  try {
    // Create a new member from the parsed data
    Membre newMembre(0, // ID will be assigned by the system
                     data["nom"], data["prenom"], data["dateNaissance"],
                     data["email"], data["telephone"], data["numeroMembre"],
                     data["dateInscription"], data["statut"],
                     std::stod(data["cotisation"]));

    // Add the member to the database
    int newId = club->ajouterMembreDB(newMembre);

    if (newId != -1) {
      std::string response_str = "HTTP/1.1 201 Created\r\nContent-Type: "
                                 "application/json\r\n\r\n{\"status\":\"Membre "
                                 "created successfully\", "
                                 "\"id\":" +
                                 std::to_string(newId) + "}";
      return response_str;
    } else {
      std::string response_str = "HTTP/1.1 500 Internal Server Error\r\n"
                                 "Content-Type: application/json\r\n\r\n"
                                 "{\"error\":\"Failed to save member to "
                                 "database\"}";
      return response_str;
    }
  } catch (const std::exception &e) {
    std::string response_str =
        "HTTP/1.1 400 Bad Request\r\nContent-Type: "
        "application/json\r\n\r\n{\"error\":\"Invalid data provided: " +
        std::string(e.what()) + "\"}";
    return response_str;
  }
}

std::string HttpServer::putMembres(const std::string &id,
                                   const std::string &body) {
  // Parse the JSON body to extract member data
  std::map<std::string, std::string> data = parseJsonBody(body);

  try {
    // Create a member from the parsed data
    Membre updatedMembre(
        std::stoi(id), data["nom"], data["prenom"], data["dateNaissance"],
        data["email"], data["telephone"], data["numeroMembre"],
        data["dateInscription"], data["statut"], std::stod(data["cotisation"]));

    // Update the member in the database
    if (club->modifierMembreDB(std::stoi(id), updatedMembre)) {
      std::string response_str = "HTTP/1.1 200 OK\r\nContent-Type: "
                                 "application/json\r\n\r\n{\"status\":\"Membre "
                                 "updated successfully\", \"id\":" +
                                 id + "}";
      return response_str;
    } else {
      std::string response_str = "HTTP/1.1 500 Internal Server Error\r\n"
                                 "Content-Type: application/json\r\n\r\n"
                                 "{\"error\":\"Failed to update member in "
                                 "database\"}";
      return response_str;
    }
  } catch (const std::exception &e) {
    std::string response_str =
        "HTTP/1.1 400 Bad Request\r\nContent-Type: "
        "application/json\r\n\r\n{\"error\":\"Invalid data provided: " +
        std::string(e.what()) + "\"}";
    return response_str;
  }
}

std::string HttpServer::deleteMembres(const std::string &id,
                                      const std::string &body) {
  try {
    std::cout << "[Server] Executing delete for member ID: " << id << std::endl;
    // Delete the member from the database
    club->supprimerMembreDB(std::stoi(id));

    std::string response_str = "HTTP/1.1 200 OK\r\nContent-Type: "
                               "application/json\r\n\r\n{\"status\":\"Membre "
                               "deleted successfully\", \"id\":" +
                               id + "}";
    return response_str;
  } catch (const std::exception &e) {
    std::string response_str =
        "HTTP/1.1 500 Internal Server Error\r\nContent-Type: "
        "application/json\r\n\r\n{\"error\":\"Failed to delete member\"}";
    return response_str;
  }
}

std::string HttpServer::getEquipes(const std::string &body) {
  std::vector<Equipe> equipes = club->listerEquipesDB();
  std::ostringstream oss;
  oss << "[";
  for (size_t i = 0; i < equipes.size(); ++i) {
    const Equipe &e = equipes[i];
    if (i > 0)
      oss << ",";
    oss << "{"
        << "\"id\":" << e.getId() << ","
        << "\"nom\":\"" << e.getNom() << "\","
        << "\"sport\":\"" << e.getSport() << "\","
        << "\"coach\":" << e.getCoach() << "}";
  }
  oss << "]";

  std::string json_str = oss.str();
  std::string response_str =
      "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n" + json_str;
  return response_str;
}

std::string HttpServer::getEquipeById(const std::string &id,
                                      const std::string &body) {
  std::string response_str =
      "HTTP/1.1 404 Not Found\r\nContent-Type: "
      "application/json\r\n\r\n{\"error\":\"Equipe not implemented yet\"}";
  return response_str;
}

std::string HttpServer::postEquipes(const std::string &body) {
  // Parse the JSON body to extract equipe data
  std::map<std::string, std::string> data = parseJsonBody(body);

  try {
    // Create a new equipe from the parsed data
    Equipe newEquipe(0, // ID will be assigned by the system
                     data["nom"], data["sport"], std::stoi(data["coach"]));

    // Add the equipe to the database
    int newId = club->ajouterEquipeDB(newEquipe);

    if (newId != -1) {
      std::string response_str = "HTTP/1.1 201 Created\r\nContent-Type: "
                                 "application/json\r\n\r\n{\"status\":\"Equipe "
                                 "created successfully\", "
                                 "\"id\":" +
                                 std::to_string(newId) + "}";
      return response_str;
    } else {
      return "HTTP/1.1 500 Internal Server Error\r\nContent-Type: "
             "application/json\r\n\r\n{\"error\":\"Failed to save equipe\"}";
    }
  } catch (const std::exception &e) {
    std::string response_str =
        "HTTP/1.1 400 Bad Request\r\nContent-Type: "
        "application/json\r\n\r\n{\"error\":\"Invalid data provided: " +
        std::string(e.what()) + "\"}";
    return response_str;
  }
}

std::string HttpServer::putEquipes(const std::string &id,
                                   const std::string &body) {
  // Parse the JSON body to extract equipe data
  std::map<std::string, std::string> data = parseJsonBody(body);

  try {
    // Create an equipe from the parsed data
    Equipe updatedEquipe(std::stoi(id), data["nom"], data["sport"],
                         std::stoi(data["coach"]));

    // Update the equipe in the database
    if (club->modifierEquipeDB(std::stoi(id), updatedEquipe)) {
      std::string response_str = "HTTP/1.1 200 OK\r\nContent-Type: "
                                 "application/json\r\n\r\n{\"status\":\"Equipe "
                                 "updated successfully\", \"id\":" +
                                 id + "}";
      return response_str;
    } else {
      return "HTTP/1.1 500 Internal Server Error\r\nContent-Type: "
             "application/json\r\n\r\n{\"error\":\"Failed to update equipe\"}";
    }
  } catch (const std::exception &e) {
    std::string response_str =
        "HTTP/1.1 400 Bad Request\r\nContent-Type: "
        "application/json\r\n\r\n{\"error\":\"Invalid data provided: " +
        std::string(e.what()) + "\"}";
    return response_str;
  }
}

std::string HttpServer::deleteEquipes(const std::string &id,
                                      const std::string &body) {
  try {
    // Delete the equipe from the database
    if (club->supprimerEquipeDB(std::stoi(id))) {
      std::string response_str = "HTTP/1.1 200 OK\r\nContent-Type: "
                                 "application/json\r\n\r\n{\"status\":\"Equipe "
                                 "deleted successfully\", \"id\":" +
                                 id + "}";
      return response_str;
    } else {
      return "HTTP/1.1 500 Internal Server Error\r\nContent-Type: "
             "application/json\r\n\r\n{\"error\":\"Failed to delete equipe\"}";
    }
  } catch (const std::exception &e) {
    std::string response_str =
        "HTTP/1.1 500 Internal Server Error\r\nContent-Type: "
        "application/json\r\n\r\n{\"error\":\"Error: " +
        std::string(e.what()) + "\"}";
    return response_str;
  }
}

std::string HttpServer::getEntraineurs(const std::string &body) {
  std::vector<Entraineur> entraineurs = club->listerEntraineursDB();
  std::ostringstream oss;
  oss << "[";
  for (size_t i = 0; i < entraineurs.size(); ++i) {
    const Entraineur &e = entraineurs[i];
    if (i > 0)
      oss << ",";
    oss << "{"
        << "\"id\":" << e.getId() << ","
        << "\"nom\":\"" << e.getNom() << "\","
        << "\"prenom\":\"" << e.getPrenom() << "\","
        << "\"dateNaissance\":\"" << e.getDateNaissance() << "\","
        << "\"email\":\"" << e.getEmail() << "\","
        << "\"telephone\":\"" << e.getTelephone() << "\","
        << "\"specialite\":\"" << e.getSpecialite() << "\","
        << "\"salaire\":" << e.getSalaire() << "}";
  }
  oss << "]";

  std::string json_str = oss.str();
  std::string response_str =
      "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n" + json_str;
  return response_str;
}

std::string HttpServer::getEntraineurById(const std::string &id,
                                          const std::string &body) {
  std::string response_str =
      "HTTP/1.1 404 Not Found\r\nContent-Type: "
      "application/json\r\n\r\n{\"error\":\"Entraineur not implemented yet\"}";
  return response_str;
}

std::string HttpServer::postEntraineurs(const std::string &body) {
  // Parse the JSON body to extract entraineur data
  std::map<std::string, std::string> data = parseJsonBody(body);

  try {
    // Create a new entraineur from the parsed data
    Entraineur newEntraineur(0, // ID will be assigned by the system
                             data["nom"], data["prenom"], data["dateNaissance"],
                             data["email"], data["telephone"],
                             data["specialite"], std::stod(data["salaire"]));

    // Add the entraineur to the database
    int newId = club->ajouterEntraineurDB(newEntraineur);

    if (newId != -1) {
      std::string response_str =
          "HTTP/1.1 201 Created\r\nContent-Type: "
          "application/json\r\n\r\n{\"status\":\"Entraineur created "
          "successfully\", \"id\":" +
          std::to_string(newId) + "}";
      return response_str;
    } else {
      return "HTTP/1.1 500 Internal Server Error\r\nContent-Type: "
             "application/json\r\n\r\n{\"error\":\"Failed to save "
             "entraineur\"}";
    }
  } catch (const std::exception &e) {
    std::string response_str =
        "HTTP/1.1 400 Bad Request\r\nContent-Type: "
        "application/json\r\n\r\n{\"error\":\"Invalid data provided: " +
        std::string(e.what()) + "\"}";
    return response_str;
  }
}

std::string HttpServer::putEntraineurs(const std::string &id,
                                       const std::string &body) {
  // Parse the JSON body to extract entraineur data
  std::map<std::string, std::string> data = parseJsonBody(body);

  try {
    // Create an entraineur from the parsed data
    Entraineur updatedEntraineur(std::stoi(id), data["nom"], data["prenom"],
                                 data["dateNaissance"], data["email"],
                                 data["telephone"], data["specialite"],
                                 std::stod(data["salaire"]));

    // Update the entraineur in the database
    if (club->modifierEntraineurDB(std::stoi(id), updatedEntraineur)) {
      std::string response_str =
          "HTTP/1.1 200 OK\r\nContent-Type: "
          "application/json\r\n\r\n{\"status\":\"Entraineur updated "
          "successfully\", \"id\":" +
          id + "}";
      return response_str;
    } else {
      return "HTTP/1.1 500 Internal Server Error\r\nContent-Type: "
             "application/json\r\n\r\n{\"error\":\"Failed to update "
             "entraineur\"}";
    }
  } catch (const std::exception &e) {
    std::string response_str =
        "HTTP/1.1 400 Bad Request\r\nContent-Type: "
        "application/json\r\n\r\n{\"error\":\"Invalid data provided: " +
        std::string(e.what()) + "\"}";
    return response_str;
  }
}

std::string HttpServer::deleteEntraineurs(const std::string &id,
                                          const std::string &body) {
  try {
    // Delete the entraineur from the database
    if (club->supprimerEntraineurDB(std::stoi(id))) {
      std::string response_str =
          "HTTP/1.1 200 OK\r\nContent-Type: "
          "application/json\r\n\r\n{\"status\":\"Entraineur deleted "
          "successfully\", \"id\":" +
          id + "}";
      return response_str;
    } else {
      return "HTTP/1.1 500 Internal Server Error\r\nContent-Type: "
             "application/json\r\n\r\n{\"error\":\"Failed to delete "
             "entraineur\"}";
    }
  } catch (const std::exception &e) {
    std::string response_str =
        "HTTP/1.1 500 Internal Server Error\r\nContent-Type: "
        "application/json\r\n\r\n{\"error\":\"Error: " +
        std::string(e.what()) + "\"}";
    return response_str;
  }
}

std::string HttpServer::getSeances(const std::string &body) {
  std::vector<Seance> seances = club->listerSeancesDB();
  std::ostringstream oss;
  oss << "[";
  for (size_t i = 0; i < seances.size(); ++i) {
    const Seance &s = seances[i];
    if (i > 0)
      oss << ",";
    oss << "{"
        << "\"id\":" << s.getId() << ","
        << "\"equipe\":" << s.getIdEquipe() << ","
        << "\"date\":\"" << s.getDate() << "\","
        << "\"heure\":\"" << s.getHeureDebut() << "\","
        << "\"duree\":" << s.getDuree() << ","
        << "\"type\":\"" << s.getType() << "\","
        << "\"coach\":\"" << s.getCoach() << "\"}";
  }
  oss << "]";

  std::string json_str = oss.str();
  std::string response_str =
      "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n" + json_str;
  return response_str;
}

std::string HttpServer::getSeanceById(const std::string &id,
                                      const std::string &body) {
  std::string response_str =
      "HTTP/1.1 404 Not Found\r\nContent-Type: "
      "application/json\r\n\r\n{\"error\":\"Seance not implemented yet\"}";
  return response_str;
}

std::string HttpServer::postSeances(const std::string &body) {
  // Parse the JSON body to extract seance data
  std::map<std::string, std::string> data = parseJsonBody(body);

  try {
    // Create a new seance from the parsed data
    Seance newSeance(0, // ID will be assigned by the system
                     std::stoi(data["equipe"]), data["date"], data["heure"],
                     std::stoi(data["duree"]), data["type"], data["coach"]);

    // Add the seance to the database
    int newId = club->creerSeanceDB(newSeance);

    if (newId != -1) {
      std::string response_str = "HTTP/1.1 201 Created\r\nContent-Type: "
                                 "application/json\r\n\r\n{\"status\":\"Seance "
                                 "created successfully\", "
                                 "\"id\":" +
                                 std::to_string(newId) + "}";
      return response_str;
    } else {
      return "HTTP/1.1 500 Internal Server Error\r\nContent-Type: "
             "application/json\r\n\r\n{\"error\":\"Failed to save seance\"}";
    }
  } catch (const std::exception &e) {
    std::string response_str =
        "HTTP/1.1 400 Bad Request\r\nContent-Type: "
        "application/json\r\n\r\n{\"error\":\"Invalid data provided: " +
        std::string(e.what()) + "\"}";
    return response_str;
  }
}

std::string HttpServer::putSeances(const std::string &id,
                                   const std::string &body) {
  // Parse the JSON body to extract seance data
  std::map<std::string, std::string> data = parseJsonBody(body);

  try {
    // Create a seance from the parsed data
    Seance updatedSeance(std::stoi(id), std::stoi(data["equipe"]), data["date"],
                         data["heure"], std::stoi(data["duree"]), data["type"],
                         data["coach"]);

    // Update the seance in the database
    if (club->modifierSeanceDB(std::stoi(id), updatedSeance)) {
      std::string response_str = "HTTP/1.1 200 OK\r\nContent-Type: "
                                 "application/json\r\n\r\n{\"status\":\"Seance "
                                 "updated successfully\", \"id\":" +
                                 id + "}";
      return response_str;
    } else {
      return "HTTP/1.1 500 Internal Server Error\r\nContent-Type: "
             "application/json\r\n\r\n{\"error\":\"Failed to update seance\"}";
    }
  } catch (const std::exception &e) {
    std::string response_str =
        "HTTP/1.1 400 Bad Request\r\nContent-Type: "
        "application/json\r\n\r\n{\"error\":\"Invalid data provided: " +
        std::string(e.what()) + "\"}";
    return response_str;
  }
}

std::string HttpServer::deleteSeances(const std::string &id,
                                      const std::string &body) {
  try {
    // Delete the seance from the database
    if (club->supprimerSeanceDB(std::stoi(id))) {
      std::string response_str = "HTTP/1.1 200 OK\r\nContent-Type: "
                                 "application/json\r\n\r\n{\"status\":\"Seance "
                                 "deleted successfully\", \"id\":" +
                                 id + "}";
      return response_str;
    } else {
      return "HTTP/1.1 500 Internal Server Error\r\nContent-Type: "
             "application/json\r\n\r\n{\"error\":\"Failed to delete seance\"}";
    }
  } catch (const std::exception &e) {
    std::string response_str =
        "HTTP/1.1 500 Internal Server Error\r\nContent-Type: "
        "application/json\r\n\r\n{\"error\":\"Error: " +
        std::string(e.what()) + "\"}";
    return response_str;
  }
}

std::string HttpServer::getPaiements(const std::string &body) {
  std::vector<Paiement> paiements = club->getPaiementsParMembreDB(
      0); // This should be adjusted to get all payments
  std::ostringstream oss;
  oss << "[";
  for (size_t i = 0; i < paiements.size(); ++i) {
    const Paiement &p = paiements[i];
    if (i > 0)
      oss << ",";
    oss << "{"
        << "\"id\":" << p.getId() << ","
        << "\"membre\":" << p.getIdMembre() << ","
        << "\"montant\":" << p.getMontant() << ","
        << "\"date\":\"" << p.getDate() << "\","
        << "\"statut\":\"" << p.getStatut() << "\","
        << "\"type\":\"" << p.getType() << "}";
  }
  oss << "]";

  std::string json_str = oss.str();
  std::string response_str =
      "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n" + json_str;
  return response_str;
}

std::string HttpServer::getPaiementById(const std::string &id,
                                        const std::string &body) {
  std::string response_str =
      "HTTP/1.1 404 Not Found\r\nContent-Type: "
      "application/json\r\n\r\n{\"error\":\"Paiement not implemented yet\"}";
  return response_str;
}

std::string HttpServer::postPaiements(const std::string &body) {
  // Parse the JSON body to extract paiement data
  std::map<std::string, std::string> data = parseJsonBody(body);

  try {
    // Create a new paiement from the parsed data
    Paiement newPaiement(0, // ID will be assigned by the system
                         std::stoi(data["membre"]), std::stod(data["montant"]),
                         data["date"], data["statut"], data["type"]);

    // Add the paiement to the database
    int newId = club->enregistrerPaiementDB(newPaiement);

    if (newId != -1) {
      std::string response_str =
          "HTTP/1.1 201 Created\r\nContent-Type: "
          "application/json\r\n\r\n{\"status\":\"Paiement "
          "created successfully\", \"id\":" +
          std::to_string(newId) + "}";
      return response_str;
    } else {
      return "HTTP/1.1 500 Internal Server Error\r\nContent-Type: "
             "application/json\r\n\r\n{\"error\":\"Failed to save "
             "paiement\"}";
    }
  } catch (const std::exception &e) {
    std::string response_str =
        "HTTP/1.1 400 Bad Request\r\nContent-Type: "
        "application/json\r\n\r\n{\"error\":\"Invalid data provided: " +
        std::string(e.what()) + "\"}";
    return response_str;
  }
}

std::string HttpServer::putPaiements(const std::string &id,
                                     const std::string &body) {
  // Parse the JSON body to extract paiement data
  std::map<std::string, std::string> data = parseJsonBody(body);

  try {
    // Create a paiement from the parsed data
    Paiement updatedPaiement(std::stoi(id), std::stoi(data["membre"]),
                             std::stod(data["montant"]), data["date"],
                             data["statut"], data["type"]);

    // Update the paiement in the database
    if (club->modifierPaiementDB(std::stoi(id), updatedPaiement)) {
      std::string response_str =
          "HTTP/1.1 200 OK\r\nContent-Type: "
          "application/json\r\n\r\n{\"status\":\"Paiement "
          "updated successfully\", \"id\":" +
          id + "}";
      return response_str;
    } else {
      return "HTTP/1.1 500 Internal Server Error\r\nContent-Type: "
             "application/json\r\n\r\n{\"error\":\"Failed to update "
             "paiement\"}";
    }
  } catch (const std::exception &e) {
    std::string response_str =
        "HTTP/1.1 400 Bad Request\r\nContent-Type: "
        "application/json\r\n\r\n{\"error\":\"Invalid data provided: " +
        std::string(e.what()) + "\"}";
    return response_str;
  }
}

std::string HttpServer::deletePaiements(const std::string &id,
                                        const std::string &body) {
  try {
    // Delete the paiement from the database
    if (club->supprimerPaiementDB(std::stoi(id))) {
      std::string response_str =
          "HTTP/1.1 200 OK\r\nContent-Type: "
          "application/json\r\n\r\n{\"status\":\"Paiement "
          "deleted successfully\", \"id\":" +
          id + "}";
      return response_str;
    } else {
      return "HTTP/1.1 500 Internal Server Error\r\nContent-Type: "
             "application/json\r\n\r\n{\"error\":\"Failed to delete "
             "paiement\"}";
    }
  } catch (const std::exception &e) {
    std::string response_str =
        "HTTP/1.1 500 Internal Server Error\r\nContent-Type: "
        "application/json\r\n\r\n{\"error\":\"Error: " +
        std::string(e.what()) + "\"}";
    return response_str;
  }
}

std::string HttpServer::generateToken() {
  const std::string charset =
      "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  std::string token;
  token.reserve(32);
  for (int i = 0; i < 32; ++i) {
    token += charset[rand() % charset.length()];
  }
  return token;
}

bool HttpServer::isAuthorized(const std::string &request,
                              std::string &username) {
  size_t authPos = request.find("Authorization: Bearer ");
  if (authPos == std::string::npos) {
    authPos = request.find("authorization: bearer ");
  }
  if (authPos == std::string::npos) {
    authPos = request.find("Authorization: bearer ");
  }
  if (authPos == std::string::npos) {
    authPos = request.find("authorization: Bearer ");
  }

  if (authPos == std::string::npos) {
    return false;
  }

  size_t tokenStart = authPos + 22;
  size_t tokenEnd = request.find("\r\n", tokenStart);
  if (tokenEnd == std::string::npos) {
    tokenEnd = request.length();
  }

  std::string token = request.substr(tokenStart, tokenEnd - tokenStart);

  std::lock_guard<std::mutex> lock(sessions_mutex);
  if (active_sessions.find(token) != active_sessions.end()) {
    username = active_sessions[token];
    return true;
  }

  return false;
}

std::string HttpServer::handleCheckAuth(const std::string &request) {
  std::string username;
  if (isAuthorized(request, username)) {
    // In a real app, you'd fetch user details from DB
    return "HTTP/1.1 200 OK\r\nContent-Type: "
           "application/json\r\n\r\n{\"success\":true, \"username\":\"" +
           username + "\"}";
  } else {
    return "HTTP/1.1 401 Unauthorized\r\nContent-Type: "
           "application/json\r\n\r\n{\"success\":false}";
  }
}

std::string HttpServer::handleLogin(const std::string &body) {
  std::map<std::string, std::string> data = parseJsonBody(body);
  std::string username = data["username"];
  std::string password = data["password"];

  Utilisateur utilisateurConnecte;
  if (club->authentifierUtilisateur(username, password, utilisateurConnecte)) {
    std::string token = generateToken();

    {
      std::lock_guard<std::mutex> lock(sessions_mutex);
      active_sessions[token] = username;
    }

    std::string response_str =
        "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n"
        "Cache-Control: no-store, no-cache, must-revalidate\r\n\r\n"
        "{\"success\":true, \"token\":\"" +
        token +
        "\", \"user\":{\"id\":" + std::to_string(utilisateurConnecte.getId()) +
        ",\"username\":\"" + utilisateurConnecte.getUsername() +
        "\",\"role\":\"" + utilisateurConnecte.getRole() + "\"}}";
    return response_str;
  } else {
    return "HTTP/1.1 401 Unauthorized\r\nContent-Type: "
           "application/json\r\n\r\n{\"success\":false, \"message\":\"Nom "
           "d'utilisateur ou mot de passe incorrect\"}";
  }
}

std::string HttpServer::handleLogout(const std::string &request) {
  size_t authPos = request.find("Authorization: Bearer ");
  if (authPos == std::string::npos) {
    authPos = request.find("authorization: bearer ");
  }
  if (authPos == std::string::npos) {
    authPos = request.find("Authorization: bearer ");
  }
  if (authPos == std::string::npos) {
    authPos = request.find("authorization: Bearer ");
  }

  if (authPos != std::string::npos) {
    size_t tokenStart = authPos + 22;
    size_t tokenEnd = request.find("\r\n", tokenStart);
    if (tokenEnd == std::string::npos)
      tokenEnd = request.length();
    std::string token = request.substr(tokenStart, tokenEnd - tokenStart);

    std::lock_guard<std::mutex> lock(sessions_mutex);
    active_sessions.erase(token);
  }

  return "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n"
         "Cache-Control: no-store, no-cache, must-revalidate\r\n\r\n"
         "{\"status\":\"Logout successful\"}";
}