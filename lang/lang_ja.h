// =============================================================================
// Solar Controller — Language JA (日本語)
// To switch language: replace #include "lang_fr.h" with #include "lang_ja.h"
// =============================================================================
#pragma once

// --- SVG : chart titles ---
#define L_SVG_PRESENT        "現在 — 過去24時間"
#define L_SVG_NOCTURNE       "夜間状態 — バッテリー電圧"
#define L_SVG_FUTUR          "予報 — 今後7日間 (lux)"
#define L_SVG_PASSE          "履歴 — 過去30日間"
#define L_SVG_ENERGIE        "電力 — 電流と電力 24h"
#define L_SVG_BILAN          "日次ログ 30日 — Ah 入/出"

// --- SVG : legends ---
#define L_SVG_TENSION        "電圧 (V)"
#define L_SVG_LUMINOSITE     "照度 (lux)"
#define L_SVG_TENSION_NUIT   "夜間電圧 (V)"
#define L_SVG_COURANT        "電流 (A)"
#define L_SVG_PUISSANCE      "電力 (W)"
#define L_SVG_ENTRANTS       "入力 (Ah)"
#define L_SVG_SORTANTS       "出力 (Ah)"
#define L_SVG_COUPURE        "遮断 %.2fV"
#define L_SVG_REPRISE        "再開"
#define L_SVG_SEUIL_ON       "ON閾値"
#define L_SVG_ATTENTE        "最初の測定を待っています..."
#define L_SVG_MAJ            "(毎時更新)"
#define L_SVG_PAS_DONNEES    "データなし"
#define L_SVG_BARRE_VISIBLE  "(今日の棒グラフは%d分後に表示)"

// --- Main page header ---
#define L_HDR_CHARGE_ON      "負荷 : ON"
#define L_HDR_CHARGE_OFF     "負荷 : OFF"
#define L_HDR_LUX            "Lux: "
#define L_HDR_SEUIL_ON       "ON閾値: "
#define L_HDR_COUPURE        "遮断: "
#define L_HDR_HORS_MPPT      "&#9888; MPPTなし &mdash; GND非共有 &mdash; 測定値無効"
#define L_HDR_TOAST          "☀️ 本日の予報を更新しました"

// --- Main page sections ---
#define L_SEC_PRESENT        "現在"
#define L_SEC_NOCTURNE       "夜間状態"
#define L_SEC_ENERGIE        "電力 24H"
#define L_SEC_BILAN          "日次ログ 30D"
#define L_SEC_FUTUR          "予報"
#define L_SEC_PASSE          "履歴"

// --- Hamburger menu ---
#define L_MENU_ACCUEIL       "&#8962; ホーム"
#define L_MENU_CONFIG        "&#9881; 設定"
#define L_MENU_DEBUG         "&#128196; デバッグ"
#define L_MENU_DEBUG_BILAN   "&#128202; ログデバッグ"
#define L_MENU_DOC           "&#9432; ドキュメント"

// --- Common buttons ---
#define L_BTN_RETOUR         "&#8592; 戻る"
#define L_BTN_ENREGISTRER    "💾 保存"
#define L_BTN_ANNULER        "&#8635; 最後を元に戻す"
#define L_BTN_RESET          "&#128465; 0日リセット"
#define L_BTN_DEBLOQUER      "&#128275; MOSFETロック解除"

// --- Settings page: titles and tabs ---
#define L_CFG_TITRE          "&#9881; 設定"
#define L_TAB_GENERAL        "一般"
#define L_TAB_LUMINOSITE     "照度"
#define L_TAB_BATTERIE       "バッテリー"
#define L_CFG_SAUVEGARDEE    "✓ 設定を保存しました。戻ります..."

// --- Settings > General ---
#define L_CFG_OWM_KEY        "OpenWeatherMap APIキー"
#define L_CFG_RESEAU         "&#127760; ネットワーク — 静的IP (再起動が必要)"
#define L_CFG_IP             "ESP32 IP"
#define L_CFG_GW             "ゲートウェイ"
#define L_CFG_MASQUE         "サブネットマスク"
#define L_CFG_DNS            "DNS"
#define L_CFG_LAT            "緯度"
#define L_CFG_LON            "経度"
#define L_CFG_TILT           "パネル傾斜角"
#define L_CFG_TILT_UNIT      "° (0=水平, 90=垂直)"
#define L_CFG_AZIMUTH        "パネル方位角"
#define L_CFG_AZIMUTH_UNIT   "° (180=南, 270=西)"
#define L_CFG_RATIO          "出力電圧 (負荷)"
#define L_CFG_RATIO_HINT     "150k&#937; + 33k&#937; → (150+33)/33 = 5.5454"
#define L_CFG_SLEEP          "就寝時刻"
#define L_CFG_SLEEP_UNIT     "分 (既定 19:45)"
#define L_CFG_REVEIL         "起床時刻"
#define L_CFG_REVEIL_UNIT    "分 (既定 09:15)"
#define L_CFG_RELEVES        "夜間測定回数"
#define L_CFG_RELEVES_HINT   "夜間を等間隔に分割して測定"
#define L_CFG_ARCHIVAGE      "就寝前のログ保存"
#define L_CFG_ARCHIVAGE_UNIT "h前 (0=就寝時, 1-3h前)"
#define L_CFG_ARCHIVAGE_HINT "就寝N時間前にアーカイブ。推奨: 1h"

