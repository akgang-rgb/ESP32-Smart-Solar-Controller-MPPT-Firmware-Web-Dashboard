// =============================================================================
// Solar Controller — Language SV (Svenska)
// To switch language: replace #include "lang_fr.h" with #include "lang_sv.h"
// =============================================================================
#pragma once

// --- SVG : chart titles ---
#define L_SVG_PRESENT        "AKTUELLT \xe2\x80\x94 senaste 24 timmarna"
#define L_SVG_NOCTURNE       "NATTSTATUS \xe2\x80\x94 batterispänning"
#define L_SVG_FUTUR          "PROGNOS \xe2\x80\x94 kommande 7 dagar (lux)"
#define L_SVG_PASSE          "HISTORIK \xe2\x80\x94 senaste 30 dagarna"
#define L_SVG_ENERGIE        "ENERGI \xe2\x80\x94 Ström &amp; Effekt 24h"
#define L_SVG_BILAN          "DAGSLOGG 30d \xe2\x80\x94 Ah in / ut"

// --- SVG : legends ---
#define L_SVG_TENSION        "Spänning (V)"
#define L_SVG_LUMINOSITE     "Ljusstyrka (lux)"
#define L_SVG_TENSION_NUIT   "Nattspänning (V)"
#define L_SVG_COURANT        "Ström (A)"
#define L_SVG_PUISSANCE      "Effekt (W)"
#define L_SVG_ENTRANTS       "In (Ah)"
#define L_SVG_SORTANTS       "Ut (Ah)"
#define L_SVG_COUPURE        "frånkoppl. %.2fV"
#define L_SVG_REPRISE        "återuppta"
#define L_SVG_SEUIL_ON       "PÅ-tröskel"
#define L_SVG_ATTENTE        "Väntar på första mätningarna..."
#define L_SVG_MAJ            "(uppdateras varje timme)"
#define L_SVG_PAS_DONNEES    "Inga data ännu"
#define L_SVG_BARRE_VISIBLE  "(dagens stapel synlig om %d min)"

// --- Main page header ---
#define L_HDR_CHARGE_ON      "LAST : PÅ"
#define L_HDR_CHARGE_OFF     "LAST : AV"
#define L_HDR_LUX            "Lux: "
#define L_HDR_SEUIL_ON       "PÅ-tröskel: "
#define L_HDR_COUPURE        "Frånkoppling: "
#define L_HDR_HORS_MPPT      "&#9888; Ingen MPPT &mdash; GND ej gemensam &mdash; mätningar ogiltiga"
#define L_HDR_TOAST          "☀️ Prognos uppdaterad för idag"

// --- Main page sections ---
#define L_SEC_PRESENT        "AKTUELLT"
#define L_SEC_NOCTURNE       "NATTSTATUS"
#define L_SEC_ENERGIE        "ENERGI 24H"
#define L_SEC_BILAN          "DAGSLOGG 30D"
#define L_SEC_FUTUR          "PROGNOS"
#define L_SEC_PASSE          "HISTORIK"

// --- Hamburger menu ---
#define L_MENU_ACCUEIL       "&#8962; Hem"
#define L_MENU_CONFIG        "&#9881; Inställningar"
#define L_MENU_DEBUG         "&#128196; Felsökning"
#define L_MENU_DEBUG_BILAN   "&#128202; Felsökningslogg"
#define L_MENU_DOC           "&#9432; Dokumentation"

// --- Common buttons ---
#define L_BTN_RETOUR         "&#8592; Tillbaka"
#define L_BTN_ENREGISTRER    "💾 Spara"
#define L_BTN_ANNULER        "&#8635; Ångra sista"
#define L_BTN_RESET          "&#128465; Återställ dag 0"
#define L_BTN_DEBLOQUER      "&#128275; Lås upp MOSFET"

// --- Settings page: titles and tabs ---
#define L_CFG_TITRE          "&#9881; Inställningar"
#define L_TAB_GENERAL        "Allmänt"
#define L_TAB_LUMINOSITE     "Ljus"
#define L_TAB_BATTERIE       "Batteri"
#define L_CFG_SAUVEGARDEE    "✓ Inställningar sparade. Återgår..."

