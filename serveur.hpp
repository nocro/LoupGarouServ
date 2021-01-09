#ifndef SERVEUR_HPP
#define SERVEUR_HPP


#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <string> 
#include <iostream>
#include <cstdlib> 
#include <thread>
#include <arpa/inet.h>
#include <mutex>
#include <vector>
#include <queue>
#include <map>
#include "utils.hpp"
#define PORT 8080 


 
// pas copiable car mutex pas copiable 
class SharedMessage
{
    private:
        std::string message;
        std::mutex m_mutex;
    public:
        SharedMessage()
        {
            
        }
        void change(std::string newMessage)
        {
            m_mutex.lock();
            message = newMessage;
            m_mutex.unlock();
        }
        std::string read()
        {
            m_mutex.lock();
            std::string tmp = message;
            m_mutex.unlock();
            return tmp;
        }

};

template<typename T>
class SharedQueue
{
    private:
        std::queue<T> messages;
        std::mutex m_mutex;
    public:
        SharedQueue()
        {
            
        }
        void push(T newMessage)
        {
            m_mutex.lock();
            messages.push(newMessage);
            m_mutex.unlock();
        }
        T pop()
        {
            m_mutex.lock();
            if(messages.empty())
            {
                m_mutex.unlock();
                return T();
            }
            else
            {
                T tmp = messages.front();
                messages.pop();
                m_mutex.unlock();
                return tmp;

            }          
        }
};


// pas copiable car SharedQueue pas copiable 
class Client // faudra  implementé la gestion de deconexion avec un bool actif et des messages envoyer pour verifié l'etat de la connexion
{
    private:
        std::string m_pseudo;
        int m_socket;
        sockaddr_in m_address;
        std::string m_address_str;


        SharedQueue<std::string> lastMessageReceved;
        // int newMessage;

    public:
        Client(std::string pseudo, int socket, sockaddr_in address);
        ~Client();
        int sendMe(std::string&& toSend)const ;
        int sendMe(std::string& toSend)const ;
        std::string get_pseudo();
        std::string get_address();
        // bool have_message(); 
        std::string get_message();// ! attention ! supprime le dernier message donc peut etre lu que une fois (normalement ca pause pas de probleme)

        void listen(); // attention bloquant
};



// copiable car clients est un vecteur de pointeur 
class Server
{
private: 
    int m_socket;
    std::vector<Client*> clients;
    std::vector<std::thread> readers;
    // std::map<std::string, int> pseudo_socket;
    std::map<std::string, int> indiceClient; // clef pseudo -> valeur : indice dans le tableau clients

public:
    Server();
    ~Server();

    std::string waitClient(); //bloquant  renvoi le pseudo 

    int sendTo(std::string pseudo, std::string toSend); 
    void broadcast(std::string& toSend) const;
    void broadcast(std::string&& toSend) const;

    std::string getMessage(std::string pseudo);
    void stop();


};






















#endif