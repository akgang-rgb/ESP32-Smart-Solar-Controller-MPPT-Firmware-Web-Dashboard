// Solar Controller 123f — FIX: suppression prefs.begin() orphelin dans setup court (crash tronçon 3 réveil matin).
// FIX: arCount anti-bootloop non incrémenté sur réveil timer (évite faux positif MOSFET bloqué au réveil jour).

// --- Langue / Language ---
#include "lang_fr.h"  //Replace with lang_en.h for English / or else (see lang repository : not tested)
#include <WiFi.h>
#include "mbedtls/aes.h"
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TimeLib.h>
#include <AsyncTCP.h>
#include <Preferences.h>
#include "Adafruit_VEML7700.h"
#include <Wire.h>
#include <INA226_WE.h>
#include "esp_sleep.h"
#include "driver/rtc_io.h"
#include "esp_task_wdt.h"    // Watchdog logiciel
#include "esp_system.h"      // esp_reset_reason()
#include "esp_mac.h"          // esp_read_mac()

#define WDT_TIMEOUT 30
#define HEAP_CRITIQUE 8000

// Prototypes (fonctions définies après setup/loop)
void chargerConfig();
void sauvegarderConfig();
void actualiserParametresDynamiques();
uint64_t dureeTroncon(int troncon);
bool estDansPlageSommeil(int h, int m);

// ==========================================================================
// DEEP SLEEP — RTC Memory (survit au deep sleep)
// ==========================================================================
// Variables persistées en mémoire RTC (4KB dispo, survit au deep sleep)
RTC_DATA_ATTR int rtc_owmDernierJour  = -1;
RTC_DATA_ATTR int rtc_indexH24        = 0;
RTC_DATA_ATTR int rtc_indexReleve     = 0;
RTC_DATA_ATTR int rtc_index30j        = 0;
RTC_DATA_ATTR int rtc_indexBilan30j   = 0;
RTC_DATA_ATTR float rtc_VOLT_COUPURE_DYN = 12.10;
RTC_DATA_ATTR float rtc_CURRENT_LUX_ON  = 900.0;
RTC_DATA_ATTR bool rtc_batterieEstOK    = true;
RTC_DATA_ATTR int   rtc_accC[3]           = {0,0,0};
RTC_DATA_ATTR float rtc_accV[3]           = {0,0,0};
RTC_DATA_ATTR float rtc_accL[3]           = {0,0,0};
RTC_DATA_ATTR float rtc_accI[3]           = {0,0,0};
RTC_DATA_ATTR int rtc_derHeureEch       = -1;
RTC_DATA_ATTR int rtc_derHeureEnr       = -1;
RTC_DATA_ATTR int rtc_nbReels           = 0;   // nb de relevés nocturnes réels (pour pente)
RTC_DATA_ATTR int rtc_troncon           = 0;   // numéro du dernier tronçon de sleep écoulé
// INA226 — ahEntrants/ahSortants persistés en NVS uniquement (pas besoin de RTC)
RTC_DATA_ATTR float rtc_socPct          = 50.0;// SOC% par coulomb counting
RTC_DATA_ATTR int   rtc_indexI24        = 0;   // index buffer courant 24h
RTC_DATA_ATTR float rtc_i24_Amp[24]    = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
RTC_DATA_ATTR int   rtc_bilCnt30j      = 0;   // nb de jours archivés (0..30)

// --- Détection boot loop (chute tension MPPT) ---
RTC_DATA_ATTR int64_t rtc_dernierBootUs  = 0;    // timestamp µs du dernier boot (esp_timer_get_time)
RTC_DATA_ATTR bool    rtc_mosfetWasHigh  = false; // MOSFET était HIGH au dernier boot
RTC_DATA_ATTR float   rtc_dernierCourant = 0.0;   // dernier courant mesuré avant reboot
RTC_DATA_ATTR int64_t rtc_mosfetBloqueJusquA = 0; // timestamp µs jusqu'auquel MOSFET reste LOW
                                                // va de 0 (init) à NB_RELEVES_NUIT+1
                                                // réveils intermédiaires tant que <= NB_RELEVES_NUIT
                                                // setup complet quand == NB_RELEVES_NUIT+1

// ==========================================================================
// 1. CONFIGURATION & VARIABLES
// ==========================================================================
const char* key = "SolaireSecret123"; 

const int LED_PIN = 2; 
const int BOOT_BUTTON_PIN = 0; 
const int MOSFET_PIN = 18; 
const int BAT_PIN    = 34;

unsigned long buttonPressStartTime = 0;
const float FILTRE_ANOMALIE_ANNEE = 0.4; 

// --- Paramètres configurables — chargés depuis NVS au démarrage ---
// Valeurs par défaut (utilisées si NVS vide)
float RATIO_PONT_DIVISEUR = 6.4211;  // Ajusté : 5.5454 × (12.53 / 10.82)

// Seuils batterie
float VOLT_PLEINE      = 13.50;
float VOLT_REPRISE     = 12.30;
float VOLT_COUPURE     = 12.10;
float VOLT_CRITIQUE    = 12.20;
float VOLT_COUPURE_MIN = 11.90;
float VOLT_COUPURE_DYN = 12.10; // calculé dynamiquement, pas persisté

// Seuils luminosité
float LUX_ON_DEFAUT    = 1500.0;
float LUX_ON_AMPLITUDE = 1400.0;
float LUX_CONFIANCE_MAX = 6000.0;

// Géolocalisation / panneau
float LATITUDE  = 43.4614;
float LONGITUDE = 1.2457;
int   TILT      = 90;
int   AZIMUTH   = 290;

// Horaires sleep
int NB_RELEVES_NUIT    = 2;    // nombre de relevés nocturnes (1 à 5) — tronçons calculés automatiquement
int HEURE_REVEIL_JOUR  = 9;
int MIN_REVEIL_JOUR    = 15;  // réveil 09h15 (WiFi box allumée à 09h00)
int HEURE_SOMMEIL_SOIR = 19;  // sleep à 19h45 (WiFi box coupe à 20h00)
int MIN_SOMMEIL_SOIR   = 45;

// Bilan 30j — archivage avant sleep
int AVANCE_CONDENSER = 1;  // heures avant sleep pour déclencher le condenser (0 = uniquement dans entrerDeepSleep)

// Clé OWM
const String OWM_KEY_DEFAUT = "bc96e2df041cdac5055493f20aa2f760";
String OWM_KEY = OWM_KEY_DEFAUT;

// IP statique ESP32 — configurables depuis l'interface web
String CFG_IP       = "192.168.1.253";
String CFG_GW       = "192.168.1.1";
String CFG_MASK     = "255.255.255.0";
String CFG_DNS      = "8.8.8.8";

// ==========================================================================
// 2. STRUCTURES & GLOBALES
// ==========================================================================
struct Prevision { float clouds[3]; float lux[3]; };
Prevision previsions[7];

// Sentinelle -1 : case non encore renseignée (buffer frais après flash/reset NVS).
// Le graphe PRÉSENT ignore ces cases pour ne pas tracer de valeurs aberrantes.
float h24_Volt[24] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
float h24_Lux[24]  = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int indexH24 = 0;
float historiqueTension[40]; // ~3 relevés/nuit × 13 nuits
int indexReleve = 0;

bool timeLog=false; //log de l' heure systeme
int lastGotIP = 0; //timestamp pile wifi ok pour demande meteo owm & nasa

struct JourneeSolaire { float vMatin, lMatin, vMidi, lMidi, vSoir, lSoir; };
JourneeSolaire historique30j[30];
int index30j = 0;

// Accumulateurs pour échantillonnage horaire (06h -> 21h)
float accV[3] = {0,0,0}, accL[3] = {0,0,0}; 
int accC[3] = {0,0,0};

float CURRENT_LUX_ON = 900.0;
bool batterieEstOK = true;
bool bilanFait     = false; // true dès l'archivage du jour, remis à false au réveil matin
int  derHeureCondenser = -1; // garde-fou : empêche double déclenchement du condenser avancé

// --- INA226 ---
float CAPACITE_BAT   = 11.5;  // Ah — configurable dans l'interface
int   DELAI_ANTI_REBOOT   = 60;   // secondes — fenêtre détection boot loop surcharge
int   DUREE_BLOCAGE_S     = 3600; // secondes — durée blocage MOSFET si boot loop détecté
int   INTERVALLE_CHECK_MIN = 10;  // minutes — cadence mesure/sauvegarde (10/15/30/45/60)
// Buffer courant 24h (miroir de h24_Volt)
float i24_Amp[24] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int   indexI24    = 0;
// Accumulateurs courant matin/midi/soir
float accI[3]     = {0,0,0};
// Bilan Ah journalier
volatile float ahEntrants  = 0.0;
volatile float ahSortants  = 0.0;
// SOC% par coulomb counting (0-100)
float socPct      = 50.0;
// Courant lu (volatile : écrit loop, lu callbacks)
volatile float courantDernier = 0.0;
// Historique 30j bilan Ah (entrants / sortants)
struct BilanJour { float ahIn; float ahOut; };
struct AhJour { float ahIn; float ahOut; }; // atomique pour NVS

// ==========================================================================
// STRUCTS NVS — 22 clés individuelles → 5 clés, fragmentation réduite
// ==========================================================================
struct NVSState {   // clé "st"   — toutes les 10 min (~76 bytes)
  float  socPct;
  float  ahIn, ahOut;
  float  accV[3], accL[3], accI[3];
  int    accC[3];
  uint8_t arMosfetHigh;
  float  arCourant;
  int    arCount;
  int    blocageS;
};
struct NVSH24 {     // clé "h24"  — toutes les heures (~1024 bytes)
  float volt[24], lux[24], amp[24];
  int   iH, iI24, dHEch;
  float jVMatin[30], jLMatin[30], jVMidi[30], jLMidi[30], jVSoir[30], jLSoir[30];
  int   idx30j;
};
struct NVSNuit {    // clé "nuit" — 1×/nuit (~172 bytes)
  float tension[40];
  int   idxReleve, nbReels, derHeureEnr;
};
struct NVSBilan {   // clé "bil"  — condenser (~249 bytes)
  BilanJour b30[30];
  int       idxBil, bilCnt;
  uint8_t   bilanFait;
};

BilanJour bilan30j[30];
int indexBilan30j = 0;  // prochain slot d'écriture
int bilCnt30j     = 0;  // nombre de jours effectivement archivés (0..30)
bool isGoingToSleep = false; //preparation mise en veille nocturne pour eviter les wifi reconnet
// FIX REVEIL ANTICIPE : armé au réveil deep sleep, désarmé dès que l'heure de réveil configurée est passée.
// Empêche entrerDeepSleep() de se redéclencher si le timer a sonné quelques minutes trop tôt.
bool ignorerSommeilJusquAuReveil = false;
volatile bool horsAlimentation = false;  // écrit Core1/loop(), lu Core0/callbacks — volatile requis
SemaphoreHandle_t xMutexMeteo = NULL; // Protège VOLT_COUPURE_DYN, CURRENT_LUX_ON, batterieEstOK, owmDernierJour, miseAJourEnCours
SemaphoreHandle_t xMutexLog   = NULL; // Protège logBuffer[] et logIndex — addLog() appelé des deux cores
SemaphoreHandle_t xMutexH24   = NULL;
bool miseAJourEnCours = false;         // true pendant la tâche meteoForecast — protégé par xMutexMeteo
volatile float luxDernier      = -1.0;  // écrit Core1/loop(), lu Core0/callbacks — volatile requis
volatile float tensionDerniere =  0.0;  // idem
volatile float inaVoltDerniere =  0.0;  // tension bus INA226 (côté batterie direct)
int nbReels = 0;// nb de relevés nocturnes valides accumulés (max 40)
volatile bool  demandeMeteo    = false; // écrit Core0/callback /upd, lu Core1/loop — volatile requis
volatile bool  forceCondenser  = false; // écrit Core0/callback /force, lu Core1/loop
volatile bool  forceUndo       = false; // écrit Core0/callback /undo,  lu Core1/loop
volatile bool  forceReset      = false; // écrit Core0/callback /reset, lu Core1/loop
int owmDernierJour = -1;
int derHeureEch = -1; 
int derHeureEnr = -1;
int derHeureSynchro = -1;
bool wifiOK =false;

// Buffer de logs pour la page /debug — 30 lignes circulaires
// Tableau fixe char (pas de String) : zéro allocation heap, zéro fragmentation
#define LOG_MAX 50
#define LOG_LINE 90  // 80 + 6 pour la date DD/MM
char logBuffer[LOG_MAX][LOG_LINE];  // 50 × 90 = 4500 bytes statiques
int logIndex = 0;
Preferences prefs;

void sauvegarderLogs();  // forward declaration

// Un flag dirty est levé ; la sauvegarde se fait en batch dans loop() toutes les 60s.
// Avantage : les callbacks AsyncWebServer (Core 0) ne bloquent plus le bus Flash.
volatile bool logsDirty = false;

void addLog(const char* msg) {
  struct tm ti; char ts[18];
  if (getLocalTime(&ti)) snprintf(ts, sizeof(ts), "%02d/%02d %02d:%02d:%02d", ti.tm_mday, ti.tm_mon+1, ti.tm_hour, ti.tm_min, ti.tm_sec);
  else strcpy(ts, "--/-- --:--:--");

  if (xMutexLog) xSemaphoreTake(xMutexLog, portMAX_DELAY);
  snprintf(logBuffer[logIndex], LOG_LINE, "%s %s", ts, msg);
  logIndex = (logIndex + 1) % LOG_MAX;
  logsDirty = true;   // sera sauvegardé en NVS par le prochain tick de lastMem (loop)
  if (xMutexLog) xSemaphoreGive(xMutexLog);
}

// Surcharge String pour compatibilité avec les appels existants
void addLog(String msg) { addLog(msg.c_str()); }

// Sauvegarde le buffer en NVS — appelé uniquement depuis loop() (pas depuis callbacks)
void sauvegarderLogs() {
  prefs.begin("logs", false);
  prefs.putInt("lidx", logIndex);
  prefs.putBytes("lbuf", logBuffer, sizeof(logBuffer));
  prefs.end();
  logsDirty = false;
}

// Charge les logs NVS au démarrage — permet de voir ce qui s'est passé avant un crash
void chargerLogs() {
  prefs.begin("logs", true);
  int savedIdx = prefs.getInt("lidx", 0);
  size_t loaded = prefs.getBytes("lbuf", logBuffer, sizeof(logBuffer));
  prefs.end();
  if (loaded == sizeof(logBuffer)) logIndex = savedIdx;
  else memset(logBuffer, 0, sizeof(logBuffer)); // NVS vide ou taille incorrecte
}

// Retourne une description lisible de la cause du dernier reset
String getCrashReason() {
  switch (esp_reset_reason()) {
    case ESP_RST_POWERON:   return L_RST_POWERON;
    case ESP_RST_EXT:       return L_RST_EXT;
    case ESP_RST_SW:        return L_RST_SW;
    case ESP_RST_PANIC:     return L_RST_PANIC;
    case ESP_RST_INT_WDT:   return L_RST_INT_WDT;
    case ESP_RST_TASK_WDT:  return L_RST_TASK_WDT;
    case ESP_RST_WDT:       return L_RST_WDT;
    case ESP_RST_DEEPSLEEP: return L_RST_DEEPSLEEP;
    case ESP_RST_BROWNOUT:  return L_RST_BROWNOUT;
    case ESP_RST_SDIO:      return L_RST_SDIO;
    default:                return L_RST_INCONNU;
  }
}

bool serverRunning = false;
bool wifiConnecting = false;
unsigned long lastWifiAttempt = 0;

INA226_WE ina;
Adafruit_VEML7700 veml = Adafruit_VEML7700();
bool vemlOK = false;

// INA226 sur Wire1 (SDA=19, SCL=23) — séparé du VEML7700 (Wire0 par défaut)
//INA226_WE ina = INA226_WE(0x40);
bool inaOK = false;

AsyncWebServer server(80);
DNSServer dnsServer;
StaticJsonDocument<200> nasaFilter;

// ==========================================================================
// 3. CRYPTOGRAPHIE & LOGIQUE MÉTIER
// ==========================================================================
// W2 FIX : encryptLong accepte maintenant un IV externe (généré aléatoirement par l'appelant)
void encryptLong(String plain, unsigned char* output, unsigned char* iv) {
    mbedtls_aes_context aes;
    unsigned char iv_copy[16];
    memcpy(iv_copy, iv, 16); // AES-CBC modifie l'IV en place, on travaille sur une copie
    unsigned char input[64] = {0};
    plain.toCharArray((char*)input, 64);
    mbedtls_aes_init(&aes);
    mbedtls_aes_setkey_enc(&aes, (const unsigned char*)key, 128);
    mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, 64, iv_copy, input, output);
    mbedtls_aes_free(&aes);
}

// W2 FIX : decryptLong accepte maintenant l'IV persisté en NVS
String decryptLong(unsigned char* cipher, unsigned char* iv) {
    mbedtls_aes_context aes;
    unsigned char iv_copy[16];
    memcpy(iv_copy, iv, 16);
    unsigned char output[64] = {0};
    mbedtls_aes_init(&aes);
    mbedtls_aes_setkey_dec(&aes, (const unsigned char*)key, 128);
    mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, 64, iv_copy, cipher, output);
    mbedtls_aes_free(&aes);
    output[63] = '\0'; 
    return String((char*)output);
}

float calcLux(float np_moyenne, float pct_cloudy) {
  return np_moyenne * (1.0 - (pct_cloudy / 100.0) * 0.85) * 10.0;
}

// --------------------------------------------------------------------------
// INA226 — getCourant()
// Retourne le courant en Ampères (positif = charge, négatif = décharge).
// Shunt R010 (0.01Ω), plage ±20A, adresse 0x40, Wire1 (SDA=19, SCL=23).
// Retourne 0.0 si le capteur n'est pas disponible.
// --------------------------------------------------------------------------
float getCourant() {
  if (!inaOK) return 0.0;
  return ina.getCurrent_A();
}

// Mise à jour SOC% par coulomb counting (appelée toutes les 10min = 600s)
void majSOC(float courant_A, float v) {
  float DT_H = (float)INTERVALLE_CHECK_MIN / 60.0f; // intervalle en heures
  float dAh = courant_A * DT_H;
  if (dAh > 0) ahEntrants  += dAh;
  else          ahSortants  += (-dAh);
  socPct += (dAh / CAPACITE_BAT) * 100.0;
  socPct  = constrain(socPct, 0.0, 100.0);
  // Reset à 100% quand la batterie est pleine
  if (v >= VOLT_PLEINE) socPct = 100.0;
}

float calculerMoyenneBatterie() {
  float somme = 0; int compte = 0;
  for (int i = 0; i < 40; i++) {
    if (historiqueTension[i] > 7.0) { somme += historiqueTension[i]; compte++; }
  }
  return (compte > 0) ? (somme / compte) : VOLT_CRITIQUE;
}

// --------------------------------------------------------------------------
// PENTE — Régression linéaire sur les 10 derniers relevés nocturnes
// Retourne la pente en V/relevé (positif = batterie remonte, négatif = descend)
// Identique à l'algorithme du Nano, adapté au buffer circulaire ESP32.
// --------------------------------------------------------------------------

float calculerPenteTrend() {
  if (nbReels < 10) return 0.0;

  // Collecte les 10 derniers relevés valides — skip les invalides (pas de valeur de repli)
  float valeurs[10];
  int count = 0;
  for (int i = 0; i < 10; i++) {
    int idx = (indexReleve - 1 - i + 40) % 40;
    float v = historiqueTension[idx];
    if (isnan(v) || v < 7.0f) continue;
    valeurs[count++] = v;
  }
  if (count < 5) return 0.0; // pas assez de points valides

  // Régression linéaire sur count points (x_moy et den recalculés dynamiquement)
  float x_moy = (count - 1) / 2.0f;
  float y_moy = 0.0f;
  for (int i = 0; i < count; i++) y_moy += valeurs[i];
  y_moy /= count;

  float num = 0.0f, den2 = 0.0f;
  for (int i = 0; i < count; i++) {
    float x = float(i);
    num  += (x - x_moy) * (valeurs[i] - y_moy);
    den2 += (x - x_moy) * (x - x_moy);
  }
  if (den2 == 0.0f) return 0.0f;
  return num / den2;
}

void actualiserParametresDynamiques() {
  float moy = calculerMoyenneBatterie();
  batterieEstOK = (moy > VOLT_COUPURE_DYN);
  float ratioSante = constrain((moy - VOLT_COUPURE) / (VOLT_PLEINE - VOLT_COUPURE), 0.0, 1.0);
  // Confiance météo : moyenne pondérée des lux prévus sur 7 jours
  // Poids décroissants : J+1=7, J+2=6, ..., J+7=1 (le plus proche compte le plus)
  float luxPondere = 0; float poidsTotal = 0;
  for (int d = 0; d < 7; d++) {
    float poids = float(7 - d); // 7, 6, 5, 4, 3, 2, 1
    float luxMoyJour = (previsions[d].lux[0] + previsions[d].lux[1] + previsions[d].lux[2]) / 3.0;
    luxPondere  += luxMoyJour * poids;
    poidsTotal  += poids;
  }
  if (poidsTotal <= 0.0f) poidsTotal = 1.0f; // garde-fou division par zéro
  float ratioConfiance = constrain((luxPondere / poidsTotal) / LUX_CONFIANCE_MAX, 0.0, 1.0);
  float scoreGlobal = (ratioSante * 0.7) + (ratioConfiance * 0.3);
  CURRENT_LUX_ON = LUX_ON_DEFAUT - (scoreGlobal * LUX_ON_AMPLITUDE);

  // Seuil de coupure dynamique :
  // Bonne météo (scoreGlobal≈1) → on peut descendre jusqu'à VOLT_COUPURE_MIN (11.9V)
  // Mauvaise météo (scoreGlobal≈0) → on reste sur VOLT_COUPURE (12.1V), le seuil prudent.
  // La recharge sera rapide ou lente selon les prévisions.
  VOLT_COUPURE_DYN = VOLT_COUPURE - (scoreGlobal * (VOLT_COUPURE - VOLT_COUPURE_MIN));
  // Sécurité : jamais en dessous du plancher absolu, jamais au dessus du seuil statique
  VOLT_COUPURE_DYN = constrain(VOLT_COUPURE_DYN, VOLT_COUPURE_MIN, VOLT_COUPURE);

  // --- CORRECTION PENTE NOCTURNE (hérité du Nano) ---
  // Régression linéaire sur les 10 derniers relevés nocturnes.
  // Batterie qui descend vite → on remonte le seuil de coupure, quelle que soit la météo.
  // Batterie qui remonte  → on peut se permettre d'être plus permissif.
  float pente = calculerPenteTrend();
  if (nbReels >= 10) {
    if (pente > 0.005f) {
      // Tendance hausse → légère détente (−0.05V max)
      VOLT_COUPURE_DYN = max(VOLT_COUPURE_MIN, VOLT_COUPURE_DYN - 0.05f);
    } else if (pente < -0.005f) {
      // Tendance baisse → on resserre (+0.10V max, plafonné à VOLT_COUPURE)
      VOLT_COUPURE_DYN = min(VOLT_COUPURE, VOLT_COUPURE_DYN + 0.10f);
    }
  }

  Serial.printf("[DYN] scoreGlobal=%.2f  pente=%.4fV/rel  VOLT_COUPURE_DYN=%.2fV  CURRENT_LUX_ON=%.0f lx\n",
                scoreGlobal, pente, VOLT_COUPURE_DYN, CURRENT_LUX_ON);
}

void condenserEtSauvegarderJournee() {
    addLog("[BILAN] condenser ahIn=" + String(ahEntrants,3) + " ahOut=" + String(ahSortants,3));

    // --- JourneeSolaire (V + lux matin/midi/soir) ---
    JourneeSolaire j;
    j.vMatin = (accC[0] > 0) ? accV[0]/accC[0] : 0;
    j.lMatin = (accC[0] > 0) ? accL[0]/accC[0] : 0;
    j.vMidi  = (accC[1] > 0) ? accV[1]/accC[1] : 0;
    j.lMidi  = (accC[1] > 0) ? accL[1]/accC[1] : 0;
    j.vSoir  = (accC[2] > 0) ? accV[2]/accC[2] : 0;
    j.lSoir  = (accC[2] > 0) ? accL[2]/accC[2] : 0;
    historique30j[index30j] = j;
    index30j = (index30j + 1) % 30;

    // --- BilanJour (Ah entrants / sortants) ---
    bilan30j[indexBilan30j] = { ahEntrants, ahSortants };
    indexBilan30j = (indexBilan30j + 1) % 30;
    if (bilCnt30j < 30) bilCnt30j++;

    // Reset accumulateurs journaliers
    ahEntrants = 0.0; ahSortants = 0.0;
    for (int i = 0; i < 3; i++) { accV[i]=0; accL[i]=0; accC[i]=0; accI[i]=0; }

    // Persist NVS — structs NVSBilan + NVSState reset + NVSH24
    { NVSBilan _b; memcpy(_b.b30, bilan30j, sizeof(bilan30j));
      _b.idxBil=indexBilan30j; _b.bilCnt=bilCnt30j; _b.bilanFait=1;
      prefs.begin("solar", false); prefs.putBytes("bil",&_b,sizeof(_b)); prefs.end(); }
    // NVSState : reset ah+acc, garder socPct+ar
    { NVSState _s={}; prefs.begin("solar",true); prefs.getBytes("st",&_s,sizeof(_s)); prefs.end();
      _s.ahIn=0.0f; _s.ahOut=0.0f; _s.socPct=socPct;
      for(int i=0;i<3;i++){_s.accV[i]=0;_s.accL[i]=0;_s.accI[i]=0;_s.accC[i]=0;}
      prefs.begin("solar", false); prefs.putBytes("st",&_s,sizeof(_s)); prefs.end(); }
    // NVSH24 : mise à jour h30j + idx30j
    { NVSH24 _h={}; prefs.begin("solar",true); prefs.getBytes("h24",&_h,sizeof(_h)); prefs.end();
      for(int i=0;i<30;i++){_h.jVMatin[i]=historique30j[i].vMatin;_h.jLMatin[i]=historique30j[i].lMatin;
        _h.jVMidi[i]=historique30j[i].vMidi;_h.jLMidi[i]=historique30j[i].lMidi;
        _h.jVSoir[i]=historique30j[i].vSoir;_h.jLSoir[i]=historique30j[i].lSoir;}
      _h.idx30j=index30j;
      prefs.begin("solar", false); prefs.putBytes("h24",&_h,sizeof(_h)); prefs.end(); }
}
// ==========================================================================
// GRAPHIQUES SVG
// Dimensions communes : viewBox 0 0 300 130
//   Zone de tracé : x=[30..295] y=[5..105] => w=265, h=100
//   Zone légende  : y=[115..130]
// Axes : graduations Volt (gauche) et Lux (droite)
// ==========================================================================

// Helpers de mise en page
static const int GW = 265; // largeur zone tracé
static const int GH = 100; // hauteur zone tracé
static const int GX = 30;  // origine x
static const int GY = 5;   // origine y (haut)

// Convertit une tension (11.0-15.0V) en coordonnée Y SVG
float voltToY(float v) {
  return GY + GH - constrain((v - 11.0) / 4.0, 0.0, 1.0) * GH;
}
// Convertit un lux (0-8000) en coordonnée Y SVG
float luxToY(float l) {
  return GY + GH - constrain(l / 8000.0, 0.0, 1.0) * GH;
}

// ==========================================================================
// GRAPHIQUES SVG — version streaming (Print*)
// Chaque générateur écrit directement dans le flux HTTP via Print*
// => zéro String alloué sur le heap, zéro fragmentation, zéro crash WiFi
// ==========================================================================

