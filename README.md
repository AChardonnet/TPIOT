# TP IOT
## Introduction
Ce dépot rassemble tous les fichiers nécessaire à la réalisation d'un TP d'IOT.
Le TP suit une architecture Edge Fog Cloud. Le cloud est hébergé sur un ordinateur personnel, le fog sur une machine du type Raspberry Pi et le edge sur une carte esp32.

Pour que tout fonctionne correctement, les 3 machines doivent être connectées au même réseau. Si c'est disponible sur votre ordinateur, il est plus simple de les connecter au partage de connexion de votre ordinateur afin de créer un réseau isolé.
## Installation

 - ### Cloud
	 1. Se placer dans le dossier cloud.
	 2. Dans le fichier `docker-compose.yml`, modifier le service `kafka-broker` modifiez la variable d'environnement `KAFKA_ADVERTISED_LISTENERS` pour que l'ip indiquée corresponde à celle du cloud.
	 3. `docker compose up -d`
	 4. Après quelques instants, la page `http://localhost:9021` devrait afficher une page d'accueil.
 - ### Fog
	 1. Préparer la machine.
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