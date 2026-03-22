// =============================================================================
// Solar Controller — Language FR (Français)
// Pour changer de langue : remplacer #include "lang_fr.h" par #include "lang_en.h"
// =============================================================================
#pragma once

// --- SVG : titres des graphiques ---
#define L_SVG_PRESENT        "PRÉSENT — 24 dernières heures"
#define L_SVG_NOCTURNE       "ÉTAT NOCTURNE — tension batterie"
#define L_SVG_FUTUR          "PRÉVISION — 7 prochains jours (lux)"
#define L_SVG_PASSE          "HISTORIQUE — 30 derniers jours"
#define L_SVG_ENERGIE        "ÉNERGIE — Courant &amp; Puissance 24h"
#define L_SVG_BILAN          "BILAN 30j — Ah entrants / sortants"

// --- SVG : légendes ---
#define L_SVG_TENSION        "Tension (V)"
#define L_SVG_LUMINOSITE     "Luminosité (lux)"
#define L_SVG_TENSION_NUIT   "Tension nuit (V)"
#define L_SVG_COURANT        "Courant (A)"
#define L_SVG_PUISSANCE      "Puissance (W)"
#define L_SVG_ENTRANTS       "Entrants (Ah)"
#define L_SVG_SORTANTS       "Sortants (Ah)"
#define L_SVG_COUPURE        "coupure %.2fV"
#define L_SVG_REPRISE        "reprise"
#define L_SVG_SEUIL_ON       "Seuil ON"
#define L_SVG_ATTENTE        "En attente des premières mesures..."
#define L_SVG_MAJ            "(mis à jour toutes les heures)"
#define L_SVG_PAS_DONNEES    "Pas encore de données"
#define L_SVG_BARRE_VISIBLE  "(barre du jour visible dans %d min)"

// --- En-tête page principale ---
#define L_HDR_CHARGE_ON      "CHARGE : ON"
#define L_HDR_CHARGE_OFF     "CHARGE : OFF"
#define L_HDR_LUX            "Lux : "
#define L_HDR_SEUIL_ON       "Seuil ON : "
#define L_HDR_COUPURE        "Coupure : "
#define L_HDR_HORS_MPPT      "&#9888; Sans MPPT &mdash; GND non commun &mdash; mesures invalides"
#define L_HDR_TOAST          "☀️ Prévision mise à jour pour aujourd'hui"

// --- Sections page principale ---
#define L_SEC_PRESENT        "PRÉSENT"
#define L_SEC_NOCTURNE       "ÉTAT NOCTURNE"
#define L_SEC_ENERGIE        "ÉNERGIE 24H"
#define L_SEC_BILAN          "BILAN 30J"
#define L_SEC_FUTUR          "PRÉVISION"
#define L_SEC_PASSE          "HISTORIQUE"

// --- Menu hamburger ---
#define L_MENU_ACCUEIL       "&#8962; Accueil"
#define L_MENU_CONFIG        "&#9881; Configuration"
#define L_MENU_DEBUG         "&#128196; Debug"
#define L_MENU_DEBUG_BILAN   "&#128202; Debug Bilan"
#define L_MENU_DOC           "&#9432; Documentation"

