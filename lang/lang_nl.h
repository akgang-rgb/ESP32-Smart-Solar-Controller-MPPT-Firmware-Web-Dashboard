// =============================================================================
// Solar Controller — Language NL (Nederlands)
// To switch language: replace #include "lang_fr.h" with #include "lang_nl.h"
// =============================================================================
#pragma once

// --- SVG : chart titles ---
#define L_SVG_PRESENT        "HUIDIG \xe2\x80\x94 laatste 24 uur"
#define L_SVG_NOCTURNE       "NACHTSTATUS \xe2\x80\x94 accuspanning"
#define L_SVG_FUTUR          "VERWACHTING \xe2\x80\x94 komende 7 dagen (lux)"
#define L_SVG_PASSE          "GESCHIEDENIS \xe2\x80\x94 laatste 30 dagen"
#define L_SVG_ENERGIE        "ENERGIE \xe2\x80\x94 Stroom &amp; Vermogen 24u"
#define L_SVG_BILAN          "DAGBOEK 30d \xe2\x80\x94 Ah in / uit"

// --- SVG : legends ---
#define L_SVG_TENSION        "Spanning (V)"
#define L_SVG_LUMINOSITE     "Lichtsterkte (lux)"
#define L_SVG_TENSION_NUIT   "Nachtspanning (V)"
#define L_SVG_COURANT        "Stroom (A)"
#define L_SVG_PUISSANCE      "Vermogen (W)"
#define L_SVG_ENTRANTS       "In (Ah)"
#define L_SVG_SORTANTS       "Uit (Ah)"
#define L_SVG_COUPURE        "uitsch. %.2fV"
#define L_SVG_REPRISE        "hervatten"
#define L_SVG_SEUIL_ON       "AAN-drempel"
#define L_SVG_ATTENTE        "Wachten op eerste metingen..."
#define L_SVG_MAJ            "(elk uur bijgewerkt)"
#define L_SVG_PAS_DONNEES    "Nog geen gegevens"
#define L_SVG_BARRE_VISIBLE  "(balk van vandaag zichtbaar over %d min)"

// --- Main page header ---
#define L_HDR_CHARGE_ON      "BELASTING : AAN"
#define L_HDR_CHARGE_OFF     "BELASTING : UIT"
#define L_HDR_LUX            "Lux: "
#define L_HDR_SEUIL_ON       "AAN-drempel: "
#define L_HDR_COUPURE        "Uitschakeling: "
#define L_HDR_HORS_MPPT      "&#9888; Geen MPPT &mdash; GND niet gemeenschappelijk &mdash; metingen ongeldig"
#define L_HDR_TOAST          "☀️ Verwachting bijgewerkt voor vandaag"

// --- Main page sections ---
#define L_SEC_PRESENT        "HUIDIG"
#define L_SEC_NOCTURNE       "NACHTSTATUS"
#define L_SEC_ENERGIE        "ENERGIE 24U"
#define L_SEC_BILAN          "DAGBOEK 30D"
#define L_SEC_FUTUR          "VERWACHTING"
#define L_SEC_PASSE          "GESCHIEDENIS"

// --- Hamburger menu ---
#define L_MENU_ACCUEIL       "&#8962; Home"
#define L_MENU_CONFIG        "&#9881; Instellingen"
#define L_MENU_DEBUG         "&#128196; Debug"
#define L_MENU_DEBUG_BILAN   "&#128202; Debug Logboek"
#define L_MENU_DOC           "&#9432; Documentatie"

// --- Common buttons ---
#define L_BTN_RETOUR         "&#8592; Terug"
#define L_BTN_ENREGISTRER    "💾 Opslaan"
#define L_BTN_ANNULER        "&#8635; Laatste ongedaan"
#define L_BTN_RESET          "&#128465; Reset dag 0"
#define L_BTN_DEBLOQUER      "&#128275; MOSFET deblokkeren"

// --- Settings page: titles and tabs ---
#define L_CFG_TITRE          "&#9881; Instellingen"
#define L_TAB_GENERAL        "Algemeen"
#define L_TAB_LUMINOSITE     "Licht"
#define L_TAB_BATTERIE       "Accu"
#define L_CFG_SAUVEGARDEE    "✓ Instellingen opgeslagen. Terug..."

