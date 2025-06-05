const char html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>ESP Pushbutton Web Server</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 20px;
            text-align: center;
        }
        .button-container {
            margin: 20px 0;
        }
        .algorithm-button {
            padding: 10px 30px;
            font-size: 16px;
            margin: 5px;
            cursor: pointer;
            background-color: #34495e;
            color: white;
            border: none;
            border-radius: 5px;
        }
        .algorithm-button:hover {
            background-color: #2c3e50;
        }
        .led-container {
            margin: 20px;
            display: flex;
            justify-content: center;
            gap: 30px;
        }
        .led {
            width: 80px;
            height: 80px;
            border: 2px solid #000;
        }
        #led1 { background-color: #ff0000; }
        #led2 { background-color: #ffa500; }
        #led3 { background-color: #ffffff; border: 2px solid #008000; }
        .controller-select {
            margin: 20px 0;
        }
    </style>
</head>
<body>
    <h1>ESP Pushbutton Web Server</h1>
    
    <div class="controller-select">
        <label>Control: </label>
        <select id="targetController">
            <option value="local">This Controller</option>
            <option value="remote">Remote Controller</option>
        </select>
    </div>

    <div class="button-container">
        <button class="algorithm-button" onclick="sendCommand('algorithm1')">Algorithm 1</button>
        <button class="algorithm-button" onclick="sendCommand('algorithm2')">Algorithm 2</button>
    </div>

    <div class="led-container">
        <div class="led" id="led1"></div>
        <div class="led" id="led2"></div>
        <div class="led" id="led3"></div>
    </div>

    <script>
        function sendCommand(algorithm) {
            const target = document.getElementById('targetController').value;
            const url = target === 'local' ? '/change' : '/remote';
            
            fetch(url)
                .then(response => response.text())
                .then(data => {
                    console.log('Server response:', data);
                    animateLocalLEDs();
                })
                .catch(error => {
                    console.error('Error:', error);
                });
        }

        function animateLocalLEDs() {
            const leds = [
                document.getElementById('led1'),
                document.getElementById('led2'),
                document.getElementById('led3')
            ];
            
            let currentLed = 0;
            const interval = setInterval(() => {
                leds.forEach(led => led.style.opacity = '0.3');
                leds[currentLed].style.opacity = '1';
                currentLed = (currentLed + 1) % 3;
            }, 500);

            setTimeout(() => {
                clearInterval(interval);
                leds.forEach(led => led.style.opacity = '1');
            }, 5000);
        }
    </script>
</body>
</html>
)rawliteral";