// Cadre + axes communs (Volt gauche, Lux droite)
void svgDebut(Print* out, const char* titre) {
  char b[256];
  out->print("<svg viewBox='0 0 320 135' style='background:#0d0d0d;border-radius:8px;width:100%;max-width:500px;display:block;'>"
             "<style>.dot text{visibility:hidden;pointer-events:none;}.dot:hover text{visibility:visible;}.dot circle{cursor:pointer;}</style>");
  snprintf(b, sizeof(b), "<text x='150' y='16' text-anchor='middle' fill='#888' font-size='8' font-family='monospace'>%s</text>", titre);
  out->print(b);
  snprintf(b, sizeof(b), "<rect x='%d' y='%d' width='%d' height='%d' fill='#111' rx='3'/>", GX, GY+18, GW, GH-18);
  out->print(b);
  for (int i = 0; i <= 4; i++) {
    float yg = GY + GH - (float(i) / 4.0f) * GH;
    snprintf(b, sizeof(b), "<line x1='%d' y1='%.1f' x2='%d' y2='%.1f' stroke='#222' stroke-width='0.5'/>", GX, yg, GX+GW, yg);
    out->print(b);
    snprintf(b, sizeof(b), "<text x='%d' y='%.1f' text-anchor='end' fill='#ff8c00' font-size='6' font-family='monospace'>%dV</text>", GX-3, yg+3, 11+i);
    out->print(b);
  }
  for (int i = 0; i <= 4; i++) {
    float yg = GY + GH - (float(i) / 4.0f) * GH;
    int lv = i * 2000;
    if (lv >= 1000) snprintf(b, sizeof(b), "<text x='%d' y='%.1f' text-anchor='start' fill='#ffeb3b' font-size='6' font-family='monospace'>%dk</text>", GX+GW+3, yg+3, lv/1000);
    else            snprintf(b, sizeof(b), "<text x='%d' y='%.1f' text-anchor='start' fill='#ffeb3b' font-size='6' font-family='monospace'>%d</text>",  GX+GW+3, yg+3, lv);
    out->print(b);
  }
  out->print("<rect x='30' y='120' width='8' height='3' fill='#ff8c00'/>"
             "<text x='41' y='123' fill='#ff8c00' font-size='7' font-family='monospace'>" L_SVG_TENSION "</text>"
             "<rect x='130' y='120' width='8' height='3' fill='#ffeb3b'/>"
             "<text x='141' y='123' fill='#ffeb3b' font-size='7' font-family='monospace'>" L_SVG_LUMINOSITE "</text>");
}

// --------------------------------------------------------------------------
// GRAPHIQUE 1 : PRÉSENT — 24 dernières heures
// --------------------------------------------------------------------------
void genererSVG_Present(Print* out) {
  char b[320];
  if (xMutexH24) xSemaphoreTake(xMutexH24, portMAX_DELAY);

  // Pré-scan : y a-t-il au moins un point valide ?
  bool auMoinsUnPoint = false;
  for (int i = 0; i < 24; i++) { if (h24_Volt[(indexH24+i)%24] > 9.0) { auMoinsUnPoint = true; break; } }

  svgDebut(out, L_SVG_PRESENT);
  int hAct = hour();

  if (!auMoinsUnPoint) {
    out->print("<text x='150' y='75' text-anchor='middle' fill='#555' font-size='8' font-family='monospace'>" L_SVG_ATTENTE "</text>"
               "<text x='150' y='88' text-anchor='middle' fill='#444' font-size='7' font-family='monospace'>" L_SVG_MAJ "</text>");
  } else {
    // Lux polyline
    bool hasLux = false;
    for (int i = 0; i < 24; i++) { if (h24_Lux[(indexH24+i)%24] > 0) { hasLux = true; break; } }
    if (hasLux) {
      out->print("<polyline fill='none' stroke='#ffeb3b' stroke-width='1' opacity='0.7' points='");
      for (int i = 0; i < 24; i++) {
        int idx = (indexH24+i)%24;
        if (h24_Lux[idx] > 0) { snprintf(b, sizeof(b), "%.1f,%.1f ", GX+(float(i)/23.0f)*GW, luxToY(h24_Lux[idx])); out->print(b); }
      }
      out->print("'/>");
    }
    // Volt polyline
    out->print("<polyline fill='none' stroke='#ff8c00' stroke-width='1.5' points='");
    for (int i = 0; i < 24; i++) {
      int idx = (indexH24+i)%24;
      if (h24_Volt[idx] > 9.0) { snprintf(b, sizeof(b), "%.1f,%.1f ", GX+(float(i)/23.0f)*GW, voltToY(h24_Volt[idx])); out->print(b); }
    }
    out->print("'/>");

    // Dots Volt avec label au survol
    for (int i = 0; i < 24; i++) {
      int idx = (indexH24+i)%24;
      if (h24_Volt[idx] > 9.0) {
        float cx = GX+(float(i)/23.0f)*GW;
        float cy = voltToY(h24_Volt[idx]);
        float ly = (cy < GY+14) ? cy+10 : cy-5;
        snprintf(b, sizeof(b), "<g class='dot'><circle cx='%.1f' cy='%.1f' r='2' fill='#ff8c00' stroke='#0d0d0d' stroke-width='1'/>"
                               "<text x='%.1f' y='%.1f' text-anchor='middle' fill='#ff8c00' font-size='8' font-family='monospace'>%.2fV</text></g>",
                 cx, cy, cx, ly, h24_Volt[idx]);
        out->print(b);
      }
    }
    // Dots Lux avec label au survol
    if (hasLux) {
      for (int i = 0; i < 24; i++) {
        int idx = (indexH24+i)%24;
        if (h24_Lux[idx] > 0) {
          float cx = GX+(float(i)/23.0f)*GW;
          float cy = luxToY(h24_Lux[idx]);
          float ly = (cy < GY+14) ? cy+10 : cy-5;
          snprintf(b, sizeof(b), "<g class='dot'><circle cx='%.1f' cy='%.1f' r='1.5' fill='#ffeb3b' stroke='#0d0d0d' stroke-width='1'/>"
                                 "<text x='%.1f' y='%.1f' text-anchor='middle' fill='#ffeb3b' font-size='8' font-family='monospace'>%.0flx</text></g>",
                   cx, cy, cx, ly, h24_Lux[idx]);
          out->print(b);
        }
      }
    }
  }

  // Axe X : n'afficher le label d'heure que si le slot a une donnée ou est le slot actuel
  // Évite d'afficher des heures nocturnes vides sans donnée correspondante
  // Calcul dynamique des étiquettes de l'axe X pour sauter la période de sleep
  int heures_axe[24];
  int hr = hAct;
  
  // Si l'heure actuelle tombe pendant le sommeil, on part de la dernière heure enregistrée avant de dormir
  bool inSleepNow = false;
  if (HEURE_SOMMEIL_SOIR < HEURE_REVEIL_JOUR) {
      if (hr > HEURE_SOMMEIL_SOIR && hr < HEURE_REVEIL_JOUR) inSleepNow = true;
  } else { // Cas rare où le réveil est avant minuit et le sommeil après minuit
      if (hr > HEURE_SOMMEIL_SOIR || hr < HEURE_REVEIL_JOUR) inSleepNow = true;
  }
  if (inSleepNow) hr = HEURE_SOMMEIL_SOIR;

  // On recule dans le temps pour les 24 slots (de 23 jusqu'à 0)
  for (int k = 23; k >= 0; k--) {
      heures_axe[k] = hr;
      hr--; // Heure précédente
      if (hr < 0) hr = 23; // Gestion de minuit
      
      // On vérifie si la nouvelle heure (hr) tombe dans la période de sommeil
      bool isSleep = false;
      if (HEURE_SOMMEIL_SOIR < HEURE_REVEIL_JOUR) {
          if (hr > HEURE_SOMMEIL_SOIR && hr < HEURE_REVEIL_JOUR) isSleep = true;
      } else {
          if (hr > HEURE_SOMMEIL_SOIR || hr < HEURE_REVEIL_JOUR) isSleep = true;
      }
      
      // Si on entre dans la zone de sommeil en reculant, on "saute" directement au soir précédent
      if (isSleep) {
          hr = HEURE_SOMMEIL_SOIR;
      }
  }

  // Axe X : n'afficher le label d'heure que si le slot a une donnée ou est le slot actuel
  for (int i = 0; i < 24; i++) {
    int idx = (indexH24+i)%24;
    bool aData = (h24_Volt[idx] > 9.0 || h24_Lux[idx] > 0);
    bool estMarque = (i % 6 == 0); // tous les 6 slots on force un repère
    
    if (!aData && !estMarque) continue; // slot vide + pas un repère → on saute
    
    float x = GX + (float(i)/23.0f)*GW;
    
    // On utilise notre tableau dynamique calculé précédemment !
    int hLabel = heures_axe[i]; 
    
    char colorStr[10] = "#2a2a2a"; // Gris très sombre par défaut (sans donnée)
    
    if (aData) {
        // Si l'heure est dans la fenêtre de relevé jour
        if (hLabel >= HEURE_REVEIL_JOUR && hLabel <= HEURE_SOMMEIL_SOIR && HEURE_SOMMEIL_SOIR > HEURE_REVEIL_JOUR) {
            float ratio = (float)(hLabel - HEURE_REVEIL_JOUR) / (float)(HEURE_SOMMEIL_SOIR - HEURE_REVEIL_JOUR);
            int r = 255, g, b_col;
            
            if (ratio <= 0.5) {
                // Première moitié : Jaune (255, 235, 59) vers Blanc (255, 255, 255)
                float localRatio = ratio * 2.0;
                g = 235 + (int)(20 * localRatio);
                b_col = 59 + (int)(196 * localRatio);
            } else {
                // Seconde moitié : Blanc (255, 255, 255) vers Rouge (255, 59, 59)
                float localRatio = (ratio - 0.5) * 2.0;
                g = 255 - (int)(196 * localRatio);
                b_col = 255 - (int)(196 * localRatio);
            }
            snprintf(colorStr, sizeof(colorStr), "#%02x%02x%02x", r, g, b_col);
        } else {
            // Heures hors de la plage jour avec des données (par ex. relevés nocturnes)
            snprintf(colorStr, sizeof(colorStr), "#888888");
        }
    }

    snprintf(b, sizeof(b), "<text x='%.1f' y='%d' text-anchor='middle' fill='%s' font-size='6' font-family='monospace'>%dh</text>", x, GY+GH+10, colorStr, hLabel);
    out->print(b);
    snprintf(b, sizeof(b), "<line x1='%.1f' y1='%d' x2='%.1f' y2='%d' stroke='#1a1a1a' stroke-width='0.5'/>", x, GY, x, GY+GH);
    out->print(b);
  }
  
  snprintf(b, sizeof(b), "<line x1='%d' y1='%.1f' x2='%d' y2='%.1f' stroke='#ff3b3b' stroke-width='0.7' stroke-dasharray='3,2'/>", GX, voltToY(VOLT_COUPURE_DYN), GX+GW, voltToY(VOLT_COUPURE_DYN));
  out->print(b);
  snprintf(b, sizeof(b), "<text x='%d' y='%.1f' fill='#ff3b3b' font-size='5.5' font-family='monospace'>" L_SVG_COUPURE "</text>", GX+2, voltToY(VOLT_COUPURE_DYN)-2, VOLT_COUPURE_DYN);
  out->print(b);
  snprintf(b, sizeof(b), "<line x1='%d' y1='%.1f' x2='%d' y2='%.1f' stroke='#00c896' stroke-width='0.7' stroke-dasharray='3,2'/>", GX, voltToY(VOLT_REPRISE), GX+GW, voltToY(VOLT_REPRISE));
  out->print(b);
  snprintf(b, sizeof(b), "<text x='%d' y='%.1f' fill='#00c896' font-size='5.5' font-family='monospace'>" L_SVG_REPRISE "</text>", GX+2, voltToY(VOLT_REPRISE)-2);
  out->print(b);
  out->print("</svg>");

  if (xMutexH24) xSemaphoreGive(xMutexH24);
}

// --------------------------------------------------------------------------
// GRAPHIQUE 1b : ÉTAT NOCTURNE
// --------------------------------------------------------------------------
void genererSVG_Nuit(Print* out) {
  char b[320];
  out->print("<svg viewBox='0 0 320 135' style='background:#0d0d0d;border-radius:8px;width:100%;max-width:500px;display:block;'>"
             "<style>.dot text{visibility:hidden;pointer-events:none;}.dot:hover text{visibility:visible;}.dot circle{cursor:pointer;}</style>"
             "<text x='150' y='16' text-anchor='middle' fill='#888' font-size='8' font-family='monospace'>" L_SVG_NOCTURNE "</text>");
  snprintf(b, sizeof(b), "<rect x='%d' y='%d' width='%d' height='%d' fill='#111' rx='3'/>", GX, GY+18, GW, GH-18);
  out->print(b);
  for (int i = 0; i <= 4; i++) {
    float yg = GY + GH - (float(i)/4.0f)*GH;
    snprintf(b, sizeof(b), "<line x1='%d' y1='%.1f' x2='%d' y2='%.1f' stroke='#222' stroke-width='0.5'/>", GX, yg, GX+GW, yg);
    out->print(b);
    snprintf(b, sizeof(b), "<text x='%d' y='%.1f' text-anchor='end' fill='#ff8c00' font-size='6' font-family='monospace'>%dV</text>", GX-3, yg+3, 11+i);
    out->print(b);
  }

  // Collect valid points into stack array (no heap)
  float vPoints[40]; int nPoints = 0;
  for (int i = 0; i < 40; i++) {
    int idx = (indexReleve+1+i)%40;
    if (historiqueTension[idx] > 7.0) vPoints[nPoints++] = historiqueTension[idx];
  }

  if (nPoints < 2) {
    out->print("<text x='150' y='65' text-anchor='middle' fill='#444' font-size='9' font-family='monospace'>" L_SVG_PAS_DONNEES "</text>");
    snprintf(b,sizeof(b),"<text x='150' y='77' text-anchor='middle' fill='#333' font-size='7' font-family='monospace'>" L_SVG_BARRE_VISIBLE "</text>",INTERVALLE_CHECK_MIN);
    out->print(b);
  } else {
    out->print("<polyline fill='none' stroke='#4dabf7' stroke-width='1.5' points='");
    for (int i = 0; i < nPoints; i++) {
      snprintf(b, sizeof(b), "%.1f,%.1f ", GX+(float(i)/float(nPoints-1))*GW, voltToY(vPoints[i]));
      out->print(b);
    }
    out->print("'/>");
    // Dots avec label au survol — tous les points nuit
    for (int i = 0; i < nPoints; i++) {
      float cx = GX+(float(i)/float(nPoints-1))*GW;
      float cy = voltToY(vPoints[i]);
      float ly = (cy < GY+14) ? cy+10 : cy-5;
      snprintf(b, sizeof(b), "<g class='dot'><circle cx='%.1f' cy='%.1f' r='2' fill='#4dabf7' stroke='#0d0d0d' stroke-width='1'/>"
                             "<text x='%.1f' y='%.1f' text-anchor='middle' fill='#4dabf7' font-size='8' font-family='monospace'>%.2fV</text></g>",
               cx, cy, cx, ly, vPoints[i]);
      out->print(b);
    }
    for (int i = 0; i < nPoints; i += 9) {
      float x = GX+(float(i)/float(nPoints-1))*GW;
      snprintf(b, sizeof(b), "<text x='%.1f' y='%d' text-anchor='middle' fill='#555' font-size='6' font-family='monospace'>-%dn</text>", x, GY+GH+10, (nPoints-1-i)/3);
      out->print(b);
      snprintf(b, sizeof(b), "<line x1='%.1f' y1='%d' x2='%.1f' y2='%d' stroke='#1a1a1a' stroke-width='0.5'/>", x, GY, x, GY+GH);
      out->print(b);
    }
  }
  snprintf(b, sizeof(b), "<line x1='%d' y1='%.1f' x2='%d' y2='%.1f' stroke='#ff3b3b' stroke-width='0.7' stroke-dasharray='3,2'/>", GX, voltToY(VOLT_COUPURE_DYN), GX+GW, voltToY(VOLT_COUPURE_DYN));
  out->print(b);
  snprintf(b, sizeof(b), "<text x='%d' y='%.1f' fill='#ff3b3b' font-size='5.5' font-family='monospace'>coupure</text>", GX+2, voltToY(VOLT_COUPURE_DYN)-2);
  out->print(b);
  out->print("<rect x='30' y='120' width='8' height='3' fill='#4dabf7'/>"
             "<text x='41' y='123' fill='#4dabf7' font-size='7' font-family='monospace'>" L_SVG_TENSION_NUIT "</text>"
             "</svg>");
}

// --------------------------------------------------------------------------
// GRAPHIQUE 2 : FUTUR — Prévisions 7 jours
// --------------------------------------------------------------------------
void genererSVG_Futur(Print* out) {
  const int N = 7;
  const float barW = float(GW) / (N * 3 + N);
  const char* labels[7] = {"J+1","J+2","J+3","J+4","J+5","J+6","J+7"};
  const char* couleurs[3] = {"#4dabf7","#ffeb3b","#ff8c00"};
  char b[256];

  float luxMax = 2000.0;
  for (int d = 0; d < N; d++)
    for (int t = 0; t < 3; t++)
      if (previsions[d].lux[t] > luxMax) luxMax = previsions[d].lux[t];
  luxMax = ceil(luxMax / 1000.0f) * 1000.0f;

  out->print("<svg viewBox='0 0 320 135' style='background:#0d0d0d;border-radius:8px;width:100%;max-width:500px;display:block;'>"
             "<text x='150' y='16' text-anchor='middle' fill='#888' font-size='8' font-family='monospace'>" L_SVG_FUTUR "</text>");
  snprintf(b, sizeof(b), "<rect x='%d' y='%d' width='%d' height='%d' fill='#111' rx='3'/>", GX, GY+18, GW, GH-18);
  out->print(b);

  for (int i = 0; i <= 4; i++) {
    float val = (luxMax/4.0f)*i;
    float yg  = GY+GH-(float(i)/4.0f)*GH;
    snprintf(b, sizeof(b), "<line x1='%d' y1='%.1f' x2='%d' y2='%.1f' stroke='#222' stroke-width='0.5'/>", GX, yg, GX+GW, yg);
    out->print(b);
    if (val >= 1000) snprintf(b, sizeof(b), "<text x='%d' y='%.1f' text-anchor='end' fill='#ffeb3b' font-size='6' font-family='monospace'>%dk</text>", GX-3, yg+3, (int)(val/1000));
    else             snprintf(b, sizeof(b), "<text x='%d' y='%.1f' text-anchor='end' fill='#ffeb3b' font-size='6' font-family='monospace'>%d</text>",  GX-3, yg+3, (int)val);
    out->print(b);
  }

  for (int d = 0; d < N; d++) {
    float xBase = GX+(float(d)/N)*GW+barW*0.5f;
    for (int t = 0; t < 3; t++) {
      float bh = constrain(previsions[d].lux[t]/luxMax, 0.0f, 1.0f)*GH;
      snprintf(b, sizeof(b), "<rect x='%.1f' y='%.1f' width='%.1f' height='%.1f' fill='%s' opacity='0.8' rx='1'/>",
               xBase+t*(barW+1), GY+GH-bh, barW, bh, couleurs[t]);
      out->print(b);
    }
    snprintf(b, sizeof(b), "<text x='%.1f' y='%d' text-anchor='middle' fill='#555' font-size='6' font-family='monospace'>%s</text>", xBase+barW+1, GY+GH+10, labels[d]);
    out->print(b);
    if (d > 0) {
      snprintf(b, sizeof(b), "<line x1='%.1f' y1='%d' x2='%.1f' y2='%d' stroke='#1a1a1a' stroke-width='0.5'/>", GX+(float(d)/N)*GW, GY, GX+(float(d)/N)*GW, GY+GH);
      out->print(b);
    }
  }

  float yLuxOn = GY+GH-constrain(CURRENT_LUX_ON/luxMax, 0.0f, 1.0f)*GH;
  snprintf(b, sizeof(b), "<line x1='%d' y1='%.1f' x2='%d' y2='%.1f' stroke='#00c896' stroke-width='0.8' stroke-dasharray='4,2'/>", GX, yLuxOn, GX+GW, yLuxOn);
  out->print(b);
  snprintf(b, sizeof(b), "<text x='%d' y='%.1f' fill='#00c896' font-size='5.5' font-family='monospace'>" L_SVG_SEUIL_ON "</text>", GX+2, yLuxOn-2);
  out->print(b);
  out->print("<rect x='30'  y='120' width='8' height='3' fill='#4dabf7'/><text x='41'  y='123' fill='#4dabf7' font-size='7' font-family='monospace'>Matin</text>"
             "<rect x='85'  y='120' width='8' height='3' fill='#ffeb3b'/><text x='96'  y='123' fill='#ffeb3b' font-size='7' font-family='monospace'>Midi</text>"
             "<rect x='130' y='120' width='8' height='3' fill='#ff8c00'/><text x='141' y='123' fill='#ff8c00' font-size='7' font-family='monospace'>Soir</text>"
             "</svg>");
}

// --------------------------------------------------------------------------
// GRAPHIQUE 3 : PASSÉ — Historique 30 jours
// --------------------------------------------------------------------------
void genererSVG_Passe(Print* out) {
  char b[320];

  // Pré-scan pour savoir si on a des données avant de commencer les polylines
  int count = 0;
  for (int i = 0; i < 30; i++) {
    int idx = (index30j+i)%30;
    JourneeSolaire& j = historique30j[idx];
    if (j.vMatin>0||j.vMidi>0||j.vSoir>0||j.lMatin>0||j.lMidi>0||j.lSoir>0) count++;
  }

  svgDebut(out, L_SVG_PASSE);

  // Labels X (avant les polylines pour ne pas interférer avec l'ordre SVG)
  for (int i = 0; i < 30; i++) {
    int idx = (index30j+i)%30;
    JourneeSolaire& j = historique30j[idx];
    bool hasData = (j.vMatin>0||j.vMidi>0||j.vSoir>0||j.lMatin>0||j.lMidi>0||j.lSoir>0);
    if (!hasData) continue;
    if (i % 5 == 0) {
      float x = GX+(float(i)/29.0f)*GW;
      snprintf(b, sizeof(b), "<text x='%.1f' y='%d' text-anchor='middle' fill='#555' font-size='6' font-family='monospace'>" L_SVG_JOUR_N "</text>", x, GY+GH+10, 30-i);
      out->print(b);
      snprintf(b, sizeof(b), "<line x1='%.1f' y1='%d' x2='%.1f' y2='%d' stroke='#1a1a1a' stroke-width='0.5'/>", x, GY, x, GY+GH);
      out->print(b);
    }
  }

  if (count < 2) {
    out->print("<text x='150' y='60' text-anchor='middle' fill='#444' font-size='9' font-family='monospace'>" L_SVG_PAS_DONNEES "</text>"
               "<text x='150' y='72' text-anchor='middle' fill='#333' font-size='7' font-family='monospace'>" L_SVG_APRES_JOURS "</text>");
  } else {
    // Lux polyline
    bool hasLux = false;
    for (int i = 0; i < 30; i++) { JourneeSolaire& j=historique30j[(index30j+i)%30]; if(j.lMatin>0||j.lMidi>0||j.lSoir>0){hasLux=true;break;} }
    if (hasLux) {
      out->print("<polyline fill='none' stroke='#ffeb3b' stroke-width='1' opacity='0.7' points='");
      for (int i = 0; i < 30; i++) {
        int idx=(index30j+i)%30; JourneeSolaire& j=historique30j[idx];
        float lMoy=0; int nL=0;
        if(j.lMatin>0){lMoy+=j.lMatin;nL++;} if(j.lMidi>0){lMoy+=j.lMidi;nL++;} if(j.lSoir>0){lMoy+=j.lSoir;nL++;}
        if(nL>0){snprintf(b,sizeof(b),"%.1f,%.1f ",GX+(float(i)/29.0f)*GW,luxToY(lMoy/nL));out->print(b);}
      }
      out->print("'/>");
    }
    // Volt polyline
    out->print("<polyline fill='none' stroke='#ff8c00' stroke-width='1.5' points='");
    for (int i = 0; i < 30; i++) {
      int idx=(index30j+i)%30; JourneeSolaire& j=historique30j[idx];
      float vMoy=0; int nV=0;
      if(j.vMatin>0){vMoy+=j.vMatin;nV++;} if(j.vMidi>0){vMoy+=j.vMidi;nV++;} if(j.vSoir>0){vMoy+=j.vSoir;nV++;}
      if(nV>0){snprintf(b,sizeof(b),"%.1f,%.1f ",GX+(float(i)/29.0f)*GW,voltToY(vMoy/nV));out->print(b);}
    }
    out->print("'/>");

    // Dots Volt 30j avec label au survol
    for (int i = 0; i < 30; i++) {
      int idx=(index30j+i)%30; JourneeSolaire& j=historique30j[idx];
      float vMoy=0; int nV=0;
      if(j.vMatin>0){vMoy+=j.vMatin;nV++;} if(j.vMidi>0){vMoy+=j.vMidi;nV++;} if(j.vSoir>0){vMoy+=j.vSoir;nV++;}
      if(nV>0){
        float cx=GX+(float(i)/29.0f)*GW, cy=voltToY(vMoy/nV);
        float ly=(cy<GY+14)?cy+10:cy-5;
        snprintf(b,sizeof(b),"<g class='dot'><circle cx='%.1f' cy='%.1f' r='2' fill='#ff8c00' stroke='#0d0d0d' stroke-width='1'/>"
                             "<text x='%.1f' y='%.1f' text-anchor='middle' fill='#ff8c00' font-size='8' font-family='monospace'>%.2fV</text></g>",
                 cx,cy,cx,ly,vMoy/nV);
        out->print(b);
      }
    }
    // Dots Lux 30j avec label au survol
    if (hasLux) {
      for (int i = 0; i < 30; i++) {
        int idx=(index30j+i)%30; JourneeSolaire& j=historique30j[idx];
        float lMoy=0; int nL=0;
        if(j.lMatin>0){lMoy+=j.lMatin;nL++;} if(j.lMidi>0){lMoy+=j.lMidi;nL++;} if(j.lSoir>0){lMoy+=j.lSoir;nL++;}
        if(nL>0){
          float cx=GX+(float(i)/29.0f)*GW, cy=luxToY(lMoy/nL);
          float ly=(cy<GY+14)?cy+10:cy-5;
          snprintf(b,sizeof(b),"<g class='dot'><circle cx='%.1f' cy='%.1f' r='1.5' fill='#ffeb3b' stroke='#0d0d0d' stroke-width='1'/>"
                               "<text x='%.1f' y='%.1f' text-anchor='middle' fill='#ffeb3b' font-size='8' font-family='monospace'>%.0flx</text></g>",
                   cx,cy,cx,ly,lMoy/nL);
          out->print(b);
        }
      }
    }
  }

  snprintf(b, sizeof(b), "<line x1='%d' y1='%.1f' x2='%d' y2='%.1f' stroke='#ff3b3b' stroke-width='0.7' stroke-dasharray='3,2'/>", GX, voltToY(VOLT_COUPURE), GX+GW, voltToY(VOLT_COUPURE));
  out->print(b);
  snprintf(b, sizeof(b), "<line x1='%d' y1='%.1f' x2='%d' y2='%.1f' stroke='#00c896' stroke-width='0.7' stroke-dasharray='3,2'/>", GX, voltToY(VOLT_REPRISE), GX+GW, voltToY(VOLT_REPRISE));
  out->print(b);
  out->print("</svg>");
}

