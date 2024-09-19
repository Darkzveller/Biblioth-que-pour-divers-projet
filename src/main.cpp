#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// Informations du point d'accès
const char* ssid = "Test wifi";
const char* password = "123456789";

// Déclaration du serveur web sur le port 80
AsyncWebServer server(80);

// Pin utilisé pour une LED (ou autre composant)
const int ledPin = 2; // Pin GPIO 2 pour la LED
bool isLedOn = false; // Variable pour suivre l'état de la LED

// Variables pour l'état des boutons
bool asserActif = false;
bool powerOn = false;

// Variable prédéfinie pour la tension de batterie
float batteryVoltage = 6.5;  // Exemple de valeur de tension, modifiable

void setup() {
  // Initialisation de la communication série
  Serial.begin(115200);

  // Configurer le GPIO 2 en mode sortie pour la LED
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);  // Éteindre la LED au démarrage

  // Initialisation du point d'accès Wi-Fi
  WiFi.softAP(ssid, password);

  // Afficher l'adresse IP du serveur dans le moniteur série
  Serial.println();
  Serial.println("Point d'accès Wi-Fi démarré");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("Mot de passe: ");
  Serial.println(password);

  // Attendre un court instant pour obtenir l'adresse IP
  delay(1000);

  // Afficher l'adresse IP dans le moniteur série
  Serial.print("Adresse IP du point d'accès : ");
  Serial.println(WiFi.softAPIP());

  // Page web simplifiée avec seulement l'affichage de la tension et les boutons
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = R"rawliteral(
    <!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>IHM Gyropode</title>
    <style>
        body {
            display: flex;
            flex-direction: column;
            align-items: center;
            height: 100vh;
            margin: 0;
            background-color: #f0f0f0;
            font-family: Arial, sans-serif;
        }
        .title {
            margin-top: 20px;
            font-size: 24px;
            font-weight: bold;
            color: #333;
            text-align: center;
        }
        .battery {
            margin-top: 20px;
            background-color: #fff;
            border: 2px solid #ddd;
            padding: 10px;
            border-radius: 8px;
            transition: border-color 0.3s ease;
        }
        .battery-text {
            font-size: 18px;
            font-weight: bold;
        }
        .controls {
            margin-top: 30px;
            display: flex;
            gap: 10px; /* Espacement entre les boutons */
        }
        button {
            padding: 10px 20px;
            font-size: 16px;
            cursor: pointer;
        }
    </style>
</head>
<body>
    <div class="title">IHM Gyropode</div>
    <div class="battery" id="batteryDisplay">
        <div class="battery-text">Tension: <span id="batteryVoltage">0.0V</span></div>
    </div>
    <div class="controls">
        <button id="ledButton">Asser Actif</button>
        <button id="powerButton">Power On</button>
    </div>
    <script>
        // Fonction pour changer la couleur de la bordure en fonction de la tension
        function updateBatteryDisplay(voltage) {
            const batteryDisplay = document.getElementById('batteryDisplay');
            if (voltage > 9) {
                batteryDisplay.style.borderColor = 'green';
            } else if (voltage >= 7 && voltage <= 9) {
                batteryDisplay.style.borderColor = 'orange';
            } else {
                batteryDisplay.style.borderColor = 'red';
            }
        }

        // Charger la valeur de la tension envoyée par le serveur
        fetch('/get-battery').then(response => response.text()).then(data => {
            const voltage = parseFloat(data);
            document.getElementById('batteryVoltage').innerText = voltage + 'V';
            updateBatteryDisplay(voltage);  // Mettre à jour la couleur en fonction de la tension
        });

        // Fonction pour mettre à jour l'état des boutons
        function updateButtonStates() {
            // Charger l'état du bouton "Asser"
            fetch('/get-asser').then(response => response.text()).then(data => {
                const asserButton = document.getElementById('ledButton');
                if (data === 'true') {
                    asserButton.innerText = 'Asser Inactif';
                } else {
                    asserButton.innerText = 'Asser Actif';
                }
            });

            // Charger l'état du bouton "Power"
            fetch('/get-power').then(response => response.text()).then(data => {
                const powerButton = document.getElementById('powerButton');
                if (data === 'true') {
                    powerButton.innerText = 'Power Off';
                } else {
                    powerButton.innerText = 'Power On';
                }
            });
        }

        // Charger l'état des boutons au démarrage
        updateButtonStates();

        // Événements pour les boutons
        document.getElementById('ledButton').addEventListener('click', () => {
            fetch('/toggle-led').then(() => {
                updateButtonStates(); // Mettre à jour les boutons après le clic
            });
        });

        document.getElementById('powerButton').addEventListener('click', () => {
            fetch('/toggle-power').then(() => {
                updateButtonStates(); // Mettre à jour les boutons après le clic
            });
        });
    </script>
</body>
</html>
    )rawliteral";
    request->send(200, "text/html", html);
  });

  // API pour récupérer la tension de batterie
  server.on("/get-battery", HTTP_GET, [](AsyncWebServerRequest *request){
    String voltage = String(batteryVoltage, 2);  // Convertir la tension en chaîne avec 2 décimales
    request->send(200, "text/plain", voltage);   // Envoyer la valeur de la tension
  });

  // API pour récupérer l'état du bouton "Asser"
  server.on("/get-asser", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", asserActif ? "true" : "false");
  });

  // API pour récupérer l'état du bouton "Power"
  server.on("/get-power", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", powerOn ? "true" : "false");
  });

  // Activer/désactiver la LED lorsque l'utilisateur clique sur le bouton
  server.on("/toggle-led", HTTP_GET, [](AsyncWebServerRequest *request){
    asserActif = !asserActif; // Inverser l'état du bouton "Asser"
    int ledState = digitalRead(ledPin);
    digitalWrite(ledPin, !ledState); // Inverser l'état de la LED
    request->send(200, "text/plain", "LED toggled");
  });

  // Activer/désactiver la puissance lorsque l'utilisateur clique sur le bouton
  server.on("/toggle-power", HTTP_GET, [](AsyncWebServerRequest *request){
    powerOn = !powerOn; // Inverser l'état du bouton "Power"
    request->send(200, "text/plain", "Power toggled");
  });

  // Démarrage du serveur
  server.begin();
}

void loop() {
  // Rien à faire ici, tout est géré par les événements du serveur web
}
