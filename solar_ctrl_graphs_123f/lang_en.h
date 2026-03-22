// =============================================================================
// Solar Controller — Language EN (English)
// To switch language: replace #include "lang_fr.h" with #include "lang_en.h"
// =============================================================================
#pragma once

// --- SVG: chart titles ---
#define L_SVG_PRESENT        "PRESENT \xe2\x80\x94 last 24 hours"
#define L_SVG_NOCTURNE       "NIGHT STATUS \xe2\x80\x94 battery voltage"
#define L_SVG_FUTUR          "FORECAST \xe2\x80\x94 next 7 days (lux)"
#define L_SVG_PASSE          "HISTORY \xe2\x80\x94 last 30 days"
#define L_SVG_ENERGIE        "ENERGY \xe2\x80\x94 Current &amp; Power 24h"
#define L_SVG_BILAN          "DAILY LOG 30d \xe2\x80\x94 Ah in / out"

// --- SVG: legends ---
#define L_SVG_TENSION        "Voltage (V)"
#define L_SVG_LUMINOSITE     "Light (lux)"
#define L_SVG_TENSION_NUIT   "Night voltage (V)"
#define L_SVG_COURANT        "Current (A)"
#define L_SVG_PUISSANCE      "Power (W)"
#define L_SVG_ENTRANTS       "In (Ah)"
#define L_SVG_SORTANTS       "Out (Ah)"
#define L_SVG_COUPURE        "cutoff %.2fV"
#define L_SVG_REPRISE        "resume"
#define L_SVG_SEUIL_ON       "ON threshold"
#define L_SVG_ATTENTE        "Waiting for first measurements..."
#define L_SVG_MAJ            "(updated every hour)"
#define L_SVG_PAS_DONNEES    "No data yet"
#define L_SVG_BARRE_VISIBLE  "(today's bar visible in %d min)"

// --- Main page header ---
#define L_HDR_CHARGE_ON      "LOAD : ON"
#define L_HDR_CHARGE_OFF     "LOAD : OFF"
#define L_HDR_LUX            "Lux: "
#define L_HDR_SEUIL_ON       "ON threshold: "
#define L_HDR_COUPURE        "Cutoff: "
#define L_HDR_HORS_MPPT      "&#9888; No MPPT &mdash; GND not common &mdash; readings invalid"
#define L_HDR_TOAST          "☀️ Forecast updated for today"

// --- Main page sections ---
#define L_SEC_PRESENT        "PRESENT"
#define L_SEC_NOCTURNE       "NIGHT STATUS"
#define L_SEC_ENERGIE        "ENERGY 24H"
#define L_SEC_BILAN          "DAILY LOG 30D"
#define L_SEC_FUTUR          "FORECAST"
#define L_SEC_PASSE          "HISTORY"

// --- Hamburger menu ---
#define L_MENU_ACCUEIL       "&#8962; Home"
#define L_MENU_CONFIG        "&#9881; Settings"
#define L_MENU_DEBUG         "&#128196; Debug"
#define L_MENU_DEBUG_BILAN   "&#128202; Debug Log"
#define L_MENU_DOC           "&#9432; Documentation"

// --- Common buttons ---
#define L_BTN_RETOUR         "&#8592; Back"
#define L_BTN_ENREGISTRER    "💾 Save"
#define L_BTN_ANNULER        "&#8635; Undo last"
#define L_BTN_RESET          "&#128465; Reset day 0"
#define L_BTN_DEBLOQUER      "&#128275; Unblock MOSFET"

// --- Settings page: titles and tabs ---
#define L_CFG_TITRE          "&#9881; Settings"
#define L_TAB_GENERAL        "General"
#define L_TAB_LUMINOSITE     "Light"
#define L_TAB_BATTERIE       "Battery"
#define L_CFG_SAUVEGARDEE    "✓ Settings saved. Returning..."

