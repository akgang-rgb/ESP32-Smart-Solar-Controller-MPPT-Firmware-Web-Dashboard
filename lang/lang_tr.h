// =============================================================================
// Solar Controller — Language TR (Türkçe)
// To switch language: replace #include "lang_fr.h" with #include "lang_tr.h"
// =============================================================================
#pragma once

// --- SVG : chart titles ---
#define L_SVG_PRESENT        "ANLÍK \xe2\x80\x94 son 24 saat"
#define L_SVG_NOCTURNE       "GECE DURUMU \xe2\x80\x94 akü gerilimi"
#define L_SVG_FUTUR          "TAHMIN \xe2\x80\x94 sonraki 7 gün (lux)"
#define L_SVG_PASSE          "GEÇMİŞ \xe2\x80\x94 son 30 gün"
#define L_SVG_ENERGIE        "ENERJİ \xe2\x80\x94 Akım &amp; Güç 24s"
#define L_SVG_BILAN          "GÜNLÜK LOG 30g \xe2\x80\x94 Ah gir / çık"

// --- SVG : legends ---
#define L_SVG_TENSION        "Gerilim (V)"
#define L_SVG_LUMINOSITE     "Işık (lux)"
#define L_SVG_TENSION_NUIT   "Gece gerilimi (V)"
#define L_SVG_COURANT        "Akım (A)"
#define L_SVG_PUISSANCE      "Güç (W)"
#define L_SVG_ENTRANTS       "Giriş (Ah)"
#define L_SVG_SORTANTS       "Çıkış (Ah)"
#define L_SVG_COUPURE        "kesim %.2fV"
#define L_SVG_REPRISE        "yeniden başlatma"
#define L_SVG_SEUIL_ON       "AÇIK eşiği"
#define L_SVG_ATTENTE        "İlk ölçümler bekleniyor..."
#define L_SVG_MAJ            "(saatlik güncelleme)"
#define L_SVG_PAS_DONNEES    "Henüz veri yok"
#define L_SVG_BARRE_VISIBLE  "(bugünkü çubuk %d dk sonra görünür)"

// --- Main page header ---
#define L_HDR_CHARGE_ON      "YÜK : AÇIK"
#define L_HDR_CHARGE_OFF     "YÜK : KAPALI"
#define L_HDR_LUX            "Lux: "
#define L_HDR_SEUIL_ON       "AÇIK eşiği: "
#define L_HDR_COUPURE        "Kesim: "
#define L_HDR_HORS_MPPT      "&#9888; MPPT yok &mdash; GND ortak değil &mdash; ölçümler geçersiz"
#define L_HDR_TOAST          "☀️ Bugün için tahmin güncellendi"

// --- Main page sections ---
#define L_SEC_PRESENT        "ANLÍK"
#define L_SEC_NOCTURNE       "GECE DURUMU"
#define L_SEC_ENERGIE        "ENERJİ 24S"
#define L_SEC_BILAN          "GÜNLÜK LOG 30G"
#define L_SEC_FUTUR          "TAHMİN"
#define L_SEC_PASSE          "GEÇMİŞ"

// --- Hamburger menu ---
#define L_MENU_ACCUEIL       "&#8962; Ana Sayfa"
#define L_MENU_CONFIG        "&#9881; Ayarlar"
#define L_MENU_DEBUG         "&#128196; Hata Ayıklama"
#define L_MENU_DEBUG_BILAN   "&#128202; Log Hata Ayıklama"
#define L_MENU_DOC           "&#9432; Dokümantasyon"

// --- Common buttons ---
#define L_BTN_RETOUR         "&#8592; Geri"
#define L_BTN_ENREGISTRER    "💾 Kaydet"
#define L_BTN_ANNULER        "&#8635; Sonuncuyu geri al"
#define L_BTN_RESET          "&#128465; Gün 0 sıfırla"
#define L_BTN_DEBLOQUER      "&#128275; MOSFET kilidi aç"

// --- Settings page: titles and tabs ---
#define L_CFG_TITRE          "&#9881; Ayarlar"
#define L_TAB_GENERAL        "Genel"
#define L_TAB_LUMINOSITE     "Işık"
#define L_TAB_BATTERIE       "Akü"
#define L_CFG_SAUVEGARDEE    "✓ Ayarlar kaydedildi. Geri dönülüyor..."

