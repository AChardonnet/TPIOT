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
### OS
Tout d'abord il faut "flasher" un os sur une carte micro SD. Pour cela on utilise un logiciel spécifique (ex: [Raspberry Pi Imager](https://www.raspberrypi.com/software/)).
Cliquer sur `CHOISIR L'OS` puis sur `Raspberry Pi OS (other)` et enfin `Raspberry Pi OS Lite (64-bit)`.
Sélectionner la carte SD en cliquant sur `CHOISIR LE STOCKAGE` 
Puis Cliquer sur `Suivant`.
Mettre la carte SD dans le Raspberry Pi puis le démarrer.
L'identifiant par défaut est `pi` et le mot de passe `raspberry`
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