// --- Settings > General ---
#define L_CFG_OWM_KEY        "OpenWeatherMap API-sleutel"
#define L_CFG_RESEAU         "&#127760; Netwerk — vast IP (herstart vereist)"
#define L_CFG_IP             "ESP32 IP"
#define L_CFG_GW             "Gateway"
#define L_CFG_MASQUE         "Subnetmasker"
#define L_CFG_DNS            "DNS"
#define L_CFG_LAT            "Breedtegraad"
#define L_CFG_LON            "Lengtegraad"
#define L_CFG_TILT           "Paneelhelling"
#define L_CFG_TILT_UNIT      "° (0=plat, 90=verticaal)"
#define L_CFG_AZIMUTH        "Paneelazimut"
#define L_CFG_AZIMUTH_UNIT   "° (180=Zuid, 270=West)"
#define L_CFG_RATIO          "Uitgangsspanning (belasting)"
#define L_CFG_RATIO_HINT     "150k&#937; + 33k&#937; → (150+33)/33 = 5.5454"
#define L_CFG_SLEEP          "Slaaptijd"
#define L_CFG_SLEEP_UNIT     "min (standaard 19:45)"
#define L_CFG_REVEIL         "Wektijd"
#define L_CFG_REVEIL_UNIT    "min (standaard 09:15)"
#define L_CFG_RELEVES        "Nachtmetingen"
#define L_CFG_RELEVES_HINT   "Nachtvenster verdeeld in gelijke intervallen"
#define L_CFG_ARCHIVAGE      "Daglogarchivering voor slaap"
#define L_CFG_ARCHIVAGE_UNIT "u voor slaaptijd (0=bij slaap, 1-3u ervoor)"
#define L_CFG_ARCHIVAGE_HINT "Archiveert N uur voor slaap. Aanbevolen: 1u"

// --- Settings > Light ---
#define L_CFG_LUX_MAX        "Max lux-drempel (lage accu + slecht weer)"
#define L_CFG_LUX_MAX_HINT   "AAN-drempel bij ongunstige omstandigheden"
#define L_CFG_LUX_AMP        "Dynamisch bereik"
#define L_CFG_LUX_AMP_HINT   "Min AAN-drempel = Standaard - Bereik (ideale omstandigheid)"
#define L_CFG_LUX_CONF       "100% weerbetrouwbaarheid lux"
#define L_CFG_LUX_CONF_HINT  "Boven deze voorspelde waarde wordt weer 100% vertrouwd"
#define L_CFG_LUX_ACTUEL     "Huidige AAN-drempel: "
#define L_CFG_LUX_PLAGE      "Bereik: "

// --- Settings > Battery ---
#define L_CFG_V_PLEINE       "Vollaadspanning"
#define L_CFG_V_REPRISE      "Hervattingsspanning"
#define L_CFG_V_REPRISE_HINT "Drempel waarboven laden kan hervatten"
#define L_CFG_V_COUPURE      "Uitschakelspanning (mooi weer)"
#define L_CFG_V_COUPURE_HINT "Statische drempel bij slecht weersvoorspelling"
#define L_CFG_V_CRITIQUE     "Kritieke spanning"
#define L_CFG_V_PLANCHER     "Absolute ondergrens (nooit lager)"
#define L_CFG_V_PLANCHER_HINT "Uitschakelbodem bij mooi weer — min 11.9V aanbevolen"
#define L_CFG_CAPACITE       "Accucapaciteit (INA226 coulombtelling)"
#define L_CFG_CAPACITE_HINT  "Nominale capaciteit — bijv. 11.5Ah voor 12V/11.5Ah"
#define L_CFG_ANTIREBOOT     "Overbelastings-herstart vertraging"
#define L_CFG_ANTIREBOOT_HINT "Bootloop-venster (standaard 60s) — 2 herstarts → MOSFET UIT 1u"
#define L_CFG_CADENCE        "Meet- / opslaginterval"
#define L_CFG_CADENCE_HINT   "Sensorpoll, Ah-teling en NVS-opslag. Standaard: 10 min"
#define L_CFG_BLOCAGE        "MOSFET-blokkeertijd bij overbelasting"
#define L_CFG_COUPURE_DYN    "Huidige dynamische uitsch.: "