// --- Settings > General ---
#define L_CFG_OWM_KEY        "OpenWeatherMap API anahtarı"
#define L_CFG_RESEAU         "&#127760; Ağ — sabit IP (yeniden başlatma gerekli)"
#define L_CFG_IP             "ESP32 IP"
#define L_CFG_GW             "Ağ geçidi"
#define L_CFG_MASQUE         "Alt ağ maskesi"
#define L_CFG_DNS            "DNS"
#define L_CFG_LAT            "Enlem"
#define L_CFG_LON            "Boylam"
#define L_CFG_TILT           "Panel eğimi"
#define L_CFG_TILT_UNIT      "° (0=yatay, 90=dikey)"
#define L_CFG_AZIMUTH        "Panel azimut"
#define L_CFG_AZIMUTH_UNIT   "° (180=Güney, 270=Batı)"
#define L_CFG_RATIO          "Çıkış gerilimi (yük)"
#define L_CFG_RATIO_HINT     "150k&#937; + 33k&#937; → (150+33)/33 = 5.5454"
#define L_CFG_SLEEP          "Uyku saati"
#define L_CFG_SLEEP_UNIT     "dak (varsayılan 19:45)"
#define L_CFG_REVEIL         "Uyanma saati"
#define L_CFG_REVEIL_UNIT    "dak (varsayılan 09:15)"
#define L_CFG_RELEVES        "Gece ölçümleri"
#define L_CFG_RELEVES_HINT   "Gece penceresi eşit dilimlere bölünür"
#define L_CFG_ARCHIVAGE      "Uyku öncesi log arşivleme"
#define L_CFG_ARCHIVAGE_UNIT "saat önce (0=uyku anında, 1-3s önce)"
#define L_CFG_ARCHIVAGE_HINT "Uykudan N saat önce arşivler. Öneri: 1s"

// --- Settings > Light ---
#define L_CFG_LUX_MAX        "Maksimum lux eşiği (düşük akü + kötü hava)"
#define L_CFG_LUX_MAX_HINT   "Olumsuz koşullardaki AÇIK eşiği"
#define L_CFG_LUX_AMP        "Dinamik aralık"
#define L_CFG_LUX_AMP_HINT   "Min AÇIK eşiği = Varsayılan - Aralık (ideal koşullar)"
#define L_CFG_LUX_CONF       "Hava tahmininde %100 güven lux'u"
#define L_CFG_LUX_CONF_HINT  "Bu değerin üzerinde hava tahminine %100 güvenilir"
#define L_CFG_LUX_ACTUEL     "Geçerli AÇIK eşiği: "
#define L_CFG_LUX_PLAGE      "Aralık: "

// --- Settings > Battery ---
#define L_CFG_V_PLEINE       "Tam şarj gerilimi"
#define L_CFG_V_REPRISE      "Yeniden başlatma gerilimi"
#define L_CFG_V_REPRISE_HINT "Şarjın yeniden başlayabileceği eşik"
#define L_CFG_V_COUPURE      "Kesim gerilimi (güzel hava)"
#define L_CFG_V_COUPURE_HINT "Kötü hava tahmini durumunda statik eşik"
#define L_CFG_V_CRITIQUE     "Kritik gerilim"
#define L_CFG_V_PLANCHER     "Mutlak alt sınır (asla aşağı inmez)"
#define L_CFG_V_PLANCHER_HINT "Güzel hava kesim tabanı — min 11.9V önerilir"
#define L_CFG_CAPACITE       "Akü kapasitesi (INA226 coulomb sayımı)"
#define L_CFG_CAPACITE_HINT  "Nominal kapasite — ör. 12V/11.5Ah için 11.5Ah"
#define L_CFG_ANTIREBOOT     "Aşırı yük yeniden başlatma gecikmesi"
#define L_CFG_ANTIREBOOT_HINT "Bootloop penceresi (varsayılan 60s)"
#define L_CFG_CADENCE        "Ölçüm / kaydetme aralığı"
#define L_CFG_CADENCE_HINT   "Sensör sorgulama, Ah sayımı ve NVS kaydetme. Varsayılan: 10 dak"
#define L_CFG_BLOCAGE        "Aşırı yükte MOSFET kilit süresi"
#define L_CFG_COUPURE_DYN    "Geçerli dinamik kesim: "

