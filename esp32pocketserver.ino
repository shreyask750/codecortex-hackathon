#include <WiFi.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include <DNSServer.h>

const char* accessPointName = "PocketServer";
const char* accessPointPassword = "123456789"; 

DNSServer dnsServer;
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Chiller Efficiency Calculator</title>
    <style>
        :root {
            /* Base font size */
            font-size: 10px;
            /* Set neon color */
            --neon-text-color: rgb(46, 71, 126);
            --neon-border-color: rgb(8, 24, 39);
        }

        @font-face {
            font-family: title;
            src: url("./GraduateShades.otf");
        }

        @font-face {
            font-family: form;
            src: url("./Roboto-Condensed.ttf");
        }

        /* General styling for the body */
        body {
            font-family: 'Exo 2', sans-serif; /* Updated font */
            margin: 0;
            padding: 0;
            color: #7e60bf;
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            min-height: 100vh;
            background: #7e60bf; /* Added background color */
        }

        /* Navbar styling */
        nav {
            background-color: #433878;
            box-shadow: 10px 10px black;
            top: 0;
            left: 0;
            width: 100%;
            height: 75px;
            padding: 15px 20px;
            z-index: 1000;
            display: flex;
            align-items: center;
            justify-content: space-between; /* Space between elements */
        }

        /* Navbar heading/logo */
        nav h2 {
            color: #ffe1ff;
            margin: 0;
            font-size: 80px;
            font-weight: bold;
            text-align: left;
            font-family: title;
        }

        /* "View Trends" button styling */
        .view-trends-button {
            width: 20%;
            padding: 12px;
            background-color: #5b3878;
            border-radius: 8px;
            color: #ffe1ff;
            box-shadow: 4px 4px black;
            border: none;
            cursor: pointer;
            font-size: 18px;
            font-weight: bold;
            transition: background-color 0.3s ease, color 0.3s ease;
            font-family: form;
            text-align: center;
            text-decoration: none;
        }

        .view-trends-button:hover {
            background-color: #e4b1f0;
            color: #433878;
            box-shadow: none;
        }

        /* Tagline styling */
        .tagline {
            text-align: center;
            color: #433878;
            font-family: form;
            font-size: 40px;
        }

        .tagline h1 {
            color: #BBE1FA;
            font-family: form;
            font-size: 40px;
        }

        /* Container for the form */
        .container {
            padding: 20px;
            border-radius: 5px;
            width: 1000px;
            margin-top: 30px;
            padding-bottom: 40px;
            padding-left: 30px;
            padding-right: 30px;
            background-color: #ffe1ff;
            box-shadow: 10px 10px black;
        }

        /* Form title styling */
        .form-title {
            text-align: center;
            font-size: 46px;
            color: #8323c8;
            margin-bottom: 60px;
            font-family: form;
        }

        /* Styling for the form inputs and labels */
        .input-group {
            display: flex;
            justify-content: space-between;
            margin-bottom: 20px;
            padding-left: 20px;
            padding-right: 20px;
            font-family: form;
        }

        /* Inner input box styling */
        .input-box {
            width: 48%;
            margin-bottom: 40px;
            text-align: center; /* Center the text in the input box */
        }

        .input-box label {
            font-size: 20px;
            padding-bottom: 10px;
            font-family: form;
        }

        .input-box::placeholder {
            padding-left: 20px;
            font-family: form;
        }

        /* Updated input styling */
        form input[type="number"] {
            width: 100%;
            padding: 10px;
            border: 2px solid #8532b8;
            border-radius: 5px;
            background-color: #c8a7e5;
            color: #5a009e;
            font-size: 16px;
            box-sizing: border-box;
            font-family: form;
            box-shadow: 5px 5px #433878;
        }

        form select {
            width: 100%;
            padding: 10px;
            border: 2px solid #8532b8;
            border-radius: 5px;
            background-color: #c8a7e5;
            color: #5a009e;
            font-size: 16px;
            box-shadow: 5px 5px #433878;
            font-family: form;
        }

        .submitButton {
            display: flex;
            justify-content: center;
            margin-top: 20px;
        }

        input[type="submit"] {
            width: 20%;
            padding: 12px;
            background-color: #5b3878;
            border-radius: 8px;
            color: #ffe1ff;
            box-shadow: 4px 4px black;
            border: none;
            cursor: pointer;
            font-size: 18px;
            font-weight: bold;
            transition: background-color 0.3s ease, color 0.3s ease;
            font-family: form;
        }

        input[type="submit"]:hover {
            background-color: #e4b1f0;
            color: #433878;
            box-shadow: none;
        }

        /* Animations */
        @keyframes slideIn {
            0% {
                transform: translateY(100px);
                opacity: 0;
            }
            100% {
                transform: translateY(0);
                opacity: 1;
            }
        }

        @keyframes fadeIn {
            from {
                opacity: 0;
            }
            to {
                opacity: 1;
            }
        }

        /* Gradient animation keyframes */
        @-webkit-keyframes AnimationName {
            0% {
                background-position: 91% 0%;
            }
            50% {
                background-position: 10% 100%;
            }
            100% {
                background-position: 91% 0%;
            }
        }

        @-moz-keyframes AnimationName {
            0% {
                background-position: 91% 0%;
            }
            50% {
                background-position: 10% 100%;
            }
            100% {
                background-position: 91% 0%;
            }
        }

        @keyframes AnimationName {
            0% {
                background-position: 91% 0%;
            }
            50% {
                background-position: 10% 100%;
            }
            100% {
                background-position: 91% 0%;
            }
        }
    </style>