// --------------------------------------------------------------------------
// GRAPHIQUE 5 : ÉNERGIE 24h — courant (A) et puissance (W)
// --------------------------------------------------------------------------
void genererSVG_Energie(Print* out) {
  char b[320];
  out->print("<svg viewBox='0 0 320 135' style='background:#0d0d0d;border-radius:8px;width:100%;max-width:500px;display:block;'>"
             "<style>.dot text{visibility:hidden;pointer-events:none;}.dot:hover text{visibility:visible;}.dot circle{cursor:pointer;}</style>");
  out->print("<text x='150' y='16' text-anchor='middle' fill='#888' font-size='8' font-family='monospace'>" L_SVG_ENERGIE "</text>");
  snprintf(b, sizeof(b), "<rect x='%d' y='%d' width='%d' height='%d' fill='#111' rx='3'/>", GX, GY+18, GW, GH-18);
  out->print(b);

  // Pré-scan : max courant pour axe dynamique
  bool auMoinsUn = false;
  float ampMax = 0.0f;
  for (int i = 0; i < 24; i++) {
    float a = i24_Amp[(indexI24+i)%24];
    if (a >= 0) { auMoinsUn = true; if (a > ampMax) ampMax = a; }
  }
  // Arrondir au palier supérieur : 1, 2, 5, 10, 20A
  float ampScale;
  if      (ampMax <= 1.0f)  ampScale = 1.0f;
  else if (ampMax <= 2.0f)  ampScale = 2.0f;
  else if (ampMax <= 5.0f)  ampScale = 5.0f;
  else if (ampMax <= 10.0f) ampScale = 10.0f;
  else                      ampScale = 20.0f;
  float vRef = (tensionDerniere > 9.0) ? tensionDerniere : 12.0;
  float wScale = ampScale * vRef;

  // Axes graduations courant (gauche) et puissance (droite) — dynamiques
  for (int i = 0; i <= 4; i++) {
    float yg = GY + GH - (float(i)/4.0f)*GH;
    float aVal = (ampScale/4.0f)*i;
    float wVal = (wScale/4.0f)*i;
    snprintf(b, sizeof(b), "<line x1='%d' y1='%.1f' x2='%d' y2='%.1f' stroke='#222' stroke-width='0.5'/>", GX, yg, GX+GW, yg);
    out->print(b);
    snprintf(b, sizeof(b), "<text x='%d' y='%.1f' text-anchor='end' fill='#4dabf7' font-size='6' font-family='monospace'>%.2gA</text>", GX-3, yg+3, aVal);
    out->print(b);
    snprintf(b, sizeof(b), "<text x='%d' y='%.1f' text-anchor='start' fill='#a9e34b' font-size='6' font-family='monospace'>%.0fW</text>", GX+GW+3, yg+3, wVal);
    out->print(b);
  }

  // Helpers locaux : A→Y et W→Y
  auto ampToY = [ampScale](float a) -> float {
    return GY + GH - constrain(a / ampScale, 0.0f, 1.0f) * GH;
  };

  if (!auMoinsUn) {
    out->print("<text x='150' y='75' text-anchor='middle' fill='#555' font-size='8' font-family='monospace'>" L_SVG_ATTENTE "</text>");
  } else {
    // Polyline puissance (W)
    bool hasPow = false;
    out->print("<polyline fill='none' stroke='#a9e34b' stroke-width='1' opacity='0.7' points='");
    for (int i = 0; i < 24; i++) {
      int idx = (indexI24+i)%24;
      float amp = i24_Amp[idx];
      if (amp >= 0) {
        float w = amp * vRef;
        float y = GY + GH - constrain(w / wScale, 0.0f, 1.0f) * GH;
        snprintf(b, sizeof(b), "%.1f,%.1f ", GX+(float(i)/23.0f)*GW, y);
        out->print(b);
        hasPow = true;
      }
    }
    out->print("'/>");

    // Polyline courant (A)
    out->print("<polyline fill='none' stroke='#4dabf7' stroke-width='1.5' points='");
    for (int i = 0; i < 24; i++) {
      int idx = (indexI24+i)%24;
      if (i24_Amp[idx] >= 0) {
        snprintf(b, sizeof(b), "%.1f,%.1f ", GX+(float(i)/23.0f)*GW, ampToY(i24_Amp[idx]));
        out->print(b);
      }
    }
    out->print("'/>");

    // Dots courant avec label au survol
    for (int i = 0; i < 24; i++) {
      int idx = (indexI24+i)%24;
      if (i24_Amp[idx] >= 0) {
        float cx = GX+(float(i)/23.0f)*GW;
        float cy = ampToY(i24_Amp[idx]);
        float ly = (cy < GY+14) ? cy+10 : cy-5;
        float wPt = i24_Amp[idx] * vRef;
        snprintf(b, sizeof(b),
          "<g class='dot'><circle cx='%.1f' cy='%.1f' r='2' fill='#4dabf7' stroke='#0d0d0d' stroke-width='1'/>"
          "<text x='%.1f' y='%.1f' text-anchor='middle' fill='#4dabf7' font-size='8' font-family='monospace'>%.1fA / %.0fW</text></g>",
          cx, cy, cx, ly, i24_Amp[idx], wPt);
        out->print(b);
      }
    }
  }

  // Légende
  out->print("<rect x='30' y='120' width='8' height='3' fill='#4dabf7'/>"
             "<text x='41' y='123' fill='#4dabf7' font-size='7' font-family='monospace'>" L_SVG_COURANT "</text>"
             "<rect x='130' y='120' width='8' height='3' fill='#a9e34b'/>"
             "<text x='141' y='123' fill='#a9e34b' font-size='7' font-family='monospace'>" L_SVG_PUISSANCE "</text>");

  // Axe X dynamique — même logique que PRESENT
  int hAct = hour();
  int heures_axe[24];
  int hr = hAct;
  bool inSleepNow = false;
  if (HEURE_SOMMEIL_SOIR < HEURE_REVEIL_JOUR) {
    if (hr > HEURE_SOMMEIL_SOIR && hr < HEURE_REVEIL_JOUR) inSleepNow = true;
  } else {
    if (hr > HEURE_SOMMEIL_SOIR || hr < HEURE_REVEIL_JOUR) inSleepNow = true;
  }
  if (inSleepNow) hr = HEURE_SOMMEIL_SOIR;
  for (int k = 23; k >= 0; k--) {
    heures_axe[k] = hr;
    hr--;
    if (hr < 0) hr = 23;
    bool isSleep = false;
    if (HEURE_SOMMEIL_SOIR < HEURE_REVEIL_JOUR) {
      if (hr > HEURE_SOMMEIL_SOIR && hr < HEURE_REVEIL_JOUR) isSleep = true;
    } else {
      if (hr > HEURE_SOMMEIL_SOIR || hr < HEURE_REVEIL_JOUR) isSleep = true;
    }
    if (isSleep) hr = HEURE_SOMMEIL_SOIR;
  }
  for (int i = 0; i < 24; i++) {
    int idx = (indexI24+i)%24;
    bool aData = (i24_Amp[idx] >= 0);
    bool estMarque = (i % 6 == 0);
    if (!aData && !estMarque) continue;
    float x = GX + (float(i)/23.0f)*GW;
    int hLabel = heures_axe[i];
    char colorStr[10] = "#2a2a2a";
    if (aData) {
      if (hLabel >= HEURE_REVEIL_JOUR && hLabel <= HEURE_SOMMEIL_SOIR && HEURE_SOMMEIL_SOIR > HEURE_REVEIL_JOUR) {
        float ratio = (float)(hLabel - HEURE_REVEIL_JOUR) / (float)(HEURE_SOMMEIL_SOIR - HEURE_REVEIL_JOUR);
        int r = 255, g, b_col;
        if (ratio <= 0.5) {
          float lr = ratio * 2.0;
          g = 235 + (int)(20 * lr); b_col = 59 + (int)(196 * lr);
        } else {
          float lr = (ratio - 0.5) * 2.0;
          g = 255 - (int)(196 * lr); b_col = 255 - (int)(196 * lr);
        }
        snprintf(colorStr, sizeof(colorStr), "#%02x%02x%02x", r, g, b_col);
      } else {
        snprintf(colorStr, sizeof(colorStr), "#888888");
      }
    }
    snprintf(b, sizeof(b), "<text x='%.1f' y='%d' text-anchor='middle' fill='%s' font-size='6' font-family='monospace'>%dh</text>", x, GY+GH+10, colorStr, hLabel);
    out->print(b);
    snprintf(b, sizeof(b), "<line x1='%.1f' y1='%d' x2='%.1f' y2='%d' stroke='#1a1a1a' stroke-width='0.5'/>", x, GY, x, GY+GH);
    out->print(b);
  }

  out->print("</svg>");
}

// --------------------------------------------------------------------------
// GRAPHIQUE 6 : BILAN 30j — Ah entrants (vert) / sortants (rouge)
//
// Rendu :  bilCnt30j barres archivées (les plus anciennes à gauche)
//          + 1 barre "aujourd'hui en cours" hachurée si !bilanFait
//          bilCnt30j est autoritaire — jamais de comptage à la volée.
// --------------------------------------------------------------------------
void genererSVG_Bilan(Print* out) {
  char b[320];

  // --- Nombre total de colonnes affichées ---
  bool afficherEnCours = (ahEntrants > 0.001 || ahSortants > 0.001);
  int  nbCols = bilCnt30j + (afficherEnCours ? 1 : 0);
  if (nbCols == 0) {
    out->print("<svg viewBox='0 0 320 135' style='background:#0d0d0d;border-radius:8px;width:100%;max-width:500px;display:block;'>");
    out->print("<text x='150' y='16' text-anchor='middle' fill='#888' font-size='8' font-family='monospace'>" L_SVG_BILAN "</text>");
    snprintf(b, sizeof(b), "<rect x='%d' y='%d' width='%d' height='%d' fill='#111' rx='3'/>", GX, GY+18, GW, GH-18);
    out->print(b);
    out->print("<text x='150' y='65' text-anchor='middle' fill='#444' font-size='9' font-family='monospace'>" L_SVG_PAS_DONNEES "</text>");
    snprintf(b,sizeof(b),"<text x='150' y='77' text-anchor='middle' fill='#333' font-size='7' font-family='monospace'>" L_SVG_BARRE_VISIBLE "</text></svg>",INTERVALLE_CHECK_MIN);
    out->print(b);
    return;
  }

  // --- Calcul échelle Ah dynamique ---
  // Trouver le vrai max sur les données disponibles (plancher 1 Ah)
  float ahMax = 1.0;
  for (int i = 0; i < bilCnt30j; i++) {
    int idx = (indexBilan30j - bilCnt30j + i + 30) % 30;
    if (bilan30j[idx].ahIn  > ahMax) ahMax = bilan30j[idx].ahIn;
    if (bilan30j[idx].ahOut > ahMax) ahMax = bilan30j[idx].ahOut;
  }
  if (afficherEnCours) {
    if (ahEntrants > ahMax) ahMax = ahEntrants;
    if (ahSortants > ahMax) ahMax = ahSortants;
  }
  // Step "propre" : 0.5 / 1 / 2 / 5 / 10 / 20 Ah selon magnitude
  float step;
  if      (ahMax <= 2.0f)  step = 0.5f;
  else if (ahMax <= 4.0f)  step = 1.0f;
  else if (ahMax <= 10.0f) step = 2.0f;
  else if (ahMax <= 20.0f) step = 5.0f;
  else if (ahMax <= 40.0f) step = 10.0f;
  else                      step = 20.0f;
  // 4 graduations au-dessus de 0 — ahMax arrondi au step supérieur
  ahMax = ceil(ahMax / step) * step;
  // S'assurer qu'on a au moins 4 graduations visibles
  if (ahMax / step < 4) ahMax = step * 4;

  // --- SVG header + fond + graduations ---
  out->print("<svg viewBox='0 0 320 135' style='background:#0d0d0d;border-radius:8px;width:100%;max-width:500px;display:block;'>"
             "<style>"
             "@keyframes pulseL{0%,100%{opacity:0.35}50%{opacity:0.75}}"
             "@keyframes pulseH{0%,100%{opacity:0.6}50%{opacity:1.0}}"
             ".pL{animation:pulseL 2s ease-in-out infinite;}"
             ".pH{animation:pulseH 2s ease-in-out infinite;}"
             "</style>");
  out->print("<text x='150' y='16' text-anchor='middle' fill='#888' font-size='8' font-family='monospace'>" L_SVG_BILAN "</text>");
  snprintf(b, sizeof(b), "<rect x='%d' y='%d' width='%d' height='%d' fill='#111' rx='3'/>", GX, GY+18, GW, GH-18);
  out->print(b);
  int nbGrad = (int)(ahMax / step); // nombre de graduations (4 en général)
  for (int i = 0; i <= nbGrad; i++) {
    float val = step * i;
    float yg  = GY + GH - (val / ahMax) * GH;
    snprintf(b, sizeof(b), "<line x1='%d' y1='%.1f' x2='%d' y2='%.1f' stroke='#222' stroke-width='0.5'/>", GX, yg, GX+GW, yg);
    out->print(b);
    // Label : 1 décimale si step < 1, sinon entier
    if (step < 1.0f)
      snprintf(b, sizeof(b), "<text x='%d' y='%.1f' text-anchor='end' fill='#888' font-size='6' font-family='monospace'>%.1f</text>", GX-3, yg+3, val);
    else
      snprintf(b, sizeof(b), "<text x='%d' y='%.1f' text-anchor='end' fill='#888' font-size='6' font-family='monospace'>%.0f</text>", GX-3, yg+3, val);
    out->print(b);
  }

  // --- Barres archivées ---
  // Disposition : J0 (aujourd'hui) = slot NCOLS_MAX-1 (extrême droite)
  //               J-1 (hier)       = slot NCOLS_MAX-2
  //               J-N (le plus ancien) = slot NCOLS_MAX-1-bilCnt30j
  // i=0 → plus ancien, i=bilCnt30j-1 → hier
  const int NCOLS_MAX = 31;
  float colW = float(GW) / NCOLS_MAX;
  float barW = colW * 0.38f;

  for (int i = 0; i < bilCnt30j; i++) {
    int idx   = (indexBilan30j - bilCnt30j + i + 30) % 30;
    BilanJour& bj = bilan30j[idx];
    // i=0 (plus ancien) → slot NCOLS_MAX-1-bilCnt30j
    // i=bilCnt30j-1 (hier) → slot NCOLS_MAX-2
    float xBase = GX + float(NCOLS_MAX - 1 - bilCnt30j + i) * colW + colW * 0.08f;
    int age = bilCnt30j - i; // J-1 pour le plus récent, J-bilCnt30j pour le plus ancien

    if (bj.ahIn > 0.001f) {
      float bh = constrain(bj.ahIn / ahMax, 0.0f, 1.0f) * GH;
      if (bh < 2.0f) bh = 2.0f;
      snprintf(b, sizeof(b),
        "<rect x='%.1f' y='%.1f' width='%.1f' height='%.1f' fill='#00c896' opacity='0.85' rx='1'>"
        "<title>" L_SVG_JOUR_N " : +%.2f Ah</title></rect>",
        xBase, GY+GH-bh, barW, bh, age, bj.ahIn);
      out->print(b);
    }
    if (bj.ahOut > 0.001f) {
      float bh = constrain(bj.ahOut / ahMax, 0.0f, 1.0f) * GH;
      if (bh < 2.0f) bh = 2.0f;
      snprintf(b, sizeof(b),
        "<rect x='%.1f' y='%.1f' width='%.1f' height='%.1f' fill='#ff3b3b' opacity='0.75' rx='1'>"
        "<title>" L_SVG_JOUR_N " : -%.2f Ah</title></rect>",
        xBase + barW + 1.0f, GY+GH-bh, barW, bh, age, bj.ahOut);
      out->print(b);
    }
  }

  // --- Barre "aujourd'hui en cours" — toujours slot NCOLS_MAX-1 (extrême droite) ---
  if (afficherEnCours) {
    float xBase  = GX + float(NCOLS_MAX - 1) * colW + colW * 0.08f;
    float xLabel = GX + float(NCOLS_MAX - 1) * colW + colW * 0.5f;
    // Deux patterns hachurés colorés — vert pour entrants, rouge pour sortants
    out->print("<defs>"
               "<pattern id='hatchIn' patternUnits='userSpaceOnUse' width='4' height='4'>"
               "<path d='M-1,1 l2,-2 M0,4 l4,-4 M3,5 l2,-2' stroke='#00c896' stroke-width='1.2'/>"
               "</pattern>"
               "<pattern id='hatchOut' patternUnits='userSpaceOnUse' width='4' height='4'>"
               "<path d='M-1,1 l2,-2 M0,4 l4,-4 M3,5 l2,-2' stroke='#ff3b3b' stroke-width='1.2'/>"
               "</pattern>"
               "</defs>");
    if (ahEntrants > 0.001f) {
      float bh = constrain(ahEntrants / ahMax, 0.0f, 1.0f) * GH;
      if (bh < 2.0f) bh = 2.0f;
      snprintf(b, sizeof(b),
        "<rect x='%.1f' y='%.1f' width='%.1f' height='%.1f' fill='#00c896' rx='1' class='pL'>"
        "</rect>"
        "<rect x='%.1f' y='%.1f' width='%.1f' height='%.1f' fill='url(#hatchIn)' rx='1' class='pH'>"
        "<title>" L_SVG_AUJ " : +%.2f Ah</title>"
        "</rect>",
        xBase, GY+GH-bh, barW, bh,
        xBase, GY+GH-bh, barW, bh, ahEntrants);
      out->print(b);
    }
    if (ahSortants > 0.001f) {
      float bh = constrain(ahSortants / ahMax, 0.0f, 1.0f) * GH;
      if (bh < 2.0f) bh = 2.0f;
      snprintf(b, sizeof(b),
        "<rect x='%.1f' y='%.1f' width='%.1f' height='%.1f' fill='#ff3b3b' rx='1' class='pL'>"
        "</rect>"
        "<rect x='%.1f' y='%.1f' width='%.1f' height='%.1f' fill='url(#hatchOut)' rx='1' class='pH'>"
        "<title>" L_SVG_AUJ " : -%.2f Ah</title>"
        "</rect>",
        xBase + barW + 1.0f, GY+GH-bh, barW, bh,
        xBase + barW + 1.0f, GY+GH-bh, barW, bh, ahSortants);
      out->print(b);
    }
    snprintf(b, sizeof(b),
      "<text x='%.1f' y='%d' text-anchor='middle' fill='#555' font-size='5' font-family='monospace'>" L_SVG_AUJ_COURT "</text>",
      xLabel, GY+GH+9);
    out->print(b);
  }

  // --- Labels X : J-age sous les barres archivées, pas adaptatif ---
  int pas = (bilCnt30j >= 20) ? 5 : (bilCnt30j >= 10) ? 3 : (bilCnt30j >= 5) ? 2 : 1;
  for (int i = 0; i < bilCnt30j; i += pas) {
    float x = GX + float(NCOLS_MAX - 1 - bilCnt30j + i) * colW + colW * 0.5f;
    int age = bilCnt30j - i;
    snprintf(b, sizeof(b),
      "<text x='%.1f' y='%d' text-anchor='middle' fill='#444' font-size='5' font-family='monospace'>" L_SVG_JOUR_N "</text>",
      x, GY+GH+9, age);
    out->print(b);
  }

  // --- Légende + SOC ---
  char socBuf[24]; snprintf(socBuf, sizeof(socBuf), "SOC: %.0f%%", socPct);
  out->print("<rect x='30' y='120' width='8' height='3' fill='#00c896'/>"
             "<text x='41' y='123' fill='#00c896' font-size='7' font-family='monospace'>" L_SVG_ENTRANTS "</text>"
             "<rect x='140' y='120' width='8' height='3' fill='#ff3b3b'/>"
             "<text x='151' y='123' fill='#ff3b3b' font-size='7' font-family='monospace'>" L_SVG_SORTANTS "</text>");
  snprintf(b, sizeof(b), "<text x='295' y='123' text-anchor='end' fill='#ffeb3b' font-size='7' font-family='monospace'>%s</text>", socBuf);
  out->print(b);
  out->print("</svg>");
}
// ==========================================================================
// PERSISTANCE PRÉVISIONS (NVS namespace "solar", clé "prev")
// malgré owmDernierJour == day() qui bloquait le refetch OWM/NASA
// ==========================================================================
void sauvegarderPrevisions() {
  prefs.begin("solar2", false);
  prefs.putBytes("prev", previsions, sizeof(previsions));
  prefs.putInt("owmJour", owmDernierJour);
  prefs.end();
}

void chargerPrevisions() {  
  prefs.begin("solar2", true);
  owmDernierJour = prefs.getInt("owmJour", -1);
  size_t loaded = prefs.getBytes("prev", previsions, sizeof(previsions));
  prefs.end();
  addLog("[PREV] owmJour=" + String(owmDernierJour) + " loaded=" + String(loaded));
  if (loaded != sizeof(previsions)) {
    memset(previsions, 0, sizeof(previsions)); // NVS vide = premier démarrage
    Serial.println("[PREV] Pas de prévisions en NVS (premier boot ou reset).");
  } else {
    Serial.println("[PREV] Prévisions restaurées depuis NVS.");
  }
}

// ==========================================================================
// 4. METEO & SERVICES
// ==========================================================================
bool getOWMClouds() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[OWM] WiFi non connecte.");
    return false;
  }
  // Filtre JSON : ne garde que hourly[*].clouds et daily[*].clouds
  // Réduit drastiquement la RAM nécessaire (réponse OWM brute = 30-50KB)
  StaticJsonDocument<200> owmFilter;
  owmFilter["hourly"][0]["clouds"] = true;
  owmFilter["daily"][0]["clouds"]  = true;

  HTTPClient http;
  String url = "http://api.openweathermap.org/data/3.0/onecall?lat=" + String(LATITUDE)
             + "&lon=" + String(LONGITUDE)
             + "&exclude=minutely,current,alerts&appid=" + OWM_KEY;
  http.begin(url);
  http.setTimeout(15000);        // ← 15s max
  http.setConnectTimeout(10000); // ← 10s max pour la connexion
  Serial.println("[OWM] Requete en cours...");
  int httpCode = http.GET();

  if (httpCode == 200) {
    // Stream direct — pas de String intermédiaire en RAM
    DynamicJsonDocument oDoc(12 * 1024);
    DeserializationError error = deserializeJson(oDoc, http.getStream(),
                                  DeserializationOption::Filter(owmFilter));
    http.end();
    if (!error) {
      Serial.printf("[OWM] Heap apres parse: %u bytes\n", ESP.getFreeHeap());
      for (int d = 0; d < 7; d++) {
        float c;
        if (d < 2) {
          float somC = 0; int nC = 0;
          int hDebut = d * 24, hFin = hDebut + 24;
          for (int hi = hDebut; hi < hFin; hi++) {
            float val = oDoc["hourly"][hi]["clouds"].as<float>();
            if (val >= 0) { somC += val; nC++; }
          }
          c = (nC > 0) ? somC / nC : 50.0;
        } else {
          c = oDoc["daily"][d+1]["clouds"].as<float>();
        }
        Serial.printf("[OWM] J+%d nuages: %.0f%%\n", d+1, c);
        for (int t = 0; t < 3; t++) previsions[d].clouds[t] = c;
      }
      return true;
    } else {
      Serial.print("[OWM] Erreur deserialization : ");
      Serial.println(error.f_str());
    }
  } else {
    http.end();
    Serial.printf("[OWM] Erreur HTTP : %d\n", httpCode);
    if (httpCode == 401) Serial.println("[OWM] ASTUCE : Verifie ta cle API !");
    if (httpCode == 404) Serial.println("[OWM] ASTUCE : Verifie la syntaxe url !");
  }
  return false;
}

bool getNasaHistory(float tN[3][7][3]) {
  for (int y = 0; y < 3; y++) {
    int yearOffset = y + 2;
    time_t tD = now() + 86400; time_t tF = now() + (7 * 86400);
    char sB[9], eB[9];
    sprintf(sB, "%04d%02d%02d", year(tD)-yearOffset, month(tD), day(tD));
    sprintf(eB, "%04d%02d%02d", year(tF)-yearOffset, month(tF), day(tF));
    HTTPClient http;
    String url = "https://power.larc.nasa.gov/api/temporal/hourly/point?parameters=ALLSKY_SFC_SW_DWN&community=RE&longitude=" + String(LONGITUDE, 4) + "&latitude=" + String(LATITUDE, 4) + "&start=" + String(sB) + "&end=" + String(eB) + "&format=JSON&tilt=" + String(TILT) + "&azimuth=" + String(AZIMUTH);
    http.begin(url);
    http.setTimeout(15000);        // ← 15s max
    http.setConnectTimeout(10000); // ← 10s max pour la connexion 
    int httpCode = http.GET();
    if (httpCode != 200) { http.end(); return false; }
    DynamicJsonDocument doc(32 * 1024);
    deserializeJson(doc, http.getStream(), DeserializationOption::Filter(nasaFilter));
    http.end();
    JsonObject data = doc["properties"]["parameter"]["ALLSKY_SFC_SW_DWN"];
    for (int d = 0; d < 7; d++) {
      time_t tC = now() + ((d + 1) * 86400);
      char dk[9]; sprintf(dk, "%04d%02d%02d", year(tC)-yearOffset, month(tC), day(tC));
      float m=0, mi=0, s=0;
      for (int h=6; h<10; h++)  m  += data[String(dk) + (h<10?"0":"") + String(h)].as<float>();
      for (int h=11; h<15; h++) mi += data[String(dk) + String(h)].as<float>();
      for (int h=16; h<20; h++) s  += data[String(dk) + String(h)].as<float>();
      tN[y][d][0] = m/4.0; tN[y][d][1] = mi/4.0; tN[y][d][2] = s/4.0;
    }
    delay(2000); 
  }
  return true;
}

void meteoForecast() {
  // Lecture atomique de miseAJourEnCours et owmDernierJour
  if (xMutexMeteo == NULL) return;
  xSemaphoreTake(xMutexMeteo, portMAX_DELAY);
  bool dejaCours  = miseAJourEnCours;
  int  dernierJour = owmDernierJour;
  xSemaphoreGive(xMutexMeteo);
  if (dejaCours) return;

  // Attend la synchronisation NTP (max 10s)
  int ntpRetry = 0;
  while (year() < 2020 && ntpRetry < 10) {
    Serial.println("[METEO] Attente NTP... (" + String(ntpRetry+1) + "/10)");
    delay(1000);
    ntpRetry++;
  }
  if (year() < 2020) {
    Serial.println("[METEO] NTP non disponible apres 10s, mise a jour annulee.");
    return;
  }

  int jourdujour = day();
  if (dernierJour == jourdujour) {
    Serial.println("[METEO] Donnees OWM deja a jour pour aujourd'hui, appel ignore.");
    return;
  }

  // Marque "en cours" sous mutex
  xSemaphoreTake(xMutexMeteo, portMAX_DELAY);
  miseAJourEnCours = true;
  xSemaphoreGive(xMutexMeteo);

  // Requêtes HTTP hors mutex (longues, ~15s, pas d'accès aux variables partagées)
  float tNasa[3][7][3];
  bool owmOK  = getOWMClouds();
  bool nasaOK = getNasaHistory(tNasa);

  // Écriture des résultats sous mutex
  xSemaphoreTake(xMutexMeteo, portMAX_DELAY);
  if (owmOK && nasaOK) {
    for (int d = 0; d < 7; d++) {
      for (int t = 0; t < 3; t++) {
        float v[3] = {tNasa[0][d][t], tNasa[1][d][t], tNasa[2][d][t]};
        float moy = (v[0] + v[1] + v[2]) / 3.0;
        float som = 0; int n = 0;
        for (int i = 0; i < 3; i++) {
          if (moy < 5.0 || abs(v[i] - moy) <= moy * FILTRE_ANOMALIE_ANNEE) { som += v[i]; n++; }
        }
        previsions[d].lux[t] = calcLux(n > 0 ? som / n : moy, previsions[d].clouds[t]);
      }
    }
    actualiserParametresDynamiques();
    owmDernierJour = day();
    prefs.begin("solar2", false);
    prefs.putInt("owmJour", owmDernierJour);
    prefs.end();

    miseAJourEnCours = false;
    xSemaphoreGive(xMutexMeteo);
    sauvegarderPrevisions(); // FIX GRAPHS: persiste previsions en NVS pour survie au deep sleep
    Serial.println("[METEO] Mise a jour complete. Prochain appel demain.");
    addLog("[METEO] MAJ OK. Coupure=" + String(VOLT_COUPURE_DYN,2) + "V SeuilON=" + String(CURRENT_LUX_ON,0) + "lx");
  } else {
    miseAJourEnCours = false;
    xSemaphoreGive(xMutexMeteo);
    Serial.println("[METEO] Echec OWM ou NASA — retry possible demain.");
  }
}

// ==========================================================================
// 5. SERVEUR WEB & WIFI
// ==========================================================================