// --- Boutons communs ---
#define L_BTN_RETOUR         "&#8592; Retour"
#define L_BTN_ENREGISTRER    "💾 Enregistrer"
#define L_BTN_ANNULER        "&#8635; Annuler le dernier"
#define L_BTN_RESET          "&#128465; Réinitialiser jour 0"
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
#define L_CFG_MASQUE         "Masque sous-réseau"
#define L_CFG_DNS            "DNS"
#define L_CFG_LAT            "Latitude"
#define L_CFG_LON            "Longitude"
#define L_CFG_TILT           "Inclinaison panneau"
#define L_CFG_TILT_UNIT      "° (0=plat, 90=vertical)"
#define L_CFG_AZIMUTH        "Azimut panneau"
#define L_CFG_AZIMUTH_UNIT   "° (180=Sud, 270=Ouest)"
#define L_CFG_RATIO          "Tension de sortie (charge)"
#define L_CFG_RATIO_HINT     "150k&#937; + 33k&#937; → (150+33)/33 = 5.5454"
#define L_CFG_SLEEP          "Heure de sommeil"
#define L_CFG_SLEEP_UNIT     "min (défaut 19:45)"
#define L_CFG_REVEIL         "Heure de réveil"
#define L_CFG_REVEIL_UNIT    "min (défaut 09:15)"
#define L_CFG_RELEVES        "Relevés nocturnes"
#define L_CFG_RELEVES_HINT   "Fenêtre nuit découpée en tranches égales. Ex : 19:45→09:15 avec 2 relevés = 3 tranches de ~4h26"
#define L_CFG_ARCHIVAGE      "Archivage bilan avant sommeil"
#define L_CFG_ARCHIVAGE_UNIT "h avant le sommeil (0=à l'heure du sommeil, 1-3h avant)"
#define L_CFG_ARCHIVAGE_HINT "Déclenche l'archivage N heures avant le sommeil. Recommandé : 1h"

// --- Config > Luminosité ---
#define L_CFG_LUX_MAX        "Seuil lux maximum (batterie faible + mauvais temps)"
#define L_CFG_LUX_MAX_HINT   "Seuil ON quand les conditions sont défavorables"
#define L_CFG_LUX_AMP        "Plage dynamique"
#define L_CFG_LUX_AMP_HINT   "Seuil ON minimum = Défaut - Plage (conditions idéales)"
#define L_CFG_LUX_CONF       "Lux de confiance météo 100%"
#define L_CFG_LUX_CONF_HINT  "Au-dessus de cette valeur prévue, la météo est fiable à 100%"
#define L_CFG_LUX_ACTUEL     "Seuil ON actuel : "
#define L_CFG_LUX_PLAGE      "Plage : "

// --- Config > Batterie ---
#define L_CFG_V_PLEINE       "Tension pleine charge"
#define L_CFG_V_REPRISE      "Tension de reprise"
#define L_CFG_V_REPRISE_HINT "Seuil au-dessus duquel la charge peut reprendre"
#define L_CFG_V_COUPURE      "Tension de coupure (beau temps)"
#define L_CFG_V_COUPURE_HINT "Seuil statique utilisé quand mauvais temps prévu"
#define L_CFG_V_CRITIQUE     "Tension critique"
#define L_CFG_V_PLANCHER     "Plancher absolu (jamais en dessous)"
#define L_CFG_V_PLANCHER_HINT "Plancher de coupure par beau temps — min 11.9V recommandé"
#define L_CFG_CAPACITE       "Capacité batterie (comptage coulombs INA226)"
#define L_CFG_CAPACITE_HINT  "Capacité nominale — ex : 11.5Ah pour batterie 12V/11.5Ah"
#define L_CFG_ANTIREBOOT     "Délai anti-surcharge"
#define L_CFG_ANTIREBOOT_HINT "Fenêtre bootloop (défaut 60s) — si 2 reboots sans période stable, MOSFET OFF 1h"
#define L_CFG_CADENCE        "Intervalle mesures / sauvegarde"
#define L_CFG_CADENCE_HINT   "Polling capteurs, comptage Ah et sauvegarde NVS. Défaut : 10 min"
#define L_CFG_BLOCAGE        "Durée blocage MOSFET sur surcharge"
#define L_CFG_COUPURE_DYN    "Coupure dynamique actuelle : "

// --- Badges d'alerte ---
#define L_BADGE_BLOCAGE      "&#9888; Anti-bootloop"
#define L_BADGE_HORS_MPPT    "&#9888; Sans MPPT"

// --- Page Debug Bilan ---
#define L_DBG_BILAN_TITRE    "&#128202; Debug Bilan 30j"
#define L_DBG_BILAN_AH_IN    "<b>ahIn :</b>"
#define L_DBG_BILAN_AH_OUT   "<b>ahOut :</b>"