// --- Alert badges ---
#define L_BADGE_BLOCAGE      "&#9888; Anti-bootloop"
#define L_BADGE_HORS_MPPT    "&#9888; Geen MPPT"

// --- Debug Log page ---
#define L_DBG_BILAN_TITRE    "&#128202; Debug Dagboek 30d"
#define L_DBG_BILAN_AH_IN    "<b>ahIn:</b>"
#define L_DBG_BILAN_AH_OUT   "<b>ahOut:</b>"

// --- Debug logs page ---
#define L_DBG_TITRE          "&#11035; Debug \xe2\x80\x94 systeemlogboek"
#define L_DBG_REFRESH        "(ververs 60s)"

// --- Documentation page ---
#define L_DOC_TITRE          "&#9432; Documentatie \xe2\x80\x94 Zonneregelaar"
#define L_DOC_MATERIEL       "&#128268; Hardware"
#define L_DOC_CAPTEURS       "Sensoren"
#define L_DOC_SORTIES        "Uitgangen"
#define L_DOC_BADGES         "&#127968; Kopbadges"
#define L_DOC_GRAPHIQUES     "&#128202; Grafieken"
#define L_DOC_LOGIQUE        "&#9889; Laadlogica"
#define L_DOC_PROTECTION     "&#128737; Anti-bootloop beveiliging"
#define L_DOC_SOC            "&#128267; SOC \xe2\x80\x94 Laadtoestand"

// --- Form validation (JS) ---
#define L_VAL_HEURE_SLEEP    "Ongeldig slaapuur (0-23)"
#define L_VAL_MIN_SLEEP      "Ongeldige slaapminuten (0-59)"
#define L_VAL_HEURE_REVEIL   "Ongeldig wekuur (0-23)"
#define L_VAL_MIN_REVEIL     "Ongeldige wekminuten (0-59)"
#define L_VAL_PLAGE_COURTE   "Slaapvenster te kort (min 2u)"

// --- Reset reasons (debug page) ---
#define L_RST_POWERON        "Inschakelen"
#define L_RST_EXT            "Externe reset (EN)"
#define L_RST_SW             "Softwarehersart (ESP.restart)"
#define L_RST_PANIC          "PANIC / Uitzondering (crash!)"
#define L_RST_INT_WDT        "Interrupt-watchdog"
#define L_RST_TASK_WDT       "Taak-watchdog (lus geblokkeerd!)"
#define L_RST_WDT            "Andere watchdog"
#define L_RST_DEEPSLEEP      "Diepe slaap wake-up (normaal)"
#define L_RST_BROWNOUT       "Brownout (te lage voeding!)"
#define L_RST_SDIO           "SDIO reset"
#define L_RST_INCONNU        "Onbekend"

// --- AP config page (first boot) ---
#define L_AP_TITRE           "Zonneregeling instellen"
#define L_AP_BTN_SAVE        "Opslaan"

// --- Main page: section titles ---
#define L_H3_PRESENT         "&#9201; Huidig"
#define L_H3_NOCTURNE        "&#127762; Nachtstatus"
#define L_H3_ENERGIE         "&#128262; Energie 24u"
#define L_H3_BILAN           "&#9878; Dagboek 30d"
#define L_H3_FUTUR           "&#128301; Verwachting"
#define L_H3_PASSE           "&#128197; Geschiedenis"

// --- Anti-bootloop badge (dynamic) ---
#define L_BADGE_BL_ACTIF     "&#9888; Anti-bootloop actief \xe2\x80\x94 MOSFET UIT voor %s"

// --- Off-MPPT inline ---
#define L_HORS_MPPT_INLINE   "&#9888; Geen MPPT &mdash; GND niet gemeenschappelijk &mdash; metingen ongeldig"

// --- Footer credit ---
#define L_FOOTER_CREDIT      "v1.23f &mdash; Gemaakt door"

// --- Config: brightness range hint ---
#define L_CFG_LUX_PLAGE_VAL  "Bereik: "

