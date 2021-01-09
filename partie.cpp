#include "partie.hpp"



void waitingClientTrigger(Partie* p )
{
    p->waitingClient();
}

Partie::Partie(): enAttenteDeCo(false), portier(waitingClientTrigger, this)
{
    
}

Partie::~Partie()
{
    server.stop();
    std::cout << "le portier n'a pas fini sont service\n"; 
    portier.join();
    std::cout << "le portier a fini son service \n";
}

void Partie::waitingClient()
{
    while(true)
    {
        std::string pseudo = server.waitClient(); // bloquant 
        if(enAttenteDeCo && pseudo != "error")
        {
            std::cout << " nouvelle co \n"; 
            joueurs.push_back(Joueur(pseudo));
            std::cout << "bienvenue a " + pseudo + "\n";
        }
        else 
        {
            if(pseudo == "error")
            {
                std::cout << "erreur survenu pendant l'attente de nouvelle connexions" << std::endl;
            }
            else
            {
                std::cout << " connexion ignore " << std::endl;
            }
            break;
        }
    }        
}


/*
correspond au moment ou le client est dans le loby 
devra etre trigger avant que le premier client se connect 

*/
void Partie::demarage()
{
    std::string chef ="";

    enAttenteDeCo = true;
    while(true)
    {
        // std::string pseudo = server.waitClient(); // bloquant 
        // joueurs.push_back(Joueur(pseudo));
        
        // si c'est le premier connecter on le designe comme chef ( ! attention ! si le premier perd la connexion on aura plus de chef donc mdr la game)
        if(chef == "" && joueurs.size() > 0)
        {
            chef = joueurs[0].get_pseudo();
            server.sendTo(chef, "chef");
            std::cout << " le chef est " << chef << std::endl;
        }
        if( chef != "" )
        {
            
            std::string message  = server.getMessage(chef);
            if (message == std::string())
            {
                
                // rendre la main au cpu 
                std::this_thread::sleep_for(std::chrono::seconds(0));
                continue;
            }
            else if(message.substr(0,5) == "roles")
            {
                //std::cout << "recu " << message << std::endl;
                role_actif = split(message.substr(6,message.size()-6));
                //std::cout << "roles split : message recu = " + message + " apres traitement :" << std::endl;
                print(role_actif, " | ");
            }
            else if(message == "lancer")
            {
                std::cout << "cacacacacacaca\n" ;
                // std::cout << "ca va partir et on a " << joueurs.size() << " joueurs" << std::endl;
                enAttenteDeCo = false;
                // std::cout << "je suis la " << std::endl;
                // la partie doit se lancer il faut donc repartir les rôle
                // dans un premier temp test on vas mettre cupidon , loup_garou sorciere

                // distribution des roles 
                std::random_shuffle ( role_actif.begin(), role_actif.end() );
                


                for(unsigned int i = 0 ; i< joueurs.size(); i++)
                {
                    std::cout << "ennvoi a " + joueurs[i].get_pseudo() + "lancer " + role_actif[i] +"\n";
                    server.sendTo(joueurs[i].get_pseudo(), "lancer " + role_actif[i]);;
                    joueurs[i].set_role(role_actif[i]);
                    role_joueur[role_actif[i]] = i;
                    pseudo_role[joueurs[i].get_pseudo()] = role_actif[i];
                }
                break;
            }
            else 
            {
                std::cout << "message recu non pris en compte " << message << std::endl;
            }
            
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::seconds(0));// on passe la main au cpu 
        }
    }
    // server.sendTo(chef , "stop");
}