// --- Settings > Light ---
#define L_CFG_LUX_MAX        "最大lux閾値 (低バッテリー+悪天候)"
#define L_CFG_LUX_MAX_HINT   "条件が悪い時のON閾値"
#define L_CFG_LUX_AMP        "ダイナミックレンジ"
#define L_CFG_LUX_AMP_HINT   "最小ON閾値 = 既定 - レンジ (理想条件)"
#define L_CFG_LUX_CONF       "天気信頼度100% lux"
#define L_CFG_LUX_CONF_HINT  "この値以上で天気予報を100%信頼"
#define L_CFG_LUX_ACTUEL     "現在のON閾値: "
#define L_CFG_LUX_PLAGE      "レンジ: "

// --- Settings > Battery ---
#define L_CFG_V_PLEINE       "満充電電圧"
#define L_CFG_V_REPRISE      "充電再開電圧"
#define L_CFG_V_REPRISE_HINT "充電再開可能な閾値"
#define L_CFG_V_COUPURE      "遮断電圧 (晴天時)"
#define L_CFG_V_COUPURE_HINT "悪天候予報時の静的閾値"
#define L_CFG_V_CRITIQUE     "臨界電圧"
#define L_CFG_V_PLANCHER     "絶対下限 (これ以下にならない)"
#define L_CFG_V_PLANCHER_HINT "晴天時の遮断下限 — 最低11.9V推奨"
#define L_CFG_CAPACITE       "バッテリー容量 (INA226クーロン計測)"
#define L_CFG_CAPACITE_HINT  "公称容量 — 例: 12V/11.5Ahなら11.5Ah"
#define L_CFG_ANTIREBOOT     "過負荷時の再起動遅延"
#define L_CFG_ANTIREBOOT_HINT "bootloop検出窓 (既定60s)"
#define L_CFG_CADENCE        "測定/保存間隔"
#define L_CFG_CADENCE_HINT   "センサーポーリング、Ah計測、NVS保存間隔。既定: 10分"
#define L_CFG_BLOCAGE        "過負荷時MOSFETロック時間"
#define L_CFG_COUPURE_DYN    "現在の動的遮断: "

// --- Alert badges ---
#define L_BADGE_BLOCAGE      "&#9888; bootloop保護"
#define L_BADGE_HORS_MPPT    "&#9888; MPPTなし"

// --- Debug Log page ---
#define L_DBG_BILAN_TITRE    "&#128202; デバッグ 日次ログ 30日"
#define L_DBG_BILAN_AH_IN    "<b>ahIn:</b>"
#define L_DBG_BILAN_AH_OUT   "<b>ahOut:</b>"

// --- Debug logs page ---
#define L_DBG_TITRE          "&#11035; デバッグ — システムログ"
#define L_DBG_REFRESH        "(60秒更新)"

// --- Documentation page ---
#define L_DOC_TITRE          "&#9432; ドキュメント — ソーラーコントローラー"
#define L_DOC_MATERIEL       "&#128268; ハードウェア"
#define L_DOC_CAPTEURS       "センサー"
#define L_DOC_SORTIES        "出力"
#define L_DOC_BADGES         "&#127968; ヘッダーバッジ"
#define L_DOC_GRAPHIQUES     "&#128202; グラフ"
#define L_DOC_LOGIQUE        "&#9889; 充電ロジック"
#define L_DOC_PROTECTION     "&#128737; bootloop過負荷保護"
#define L_DOC_SOC            "&#128267; SOC — バッテリー残量"

// --- Form validation (JS) ---
#define L_VAL_HEURE_SLEEP    "就寝時間が無効 (0-23)"
#define L_VAL_MIN_SLEEP      "就寝分が無効 (0-59)"
#define L_VAL_HEURE_REVEIL   "起床時間が無効 (0-23)"
#define L_VAL_MIN_REVEIL     "起床分が無効 (0-59)"
#define L_VAL_PLAGE_COURTE   "夜間ウィンドウが短すぎます (最低2h)"

