Commande pour se connecter à la base de données
```
psql -h localhost -p 5432 -U user iot
```

Commande pour aligner le compteur des id avec les id déjà enregistrées
```
SELECT setval('humidity_id_seq', (SELECT MAX(id) FROM humidity) + 1);
SELECT setval('temperature_id_seq', (SELECT MAX(id) FROM temperature) + 1);
```

Commande pour réinitialiser la base de données 
```
DELETE FROM humidity;
DELETE FROM temperature;

SELECT setval('humidity_id_seq', 1, false);
SELECT setval('temperature_id_seq', 1, false);

```