// --- Settings > General ---
#define L_CFG_OWM_KEY        "OpenWeatherMap API-nyckel"
#define L_CFG_RESEAU         "&#127760; Nätverk — fast IP (omstart krävs)"
#define L_CFG_IP             "ESP32 IP"
#define L_CFG_GW             "Gateway"
#define L_CFG_MASQUE         "Nätmask"
#define L_CFG_DNS            "DNS"
#define L_CFG_LAT            "Latitud"
#define L_CFG_LON            "Longitud"
#define L_CFG_TILT           "Panellutning"
#define L_CFG_TILT_UNIT      "° (0=plant, 90=vertikalt)"
#define L_CFG_AZIMUTH        "Panelazimut"
#define L_CFG_AZIMUTH_UNIT   "° (180=Söder, 270=Väster)"
#define L_CFG_RATIO          "Utgångsspänning (last)"
#define L_CFG_RATIO_HINT     "150k&#937; + 33k&#937; → (150+33)/33 = 5.5454"
#define L_CFG_SLEEP          "Läggningstid"
#define L_CFG_SLEEP_UNIT     "min (standard 19:45)"
#define L_CFG_REVEIL         "Uppvakningstid"
#define L_CFG_REVEIL_UNIT    "min (standard 09:15)"
#define L_CFG_RELEVES        "Nattmätningar"
#define L_CFG_RELEVES_HINT   "Nattfönstret delas i lika intervall"
#define L_CFG_ARCHIVAGE      "Logarchivering före sömn"
#define L_CFG_ARCHIVAGE_UNIT "h före sömn (0=vid sömn, 1-3h före)"
#define L_CFG_ARCHIVAGE_HINT "Arkiverar N timmar före sömn. Rekommenderat: 1h"

// --- Settings > Light ---
#define L_CFG_LUX_MAX        "Max lux-tröskel (lågt batteri + dåligt väder)"
#define L_CFG_LUX_MAX_HINT   "PÅ-tröskel vid ogynnsamma förhållanden"
#define L_CFG_LUX_AMP        "Dynamiskt område"
#define L_CFG_LUX_AMP_HINT   "Min PÅ-tröskel = Standard - Område (idealiska förhållanden)"
#define L_CFG_LUX_CONF       "100% väderförtroende lux"
#define L_CFG_LUX_CONF_HINT  "Över detta prognosvärde litas väderdata 100%"
#define L_CFG_LUX_ACTUEL     "Aktuell PÅ-tröskel: "
#define L_CFG_LUX_PLAGE      "Område: "

// --- Settings > Battery ---
#define L_CFG_V_PLEINE       "Fulladdningsspänning"
#define L_CFG_V_REPRISE      "Återupptagningsspänning"
#define L_CFG_V_REPRISE_HINT "Tröskel för att laddning ska kunna återupptas"
#define L_CFG_V_COUPURE      "Frånkopplingsspänning (fint väder)"
#define L_CFG_V_COUPURE_HINT "Statisk tröskel vid dålig väderprognos"
#define L_CFG_V_CRITIQUE     "Kritisk spänning"
#define L_CFG_V_PLANCHER     "Absolut golv (aldrig under)"
#define L_CFG_V_PLANCHER_HINT "Frånkopplingsgolv vid fint väder — min 11.9V rekommenderat"
#define L_CFG_CAPACITE       "Batterikapacitet (INA226 coulombräkning)"
#define L_CFG_CAPACITE_HINT  "Nominell kapacitet — t.ex. 11.5Ah för 12V/11.5Ah"
#define L_CFG_ANTIREBOOT     "Överlastnings-omstartsfördröjning"
#define L_CFG_ANTIREBOOT_HINT "Bootloop-fönster (standard 60s) — 2 omstarter → MOSFET AV 1h"
#define L_CFG_CADENCE        "Mätnings- / sparintervall"
#define L_CFG_CADENCE_HINT   "Sensoravfrågan, Ah-räkning och NVS-sparning. Standard: 10 min"
#define L_CFG_BLOCAGE        "MOSFET-låsningstid vid överlast"
#define L_CFG_COUPURE_DYN    "Aktuell dynamisk frånk.: "