void Partie::run()
{
    // pour la sorciere , si y'en a pas il y aura juste deux booleen pour rien 
    bool reviveSpell = true;
    bool deathSpell = true; 
    std::string witchTarget = "";

    std::string love1 = "";
    std::string love2 = "";

    // salvateur 
    std::string last_protected = " ";// avec un espace pour l'init diferement de la variable local en phase salvateur 

    // loup_garou 
    std::string cible = "";



    // debut de la premiere nuit
    if(std::find(role_actif.begin(), role_actif.end(),"cupidon") != role_actif.end())
    {
        server.broadcast("phase cupidon");
        std::string cupidon = joueurs[role_joueur["cupidon"]].get_pseudo();

        std::string message_targets = "" ; //  variable qui vas contenir le message envoyer par cupidon de la forme "love pseudo1 pseudo2"
        while(message_targets == "")
        {
            std::this_thread::sleep_for(std::chrono::seconds(0)); // on rend la main au proc 
            message_targets = server.getMessage(cupidon);
        }
        unsigned int debut1 = 5;
        unsigned int nbchar1 = 1;
        while(message_targets[debut1 + nbchar1] != ' ')
        {
            nbchar1 ++;
        }
        nbchar1 --;
        unsigned int debut2 = debut1 + nbchar1 + 2;
        unsigned int nbchar2 = message_targets.size() - debut2;

        love1 = message_targets.substr(debut1, nbchar1);
        love2 = message_targets.substr(debut2, nbchar2);
        server.sendTo(love1, "love " + love2);
        server.sendTo(love2, "love " + love1);

        server.broadcast("phase amoureux");
        // bon quand y aura un timer
    }
    bool notend = true;
    while(notend)
    {
        //voyante
        if(std::find(role_actif.begin(), role_actif.end(),"voyante") != role_actif.end())
        {
            server.broadcast("phase voyante");
            
            std::string message_target = "" ; //  variable qui vas contenir le message envoyer par la sorciere "look pseudo"
            while(message_target == "")
            {
                std::this_thread::sleep_for(std::chrono::seconds(0)); // on rend la main au proc 
                message_target = server.getMessage(joueurs[role_joueur["voyante"]].get_pseudo());
            }
            std::string a_voir = message_target.substr(5, message_target.size()-5);
            std::string role_vue = pseudo_role[a_voir]; 
            server.sendTo(joueurs[role_joueur["voyante"]].get_pseudo(), "looked " + role_vue);
        }


        //salvateur 
        if(std::find(role_actif.begin(), role_actif.end(),"salvateur") != role_actif.end())
        {
            server.broadcast("phase salvateur");
            std::string wanted_to_protect = "" ;
            do
            {
                if(wanted_to_protect == last_protected)
                {
                    server.sendTo(joueurs[role_joueur["salvateur"]].get_pseudo(), "already protected");

                }
                std::string message_target = "" ; //  variable qui vas contenir le message envoyer par la sorciere "protect pseudo"
                while(message_target == "")
                {
                    std::this_thread::sleep_for(std::chrono::seconds(0)); // on rend la main au proc 
                    message_target = server.getMessage(joueurs[role_joueur["salvateur"]].get_pseudo());
                }
                wanted_to_protect = message_target.substr(8, message_target.size()-8);
            }while(last_protected == wanted_to_protect);
            last_protected = wanted_to_protect;
        }

        // loup_garou (a revoir quand on aura casé la possibilité de mettre plusieur loup) remarque : on peut considéré loup_garou1 loup_garou2 comme 2 role different du meme camp 
        if(std::find(role_actif.begin(), role_actif.end(),"loup_garou") != role_actif.end())
        {
            std::cout << "phase loup_garou \n";
            server.broadcast("phase loup_garou");
            
            std::string message_target = "" ; //  variable qui vas contenir le message envoyer par chacun des loup garou "kill pseudo"
            while(message_target == "")
            {
                std::this_thread::sleep_for(std::chrono::seconds(0)); // on rend la main au proc 
                message_target = server.getMessage(joueurs[role_joueur["loup_garou"]].get_pseudo());
            }
            cible = message_target.substr(5, message_target.size()-5);
            std::cout << "la cible est " + message_target + " de taille " <<  message_target.size()  << "\n"; 
        }

        //sorciere
        if(std::find(role_actif.begin(), role_actif.end(),"sorciere") != role_actif.end())
        {
            server.broadcast("phase sorciere");

            std::string action= "";
            while(action == "")
            {
                std::this_thread::sleep_for(std::chrono::seconds(0)); // on rend la main au proc 
                server.sendTo(joueurs[role_joueur["sorciere"]].get_pseudo(), "target " + cible);
                action = server.getMessage(joueurs[role_joueur["sorciere"]].get_pseudo());// "kill x revive"
            }
            if(action.substr(0, 4) == "kill")
            {
                unsigned int debut = 5;
                unsigned int nbchar = 1;
                while(action[debut + nbchar] != ' ')
                {
                    nbchar ++;
                }
                if(deathSpell)
                {
                   witchTarget = action.substr(5,nbchar-1);
                   deathSpell = false;
                }
                if(action.substr(debut + nbchar + 1 , 6 ) == "revive" )
                {
                    if(reviveSpell)
                    {
                        cible = "";
                        reviveSpell = false;
                    }
                }
            }
            else if(action.substr(0, 6) == "revive")
            {
                if(reviveSpell)
                {
                    cible = "";
                    reviveSpell = false ;
                }
            }
        }


        // preparation jour 

        if(isEnd()) // s'arrette a la premiere nuit, a changer quand le reste est fait 
        {
            notend=false;
        }
        if(cible != "")
        {
            std::string roleMort = pseudo_role[cible];
            unsigned int indice_mort = role_joueur[roleMort];
            std::string pseudo_mort = joueurs[indice_mort].get_pseudo();
            server.broadcast("mort " + cible + " role " + roleMort);
        }
        server.broadcast("mort " + witchTarget);

        //jour 
        // server.broadcast("phase jour");

        // server.broadcast("phase election");
        ///

        // server.broadcast("phase vote");

    }
}

bool Partie::isEnd() // ne prend pas en compte les camp qui peuvent gagner avec plusieur camp 
{
    // int camp = -1;
    // for(unsigned int i =0 ; i<joueurs.size; i++ )
    // {
    //     if(joueurs[i].isAlive() )
    //     {
    //         if(camp = -1)
    //         {
    //             camp = joueurs[i].get_camp();
    //         }
    //         else if(joueurs[i].get_camp() != camp )
    //         {
    //             return false;
    //         }
    //     }
        
    // }
    return true;

    /// pour le moment j'ai pas encore mis d'acces au camp de facon simple donc on revera ca 
}