// =============================================================================
// Solar Controller — Langue FR (français)
// Pour changer de langue : remplacer #include "lang_fr.h" par #include "lang_en.h"
// =============================================================================
#pragma once

// --- SVG : titres graphiques ---
#define L_SVG_PRESENT        "PRESENT \xe2\x80\x94 24 dernieres heures"
#define L_SVG_NOCTURNE       "ETAT NOCTURNE \xe2\x80\x94 tension nuit"
#define L_SVG_FUTUR          "FUTUR \xe2\x80\x94 Previsions 7 jours (lux)"
#define L_SVG_PASSE          "PASSE \xe2\x80\x94 Historique 30 jours"
#define L_SVG_ENERGIE        "ENERGIE \xe2\x80\x94 Courant &amp; Puissance 24h"
#define L_SVG_BILAN          "BILAN 30j \xe2\x80\x94 Ah entrants / sortants"

// --- SVG : légendes ---
#define L_SVG_TENSION        "Tension (V)"
#define L_SVG_LUMINOSITE     "Luminosite (lux)"
#define L_SVG_TENSION_NUIT   "Tension nuit (V)"
#define L_SVG_COURANT        "Courant (A)"
#define L_SVG_PUISSANCE      "Puissance (W)"
#define L_SVG_ENTRANTS       "Entrants (Ah)"
#define L_SVG_SORTANTS       "Sortants (Ah)"
#define L_SVG_COUPURE        "coupure %.2fV"
#define L_SVG_REPRISE        "reprise"
#define L_SVG_SEUIL_ON       "seuil ON"
#define L_SVG_ATTENTE        "En attente des premieres mesures..."
#define L_SVG_MAJ            "(mise a jour toutes les heures)"
#define L_SVG_PAS_DONNEES    "Pas encore de donnees"
#define L_SVG_BARRE_VISIBLE  "(barre du jour visible dans %d min)"

// --- En-tête page principale ---
#define L_HDR_CHARGE_ON      "CHARGE : ON"
#define L_HDR_CHARGE_OFF     "CHARGE : OFF"
#define L_HDR_LUX            "Lux : "
#define L_HDR_SEUIL_ON       "Seuil ON : "
#define L_HDR_COUPURE        "Coupure : "
#define L_HDR_HORS_MPPT      "&#9888; Hors MPPT &mdash; GND non commun &mdash; mesures invalides"
#define L_HDR_TOAST          "☀️ Previsions à jour pour aujourd'hui"

// --- Sections page principale ---
#define L_SEC_PRESENT        "PRESENT"
#define L_SEC_NOCTURNE       "ETAT NOCTURNE"
#define L_SEC_ENERGIE        "ENERGIE 24H"
#define L_SEC_BILAN          "BILAN 30J"
#define L_SEC_FUTUR          "FUTUR"
#define L_SEC_PASSE          "PASSE"

// --- Menu hamburger ---
#define L_MENU_ACCUEIL       "&#8962; Accueil"
#define L_MENU_CONFIG        "&#9881; Configuration"
#define L_MENU_DEBUG         "&#128196; Debug"
#define L_MENU_DEBUG_BILAN   "&#128202; Debug Bilan"
#define L_MENU_DOC           "&#9432; Documentation"

// --- Boutons communs ---
#define L_BTN_RETOUR         "&#8592; Retour"
#define L_BTN_ENREGISTRER    "💾 Enregistrer"
#define L_BTN_ANNULER        "&#8635; Annuler dernier"
#define L_BTN_RESET          "&#128465; Reset J0"
#define L_BTN_DEBLOQUER      "&#128275; Débloquer MOSFET"

// --- Page Configuration : titres et onglets ---
#define L_CFG_TITRE          "&#9881; Configuration"
#define L_TAB_GENERAL        "Général"
#define L_TAB_LUMINOSITE     "Luminosité"
#define L_TAB_BATTERIE       "Batterie"
#define L_CFG_SAUVEGARDEE    "✓ Configuration enregistrée. Retour..."