// Menu hamburger commun — injecté dans toutes les pages HTML
void menuHamburger(AsyncResponseStream* resp) {
  resp->print("<div id='hmb' onclick='toggleMenu(event)' style='position:fixed;top:12px;right:12px;cursor:pointer;font-size:1.4rem;color:#888;padding:6px 10px;border-radius:6px;border:1px solid #333;background:#111;z-index:999;line-height:1;'>&#9776;</div>");
  resp->print("<div id='hmenu' style='display:none;position:fixed;top:0;right:12px;background:#1a1a1a;border:1px solid #333;border-radius:8px;min-width:200px;z-index:1000;padding:6px 0;box-shadow:0 4px 16px #000;'>");
  resp->print("<div onclick=\"location.href='/'\" style='padding:10px 16px;cursor:pointer;color:#888;font-family:monospace;font-size:0.85rem;'>" L_MENU_ACCUEIL "</div>");
  resp->print("<div onclick=\"location.href='/config'\" style='padding:10px 16px;cursor:pointer;color:#888;font-family:monospace;font-size:0.85rem;'>" L_MENU_CONFIG "</div>");
  resp->print("<div onclick=\"location.href='/debug'\" style='padding:10px 16px;cursor:pointer;color:#888;font-family:monospace;font-size:0.85rem;'>" L_MENU_DEBUG "</div>");
  resp->print("<div onclick=\"location.href='/debug/bilan'\" style='padding:10px 16px;cursor:pointer;color:#888;font-family:monospace;font-size:0.85rem;'>" L_MENU_DEBUG_BILAN "</div>");
  resp->print("<div onclick=\"location.href='/doc'\" style='padding:10px 16px;cursor:pointer;color:#888;font-family:monospace;font-size:0.85rem;'>" L_MENU_DOC "</div>");
  resp->print("</div>");
  resp->print("<script>"
    "function toggleMenu(e){e.stopPropagation();var m=document.getElementById('hmenu');"
    "if(m.style.display==='block'){m.style.display='none';return;}"
    "var r=document.getElementById('hmb').getBoundingClientRect();"
    "m.style.top=(r.bottom+6)+'px';m.style.display='block';}"
    "document.addEventListener('click',function(e){var b=document.getElementById('hmb');"
    "if(b&&!b.contains(e.target))document.getElementById('hmenu').style.display='none';});"
    "</script>");
}