</head>
<body class="css-selector">
    <!-- Navigation bar -->
    <nav>
        <h2>Cool Solutions</h2>
        <button class="view-trends-button" onclick="window.location.href='trends.html'">View Trends</button>
    </nav>

    <!-- Form section -->
    <div class="container">
        <h2 class="form-title">Chiller Load Estimator</h2>
        <div class="tagline">
            <h3>Comfort Cooling!</h3>
        </div>
        <form action="/predict" method="POST" id="aiform">
            <div class="input-group">
                <div class="input-box">
                    <label for="ambientTemperature">Ambient Temperature (°C):</label>
                    <input type="number" id="ambientTemperature" name="ambientTemperature" placeholder="Enter ambient temperature" step="0.1" required>
                </div>

                <div class="input-box">
                    <label for="relativeHumidity">Relative Humidity (%):</label>
                    <input type="number" id="relativeHumidity" name="relativeHumidity" placeholder="Enter relative humidity" step="0.1" required>
                </div>
            </div>

            <div class="input-group">
                <div class="input-box">
                    <label for="wetBulbTemperature">Wet Bulb Temperature (°C):</label>
                    <input type="number" id="wetBulbTemperature" name="wetBulbTemperature" placeholder="Enter wet bulb temperature" step="0.1" required>
                </div>

                <div class="input-box">
                    <label for="chilledWaterSupplyTemperature">Chilled Water Supply Temperature (°C):</label>
                    <input type="number" id="chilledWaterSupplyTemperature" name="chilledWaterSupplyTemperature" placeholder="Enter chilled water supply temperature" step="0.1" required>
                </div>
            </div>

            <div class="input-group">
                <div class="input-box">
                    <label for="chilledWaterReturnTemperature">Chilled Water Return Temperature (°C):</label>
                    <input type="number" id="chilledWaterReturnTemperature" name="chilledWaterReturnTemperature" placeholder="Enter chilled water return temperature" step="0.1" required>
                </div>

                <div class="input-box">
                    <label for="flowRate">Flow Rate (L/s):</label>
                    <input type="number" id="flowRate" name="flowRate" placeholder="Enter flow rate" step="0.1" required>
                </div>
            </div>

            <div class="input-group">
                <div class="input-box">
                    <label for="plantEfficiency">Plant Efficiency (%):</label>
                    <input type="number" id="plantEfficiency" name="plantEfficiency" placeholder="Enter plant efficiency" step="0.1" required>
                </div>

                <div class="input-box">
                    <label for="totalChillerPower">Total Chiller Power (kW):</label>
                    <input type="number" id="totalChillerPower" name="totalChillerPower" placeholder="Enter total chiller power" step="0.1" required>
                </div>
            </div>

            <div class="input-group">
                <div class="input-box" style="text-align: center;">
                    <label for="isWeekend">Is Weekend:</label>
                    <select id="isWeekend" name="isWeekend" required>
                        <option value="" disabled selected>Select Weekend/Weekday</option>
                        <option value="0">Weekday</option>
                        <option value="1">Weekend</option>
                    </select>
                </div>
            </div>

            <div class="submitButton">
                <input type="submit" value="Predict">
            </div>
        </form>
    </div>
</body>
</html>
)rawliteral";

class CaptivePortalHandler : public AsyncWebHandler {
public:
  CaptivePortalHandler() {}
  virtual ~CaptivePortalHandler() {}

  bool canHandle(AsyncWebServerRequest *request){
    return request->url() == "/";
  }

  void handleRequest(AsyncWebServerRequest *request) {
    if (request->method() == HTTP_GET && request->url() == "/") {
      request->send(200, "text/html", index_html);
    } else {
      request->send(200, "text/html", index_html);
    }
  }
};

void setupServer(){
  server.addHandler(new CaptivePortalHandler()).setFilter(ON_AP_FILTER);

  server.onNotFound([&](AsyncWebServerRequest *request){
    request->send(200, "text/html", index_html); 
  });
}

void setup(){
  Serial.begin(115200);
  delay(10);

  WiFi.mode(WIFI_AP); 
  WiFi.softAP(accessPointName, accessPointPassword); // Set AP with SSID and password

  dnsServer.start(53, "*", WiFi.softAPIP());
  setupServer();

  server.begin();
  Serial.println("Started Captive Portal...");
}

void loop(){
  dnsServer.processNextRequest();
}