// --- Config > Général ---
#define L_CFG_OWM_KEY        "Clé API OpenWeatherMap"
#define L_CFG_RESEAU         "&#127760; Réseau — IP statique (redémarrage requis)"
#define L_CFG_IP             "IP ESP32"
#define L_CFG_GW             "Passerelle"
#define L_CFG_MASQUE         "Masque"
#define L_CFG_DNS            "DNS"
#define L_CFG_LAT            "Latitude"
#define L_CFG_LON            "Longitude"
#define L_CFG_TILT           "Inclinaison panneau (Tilt)"
#define L_CFG_TILT_UNIT      "° (0=horizontal, 90=vertical)"
#define L_CFG_AZIMUTH        "Orientation panneau (Azimuth)"
#define L_CFG_AZIMUTH_UNIT   "° (180=Sud, 270=Ouest)"
#define L_CFG_RATIO          "Tension sortie (charge)"
#define L_CFG_RATIO_HINT     "150k&#937; + 33k&#937; → (150+33)/33 = 5.5454"
#define L_CFG_SLEEP          "Entrée en sleep"
#define L_CFG_SLEEP_UNIT     "min (défaut 19h45)"
#define L_CFG_REVEIL         "Mise en route"
#define L_CFG_REVEIL_UNIT    "min (défaut 09h15)"
#define L_CFG_RELEVES        "Relevés nocturnes"
#define L_CFG_RELEVES_HINT   "La plage de sommeil est divisée en tronçons égaux. Ex : 19h45→09h15 avec 2 relevés = 3 tronçons de ~4h26"
#define L_CFG_ARCHIVAGE      "Archivage bilan avant sleep"
#define L_CFG_ARCHIVAGE_UNIT "h avant sleep (0=au moment du sleep, 1-3h avant)"
#define L_CFG_ARCHIVAGE_HINT "D&eacute;clenche l&rsquo;archivage N heures avant l&rsquo;entr&eacute;e en sleep. Recommand&eacute; : 1h"

// --- Config > Luminosité ---
#define L_CFG_LUX_MAX        "Seuil lux maximum (batterie faible + mauvais temps)"
#define L_CFG_LUX_MAX_HINT   "Seuil ON quand la situation est défavorable"
#define L_CFG_LUX_AMP        "Amplitude dynamique"
#define L_CFG_LUX_AMP_HINT   "Seuil ON minimum = Défaut - Amplitude (conditions idéales)"
#define L_CFG_LUX_CONF       "Lux de confiance météo 100%"
#define L_CFG_LUX_CONF_HINT  "Au-delà de cette valeur prévue, la météo est jugée fiable à 100%"
#define L_CFG_LUX_ACTUEL     "Seuil ON actuel : "
#define L_CFG_LUX_PLAGE      "Plage : "

// --- Config > Batterie ---
#define L_CFG_V_PLEINE       "Tension pleine charge"
#define L_CFG_V_REPRISE      "Tension de reprise charge"
#define L_CFG_V_REPRISE_HINT "Seuil au-dessus duquel la charge peut reprendre"
#define L_CFG_V_COUPURE      "Tension de coupure (beau temps)"
#define L_CFG_V_COUPURE_HINT "Seuil statique utilisé par mauvais temps prévu"
#define L_CFG_V_CRITIQUE     "Tension critique"
#define L_CFG_V_PLANCHER     "Plancher absolu (jamais en dessous)"
#define L_CFG_V_PLANCHER_HINT "Seuil de coupure par très beau temps prévu — minimum 11.9V conseillé"
#define L_CFG_CAPACITE       "Capacité batterie (INA226 coulomb counting)"
#define L_CFG_CAPACITE_HINT  "Capacité nominale — ex: 11.5Ah pour un 12V/11.5Ah"
#define L_CFG_ANTIREBOOT     "Délai anti reboot de surcharge"
#define L_CFG_ANTIREBOOT_HINT "Fenêtre de détection boot loop (défaut 60s) — si 2 reboots dans ce délai avec charge élevée, MOSFET OFF 1h"
#define L_CFG_CADENCE        "Cadence mesure / sauvegarde"
#define L_CFG_CADENCE_HINT   "Cadence de lecture capteurs, comptage Ah et sauvegarde NVS. Défaut : 10 min"
#define L_CFG_BLOCAGE        "Durée blocage MOSFET si surcharge"
#define L_CFG_COUPURE_DYN    "Coupure dynamique actuelle : "

// --- Badges alertes ---
#define L_BADGE_BLOCAGE      "&#9888; Anti-bootloop"
#define L_BADGE_HORS_MPPT    "&#9888; Hors MPPT"

// --- Page Debug Bilan ---
#define L_DBG_BILAN_TITRE    "&#128202; Debug Bilan 30j"
#define L_DBG_BILAN_AH_IN    "<b>ahEntrants :</b>"
#define L_DBG_BILAN_AH_OUT   "<b>ahSortants :</b>"