void configurerRoutesServeur() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        uint32_t mv = analogReadMilliVolts(BAT_PIN);  // Vref calibrée via eFuse ESP32
        float vL = (mv > 0) ? (mv / 1000.0) * RATIO_PONT_DIVISEUR : 0;
        bool mosfet = (digitalRead(MOSFET_PIN) == HIGH);

        AsyncResponseStream *resp = request->beginResponseStream("text/html");
        resp->print("<!DOCTYPE html><html><head><meta charset='UTF-8'>");

        resp->print("<meta name='viewport' content='width=device-width,initial-scale=1'>");

        resp->print("<style>");

        resp->print("body{background:#0d0d0d;color:#eee;font-family:monospace;padding:12px;margin:0;}");

        resp->print("h3{font-size:0.75rem;letter-spacing:0.15em;text-transform:uppercase;color:#555;margin:18px 0 6px;}");

        resp->print(".stat{display:inline-block;background:#111;border:1px solid #222;border-radius:6px;padding:8px 14px;margin:4px;font-size:0.9rem;}");

        resp->print(".volt{color:#ff8c00;font-size:1.3rem;font-weight:bold;}");

        resp->print(".on{color:#00c896;} .off{color:#ff3b3b;}");

        resp->print("button{background:#1a1a1a;color:#ff8c00;border:1px solid #ff8c00;border-radius:6px;padding:10px 20px;cursor:pointer;font-family:monospace;margin-top:14px;}");

        resp->print("button:hover{background:#ff8c00;color:#000;}");

        resp->print(".btn-grey{background:#1a1a1a;color:#555;border:1px solid #333;border-radius:6px;padding:10px 20px;cursor:pointer;font-family:monospace;margin-top:14px;}");

        resp->print(".btn-grey:hover{background:#333;color:#eee;}");

        // Toast notification
        resp->print(".toast{position:fixed;bottom:-80px;left:50%;transform:translateX(-50%);");

        resp->print("background:#1a1a1a;border:1px solid #ff8c00;border-radius:8px;");

        resp->print("padding:12px 20px;font-size:0.82rem;color:#eee;white-space:nowrap;");

        resp->print("transition:bottom 0.4s ease;z-index:999;}");

        resp->print(".toast.show{bottom:24px;}");

        resp->print("</style></head>");

        resp->print("<body>");
        // Hamburger fixe top-right — indépendant du layout des badges
        resp->print("<div id='hmb' onclick='toggleMenu(event)' style='position:fixed;top:12px;right:12px;cursor:pointer;font-size:1.4rem;color:#888;padding:6px 10px;border-radius:6px;border:1px solid #333;background:#111;z-index:999;line-height:1;'>&#9776;</div>");
        resp->print("<div id='hmenu' style='display:none;position:fixed;top:0;right:12px;background:#1a1a1a;border:1px solid #333;border-radius:8px;min-width:200px;z-index:1000;padding:6px 0;box-shadow:0 4px 16px #000;'>");
        resp->print("<div onclick='majMeteo()' style='padding:10px 16px;cursor:pointer;color:#ff8c00;font-family:monospace;font-size:0.85rem;'>" L_MENU_MAJ_METEO "</div>");
        resp->print("<div onclick=\"location.href='/config'\" style='padding:10px 16px;cursor:pointer;color:#888;font-family:monospace;font-size:0.85rem;'>" L_MENU_CONFIG "</div>");
        resp->print("<div onclick=\"location.href='/debug'\" style='padding:10px 16px;cursor:pointer;color:#888;font-family:monospace;font-size:0.85rem;'>" L_MENU_DEBUG "</div>");
        resp->print("<div onclick=\"location.href='/debug/bilan'\" style='padding:10px 16px;cursor:pointer;color:#888;font-family:monospace;font-size:0.85rem;'>" L_MENU_DEBUG_BILAN "</div>");
        resp->print("<div onclick=\"location.href='/doc'\" style='padding:10px 16px;cursor:pointer;color:#888;font-family:monospace;font-size:0.85rem;'>" L_MENU_DOC "</div>");
        resp->print("</div>");

        // Toast HTML
        resp->print("<div class='toast' id='toast'>" L_HDR_TOAST "</div>");

        // JS
        // Déjà fait = succès aujourd'hui ET pas de MAJ en cours ou demandée
        bool _enc = false; int _owmJ = -1;
        if (xMutexMeteo) { xSemaphoreTake(xMutexMeteo, portMAX_DELAY); _enc = miseAJourEnCours; _owmJ = owmDernierJour; xSemaphoreGive(xMutexMeteo); }
        bool dejaFait = (_owmJ == day()) && !_enc && !demandeMeteo;
        // Keyframes injectées une seule fois dans le head au chargement
        resp->print("<style>"
          "@keyframes batPulse{0%,100%{opacity:1;}50%{opacity:0.4;}}"
          "@keyframes batBlink{0%,100%{opacity:1;}50%{opacity:0.15;}}"
          "</style>");
        resp->print("<script>");

        if (dejaFait) {
          resp->print("window.onload=function(){showToast();startLive();loadSVGs();};");
          resp->print("function majMeteo(){showToast();}");
        } else {
          resp->print("window.onload=function(){startLive();loadSVGs();};");
          resp->print("function majMeteo(){window.location.href='/upd';}");
        }
        resp->print("function showToast(){var t=document.getElementById('toast');");
        resp->print("t.classList.add('show');setTimeout(function(){t.classList.remove('show');},3000);}");

        // Polling live — majBat reconstruit le innerHTML du wrapper (setAttribute SVG peu fiable)
        // Couleur ⚡ : toujours blanc sauf sur fond vert (soc>70 en charge)
        resp->print("function majBat(soc,amp){"
          "var w=document.getElementById('bat-wrapper');if(!w)return;"
          "var charge=(amp>0.02),drain=(amp<-0.02),crit=(soc<=30&&!charge);"
          "var col=(soc>70)?'#00c896':(soc>30)?'#ff8c00':'#ff3b3b';"
          "var fw=Math.max(2,Math.min(116,Math.round(116*soc/100)));"
          "var icon=charge?' \u26a1':(drain?' \u25bc':'');"
          "var lbl=soc.toFixed(0)+'%'+icon;"
          "var tf=(soc>70&&charge)?'#002211':'#fff';"
          "var anim=crit?'batBlink 0.8s ease-in-out infinite':(drain?'batPulse 1.6s ease-in-out infinite':'none');"
          "var wave=charge?"
          "'<rect x=\"-40\" y=\"4\" width=\"35\" height=\"20\" rx=\"2\" fill=\"rgba(255,255,255,0.22)\" clip-path=\"url(#bcl)\">'"
          "+'<animateTransform attributeName=\"transform\" type=\"translate\" from=\"-10 0\" to=\"130 0\" dur=\"1.3s\" repeatCount=\"indefinite\"/>'"
          "+'</rect>':'';"
          "w.innerHTML="
          "'<svg width=\"118\" height=\"20\" viewBox=\"0 0 125 28\" style=\"vertical-align:middle;display:block;\">'"
          "+'<defs><clipPath id=\"bcl\"><rect x=\"2\" y=\"3\" width=\"116\" height=\"22\" rx=\"3\"/></clipPath></defs>'"
          "+'<rect x=\"0\" y=\"2\" width=\"118\" height=\"24\" rx=\"4\" fill=\"#111\" stroke=\"'+col+'\" stroke-width=\"1.5\"/>'"
          "+'<rect x=\"119\" y=\"9\" width=\"5\" height=\"10\" rx=\"2\" fill=\"'+col+'\"/>'"
          "+'<rect x=\"2\" y=\"4\" width=\"'+fw+'\" height=\"20\" rx=\"3\" fill=\"'+col+'\" opacity=\"0.9\" style=\"animation:'+anim+'\"/>'"
          "+wave"
          "+'<text x=\"59\" y=\"18\" text-anchor=\"middle\" font-family=\"monospace\" font-size=\"11\" font-weight=\"bold\" fill=\"'+tf+'\">'+lbl+'</text>'"
          "+'</svg>';"
          "}");
        resp->print("function startLive(){");
        // Appel immédiat dès le chargement de la page — pas d'attente du premier setInterval
        resp->print("fetch('/live').then(function(r){return r.json();}).then(function(d){majBat(d.soc,d.amp);}).catch(function(){});");
        resp->print("setInterval(function(){");
        resp->print("fetch('/live').then(function(r){return r.json();}).then(function(d){");
        resp->print("var hdr=document.getElementById('header-badges');");
        resp->print("if(d.hors_mppt){");
        resp->print("hdr.innerHTML=\"<span class='stat' style='background:#ff3b3b22;border:1px solid #ff3b3b;border-radius:4px;padding:4px 10px;'>" L_HORS_MPPT_INLINE "</span>\";");
        resp->print("}else{");
        resp->print("document.getElementById('lv').textContent=d.v+' V';");
        resp->print("var lvi=document.getElementById('lvi');if(lvi&&d.vi>9)lvi.textContent='\u25cf '+parseFloat(d.vi).toFixed(2)+'V';");
        resp->print("var ce=document.getElementById('lc-wrap');");
        resp->print("ce.innerHTML=d.c?\"<span class='on'>" L_HDR_CHARGE_ON "</span>\":\"<span class='off'>" L_HDR_CHARGE_OFF "</span>\";");
        resp->print("document.getElementById('lx').textContent=d.x+' lx';");
        resp->print("document.getElementById('ll').textContent=d.l+' lx';");
        resp->print("document.getElementById('lk').textContent=d.k+' V';");
        resp->print("majBat(d.soc,d.amp);");  // FIX : icône ⚡/▼ mise à jour au polling
        // Badge anti-bootloop : mise à jour temps restant
        resp->print("var bb=document.getElementById('bl-badge');"
          "if(bb){if(d.bl_sec>0){"
          "var t=d.bl_sec>=60?(Math.floor(d.bl_sec/60)+'min'):(d.bl_sec+'s');"
          "bb.innerHTML='" L_BADGE_BL_AVANT "'+t;"
          "bb.style.display='inline';"
          "}else{bb.style.display='none';}}");
        resp->print("}");
        resp->print("}).catch(function(){});");
        resp->print("},30000);}");  // poll /live toutes les 30s
        // Chargement SÉQUENTIEL des SVG — un seul fetch à la fois
        // Evite que 4 genererSVG_*() s'exécutent en parallèle sur le même core
        // et saturent la heap (→ crash WiFi sans reboot)
        resp->print("function loadSVGs(){");
        resp->print("var ids=['present','nuit','energie','bilan','futur','passe'];");
        resp->print("var i=0;");
        resp->print("function next(){");
        resp->print("if(i>=ids.length)return;");
        resp->print("var id=ids[i++];");
        resp->print("fetch('/svg/'+id).then(function(r){return r.text();})");
        resp->print(".then(function(html){");
        resp->print("document.getElementById('svg-'+id).innerHTML=html;");
        resp->print("setTimeout(next,200);");  // 200ms entre chaque requête
        resp->print("}).catch(function(){setTimeout(next,1000);});");
        resp->print("}");
        resp->print("next();");
        resp->print("}");
        resp->print("function refreshPresent(){");
        resp->print("fetch('/svg/present').then(function(r){return r.text();})");
        resp->print(".then(function(html){document.getElementById('svg-present').innerHTML=html;})");
        resp->print(".catch(function(){});");
        resp->print("}");
        resp->print("setInterval(refreshPresent," + String((unsigned long)INTERVALLE_CHECK_MIN * 60000UL) + ");");
        resp->print("function toggleMenu(e){e.stopPropagation();var m=document.getElementById('hmenu');if(m.style.display==='block'){m.style.display='none';return;}var r=document.getElementById('hmb').getBoundingClientRect();m.style.top=(r.bottom+6)+'px';m.style.display='block';}");
        resp->print("document.addEventListener('click',function(e){var b=document.getElementById('hmb');if(b&&!b.contains(e.target))document.getElementById('hmenu').style.display='none';});");
        resp->print("</script>");

        // En-tête statuts
        float luxActuel = luxDernier; // Lu depuis la variable globale — pas d'I2C dans le callback
        // Lecture thread-safe des seuils dynamiques (écrits par tacheMeteo sur Core 0)
        float _hdr_luxOn = CURRENT_LUX_ON, _hdr_coupDyn = VOLT_COUPURE_DYN;
        if (xMutexMeteo) { xSemaphoreTake(xMutexMeteo, portMAX_DELAY); _hdr_luxOn = CURRENT_LUX_ON; _hdr_coupDyn = VOLT_COUPURE_DYN; xSemaphoreGive(xMutexMeteo); }
        resp->print("<div id='header-badges' style='padding-right:52px'>");

        if (horsAlimentation) {
          resp->print("<span class='stat' style='background:#ff3b3b22;border:1px solid #ff3b3b;border-radius:4px;padding:4px 10px;'>⚠ " L_HORS_MPPT_INLINE "</span>");

        } else {
          // Badge anti-bootloop — toujours présent dans le DOM, visible si actif (mis à jour par polling)
          {
            bool blActif = (rtc_mosfetBloqueJusquA > 0 && esp_timer_get_time() < rtc_mosfetBloqueJusquA);
            int resteS = blActif ? (int)((rtc_mosfetBloqueJusquA - esp_timer_get_time()) / 1000000LL) : 0;
            blActif = blActif && (resteS > 0);
            char resteTxt[16]; 
            if (resteS >= 60) snprintf(resteTxt, sizeof(resteTxt), "%dmin", resteS/60);
            else              snprintf(resteTxt, sizeof(resteTxt), "%ds", resteS);
            char blTxt[80]; snprintf(blTxt, sizeof(blTxt), L_BADGE_BL_ACTIF, resteTxt);
            resp->print(String("<span id='bl-badge' class='stat' style='background:#ff8c0022;border:1px solid #ff8c00;color:#ff8c00;display:") + (blActif ? "inline" : "none") + "'>" + blTxt + "</span>");
          }
          resp->print("<span class='stat'><span class='volt' id='lv' title='" L_CFG_RATIO "'>" + String(vL, 2) + " V</span>"
                      + (inaOK && inaVoltDerniere > 9.0
                         ? "<span style='font-size:0.65rem;color:#4dabf7;margin-left:5px;vertical-align:middle;opacity:0.85;' id='lvi' title='Tension batterie (INA226)'>&#9679; " + String(inaVoltDerniere, 2) + "V</span>"
                         : "")
                      + "</span>");

          resp->print("<span class='stat' id='lc-wrap'>" + String(mosfet ? ("<span class='on'>" L_HDR_CHARGE_ON "</span>") : ("<span class='off'>" L_HDR_CHARGE_OFF "</span>")) + "</span>");

          resp->print("<span class='stat'>" L_HDR_LUX "<span style='color:#ffeb3b' id='lx'>" + (luxActuel >= 0 ? String(luxActuel, 0) : "---") + " lx</span></span>");

          resp->print("<span class='stat'>" L_HDR_SEUIL_ON "<span style='color:#ffeb3b' id='ll'>" + String(_hdr_luxOn, 0) + " lx</span></span>");

          resp->print("<span class='stat'>" L_HDR_COUPURE "<span style='color:#ff8c00' id='lk'>" + String(_hdr_coupDyn, 2) + " V</span></span>");

          // --- Icône batterie animée (SOC%) ---
          {
            float soc   = socPct;
            float amp   = courantDernier;
            bool charge = (amp > 0.02);
            bool drain  = (amp < -0.02);
            bool crit   = (soc <= 30 && !charge);
            const char* strokeCol = (soc > 70) ? "#00c896" : (soc > 30) ? "#ff8c00" : "#ff3b3b";
            const char* fillCol   = strokeCol;
            int fillW = (int)constrain(116.0f * soc / 100.0f, 2.0f, 116.0f);
            const char* textFill  = (soc > 20 && charge) ? "#002211" : "#fff";
            char socLbl[24];
            if      (charge) snprintf(socLbl, sizeof(socLbl), "%.0f%% \xe2\x9a\xa1", soc); // ⚡
            else if (drain)  snprintf(socLbl, sizeof(socLbl), "%.0f%% \xe2\x96\xbc", soc); // ▼
            else             snprintf(socLbl, sizeof(socLbl), "%.0f%%", soc);
            // Animation CSS embarquée (une seule fois dans la page — pas grave si répété)
            char batSvg[1200];
            snprintf(batSvg, sizeof(batSvg),
              "<span id='bat-wrapper' class='stat' style='padding:8px 14px;display:inline-flex;align-items:center;vertical-align:bottom;'>"
              "<svg id='batSvg' width='118' height='20' viewBox='0 0 125 28' style='vertical-align:middle;display:block;'>"
              "<defs><clipPath id='bcl'><rect x='2' y='3' width='116' height='22' rx='3'/></clipPath></defs>"
              "<rect x='0' y='2' width='118' height='24' rx='4' fill='#111' stroke='%s' stroke-width='1.5'/>"
              "<rect x='119' y='9' width='5' height='10' rx='2' fill='%s'/>"
              "<rect id='bfill' x='2' y='4' width='%d' height='20' rx='3' fill='%s' opacity='0.9'"
              " style='animation:%s'/>"
              "%s"  // vague charge si applicable
              "<text x='59' y='18' text-anchor='middle' font-family='monospace' font-size='11'"
              " font-weight='bold' fill='%s'>%s</text>"
              "</svg></span>",
              strokeCol, fillCol, fillW, fillCol,
              crit  ? "batBlink 0.8s ease-in-out infinite" :
              drain ? "batPulse 1.6s ease-in-out infinite" : "none",
              charge ?
                "<rect x='-40' y='4' width='35' height='20' rx='2'"
                " fill='rgba(255,255,255,0.22)' clip-path='url(#bcl)'>"
                "<animateTransform attributeName='transform' type='translate'"
                " from='-10 0' to='130 0' dur='1.3s' repeatCount='indefinite'/>"
                "</rect>" : "",
              textFill, socLbl
            );
            resp->print(batSvg);
          }

        }
        resp->print("</div>");

        // Graphiques — placeholders chargés en JS via fetch /svg/xxx
        // Évite de générer tous les SVG en RAM simultanément (crash heap)
        const char* svgPH = "<div style='min-height:140px;background:#111;border-radius:8px;"
                            "display:flex;align-items:center;justify-content:center;'>"
                            "<span style='color:#333;font-size:0.8rem;font-family:monospace'>" L_CHARGEMENT "</span></div>";

        resp->print("<div style='display:flex;flex-wrap:wrap;gap:8px;'>");
        resp->print("<div style='flex:1;min-width:281px;'>");
        resp->print("<h3>" L_H3_PRESENT "</h3>");
        resp->print("<div id='svg-present'>"); resp->print(svgPH); resp->print("</div>");
        resp->print("</div>");
        resp->print("<div style='flex:1;min-width:281px;'>");
        resp->print("<h3>" L_H3_NOCTURNE "</h3>");
        resp->print("<div id='svg-nuit'>");    resp->print(svgPH); resp->print("</div>");
        resp->print("</div>");
        resp->print("</div>");

        resp->print("<div style='display:flex;flex-wrap:wrap;gap:8px;'>");
        resp->print("<div style='flex:1;min-width:281px;'>");
        resp->print("<h3>" L_H3_ENERGIE "</h3>");
        resp->print("<div id='svg-energie'>"); resp->print(svgPH); resp->print("</div>");
        resp->print("</div>");
        resp->print("<div style='flex:1;min-width:281px;'>");
        resp->print("<h3>" L_H3_BILAN "</h3>");
        resp->print("<div id='svg-bilan'>"); resp->print(svgPH); resp->print("</div>");
        resp->print("</div>");
        resp->print("</div>");

        resp->print("<div style='display:flex;flex-wrap:wrap;gap:8px;'>");
        resp->print("<div style='flex:1;min-width:281px;'>");
        resp->print("<h3>" L_H3_FUTUR "</h3>");
        resp->print("<div id='svg-futur'>"); resp->print(svgPH); resp->print("</div>");
        resp->print("</div>");
        resp->print("<div style='flex:1;min-width:281px;'>");
        resp->print("<h3>" L_H3_PASSE "</h3>");
        resp->print("<div id='svg-passe'>"); resp->print(svgPH); resp->print("</div>");
        resp->print("</div>");
        resp->print("</div>");

        resp->print("<div style='text-align:center;margin-top:24px;font-size:0.7rem;color:#333;font-family:monospace;'>"
          L_FOOTER_CREDIT " <a href='mailto:fraise_prieres9v@icloud.com' style='color:#444;text-decoration:none;'>F.B</a>"
          " &amp; <a href='https://claude.ai' target='_blank' style='color:#444;text-decoration:none;'>Claude</a>"
          " (Anthropic)</div>");
        resp->print("</body></html>");

        request->send(resp);
    });

    // /status : retourne "1" si MAJ en cours, "0" si terminée — interrogé par le JS de /upd
    server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
        bool _enCours = false;
        if (xMutexMeteo) { xSemaphoreTake(xMutexMeteo, portMAX_DELAY); _enCours = miseAJourEnCours; xSemaphoreGive(xMutexMeteo); }
        request->send(200, "text/plain", _enCours ? "1" : "0");
    });

    // /svg/present|nuit|futur|passe — chaque graphe servi séparément
    // Les générateurs écrivent directement dans le flux — zéro String alloué sur le heap
    server.on("/svg/present", HTTP_GET, [](AsyncWebServerRequest *request){
        AsyncResponseStream *r = request->beginResponseStream("image/svg+xml");
        genererSVG_Present(r);
        request->send(r);
    });
    server.on("/svg/nuit", HTTP_GET, [](AsyncWebServerRequest *request){
        AsyncResponseStream *r = request->beginResponseStream("image/svg+xml");
        genererSVG_Nuit(r);
        request->send(r);
    });
    server.on("/svg/futur", HTTP_GET, [](AsyncWebServerRequest *request){
        AsyncResponseStream *r = request->beginResponseStream("image/svg+xml");
        genererSVG_Futur(r);
        request->send(r);
    });
    server.on("/svg/passe", HTTP_GET, [](AsyncWebServerRequest *request){
        AsyncResponseStream *r = request->beginResponseStream("image/svg+xml");
        genererSVG_Passe(r);
        request->send(r);
    });
    server.on("/svg/energie", HTTP_GET, [](AsyncWebServerRequest *request){
        AsyncResponseStream *r = request->beginResponseStream("image/svg+xml");
        genererSVG_Energie(r);
        request->send(r);
    });
    server.on("/svg/bilan", HTTP_GET, [](AsyncWebServerRequest *request){
        AsyncResponseStream *r = request->beginResponseStream("image/svg+xml");
        genererSVG_Bilan(r);
        request->send(r);
    });

    // /debug/bilan/force : force le condenser manuellement via flag Core1
    server.on("/debug/bilan/force", HTTP_GET, [](AsyncWebServerRequest *request){
        forceCondenser = true;
        addLog("[BILAN] condenser force manuellement via /debug/bilan/force");
        request->send(200, "text/html",
            "<html><head><meta charset='utf-8'>"
            "<meta http-equiv='refresh' content='1;url=/debug/bilan'>"
            "<style>body{background:#0d0d0d;color:#ff8c00;font-family:monospace;padding:20px;}</style>"
            "</head><body>" L_DBG_BIL_FORCE_OK "</body></html>");
    });

    // /debug/bilan/undo : annule le dernier condenser archivé
    server.on("/debug/bilan/undo", HTTP_GET, [](AsyncWebServerRequest *request){
        forceUndo = true;
        addLog("[BILAN] undo demande via /debug/bilan/undo");
        request->send(200, "text/html",
            "<html><head><meta charset='utf-8'>"
            "<meta http-equiv='refresh' content='1;url=/debug/bilan'>"
            "<style>body{background:#0d0d0d;color:#eee;font-family:monospace;padding:20px;}</style>"
            "</head><body>" L_DBG_BIL_UNDO_OK "</body></html>");
    });

    server.on("/debug/bilan/reset", HTTP_GET, [](AsyncWebServerRequest *request){
        forceReset = true;
        addLog("[BILAN] reset demande via /debug/bilan/reset");
        request->send(200, "text/html",
            "<html><head><meta charset='utf-8'>"
            "<meta http-equiv='refresh' content='1;url=/debug/bilan'>"
            "<style>body{background:#0d0d0d;color:#ff3b3b;font-family:monospace;padding:20px;}</style>"
            "</head><body>" L_DBG_BIL_RESET_OK "</body></html>");
    });

    // /debug/bilan : état temps réel du bilan 30j (streaming, zéro String heap)
    server.on("/debug/bilan", HTTP_GET, [](AsyncWebServerRequest *request){
        AsyncResponseStream *r = request->beginResponseStream("text/html");
        char b[180];
        r->print("<!DOCTYPE html><html><head><meta charset='utf-8'>"
                 "<meta http-equiv='refresh' content='30'>"
                 "<style>"
                 "body{background:#0d0d0d;color:#eee;font-family:monospace;padding:16px;}"
                 "table{border-collapse:collapse;margin-top:8px;}"
                 "td,th{border:1px solid #333;padding:4px 10px;font-size:0.8rem;}"
                 "th{color:#ff8c00;}"
                 ".ok{color:#00c896;}.warn{color:#ffeb3b;}"
                 ".btn{display:inline-block;background:#1a1a1a;color:#555;border:1px solid #333;"
                 "border-radius:6px;padding:8px 16px;font-family:monospace;font-size:0.8rem;"
                 "text-decoration:none;margin-right:8px;}"
                 ".btn:hover{background:#333;color:#eee;}"
                 ".btn-o{display:inline-block;background:#1a1a1a;color:#ff8c00;border:1px solid #ff8c00;"
                 "border-radius:6px;padding:8px 16px;font-family:monospace;font-size:0.8rem;"
                 "text-decoration:none;margin-right:8px;}"
                 ".btn-o:hover{background:#ff8c00;color:#000;}"
                 ".btn-r{display:inline-block;background:#1a1a1a;color:#ff3b3b;border:1px solid #ff3b3b;"
                 "border-radius:6px;padding:8px 16px;font-family:monospace;font-size:0.8rem;"
                 "text-decoration:none;margin-right:8px;}"
                 ".btn-r:hover{background:#ff3b3b;color:#000;}"
                 "</style></head><body>");
        menuHamburger(r);
        r->print("<h2>" L_DBG_BILAN_TITRE "</h2>");
        r->print("<a href='/' class='btn'>" L_BTN_RETOUR "</a><br><br>");

        // --- État en RAM ---
        snprintf(b, sizeof(b), "<p>" L_DBG_BIL_AHIN " <span class='ok'>%.4f Ah</span> &nbsp;"
                               L_DBG_BIL_AHOUT " <span class='ok'>%.4f Ah</span></p>",
                 ahEntrants, ahSortants);
        r->print(b);
        snprintf(b, sizeof(b), "<p><b>bilanFait :</b> <span class='%s'>%s</span> &nbsp;"
                               "<b>indexBilan30j :</b> %d &nbsp;"
                               "<b>bilCnt30j :</b> %d</p>",
                 bilanFait ? "warn" : "ok", bilanFait ? "true" : "false",
                 indexBilan30j, bilCnt30j);
        r->print(b);
        snprintf(b, sizeof(b), "<p><b>inaOK :</b> <span class='%s'>%s</span> &nbsp;"
                               "<b>courant :</b> %.4f A &nbsp;"
                               "<b>SOC :</b> %.1f%%</p>",
                 inaOK ? "ok" : "warn", inaOK ? "true" : "false",
                 courantDernier, socPct);
        r->print(b);

        // --- Actions ---
        r->print("<p>"
                 "<a href='/debug/bilan/force' class='btn-o'>" L_DBG_BIL_FORCE "</a>"
                 "<a href='/debug/bilan/undo' class='btn'>" L_BTN_ANNULER "</a>"
                 "<a href='/debug/bilan/reset' class='btn-r'>" L_BTN_RESET "</a>"
                 "</p>");

        // --- Tableau des 30 cases ---
        r->print("<table><tr><th>" L_DBG_BIL_TH_CASE "</th><th>ahIn</th><th>ahOut</th><th>" L_DBG_BIL_TH_NOTE "</th></tr>");
        for (int i = 0; i < 30; i++) {
            bool estProchain = (i == indexBilan30j);
            // Case dans les bilCnt30j données valides ?
            // Les cases valides vont de (indexBilan30j - bilCnt30j + 30)%30 à (indexBilan30j - 1 + 30)%30
            int startIdx = (indexBilan30j - bilCnt30j + 30) % 30;
            bool estValide = false;
            if (bilCnt30j > 0) {
                if (bilCnt30j == 30) estValide = true;
                else {
                    int s = startIdx, e = (indexBilan30j - 1 + 30) % 30;
                    if (s <= e) estValide = (i >= s && i <= e);
                    else        estValide = (i >= s || i <= e);
                }
            }
            const char* rowStyle = estProchain ? " style='background:#1a0d00;'" : "";
            snprintf(b, sizeof(b), "<tr%s><td>%d%s</td><td>%.3f</td><td>%.3f</td><td>%s</td></tr>",
                     rowStyle, i,
                     estProchain ? " &larr;" : "",
                     bilan30j[i].ahIn, bilan30j[i].ahOut,
                     estProchain ? L_DBG_BIL_PROCHAIN : (estValide ? L_DBG_BIL_VALIDE : L_DBG_BIL_VIDE));
            r->print(b);
        }
        r->print("</table>");
        r->print("<br><a href='/' class='btn'>" L_BTN_RETOUR "</a></body></html>");
        request->send(r);
    });

    // /live : JSON temps réel — FIX: snprintf statique, zéro allocation heap
    server.on("/live", HTTP_GET, [](AsyncWebServerRequest *request){
        uint32_t mv = analogReadMilliVolts(BAT_PIN);
        float v = (mv > 0) ? (mv / 1000.0) * RATIO_PONT_DIVISEUR : 0;

        bool horsAlim = (v < 9.5 || v > 20.0);
        bool charge = (!horsAlim) && (digitalRead(MOSFET_PIN) == HIGH);
        float lux = luxDernier;
        float _luxOn = CURRENT_LUX_ON, _coupDyn = VOLT_COUPURE_DYN;
        if (xMutexMeteo) { xSemaphoreTake(xMutexMeteo, portMAX_DELAY);
           _luxOn   = CURRENT_LUX_ON;
           _coupDyn = VOLT_COUPURE_DYN;
           xSemaphoreGive(xMutexMeteo); }

        // Buffer sur la stack — pas de static pour éviter les conflits si callbacks concurrents
        int64_t _maintenant = esp_timer_get_time();
        int bl_sec = (rtc_mosfetBloqueJusquA > 0 && _maintenant < rtc_mosfetBloqueJusquA)
                     ? (int)((rtc_mosfetBloqueJusquA - _maintenant) / 1000000LL)
                     : 0;
        // courantDernier mis à jour toutes les 30s dans loop() — thread-safe
        char json[360];
        snprintf(json, sizeof(json),
          "{\"hors_mppt\":%s,\"v\":\"%.2f\",\"vi\":%.2f,\"c\":%s,\"x\":\"%s\",\"l\":\"%.0f\",\"k\":\"%.2f\",\"soc\":%.1f,\"amp\":%.3f,\"bl_sec\":%d}",
          horsAlim ? "true" : "false",
          v,
          inaVoltDerniere,
          charge   ? "true" : "false",
          (lux >= 0) ? String(lux, 0).c_str() : "---",
          _luxOn, _coupDyn,
          socPct, courantDernier,
          bl_sec);
        request->send(200, "application/json", json);
    });

    // /config : page de configuration complète à onglets
    server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request){
        AsyncResponseStream *resp = request->beginResponseStream("text/html");
        resp->print("<!DOCTYPE html><html><head><meta charset='UTF-8'>");

        resp->print("<meta name='viewport' content='width=device-width,initial-scale=1'>");

        resp->print("<style>");

        resp->print("body{background:#0d0d0d;color:#eee;font-family:monospace;padding:12px;margin:0;}");

        resp->print("h2{color:#ff8c00;font-size:1rem;letter-spacing:0.1em;margin:0 0 16px;}");

        resp->print(".tabs{display:flex;gap:4px;margin-bottom:16px;}");

        resp->print(".tab{padding:8px 16px;background:#111;border:1px solid #222;border-radius:6px 6px 0 0;cursor:pointer;font-size:0.8rem;color:#555;}");

        resp->print(".tab.active{border-color:#ff8c00;color:#ff8c00;background:#1a1a1a;}");

        resp->print(".panel{display:none;background:#111;border:1px solid #222;border-radius:0 6px 6px 6px;padding:16px;}");

        resp->print(".panel.active{display:block;}");

        resp->print(".row{margin-bottom:12px;}");

        resp->print("label{display:block;color:#888;font-size:0.75rem;margin-bottom:4px;}");

        resp->print("input{background:#0d0d0d;color:#eee;border:1px solid #333;border-radius:4px;padding:6px 8px;width:160px;font-family:monospace;}");

        resp->print("input:focus{outline:none;border-color:#ff8c00;}");

        resp->print(".unit{color:#555;font-size:0.75rem;margin-left:6px;}");

        resp->print(".hint{color:#333;font-size:0.7rem;margin-top:2px;}");

        resp->print(".btn{display:inline-block;box-sizing:border-box;background:#1a1a1a;color:#555;border:1px solid #333;border-radius:6px;");

        resp->print("padding:8px 16px;cursor:pointer;font-family:monospace;margin-top:12px;font-size:0.8rem;line-height:1.2;text-decoration:none;vertical-align:middle;}");

        resp->print(".btn:hover{background:#333;color:#eee;text-decoration:none;}");

        resp->print(".back{color:#555;font-size:0.8rem;margin-left:14px;text-decoration:none;}");

        resp->print(".back:hover{color:#ff8c00;}");

        resp->print(".sep{border:none;border-top:1px solid #1a1a1a;margin:14px 0;}");

        resp->print("</style>");

        resp->print("<script>");

        resp->print("function tab(n){");

        resp->print("document.querySelectorAll('.tab').forEach(function(t,i){t.classList.toggle('active',i==n);});");

        resp->print("document.querySelectorAll('.panel').forEach(function(p,i){p.classList.toggle('active',i==n);});");

        resp->print("}");

        resp->print("</script>");

        resp->print("</head><body>");
        menuHamburger(resp);
        resp->print("<h2>" L_CFG_TITRE "</h2>");
        resp->print("<a href='/' class='btn'>" L_BTN_RETOUR "</a><br><br>");

        resp->print("<form action='/saveconfig' method='POST'>");

        // --- ONGLETS ---
        resp->print("<div class='tabs'>");

        resp->print("<div class='tab active' onclick='tab(0)'>" L_TAB_GENERAL "</div>");

        resp->print("<div class='tab' onclick='tab(1)'>" L_TAB_LUMINOSITE "</div>");

        resp->print("<div class='tab' onclick='tab(2)'>" L_TAB_BATTERIE "</div>");

        resp->print("</div>");

        // ===================== ONGLET 0 : GÉNÉRAL =====================
        resp->print("<div class='panel active'>");

        resp->print("<div class='row'><label>" L_CFG_OWM_KEY "</label>");

        resp->print("<input name='owm' value='" + OWM_KEY + "' style='width:260px'></div>");

        resp->print("<hr class='sep'>");

        // --- Réseau / IP statique ---
        resp->print("<div style='color:#aaa;font-size:0.75rem;margin-bottom:6px'>" L_CFG_RESEAU "</div>");
        resp->print("<div class='row'><label>" L_CFG_IP "</label><input name='cfgIP' value='" + CFG_IP + "' style='width:140px'></div>");
        resp->print("<div class='row'><label>" L_CFG_GW "</label><input name='cfgGW' value='" + CFG_GW + "' style='width:140px'></div>");
        resp->print("<div class='row'><label>" L_CFG_MASQUE "</label><input name='cfgMask' value='" + CFG_MASK + "' style='width:140px'></div>");
        resp->print("<div class='row'><label>" L_CFG_DNS "</label><input name='cfgDNS' value='" + CFG_DNS + "' style='width:140px'></div>");

        resp->print("<hr class='sep'>");

        resp->print("<div class='row'><label>" L_CFG_LAT "</label>");

        resp->print("<input name='lat' value='" + String(LATITUDE, 4) + "'><span class='unit'>°</span></div>");

        resp->print("<div class='row'><label>" L_CFG_LON "</label>");

        resp->print("<input name='lon' value='" + String(LONGITUDE, 4) + "'><span class='unit'>°</span></div>");

        resp->print("<div class='row'><label>" L_CFG_TILT "</label>");

        resp->print("<input name='tilt' value='" + String(TILT) + "'><span class='unit'>" L_CFG_TILT_UNIT "</span></div>");

        resp->print("<div class='row'><label>" L_CFG_AZIMUTH "</label>");

        resp->print("<input name='azimuth' value='" + String(AZIMUTH) + "'><span class='unit'>" L_CFG_AZIMUTH_UNIT "</span></div>");

        resp->print("<hr class='sep'>");

        resp->print("<div class='row'><label>" L_CFG_RATIO "</label>");

        resp->print("<input name='ratio' value='" + String(RATIO_PONT_DIVISEUR, 4) + "'>");

        resp->print("<div class='hint'>150kΩ + 33kΩ → (150+33)/33 = 5.5454</div></div>");

        resp->print("<hr class='sep'>");

        // Styles widgets +/- (injectés ici, dans le panel déjà ouvert)
        resp->print("<style>"
          ".tg{display:inline-flex;align-items:center;gap:6px;flex-wrap:wrap;}"
          ".tf{display:inline-flex;align-items:center;border:1px solid #333;border-radius:6px;overflow:hidden;background:#0d0d0d;}"
          ".tf input{width:42px;text-align:center;background:#0d0d0d;color:#ff8c00;font-family:monospace;"
            "font-size:1.1rem;font-weight:bold;border:none;outline:none;padding:8px 4px;"
            "-moz-appearance:textfield;}"
          ".tf input::-webkit-outer-spin-button,.tf input::-webkit-inner-spin-button{-webkit-appearance:none;margin:0;}"
          ".tf button{background:#1a1a1a;color:#ff8c00;border:none;padding:8px 13px;font-size:1.1rem;"
            "cursor:pointer;font-family:monospace;line-height:1;transition:background 0.15s;"
            "user-select:none;-webkit-user-select:none;}"
          ".tf button:hover{background:#ff8c00;color:#000;}"
          ".tf button:active{background:#cc7000;color:#000;}"
          ".tsep{color:#555;font-size:1.2rem;font-weight:bold;padding:0 2px;}"
          "</style>");

        resp->print("<script>"
          "function adj(id,d,mn,mx){var e=document.getElementById(id);"
          "var v=parseInt(e.value)+d;if(v<mn)v=mn;if(v>mx)v=mx;e.value=v;}"
          "document.addEventListener('DOMContentLoaded',function(){"
          "document.querySelectorAll('.tf button').forEach(function(b){"
          "var t=null,f=null;"
          "b.addEventListener('pointerdown',function(){b.click();"
          "t=setTimeout(function(){f=setInterval(function(){b.click();},120);},500);});"
          "function stop(){clearTimeout(t);clearInterval(f);}"
          "b.addEventListener('pointerup',stop);b.addEventListener('pointerleave',stop);});});"
          "</script>");

        resp->print("<div class='row'><label>" L_CFG_SLEEP "</label>");
        resp->print("<div class='tg'>");
        resp->print("<div class='tf'>"
          "<button type='button' onclick=\"adj('hSoir',-1,0,23)\">&#8722;</button>"
          "<input type='number' id='hSoir' name='hSoir' min='0' max='23' value='" + String(HEURE_SOMMEIL_SOIR) + "'>"
          "<button type='button' onclick=\"adj('hSoir',+1,0,23)\">+</button>"
          "</div>");
        resp->print("<span class='tsep'>h</span>");
        resp->print("<div class='tf'>"
          "<button type='button' onclick=\"adj('mSoir',-5,0,59)\">&#8722;</button>"
          "<input type='number' id='mSoir' name='mSoir' min='0' max='59' value='" + String(MIN_SOMMEIL_SOIR) + "'>"
          "<button type='button' onclick=\"adj('mSoir',+5,0,59)\">+</button>"
          "</div>");
        resp->print("<span class='unit'>" L_CFG_SLEEP_UNIT "</span>");
        resp->print("</div></div>");

        resp->print("<div class='row'><label>" L_CFG_REVEIL "</label>");
        resp->print("<div class='tg'>");
        resp->print("<div class='tf'>"
          "<button type='button' onclick=\"adj('hJour',-1,0,23)\">&#8722;</button>"
          "<input type='number' id='hJour' name='hJour' min='0' max='23' value='" + String(HEURE_REVEIL_JOUR) + "'>"
          "<button type='button' onclick=\"adj('hJour',+1,0,23)\">+</button>"
          "</div>");
        resp->print("<span class='tsep'>h</span>");
        resp->print("<div class='tf'>"
          "<button type='button' onclick=\"adj('mJour',-5,0,59)\">&#8722;</button>"
          "<input type='number' id='mJour' name='mJour' min='0' max='59' value='" + String(MIN_REVEIL_JOUR) + "'>"
          "<button type='button' onclick=\"adj('mJour',+5,0,59)\">+</button>"
          "</div>");
        resp->print("<span class='unit'>" L_CFG_REVEIL_UNIT "</span>");
        resp->print("</div></div>");

        resp->print("<div class='row'><label>" L_CFG_RELEVES "</label>");
        resp->print("<div class='tg'>");
        resp->print("<div class='tf'>"
          "<button type='button' onclick=\"adj('nbReleves',-1,1,5)\">&#8722;</button>"
          "<input type='number' id='nbReleves' name='nbReleves' min='1' max='5' style='width:36px' value='" + String(NB_RELEVES_NUIT) + "'>"
          "<button type='button' onclick=\"adj('nbReleves',+1,1,5)\">+</button>"
          "</div>");
        resp->print("</div>");
        resp->print("<div class='hint'>" L_CFG_RELEVES_HINT "</div></div>");

        resp->print("<hr class='sep'>");

        resp->print("<div class='row'><label>" L_CFG_ARCHIVAGE "</label>");
        resp->print("<div class='tg'>");
        resp->print("<div class='tf'>"
          "<button type='button' onclick=\"adj('avanceCond',-1,0,3)\">&#8722;</button>"
          "<input type='number' id='avanceCond' name='avanceCond' min='0' max='3' style='width:36px' value='" + String(AVANCE_CONDENSER) + "'>"
          "<button type='button' onclick=\"adj('avanceCond',+1,0,3)\">+</button>"
          "</div>");
        resp->print("<span class='unit'>" L_CFG_ARCHIVAGE_UNIT "</span>");
        resp->print("</div>");
        resp->print("<div class='hint'>" L_CFG_ARCHIVAGE_HINT "</div></div>");

        // Script de validation côté client
        resp->print("<script>");
        resp->print("document.querySelector('form').addEventListener('submit',function(e){");
        resp->print("var hS=parseInt(document.querySelector('[name=hSoir]').value);");
        resp->print("var mS=parseInt(document.querySelector('[name=mSoir]').value);");
        resp->print("var hJ=parseInt(document.querySelector('[name=hJour]').value);");
        resp->print("var mJ=parseInt(document.querySelector('[name=mJour]').value);");
        resp->print("var err=[];");
        resp->print("if(isNaN(hS)||hS<0||hS>23) err.push('" L_VAL_HEURE_SLEEP "');");
        resp->print("if(isNaN(mS)||mS<0||mS>59) err.push('" L_VAL_MIN_SLEEP "');");
        resp->print("if(isNaN(hJ)||hJ<0||hJ>23) err.push('" L_VAL_HEURE_REVEIL "');");
        resp->print("if(isNaN(mJ)||mJ<0||mJ>59) err.push('" L_VAL_MIN_REVEIL "');");
        resp->print("var minSoir=hS*60+mS; var minJour=hJ*60+mJ;");
        resp->print("var plage=(minSoir>minJour)?(1440-minSoir+minJour):(minJour-minSoir);");
        resp->print("if(plage<120) err.push('" L_VAL_PLAGE_COURTE "');");
        resp->print("if(err.length){e.preventDefault();alert(err.join('\\n'));}");
        resp->print("});");
        resp->print("</script>");

        resp->print("</div>"); // fin panel Général

        // ===================== ONGLET 1 : LUMINOSITÉ =====================
        resp->print("<div class='panel'>");

        resp->print("<div class='row'><label>" L_CFG_LUX_MAX "</label>");

        resp->print("<input name='luxDefaut' value='" + String(LUX_ON_DEFAUT, 0) + "'><span class='unit'>lux</span>");

        resp->print("<div class='hint'>" L_CFG_LUX_MAX_HINT "</div></div>");

        resp->print("<div class='row'><label>" L_CFG_LUX_AMP "</label>");

        resp->print("<input name='luxAmp' value='" + String(LUX_ON_AMPLITUDE, 0) + "'><span class='unit'>lux</span>");

        resp->print("<div class='hint'>" L_CFG_LUX_AMP_HINT "</div></div>");

        resp->print("<div class='row'><label>" L_CFG_LUX_CONF "</label>");

        resp->print("<input name='luxConf' value='" + String(LUX_CONFIANCE_MAX, 0) + "'><span class='unit'>lux</span>");

        resp->print("<div class='hint'>" L_CFG_LUX_CONF_HINT "</div></div>");

        resp->print("<hr class='sep'>");

        resp->print("<div style='color:#555;font-size:0.75rem'>" L_CFG_LUX_ACTUEL "<span style='color:#ffeb3b'>" + String(CURRENT_LUX_ON, 0) + " lux</span> — ");

        resp->print(L_CFG_LUX_PLAGE_VAL "[" + String(LUX_ON_DEFAUT - LUX_ON_AMPLITUDE, 0) + " → " + String(LUX_ON_DEFAUT, 0) + " lux]</div>");

        resp->print("</div>"); // fin panel Luminosité

        // ===================== ONGLET 2 : BATTERIE =====================
        resp->print("<div class='panel'>");

        resp->print("<div class='row'><label>" L_CFG_V_PLEINE "</label>");

        resp->print("<input name='vPleine' value='" + String(VOLT_PLEINE, 2) + "'><span class='unit'>V</span></div>");

        resp->print("<div class='row'><label>" L_CFG_V_REPRISE "</label>");

        resp->print("<input name='vReprise' value='" + String(VOLT_REPRISE, 2) + "'><span class='unit'>V</span>");

        resp->print("<div class='hint'>" L_CFG_V_REPRISE_HINT "</div></div>");

        resp->print("<div class='row'><label>" L_CFG_V_COUPURE "</label>");

        resp->print("<input name='vCoupure' value='" + String(VOLT_COUPURE, 2) + "'><span class='unit'>V</span>");

        resp->print("<div class='hint'>" L_CFG_V_COUPURE_HINT "</div></div>");

        resp->print("<div class='row'><label>" L_CFG_V_CRITIQUE "</label>");

        resp->print("<input name='vCritique' value='" + String(VOLT_CRITIQUE, 2) + "'><span class='unit'>V</span></div>");

        resp->print("<div class='row'><label>" L_CFG_V_PLANCHER "</label>");

        resp->print("<input name='vMin' value='" + String(VOLT_COUPURE_MIN, 2) + "'><span class='unit'>V</span>");

        resp->print("<div class='hint'>" L_CFG_V_PLANCHER_HINT "</div></div>");

        resp->print("<hr class='sep'>");

        resp->print("<div class='row'><label>" L_CFG_CAPACITE "</label>");
        resp->print("<input name='capBat' value='" + String(CAPACITE_BAT, 1) + "'><span class='unit'>Ah</span>");
        resp->print("<div class='hint'>" L_CFG_CAPACITE_HINT "</div></div>");
        resp->print("<div class='row'><label>" L_CFG_ANTIREBOOT "</label>");
        resp->print("<input name='delaiAR' value='" + String(DELAI_ANTI_REBOOT) + "' style='width:60px'><span class='unit'>s</span>");
        resp->print("<div class='hint'>" L_CFG_ANTIREBOOT_HINT "</div></div>");
        resp->print("<div class='row'><label>" L_CFG_CADENCE "</label>");
        resp->print("<select name='intervalleCheck'>");
        int ichOpts[] = {10, 15, 30, 45, 60};
        const char* ichLabels[] = {"10 min","15 min","30 min","45 min","60 min"};
        for (int i = 0; i < 5; i++) {
          resp->print("<option value='" + String(ichOpts[i]) + "'" + (INTERVALLE_CHECK_MIN == ichOpts[i] ? " selected" : "") + ">" + ichLabels[i] + "</option>");
        }
        resp->print("</select>");
        resp->print("<div class='hint'>" L_CFG_CADENCE_HINT "</div></div>");

        resp->print("<div class='row'><label>" L_CFG_BLOCAGE "</label>");
        resp->print("<select name='dureeBloc'>");
        int opts[] = {600, 900, 1800, 3600, 7200, 10800};
        const char* labels[] = {"10 min","15 min","30 min","1h","2h","3h"};
        for (int i = 0; i < 6; i++) {
          resp->print("<option value='" + String(opts[i]) + "'" + (DUREE_BLOCAGE_S == opts[i] ? " selected" : "") + ">" + labels[i] + "</option>");
        }
        resp->print("</select></div>");

        resp->print("<hr class='sep'>");

        resp->print("<div style='color:#555;font-size:0.75rem'>" L_CFG_COUPURE_DYN "<span style='color:#ff8c00'>" + String(VOLT_COUPURE_DYN, 2) + " V</span></div>");

        resp->print("</div>"); // fin panel Batterie

        // --- BOUTON ---
        resp->print("<br><button class='btn' type='submit'>" L_BTN_ENREGISTRER "</button>");

        resp->print(" <a href='/' class='btn'>" L_BTN_RETOUR "</a>");

        resp->print("</form></body></html>");

        request->send(resp);
    });

    server.on("/saveconfig", HTTP_POST, [](AsyncWebServerRequest *request){
        // Général
        if (request->hasArg("owm"))      { OWM_KEY = request->arg("owm"); OWM_KEY.trim(); }
        if (request->hasArg("lat"))       LATITUDE            = request->arg("lat").toFloat();
        if (request->hasArg("lon"))       LONGITUDE           = request->arg("lon").toFloat();
        if (request->hasArg("tilt"))      TILT                = request->arg("tilt").toInt();
        if (request->hasArg("azimuth"))   AZIMUTH             = request->arg("azimuth").toInt();
        if (request->hasArg("ratio"))     RATIO_PONT_DIVISEUR = request->arg("ratio").toFloat();
        if (request->hasArg("hSoir"))     { HEURE_SOMMEIL_SOIR  = constrain(request->arg("hSoir").toInt(),  0, 23); }
        if (request->hasArg("mSoir"))     { MIN_SOMMEIL_SOIR    = constrain(request->arg("mSoir").toInt(),  0, 59); }
        if (request->hasArg("nbReleves")) { NB_RELEVES_NUIT     = constrain(request->arg("nbReleves").toInt(), 1, 5); }
        if (request->hasArg("hJour"))     { HEURE_REVEIL_JOUR   = constrain(request->arg("hJour").toInt(),  0, 23); }
        if (request->hasArg("mJour"))     { MIN_REVEIL_JOUR     = constrain(request->arg("mJour").toInt(),  0, 59); }
        // Luminosité
        if (request->hasArg("luxDefaut")) LUX_ON_DEFAUT       = request->arg("luxDefaut").toFloat();
        if (request->hasArg("luxAmp"))    LUX_ON_AMPLITUDE    = request->arg("luxAmp").toFloat();
        if (request->hasArg("luxConf"))   LUX_CONFIANCE_MAX   = request->arg("luxConf").toFloat();
        // Batterie
        if (request->hasArg("vPleine"))   VOLT_PLEINE         = request->arg("vPleine").toFloat();
        if (request->hasArg("vReprise"))  VOLT_REPRISE        = request->arg("vReprise").toFloat();
        if (request->hasArg("vCoupure"))  VOLT_COUPURE        = request->arg("vCoupure").toFloat();
        if (request->hasArg("vCritique")) VOLT_CRITIQUE       = request->arg("vCritique").toFloat();
        if (request->hasArg("vMin"))      VOLT_COUPURE_MIN    = request->arg("vMin").toFloat();
        if (request->hasArg("capBat"))    CAPACITE_BAT        = constrain(request->arg("capBat").toFloat(), 1.0, 500.0);
        if (request->hasArg("intervalleCheck")) {
          int v = request->arg("intervalleCheck").toInt();
          int allowed[] = {10, 15, 30, 45, 60};
          for (int i = 0; i < 5; i++) { if (v == allowed[i]) { INTERVALLE_CHECK_MIN = v; break; } }
        }
        if (request->hasArg("delaiAR"))   DELAI_ANTI_REBOOT   = constrain(request->arg("delaiAR").toInt(), 10, 300);
        if (request->hasArg("dureeBloc")) DUREE_BLOCAGE_S     = request->arg("dureeBloc").toInt();
        if (request->hasArg("avanceCond"))  AVANCE_CONDENSER = constrain(request->arg("avanceCond").toInt(), 0, 3);
        // Réseau IP statique — validation basique (fromString retourne false si invalide)
        if (request->hasArg("cfgIP"))   { IPAddress t; if(t.fromString(request->arg("cfgIP")))   CFG_IP   = request->arg("cfgIP"); }
        if (request->hasArg("cfgGW"))   { IPAddress t; if(t.fromString(request->arg("cfgGW")))   CFG_GW   = request->arg("cfgGW"); }
        if (request->hasArg("cfgMask")) { IPAddress t; if(t.fromString(request->arg("cfgMask"))) CFG_MASK = request->arg("cfgMask"); }
        if (request->hasArg("cfgDNS"))  { IPAddress t; if(t.fromString(request->arg("cfgDNS")))  CFG_DNS  = request->arg("cfgDNS"); }

        sauvegarderConfig();
        // owmDernierJour et actualiserParametresDynamiques écrivent des variables
        // protégées par xMutexMeteo — prise du mutex obligatoire (callback Core 0)
        if (xMutexMeteo) xSemaphoreTake(xMutexMeteo, portMAX_DELAY);
        owmDernierJour = -1; // force MAJ météo si lat/lon/clé ont changé
        actualiserParametresDynamiques();
        if (xMutexMeteo) xSemaphoreGive(xMutexMeteo);

        request->send(200, "text/html",
            "<!DOCTYPE html><html><head><meta charset='UTF-8'>"
            "<meta http-equiv='refresh' content='2;url=/config'></head>"
            "<body style='background:#0d0d0d;color:#00c896;font-family:monospace;padding:20px'>"
            L_CFG_SAUVEGARDEE "</body></html>");
    });

    // /doc : documentation du système
    server.on("/doc", HTTP_GET, [](AsyncWebServerRequest *request){
        AsyncResponseStream *resp = request->beginResponseStream("text/html");
        resp->print("<!DOCTYPE html><html><head><meta charset='UTF-8'>");
        resp->print("<meta name='viewport' content='width=device-width,initial-scale=1'>");
        resp->print("<title>" L_DOC_TITRE "</title>");
        resp->print("<style>"
          "body{background:#0d0d0d;color:#ccc;font-family:monospace;padding:16px;max-width:700px;margin:auto;}"
          "h1{color:#ff8c00;font-size:1.1rem;margin-top:0;}"
          "h2{color:#ffeb3b;font-size:0.95rem;margin-top:24px;border-bottom:1px solid #222;padding-bottom:4px;}"
          "h3{color:#00c896;font-size:0.85rem;margin-top:16px;}"
          "p,li{font-size:0.8rem;line-height:1.6;color:#aaa;}"
          "ul{padding-left:18px;}"
          "code{background:#1a1a1a;color:#ff8c00;padding:1px 5px;border-radius:3px;font-size:0.8rem;}"
          ".badge{display:inline-block;padding:2px 8px;border-radius:4px;font-size:0.75rem;margin:2px;}"
          ".green{background:#00c89622;border:1px solid #00c896;color:#00c896;}"
          ".orange{background:#ff8c0022;border:1px solid #ff8c00;color:#ff8c00;}"
          ".red{background:#ff3b3b22;border:1px solid #ff3b3b;color:#ff3b3b;}"
          ".back{display:inline-block;margin-bottom:16px;color:#888;text-decoration:none;font-size:0.8rem;}"
          ".btn{display:inline-block;background:#1a1a1a;color:#555;border:1px solid #333;border-radius:6px;padding:8px 16px;cursor:pointer;font-family:monospace;font-size:0.8rem;text-decoration:none;margin-top:12px;}"
          ".btn:hover{background:#333;color:#eee;text-decoration:none;}"
          "</style></head><body>");
        menuHamburger(resp);
        resp->print("<h1>" L_DOC_TITRE "</h1>");
        resp->print("<a href='/' class='btn'>" L_BTN_RETOUR "</a><br><br>");

        resp->print("<h2>" L_DOC_MATERIEL "</h2>");
        resp->print("<h3>" L_DOC_CAPTEURS "</h3><ul>");
        resp->print("<li><code>VEML7700</code> — " L_DOC_VEML "</li>");
        resp->print("<li><code>INA226</code> — " L_DOC_INA "</li>");
        resp->print("<li><code>GPIO 34</code> — " L_DOC_GPIO34 "</li>");
        resp->print("</ul>");
        resp->print("<h3>" L_DOC_SORTIES "</h3><ul>");
        resp->print("<li><code>GPIO 18</code> — " L_DOC_GPIO18 "</li>");
        resp->print("<li><code>GPIO 2</code> — " L_DOC_GPIO2 "</li>");
        resp->print("</ul>");

        resp->print("<h2>" L_DOC_BADGES "</h2><ul>");
        resp->print("<li><span class='badge green'>V</span> " L_DOC_BADGE_V "</li>");
        resp->print("<li><span class='badge green'>" L_DOC_LBL_CHARGE "</span> " L_DOC_BADGE_CHARGE "</li>");
        resp->print("<li><span class='badge orange'>Lux</span> " L_DOC_BADGE_LUX "</li>");
        resp->print("<li><span class='badge orange'>" L_DOC_LBL_SEUILON "</span> " L_DOC_BADGE_SEUIL "</li>");
        resp->print("<li><span class='badge orange'>" L_DOC_LBL_COUPURE "</span> " L_DOC_BADGE_COUPURE "</li>");
        resp->print("<li>" L_DOC_BADGE_SOC "</li>");
        resp->print("<li><span class='badge red'>&#9888; " L_BADGE_HORS_MPPT "</span> " L_DOC_BADGE_HORS "</li>");
        resp->print("<li><span class='badge orange'>&#9888; " L_BADGE_BLOCAGE "</span> " L_DOC_BADGE_BL "</li>");
        resp->print("</ul>");

        resp->print("<h2>" L_DOC_GRAPHIQUES "</h2><ul>");
        resp->print("<li>" L_DOC_G_PRESENT "</li>");
        resp->print("<li>" L_DOC_G_NOCTURNE "</li>");
        resp->print("<li>" L_DOC_G_ENERGIE "</li>");
        resp->print("<li>" L_DOC_G_BILAN "</li>");
        resp->print("<li>" L_DOC_G_FUTUR "</li>");
        resp->print("<li>" L_DOC_G_PASSE "</li>");
        resp->print("</ul>");

        resp->print("<h2>" L_DOC_LOGIQUE "</h2><ul>");
        resp->print("<li>" L_DOC_L_ON "</li>");
        resp->print("<li>" L_DOC_L_OFF "</li>");
        resp->print("<li>" L_DOC_L_DYN "</li>");
        resp->print("</ul>");

        resp->print("<h2>" L_DOC_P_TITRE "</h2>");
        resp->print("<p>" L_DOC_P_TEXTE1 "</p>");
        resp->print("<p>" L_DOC_P_TEXTE2 "</p>");

        resp->print("<h2>" L_DOC_S_TITRE "</h2>");
        resp->print("<p>" L_DOC_S_TEXTE1 "</p>");
        resp->print("<p>" L_DOC_S_TEXTE2 "</p>");

        resp->print("<h2>" L_DOC_V_TITRE "</h2><ul>");
        resp->print("<li>" L_DOC_V_LI1 "</li>");
        resp->print("<li>" L_DOC_V_LI2 "</li>");
        resp->print("<li>" L_DOC_V_LI3 "</li>");
        resp->print("</ul>");

        resp->print("<h2>" L_DOC_VER_TITRE "</h2>");
        resp->print("<p>" L_DOC_VER_TEXTE "</p>");

        resp->print("<a href='/' class='btn'>" L_BTN_RETOUR "</a>");
        resp->print("</body></html>");
        request->send(resp);
    });

    // /debug : affichage des derniers logs système (30 entrées circulaires)
    server.on("/debug", HTTP_GET, [](AsyncWebServerRequest *request){
        AsyncResponseStream *resp = request->beginResponseStream("text/html");
        resp->print("<!DOCTYPE html><html><head><meta charset='UTF-8'>");

        resp->print("<meta name='viewport' content='width=device-width,initial-scale=1'>");

        resp->print("<meta http-equiv='refresh' content='60'>"); // FIX: 60s au lieu de 10s
        resp->print("<style>");

        resp->print("body{background:#0d0d0d;color:#eee;font-family:monospace;padding:12px;margin:0;font-size:0.8rem;}");

        resp->print("h2{color:#555;font-size:0.9rem;letter-spacing:0.1em;margin:0 0 12px;}");

        resp->print(".line{padding:3px 6px;border-bottom:1px solid #111;white-space:pre;}");

        resp->print(".line:nth-child(odd){background:#0a0a0a;}");

        resp->print(".ok{color:#00c896;} .err{color:#ff3b3b;} .info{color:#4dabf7;} .warn{color:#ffeb3b;}");

        resp->print(".btn,.btn-orange{display:inline-block;box-sizing:border-box;border-radius:6px;padding:8px 16px;cursor:pointer;font-family:monospace;margin-top:12px;font-size:0.8rem;line-height:1.2;text-decoration:none;vertical-align:middle;}");
        resp->print(".btn{background:#1a1a1a;color:#555;border:1px solid #333;}");
        resp->print(".btn:hover{background:#333;color:#eee;text-decoration:none;}");
        resp->print(".btn-orange{background:#1a1a1a;color:#ff8c00;border:1px solid #ff8c00;margin-left:8px;}");
        resp->print(".btn-orange:hover{background:#ff8c00;color:#000;}");

        resp->print("</style></head><body>");
        menuHamburger(resp);
        resp->print("<h2>" L_DBG_TITRE " <span style='color:#333;font-size:0.7rem'>" L_DBG_REFRESH "</span></h2>");
        resp->print("<a href='/' class='btn'>" L_BTN_RETOUR "</a><br><br>");

        // Cause du dernier reset — en rouge si crash
          String reason = getCrashReason();
          bool isCrash = (esp_reset_reason() == ESP_RST_PANIC ||
                          esp_reset_reason() == ESP_RST_INT_WDT ||
                          esp_reset_reason() == ESP_RST_TASK_WDT ||
                          esp_reset_reason() == ESP_RST_BROWNOUT);
          String col = isCrash ? "#ff3b3b" : "#00c896";
          resp->print("<div style='margin-bottom:10px;padding:6px 10px;border:1px solid " + col +
                      ";border-radius:4px;color:" + col + ";font-size:0.78rem;'>");
          resp->print((isCrash ? L_DBG_RST_CRASH_LBL : L_DBG_RST_OK_LBL) + reason);
          resp->print("</div>");

        resp->print("<div style='background:#111;border:1px solid #222;border-radius:6px;padding:4px;'>");

        // Parcours chronologique du buffer circulaire — sous mutex (addLog appelé des deux cores)
        if (xMutexLog){ xSemaphoreTake(xMutexLog, portMAX_DELAY);
          for (int i = 0; i < LOG_MAX; i++) {
            int idx = (logIndex + i) % LOG_MAX;
            if (logBuffer[idx][0] == '\0') continue;  // case vide
            String css = "info";
            if (strstr(logBuffer[idx], "[CHARGE] ON"))  css = "ok";
            if (strstr(logBuffer[idx], "[CHARGE] OFF")) css = "warn";
            if (strstr(logBuffer[idx], "non trouve"))   css = "err";
            if (strstr(logBuffer[idx], "Echec"))        css = "err";
            if (strstr(logBuffer[idx], "[CRASH]"))      css = "err";
            if (strstr(logBuffer[idx], "[SLEEP]"))      css = "warn";
            if (strstr(logBuffer[idx], "[MESURE]"))     css = "info";
            if (strstr(logBuffer[idx], "[METEO] MAJ"))  css = "ok";
            if (strstr(logBuffer[idx], "[BOOT]"))       css = "ok";
            resp->print("<div class='line " + css + "'>" + String(logBuffer[idx]) + "</div>");
          }
        }
        if (xMutexLog) xSemaphoreGive(xMutexLog);
        resp->print("</div>");

        // Infos système en temps réel
        resp->print("<div style='margin-top:12px;color:#333;font-size:0.75rem;'>");

        resp->print(L_DBG_HEAP + String(ESP.getFreeHeap()) + L_DBG_OCTETS);

        resp->print(L_DBG_UPTIME + String(millis()/1000) + "s — ");

        resp->print(L_DBG_WIFI + WiFi.localIP().toString());
        resp->print("</div>");

        resp->print("<br><a href='/' class='btn'>" L_BTN_RETOUR "</a>");
        // Bouton désactivation anti-bootloop (visible seulement si actif)
        if (rtc_mosfetBloqueJusquA > 0 && esp_timer_get_time() < rtc_mosfetBloqueJusquA) {
          resp->print("<button class='btn-orange' onclick=\"if(confirm('" L_DBG_BL_CONFIRM "'))window.location.href='/cancelbl'\">" L_DBG_BL_BTN "</button>");
        }

        resp->print("</body></html>");

        request->send(resp);
    });

    // /cancelbl : désactivation manuelle du blocage anti-bootloop
    server.on("/cancelbl", HTTP_GET, [](AsyncWebServerRequest *request){
        rtc_mosfetBloqueJusquA = 0;
        { NVSState _s={}; prefs.begin("solar",true); prefs.getBytes("st",&_s,sizeof(_s)); prefs.end();
          _s.blocageS=0; _s.arMosfetHigh=0; _s.arCount=0;
          prefs.begin("solar",false); prefs.putBytes("st",&_s,sizeof(_s)); prefs.end(); }
        addLog("[CHARGE] Anti-bootloop désactivé manuellement");
        Serial.println("[CHARGE] Anti-bootloop désactivé manuellement");

        // Évaluation MOSFET immédiate — pas d'attente du prochain lastCheck
        uint32_t mv = analogReadMilliVolts(BAT_PIN);
        float v = (mv > 0) ? (mv / 1000.0) * RATIO_PONT_DIVISEUR : 0.0;
        float lux = luxDernier;
        float _coupDyn = VOLT_COUPURE_DYN, _luxOn = CURRENT_LUX_ON; bool _batOK = batterieEstOK;
        if (xMutexMeteo) { xSemaphoreTake(xMutexMeteo, portMAX_DELAY); _coupDyn = VOLT_COUPURE_DYN; _luxOn = CURRENT_LUX_ON; _batOK = batterieEstOK; xSemaphoreGive(xMutexMeteo); }
        if (lux > _luxOn && v > VOLT_REPRISE && _batOK && v > _coupDyn) {
          digitalWrite(MOSFET_PIN, HIGH);
          rtc_mosfetWasHigh = true;
          addLog("[CHARGE] ON après annulation blocage " + String(v, 2) + "V " + String(lux, 0) + "lx");
        } else {
          digitalWrite(MOSFET_PIN, LOW);
          rtc_mosfetWasHigh = false;
          addLog("[CHARGE] OFF après annulation blocage " + String(v, 2) + "V " + String(lux, 0) + "lx");
        }

        request->send(200, "text/html",
          "<!DOCTYPE html><html><head><meta charset='UTF-8'>"
          "<meta http-equiv='refresh' content='2;url=/'></head>"
          "<body style='background:#0d0d0d;color:#00c896;font-family:monospace;padding:20px'>"
          L_DBG_BL_OK "</body></html>");
    });

    server.on("/upd", HTTP_GET, [](AsyncWebServerRequest *request){ 
        Serial.println(">>> REQUETE /upd RECUE !");
        demandeMeteo = true;
        String page;
        page.reserve(900);
        page = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
        page += "<style>body{background:#0d0d0d;color:#eee;font-family:monospace;display:flex;";
        page += "flex-direction:column;align-items:center;justify-content:center;height:100vh;margin:0;}";
        page += ".spinner{width:40px;height:40px;border:3px solid #222;border-top-color:#ff8c00;";
        page += "border-radius:50%;animation:spin 0.8s linear infinite;margin-bottom:20px;}";
        page += "@keyframes spin{to{transform:rotate(360deg)}}";
        page += ".msg{color:#888;font-size:0.85rem;margin-top:10px;}</style></head><body>";
        page += "<div class='spinner'></div>";
        page += "<div style='color:#ff8c00;font-size:1rem;'>" L_UPD_TITRE "</div>";
        page += "<div class='msg' id='info'>" L_UPD_SOUS_TITRE "</div>";
        page += "<script>";
        page += "var t=setInterval(function(){";
        page += "fetch('/status').then(r=>r.text()).then(s=>{";
        page += "if(s==='0'){clearInterval(t);window.location.href='/';}";
        page += "else{document.getElementById('info').textContent='" L_UPD_TRAITEMENT "';}";
        page += "}).catch(()=>{});";
        page += "},2000);";
        page += "</script></body></html>";
        request->send(200, "text/html", page);
    });

    server.on("/save", HTTP_POST, [](AsyncWebServerRequest *request){
    if (request->hasArg("s") && request->hasArg("p")) {
        String ssid_clair = request->arg("s");
        String pass_clair = request->arg("p");
        
        ssid_clair.trim();
        pass_clair.trim();

        unsigned char s_crypte[64] = {0};
        unsigned char p_crypte[64] = {0};

        // W2 FIX : génération d'un IV aléatoire unique à chaque sauvegarde.
        // L'IV est persisté en NVS et sera relu par setupWiFi() pour déchiffrer.
        unsigned char iv_rand[16];
        esp_fill_random(iv_rand, 16);

        encryptLong(ssid_clair, s_crypte, iv_rand);
        encryptLong(pass_clair, p_crypte, iv_rand);

        prefs.begin("config", false); 
        prefs.putBytes("s", s_crypte, 64);
        prefs.putBytes("p", p_crypte, 64);
        prefs.putBytes("iv", iv_rand, 16); // W2 FIX : on stocke l'IV
        prefs.end();
        // Clé OWM sauvegardée dans namespace "params" (lu par chargerConfig)
        // Note : namespace "config" ne contient que les credentials WiFi chiffrés
        if (request->hasArg("owm") && request->arg("owm").length() > 0) {
            prefs.begin("params", false);
            prefs.putString("owmkey", request->arg("owm"));
            prefs.end();
            Serial.println("[OWM] Clé sauvegardée en NVS (params).");
        }
        
        Serial.println("[OK] Données cryptées et sauvegardées (IV aléatoire) !");
        
        request->send(200, "text/plain", "Configuration enregistree. Reboot...");
        
        delay(2000); 
        ESP.restart();
    } else {
        request->send(400, "text/plain", "Erreur : SSID ou Pass manquant");
    }
});
}