// --- Alert badges ---
#define L_BADGE_BLOCAGE      "&#9888; Anti-bootloop"
#define L_BADGE_HORS_MPPT    "&#9888; Ingen MPPT"

// --- Debug Log page ---
#define L_DBG_BILAN_TITRE    "&#128202; Felsökning Dagslogg 30d"
#define L_DBG_BILAN_AH_IN    "<b>ahIn:</b>"
#define L_DBG_BILAN_AH_OUT   "<b>ahOut:</b>"

// --- Debug logs page ---
#define L_DBG_TITRE          "&#11035; Felsökning \xe2\x80\x94 systemloggar"
#define L_DBG_REFRESH        "(uppdatering 60s)"

// --- Documentation page ---
#define L_DOC_TITRE          "&#9432; Dokumentation \xe2\x80\x94 Solregulator"
#define L_DOC_MATERIEL       "&#128268; Hårdvara"
#define L_DOC_CAPTEURS       "Sensorer"
#define L_DOC_SORTIES        "Utgångar"
#define L_DOC_BADGES         "&#127968; Huvud-badges"
#define L_DOC_GRAPHIQUES     "&#128202; Grafer"
#define L_DOC_LOGIQUE        "&#9889; Laddningslogik"
#define L_DOC_PROTECTION     "&#128737; Anti-bootloop överlastsskydd"
#define L_DOC_SOC            "&#128267; SOC \xe2\x80\x94 Laddningstillstånd"

// --- Form validation (JS) ---
#define L_VAL_HEURE_SLEEP    "Ogiltigt sömnstimme (0-23)"
#define L_VAL_MIN_SLEEP      "Ogiltiga sömnminuter (0-59)"
#define L_VAL_HEURE_REVEIL   "Ogiltigt uppvakningsimme (0-23)"
#define L_VAL_MIN_REVEIL     "Ogiltiga uppvakningsminuter (0-59)"
#define L_VAL_PLAGE_COURTE   "Sömnfönstret för kort (min 2h)"

// --- Reset reasons (debug page) ---
#define L_RST_POWERON        "Strömtillslag"
#define L_RST_EXT            "Extern återställning (EN)"
#define L_RST_SW             "Programvaruomstart (ESP.restart)"
#define L_RST_PANIC          "PANIC / Undantag (krasch!)"
#define L_RST_INT_WDT        "Avbrottsvakthund (INT_WDT)"
#define L_RST_TASK_WDT       "Uppgiftsvakthund (TASK_WDT — slinga blockerad!)"
#define L_RST_WDT            "Annan vakthund"
#define L_RST_DEEPSLEEP      "Djupsömns-uppvakning (normalt)"
#define L_RST_BROWNOUT       "Brownout (för låg matningsspänning!)"
#define L_RST_SDIO           "SDIO-återställning"
#define L_RST_INCONNU        "Okänt"

// --- AP config page (first boot) ---
#define L_AP_TITRE           "Solkonfiguration"
#define L_AP_BTN_SAVE        "Spara"

// --- Main page: section titles ---
#define L_H3_PRESENT         "&#9201; Aktuellt"
#define L_H3_NOCTURNE        "&#127762; Nattstatus"
#define L_H3_ENERGIE         "&#128262; Energi 24h"
#define L_H3_BILAN           "&#9878; Dagslogg 30d"
#define L_H3_FUTUR           "&#128301; Prognos"
#define L_H3_PASSE           "&#128197; Historik"

// --- Anti-bootloop badge (dynamic) ---
#define L_BADGE_BL_ACTIF     "&#9888; Anti-bootloop aktiv \xe2\x80\x94 MOSFET AV i %s"

// --- Off-MPPT inline ---
#define L_HORS_MPPT_INLINE   "&#9888; Ingen MPPT &mdash; GND ej gemensam &mdash; mätningar ogiltiga"

// --- Footer credit ---
#define L_FOOTER_CREDIT      "v1.23f &mdash; Skapad av"

// --- Config: brightness range hint ---
#define L_CFG_LUX_PLAGE_VAL  "Område: "