// --- Alert badges ---
#define L_BADGE_BLOCAGE      "&#9888; Bootloop koruması"
#define L_BADGE_HORS_MPPT    "&#9888; MPPT yok"

// --- Debug Log page ---
#define L_DBG_BILAN_TITRE    "&#128202; Hata Ayıklama Günlük Log 30g"
#define L_DBG_BILAN_AH_IN    "<b>ahIn:</b>"
#define L_DBG_BILAN_AH_OUT   "<b>ahOut:</b>"

// --- Debug logs page ---
#define L_DBG_TITRE          "&#11035; Hata Ayıklama \xe2\x80\x94 sistem logları"
#define L_DBG_REFRESH        "(60s yenile)"

// --- Documentation page ---
#define L_DOC_TITRE          "&#9432; Dokümantasyon \xe2\x80\x94 Güneş Kontrolcüsü"
#define L_DOC_MATERIEL       "&#128268; Donanım"
#define L_DOC_CAPTEURS       "Sensörler"
#define L_DOC_SORTIES        "Çıkışlar"
#define L_DOC_BADGES         "&#127968; Başlık rozetleri"
#define L_DOC_GRAPHIQUES     "&#128202; Grafikler"
#define L_DOC_LOGIQUE        "&#9889; Şarj mantığı"
#define L_DOC_PROTECTION     "&#128737; Bootloop aşırı yük koruması"
#define L_DOC_SOC            "&#128267; SOC \xe2\x80\x94 Şarj durumu"

// --- Form validation (JS) ---
#define L_VAL_HEURE_SLEEP    "Geçersiz uyku saati (0-23)"
#define L_VAL_MIN_SLEEP      "Geçersiz uyku dakikası (0-59)"
#define L_VAL_HEURE_REVEIL   "Geçersiz uyanma saati (0-23)"
#define L_VAL_MIN_REVEIL     "Geçersiz uyanma dakikası (0-59)"
#define L_VAL_PLAGE_COURTE   "Uyku penceresi çok kısa (en az 2s)"

// --- Reset reasons (debug page) ---
#define L_RST_POWERON        "Güç açıldı"
#define L_RST_EXT            "Harici sıfırlama (EN)"
#define L_RST_SW             "Yazılım yeniden başlatma"
#define L_RST_PANIC          "PANIC / İstisna (çökme!)"
#define L_RST_INT_WDT        "Kesme watchdog"
#define L_RST_TASK_WDT       "Görev watchdog (döngü engellendi!)"
#define L_RST_WDT            "Diğer watchdog"
#define L_RST_DEEPSLEEP      "Derin uyku uyanışı (normal)"
#define L_RST_BROWNOUT       "Brownout (düşük gerilim!)"
#define L_RST_SDIO           "SDIO sıfırlama"
#define L_RST_INCONNU        "Bilinmiyor"

// --- AP config page (first boot) ---
#define L_AP_TITRE           "Güneş Yapılandırması"
#define L_AP_BTN_SAVE        "Kaydet"

// --- Main page: section titles ---
#define L_H3_PRESENT         "&#9201; Anlık"
#define L_H3_NOCTURNE        "&#127762; Gece Durumu"
#define L_H3_ENERGIE         "&#128262; Enerji 24s"
#define L_H3_BILAN           "&#9878; Günlük Log 30g"
#define L_H3_FUTUR           "&#128301; Tahmin"
#define L_H3_PASSE           "&#128197; Geçmiş"

// --- Anti-bootloop badge (dynamic) ---
#define L_BADGE_BL_ACTIF     "&#9888; Bootloop koruması aktif \xe2\x80\x94 MOSFET KAPALI %s"

// --- Off-MPPT inline ---
#define L_HORS_MPPT_INLINE   "&#9888; MPPT yok &mdash; GND ortak değil &mdash; ölçümler geçersiz"

// --- Footer credit ---
#define L_FOOTER_CREDIT      "v1.23f &mdash; Yapan:"

// --- Config: brightness range hint ---
#define L_CFG_LUX_PLAGE_VAL  "Aralık: "

