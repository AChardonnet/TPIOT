# TP IOT
## Introduction
Ce dépot rassemble tous les fichiers nécessaire à la réalisation d'un TP d'IOT.
Le TP suit une architecture Edge Fog Cloud. Le cloud est hébergé sur un ordinateur personnel, le fog sur une machine du type Raspberry Pi et le edge sur une carte esp32.

Pour que tout fonctionne correctement, les 3 machines doivent être connectées au même réseau. Si c'est disponible sur votre ordinateur, il est plus simple de les connecter au partage de connexion de votre ordinateur afin de créer un réseau isolé.
## Installation

 - ### Cloud
	 1. Se placer dans le dossier cloud.
	 2. Dans le fichier `docker-compose.yml`, modifier le service `kafka-broker` modifiez la variable d'environnement `KAFKA_ADVERTISED_LISTENERS` pour que l'ip indiquée corresponde à celle du cloud (de votre ordinateur). Attention un ordinateur peut avoir plusieurs adresses ip. Celle du partage de connexion commencera généralement par `192.168`.
	 3. `docker compose up -d`
	 4. Après quelques instants, la page `http://localhost:9021` devrait afficher une page d'accueil.
 - ### Fog
	 1. Préparer la machine qui va héberger le fog (voir section détaillée ci-après).
	 2. Se placer dans le dossier fog.
	 3. Dans le fichier `docker-compose.yml`, modifier l'ip dans `KAFKA_MQTT_BOOTSTRAP_SERVERS` pour quelle corresponde à celle du cloud
	 4. `docker compose up -d`
 - ### Edge
	 1. Créer une copie du fichier `credentials.h` dans les dossier se terminant par `.ino`
	 2. Entrer le SSID et le mot de passe du Wi-Fi à utiliser dans tous les fichiers `credentials.h` créés.
	 3. Modifier l'ip dans les fichiers `conf.h` pour qu'elle corresponde à l'ip de la machine fog.
	 4. Avec l'IDE Arduino ouvrir le dossier .ino et transférer le code sur la carte esp32.
 - ### Consumer
	 1. Créer un environnement virtuel `python  -m  venv venv`
	 2. Activer l'environnement virtuel ([se référer au tableau de la documentation](https://docs.python.org/3/library/venv.html#how-venvs-work)).
	 3. Installer les dépendances `pip install -r requirements.txt`
	 4. Exécuter le code `python ./DHTconsumer.py`

## Installer un Raspberry Pi

### SSH

SSH (_Secure Shell_) est un protocole permettant d’établir une connexion sécurisée à distance entre deux machines via un réseau, en chiffrant les communications. Il est principalement utilisé pour l’administration de serveurs et le transfert de fichiers de manière sécurisée.

Pour vous connecter au Raspberry Pi sans utiliser de périphériques, il faut utiliser SSH. Si la commande `ssh` renvoie un message sur l'utilisation de la commande alors SSH est déjà installé, sinon il faut l'installer.
```
>ssh
usage: ssh [-46AaCfGgKkMNnqsTtVvXxYy] [-B bind_interface] [-b bind_address]
           [-c cipher_spec] [-D [bind_address:]port] [-E log_file]
           [-e escape_char] [-F configfile] [-I pkcs11] [-i identity_file]
           [-J destination] [-L address] [-l login_name] [-m mac_spec]
           [-O ctl_cmd] [-o option] [-P tag] [-p port] [-Q query_option]
           [-R address] [-S ctl_path] [-W host:port] [-w local_tun[:remote_tun]]
           destination [command [argument ...]]
```
#### Installer OpenSSH (Windows)
https://learn.microsoft.com/fr-fr/windows-server/administration/openssh/openssh_install_firstuse?tabs=gui&pivots=windows-server-2025

#### Installer SSH (Linux)
`sudo apt install openssh-client`

#### Générer une paire de clefs

La paire de clés en cryptographie asymétrique (comme SSH) fonctionne comme un cadenas et une clé : la clé publique est comme un cadenas que tout le monde peut fermer (chiffrer un message), mais seule la clé privée, gardée secrète, peut l’ouvrir (déchiffrer le message).

Générer les clefs `ssh-keygen -t ecdsa`. En cas de problème, se référer à cette [page](https://learn.microsoft.com/en-us/windows-server/administration/openssh/openssh_keymanagement).
Par défaut les clefs se trouvent dans le dossier `C:\Users\<Utilisateur>\.ssh` (`/home/<Utilisateur>/.ssh` sous Linux). Le fichier `id_ecdsa` contient la clef **privée** et le fichier `id_ecdsa.pub` contient la clef **publique**.


### OS (Système d'expoitation)

Tout d'abord il faut "flasher" un os sur une carte micro SD. Pour cela on utilise un logiciel spécifique (ex: [Raspberry Pi Imager](https://www.raspberrypi.com/software/)).
Cliquer sur `CHOISIR L'OS` puis sur `Raspberry Pi OS (other)` et enfin `Raspberry Pi OS Lite (64-bit)`.
Sélectionner la carte SD en cliquant sur `CHOISIR LE STOCKAGE`.
Afficher les options avancées `Ctrl+Shift+X`.
Compléter les réglages de la page `Général` et modifier les valeurs par défaut si besoin.
Dans la Page `Services` cocher la case `activer SSH` puis `Authentification via clef publique` et renseigner la clef publique.
Cliquer sur `Enregistrer`
Puis Cliquer sur `Suivant`.
Mettre la carte SD dans le Raspberry Pi puis le démarrer.
La connexion se fait en utilisant la commande `ssh utilisateur@hôte` (ex : `ssh pi@192.168.1.3`).
Par défaut, l'identifiant  est `pi` et le mot de passe `raspberry`

### Docker 

Docker est un outil qui permet d'exécuter des applications dans des **conteneurs**, des environnements isolés et légers. Cela garantit que l'application fonctionne de la même manière sur n'importe quel ordinateur, sans conflits avec le système.
 1. Retirer les paquets créant des conflits:
```
for pkg in docker.io docker-doc docker-compose podman-docker containerd runc; do sudo apt-get remove $pkg; done
```
2. Ajouter le dépot apt de Docker
```
# Add Docker's official GPG key:
sudo apt-get update
sudo apt-get install ca-certificates curl
sudo install -m 0755 -d /etc/apt/keyrings
sudo curl -fsSL https://download.docker.com/linux/debian/gpg -o /etc/apt/keyrings/docker.asc
sudo chmod a+r /etc/apt/keyrings/docker.asc

# Add the repository to Apt sources:
echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.asc] https://download.docker.com/linux/debian \
  $(. /etc/os-release && echo "$VERSION_CODENAME") stable" | \
  sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
sudo apt-get update
```
3. Installer Docker
```
sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
```