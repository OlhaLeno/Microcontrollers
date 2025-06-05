const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial; text-align: center; margin:0px auto; padding-top: 30px;}
    .button {
      padding: 10px 20px;
      font-size: 24px;
      text-align: center;
      outline: none;
      color: #fff;
      background-color: #2f4468;
      border: none;
      border-radius: 5px;
      box-shadow: 0 6px #999;
      cursor: pointer;
    }
    .button:hover {background-color: #1f2e45}
    .button:active {
      background-color: #1f2e45;
      box-shadow: 0 4px #666;
      transform: translateY(2px);
    }
  </style>
</head>
<body>
  <h1>ESP8266 LED Control</h1>
  <button class="button" onclick="changeSpeed();">Increase Speed</button>
  <script>
    function changeSpeed() {
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/increase", true);
      xhr.send();
    }
  </script>
</body>
</html>
)rawliteral";

void handleButtonPressWeb(ESP8266WebServer& server) {
    interval = (interval <= MIN_INTERVAL) ? DEFAULT_INTERVAL : interval - DECREASE_INTERVAL;
    server.send(200, "text/plain", String("Interval changed to ") + String(interval));
}