// --- Page Debug logs ---
#define L_DBG_TITRE          "&#11035; Debug &#8212; logs systeme"
#define L_DBG_REFRESH        "(refresh 60s)"

// --- Page Documentation ---
#define L_DOC_TITRE          "&#9432; Documentation — Solar Controller"
#define L_DOC_MATERIEL       "&#128268; Matériel"
#define L_DOC_CAPTEURS       "Capteurs"
#define L_DOC_SORTIES        "Sorties"
#define L_DOC_BADGES         "&#127968; Badges en-tête"
#define L_DOC_GRAPHIQUES     "&#128202; Graphiques"
#define L_DOC_LOGIQUE        "&#9889; Logique de charge"
#define L_DOC_PROTECTION     "&#128737; Protection anti-bootloop surcharge"
#define L_DOC_SOC            "&#128267; SOC — État de charge batterie"

// --- Validation formulaire (JS) ---
#define L_VAL_HEURE_SLEEP    "Heure sleep invalide (0-23)"
#define L_VAL_MIN_SLEEP      "Minutes sleep invalides (0-59)"
#define L_VAL_HEURE_REVEIL   "Heure réveil invalide (0-23)"
#define L_VAL_MIN_REVEIL     "Minutes réveil invalides (0-59)"
#define L_VAL_PLAGE_COURTE   "Plage de sommeil trop courte (min 2h)"

// --- Reset reasons (page debug) ---
#define L_RST_POWERON        "Power-on"
#define L_RST_EXT            "Reset externe (EN)"
#define L_RST_SW             "Reboot logiciel (ESP.restart)"
#define L_RST_PANIC          "PANIC / Exception (crash!)"
#define L_RST_INT_WDT        "Watchdog interruption (INT_WDT)"
#define L_RST_TASK_WDT       "Watchdog tache (TASK_WDT — loop bloquee!)"
#define L_RST_WDT            "Watchdog autre"
#define L_RST_DEEPSLEEP      "Reveil deep sleep (normal)"
#define L_RST_BROWNOUT       "Brownout (sous-tension alimentation!)"
#define L_RST_SDIO           "Reset SDIO"
#define L_RST_INCONNU        "Inconnu"

// --- Page AP config (premier démarrage) ---
#define L_AP_TITRE           "Configuration Solaire"
#define L_AP_BTN_SAVE        "Enregistrer"

// --- Page principale : titres sections ---
#define L_H3_PRESENT         "&#9201; Present"
#define L_H3_NOCTURNE        "&#127762; Etat Nocturne"
#define L_H3_ENERGIE         "&#128262; Energie 24h"
#define L_H3_BILAN           "&#9878; Bilan 30j"
#define L_H3_FUTUR           "&#128301; Futur"
#define L_H3_PASSE           "&#128197; Passe"

// --- Badge anti-bootloop (dynamique) ---
#define L_BADGE_BL_ACTIF     "&#9888; Anti-bootloop actif \xe2\x80\x94 MOSFET OFF encore %s"

// --- Hors MPPT inline ---
#define L_HORS_MPPT_INLINE   "&#9888; Hors MPPT &mdash; GND non commun &mdash; mesures invalides"

// --- Footer crédit ---
#define L_FOOTER_CREDIT      "v1.23f &mdash; R&eacute;alis&eacute; par"

// --- Config : hint plage luminosité ---
#define L_CFG_LUX_PLAGE_VAL  "Plage : "

// --- Debug page ---
#define L_DBG_HEAP           "Heap libre : "
#define L_DBG_OCTETS         " octets \xe2\x80\x94 "
#define L_DBG_UPTIME         "Uptime : "
#define L_DBG_WIFI           "WiFi : "
#define L_DBG_BL_CONFIRM     "D\xc3\xa9sactiver le blocage anti-bootloop ?"
#define L_DBG_BL_BTN         "&#128737; D\xc3\xa9sactiver anti-bootloop"
#define L_DBG_BL_OK          "&#10003; Blocage anti-bootloop d\xc3\xa9sactiv\xc3\xa9. Retour..."