// --- Debug page ---
#define L_DBG_HEAP           "Vrij geheugen: "
#define L_DBG_OCTETS         " bytes \xe2\x80\x94 "
#define L_DBG_UPTIME         "Uptime: "
#define L_DBG_WIFI           "WiFi: "
#define L_DBG_BL_CONFIRM     "Anti-bootloop vergrendeling uitschakelen?"
#define L_DBG_BL_BTN         "&#128737; Vergrendeling uitschakelen"
#define L_DBG_BL_OK          "&#10003; Vergrendeling uitgeschakeld. Terug..."

// --- Debug balance: table and buttons ---
#define L_DBG_BIL_FORCE      "&#9654; Archivering forceren"
#define L_DBG_BIL_TH_CASE    "Slot"
#define L_DBG_BIL_TH_NOTE    "Opmerking"
#define L_DBG_BIL_PROCHAIN   "volgende"
#define L_DBG_BIL_VALIDE     "<span class='ok'>geldig</span>"
#define L_DBG_BIL_VIDE       "<span style='color:#333'>leeg</span>"
#define L_DBG_BIL_AHIN       "<b>Ah in:</b>"
#define L_DBG_BIL_AHOUT      "<b>Ah uit:</b>"

// --- Documentation: hardware ---
#define L_DOC_VEML           "Lichtsterkte (lux) op Wire0 GPIO 21/22 adres 0x10"
#define L_DOC_INA            "Stroom &amp; accuspanning op Wire1 GPIO 19/23 adres 0x40"
#define L_DOC_GPIO34         "Spanningsdeler (instelbare verhouding)"
#define L_DOC_GPIO18         "MOSFET (laadregeling)"
#define L_DOC_GPIO2          "Indicator-LED (optioneel)"

// --- Documentation: badges ---
#define L_DOC_BADGE_V        "Accuspanning via spanningsdeler (GPIO 34)"
#define L_DOC_BADGE_CHARGE   "MOSFET-toestand"
#define L_DOC_BADGE_LUX      "Huidige lichtsterkte (VEML7700)"
#define L_DOC_BADGE_SEUIL    "Minimale lichtsterkte voor laden (dynamisch)"
#define L_DOC_BADGE_COUPURE  "Minimale spanning voor uitschakeling (dynamisch)"
#define L_DOC_BADGE_SOC      "Accupictogram \xe2\x80\x94 SOC%% via INA226 coulombtelling. <span class='badge green'>groen &gt;70%%</span> <span class='badge orange'>oranje 30-70%%</span> <span class='badge red'>rood &lt;30%%</span>. &#9889;=laden, &#9660;=ontladen."
#define L_DOC_BADGE_HORS     "Gemeenschappelijke GND niet gevonden \xe2\x80\x94 spanningmeting ongeldig"
#define L_DOC_BADGE_BL       "Overbelastingsbeveiliging actief \xe2\x80\x94 MOSFET vergrendeld UIT"

// --- Documentation: charts ---
#define L_DOC_G_PRESENT      "<b>HUIDIG 24u</b> \xe2\x80\x94 Spanning (V) en lichtsterkte (lux) laatste 24 uur"
#define L_DOC_G_NOCTURNE     "<b>NACHTSTATUS</b> \xe2\x80\x94 Accuspanning bij nachtelijk wekkers"
#define L_DOC_G_ENERGIE      "<b>ENERGIE 24u</b> \xe2\x80\x94 Stroom (A) en vermogen (W) INA226 over 24u"
#define L_DOC_G_BILAN        "<b>DAGBOEK 30d</b> \xe2\x80\x94 Ah in (zonneladen) en uit (verbruik) per dag"
#define L_DOC_G_FUTUR        "<b>VERWACHTING 7d</b> \xe2\x80\x94 OpenWeatherMap lichtsterkte-verwachting 7 dagen"
#define L_DOC_G_PASSE        "<b>GESCHIEDENIS 30d</b> \xe2\x80\x94 Spanning en lichtsterkte laatste 30 dagen"

