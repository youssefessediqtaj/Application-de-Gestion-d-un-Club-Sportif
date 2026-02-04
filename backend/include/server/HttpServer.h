#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <condition_variable>
#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <thread>

class ClubSportif; // Forward declaration

class HttpServer {
private:
  int server_fd;
  int port;
  std::thread server_thread;
  bool running;
  ClubSportif *club;

  std::map<std::string,
           std::function<std::string(const std::string &, const std::string &)>>
      routes;
  std::mutex mtx;
  std::condition_variable cv;

  // Auth and Session Management
  std::map<std::string, std::string> active_sessions; // Token -> Username
  std::mutex sessions_mutex;

  std::string generateToken();
  bool isAuthorized(const std::string &request, std::string &username);
  std::string handleCheckAuth(const std::string &request);

public:
  HttpServer(int port, ClubSportif *club);
  ~HttpServer();

  void start();
  void stop();

  void setupRoutes();
  std::string handleRequest(const std::string &method, const std::string &path,
                            const std::string &body,
                            const std::string &full_request);

  // API endpoints
  std::string getStatsGeneral(const std::string &body);
  std::string getStatsMembres(const std::string &body);
  std::string getStatsPaiements(const std::string &body);
  std::string getStatsEquipes(const std::string &body);
  std::string getStatsEntraineurs(const std::string &body);
  std::string getStatsSeances(const std::string &body);

  // CRUD endpoints
  std::string getMembres(const std::string &body);
  std::string getMembreById(const std::string &id, const std::string &body);
  std::string postMembres(const std::string &body);
  std::string putMembres(const std::string &id, const std::string &body);
  std::string deleteMembres(const std::string &id, const std::string &body);

  std::string getEquipes(const std::string &body);
  std::string getEquipeById(const std::string &id, const std::string &body);
  std::string postEquipes(const std::string &body);
  std::string putEquipes(const std::string &id, const std::string &body);
  std::string deleteEquipes(const std::string &id, const std::string &body);

  std::string getEntraineurs(const std::string &body);
  std::string getEntraineurById(const std::string &id, const std::string &body);
  std::string postEntraineurs(const std::string &body);
  std::string putEntraineurs(const std::string &id, const std::string &body);
  std::string deleteEntraineurs(const std::string &id, const std::string &body);

  std::string getSeances(const std::string &body);
  std::string getSeanceById(const std::string &id, const std::string &body);
  std::string postSeances(const std::string &body);
  std::string putSeances(const std::string &id, const std::string &body);
  std::string deleteSeances(const std::string &id, const std::string &body);

  std::string getPaiements(const std::string &body);
  std::string getPaiementById(const std::string &id, const std::string &body);
  std::string postPaiements(const std::string &body);
  std::string putPaiements(const std::string &id, const std::string &body);
  std::string deletePaiements(const std::string &id, const std::string &body);

  std::string handleLogin(const std::string &body);
  std::string handleLogout(const std::string &request);
};

#endif // HTTP_SERVER_H