// --- Reset reasons (debug page) ---
#define L_RST_POWERON        "電源投入"
#define L_RST_EXT            "外部リセット (EN)"
#define L_RST_SW             "ソフトウェア再起動"
#define L_RST_PANIC          "PANIC / 例外 (クラッシュ!)"
#define L_RST_INT_WDT        "割り込みウォッチドッグ"
#define L_RST_TASK_WDT       "タスクウォッチドッグ (ループブロック!)"
#define L_RST_WDT            "その他ウォッチドッグ"
#define L_RST_DEEPSLEEP      "ディープスリープ復帰 (正常)"
#define L_RST_BROWNOUT       "ブラウンアウト (低電圧!)"
#define L_RST_SDIO           "SDIOリセット"
#define L_RST_INCONNU        "不明"

// --- AP config page (first boot) ---
#define L_AP_TITRE           "ソーラー設定"
#define L_AP_BTN_SAVE        "保存"

// --- Main page: section titles ---
#define L_H3_PRESENT         "&#9201; 現在"
#define L_H3_NOCTURNE        "&#127762; 夜間状態"
#define L_H3_ENERGIE         "&#128262; 電力 24h"
#define L_H3_BILAN           "&#9878; 日次ログ 30日"
#define L_H3_FUTUR           "&#128301; 予報"
#define L_H3_PASSE           "&#128197; 履歴"

// --- Anti-bootloop badge (dynamic) ---
#define L_BADGE_BL_ACTIF     "&#9888; bootloop保護アクティブ \xe2\x80\x94 MOSFETオフ %s"

// --- Off-MPPT inline ---
#define L_HORS_MPPT_INLINE   "&#9888; MPPTなし &mdash; GND非共有 &mdash; 測定値無効"

// --- Footer credit ---
#define L_FOOTER_CREDIT      "v1.23f &mdash; 作者:"

// --- Config: brightness range hint ---
#define L_CFG_LUX_PLAGE_VAL  "レンジ: "

// --- Debug page ---
#define L_DBG_HEAP           "空きRAM: "
#define L_DBG_OCTETS         " バイト \xe2\x80\x94 "
#define L_DBG_UPTIME         "稼働時間: "
#define L_DBG_WIFI           "WiFi: "
#define L_DBG_BL_CONFIRM     "bootloopロックを解除しますか？"
#define L_DBG_BL_BTN         "&#128737; ロック解除"
#define L_DBG_BL_OK          "&#10003; ロックを解除しました。戻ります..."

// --- Debug balance: table and buttons ---
#define L_DBG_BIL_FORCE      "&#9654; 強制アーカイブ"
#define L_DBG_BIL_TH_CASE    "スロット"
#define L_DBG_BIL_TH_NOTE    "メモ"
#define L_DBG_BIL_PROCHAIN   "次"
#define L_DBG_BIL_VALIDE     "<span class='ok'>有効</span>"
#define L_DBG_BIL_VIDE       "<span style='color:#333'>空</span>"
#define L_DBG_BIL_AHIN       "<b>Ah 入力:</b>"
#define L_DBG_BIL_AHOUT      "<b>Ah 出力:</b>"

// --- Documentation: hardware ---
#define L_DOC_VEML           "照度 (lux) Wire0 GPIO 21/22 アドレス 0x10"
#define L_DOC_INA            "電流 &amp; 電圧 Wire1 GPIO 19/23 アドレス 0x40"
#define L_DOC_GPIO34         "分圧器 (可変比率)"
#define L_DOC_GPIO18         "MOSFET (充電制御)"
#define L_DOC_GPIO2          "インジケーターLED (オプション)"

// --- Documentation: badges ---
#define L_DOC_BADGE_V        "分圧器によるバッテリー電圧 (GPIO 34)"
#define L_DOC_BADGE_CHARGE   "MOSFET状態"
#define L_DOC_BADGE_LUX      "現在の照度 (VEML7700)"
#define L_DOC_BADGE_SEUIL    "充電許可の最小照度 (動的)"
#define L_DOC_BADGE_COUPURE  "充電遮断前の最小電圧 (動的)"
#define L_DOC_BADGE_SOC      "バッテリーアイコン \xe2\x80\x94 SOC%% INA226クーロン計測。<span class='badge green'>緑 &gt;70%%</span> <span class='badge orange'>橙 30-70%%</span> <span class='badge red'>赤 &lt;30%%</span>。&#9889;=充電, &#9660;=放電。"
#define L_DOC_BADGE_HORS     "共通GND未検出 \xe2\x80\x94 電圧測定値無効"
#define L_DOC_BADGE_BL       "過負荷保護アクティブ \xe2\x80\x94 MOSFETロック中"