// --- Settings > General ---
#define L_CFG_OWM_KEY        "OpenWeatherMap API key"
#define L_CFG_RESEAU         "&#127760; Network — static IP (restart required)"
#define L_CFG_IP             "ESP32 IP"
#define L_CFG_GW             "Gateway"
#define L_CFG_MASQUE         "Subnet mask"
#define L_CFG_DNS            "DNS"
#define L_CFG_LAT            "Latitude"
#define L_CFG_LON            "Longitude"
#define L_CFG_TILT           "Panel tilt"
#define L_CFG_TILT_UNIT      "° (0=flat, 90=vertical)"
#define L_CFG_AZIMUTH        "Panel azimuth"
#define L_CFG_AZIMUTH_UNIT   "° (180=South, 270=West)"
#define L_CFG_RATIO          "Output Voltage (load)"
#define L_CFG_RATIO_HINT     "150k&#937; + 33k&#937; → (150+33)/33 = 5.5454"
#define L_CFG_SLEEP          "Sleep time"
#define L_CFG_SLEEP_UNIT     "min (default 19:45)"
#define L_CFG_REVEIL         "Wake time"
#define L_CFG_REVEIL_UNIT    "min (default 09:15)"
#define L_CFG_RELEVES        "Night measurements"
#define L_CFG_RELEVES_HINT   "Sleep window split into equal slots. Ex: 19:45→09:15 with 2 readings = 3 slots of ~4h26"
#define L_CFG_ARCHIVAGE      "Daily log archive before sleep"
#define L_CFG_ARCHIVAGE_UNIT "h before sleep (0=at sleep time, 1-3h before)"
#define L_CFG_ARCHIVAGE_HINT "Triggers daily archive N hours before sleep. Recommended: 1h"

// --- Settings > Light ---
#define L_CFG_LUX_MAX        "Max lux threshold (low battery + bad weather)"
#define L_CFG_LUX_MAX_HINT   "ON threshold when conditions are unfavourable"
#define L_CFG_LUX_AMP        "Dynamic range"
#define L_CFG_LUX_AMP_HINT   "Min ON threshold = Default - Range (ideal conditions)"
#define L_CFG_LUX_CONF       "100% weather confidence lux"
#define L_CFG_LUX_CONF_HINT  "Above this forecast value, weather data is trusted 100%"
#define L_CFG_LUX_ACTUEL     "Current ON threshold: "
#define L_CFG_LUX_PLAGE      "Range: "

// --- Settings > Battery ---
#define L_CFG_V_PLEINE       "Full charge voltage"
#define L_CFG_V_REPRISE      "Resume charge voltage"
#define L_CFG_V_REPRISE_HINT "Threshold above which charging may resume"
#define L_CFG_V_COUPURE      "Cutoff voltage (fair weather)"
#define L_CFG_V_COUPURE_HINT "Static threshold used when bad weather is forecast"
#define L_CFG_V_CRITIQUE     "Critical voltage"
#define L_CFG_V_PLANCHER     "Absolute floor (never below)"
#define L_CFG_V_PLANCHER_HINT "Cutoff floor in best weather — min 11.9V recommended"
#define L_CFG_CAPACITE       "Battery capacity (INA226 coulomb counting)"
#define L_CFG_CAPACITE_HINT  "Nominal capacity — e.g. 11.5Ah for a 12V/11.5Ah battery"
#define L_CFG_ANTIREBOOT     "Overload reboot delay"
#define L_CFG_ANTIREBOOT_HINT "Boot loop window (default 60s) — if 2 reboots within delay with high load, MOSFET OFF 1h"
#define L_CFG_CADENCE        "Measurement / save interval"
#define L_CFG_CADENCE_HINT   "Sensor polling, Ah counting and NVS save interval. Default: 10 min"
#define L_CFG_BLOCAGE        "MOSFET lockout duration on overload"
#define L_CFG_COUPURE_DYN    "Current dynamic cutoff: "