// --- Debug page ---
#define L_DBG_HEAP           "Fritt minne: "
#define L_DBG_OCTETS         " bytes \xe2\x80\x94 "
#define L_DBG_UPTIME         "Drifttid: "
#define L_DBG_WIFI           "WiFi: "
#define L_DBG_BL_CONFIRM     "Inaktivera anti-bootloop-lås?"
#define L_DBG_BL_BTN         "&#128737; Inaktivera lås"
#define L_DBG_BL_OK          "&#10003; Lås inaktiverat. Återgår..."

// --- Debug balance: table and buttons ---
#define L_DBG_BIL_FORCE      "&#9654; Tvinga arkivering"
#define L_DBG_BIL_TH_CASE    "Slot"
#define L_DBG_BIL_TH_NOTE    "Anteckning"
#define L_DBG_BIL_PROCHAIN   "nästa"
#define L_DBG_BIL_VALIDE     "<span class='ok'>giltig</span>"
#define L_DBG_BIL_VIDE       "<span style='color:#333'>tom</span>"
#define L_DBG_BIL_AHIN       "<b>Ah in:</b>"
#define L_DBG_BIL_AHOUT      "<b>Ah ut:</b>"

// --- Documentation: hardware ---
#define L_DOC_VEML           "Ljusstyrka (lux) på Wire0 GPIO 21/22 adress 0x10"
#define L_DOC_INA            "Ström &amp; batterispänning på Wire1 GPIO 19/23 adress 0x40"
#define L_DOC_GPIO34         "Spänningsdelare (konfigurerbar kvot)"
#define L_DOC_GPIO18         "MOSFET (laddningsstyrning)"
#define L_DOC_GPIO2          "Indikator-LED (valfri)"

// --- Documentation: badges ---
#define L_DOC_BADGE_V        "Batterispänning via spänningsdelare (GPIO 34)"
#define L_DOC_BADGE_CHARGE   "MOSFET-tillstånd"
#define L_DOC_BADGE_LUX      "Aktuell ljusstyrka (VEML7700)"
#define L_DOC_BADGE_SEUIL    "Minsta ljusstyrka för att tillåta laddning (dynamisk)"
#define L_DOC_BADGE_COUPURE  "Minsta spänning före laddningsfrånkoppling (dynamisk)"
#define L_DOC_BADGE_SOC      "Batteriikon \xe2\x80\x94 SOC%% via INA226 coulombräkning. <span class='badge green'>grön &gt;70%%</span> <span class='badge orange'>orange 30-70%%</span> <span class='badge red'>röd &lt;30%%</span>. &#9889;=laddning, &#9660;=urladdning."
#define L_DOC_BADGE_HORS     "Gemensam GND ej detekterad \xe2\x80\x94 spänningmätning ogiltig"
#define L_DOC_BADGE_BL       "Överlastsskydd aktivt \xe2\x80\x94 MOSFET låst AV"

// --- Documentation: charts ---
#define L_DOC_G_PRESENT      "<b>AKTUELLT 24h</b> \xe2\x80\x94 Spänning (V) och ljusstyrka (lux) senaste 24 timmarna"
#define L_DOC_G_NOCTURNE     "<b>NATTSTATUS</b> \xe2\x80\x94 Batterispänning vid nattliga uppvakningar"
#define L_DOC_G_ENERGIE      "<b>ENERGI 24h</b> \xe2\x80\x94 Ström (A) och effekt (W) INA226 över 24h"
#define L_DOC_G_BILAN        "<b>DAGSLOGG 30d</b> \xe2\x80\x94 Ah in (solarladdning) och ut (förbrukning) per dag"
#define L_DOC_G_FUTUR        "<b>PROGNOS 7d</b> \xe2\x80\x94 OpenWeatherMap ljusstyrke-prognos 7 dagar"
#define L_DOC_G_PASSE        "<b>HISTORIK 30d</b> \xe2\x80\x94 Spänning och ljusstyrka senaste 30 dagarna"

