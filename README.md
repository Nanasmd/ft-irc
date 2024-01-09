ft_IRC

Ce projet consiste à créer un serveur IRC. La communication entre le client et le serveur s'effectue via TCP/IP.

Résumé

L'objectif du projet est de créer un serveur IRC. Internet Relay Chat ou IRC est un protocole de communication textuelle sur Internet. Il offre des messages en temps réel qui peuvent être publics ou privés. Les utilisateurs peuvent échanger des messages directs et rejoindre des canaux de groupe. (channel)

Le projet devait être codé en C++98, cependant, nous utilisons la bibliothèque libcurl pour le bot dans la section bonus. Libcurl est utilisée pour envoyer des requêtes à chatGPT et obtenir la réponse.

Utilisation
Le client de référence que nous avons utilisé est irssi, donc les fonctionnalités /DCC telles que le chat et les transferts de fichiers sont pris en charge.

Exécutez make à la racine du projet et lancez comme suit :


./ircserv <port> <mot de passe>

port -- le port par défaut pour IRC est 6667.
Exécutez make bonus pour ajouter le bot. Le bot effectue 2 tâches :

il surveille les canaux et exclut les utilisateurs écrivant des mots interdits,

il transfère ses messages privés d'autres utilisateurs à chatGPT en utilisant libcurl et renvoie la réponse aux utilisateurs dans un message privé :

./banbot






[Irssi est un type spécifique de client IRC. IRC, ou Internet Relay Chat, est un protocole pour la messagerie texte interactive en temps réel sur Internet (chat) ou pour des conférences synchrones. Il est principalement conçu pour la communication en groupe (de plusieurs à plusieurs) dans des forums de discussion appelés canaux, mais permet également une communication en tête-à-tête via des messages privés.

Irssi, en revanche, est un client IRC particulier. C'est un programme utilisé pour se connecter à un réseau IRC. Irssi est connu pour son interface entièrement textuelle et est hautement personnalisable. Il peut être scripté et modifié pour répondre aux besoins et préférences individuels, ce qui en fait un choix populaire pour ceux qui préfèrent une interface en ligne de commande ou ont besoin d'un client léger et efficace.

En résumé, IRC est le protocole ou le système de chat, tandis qu'Irssi est un outil ou client utilisé pour se connecter à ce système. Il existe de nombreux clients IRC différents, chacun avec ses propres fonctionnalités et interface utilisateur, et Irssi est l'un de ces clients.]

[Le chat DCC (Direct Client-to-Client) est une fonctionnalité de certains clients IRC (Internet Relay Chat) qui permet une communication directe entre deux utilisateurs, sans passer par le serveur IRC. Contrairement aux canaux de discussion standard d'IRC où tous les messages passent par un serveur central, le chat DCC établit une connexion directe entre les clients des utilisateurs, permettant un échange de messages de manière plus privée et sécurisée.

Voici quelques points clés à propos du chat DCC :

Connexion directe : Dans un chat DCC, les utilisateurs établissent une connexion peer-to-peer. Cela signifie que les messages ne transitent pas par un serveur central, ce qui peut être bénéfique pour la confidentialité et la vitesse de communication.

Transferts de fichiers : En plus des conversations textuelles, DCC est souvent utilisé pour transférer des fichiers directement d'un utilisateur à l'autre. Cette méthode de transfert de fichiers est également peer-to-peer.

Sécurité : Bien que le chat DCC offre une certaine confidentialité en évitant les serveurs IRC, il peut exposer les adresses IP des utilisateurs aux personnes avec lesquelles ils se connectent, ce qui peut être une préoccupation en matière de sécurité et de confidentialité.

Configuration : La mise en place d'une session de chat DCC peut nécessiter une configuration réseau supplémentaire, notamment la gestion du NAT (Network Address Translation) et des pare-feu, car elle nécessite une connexion directe.

Compatibilité du client : Tous les clients IRC ne supportent pas nécessairement DCC. Il est important de vérifier si cette fonctionnalité est prise en charge par le client IRC que vous utilisez.

En résumé, le chat DCC dans IRC offre une méthode de communication directe entre les utilisateurs, utile pour des discussions privées et des transferts de fichiers, mais nécessite une attention particulière en termes de configuration de réseau et de considérations de sécurité.]

[ Libcurl est une bibliothèque open source populaire et largement utilisée qui offre un ensemble de fonctions et d'outils permettant de transférer des données sur différents protocoles réseau. Elle est principalement conçue pour effectuer des requêtes réseau et récupérer ou envoyer des données depuis et vers des serveurs distants ou des ressources sur Internet. Certaines des caractéristiques clés et des aspects de libcurl comprennent :

Support de plusieurs protocoles : Libcurl prend en charge une large gamme de protocoles réseau, notamment HTTP, HTTPS, FTP, FTPS, SCP, SFTP, LDAP, POP3, SMTP, IMAP, et bien d'autres. Cette polyvalence permet aux développeurs d'interagir avec différents types de serveurs et de services.

Multiplateforme : Libcurl est multiplateforme et peut être utilisée sur divers systèmes d'exploitation, notamment Linux, macOS, Windows et bien d'autres. Cela en fait un choix polyvalent pour la création d'applications liées au réseau qui doivent s'exécuter sur différentes plates-formes.

API : Libcurl fournit une API simple et facile à utiliser dans divers langages de programmation, notamment le C, le C++, et d'autres. Cette API simplifie le processus de réalisation de requêtes réseau et de gestion des réponses.

Transfert de données : Elle gère efficacement le transfert de données, y compris le téléchargement de fichiers, l'envoi de données ou la réalisation de requêtes HTTP et la réception de réponses. Elle prend en charge le transfert de données en continu, ce qui est utile pour le téléchargement ou l'envoi de fichiers volumineux ou le traitement de données par morceaux.

Sécurité : Libcurl prend en charge les protocoles sécurisés tels que HTTPS et offre des options pour le chiffrement SSL/TLS et la validation des certificats, ce qui la rend adaptée aux communications sécurisées sur Internet.

Pooling de connexions : Elle inclut la prise en charge de la réutilisation et de la mise en pool des connexions, ce qui peut améliorer les performances des applications qui doivent effectuer de multiples requêtes vers le même serveur.

Extensibilité : Libcurl peut être étendue grâce à l'utilisation de diverses options et paramètres permettant de personnaliser son comportement en fonction des besoins spécifiques.

Open Source : Libcurl est un logiciel open source, ce qui signifie qu'elle est librement disponible pour une utilisation, une modification et une distribution en vertu de licences open source permissives (par exemple, la licence MIT ou la LGPL).

Les développeurs l'utilisent couramment lorsqu'ils créent des navigateurs Web, des gestionnaires de téléchargement, des clients de messagerie électronique et diverses autres applications liées au réseau, qui nécessitent la réalisation de requêtes HTTP, le téléchargement/chargement de fichiers ou le transfert de données sur différents protocoles réseau. Sa facilité d'utilisation, sa robustesse et son support étendu des protocoles en font un choix populaire pour la gestion des communications réseau dans le développement logiciel.]