// --- Debug bilan : tableau et boutons ---
#define L_DBG_BIL_FORCE      "&#9654; Forcer condenser"
#define L_DBG_BIL_TH_CASE    "Case"
#define L_DBG_BIL_TH_NOTE    "Note"
#define L_DBG_BIL_PROCHAIN   "prochain"
#define L_DBG_BIL_VALIDE     "<span class='ok'>valide</span>"
#define L_DBG_BIL_VIDE       "<span style='color:#333'>vide</span>"
#define L_DBG_BIL_AHIN       "<b>ahEntrants :</b>"
#define L_DBG_BIL_AHOUT      "<b>ahSortants :</b>"

// --- Documentation : hardware ---
#define L_DOC_VEML           "Luminosit\xc3\xa9 (lux) sur Wire0 GPIO 21/22 adresse 0x10"
#define L_DOC_INA            "Courant &amp; tension batterie sur Wire1 GPIO 19/23 adresse 0x40"
#define L_DOC_GPIO34         "Pont diviseur tension (ratio configurable)"
#define L_DOC_GPIO18         "MOSFET (commande charge)"
#define L_DOC_GPIO2          "LED indicateur (optionnel)"

// --- Documentation : badges ---
#define L_DOC_BADGE_V        "Tension batterie lue par pont diviseur (GPIO 34)"
#define L_DOC_BADGE_CHARGE   "\xc3\x89tat du MOSFET"
#define L_DOC_BADGE_LUX      "Luminosit\xc3\xa9 actuelle (VEML7700)"
#define L_DOC_BADGE_SEUIL    "Luminosit\xc3\xa9 minimale pour autoriser la charge (dynamique)"
#define L_DOC_BADGE_COUPURE  "Tension minimale avant coupure charge (dynamique)"
#define L_DOC_BADGE_SOC      "Ic\xc3\xb4ne batterie \xe2\x80\x94 SOC%% par coulomb counting INA226. <span class='badge green'>vert &gt;70%%</span> <span class='badge orange'>orange 30-70%%</span> <span class='badge red'>rouge &lt;30%%</span>. &#9889; = en charge, &#9660; = en d\xc3\xa9charge."
#define L_DOC_BADGE_HORS     "GND non commun d\xc3\xa9tect\xc3\xa9 \xe2\x80\x94 tension aberrante"
#define L_DOC_BADGE_BL       "Protection surcharge active \xe2\x80\x94 MOSFET bloqu\xc3\xa9 OFF"

// --- Documentation : graphiques ---
#define L_DOC_G_PRESENT      "<b>PR\xc3\x89SENT 24h</b> \xe2\x80\x94 Tension (V) et luminosit\xc3\xa9 (lux) sur les 24 derni\xc3\xa8res heures"
#define L_DOC_G_NOCTURNE     "<b>\xc3\x89TAT NOCTURNE</b> \xe2\x80\x94 Tension batterie pendant les r\xc3\xa9veils de nuit"
#define L_DOC_G_ENERGIE      "<b>\xc3\x89NERGIE 24h</b> \xe2\x80\x94 Courant (A) et puissance (W) INA226 sur 24h"
#define L_DOC_G_BILAN        "<b>BILAN 30j</b> \xe2\x80\x94 Ah entrants (charge solaire) et sortants (consommation) par jour"
#define L_DOC_G_FUTUR        "<b>FUTUR 7j</b> \xe2\x80\x94 Pr\xc3\xa9visions luminosit\xc3\xa9 OpenWeatherMap sur 7 jours"
#define L_DOC_G_PASSE        "<b>PASS\xc3\x89 30j</b> \xe2\x80\x94 Historique tension et luminosit\xc3\xa9 sur 30 jours"

// --- Documentation : logique charge ---
#define L_DOC_L_ON           "MOSFET <b>ON</b> si : lux &gt; Seuil ON <b>ET</b> tension &gt; Volt reprise <b>ET</b> batterie OK"
#define L_DOC_L_OFF          "MOSFET <b>OFF</b> si : lux &lt; 150lx <b>OU</b> tension &lt; Coupure <b>OU</b> batterie KO"
#define L_DOC_L_DYN          "Seuil lux et tension de coupure sont <b>dynamiques</b> \xe2\x80\x94 ajust\xc3\xa9s par la m\xc3\xa9t\xc3\xa9o OWM"