// --- Debug page ---
#define L_DBG_HEAP           "Boş RAM: "
#define L_DBG_OCTETS         " bayt \xe2\x80\x94 "
#define L_DBG_UPTIME         "Çalışma süresi: "
#define L_DBG_WIFI           "WiFi: "
#define L_DBG_BL_CONFIRM     "Bootloop kilidi devre dışı bırakılsın mı?"
#define L_DBG_BL_BTN         "&#128737; Kilidi devre dışı bırak"
#define L_DBG_BL_OK          "&#10003; Kilit devre dışı bırakıldı. Geri dönülüyor..."

// --- Debug balance: table and buttons ---
#define L_DBG_BIL_FORCE      "&#9654; Arşivlemeyi zorla"
#define L_DBG_BIL_TH_CASE    "Slot"
#define L_DBG_BIL_TH_NOTE    "Not"
#define L_DBG_BIL_PROCHAIN   "sonraki"
#define L_DBG_BIL_VALIDE     "<span class='ok'>geçerli</span>"
#define L_DBG_BIL_VIDE       "<span style='color:#333'>boş</span>"
#define L_DBG_BIL_AHIN       "<b>Ah giriş:</b>"
#define L_DBG_BIL_AHOUT      "<b>Ah çıkış:</b>"

// --- Documentation: hardware ---
#define L_DOC_VEML           "Işık (lux) Wire0 GPIO 21/22 adres 0x10"
#define L_DOC_INA            "Akım &amp; akü gerilimi Wire1 GPIO 19/23 adres 0x40"
#define L_DOC_GPIO34         "Gerilim bölücü (ayarlanabilir oran)"
#define L_DOC_GPIO18         "MOSFET (şarj kontrolü)"
#define L_DOC_GPIO2          "Gösterge LED (isteğe bağlı)"

// --- Documentation: badges ---
#define L_DOC_BADGE_V        "Gerilim bölücü ile akü gerilimi (GPIO 34)"
#define L_DOC_BADGE_CHARGE   "MOSFET durumu"
#define L_DOC_BADGE_LUX      "Anlık ışık şiddeti (VEML7700)"
#define L_DOC_BADGE_SEUIL    "Şarj için minimum ışık eşiği (dinamik)"
#define L_DOC_BADGE_COUPURE  "Kesim öncesi minimum gerilim (dinamik)"
#define L_DOC_BADGE_SOC      "Akü simgesi \xe2\x80\x94 INA226 coulomb sayımı SOC%%. <span class='badge green'>yeşil &gt;70%%</span> <span class='badge orange'>turuncu 30-70%%</span> <span class='badge red'>kırmızı &lt;30%%</span>. &#9889;=şarj, &#9660;=deşarj."
#define L_DOC_BADGE_HORS     "Ortak GND algılanamadı \xe2\x80\x94 gerilim ölçümü geçersiz"
#define L_DOC_BADGE_BL       "Aşırı yük koruması aktif \xe2\x80\x94 MOSFET kilitli KAPALI"

// --- Documentation: charts ---
#define L_DOC_G_PRESENT      "<b>ANLÍK 24s</b> \xe2\x80\x94 Son 24 saatte gerilim (V) ve ışık (lux)"
#define L_DOC_G_NOCTURNE     "<b>GECE DURUMU</b> \xe2\x80\x94 Gece uyanışlarında akü gerilimi"
#define L_DOC_G_ENERGIE      "<b>ENERJİ 24s</b> \xe2\x80\x94 24 saatte INA226 akım (A) ve güç (W)"
#define L_DOC_G_BILAN        "<b>GÜNLÜK LOG 30g</b> \xe2\x80\x94 Günlük Ah giriş (güneş şarjı) ve çıkış (tüketim)"
#define L_DOC_G_FUTUR        "<b>TAHMİN 7g</b> \xe2\x80\x94 OpenWeatherMap 7 günlük ışık tahmini"
#define L_DOC_G_PASSE        "<b>GEÇMİŞ 30g</b> \xe2\x80\x94 30 günlük gerilim ve ışık geçmişi"