//gestion des deconnexions
void setupWiFiEvents() {
  // FIX WIFI REVEIL : un seul mécanisme de reconnexion — setAutoReconnect(true).
  // Plus de WiFi.reconnect() dans l'event handler : ça créait une race condition
  // au réveil du deep sleep (handler DISCONNECT déclenché par disconnect(true) dans
  // setupWiFi, avant que les credentials soient chargés → reconnect sans credentials).
  // setAutoReconnect(true) suffit : le driver WiFi gère seul la reconnexion.
  // checkWiFiReconnect() reste en filet de sécurité ultime (5 min sans connexion).
  WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info){
    switch(event) {
      case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        addLog("[WIFI] connecté " + WiFi.localIP().toString());
        wifiConnecting = false;
        wifiOK = true;
        lastGotIP = millis();
        break;

      case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        wifiOK = false;
        wifiConnecting = false;
        if (!isGoingToSleep) {
          // setAutoReconnect(true) relance automatiquement — on logue seulement
          addLog("[WIFI] déconnecté — autoReconnect actif");
        }
        break;

      default:
        break;
    }
  });
}

// Elle reste disponible uniquement pour le démarrage initial (setupWiFi).
// La reconnexion en cours de vie est assurée par WiFi.setAutoReconnect(true)
// sans jamais toucher à WiFi.mode() ni WiFi.disconnect() qui tuent AsyncTCP.
void reconnectWiFiSafe() {
  // Filet de sécurité ultime — appelé depuis checkWiFiReconnect() si autoReconnect n'a pas suffi
  if (wifiConnecting || WiFi.status() == WL_CONNECTED) return;
  if (WiFi.getMode() != WIFI_STA) return;
  wifiConnecting = true;
  Serial.println("[WIFI] Reconnexion réseau (WiFi.reconnect)...");
  addLog("[WIFI] reconnect filet...");
  WiFi.reconnect();
  // wifiConnecting sera remis à false par l'event GOT_IP ou DISCONNECTED
}

void checkWiFiReconnect() {
  // setAutoReconnect(true) gère la reconnexion automatique en temps normal.
  // Cette fonction est un filet de sécurité : si autoReconnect n'a pas suffi après 5min,
  // on tente un WiFi.reconnect() doux, une seule fois toutes les 5 minutes.
  if (WiFi.getMode() != WIFI_STA) return;
  if (WiFi.status() == WL_CONNECTED) { wifiConnecting = false; return; }
  if (isGoingToSleep) return;
  if (millis() - lastWifiAttempt > 300000UL) { // 5 min — filet si autoReconnect n'a pas suffi
    lastWifiAttempt = millis();
    reconnectWiFiSafe();
  }
}

void checkHeap() {
  // Conservée pour compatibilité — la vraie vérif heap est dans le bloc lastMem de loop()
  // Cette fonction ne fait rien (évite de dupliquer la logique)
}

void setupWiFi() {
    if (wifiConnecting) return;
    wifiConnecting = true;
    addLog("[WIFI] connection...");

    Serial.println("\n[ETAPE 1] Lecture Preferences");
    prefs.begin("config", true);
    unsigned char encS[64] = {0}, encP[64] = {0};
    bool configExiste = (prefs.getBytes("s", encS, 64) > 0 && prefs.getBytes("p", encP, 64) > 0);
    unsigned char iv_stored[16] = {0};
    if (configExiste) prefs.getBytes("iv", iv_stored, 16);
    prefs.end();

    if (configExiste) {
        String s = decryptLong(encS, iv_stored);
        String p = decryptLong(encP, iv_stored);
        Serial.print("Tentative SSID : "); Serial.println(s);

        // FIX REVEIL DEEP SLEEP : NE PAS appeler WiFi.disconnect(true) avant begin().
        // disconnect(true) efface les credentials du driver ET déclenche l'event
        // ARDUINO_EVENT_WIFI_STA_DISCONNECTED → race condition avec le handler.
        // On configure le mode sans reset du driver.
        WiFi.persistent(false);
        WiFi.mode(WIFI_STA);
        WiFi.setAutoReconnect(true); // le driver gère seul toutes les reconnexions
        WiFi.setTxPower(WIFI_POWER_19_5dBm);
        uint8_t mac[6]; esp_read_mac(mac, ESP_MAC_WIFI_STA);
        char macStr[18]; snprintf(macStr,sizeof(macStr),"%02X:%02X:%02X:%02X:%02X:%02X",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
        Serial.println("[WIFI] MAC (eFuse) : " + String(macStr));

        // IP statique — chargée depuis NVS (configurable dans /config)
        IPAddress _ip, _gw, _mask, _dns;
        _ip.fromString(CFG_IP);   _gw.fromString(CFG_GW);
        _mask.fromString(CFG_MASK); _dns.fromString(CFG_DNS);
        WiFi.config(_ip, _gw, _mask, _dns);
        WiFi.begin(s.c_str(), p.c_str());
        // Attente 30s max — puis autoReconnect + checkWiFiReconnect prennent le relais
        int retry = 0;
        while (WiFi.status() != WL_CONNECTED && retry < 30) { delay(1000); Serial.print("."); retry++; }
    }

    if (WiFi.status() == WL_CONNECTED) {
        wifiConnecting = false;
        Serial.println("\n[SUCCES] IP: " + WiFi.localIP().toString());
    } else if (!configExiste) {
        // Aucun credential enregistré → mode AP pour configuration initiale
        wifiConnecting = false; // FIX : reset du flag même en mode AP
        Serial.println("\n[MODE AP] SSID: ESP32-SOLAIRE-CONFIG");
        WiFi.mode(WIFI_AP);
        WiFi.softAP("ESP32-SOLAIRE-CONFIG");
        delay(1000);
        dnsServer.start(53, "*", WiFi.softAPIP());
        server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
            String h = "<html><body><h2>" L_AP_TITRE "</h2><form action='/save' method='POST'>";
            h += "SSID:<br><input name='s'><br>Password:<br><input type='password' name='p'><br><br>";
            h += "OWM API key:<br><input name='owm' value='" + OWM_KEY + "' style='width:260px'><br><br>";
            h += "<input type='submit' value='" L_AP_BTN_SAVE "'></form></body></html>";
            request->send(200, "text/html", h);
        });
    } else {
        // Credentials OK mais box pas encore joignable → rester en STA, ne PAS passer en AP
        // autoReconnect(true) + checkWiFiReconnect() relanceront automatiquement
        wifiConnecting = false;
        Serial.println("\n[WIFI] Pas connecté après 30s — autoReconnect actif");
        addLog("[WIFI] init timeout — autoReconnect actif");
    }
    configurerRoutesServeur();
    server.begin();
    DefaultHeaders::Instance().addHeader("Connection", "close");
    DefaultHeaders::Instance().addHeader("Cache-Control", "no-store");
    // Blink LED 3× : serveur web opérationnel (cold boot uniquement)
    if (esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_TIMER) {
      for (int i = 0; i < 3; i++) {
        digitalWrite(LED_PIN, HIGH); delay(120);
        digitalWrite(LED_PIN, LOW);  delay(120);
      }
    }
}
// ==========================================================================
// PERSISTANCE CONFIGURATION (NVS namespace "params")
// ==========================================================================
void chargerConfig() {
  prefs.begin("params", true); // read-only
  RATIO_PONT_DIVISEUR = prefs.getFloat("ratio",     5.5454);
  VOLT_PLEINE         = prefs.getFloat("vPleine",   13.50);
  VOLT_REPRISE        = prefs.getFloat("vReprise",  12.30);
  VOLT_COUPURE        = prefs.getFloat("vCoupure",  12.10);
  VOLT_CRITIQUE       = prefs.getFloat("vCritique", 12.20);
  VOLT_COUPURE_MIN    = prefs.getFloat("vMin",      11.90);
  LUX_ON_DEFAUT       = prefs.getFloat("luxDefaut", 1500.0);
  LUX_ON_AMPLITUDE    = prefs.getFloat("luxAmp",    1400.0);
  LUX_CONFIANCE_MAX   = prefs.getFloat("luxConf",   6000.0);
  LATITUDE            = prefs.getFloat("lat",        43.4614);
  LONGITUDE           = prefs.getFloat("lon",         1.2457);
  TILT                = prefs.getInt  ("tilt",        90);
  AZIMUTH             = prefs.getInt  ("azimuth",    290);
  NB_RELEVES_NUIT     = prefs.getInt  ("nbReleves",    2);
  HEURE_REVEIL_JOUR   = prefs.getInt  ("hJour",       9);
  MIN_REVEIL_JOUR     = prefs.getInt  ("mJour",      15);
  HEURE_SOMMEIL_SOIR  = prefs.getInt  ("hSoir",      19);
  MIN_SOMMEIL_SOIR    = prefs.getInt  ("mSoir",      45);
  OWM_KEY             = prefs.getString("owmkey", OWM_KEY_DEFAUT);
  CAPACITE_BAT        = prefs.getFloat("capBat", 11.5);
  INTERVALLE_CHECK_MIN = prefs.getInt  ("intervalleChk", 10);
  DELAI_ANTI_REBOOT   = prefs.getInt  ("delaiAR", 60);
  DUREE_BLOCAGE_S     = prefs.getInt  ("dureeBloc", 3600);
  AVANCE_CONDENSER     = prefs.getInt("avanceCond", 1);
  CFG_IP               = prefs.getString("cfgIP",   "192.168.1.253");
  CFG_GW               = prefs.getString("cfgGW",   "192.168.1.1");
  CFG_MASK             = prefs.getString("cfgMask", "255.255.255.0");
  CFG_DNS              = prefs.getString("cfgDNS",  "8.8.8.8");
  prefs.end();
  VOLT_COUPURE_DYN = VOLT_COUPURE; // init prudente
  Serial.println("[CFG] Configuration chargée depuis NVS.");
}

void sauvegarderConfig() {
  prefs.begin("params", false);
  prefs.putFloat("ratio",     RATIO_PONT_DIVISEUR);
  prefs.putFloat("vPleine",   VOLT_PLEINE);
  prefs.putFloat("vReprise",  VOLT_REPRISE);
  prefs.putFloat("vCoupure",  VOLT_COUPURE);
  prefs.putFloat("vCritique", VOLT_CRITIQUE);
  prefs.putFloat("vMin",      VOLT_COUPURE_MIN);
  prefs.putFloat("luxDefaut", LUX_ON_DEFAUT);
  prefs.putFloat("luxAmp",    LUX_ON_AMPLITUDE);
  prefs.putFloat("luxConf",   LUX_CONFIANCE_MAX);
  prefs.putFloat("lat",       LATITUDE);
  prefs.putFloat("lon",       LONGITUDE);
  prefs.putInt  ("tilt",      TILT);
  prefs.putInt  ("azimuth",   AZIMUTH);
  prefs.putInt  ("nbReleves", NB_RELEVES_NUIT);
  prefs.putInt  ("hJour",     HEURE_REVEIL_JOUR);
  prefs.putInt  ("mJour",     MIN_REVEIL_JOUR);
  prefs.putInt  ("hSoir",     HEURE_SOMMEIL_SOIR);
  prefs.putInt  ("mSoir",     MIN_SOMMEIL_SOIR);
  prefs.putString("owmkey",   OWM_KEY);
  prefs.putFloat ("capBat",   CAPACITE_BAT);
  prefs.putInt   ("intervalleChk", INTERVALLE_CHECK_MIN);
  prefs.putInt   ("delaiAR",   DELAI_ANTI_REBOOT);
  prefs.putInt   ("dureeBloc", DUREE_BLOCAGE_S);
  prefs.putInt   ("avanceCond", AVANCE_CONDENSER);
  prefs.putString("cfgIP",   CFG_IP);
  prefs.putString("cfgGW",   CFG_GW);
  prefs.putString("cfgMask", CFG_MASK);
  prefs.putString("cfgDNS",  CFG_DNS);
  prefs.end();
  Serial.println("[CFG] Configuration sauvegardée en NVS.");
}