// --- Alert badges ---
#define L_BADGE_BLOCAGE      "&#9888; Anti-bootloop"
#define L_BADGE_HORS_MPPT    "&#9888; No MPPT"

// --- Debug Log page ---
#define L_DBG_BILAN_TITRE    "&#128202; Debug Daily Log 30d"
#define L_DBG_BILAN_AH_IN    "<b>ahIn:</b>"
#define L_DBG_BILAN_AH_OUT   "<b>ahOut:</b>"

// --- Debug logs page ---
#define L_DBG_TITRE          "&#11035; Debug &#8212; system logs"
#define L_DBG_REFRESH        "(refresh 60s)"

// --- Documentation page ---
#define L_DOC_TITRE          "&#9432; Documentation — Solar Controller"
#define L_DOC_MATERIEL       "&#128268; Hardware"
#define L_DOC_CAPTEURS       "Sensors"
#define L_DOC_SORTIES        "Outputs"
#define L_DOC_BADGES         "&#127968; Header badges"
#define L_DOC_GRAPHIQUES     "&#128202; Charts"
#define L_DOC_LOGIQUE        "&#9889; Charge logic"
#define L_DOC_PROTECTION     "&#128737; Anti-bootloop overload protection"
#define L_DOC_SOC            "&#128267; SOC — Battery state of charge"

// --- Form validation (JS) ---
#define L_VAL_HEURE_SLEEP    "Invalid sleep hour (0-23)"
#define L_VAL_MIN_SLEEP      "Invalid sleep minutes (0-59)"
#define L_VAL_HEURE_REVEIL   "Invalid wake hour (0-23)"
#define L_VAL_MIN_REVEIL     "Invalid wake minutes (0-59)"
#define L_VAL_PLAGE_COURTE   "Sleep window too short (min 2h)"

// --- Reset reasons (debug page) ---
#define L_RST_POWERON        "Power-on"
#define L_RST_EXT            "External reset (EN)"
#define L_RST_SW             "Software reboot (ESP.restart)"
#define L_RST_PANIC          "PANIC / Exception (crash!)"
#define L_RST_INT_WDT        "Interrupt watchdog (INT_WDT)"
#define L_RST_TASK_WDT       "Task watchdog (TASK_WDT — loop blocked!)"
#define L_RST_WDT            "Other watchdog"
#define L_RST_DEEPSLEEP      "Deep sleep wake (normal)"
#define L_RST_BROWNOUT       "Brownout (power undervoltage!)"
#define L_RST_SDIO           "SDIO reset"
#define L_RST_INCONNU        "Unknown"

// --- AP config page (first boot) ---
#define L_AP_TITRE           "Solar Configuration"
#define L_AP_BTN_SAVE        "Save"

// --- Main page: section titles ---
#define L_H3_PRESENT         "&#9201; Present"
#define L_H3_NOCTURNE        "&#127762; Night Status"
#define L_H3_ENERGIE         "&#128262; Energy 24h"
#define L_H3_BILAN           "&#9878; Daily Log 30d"
#define L_H3_FUTUR           "&#128301; Forecast"
#define L_H3_PASSE           "&#128197; History"

// --- Anti-bootloop badge (dynamic) ---
#define L_BADGE_BL_ACTIF     "&#9888; Anti-bootloop active \xe2\x80\x94 MOSFET OFF for %s"

// --- Off-MPPT inline ---
#define L_HORS_MPPT_INLINE   "&#9888; No MPPT &mdash; GND not common &mdash; readings invalid"

// --- Footer credit ---
#define L_FOOTER_CREDIT      "v1.23f &mdash; Made by"

// --- Config: brightness range hint ---
#define L_CFG_LUX_PLAGE_VAL  "Range: "

