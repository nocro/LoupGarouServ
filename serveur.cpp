#include "serveur.hpp"



Client::Client(std::string pseudo, int socket, sockaddr_in address): m_pseudo(pseudo), m_socket(socket), m_address(address)
{
    char buff [INET_ADDRSTRLEN] = {0};// jpense inutile
    m_address_str = inet_ntop(m_address.sin_family, (void*)&m_address.sin_addr, buff, INET_ADDRSTRLEN );
}

Client::~Client()
{
    std::cout << "destruction du client " + m_pseudo + " avec l'adress " + m_address_str << std::endl;
}

int Client::sendMe(std::string&& toSend) const 
{
    // std::cout << "envoi a " + m_pseudo + " " + toSend + "\n" ;
    char separator = 20;
    toSend += separator;
    return send(m_socket , toSend.c_str() , strlen(toSend.c_str()) , 0 ); 
}
int Client::sendMe(std::string& toSend) const 
{
    // std::cout << "envoi a " + m_pseudo + " " + toSend + "\n" ;
    char separator = 20;
    toSend += separator;
    return send(m_socket , toSend.c_str() , strlen(toSend.c_str()) , 0 ); 
}
std::string Client::get_pseudo()
{
    return m_pseudo;
}
std::string Client::get_address()
{
    return m_address_str;
}

std::string Client::get_message()
{

    return lastMessageReceved.pop();
    
}
void Client::listen()// attention blocquant
{
    char buffer[1024] = { 0 };
    std::string lastRead = "";
    int valread;
    std::cout << "listener lancer\n";
    bool stop = false;
    do {
        valread = read(m_socket, buffer, 1024);
        if (valread > 0)
        {
            char separator = 20; 
            std::vector<std::string> messages = split(std::string(buffer), separator);
            //lastRead = buffer;
            // std::cout << m_pseudo + " a envoyer " << messages.size() << "\n" ;
            for(auto m : messages)
            {
                if( m == "stop ")
                {
                    stop = true ;
                }
                else
                {
                    // std::cout << "message recu de " + m_pseudo + " " + m+  "\n";
                    lastMessageReceved.push(m);
                    // newMessage += 1;

                }
            }
            memset(buffer,0, sizeof(buffer));
        }
        else if (valread == 0)
        {
                
        }
        else
        {
            // ca peut surment vouloir dire que la connexion a été cloturé coté client et de toute facon la connexion est perdu donc on stop le reader 
            stop = true;
            std::cout << "erreur lor de la reception du client " + m_pseudo << std::endl;


        }
    } while (!stop);
    std::cout << "fin du reading sur " << m_pseudo << std::endl;

}



void listenner(Client* client )
{
    client->listen();
}




Server::Server()
{
    if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 

    int opt = 1;   
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 

    sockaddr_in address; 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(m_socket, (struct sockaddr *)&address,  sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(m_socket, 3) < 0) // la on limite a 3 connections 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
}

Server::~Server()
{
    for( unsigned int i=0 ; i< readers.size() ; i++)
    {
        readers[i].join();
    }
    std::cout << "tout les readers sont stop" << std::endl;
}



std::string Server::waitClient()
{
    sockaddr_in address;
    int addrlen = sizeof(address);
    int socket = accept(m_socket,(struct sockaddr *)&address, (socklen_t*)&addrlen); // bloquant 
    if(socket <= 0)
    {
        if(socket == 0)
        {
            std::cout << "socket == 0 je croi que la connexion ne s'est pas faite " << std::endl;
        }
        return "error";
    }

    // le client doit envoyer sont pseudo au moment de la connexion 
    char buffer[1024] = { 0 };
    int valread =  read(socket, buffer, 1024); // bloquant mais si tout est fait correctement le client envoi son pseudo direct donc pas sensé bloqué longtemp
    if(valread <= 0 )
    {
        if(valread == 0 ) 
        {
            std::cout << "jsuis pas sur mais il me semble que la connexion soi interonpu" << std::endl;
        }
        else
            std::cout << "erreur lor de la reception " << valread << std::endl;
    }


    // une foi que le client a envoyer son pseudo on l'enregistre  (buffer contien le pseudo)
    clients.push_back(new Client(buffer, socket , address));

    // on associe le pseudo au socket 
    //pseudo_socket[std::string(buffer)] = socket;
    indiceClient[std::string(buffer)] = clients.size()-1 ;

    // on lance le thread de lecture sur le socket client
    readers.push_back(std::thread(listenner, clients.back())); 
    return std::string(buffer);// on retourne le pseudo 

}

int Server::sendTo(std::string pseudo, std::string toSend)
{
    return clients[indiceClient[pseudo]]->sendMe(toSend);
    //return send(pseudo_socket[pseudo] , toSend.c_str() , strlen(toSend.c_str()) , 0 ); 
}

std::string Server::getMessage(std::string pseudo)
{
    return clients[indiceClient[pseudo]]->get_message();
}



void Server::broadcast(std::string& toSend) const 
{
    for(unsigned int i = 0; i<clients.size(); i++ )
    {
        clients[i]->sendMe(toSend);
    }
}

void Server::broadcast(std::string&& toSend) const 
{
    for(unsigned int i = 0; i<clients.size(); i++ )
    {
        if( clients[i]->sendMe(toSend) <= 0)
        {
            std::cout << "erreur envoi lor d'un broadcast" << std::endl;
        }
    }
}


void Server::stop()
{
    for(unsigned int i = 0; i< clients.size() ; i++)
    {
        clients[i]->sendMe("stop");
    }
    if(shutdown(m_socket, SHUT_RD) == -1)
    {
        std::cout << "erreur lors de l'arret du serveur" << std::endl;
    }
}