// --- Documentation: charge logic ---
#define L_DOC_L_ON           "MOSFET <b>AAN</b> als: lux &gt; AAN-drempel <b>EN</b> spanning &gt; Herv.spanning <b>EN</b> accu OK"
#define L_DOC_L_OFF          "MOSFET <b>UIT</b> als: lux &lt; 150lx <b>OF</b> spanning &lt; Uitsch. <b>OF</b> accu KO"
#define L_DOC_L_DYN          "Lux-drempel en uitschakelspanning zijn <b>dynamisch</b> \xe2\x80\x94 aangepast door OWM-weersvoorspelling"

// --- Documentation: anti-bootloop protection ---
#define L_DOC_P_TITRE        "&#128737; Anti-bootloop overbelastingsbeveiliging"
#define L_DOC_P_TEXTE1       "Als de MPPT bij overbelasting uitschakelt, herstart de ESP32 en zet de MOSFET opnieuw aan — oneindige lus."
#define L_DOC_P_TEXTE2       "Beveiliging: bij <code>N herstarts</code> zonder stabiele periode van <code>Overbelasting-vertraging</code> seconden, wordt MOSFET voor <code>Blokkeer-duur</code> vergrendeld UIT. Vergrendeling blijft in NVS na volledige stroomuitval."

// --- Documentation: SOC ---
#define L_DOC_S_TITRE        "&#128267; SOC \xe2\x80\x94 Laadtoestand accu"
#define L_DOC_S_TEXTE1       "<b>SOC</b> (State Of Charge) = resterende lading in procent. Berekend via <b>coulombtelling</b>: INA226 stroomintegratie over tijd (Ah in - Ah uit) / totale capaciteit."
#define L_DOC_S_TEXTE2       "Automatisch teruggezet naar <b>100%%</b> als spanning de <code>Vollaadspanning</code> overschrijdt. Nauwkeurigheid verbetert met gebruik."

// --- Documentation: sleep cycle ---
#define L_DOC_V_TITRE        "&#128564; Slaap/wake-cyclus"
#define L_DOC_V_LI1          "Actief van <code>Ochtendontwaktijd</code> tot <code>Avondslaaptijd</code>"
#define L_DOC_V_LI2          "Nacht verdeeld in <code>N metingen</code> \xe2\x80\x94 spanning gemeten bij elke wake-up"
#define L_DOC_V_LI3          "Diepe slaap tussen intervallen (~10\xc2\xb5A)"

// --- Documentation: version ---
#define L_DOC_VER_TITRE      "&#8505; Versie"
#define L_DOC_VER_TEXTE      "Zonneregelaar <b>123f</b> \xe2\x80\x94 ESP32 + VEML7700 + INA226 + MPPT"

// --- Page /upd: weather update ---
#define L_UPD_TITRE          "Weer bijwerken..."
#define L_UPD_SOUS_TITRE     "OWM + NASA bezig (~15s)"
#define L_UPD_TRAITEMENT     "Verwerken..."

// --- Debug/bilan transition pages ---
#define L_DBG_BIL_FORCE_OK   "&#9654; Archiveren..."
#define L_DBG_BIL_UNDO_OK    "&#8635; Ongedaan maken..."
#define L_DBG_BIL_RESET_OK   "&#128465; Resetten..."

// --- SVG: extra chart labels ---
#define L_SVG_JOUR_N         "D-%d"
#define L_SVG_AUJ            "Vandaag"
#define L_SVG_AUJ_COURT      "vandaag"
#define L_SVG_APRES_JOURS    "(beschikbaar na enkele dagen)"
#define L_CHARGEMENT         "laden..."

// --- Menu: weather update ---
#define L_MENU_MAJ_METEO     "&#x27F3; Weer bijwerken"

// --- Badge anti-bootloop: JS prefix ---
#define L_BADGE_BL_AVANT     "&#9888; Anti-bootloop actief \xe2\x80\x94 MOSFET UIT voor "

// --- Debug: last reset labels ---
#define L_DBG_RST_CRASH_LBL  "&#9888; LAATSTE RESET: "
#define L_DBG_RST_OK_LBL     "&#10003; Laatste reset: "

// --- Documentation: badge labels (inline examples) ---
#define L_DOC_LBL_CHARGE     "BELASTING AAN/UIT"
#define L_DOC_LBL_SEUILON    "AAN-drempel"
#define L_DOC_LBL_COUPURE    "Uitsch."