// --- Page Debug logs ---
#define L_DBG_TITRE          "&#11035; Debug — journaux système"
#define L_DBG_REFRESH        "(rafraîchissement 60s)"

// --- Page Documentation ---
#define L_DOC_TITRE          "&#9432; Documentation — Contrôleur Solaire"
#define L_DOC_MATERIEL       "&#128268; Matériel"
#define L_DOC_CAPTEURS       "Capteurs"
#define L_DOC_SORTIES        "Sorties"
#define L_DOC_BADGES         "&#127968; Badges en-tête"
#define L_DOC_GRAPHIQUES     "&#128202; Graphiques"
#define L_DOC_LOGIQUE        "&#9889; Logique de charge"
#define L_DOC_PROTECTION     "&#128737; Protection anti-bootloop"
#define L_DOC_SOC            "&#128267; SOC — État de charge"

// --- Validation formulaire (JS) ---
#define L_VAL_HEURE_SLEEP    "Heure de sommeil invalide (0-23)"
#define L_VAL_MIN_SLEEP      "Minutes de sommeil invalides (0-59)"
#define L_VAL_HEURE_REVEIL   "Heure de réveil invalide (0-23)"
#define L_VAL_MIN_REVEIL     "Minutes de réveil invalides (0-59)"
#define L_VAL_PLAGE_COURTE   "Fenêtre de sommeil trop courte (min 2h)"

// --- Causes de reset (page debug) ---
#define L_RST_POWERON        "Mise sous tension"
#define L_RST_EXT            "Reset externe (EN)"
#define L_RST_SW             "Redémarrage logiciel (ESP.restart)"
#define L_RST_PANIC          "PANIC / Exception (plantage !)"
#define L_RST_INT_WDT        "Watchdog interruption (INT_WDT)"
#define L_RST_TASK_WDT       "Watchdog tâche (TASK_WDT — boucle bloquée !)"
#define L_RST_WDT            "Autre watchdog"
#define L_RST_DEEPSLEEP      "Réveil deep sleep (normal)"
#define L_RST_BROWNOUT       "Brownout (sous-tension alimentation !)"
#define L_RST_SDIO           "Reset SDIO"
#define L_RST_INCONNU        "Inconnu"

// --- Page AP config (premier démarrage) ---
#define L_AP_TITRE           "Configuration Solaire"
#define L_AP_BTN_SAVE        "Enregistrer"

// --- Page principale : titres sections ---
#define L_H3_PRESENT         "&#9201; Présent"
#define L_H3_NOCTURNE        "&#127762; État Nocturne"
#define L_H3_ENERGIE         "&#128262; Énergie 24h"
#define L_H3_BILAN           "&#9878; Bilan 30j"
#define L_H3_FUTUR           "&#128301; Prévision"
#define L_H3_PASSE           "&#128197; Historique"

// --- Badge anti-bootloop (dynamique) ---
#define L_BADGE_BL_ACTIF     "&#9888; Anti-bootloop actif \xe2\x80\x94 MOSFET OFF pendant %s"

// --- Hors-MPPT inline ---
#define L_HORS_MPPT_INLINE   "&#9888; Sans MPPT &mdash; GND non commun &mdash; mesures invalides"

// --- Pied de page ---
#define L_FOOTER_CREDIT      "v1.23f &mdash; Par"

// --- Config : indice plage luminosité ---
#define L_CFG_LUX_PLAGE_VAL  "Plage : "

// --- Page debug ---
#define L_DBG_HEAP           "Tas libre : "
#define L_DBG_OCTETS         " octets \xe2\x80\x94 "
#define L_DBG_UPTIME         "Uptime : "
#define L_DBG_WIFI           "WiFi : "
#define L_DBG_BL_CONFIRM     "Désactiver le verrou anti-bootloop ?"
#define L_DBG_BL_BTN         "&#128737; Désactiver anti-bootloop"
#define L_DBG_BL_OK          "&#10003; Verrou anti-bootloop désactivé. Retour..."

