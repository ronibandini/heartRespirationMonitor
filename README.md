# ❤️ Heart & Respiration Monitor

**Contactless heart-rate and respiration monitoring using a 60 GHz mmWave sensor, ESP32-C6, and UNIHIKER.**

This project uses a **DFRobot C1001 mmWave Human Detection Sensor** to obtain heart-rate and respiration data without attaching electrodes or wearable sensors to the subject.

A **Beetle ESP32-C6** communicates with the radar sensor over UART and exposes the latest measurements through a lightweight HTTP endpoint. A **UNIHIKER** with Carrier Board retrieves the measurements over Wi-Fi, displays them on screen, and uses RGB LEDs to indicate the current heart-rate status.

> ⚠️ **This is an experimental maker project, not a medical device.**
> It is not intended for diagnosis, treatment, clinical monitoring, or emergency use.

---

## ✨ Features

* ❤️ Contactless heart-rate monitoring
* 🫁 Contactless respiration-rate monitoring
* 📡 60 GHz mmWave sensing
* 🌐 ESP32-C6 HTTP data endpoint
* 🖥️ Real-time UNIHIKER display
* 🚦 Configurable RGB heart-rate alert
* 🔵 Warm-up status indication
* 🟢 Normal measurement indication
* 🔴 High heart-rate indication
* 🐍 Python-based UNIHIKER client
* 🔧 Simple UART wiring
* 📦 Small, hackable codebase

---

## 🧠 How it works

```text
┌──────────────────────────┐
│ C1001 60 GHz mmWave      │
│ Human Detection Sensor   │
└────────────┬─────────────┘
             │ UART
             ▼
┌──────────────────────────┐
│ DFRobot Beetle ESP32-C6  │
│                          │
│ • Reads heart rate       │
│ • Reads respiration      │
│ • Runs HTTP server       │
└────────────┬─────────────┘
             │
             │ Wi-Fi / HTTP
             ▼
┌──────────────────────────┐
│ UNIHIKER + Carrier Board │
│                          │
│ • Displays measurements │
│ • Controls RGB LEDs      │
└──────────────────────────┘
```

The ESP32-C6 configures the C1001 sensor in its sleep-detection mode, where heart-rate and respiration information is available.

The current readings are returned by the ESP32 as a simple comma-separated HTTP response:

```text
70,14
```

where:

```text
70 = heart rate
14 = respiration rate
```

The UNIHIKER client polls this endpoint, parses both values, updates the display, and changes the RGB indicator according to the configured heart-rate threshold.

---

## 🧰 Hardware

| Component                                       | Purpose                                   |
| ----------------------------------------------- | ----------------------------------------- |
| **DFRobot C1001 mmWave Human Detection Sensor** | Contactless heart and respiration sensing |
| **DFRobot Beetle ESP32-C6**                     | Sensor interface and Wi-Fi HTTP server    |
| **DFRobot UNIHIKER**                            | Display and Python client                 |
| **UNIHIKER Carrier Board**                      | RGB status indication and expansion       |
| Jumper wires                                    | UART and power connections                |

---

## 🔌 Wiring

Connect the C1001 sensor to the Beetle ESP32-C6:

| C1001 | Beetle ESP32-C6          |
| ----- | ------------------------ |
| VIN   | VIN / appropriate supply |
| GND   | GND                      |
| TX    | GPIO 4                   |
| RX    | GPIO 5                   |

The firmware initializes the serial connection at:

```cpp
Serial1.begin(115200, SERIAL_8N1, 4, 5);
```

---

## 📁 Repository structure

```text
heartRespirationMonitor/
├── images/
│   └── background.png
├── clientUpload.py
├── heartRateUpload.ino
├── LICENSE
└── README.md
```

### `heartRateUpload.ino`

Firmware for the Beetle ESP32-C6.

It:

* initializes the C1001 sensor
* configures the sensor operating mode
* connects to Wi-Fi
* reads heart-rate and respiration data
* runs an HTTP server on port `80`
* returns the latest values to connected clients

### `clientUpload.py`

Python client for the UNIHIKER.

It:

* connects to the ESP32 HTTP endpoint
* reads heart-rate and respiration values
* renders the interface
* controls the Carrier Board RGB LEDs
* checks the configured heart-rate threshold

---

## 🚀 Installation

### 1. Clone the repository

```bash
git clone https://github.com/ronibandini/heartRespirationMonitor.git
cd heartRespirationMonitor
```

### 2. Install the C1001 Arduino library

Install the **DFRobot Human Detection** Arduino library.

Repository:

```text
cdjq/DFRobot_HumanDetection
```

Make sure this library is available to the Arduino IDE before compiling the ESP32 firmware.

---

## 📡 Configure the ESP32-C6

Open:

```text
heartRateUpload.ino
```

Configure your Wi-Fi network:

```cpp
const char *ssid = "YOUR_WIFI_SSID";
const char *password = "YOUR_WIFI_PASSWORD";
```

Compile and upload the sketch to the Beetle ESP32-C6.

Open the Serial Monitor at:

```text
115200 baud
```

After connecting to Wi-Fi, the ESP32 prints its assigned IP address.

Example:

```text
WiFi connected.
IP address:
192.168.1.51
```

Keep this address for the next step.

> ⏳ The C1001 may require several minutes after startup before valid measurements become available.

---

## 🖥️ Configure the UNIHIKER

Open:

```text
clientUpload.py
```

Set the IP address assigned to the ESP32:

```python
url = "http://192.168.1.51"
```

Both devices must be reachable through the same network.

Copy these items to the UNIHIKER:

```text
clientUpload.py
images/
```

Then run:

```bash
python clientUpload.py
```

---

## 🚦 Heart-rate alert

The default alert threshold is configured in `clientUpload.py`:

```python
heartRateLimit = 90
```

Change this value to suit your experiment.

The Carrier Board RGB LEDs indicate the state:

| Color    | State                                                    |
| -------- | -------------------------------------------------------- |
| 🔵 Blue  | Sensor warming up / no heart-rate reading yet            |
| 🟢 Green | Heart-rate data available and below the configured limit |
| 🔴 Red   | Heart rate above `heartRateLimit`                        |

The threshold is a user-defined visualization setting and **must not be interpreted as a medical alarm threshold**.

---

## 🌐 HTTP interface

The ESP32 runs a minimal HTTP server on port `80`.

A request to the ESP32 IP returns:

```text
heartRate,respirationRate
```

Example:

```text
72,16
```

This intentionally simple interface makes it easy to replace the UNIHIKER client with another application, dashboard, logger, Raspberry Pi, computer, or IoT system.

---

## 🔬 Ideas for extending the project

Possible improvements include:

* 📊 real-time charts
* 💾 local measurement logging
* ☁️ MQTT or cloud integration
* 📱 mobile dashboards
* 🔎 automatic ESP32 discovery using mDNS
* 🧾 JSON instead of comma-separated HTTP responses
* 🔁 automatic reconnect and network error handling
* ⚙️ on-screen threshold configuration
* 🛌 additional C1001 sleep and movement information
* 📈 historical heart-rate and respiration visualization

---

## 📰 External references

This project has also appeared outside GitHub.

### DFRobot Maker Community

**Heart and respiration monitor with C1001 mmWave Sensor and Unihiker Carrier Board**

A complete project article covering the hardware, wiring, Wi-Fi configuration, sensor library, UNIHIKER setup, and source repository.

The article also includes project photographs and a Maker Culture presentation featuring the UNIHIKER implementation.

https://community.dfrobot.com/makelog-314494.html 

### Medium

**Monitor de ritmo cardíaco y respiración con microondas de 60GHZ**

Spanish-language article by Roni Bandini about the project and the use of 60 GHz microwave radar for contactless physiological monitoring.

https://bandini.medium.com/monitor-de-ritmo-card%C3%ADaco-y-respiraci%C3%B3n-con-microondas-de-60ghz-ccdf08b9e68c?utm_source=chatgpt.com

### Ecosyste.ms Awesome

The repository is indexed by the Ecosyste.ms open-source project directory under both:

```text
esp32-c6
mmwave
```
https://awesome.ecosyste.ms/projects?keyword=mmwave&utm_source=chatgpt.com

---

## 🔗 You may also be interested in...

Other related projects by **Roni Bandini**:

### ❤️ `hr-hrv`

**ECG stress detection with Machine Learning HR/HRV**

A wearable physiological sensing project using an Arduino Portenta, ECG acquisition, heart-rate variability, and Edge Impulse machine learning.

```text
github.com/ronibandini/hr-hrv
```

---

### 📡 `mmWaveAlarm`

**Human presence detection through doors and walls using mmWave radar**

An ESP32-based project using a DFRobot mmWave sensor to detect human presence and send Telegram notifications.

```text
github.com/ronibandini/mmWaveAlarm
```

---

### 🤥 Arduino Polygraph

A DIY polygraph combining heart-rate sensing and galvanic skin response with an Arduino and analog VU meters.

The project explores another way of turning physiological measurements into a physical interactive device.

---

## 🔒 Security note

The current implementation uses a basic unauthenticated HTTP server.

Measurements are transmitted over plain HTTP and the device does not implement authentication or TLS. For experiments involving private networks or sensitive information, consider adding appropriate access control and transport security.

---

## 📜 License

This project is released under the **MIT License**.

See [`LICENSE`](LICENSE) for details.

---

## 👤 Author

**Roni Bandini**

Maker, developer, electronic artist and writer.

GitHub:

```text
@ronibandini
```

Instagram:

```text
@ronibandini
```

Contributions, forks, experiments, and adaptations are welcome.