// ==========================================================================
// 6. SETUP & LOOP
// ==========================================================================
void setup() {
  Serial.begin(115200);
  
  // chargerLogs() EN PREMIER : restaure le buffer avant tout addLog()
  // pour conserver l'historique des logs qui précèdent un crash/reboot
  chargerLogs();

  // chargerConfig() EN PREMIER : DELAI_ANTI_REBOOT et DUREE_BLOCAGE_S doivent
  // être chargés avant la détection boot loop.

  chargerConfig();
  Wire.begin(21, 22);   // Wire0 pour VEML
  Wire1.begin(19, 23);  // Wire1 pour INA226
  
  // Log de la cause du reboot — visible dans /debug même après un crash
  String reason = getCrashReason();
  bool isCrash = (esp_reset_reason() == ESP_RST_PANIC ||
                  esp_reset_reason() == ESP_RST_INT_WDT ||
                  esp_reset_reason() == ESP_RST_TASK_WDT ||
                  esp_reset_reason() == ESP_RST_BROWNOUT);
  addLog(String(isCrash ? "[CRASH] " : "[BOOT] ") + reason);
  Serial.println("[BOOT] Cause reset : " + reason);
 
  // --- Détection boot loop (chute tension MPPT → coupure → reboot) ---
  // RTC memory effacée si power-on complet → on lit mosfetWasHigh/dernierCourant depuis NVS.
  // Un compteur NVS "rebootRapide" est incrémenté à chaque boot et remis à 0 après
  // DELAI_ANTI_REBOOT secondes de fonctionnement stable.
    int64_t maintenant = esp_timer_get_time(); // µs depuis démarrage puce

    // Lire état précédent depuis NVS via NVSState
    NVSState _bs={};
    prefs.begin("solar", false);
    prefs.getBytes("st", &_bs, sizeof(_bs));
    bool  nvsModemHigh    = (_bs.arMosfetHigh != 0);
    float nvsCourant      = _bs.arCourant;
    int   blocageRestantS = _bs.blocageS;

    // FIX 123f-r1 : ne pas incrémenter arCount sur un réveil timer (réveils nocturnes).
    // Avant ce fix, chaque réveil nocturne (tronçon 1, 2) incrémentait arCount,
    // déclenchant un faux positif anti-bootloop au réveil matin (arCount atteignait 3).
    bool isTimerWakeup = (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER);
    int  nvsRebootCount = isTimerWakeup ? _bs.arCount : _bs.arCount + 1;

    if (!isTimerWakeup && nvsRebootCount >= 3 && nvsModemHigh) {
      // Boot loop détecté (vrais reboots applicatifs uniquement)
      rtc_mosfetBloqueJusquA = maintenant + (int64_t)DUREE_BLOCAGE_S * 1000000LL;
      _bs.arCount=0; _bs.blocageS=DUREE_BLOCAGE_S; _bs.arMosfetHigh=0;
      prefs.putBytes("st",&_bs,sizeof(_bs));
      prefs.end();
      char msg[80];
      snprintf(msg, sizeof(msg),
        "[CHARGE] SUSPICION charge faible — MOSFET OFF %ds (I=%.3fA boots=%d)",
        DUREE_BLOCAGE_S, nvsCourant, nvsRebootCount);
      addLog(msg);
      Serial.println(msg);
    } else if (blocageRestantS > 0) {
      // Blocage en cours — restaurer
      rtc_mosfetBloqueJusquA = maintenant + (int64_t)blocageRestantS * 1000000LL;
      _bs.arCount=0; _bs.arMosfetHigh=0;
      prefs.putBytes("st",&_bs,sizeof(_bs));
      prefs.end();
      char msg[64];
      snprintf(msg, sizeof(msg), "[CHARGE] Blocage boot loop restauré — reste %ds", blocageRestantS);
      addLog(msg);
      Serial.println(msg);
    } else {
      // Boot normal ou réveil timer — arCount inchangé si timer, incrémenté si vrai reboot
      _bs.arCount = nvsRebootCount;
      prefs.putBytes("st",&_bs,sizeof(_bs));
      prefs.end();
    }
    rtc_dernierBootUs = maintenant;
  // Watchdog logiciel : redémarre l'ESP32 si la loop ou la tâche météo
  // se bloque plus de 60s (crash silencieux, allocation mémoire infinie, etc.)
  esp_task_wdt_config_t wdt_cfg = {
    .timeout_ms = 60000,
    .idle_core_mask = 0,
    .trigger_panic = true
  };
  // init() échoue silencieusement si déjà initialisé (ESP_ERR_INVALID_STATE) — sans conséquence
  esp_task_wdt_init(&wdt_cfg);
  esp_task_wdt_reconfigure(&wdt_cfg);
  esp_task_wdt_add(NULL); // abonne la tâche courante (setup/loop sur Core 1)
  Serial.println("[WDT] Watchdog initialisé (60s)");

  // ==========================================================================
  // SETUP COURT — Réveils nocturnes (01h, 04h)
  // Pas de WiFi, pas de serveur, pas de NTP, pas de VEML.
  // On lit la tension, on sauvegarde, on re-dort immédiatement.
  // ==========================================================================
  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER) {
    // FIX TIMEZONE : sans WiFi, configTzTime() doit quand même être appelé pour
    // que getLocalTime() applique CET/CEST. Sans ça → heure UTC → décalage 1h hiver, 2h été.
    configTzTime("CET-1CEST,M3.5.0,M10.5.0/3", "pool.ntp.org");
    Serial.println("[SYSTEM] Réveil DeepSleep : Synchronisation heure NTP");
    addLog("[SYSTEM] Réveil DeepSleep : Synchronisation heure NTP");
    // Restaurer l'heure depuis la RTC interne ESP32 (tourne en deep sleep)
    struct tm ti;
    bool rtcOK = getLocalTime(&ti, 100);  // timeout 100ms — pas de WiFi

    // Réveil intermédiaire si le tronçon écoulé n'est pas encore le dernier.
    // Ce test est basé uniquement sur rtc_troncon, PAS sur l'heure — fonctionne
    // même si getLocalTime échoue (RTC non initialisée après 1er flash).
    bool estReveilNocturne = (rtc_troncon >= 1 && rtc_troncon <= NB_RELEVES_NUIT);

    if (estReveilNocturne) {
      int h = rtcOK ? ti.tm_hour : -1;
      int m = rtcOK ? ti.tm_min  :  0;

      if (rtcOK) {
        char tempLog[60];
        snprintf(tempLog, sizeof(tempLog), "[SETUP COURT] Réveil nocturne %02dh%02d troncon=%d\n", h, m, rtc_troncon);
        Serial.printf(tempLog);
        addLog(tempLog);
      } else {
        // RTC non disponible (1er flash ou dérive extrême) : on re-dort quand même
        // Le relevé tension est sauté (h==-1 bloque la condition ci-dessous)
        addLog("[SETUP COURT] RTC indispo — re-sleep sans releve (troncon=" + String(rtc_troncon) + ")");
        Serial.println("[SETUP COURT] RTC indispo — re-sleep sans releve");
      }

      // Restaurer les variables RTC indispensables
      indexReleve = rtc_indexReleve;
      nbReels     = rtc_nbReels;
      derHeureEnr = rtc_derHeureEnr;
      VOLT_COUPURE_DYN = rtc_VOLT_COUPURE_DYN;

      // Restaurer historiqueTension depuis NVS
      // FIX 123f-r1 : suppression du prefs.begin() orphelin (jamais fermé) qui précédait ce bloc.
      // Un double begin() sans end() intermédiaire peut crasher la tâche ESP32-Arduino.
      { NVSNuit _n={}; prefs.begin("solar", true); prefs.getBytes("nuit",&_n,sizeof(_n)); prefs.end();
        memcpy(historiqueTension, _n.tension, sizeof(historiqueTension)); }

      // Lire la tension batterie (seulement si RTC OK — sinon h==-1 bloque le relevé)
      pinMode(MOSFET_PIN, OUTPUT);
      digitalWrite(MOSFET_PIN, LOW);
      uint32_t mv = analogReadMilliVolts(BAT_PIN);
      float v = (mv > 0) ? (mv / 1000.0) * RATIO_PONT_DIVISEUR : 0.0;

      if (h != -1 && h != derHeureEnr && v > 9.0) {
        historiqueTension[indexReleve] = v;
        indexReleve = (indexReleve + 1) % 40;
        if (nbReels < 40) nbReels++;
        derHeureEnr = h;

        NVSNuit _n; memcpy(_n.tension, historiqueTension, sizeof(historiqueTension));
        _n.idxReleve=indexReleve; _n.nbReels=nbReels; _n.derHeureEnr=derHeureEnr;
        prefs.begin("solar", false); prefs.putBytes("nuit",&_n,sizeof(_n)); prefs.end();
        Serial.printf("[NUIT COURT] relevé %02dh : %.2fV  idx=%d\n", h, v, indexReleve);
      }

      // Mettre à jour RTC memory et re-dormir — toujours exécuté même sans RTC
      rtc_indexReleve = indexReleve;
      rtc_nbReels     = nbReels;
      rtc_derHeureEnr = derHeureEnr;

      int prochainTroncon = rtc_troncon + 1;
      if (prochainTroncon > NB_RELEVES_NUIT + 1) prochainTroncon = 1; // sécurité
      rtc_troncon = prochainTroncon;
      uint64_t duree_s = dureeTroncon(prochainTroncon);
      Serial.printf("[SLEEP COURT] Troncon %d → dans %llus (%lluh%02llum)\n",
                    prochainTroncon, duree_s, duree_s/3600, (duree_s%3600)/60);
      Serial.flush();
      delay(100);
      sauvegarderLogs(); // persiste logs avant sleep nocturne
      esp_sleep_enable_timer_wakeup(duree_s * 1000000ULL);
      isGoingToSleep = true;
      Serial.println("[SYSTEM] Preparation au Deep Sleep...");
      WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);
      delay(100);
      Serial.println("[SLEEP] Bonne nuit !");
      esp_deep_sleep_start();
      // — on n'arrive jamais ici —
    }
    // Si estReveilNocturne == false (dernier tronçon écoulé) :
    // on tombe naturellement dans le setup complet ci-dessous
  }

  // ==========================================================================
  // SETUP COMPLET — Démarrage à froid ou réveil journée (09h)
  // chargerConfig() déjà appelé en début de setup().
  // ==========================================================================
  pinMode(MOSFET_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BOOT_BUTTON_PIN, INPUT_PULLUP);

  prefs.begin("solar", false);

  // --- Charge NVSH24 (h24, i24, indexes, historique30j) ---
  { NVSH24 _h={};
    size_t _hs = prefs.getBytes("h24", &_h, sizeof(_h));
    if (_hs == sizeof(_h)) {
      for(int i=0;i<24;i++){h24_Volt[i]=_h.volt[i];h24_Lux[i]=_h.lux[i];i24_Amp[i]=_h.amp[i];}
      indexH24=_h.iH; indexI24=_h.iI24; derHeureEch=_h.dHEch;
      for(int i=0;i<30;i++){historique30j[i]={_h.jVMatin[i],_h.jLMatin[i],_h.jVMidi[i],_h.jLMidi[i],_h.jVSoir[i],_h.jLSoir[i]};}
      index30j=_h.idx30j;
      // Sanitize h24
      for(int i=0;i<24;i++){if(h24_Volt[i]<9.0f||h24_Volt[i]>20.0f)h24_Volt[i]=-1.0f;if(h24_Lux[i]<0.0f)h24_Lux[i]=-1.0f;}
    } else {
      // Premier boot — sentinelles
      for(int i=0;i<24;i++){h24_Volt[i]=-1.0f;h24_Lux[i]=-1.0f;i24_Amp[i]=-1.0f;}
      indexH24=0; indexI24=0; derHeureEch=-1; index30j=0;
      addLog("[NVS] h24 absent — premier boot");
    }
  }
  // --- Charge NVSNuit (historiqueTension, indexes nuit) ---
  { NVSNuit _n={};
    size_t _ns = prefs.getBytes("nuit", &_n, sizeof(_n));
    if (_ns == sizeof(_n)) {
      memcpy(historiqueTension, _n.tension, sizeof(historiqueTension));
      indexReleve=_n.idxReleve; nbReels=_n.nbReels; derHeureEnr=_n.derHeureEnr;
    }
  }
  // --- Charge NVSBilan (bilan30j) — commun aux deux branches ---
  { NVSBilan _b={};
    prefs.getBytes("bil", &_b, sizeof(_b));
    memcpy(bilan30j, _b.b30, sizeof(bilan30j));
  }

  bool isWakeup = (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER);
  if (isWakeup) {
    // Réveil matin : indexes depuis RTC (plus récents que NVS)
    indexH24      = rtc_indexH24;
    indexReleve   = rtc_indexReleve;
    index30j      = rtc_index30j;
    indexBilan30j = rtc_indexBilan30j;
    owmDernierJour   = rtc_owmDernierJour;
    VOLT_COUPURE_DYN = rtc_VOLT_COUPURE_DYN;
    CURRENT_LUX_ON   = rtc_CURRENT_LUX_ON;
    batterieEstOK    = rtc_batterieEstOK;
    for (int i=0;i<3;i++) { accV[i]=rtc_accV[i]; accL[i]=rtc_accL[i]; accC[i]=rtc_accC[i]; accI[i]=rtc_accI[i]; }
    derHeureEch = rtc_derHeureEch;
    derHeureEnr = rtc_derHeureEnr;
    nbReels     = rtc_nbReels;
    socPct      = rtc_socPct;
    indexI24    = rtc_indexI24;
    bilCnt30j   = rtc_bilCnt30j;
    memcpy(i24_Amp, rtc_i24_Amp, sizeof(i24_Amp));
    // ahEntrants/ahSortants : depuis NVSState (survit au power-on reset, RTC non fiable)
    { NVSState _s={}; prefs.getBytes("st",&_s,sizeof(_s));
      ahEntrants=_s.ahIn; ahSortants=_s.ahOut; }
    rtc_troncon = 0;
    ignorerSommeilJusquAuReveil = true;
    bilanFait = false;
    derHeureCondenser = -1;
    // Persiste bilanFait=false dans NVSBilan
    { NVSBilan _b={}; prefs.getBytes("bil",&_b,sizeof(_b));
      _b.bilanFait=0; prefs.putBytes("bil",&_b,sizeof(_b)); }
    addLog("[BILAN] bilanFait reset -> false (nouveau jour)");
    Serial.println("[NVS] Réveil deep sleep — index restaurés depuis RTC memory.");
  } else {
    // Cold boot : tout depuis NVSState
    { NVSState _s={};
      size_t _ss = prefs.getBytes("st", &_s, sizeof(_s));
      if (_ss == sizeof(_s)) {
        socPct=_s.socPct; ahEntrants=_s.ahIn; ahSortants=_s.ahOut;
        for(int i=0;i<3;i++){accV[i]=_s.accV[i];accL[i]=_s.accL[i];accI[i]=_s.accI[i];accC[i]=_s.accC[i];}
      } else {
        NVSState _z={}; _z.socPct=50.0f; prefs.putBytes("st",&_z,sizeof(_z)); // crée si absent
        socPct=50.0f;
      }
      addLog("[NVS] st restore: ahIn=" + String(ahEntrants,4) + " ahOut=" + String(ahSortants,4));
    }
    // NVSBilan : indexes + bilanFait
    { NVSBilan _b={};
      size_t _bs = prefs.getBytes("bil", &_b, sizeof(_b));
      if (_bs == sizeof(_b)) {
        indexBilan30j=_b.idxBil; bilCnt30j=_b.bilCnt; bilanFait=(_b.bilanFait!=0);
        memcpy(bilan30j, _b.b30, sizeof(bilan30j)); // recharge avec indexes corrects
      } else {
        NVSBilan _z={}; prefs.putBytes("bil",&_z,sizeof(_z)); // crée si absent
        indexBilan30j=0; bilCnt30j=0; bilanFait=false;
      }
    }
    rtc_bilCnt30j  = bilCnt30j;
    derHeureCondenser = -1;
    Serial.printf("[NVS] Démarrage à froid — SOC=%.1f%% ahIn=%.4f ahOut=%.4f\n", socPct, ahEntrants, ahSortants);
    addLog("[BILAN] cold boot: idx=" + String(indexBilan30j)
           + " cnt=" + String(bilCnt30j) + " bilanFait=" + String(bilanFait ? "true" : "false")
           + " ahIn=" + String(ahEntrants,2) + " ahOut=" + String(ahSortants,2));
    // Synchronise RTC memory depuis NVS
    rtc_indexH24      = indexH24;
    rtc_indexReleve   = indexReleve;
    rtc_index30j      = index30j;
    rtc_indexBilan30j = indexBilan30j;
    rtc_nbReels       = nbReels;
    rtc_derHeureEch   = derHeureEch;
    rtc_derHeureEnr   = derHeureEnr;
    rtc_indexI24      = indexI24;
    rtc_socPct        = socPct;
    memcpy(rtc_i24_Amp, i24_Amp, sizeof(i24_Amp));
    for (int i=0;i<3;i++) { rtc_accV[i]=accV[i]; rtc_accL[i]=accL[i]; rtc_accC[i]=accC[i]; rtc_accI[i]=accI[i]; }
  }
  prefs.end(); // ferme "solar"

  // Restaure les prévisions (graphe Futur) depuis NVS
  chargerPrevisions();

  // Filtre JSON pour l'API NASA — ne récupère que le paramètre solaire utile
  nasaFilter["properties"]["parameter"]["ALLSKY_SFC_SW_DWN"] = true;

  xMutexMeteo = xSemaphoreCreateMutex();
  xMutexLog   = xSemaphoreCreateMutex();
  xMutexH24 = xSemaphoreCreateMutex();
  Serial.println(xMutexMeteo ? "[MUTEX] xMutexMeteo OK" : "[MUTEX] ERREUR xMutexMeteo !");
  Serial.println(xMutexLog   ? "[MUTEX] xMutexLog OK"   : "[MUTEX] ERREUR xMutexLog !");

  setupWiFiEvents();
  lastWifiAttempt = millis(); // FIX : évite que checkWiFiReconnect se déclenche dans les 5 premières minutes
   // --- CODE DE DEBUG BILAN 30J ---
  Serial.println("--- DEBUG NVS BILAN ---");
  Serial.print("Index actuel : "); Serial.println(indexBilan30j);
  // On regarde la case "hier" (index précédent)
  int idxHier = (indexBilan30j + 29) % 30;
  Serial.print("Case Hier (index "); Serial.print(idxHier); Serial.print(") : ");
  Serial.print(bilan30j[idxHier].ahIn); Serial.print(" Ah In / ");
  Serial.print(bilan30j[idxHier].ahOut); Serial.println(" Ah Out");
  Serial.println("-----------------------");

  // --- PROMPT SERIAL : 10s pour forcer le mode AP (appuyer sur 'd') ou bypasser ('espace') ---
    bool forceAP = false;
    Serial.println("\n[WIFI] Appuyez sur 'd'+'ENTER' dans les 10s pour forcer le mode AP de configuration, 'ENTER' pour bypasser...");
    unsigned long t0 = millis();
    while (millis() - t0 < 10000) {
      if (Serial.available()) {
        char c = Serial.read();
        if (c == 'd') {
          forceAP = true;
          break;
        } else if (c == '\r' || c == '\n') {
          Serial.println("\n[WIFI] Bypass — démarrage immédiat.");
          break;
        }
      }
      // Affiche un compte à rebours chaque seconde
      static int dernSeconde = -1;
      int secondesRestantes = 10 - (int)((millis() - t0) / 1000);
      if (secondesRestantes != dernSeconde) {
        dernSeconde = secondesRestantes;
        Serial.print(".");
      }
      esp_task_wdt_reset(); // nourrit le watchdog pendant l'attente
      delay(50);
    }
    if (forceAP) {
      Serial.println("[WIFI] Mode AP forcé par l'utilisateur.");
      addLog("[WIFI] mode AP forcé (Serial 'd')");
      WiFi.persistent(false);
      WiFi.mode(WIFI_AP);
      WiFi.softAP("ESP32-SOLAIRE-CONFIG");
      delay(1000);
      dnsServer.start(53, "*", WiFi.softAPIP());
      server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        String h = "<html><body><h2>" L_AP_TITRE "</h2><form action='/save' method='POST'>";
        h += "SSID:<br><input name='s'><br>Password:<br><input type='password' name='p'><br><br>";
        h += "OWM API key:<br><input name='owm' value='" + OWM_KEY + "' style='width:260px'><br><br>";
        h += "<input type='submit' value='" L_AP_BTN_SAVE "'></form></body></html>";
        request->send(200, "text/html", h);
      });
      configurerRoutesServeur();
      server.begin();
      DefaultHeaders::Instance().addHeader("Connection", "close");
      return; // sort de setup() — loop() gérera le mode AP
    }
  // --- FIN PROMPT ---

  setupWiFi();

  configTzTime("CET-1CEST,M3.5.0,M10.5.0/3", "pool.ntp.org"); //pool.ntp.org ou 216.239.35.0

  // Affichage de l'heure si synchronisée
  // --- Validation finale et affichage de l'heure synchronisée ---
  Serial.println("[SYSTEM] Synchronisation heure NTP ");

  if (!veml.begin()) {
    Serial.println("[VEML] Capteur non trouvé — lecture lux désactivée (valeur simulée 500 lx)");
    addLog("[VEML] Capteur non trouve");
    vemlOK = false;
  } else {
    vemlOK = true;
    veml.setGain(VEML7700_GAIN_1_8);
    veml.setIntegrationTime(VEML7700_IT_100MS);
    Serial.println("[VEML] Capteur initialisé OK");
    addLog("[VEML] Capteur OK");
    delay(200);
    luxDernier = veml.readLux();
    Serial.printf("[VEML] Premiere lecture : %.0f lx\n", luxDernier);
  }

  // Initialisation INA226 sur Wire1 — scan adresse puis init
  // Le scan évite tout problème de cast d'adresse (bug byte/int)
  
    bool inaPresent = false;
    for (byte a = 1; a < 127; a++) {
      Wire1.beginTransmission(a);
      if (Wire1.endTransmission() == 0) {
        ina = INA226_WE(&Wire1, a);
        inaPresent = true;
        Serial.printf("[INA226] trouve 0x%02X\n", a);
        if (ina.init()) {
          ina.setResistorRange(0.01, 20.0);
          ina.setConversionTime(INA226_CONV_TIME_1100);
          ina.setMeasureMode(INA226_CONTINUOUS);
          inaOK = true;
          delay(100);
          float iFirst = ina.getCurrent_A();
          float vFirst = ina.getBusVoltage_V();
          Serial.printf("[INA226] rawA: %.3f A, rawV: %.3f V\n", ina.getCurrent_A());
          Serial.printf("[INA226] rawV: %.3f V\n",ina.getBusVoltage_V());
          courantDernier = iFirst;
          
          Serial.printf("[INA226] OK — %.3f A, %.3f V\n", iFirst, vFirst);
          addLog("[INA226] OK " + String(iFirst, 3) + "A, " + String(vFirst, 3) + "V");
        } else {
          addLog("[INA226] trouve, init echouee");
        }
        break;
      }
    }
    if (!inaPresent) addLog("[INA226] non trouve sur Wire1");
  
  // --- ÉVALUATION MOSFET IMMÉDIATE AU DÉMARRAGE ---
  // Sans ça, le MOSFET reste OFF jusqu'au premier lastCheck (10 min)
  // même si les conditions sont déjà bonnes.
  
    uint32_t mv = analogReadMilliVolts(BAT_PIN);
    float v = (mv > 0) ? (mv / 1000.0) * RATIO_PONT_DIVISEUR : 0.0;
    float lux = vemlOK ? veml.readLux() : -1.0;
    if (v > 9.5) { // mesure réelle
      // Si boot loop détecté → MOSFET forcé LOW
      bool bloqueBootLoop = (rtc_mosfetBloqueJusquA > 0 &&
                             esp_timer_get_time() < rtc_mosfetBloqueJusquA);
      if (bloqueBootLoop) {
        digitalWrite(MOSFET_PIN, LOW);
        int64_t resteS = (rtc_mosfetBloqueJusquA - esp_timer_get_time()) / 1000000LL;
        char b[64]; snprintf(b, sizeof(b), "[CHARGE] OFF boot loop — reste %llds", (long long)resteS);
        addLog(b); Serial.println(b);
        rtc_mosfetWasHigh = false;
      } else if (lux > CURRENT_LUX_ON && v > VOLT_REPRISE && batterieEstOK && v > VOLT_COUPURE_DYN) {
        digitalWrite(MOSFET_PIN, HIGH);
        rtc_mosfetWasHigh = true;
        // Mesure courant immédiate après mise à HIGH — utilisée par la détection boot loop
        delay(300); // laisse le courant se stabiliser
        float courantImmediat = inaOK ? ina.getCurrent_A() : 0.0;
        courantDernier = courantImmediat;
        { NVSState _s={}; prefs.begin("solar",true); prefs.getBytes("st",&_s,sizeof(_s)); prefs.end();
          _s.arMosfetHigh=1; _s.arCourant=courantImmediat;
          prefs.begin("solar",false); prefs.putBytes("st",&_s,sizeof(_s)); prefs.end(); }
        char b[64]; snprintf(b, sizeof(b), "[CHARGE] ON demarrage %.2fV %.0flx %.3fA", v, lux, courantImmediat);
        addLog(b); Serial.println(b);
      } else {
        digitalWrite(MOSFET_PIN, LOW);
        rtc_mosfetWasHigh = false;
        char b[48]; snprintf(b, sizeof(b), "[CHARGE] OFF demarrage %.2fV %.0flx", v, lux);
        Serial.println(b);
      }
    }

  // Lecture INA226 finale — initialise courantDernier après la décision MOSFET
  // (cas anti-bootloop : courantDernier restait à 0 car pas de lecture après le LOW forcé)
  if (inaOK) {
    delay(200);
    courantDernier  = ina.getCurrent_A();
    inaVoltDerniere = ina.getBusVoltage_V();
    rtc_dernierCourant = courantDernier;
    Serial.printf("[INA226] courant final setup : %.3f A, %.3f V\n", courantDernier, inaVoltDerniere);
  }

} //fin setup()

// Tâche FreeRTOS pour exécuter meteoForecast() sur Core 0
// sans bloquer loop() ni le serveur async (qui tournent sur Core 1)
void tacheMeteo(void* param) {
  esp_task_wdt_add(NULL);   // abonne cette tâche FreeRTOS au watchdog
  meteoForecast();
  esp_task_wdt_delete(NULL); // désabonne avant de se supprimer
  vTaskDelete(NULL);
}

// ==========================================================================
// DEEP SLEEP — Endormissement nuit
// ==========================================================================
// Sauvegarde l'état dans RTC memory avant chaque deep sleep.
// Le MOSFET est mis à LOW avant le sleep (sécurité).
// Durée calculée sur la base de l'heure NTP actuelle.
// Sauvegarde l'état en RTC memory avant chaque deep sleep
void sauvegarderEtatRTC() {
  rtc_indexH24         = indexH24;
  rtc_indexReleve      = indexReleve;
  rtc_index30j         = index30j;
  rtc_indexBilan30j    = indexBilan30j;
  rtc_owmDernierJour   = owmDernierJour;
  rtc_VOLT_COUPURE_DYN = VOLT_COUPURE_DYN;
  rtc_CURRENT_LUX_ON   = CURRENT_LUX_ON;
  rtc_batterieEstOK    = batterieEstOK;
  rtc_derHeureEch      = derHeureEch;
  rtc_derHeureEnr      = derHeureEnr;
  rtc_nbReels          = nbReels;
  // INA226 — ahEntrants/ahSortants gérés via NVS uniquement
  rtc_socPct      = socPct;
  rtc_indexI24    = indexI24;
  memcpy(rtc_i24_Amp, i24_Amp, sizeof(i24_Amp));
  rtc_bilCnt30j   = bilCnt30j;
  for (int i = 0; i < 3; i++) {
    rtc_accV[i] = accV[i]; rtc_accL[i] = accL[i]; rtc_accC[i] = accC[i]; rtc_accI[i] = accI[i];
  }
}