// --- Documentation: charge logic ---
#define L_DOC_L_ON           "MOSFET <b>AÇIK</b>: lux &gt; AÇIK eşiği <b>VE</b> gerilim &gt; Yeniden başlatma V <b>VE</b> akü OK"
#define L_DOC_L_OFF          "MOSFET <b>KAPALI</b>: lux &lt; 150lx <b>VEYA</b> gerilim &lt; Kesim <b>VEYA</b> akü KO"
#define L_DOC_L_DYN          "Lux eşiği ve kesim gerilimi <b>dinamiktir</b> \xe2\x80\x94 OWM hava tahminine göre ayarlanır"

// --- Documentation: anti-bootloop protection ---
#define L_DOC_P_TITRE        "&#128737; Bootloop aşırı yük koruması"
#define L_DOC_P_TEXTE1       "MPPT aşırı yük nedeniyle kesilirse ESP32 yeniden başlar ve MOSFET'i tekrar açar — sonsuz döngü oluşur."
#define L_DOC_P_TEXTE2       "Koruma: Kararlı bir süre olmaksızın <code>N önyükleme</code> algılanırsa MOSFET <code>Kilit süresi</code> boyunca KAPALI kilitlenir. Kilit NVS'de saklanır ve tam güç kesimine dayanır."

// --- Documentation: SOC ---
#define L_DOC_S_TITRE        "&#128267; SOC \xe2\x80\x94 Akü şarj durumu"
#define L_DOC_S_TEXTE1       "<b>SOC</b> (State Of Charge) = kalan şarj yüzdesi. <b>Coulomb sayımı</b> ile hesaplanır: INA226 akım integrasyonu (Ah giriş - Ah çıkış) / toplam kapasite."
#define L_DOC_S_TEXTE2       "Gerilim <code>Tam şarj gerilimini</code> aştığında otomatik olarak <b>%%100</b>'e sıfırlanır. Düzenli kalibrasyon olmadan hata birikebiili."

// --- Documentation: sleep cycle ---
#define L_DOC_V_TITRE        "&#128564; Uyku / uyanma döngüsü"
#define L_DOC_V_LI1          "<code>Sabah uyanma saatinden</code> <code>Akşam uyku saatine</code> kadar aktif"
#define L_DOC_V_LI2          "Gece <code>N ölçüm</code> dilimine bölünür \xe2\x80\x94 her uyanışta gerilim ölçülür"
#define L_DOC_V_LI3          "Dilimler arası derin uyku (~10\xc2\xb5A)"

// --- Documentation: version ---
#define L_DOC_VER_TITRE      "&#8505; Sürüm"
#define L_DOC_VER_TEXTE      "Güneş Kontrolcüsü <b>123f</b> \xe2\x80\x94 ESP32 + VEML7700 + INA226 + MPPT"

// --- Page /upd: weather update ---
#define L_UPD_TITRE          "Hava durumu güncelleniyor..."
#define L_UPD_SOUS_TITRE     "OWM + NASA işleniyor (~15s)"
#define L_UPD_TRAITEMENT     "İşleniyor..."

// --- Debug/bilan transition pages ---
#define L_DBG_BIL_FORCE_OK   "&#9654; Arşivleniyor..."
#define L_DBG_BIL_UNDO_OK    "&#8635; Geri alınıyor..."
#define L_DBG_BIL_RESET_OK   "&#128465; Sıfırlanıyor..."

// --- SVG: extra chart labels ---
#define L_SVG_JOUR_N         "G-%d"
#define L_SVG_AUJ            "Bugün"
#define L_SVG_AUJ_COURT      "bugün"
#define L_SVG_APRES_JOURS    "(birkaç gün sonra mevcut)"
#define L_CHARGEMENT         "yükleniyor..."

// --- Menu: weather update ---
#define L_MENU_MAJ_METEO     "&#x27F3; Hava durumunu güncelle"

// --- Badge anti-bootloop: JS prefix ---
#define L_BADGE_BL_AVANT     "&#9888; Bootloop koruması aktif \xe2\x80\x94 MOSFET KAPALI "

// --- Debug: last reset labels ---
#define L_DBG_RST_CRASH_LBL  "&#9888; SON SIFIRLAMA: "
#define L_DBG_RST_OK_LBL     "&#10003; Son sıfırlama: "

// --- Documentation: badge labels (inline examples) ---
#define L_DOC_LBL_CHARGE     "YÜK AÇIK/KAPALI"
#define L_DOC_LBL_SEUILON    "AÇIK eşiği"
#define L_DOC_LBL_COUPURE    "Kesim"