// --- Documentation: charge logic ---
#define L_DOC_L_ON           "MOSFET <b>PÅ</b> om: lux &gt; PÅ-tröskel <b>OCH</b> spänning &gt; Återupptag. V <b>OCH</b> batteri OK"
#define L_DOC_L_OFF          "MOSFET <b>AV</b> om: lux &lt; 150lx <b>ELLER</b> spänning &lt; Frånk. <b>ELLER</b> batteri KO"
#define L_DOC_L_DYN          "Lux-tröskel och frånkopplingsspänning är <b>dynamiska</b> \xe2\x80\x94 justeras av OWM-väderprognos"

// --- Documentation: anti-bootloop protection ---
#define L_DOC_P_TITRE        "&#128737; Anti-bootloop överlastsskydd"
#define L_DOC_P_TEXTE1       "Om MPPT kopplar bort vid överlast startar ESP32 om och sätter MOSFET ON igen — oändlig slinga."
#define L_DOC_P_TEXTE2       "Skydd: vid <code>N startningar</code> i rad utan stabil period låses MOSFET AV i <code>Låsningstid</code>. Låsning bevaras i NVS och överlever fullständig strömavstängning."

// --- Documentation: SOC ---
#define L_DOC_S_TITRE        "&#128267; SOC \xe2\x80\x94 Batteriets laddningstillstånd"
#define L_DOC_S_TEXTE1       "<b>SOC</b> (State Of Charge) = återstående laddningsprocent. Beräknas via <b>coulombräkning</b>: INA226 strömintegrering över tid (Ah in - Ah ut) / total kapacitet."
#define L_DOC_S_TEXTE2       "Återställs automatiskt till <b>100%%</b> när spänningen överstiger <code>Fulladdningsspänningen</code>. Noggrannhet förbättras med användning."

// --- Documentation: sleep cycle ---
#define L_DOC_V_TITRE        "&#128564; Sömn/uppvakningscykel"
#define L_DOC_V_LI1          "Aktiv från <code>Morgonuppvakning</code> till <code>Kvällssömn</code>"
#define L_DOC_V_LI2          "Natten delad i <code>N mätningar</code> \xe2\x80\x94 spänning mäts vid varje uppvakning"
#define L_DOC_V_LI3          "Djupsömn mellan intervall (~10\xc2\xb5A)"

// --- Documentation: version ---
#define L_DOC_VER_TITRE      "&#8505; Version"
#define L_DOC_VER_TEXTE      "Solregulator <b>123f</b> \xe2\x80\x94 ESP32 + VEML7700 + INA226 + MPPT"

// --- Page /upd: weather update ---
#define L_UPD_TITRE          "Uppdaterar väder..."
#define L_UPD_SOUS_TITRE     "OWM + NASA pågår (~15s)"
#define L_UPD_TRAITEMENT     "Bearbetar..."

// --- Debug/bilan transition pages ---
#define L_DBG_BIL_FORCE_OK   "&#9654; Arkiverar..."
#define L_DBG_BIL_UNDO_OK    "&#8635; Ångrar..."
#define L_DBG_BIL_RESET_OK   "&#128465; Återställer..."

// --- SVG: extra chart labels ---
#define L_SVG_JOUR_N         "D-%d"
#define L_SVG_AUJ            "Idag"
#define L_SVG_AUJ_COURT      "idag"
#define L_SVG_APRES_JOURS    "(tillgänglig efter några dagar)"
#define L_CHARGEMENT         "laddar..."

// --- Menu: weather update ---
#define L_MENU_MAJ_METEO     "&#x27F3; Uppdatera väder"

// --- Badge anti-bootloop: JS prefix ---
#define L_BADGE_BL_AVANT     "&#9888; Anti-bootloop aktiv \xe2\x80\x94 MOSFET AV i "

// --- Debug: last reset labels ---
#define L_DBG_RST_CRASH_LBL  "&#9888; SENASTE ÅTERSTÄLLNING: "
#define L_DBG_RST_OK_LBL     "&#10003; Senaste återställning: "

// --- Documentation: badge labels (inline examples) ---
#define L_DOC_LBL_CHARGE     "LAST PÅ/AV"
#define L_DOC_LBL_SEUILON    "PÅ-tröskel"
#define L_DOC_LBL_COUPURE    "Frånk."