// Calcule la durée d'un tronçon de sleep.
// La plage totale (HEURE_SOMMEIL_SOIR:MIN_SOMMEIL_SOIR → HEURE_REVEIL_JOUR:MIN_REVEIL_JOUR)
// est divisée en (NB_RELEVES_NUIT + 1) tronçons de durée égale.
// Aucune dépendance à l'heure courante — immunisé contre toute dérive RTC/TimeLib.
//
// Exemple : 19h45 → 09h15 avec NB_RELEVES_NUIT=2
//   plage = (24-19)*3600 - 45*60 + 9*3600 + 15*60 = 48 600 s
//   3 tronçons de 16 200 s = 4h30 chacun
//   → réveils à ~00h15 et ~04h45, puis lever à 09h15
uint64_t dureeTroncon(int ) {
  // Plage totale en secondes (traverse toujours minuit pour les horaires nocturnes)
  int minSoir  = HEURE_SOMMEIL_SOIR * 60 + MIN_SOMMEIL_SOIR;
  int minMatin = HEURE_REVEIL_JOUR  * 60 + MIN_REVEIL_JOUR;
  int plage_min;
  if (minSoir > minMatin) {
    plage_min = 1440 - minSoir + minMatin; // traverse minuit
  } else {
    plage_min = minMatin - minSoir;        // même nuit (rare)
  }
  int nbTroncons = NB_RELEVES_NUIT + 1;
  uint64_t s = (uint64_t)(plage_min * 60) / nbTroncons;
  if (s < 300) s = 300; // garde-fou : 5 min minimum par tronçon
  return s;
}

// Détecte si l'heure h:m est dans la plage de sommeil configurée
// (de HEURE_SOMMEIL_SOIR:MIN_SOMMEIL_SOIR à HEURE_REVEIL_JOUR:MIN_REVEIL_JOUR)
bool estDansPlageSommeil(int h, int m) {
  int minActuelle = h * 60 + m;
  int minSoir     = HEURE_SOMMEIL_SOIR * 60 + MIN_SOMMEIL_SOIR;
  int minMatin    = HEURE_REVEIL_JOUR  * 60 + MIN_REVEIL_JOUR;
  if (minSoir > minMatin) {
    // Cas normal : traverse minuit (ex. 19h45 → 09h15)
    return (minActuelle >= minSoir || minActuelle < minMatin);
  } else {
    // Cas rare : plage dans la même journée
    return (minActuelle >= minSoir && minActuelle < minMatin);
  }
}

// Deep sleep en 3 tronçons pour préserver les relevés nuit à 01h et 04h.
// La durée de chaque tronçon est calculée une fois pour toutes depuis la config
// (dureeTroncon) — aucune lecture de l'heure courante, immunisé contre les
// dérives TimeLib / RTC / DST.
//
//   Tronçons 1..NB_RELEVES_NUIT : réveils intermédiaires (relevé tension)
//   Tronçon 2 : 01h   → 04h00               (réveil court : relevé tension)
//   Tronçon 3 : 04h   → HEURE_REVEIL_JOUR   (réveil court : relevé tension)
//   Réveil jour → setup complet (WiFi, NTP, serveur web) + rtc_troncon remis à 0
void entrerDeepSleep() {
  digitalWrite(MOSFET_PIN, LOW);

  // Tant que le WiFi est UP, les callbacks AsyncWebServer (Core 0) peuvent appeler
  // prefs.begin() concurremment → écriture NVS ratée silencieusement → bilan30j reste
  // à zéro indéfiniment et bilanFait reste à false. Couper le WiFi ici force Core 0
  // à terminer ses callbacks en cours avant que le condenser ne touche le flash.
  isGoingToSleep = true;
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  delay(200); // laisse Core 0 terminer ses callbacks en cours

  // Archivage journalier — une seule fois par jour, protégé par flag global RAM
  addLog(String("[BILAN] bilanFait=") + (bilanFait ? "true" : "false") + " ahIn=" + String(ahEntrants,2) + " ahOut=" + String(ahSortants,2));
  if (!bilanFait) {
    condenserEtSauvegarderJournee();
    bilanFait = true;
    addLog("[SLEEP] bilan journalier archivé");
  } else {
    addLog("[SLEEP] bilan deja archivé aujourd'hui");
  }

  sauvegarderEtatRTC();

  // Avance le compteur de tronçon — stocké en RTC memory, survit au deep sleep
  int prochainTroncon = rtc_troncon + 1;
  if (prochainTroncon < 1 || prochainTroncon > NB_RELEVES_NUIT + 1) prochainTroncon = 1; // sécurité
  rtc_troncon = prochainTroncon;

  // FIX HORAIRES : le tronçon 1 est calculé depuis l'heure RÉELLE actuelle et non
  // depuis l'heure théorique de sommeil. Ainsi, même si l'ESP s'endort avec du retard
  // (reconnexions WiFi, etc.), les réveils nocturnes restent calés sur HEURE_REVEIL_JOUR.
  uint64_t duree_s;
  if (prochainTroncon == 1) {
    struct tm ti;
    if (getLocalTime(&ti)) {
      int minActuelle = ti.tm_hour * 60 + ti.tm_min;
      int minMatin    = HEURE_REVEIL_JOUR * 60 + MIN_REVEIL_JOUR;
      int plage_min;
      if (minActuelle > minMatin)
        plage_min = 1440 - minActuelle + minMatin; // traverse minuit
      else
        plage_min = minMatin - minActuelle;
      int nbTroncons = NB_RELEVES_NUIT + 1;
      duree_s = (uint64_t)(plage_min * 60) / nbTroncons;
      if (duree_s < 300) duree_s = 300; // 5 min minimum
      Serial.printf("[SLEEP] Troncon 1 dynamique depuis %02dh%02d → %llus (%lluh%02llum)\n",
                    ti.tm_hour, ti.tm_min, duree_s, duree_s/3600, (duree_s%3600)/60);
    } else {
      duree_s = dureeTroncon(1); // fallback si RTC indisponible
      Serial.println("[SLEEP] Troncon 1 : RTC indispo, duree fixe utilisee");
    }
  } else {
    duree_s = dureeTroncon(prochainTroncon); // tronçons 2+ : durée fixe (pas de dérive possible)
  }
  Serial.printf("[SLEEP] Troncon %d → dans %llus (%lluh%02llum)\n",
                prochainTroncon, duree_s, duree_s/3600, (duree_s%3600)/60);
  addLog("[SLEEP] troncon " + String(prochainTroncon)
         + " -> " + String(duree_s/3600) + "h"
         + String((duree_s%3600)/60) + "m");
  Serial.flush();
  delay(100);

  sauvegarderLogs(); // persiste logs avant deep sleep principal

  // Snapshot NVSState complet avant sleep
  { NVSState _s={}; prefs.begin("solar",true); prefs.getBytes("st",&_s,sizeof(_s)); prefs.end();
    _s.ahIn=ahEntrants; _s.ahOut=ahSortants; _s.socPct=socPct;
    for(int i=0;i<3;i++){_s.accV[i]=accV[i];_s.accL[i]=accL[i];_s.accI[i]=accI[i];_s.accC[i]=accC[i];}
    _s.arMosfetHigh=rtc_mosfetWasHigh?1:0; _s.arCourant=rtc_dernierCourant; _s.arCount=0;
    _s.blocageS=(rtc_mosfetBloqueJusquA>0)?
      (int)((rtc_mosfetBloqueJusquA-esp_timer_get_time())/1000000LL):0;
    prefs.begin("solar",false); prefs.putBytes("st",&_s,sizeof(_s)); prefs.end(); }

  esp_sleep_enable_timer_wakeup(duree_s * 1000000ULL);
  esp_deep_sleep_start();
}

void loop() {
  if (WiFi.getMode() == WIFI_AP) dnsServer.processNextRequest();

  // Flag posé par /debug/bilan/force — exécuté sur Core 1 pour éviter conflit prefs
  if (forceCondenser) {
    forceCondenser = false;
    condenserEtSauvegarderJournee();
    bilanFait = true;
    addLog("[BILAN] condenser force execute sur Core1");
  }

  // Flag posé par /debug/bilan/undo — exécuté sur Core 1
  if (forceUndo) {
    forceUndo = false;
    if (bilCnt30j > 0) {
      indexBilan30j = (indexBilan30j + 29) % 30;
      bilan30j[indexBilan30j] = { 0.0, 0.0 };
      bilCnt30j--;
      bilanFait = false;
      { NVSBilan _b; memcpy(_b.b30,bilan30j,sizeof(bilan30j));
        _b.idxBil=indexBilan30j; _b.bilCnt=bilCnt30j; _b.bilanFait=0;
        prefs.begin("solar",false); prefs.putBytes("bil",&_b,sizeof(_b)); prefs.end(); }
      rtc_indexBilan30j = indexBilan30j;
      rtc_bilCnt30j     = bilCnt30j;
      addLog("[BILAN] undo execute — index=" + String(indexBilan30j) + " cnt=" + String(bilCnt30j));
    } else {
      addLog("[BILAN] undo ignore — bilCnt30j deja 0");
    }
  }

  // Flag posé par /debug/bilan/reset — exécuté sur Core 1
  if (forceReset) {
    forceReset = false;
    memset(bilan30j, 0, sizeof(bilan30j));
    indexBilan30j = 0; bilCnt30j = 0; bilanFait = false;
    ahEntrants = 0.0; ahSortants = 0.0;
    rtc_indexBilan30j = 0; rtc_bilCnt30j = 0;
    { NVSBilan _b={}; prefs.begin("solar",false); prefs.putBytes("bil",&_b,sizeof(_b)); prefs.end(); }
    { NVSState _s={}; prefs.begin("solar",true); prefs.getBytes("st",&_s,sizeof(_s)); prefs.end();
      _s.ahIn=0.0f; _s.ahOut=0.0f;
      for(int i=0;i<3;i++){_s.accV[i]=0;_s.accL[i]=0;_s.accI[i]=0;_s.accC[i]=0;}
      prefs.begin("solar",false); prefs.putBytes("st",&_s,sizeof(_s)); prefs.end(); }
    addLog("[BILAN] reset complet execute sur Core1 — J0 repart de zero");
  }

  // --- DEEP SLEEP NUIT ---
  // Entrée en sleep dès que :
  //   - heure >= HEURE_SOMMEIL_SOIR:MIN_SOMMEIL_SOIR (19h45), OU
  //   - réveils nocturnes 01h05 / 04h05 (après relevé)
  //   - plage matinale 06h→09h15 (WiFi pas encore dispo)
  // En mode AP (configuration), on ne dort jamais.
  if (WiFi.getMode() != WIFI_AP && year() >= 2020) {
    int h = hour(); int m = minute();
    // FIX REVEIL ANTICIPE : désarme le flag dès qu'on a dépassé l'heure de réveil configurée.
    // Cela permet de tolérer un réveil quelques minutes trop tôt sans se rendormir aussitôt.
    if (ignorerSommeilJusquAuReveil) {
      int minActuelle = h * 60 + m;
      int minMatin    = HEURE_REVEIL_JOUR * 60 + MIN_REVEIL_JOUR;
      if (minActuelle >= minMatin) {
        ignorerSommeilJusquAuReveil = false;
        addLog("[SLEEP] heure reveil atteinte — surveillance sommeil active");
      }
    }
    // Entrée en sleep si on est dans la plage configurée (soir → matin)
    // ou si l'ESP démarre en plein milieu de la plage (cold boot nocturne)
    // Les réveils intermédiaires sont gérés dans le setup court — pas ici.
    if (!ignorerSommeilJusquAuReveil && estDansPlageSommeil(h, m)) {
       addLog("[SLEEP] entrée en mode DeepSleep");
       entrerDeepSleep();
    }
  }

  if (lastGotIP > 0 && millis() - lastGotIP > 10000) {
    lastGotIP = 0;  // reset en premier — toujours exécuté
    if (year() >= 2020 && owmDernierJour != day()) {
        demandeMeteo = true;
        addLog("[METEO] auto-demande owm & NASA au retour WiFi");
    }
  }
  
   if (timeStatus() == timeSet && !timeLog) {
      char timeBuf[40];
      // Affiche la date et l'heure récupérées
      snprintf(timeBuf, sizeof(timeBuf), "[LOOP] Boot time [%02d/%02d/%04d %02d:%02d:%02d]", 
               day(), month(), year(), hour(), minute(), second());      
      addLog(String(timeBuf));
      Serial.println(timeBuf);
      timeLog = true;
  }

  // Resynchronisation TimeLib sur l'heure système ESP32 — une fois par heure
  struct tm ti;
  if (minute() == 0 && hour() != derHeureSynchro && getLocalTime(&ti)) {
    setTime(ti.tm_hour, ti.tm_min, ti.tm_sec, ti.tm_mday, ti.tm_mon + 1, ti.tm_year + 1900);
    derHeureSynchro = hour();
  }
  if (minute() != 0) derHeureSynchro = -1;

  // W1 FIX v2 : meteoForecast() est lancé sur Core 0 via FreeRTOS.
  // loop() et ESPAsyncWebServer restent libres sur Core 1.
  bool _mec = false;
  if (xMutexMeteo) { xSemaphoreTake(xMutexMeteo, portMAX_DELAY); _mec = miseAJourEnCours; xSemaphoreGive(xMutexMeteo); }
  if (demandeMeteo && !_mec) {
    demandeMeteo = false;
    xTaskCreatePinnedToCore(
      tacheMeteo,   // fonction
      "meteoTask",  // nom debug
      16384,        // stack (16KB — suffisant pour HTTP + JSON)
      NULL,         // paramètre
      1,            // priorité
      NULL,         // handle (pas besoin)
      0             // Core 0
    );
  }

  // Reset Preferences (Bouton BOOT 5s) — efface WiFi ET configuration
  // Feedback LED progressif : clignote lentement pendant l'appui, 5× rapide avant reset
  if (digitalRead(BOOT_BUTTON_PIN) == LOW) {
      if (buttonPressStartTime == 0) {
          buttonPressStartTime = millis();
          Serial.println("[BOOT] Bouton maintenu detecte.");
          Serial.println("[BOOT] Maintenir 5s pour effacer toute la configuration NVS (WiFi + parametres + historique) et redemarrer.");
          Serial.println("[BOOT] Relacher pour annuler.");
      }
      unsigned long dureeAppui = millis() - buttonPressStartTime;
      // Clignotement lent pendant l'appui — non bloquant via millis()
      if ((dureeAppui / 500) % 2 == 1) digitalWrite(LED_PIN, HIGH);
      else                              digitalWrite(LED_PIN, LOW);
      // Compte a rebours Serial toutes les secondes
      static unsigned long derniereSeconde = 0;
      int secondesRestantes = 5 - (int)(dureeAppui / 1000);
      if (secondesRestantes > 0 && (millis() / 1000) != derniereSeconde) {
          derniereSeconde = millis() / 1000;
          Serial.printf("[BOOT] Reset dans %ds...\n", secondesRestantes);
      }
      if (dureeAppui > 5000) {
          // 5 flashs rapides = confirmation visuelle avant reset
          for (int i = 0; i < 5; i++) {
            digitalWrite(LED_PIN, HIGH); delay(80);
            digitalWrite(LED_PIN, LOW);  delay(80);
          }
          prefs.begin("config",   false); prefs.clear(); prefs.end();
          prefs.begin("params",   false); prefs.clear(); prefs.end();
          prefs.begin("solar",    false); prefs.clear(); prefs.end();
          prefs.begin("bilan",    false); prefs.clear(); prefs.end();
          prefs.begin("logs",     false); prefs.clear(); prefs.end();
          Serial.println("[RESET] NVS efface — reboot...");
          ESP.restart();
      }
  } else {
      if (buttonPressStartTime != 0) Serial.println("[BOOT] Relache — annule.");
      buttonPressStartTime = 0;
      digitalWrite(LED_PIN, LOW); // eteint la LED si on relache avant 5s
  }
  
  static unsigned long lastCheck = 0;
  // Mise à jour courantDernier toutes les 30s — pour l'icône batterie polling /live
  // (lire l'INA226 depuis le callback async n'est pas thread-safe → retourne 0)
  static unsigned long lastCheckCourant = 0;
  if (millis() - lastCheckCourant > 30000) {
    lastCheckCourant = millis();
    if (inaOK) {
      courantDernier  = ina.getCurrent_A();
      inaVoltDerniere = ina.getBusVoltage_V();
    }
  }

  if (millis() - lastCheck > (unsigned long)INTERVALLE_CHECK_MIN * 60000UL) {
    lastCheck = millis();
    float lux;
    if (vemlOK) {
      lux = veml.readLux();
      luxDernier = lux;
    } else {
      lux = -1.0;    // FIX: pas de valeur simulée dans les graphes — sentinelle ignorée
      luxDernier = -1.0; // badge affiche "---" au lieu de "500 lx"
    }
    uint32_t mv = analogReadMilliVolts(BAT_PIN);  // Vref calibrée via eFuse ESP32
    float v = (mv > 0) ? (mv / 1000.0) * RATIO_PONT_DIVISEUR : 0.0;
    // FIX: fallback 0.0 au lieu de 12.5 — sera détecté par horsAlimentation (< 9.5V) et ignoré
    tensionDerniere = (v > 9.0) ? v : tensionDerniere; // ne met à jour que si mesure réelle
    int h = hour();
    int m = minute();

    // --- DÉTECTION HORS ALIMENTATION MPPT (GND non commun) ---
    // Quand le GND batterie/MPPT est absent (ex: branché sur PC sans câble GND commun),
    // la tension lue est aberrante (souvent < 2V ou > 20V).
    // Seuil < 9.5V identique au Nano : on bloque tout traitement, MOSFET OFF.
    horsAlimentation = (v < 9.5 || v > 20.0);
    if (horsAlimentation) {
      digitalWrite(MOSFET_PIN, LOW);
      // Clignotement LED rapide (3×) comme signal visuel
      for (int i = 0; i < 3; i++) {
        digitalWrite(LED_PIN, HIGH); delay(50);
        digitalWrite(LED_PIN, LOW);  delay(50);
      }
      char buf[48]; snprintf(buf, sizeof(buf), "%.2fV", v);
      addLog("[HORS_MPPT] GND non commun — mesure ignoree (" + String(buf) + ")");
      // Réinitialise les flags et SORT du bloc sans faire de return :
      // le WDT doit être nourri en fin de loop().
      if (m != 0) derHeureEch = -1;
      derHeureEnr = -1;
    } else {

      // --- COURANT INA226 ---
      float courant = getCourant();
      courantDernier = courant;
      rtc_dernierCourant = courant;
      rtc_mosfetWasHigh  = (digitalRead(MOSFET_PIN) == HIGH);
      majSOC(courant, v);

      // --- SAUVEGARDE NVS "st" (NVSState) : toutes les 10 min ---
      { NVSState _s;
        _s.socPct=socPct; _s.ahIn=ahEntrants; _s.ahOut=ahSortants;
        for(int i=0;i<3;i++){_s.accV[i]=accV[i];_s.accL[i]=accL[i];_s.accI[i]=accI[i];_s.accC[i]=accC[i];}
        _s.arMosfetHigh=rtc_mosfetWasHigh?1:0; _s.arCourant=courant; _s.arCount=0;
        _s.blocageS=(rtc_mosfetBloqueJusquA>0 && esp_timer_get_time()<rtc_mosfetBloqueJusquA)
          ? (int)((rtc_mosfetBloqueJusquA-esp_timer_get_time())/1000000LL) : 0;
        bool _stOK = false;
        prefs.begin("solar", false); _stOK=prefs.putBytes("st",&_s,sizeof(_s)); prefs.end();
        addLog(String("[NVS] st=") + (_stOK?"OK":"FAIL") + " ahIn=" + String(ahEntrants,4) + "/" + String(ahSortants,4));
      }

      // --- ÉCHANTILLONNAGE HORAIRE (premier lastCheck de chaque nouvelle heure) ---
      if (h != derHeureEch) {
          if (xMutexH24) xSemaphoreTake(xMutexH24, portMAX_DELAY);
          h24_Volt[indexH24] = v;
          h24_Lux[indexH24]  = (lux >= 0) ? lux : -1.0;
          if (xMutexH24) xSemaphoreGive(xMutexH24);
          i24_Amp[indexI24]  = courant;
          indexH24 = (indexH24 + 1) % 24;
          indexI24 = (indexI24 + 1) % 24;
          derHeureEch = h;

          // NVSH24 + optionnellement NVSNuit si heure de relevé soir
          bool estHeureReleve2 = (h == HEURE_SOMMEIL_SOIR);
          if (estHeureReleve2 && h != derHeureEnr) {
            historiqueTension[indexReleve] = v;
            indexReleve = (indexReleve + 1) % 40;
            if (nbReels < 40) nbReels++;
            derHeureEnr = h;
            NVSNuit _n; memcpy(_n.tension, historiqueTension, sizeof(historiqueTension));
            _n.idxReleve=indexReleve; _n.nbReels=nbReels; _n.derHeureEnr=derHeureEnr;
            prefs.begin("solar", false); prefs.putBytes("nuit",&_n,sizeof(_n)); prefs.end();
            char b[32]; snprintf(b,sizeof(b),"%dh : %.2fV",h,v); addLog("[NUIT] releve " + String(b));
          }
          { NVSH24 _h;
            for(int i=0;i<24;i++){_h.volt[i]=h24_Volt[i];_h.lux[i]=h24_Lux[i];_h.amp[i]=i24_Amp[i];}
            _h.iH=indexH24; _h.iI24=indexI24; _h.dHEch=derHeureEch;
            for(int i=0;i<30;i++){_h.jVMatin[i]=historique30j[i].vMatin;_h.jLMatin[i]=historique30j[i].lMatin;
              _h.jVMidi[i]=historique30j[i].vMidi;_h.jLMidi[i]=historique30j[i].lMidi;
              _h.jVSoir[i]=historique30j[i].vSoir;_h.jLSoir[i]=historique30j[i].lSoir;}
            _h.idx30j=index30j;
            prefs.begin("solar", false); prefs.putBytes("h24",&_h,sizeof(_h)); prefs.end(); }

          if      (h >= 6  && h < 11) { accV[0]+=v; accL[0]+=lux; accC[0]++; accI[0]+=courant; }
          else if (h >= 11 && h < 16) { accV[1]+=v; accL[1]+=lux; accC[1]++; accI[1]+=courant; }
          else if (h >= 16 && h < 21) { accV[2]+=v; accL[2]+=lux; accC[2]++; accI[2]+=courant; }
      } else {
          // Pas de changement d'heure — vérifier quand même le relevé soir (cas rare)
          bool estHeureReleve2 = (h == HEURE_SOMMEIL_SOIR);
          if (estHeureReleve2 && h != derHeureEnr) {
            historiqueTension[indexReleve] = v;
            indexReleve = (indexReleve + 1) % 40;
            if (nbReels < 40) nbReels++;
            derHeureEnr = h;
            NVSNuit _n; memcpy(_n.tension, historiqueTension, sizeof(historiqueTension));
            _n.idxReleve=indexReleve; _n.nbReels=nbReels; _n.derHeureEnr=derHeureEnr;
            prefs.begin("solar", false); prefs.putBytes("nuit",&_n,sizeof(_n)); prefs.end();
            char b[32]; snprintf(b,sizeof(b),"%dh : %.2fV",h,v); addLog("[NUIT] releve " + String(b));
          }
      }

      // --- BILAN 30J : condenser avancé ---
      if (AVANCE_CONDENSER > 0 && !bilanFait) {
        int hCondenser = ((HEURE_SOMMEIL_SOIR - AVANCE_CONDENSER) + 24) % 24;
        if (h == hCondenser && h != derHeureCondenser) {
          condenserEtSauvegarderJournee();
          bilanFait = true;
          derHeureCondenser = h;
          addLog("[BILAN] condenser avance a " + String(h) + "h (avant sleep de " + String(AVANCE_CONDENSER) + "h)");
        }
      }

      // --- MOSFET ---
      float _coupDyn = VOLT_COUPURE_DYN; float _luxOn = CURRENT_LUX_ON; bool _batOK = batterieEstOK;
      if (xMutexMeteo) { xSemaphoreTake(xMutexMeteo, portMAX_DELAY); _coupDyn = VOLT_COUPURE_DYN; _luxOn = CURRENT_LUX_ON; _batOK = batterieEstOK; xSemaphoreGive(xMutexMeteo); }
      bool bloqueBootLoop = (rtc_mosfetBloqueJusquA > 0 && esp_timer_get_time() < rtc_mosfetBloqueJusquA);
      if (bloqueBootLoop) {
        digitalWrite(MOSFET_PIN, LOW);
        rtc_mosfetWasHigh = false;
        // blocageS déjà sauvegardé dans NVSState ci-dessus
        if (rtc_mosfetBloqueJusquA > 0 && !(esp_timer_get_time() < rtc_mosfetBloqueJusquA)) {
          rtc_mosfetBloqueJusquA = 0;
          { NVSState _s={}; prefs.begin("solar",true); prefs.getBytes("st",&_s,sizeof(_s)); prefs.end();
            _s.blocageS=0; prefs.begin("solar",false); prefs.putBytes("st",&_s,sizeof(_s)); prefs.end(); }
          addLog("[CHARGE] Blocage boot loop terminé — MOSFET autorisé");
        }
      } else {
        if (rtc_mosfetBloqueJusquA > 0) {
          rtc_mosfetBloqueJusquA = 0;
          { NVSState _s={}; prefs.begin("solar",true); prefs.getBytes("st",&_s,sizeof(_s)); prefs.end();
            _s.blocageS=0; prefs.begin("solar",false); prefs.putBytes("st",&_s,sizeof(_s)); prefs.end(); }
          addLog("[CHARGE] Blocage boot loop terminé — MOSFET autorisé");
        }
        if (lux > _luxOn && v > VOLT_REPRISE && _batOK) {
          if (digitalRead(MOSFET_PIN) == LOW) { char b[32]; snprintf(b,sizeof(b),"%.2fV %.0flx",v,lux); addLog("[CHARGE] ON  " + String(b)); }
          digitalWrite(MOSFET_PIN, HIGH);
          rtc_mosfetWasHigh = true;
        } else if (lux < 150.0 || v < _coupDyn || !_batOK) {
          if (digitalRead(MOSFET_PIN) == HIGH) { char b[32]; snprintf(b,sizeof(b),"%.2fV %.0flx",v,lux); addLog("[CHARGE] OFF " + String(b)); }
          digitalWrite(MOSFET_PIN, LOW);
          rtc_mosfetWasHigh = false;
        }
      }
      { char b[32]; snprintf(b,sizeof(b),"%.2fV %.0flx %.3fA",v,lux, courantDernier); addLog("[MESURE] " + String(b)); }

      if (h != HEURE_SOMMEIL_SOIR) derHeureEnr = -1;

    } // fin else (!horsAlimentation)
  } // fin lastCheck

  // --- RECONNEXION WIFI — vérifiée à chaque tour, tentative max 1 fois/60s ---
  // Hors lastCheck pour réagir dans la minute plutôt que dans l'heure,
  // mais intervalle de 60s pour ne pas saturer la radio ni relancer en boucle.
  
  // --- DIAGNOSTIC MÉMOIRE — toutes les 60 secondes ---
  static unsigned long lastMem = 0;
  if (millis() - lastMem > 60000) {
    lastMem = millis();
    uint32_t freeHeap = ESP.getFreeHeap();
    uint32_t maxAlloc = ESP.getMaxAllocHeap();
    Serial.printf("[MEM] Heap libre: %u bytes | Plus grand bloc: %u bytes\n", freeHeap, maxAlloc);

    if (logsDirty) sauvegarderLogs();

    char memBuf[48];
    snprintf(memBuf, sizeof(memBuf), "[MEM] heap=%u bloc=%u", freeHeap, maxAlloc);
    //addLog(memBuf);

    if (maxAlloc < HEAP_CRITIQUE) {
      addLog("[MEM] critical fragmentation → reboot");
      if (logsDirty) sauvegarderLogs(); // force save avant reboot
      delay(200);
      ESP.restart();
    }
    if (maxAlloc < 10000) {
      Serial.println("[MEM] ALERTE : heap fragmentée — risque de crash serveur web !");
    }
  }
  checkWiFiReconnect();
  
  // Pas de light sleep : incompatible avec ESPAsyncWebServer (callbacks/interruptions perdus).
  // L'économie d'énergie vient du deep sleep nuit (3 tronçons, ~7h/nuit).
  // En journée, la loop tourne librement — le lastCheck à 10min limite les écritures NVS
  // et les lectures capteur, sans bloquer le serveur web.

  esp_task_wdt_reset(); // nourrit le watchdog — prouve que loop() tourne
  delay(500);
}