// --- Documentation : protection anti-bootloop ---
#define L_DOC_P_TITRE        "&#128737; Protection anti-bootloop surcharge"
#define L_DOC_P_TEXTE1       "Si le MPPT coupe l'alimentation suite \xc3\xa0 une surcharge (chute de tension), l'ESP32 reboot, remet le MOSFET HIGH, ce qui provoque \xc3\xa0 nouveau la surcharge \xe2\x80\x94 boucle infinie."
#define L_DOC_P_TEXTE2       "Protection : si <code>N boots</code> successifs sont d\xc3\xa9tect\xc3\xa9s sans p\xc3\xa9riode stable de <code>D\xc3\xa9lai anti-reboot</code> secondes, le MOSFET est bloqu\xc3\xa9 OFF pendant <code>Dur\xc3\xa9e blocage</code> (configurable). Le blocage persiste en NVS \xe2\x80\x94 survit \xc3\xa0 un power-on complet."

// --- Documentation : SOC ---
#define L_DOC_S_TITRE        "&#128267; SOC \xe2\x80\x94 \xc3\x89tat de charge batterie"
#define L_DOC_S_TEXTE1       "<b>SOC</b> (State Of Charge) = pourcentage de charge restant. Calcul\xc3\xa9 par <b>coulomb counting</b> : int\xc3\xa9gration du courant INA226 dans le temps (Ah entrants - Ah sortants) / capacit\xc3\xa9 totale."
#define L_DOC_S_TEXTE2       "Recal\xc3\xa9 \xc3\xa0 <b>100%%</b> automatiquement quand la tension d\xc3\xa9passe <code>Volt pleine</code>. Impr\xc3\xa9cis sans recalage r\xc3\xa9gulier \xe2\x80\x94 s'am\xc3\xa9liore avec l'usage."

// --- Documentation : cycle veille ---
#define L_DOC_V_TITRE        "&#128564; Cycle veille / r\xc3\xa9veil"
#define L_DOC_V_LI1          "Actif de <code>Heure r\xc3\xa9veil matin</code> \xc3\xa0 <code>Heure sommeil soir</code>"
#define L_DOC_V_LI2          "Nuit d\xc3\xa9coup\xc3\xa9e en <code>N relev\xc3\xa9s</code> tron\xc3\xa7ons \xe2\x80\x94 relev\xc3\xa9 tension \xc3\xa0 chaque r\xc3\xa9veil"
#define L_DOC_V_LI3          "Deep sleep entre les tron\xc3\xa7ons (~10\xc2\xb5A)"

// --- Documentation : version ---
#define L_DOC_VER_TITRE      "&#8505; Version"
#define L_DOC_VER_TEXTE      "Solar Controller <b>123f</b> \xe2\x80\x94 ESP32 + VEML7700 + INA226 + MPPT"

// --- Page /upd : mise à jour météo ---
#define L_UPD_TITRE          "Mise a jour meteo..."
#define L_UPD_SOUS_TITRE     "OWM + NASA en cours (~15s)"
#define L_UPD_TRAITEMENT     "Traitement en cours..."

// --- Pages de transition debug/bilan ---
#define L_DBG_BIL_FORCE_OK   "&#9654; Archivage en cours..."
#define L_DBG_BIL_UNDO_OK    "&#8635; Annulation en cours..."
#define L_DBG_BIL_RESET_OK   "&#128465; Reset en cours..."

// --- SVG : labels graphiques supplémentaires ---
#define L_SVG_JOUR_N         "J-%d"
#define L_SVG_AUJ            "Aujourd'hui"
#define L_SVG_AUJ_COURT      "auj."
#define L_SVG_APRES_JOURS    "(disponible apres plusieurs jours)"
#define L_CHARGEMENT         "chargement..."

// --- Menu : mise à jour météo ---
#define L_MENU_MAJ_METEO     "&#x27F3; MAJ M\xc3\xa9t\xc3\xa9o"

// --- Badge anti-bootloop : préfixe JS (sans %s, pour concat JS) ---
#define L_BADGE_BL_AVANT     "&#9888; Anti-bootloop actif \xe2\x80\x94 MOSFET OFF encore "

// --- Debug : libellés dernier reset ---
#define L_DBG_RST_CRASH_LBL  "&#9888; DERNIER RESET : "
#define L_DBG_RST_OK_LBL     "&#10003; Dernier reset : "

// --- Documentation : libellés badges (exemples inline) ---
#define L_DOC_LBL_CHARGE     "CHARGE ON/OFF"
#define L_DOC_LBL_SEUILON    "Seuil ON"
#define L_DOC_LBL_COUPURE    "Coupure"