// --- Debug bilan : tableau et boutons ---
#define L_DBG_BIL_FORCE      "&#9654; Forcer l'archivage"
#define L_DBG_BIL_TH_CASE    "Case"
#define L_DBG_BIL_TH_NOTE    "Note"
#define L_DBG_BIL_PROCHAIN   "prochain"
#define L_DBG_BIL_VALIDE     "<span class='ok'>valide</span>"
#define L_DBG_BIL_VIDE       "<span style='color:#333'>vide</span>"
#define L_DBG_BIL_AHIN       "<b>Ah entrants :</b>"
#define L_DBG_BIL_AHOUT      "<b>Ah sortants :</b>"

// --- Documentation : matériel ---
#define L_DOC_VEML           "Luminosité (lux) sur Wire0 GPIO 21/22 adresse 0x10"
#define L_DOC_INA            "Courant &amp; tension batterie sur Wire1 GPIO 19/23 adresse 0x40"
#define L_DOC_GPIO34         "Pont diviseur de tension (ratio configurable)"
#define L_DOC_GPIO18         "MOSFET (contrôle de charge)"
#define L_DOC_GPIO2          "LED indicatrice (optionnelle)"

// --- Documentation : badges ---
#define L_DOC_BADGE_V        "Tension batterie lue par le diviseur (GPIO 34)"
#define L_DOC_BADGE_CHARGE   "État du MOSFET"
#define L_DOC_BADGE_LUX      "Luminosité actuelle (VEML7700)"
#define L_DOC_BADGE_SEUIL    "Luminosité minimale pour autoriser la charge (dynamique)"
#define L_DOC_BADGE_COUPURE  "Tension minimale avant coupure de charge (dynamique)"
#define L_DOC_BADGE_SOC      "Icône batterie \xe2\x80\x94 SOC%% par comptage Coulombs INA226. <span class='badge green'>vert &gt;70%%</span> <span class='badge orange'>orange 30-70%%</span> <span class='badge red'>rouge &lt;30%%</span>. &#9889; = charge, &#9660; = décharge."
#define L_DOC_BADGE_HORS     "GND commun non détecté \xe2\x80\x94 mesure de tension invalide"
#define L_DOC_BADGE_BL       "Protection surcharge active \xe2\x80\x94 MOSFET verrouillé OFF"

// --- Documentation : graphiques ---
#define L_DOC_G_PRESENT      "<b>PRÉSENT 24h</b> \xe2\x80\x94 Tension (V) et luminosité (lux) sur les 24 dernières heures"
#define L_DOC_G_NOCTURNE     "<b>ÉTAT NOCTURNE</b> \xe2\x80\x94 Tension batterie lors des réveils nocturnes"
#define L_DOC_G_ENERGIE      "<b>ÉNERGIE 24h</b> \xe2\x80\x94 Courant (A) et puissance (W) INA226 sur 24h"
#define L_DOC_G_BILAN        "<b>BILAN 30j</b> \xe2\x80\x94 Ah entrants (charge solaire) et sortants (consommation) par jour"
#define L_DOC_G_FUTUR        "<b>PRÉVISION 7j</b> \xe2\x80\x94 Prévision de luminosité OpenWeatherMap sur 7 jours"
#define L_DOC_G_PASSE        "<b>HISTORIQUE 30j</b> \xe2\x80\x94 Historique tension et luminosité sur 30 jours"

// --- Documentation : logique de charge ---
#define L_DOC_L_ON           "MOSFET <b>ON</b> si : lux &gt; Seuil ON <b>ET</b> tension &gt; Tension reprise <b>ET</b> batterie OK"
#define L_DOC_L_OFF          "MOSFET <b>OFF</b> si : lux &lt; 150lx <b>OU</b> tension &lt; Coupure <b>OU</b> batterie KO"
#define L_DOC_L_DYN          "Seuil lux et tension de coupure sont <b>dynamiques</b> \xe2\x80\x94 ajustés par la météo OWM"