// --- Debug page ---
#define L_DBG_HEAP           "Free heap: "
#define L_DBG_OCTETS         " bytes \xe2\x80\x94 "
#define L_DBG_UPTIME         "Uptime: "
#define L_DBG_WIFI           "WiFi: "
#define L_DBG_BL_CONFIRM     "Disable anti-bootloop lock?"
#define L_DBG_BL_BTN         "&#128737; Disable anti-bootloop"
#define L_DBG_BL_OK          "&#10003; Anti-bootloop lock disabled. Returning..."

// --- Debug balance: table and buttons ---
#define L_DBG_BIL_FORCE      "&#9654; Force archive"
#define L_DBG_BIL_TH_CASE    "Slot"
#define L_DBG_BIL_TH_NOTE    "Note"
#define L_DBG_BIL_PROCHAIN   "next"
#define L_DBG_BIL_VALIDE     "<span class='ok'>valid</span>"
#define L_DBG_BIL_VIDE       "<span style='color:#333'>empty</span>"
#define L_DBG_BIL_AHIN       "<b>Ah in:</b>"
#define L_DBG_BIL_AHOUT      "<b>Ah out:</b>"

// --- Documentation: hardware ---
#define L_DOC_VEML           "Luminosity (lux) on Wire0 GPIO 21/22 address 0x10"
#define L_DOC_INA            "Current &amp; battery voltage on Wire1 GPIO 19/23 address 0x40"
#define L_DOC_GPIO34         "Voltage divider (configurable ratio)"
#define L_DOC_GPIO18         "MOSFET (charge control)"
#define L_DOC_GPIO2          "Indicator LED (optional)"

// --- Documentation: badges ---
#define L_DOC_BADGE_V        "Battery voltage read by voltage divider (GPIO 34)"
#define L_DOC_BADGE_CHARGE   "MOSFET state"
#define L_DOC_BADGE_LUX      "Current luminosity (VEML7700)"
#define L_DOC_BADGE_SEUIL    "Minimum luminosity to allow charging (dynamic)"
#define L_DOC_BADGE_COUPURE  "Minimum voltage before charge cutoff (dynamic)"
#define L_DOC_BADGE_SOC      "Battery icon \xe2\x80\x94 SOC%% by INA226 coulomb counting. <span class='badge green'>green &gt;70%%</span> <span class='badge orange'>orange 30-70%%</span> <span class='badge red'>red &lt;30%%</span>. &#9889; = charging, &#9660; = discharging."
#define L_DOC_BADGE_HORS     "Common GND not detected \xe2\x80\x94 invalid voltage reading"
#define L_DOC_BADGE_BL       "Overload protection active \xe2\x80\x94 MOSFET locked OFF"

// --- Documentation: charts ---
#define L_DOC_G_PRESENT      "<b>PRESENT 24h</b> \xe2\x80\x94 Voltage (V) and luminosity (lux) over the last 24 hours"
#define L_DOC_G_NOCTURNE     "<b>NIGHT STATUS</b> \xe2\x80\x94 Battery voltage during night wake-ups"
#define L_DOC_G_ENERGIE      "<b>ENERGY 24h</b> \xe2\x80\x94 Current (A) and power (W) INA226 over 24h"
#define L_DOC_G_BILAN        "<b>DAILY LOG 30d</b> \xe2\x80\x94 Ah in (solar charge) and out (consumption) per day"
#define L_DOC_G_FUTUR        "<b>FORECAST 7d</b> \xe2\x80\x94 OpenWeatherMap luminosity forecast over 7 days"
#define L_DOC_G_PASSE        "<b>HISTORY 30d</b> \xe2\x80\x94 Voltage and luminosity history over 30 days"

// --- Documentation: charge logic ---
#define L_DOC_L_ON           "MOSFET <b>ON</b> if: lux &gt; Threshold ON <b>AND</b> voltage &gt; Resume V <b>AND</b> battery OK"
#define L_DOC_L_OFF          "MOSFET <b>OFF</b> if: lux &lt; 150lx <b>OR</b> voltage &lt; Cutoff <b>OR</b> battery KO"
#define L_DOC_L_DYN          "Lux threshold and cutoff voltage are <b>dynamic</b> \xe2\x80\x94 adjusted by OWM weather forecast"

