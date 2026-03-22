# ESP32 Smart Solar Controller — MPPT Firmware + Web Dashboard

> **Turn your cheap MPPT into a smart solar controller.**

An ESP32-based add-on module that plugs into any 12V MPPT charge controller and gives it a brain — real-time web dashboard, weather-adaptive cutoff, coulomb counting, remote access, and deep sleep.

**Actually up & running** — built and battle-tested on a real 2-panel home installation.

---

![Hardware](docs/hardware.jpg)
*ESP32 module in transparent enclosure, sitting on top of the MPPT charge controller*

![Dashboard](docs/dashboard.jpg)
*Live web dashboard — accessible locally or from anywhere via DuckDNS + free VPS*

---

## ✨ Features

- **Automatic load control** — cuts and resumes based on battery voltage + sunlight
- **Weather-adaptive thresholds** — OpenWeatherMap + NASA POWER data dynamically adjust cutoff voltage and lux threshold
- **SOC tracking** — coulomb counting via INA226 (Ah in / Ah out)
- **Deep sleep** — periodic night wake-ups for battery checks (~10µA in sleep)
- **Full web dashboard** — live gauges, 24h charts, 30-day log, 7-day forecast, night voltage history
- **Remote access** — accessible from anywhere via DuckDNS + free Oracle Cloud VPS (optional)
- **Anti-bootloop protection** — detects overload reboot loops, locks MOSFET OFF automatically
- **15 languages** — EN, FR, DE, ES, IT, PT, PL, CZ, ZH, RU, JA, KO, NL, TR, SV

---

## 🔧 Hardware

Built from off-the-shelf parts — ESP32, INA226, VEML7700 and a MOSFET.
**All components under €5 each on AliExpress. Total build cost under €15.**

Full BOM with sourcing links, wiring diagrams and photos are included in the documentation package.

---

## 🌍 15 Languages

Switch language with a single `#include` line in the sketch — no recompilation of logic needed.

EN · FR · DE · ES · IT · PT · PL · CZ · ZH · RU · JA · KO · NL · TR · SV

---

## 📦 Full package — €9

👉 **[akgaze5.gumroad.com/l/wjevp](https://akgaze5.gumroad.com/l/wjevp)**

Includes:
- ✅ Complete Arduino `.ino` firmware (this repo)
- 📄 Illustrated PDF documentation (15 pages, FR/EN)
- 🛒 Full BOM with AliExpress sourcing links
- 🔌 Wiring diagrams with photos
- ☁️ Remote access setup guide (DuckDNS + Oracle Cloud VPS + nginx)

---

## ⚖️ License

**Proprietary Non-Commercial License** — see [LICENSE.txt](LICENSE.txt)

Personal and educational use permitted. Redistribution and commercial use strictly prohibited.

© 2025 F.B — fraise_prieres9v@icloud.com