// --- Documentation : protection anti-bootloop ---
#define L_DOC_P_TITRE        "&#128737; Protection anti-bootloop surcharge"
#define L_DOC_P_TEXTE1       "Si le MPPT coupe l'alimentation par surcharge (chute de tension), l'ESP32 redémarre, remet le MOSFET à HIGH, provoquant la même surcharge \xe2\x80\x94 boucle infinie."
#define L_DOC_P_TEXTE2       "Protection : si <code>N boots</code> consécutifs sont détectés sans période stable de <code>Délai surcharge</code> secondes, le MOSFET est verrouillé OFF pendant <code>Durée blocage</code> (configurable). Le verrou persiste en NVS \xe2\x80\x94 survit à une coupure totale."

// --- Documentation : SOC ---
#define L_DOC_S_TITRE        "&#128267; SOC \xe2\x80\x94 État de charge batterie"
#define L_DOC_S_TEXTE1       "<b>SOC</b> (State Of Charge) = pourcentage de charge restant. Calculé par <b>comptage de Coulombs</b> : intégration du courant INA226 dans le temps (Ah entrants - Ah sortants) / capacité totale."
#define L_DOC_S_TEXTE2       "Réinitialisé à <b>100%%</b> automatiquement quand la tension dépasse la <code>Tension pleine charge</code>. Imprécis sans recalibration régulière \xe2\x80\x94 s'améliore avec l'usage."

// --- Documentation : cycle veille ---
#define L_DOC_V_TITRE        "&#128564; Cycle veille / réveil"
#define L_DOC_V_LI1          "Actif de <code>Heure de réveil matin</code> à <code>Heure de sommeil soir</code>"
#define L_DOC_V_LI2          "Nuit découpée en <code>N relevés</code> tranches \xe2\x80\x94 lecture tension à chaque réveil"
#define L_DOC_V_LI3          "Deep sleep entre les tranches (~10\xc2\xb5A)"

// --- Documentation : version ---
#define L_DOC_VER_TITRE      "&#8505; Version"
#define L_DOC_VER_TEXTE      "Contrôleur Solaire <b>123f</b> \xe2\x80\x94 ESP32 + VEML7700 + INA226 + MPPT"

// --- Page /upd : mise à jour météo ---
#define L_UPD_TITRE          "Mise à jour météo..."
#define L_UPD_SOUS_TITRE     "OWM + NASA en cours (~15s)"
#define L_UPD_TRAITEMENT     "Traitement..."

// --- Pages de transition debug/bilan ---
#define L_DBG_BIL_FORCE_OK   "&#9654; Archivage..."
#define L_DBG_BIL_UNDO_OK    "&#8635; Annulation..."
#define L_DBG_BIL_RESET_OK   "&#128465; Réinitialisation..."

// --- SVG : étiquettes graphiques supplémentaires ---
#define L_SVG_JOUR_N         "J-%d"
#define L_SVG_AUJ            "Auj."
#define L_SVG_AUJ_COURT      "auj."
#define L_SVG_APRES_JOURS    "(disponible après plusieurs jours)"
#define L_CHARGEMENT         "chargement..."

// --- Menu : mise à jour météo ---
#define L_MENU_MAJ_METEO     "&#x27F3; Mettre à jour météo"

// --- Badge anti-bootloop : préfixe JS (sans %s, pour concat JS) ---
#define L_BADGE_BL_AVANT     "&#9888; Anti-bootloop actif \xe2\x80\x94 MOSFET OFF pendant "

// --- Debug : étiquettes dernier reset ---
#define L_DBG_RST_CRASH_LBL  "&#9888; DERNIER RESET : "
#define L_DBG_RST_OK_LBL     "&#10003; Dernier reset : "

// --- Documentation : étiquettes badges (exemples inline) ---
#define L_DOC_LBL_CHARGE     "CHARGE ON/OFF"
#define L_DOC_LBL_SEUILON    "Seuil ON"
#define L_DOC_LBL_COUPURE    "Coupure"