// --- Documentation: anti-bootloop protection ---
#define L_DOC_P_TITRE        "&#128737; Anti-bootloop overload protection"
#define L_DOC_P_TEXTE1       "If the MPPT cuts power due to overload (voltage drop), the ESP32 reboots, sets MOSFET HIGH again, causing the same overload \xe2\x80\x94 infinite loop."
#define L_DOC_P_TEXTE2       "Protection: if <code>N boots</code> in a row are detected without a stable period of <code>Overload delay</code> seconds, MOSFET is locked OFF for <code>Lock duration</code> (configurable). Lock persists in NVS \xe2\x80\x94 survives a full power-on reset."

// --- Documentation: SOC ---
#define L_DOC_S_TITRE        "&#128267; SOC \xe2\x80\x94 Battery state of charge"
#define L_DOC_S_TEXTE1       "<b>SOC</b> (State Of Charge) = remaining charge percentage. Calculated by <b>coulomb counting</b>: INA226 current integration over time (Ah in - Ah out) / total capacity."
#define L_DOC_S_TEXTE2       "Reset to <b>100%%</b> automatically when voltage exceeds <code>Full charge voltage</code>. Inaccurate without regular reset \xe2\x80\x94 improves with use."

// --- Documentation: sleep cycle ---
#define L_DOC_V_TITRE        "&#128564; Sleep / wake cycle"
#define L_DOC_V_LI1          "Active from <code>Morning wake time</code> to <code>Evening sleep time</code>"
#define L_DOC_V_LI2          "Night split into <code>N readings</code> slots \xe2\x80\x94 voltage reading at each wake"
#define L_DOC_V_LI3          "Deep sleep between slots (~10\xc2\xb5A)"

// --- Documentation: version ---
#define L_DOC_VER_TITRE      "&#8505; Version"
#define L_DOC_VER_TEXTE      "Solar Controller <b>123f</b> \xe2\x80\x94 ESP32 + VEML7700 + INA226 + MPPT"

// --- Page /upd: weather update ---
#define L_UPD_TITRE          "Updating weather..."
#define L_UPD_SOUS_TITRE     "OWM + NASA in progress (~15s)"
#define L_UPD_TRAITEMENT     "Processing..."

// --- Debug/bilan transition pages ---
#define L_DBG_BIL_FORCE_OK   "&#9654; Archiving..."
#define L_DBG_BIL_UNDO_OK    "&#8635; Undoing..."
#define L_DBG_BIL_RESET_OK   "&#128465; Resetting..."

// --- SVG: extra chart labels ---
#define L_SVG_JOUR_N         "D-%d"
#define L_SVG_AUJ            "Today"
#define L_SVG_AUJ_COURT      "today"
#define L_SVG_APRES_JOURS    "(available after several days)"
#define L_CHARGEMENT         "loading..."

// --- Menu: weather update ---
#define L_MENU_MAJ_METEO     "&#x27F3; Update weather"

// --- Badge anti-bootloop: JS prefix (no %s, for JS concat) ---
#define L_BADGE_BL_AVANT     "&#9888; Anti-bootloop active \xe2\x80\x94 MOSFET OFF for "

// --- Debug: last reset labels ---
#define L_DBG_RST_CRASH_LBL  "&#9888; LAST RESET: "
#define L_DBG_RST_OK_LBL     "&#10003; Last reset: "

// --- Documentation: badge labels (inline examples) ---
#define L_DOC_LBL_CHARGE     "LOAD ON/OFF"
#define L_DOC_LBL_SEUILON    "ON threshold"
#define L_DOC_LBL_COUPURE    "Cutoff"