// --- Documentation: charts ---
#define L_DOC_G_PRESENT      "<b>現在 24h</b> \xe2\x80\x94 過去24時間の電圧 (V) と照度 (lux)"
#define L_DOC_G_NOCTURNE     "<b>夜間状態</b> \xe2\x80\x94 夜間起動時のバッテリー電圧"
#define L_DOC_G_ENERGIE      "<b>電力 24h</b> \xe2\x80\x94 24時間の電流 (A) と電力 (W) INA226"
#define L_DOC_G_BILAN        "<b>日次ログ 30日</b> \xe2\x80\x94 日ごとのAh入力 (太陽充電) と出力 (消費)"
#define L_DOC_G_FUTUR        "<b>予報 7日</b> \xe2\x80\x94 OpenWeatherMap 7日間照度予報"
#define L_DOC_G_PASSE        "<b>履歴 30日</b> \xe2\x80\x94 30日間の電圧と照度の履歴"

// --- Documentation: charge logic ---
#define L_DOC_L_ON           "MOSFET <b>ON</b>: lux &gt; ON閾値 <b>かつ</b> 電圧 &gt; 再開電圧 <b>かつ</b> バッテリーOK"
#define L_DOC_L_OFF          "MOSFET <b>OFF</b>: lux &lt; 150lx <b>または</b> 電圧 &lt; 遮断 <b>または</b> バッテリーKO"
#define L_DOC_L_DYN          "lux閾値と遮断電圧は<b>動的</b> \xe2\x80\x94 OWM天気予報で調整"

// --- Documentation: anti-bootloop protection ---
#define L_DOC_P_TITRE        "&#128737; bootloop過負荷保護"
#define L_DOC_P_TEXTE1       "過負荷でMPPTが電源を遮断するとESP32が再起動し、再びMOSFETをONにして同じ過負荷を引き起こす無限ループが発生します。"
#define L_DOC_P_TEXTE2       "保護: 安定期間なしに<code>N回連続起動</code>を検出すると、MOSFETを<code>ロック時間</code>の間OFFにロック。ロックはNVSに保存され、完全な電源断後も持続します。"

// --- Documentation: SOC ---
#define L_DOC_S_TITRE        "&#128267; SOC \xe2\x80\x94 バッテリー充電状態"
#define L_DOC_S_TEXTE1       "<b>SOC</b> (State Of Charge) = 残充電率。<b>クーロン計測</b>で算出: INA226電流の時間積分 (Ah入力 - Ah出力) / 総容量。"
#define L_DOC_S_TEXTE2       "電圧が<code>満充電電圧</code>を超えると自動的に<b>100%%</b>にリセット。定期的なリセットがないと誤差が生じます。"

// --- Documentation: sleep cycle ---
#define L_DOC_V_TITRE        "&#128564; スリープ/ウェイクサイクル"
#define L_DOC_V_LI1          "<code>朝の起床時刻</code>から<code>夜の就寝時刻</code>まで稼働"
#define L_DOC_V_LI2          "夜間は<code>N回の測定</code>スロットに分割 \xe2\x80\x94 各起動で電圧測定"
#define L_DOC_V_LI3          "スロット間はディープスリープ (~10\xc2\xb5A)"

// --- Documentation: version ---
#define L_DOC_VER_TITRE      "&#8505; バージョン"
#define L_DOC_VER_TEXTE      "ソーラーコントローラー <b>123f</b> \xe2\x80\x94 ESP32 + VEML7700 + INA226 + MPPT"

// --- Page /upd: weather update ---
#define L_UPD_TITRE          "天気を更新中..."
#define L_UPD_SOUS_TITRE     "OWM + NASA 処理中 (~15秒)"
#define L_UPD_TRAITEMENT     "処理中..."

// --- Debug/bilan transition pages ---
#define L_DBG_BIL_FORCE_OK   "&#9654; アーカイブ中..."
#define L_DBG_BIL_UNDO_OK    "&#8635; 元に戻し中..."
#define L_DBG_BIL_RESET_OK   "&#128465; リセット中..."

// --- SVG: extra chart labels ---
#define L_SVG_JOUR_N         "D-%d"
#define L_SVG_AUJ            "今日"
#define L_SVG_AUJ_COURT      "今日"
#define L_SVG_APRES_JOURS    "(数日後に利用可能)"
#define L_CHARGEMENT         "読み込み中..."

// --- Menu: weather update ---
#define L_MENU_MAJ_METEO     "&#x27F3; 天気を更新"

// --- Badge anti-bootloop: JS prefix ---
#define L_BADGE_BL_AVANT     "&#9888; bootloop保護アクティブ \xe2\x80\x94 MOSFETオフ "

// --- Debug: last reset labels ---
#define L_DBG_RST_CRASH_LBL  "&#9888; 最後のリセット: "
#define L_DBG_RST_OK_LBL     "&#10003; 最後のリセット: "

// --- Documentation: badge labels (inline examples) ---
#define L_DOC_LBL_CHARGE     "負荷 ON/OFF"
#define L_DOC_LBL_SEUILON    "ON閾値"
#define L_DOC_LBL_COUPURE    "遮断"
