// =============================================================================
// Solar Controller — Language KO (한국어)
// To switch language: replace #include "lang_fr.h" with #include "lang_ko.h"
// =============================================================================
#pragma once

// --- SVG : chart titles ---
#define L_SVG_PRESENT        "현재 \xe2\x80\x94 최근 24시간"
#define L_SVG_NOCTURNE       "야간 상태 \xe2\x80\x94 배터리 전압"
#define L_SVG_FUTUR          "예보 \xe2\x80\x94 향후 7일 (lux)"
#define L_SVG_PASSE          "기록 \xe2\x80\x94 최근 30일"
#define L_SVG_ENERGIE        "에너지 \xe2\x80\x94 전류 &amp; 전력 24h"
#define L_SVG_BILAN          "일일 로그 30일 \xe2\x80\x94 Ah 입/출"

// --- SVG : legends ---
#define L_SVG_TENSION        "전압 (V)"
#define L_SVG_LUMINOSITE     "조도 (lux)"
#define L_SVG_TENSION_NUIT   "야간 전압 (V)"
#define L_SVG_COURANT        "전류 (A)"
#define L_SVG_PUISSANCE      "전력 (W)"
#define L_SVG_ENTRANTS       "입력 (Ah)"
#define L_SVG_SORTANTS       "출력 (Ah)"
#define L_SVG_COUPURE        "차단 %.2fV"
#define L_SVG_REPRISE        "재개"
#define L_SVG_SEUIL_ON       "ON 임계값"
#define L_SVG_ATTENTE        "첫 측정 대기 중..."
#define L_SVG_MAJ            "(매시간 업데이트)"
#define L_SVG_PAS_DONNEES    "데이터 없음"
#define L_SVG_BARRE_VISIBLE  "(오늘 막대 %d분 후 표시)"

// --- Main page header ---
#define L_HDR_CHARGE_ON      "부하 : ON"
#define L_HDR_CHARGE_OFF     "부하 : OFF"
#define L_HDR_LUX            "Lux: "
#define L_HDR_SEUIL_ON       "ON 임계값: "
#define L_HDR_COUPURE        "차단: "
#define L_HDR_HORS_MPPT      "&#9888; MPPT 없음 &mdash; GND 비공유 &mdash; 측정값 무효"
#define L_HDR_TOAST          "☀️ 오늘 예보가 업데이트되었습니다"

// --- Main page sections ---
#define L_SEC_PRESENT        "현재"
#define L_SEC_NOCTURNE       "야간 상태"
#define L_SEC_ENERGIE        "에너지 24H"
#define L_SEC_BILAN          "일일 로그 30D"
#define L_SEC_FUTUR          "예보"
#define L_SEC_PASSE          "기록"

// --- Hamburger menu ---
#define L_MENU_ACCUEIL       "&#8962; 홈"
#define L_MENU_CONFIG        "&#9881; 설정"
#define L_MENU_DEBUG         "&#128196; 디버그"
#define L_MENU_DEBUG_BILAN   "&#128202; 디버그 로그"
#define L_MENU_DOC           "&#9432; 문서"

// --- Common buttons ---
#define L_BTN_RETOUR         "&#8592; 뒤로"
#define L_BTN_ENREGISTRER    "💾 저장"
#define L_BTN_ANNULER        "&#8635; 마지막 취소"
#define L_BTN_RESET          "&#128465; 0일 초기화"
#define L_BTN_DEBLOQUER      "&#128275; MOSFET 잠금 해제"

// --- Settings page: titles and tabs ---
#define L_CFG_TITRE          "&#9881; 설정"
#define L_TAB_GENERAL        "일반"
#define L_TAB_LUMINOSITE     "조도"
#define L_TAB_BATTERIE       "배터리"
#define L_CFG_SAUVEGARDEE    "✓ 설정이 저장되었습니다. 돌아갑니다..."

// --- Settings > General ---
#define L_CFG_OWM_KEY        "OpenWeatherMap API 키"
#define L_CFG_RESEAU         "&#127760; 네트워크 — 고정 IP (재시작 필요)"
#define L_CFG_IP             "ESP32 IP"
#define L_CFG_GW             "게이트웨이"
#define L_CFG_MASQUE         "서브넷 마스크"
#define L_CFG_DNS            "DNS"
#define L_CFG_LAT            "위도"
#define L_CFG_LON            "경도"
#define L_CFG_TILT           "패널 경사각"
#define L_CFG_TILT_UNIT      "° (0=수평, 90=수직)"
#define L_CFG_AZIMUTH        "패널 방위각"
#define L_CFG_AZIMUTH_UNIT   "° (180=남, 270=서)"
#define L_CFG_RATIO          "출력 전압 (부하)"
#define L_CFG_RATIO_HINT     "150k&#937; + 33k&#937; → (150+33)/33 = 5.5454"
#define L_CFG_SLEEP          "취침 시간"
#define L_CFG_SLEEP_UNIT     "분 (기본값 19:45)"
#define L_CFG_REVEIL         "기상 시간"
#define L_CFG_REVEIL_UNIT    "분 (기본값 09:15)"
#define L_CFG_RELEVES        "야간 측정 횟수"
#define L_CFG_RELEVES_HINT   "야간 창을 균등 분할하여 측정"
#define L_CFG_ARCHIVAGE      "취침 전 로그 보관"
#define L_CFG_ARCHIVAGE_UNIT "시간 전 (0=취침 시, 1-3시간 전)"
#define L_CFG_ARCHIVAGE_HINT "취침 N시간 전에 보관. 권장: 1시간"

// --- Settings > Light ---
#define L_CFG_LUX_MAX        "최대 lux 임계값 (낮은 배터리 + 악천후)"
#define L_CFG_LUX_MAX_HINT   "불리한 조건에서의 ON 임계값"
#define L_CFG_LUX_AMP        "동적 범위"
#define L_CFG_LUX_AMP_HINT   "최소 ON 임계값 = 기본값 - 범위 (이상적 조건)"
#define L_CFG_LUX_CONF       "기상 신뢰도 100% lux"
#define L_CFG_LUX_CONF_HINT  "이 값 이상이면 기상 예보 100% 신뢰"
#define L_CFG_LUX_ACTUEL     "현재 ON 임계값: "
#define L_CFG_LUX_PLAGE      "범위: "

// --- Settings > Battery ---
#define L_CFG_V_PLEINE       "완전 충전 전압"
#define L_CFG_V_REPRISE      "충전 재개 전압"
#define L_CFG_V_REPRISE_HINT "충전을 재개할 수 있는 임계값"
#define L_CFG_V_COUPURE      "차단 전압 (맑은 날씨)"
#define L_CFG_V_COUPURE_HINT "악천후 예보 시 정적 임계값"
#define L_CFG_V_CRITIQUE     "임계 전압"
#define L_CFG_V_PLANCHER     "절대 하한선 (이보다 낮아지지 않음)"
#define L_CFG_V_PLANCHER_HINT "맑은 날씨 차단 하한 — 최소 11.9V 권장"
#define L_CFG_CAPACITE       "배터리 용량 (INA226 쿨롱 계수)"
#define L_CFG_CAPACITE_HINT  "공칭 용량 — 예: 12V/11.5Ah면 11.5Ah"
#define L_CFG_ANTIREBOOT     "과부하 재부팅 지연"
#define L_CFG_ANTIREBOOT_HINT "부트루프 감지 창 (기본값 60초)"
#define L_CFG_CADENCE        "측정 / 저장 간격"
#define L_CFG_CADENCE_HINT   "센서 폴링, Ah 계수, NVS 저장 간격. 기본값: 10분"
#define L_CFG_BLOCAGE        "과부하 시 MOSFET 잠금 시간"
#define L_CFG_COUPURE_DYN    "현재 동적 차단: "

// --- Alert badges ---
#define L_BADGE_BLOCAGE      "&#9888; 부트루프 보호"
#define L_BADGE_HORS_MPPT    "&#9888; MPPT 없음"

// --- Debug Log page ---
#define L_DBG_BILAN_TITRE    "&#128202; 디버그 일일 로그 30일"
#define L_DBG_BILAN_AH_IN    "<b>ahIn:</b>"
#define L_DBG_BILAN_AH_OUT   "<b>ahOut:</b>"

// --- Debug logs page ---
#define L_DBG_TITRE          "&#11035; 디버그 \xe2\x80\x94 시스템 로그"
#define L_DBG_REFRESH        "(60초 갱신)"

// --- Documentation page ---
#define L_DOC_TITRE          "&#9432; 문서 \xe2\x80\x94 태양광 컨트롤러"
#define L_DOC_MATERIEL       "&#128268; 하드웨어"
#define L_DOC_CAPTEURS       "센서"
#define L_DOC_SORTIES        "출력"
#define L_DOC_BADGES         "&#127968; 헤더 배지"
#define L_DOC_GRAPHIQUES     "&#128202; 그래프"
#define L_DOC_LOGIQUE        "&#9889; 충전 논리"
#define L_DOC_PROTECTION     "&#128737; 부트루프 과부하 보호"
#define L_DOC_SOC            "&#128267; SOC \xe2\x80\x94 배터리 충전 상태"

// --- Form validation (JS) ---
#define L_VAL_HEURE_SLEEP    "잘못된 취침 시간 (0-23)"
#define L_VAL_MIN_SLEEP      "잘못된 취침 분 (0-59)"
#define L_VAL_HEURE_REVEIL   "잘못된 기상 시간 (0-23)"
#define L_VAL_MIN_REVEIL     "잘못된 기상 분 (0-59)"
#define L_VAL_PLAGE_COURTE   "야간 창이 너무 짧습니다 (최소 2시간)"

// --- Reset reasons (debug page) ---
#define L_RST_POWERON        "전원 투입"
#define L_RST_EXT            "외부 리셋 (EN)"
#define L_RST_SW             "소프트웨어 재시작"
#define L_RST_PANIC          "PANIC / 예외 (충돌!)"
#define L_RST_INT_WDT        "인터럽트 워치독"
#define L_RST_TASK_WDT       "태스크 워치독 (루프 차단!)"
#define L_RST_WDT            "기타 워치독"
#define L_RST_DEEPSLEEP      "딥 슬립 복귀 (정상)"
#define L_RST_BROWNOUT       "브라운아웃 (저전압!)"
#define L_RST_SDIO           "SDIO 리셋"
#define L_RST_INCONNU        "알 수 없음"

// --- AP config page (first boot) ---
#define L_AP_TITRE           "태양광 컨트롤러 설정"
#define L_AP_BTN_SAVE        "저장"

// --- Main page: section titles ---
#define L_H3_PRESENT         "&#9201; 현재"
#define L_H3_NOCTURNE        "&#127762; 야간 상태"
#define L_H3_ENERGIE         "&#128262; 에너지 24h"
#define L_H3_BILAN           "&#9878; 일일 로그 30일"
#define L_H3_FUTUR           "&#128301; 예보"
#define L_H3_PASSE           "&#128197; 기록"

// --- Anti-bootloop badge (dynamic) ---
#define L_BADGE_BL_ACTIF     "&#9888; 부트루프 보호 활성 \xe2\x80\x94 MOSFET OFF %s"

// --- Off-MPPT inline ---
#define L_HORS_MPPT_INLINE   "&#9888; MPPT 없음 &mdash; GND 비공유 &mdash; 측정값 무효"

// --- Footer credit ---
#define L_FOOTER_CREDIT      "v1.23f &mdash; 제작:"

// --- Config: brightness range hint ---
#define L_CFG_LUX_PLAGE_VAL  "범위: "

// --- Debug page ---
#define L_DBG_HEAP           "여유 RAM: "
#define L_DBG_OCTETS         " 바이트 \xe2\x80\x94 "
#define L_DBG_UPTIME         "가동 시간: "
#define L_DBG_WIFI           "WiFi: "
#define L_DBG_BL_CONFIRM     "부트루프 잠금을 비활성화하시겠습니까?"
#define L_DBG_BL_BTN         "&#128737; 잠금 비활성화"
#define L_DBG_BL_OK          "&#10003; 잠금이 비활성화되었습니다. 돌아갑니다..."

// --- Debug balance: table and buttons ---
#define L_DBG_BIL_FORCE      "&#9654; 강제 보관"
#define L_DBG_BIL_TH_CASE    "슬롯"
#define L_DBG_BIL_TH_NOTE    "메모"
#define L_DBG_BIL_PROCHAIN   "다음"
#define L_DBG_BIL_VALIDE     "<span class='ok'>유효</span>"
#define L_DBG_BIL_VIDE       "<span style='color:#333'>비어있음</span>"
#define L_DBG_BIL_AHIN       "<b>Ah 입력:</b>"
#define L_DBG_BIL_AHOUT      "<b>Ah 출력:</b>"

// --- Documentation: hardware ---
#define L_DOC_VEML           "조도 (lux) Wire0 GPIO 21/22 주소 0x10"
#define L_DOC_INA            "전류 &amp; 배터리 전압 Wire1 GPIO 19/23 주소 0x40"
#define L_DOC_GPIO34         "분압기 (조정 가능한 비율)"
#define L_DOC_GPIO18         "MOSFET (충전 제어)"
#define L_DOC_GPIO2          "표시 LED (선택 사항)"

// --- Documentation: badges ---
#define L_DOC_BADGE_V        "분압기를 통한 배터리 전압 (GPIO 34)"
#define L_DOC_BADGE_CHARGE   "MOSFET 상태"
#define L_DOC_BADGE_LUX      "현재 조도 (VEML7700)"
#define L_DOC_BADGE_SEUIL    "충전 허용 최소 조도 (동적)"
#define L_DOC_BADGE_COUPURE  "차단 전 최소 전압 (동적)"
#define L_DOC_BADGE_SOC      "배터리 아이콘 \xe2\x80\x94 INA226 쿨롱 계수 SOC%%. <span class='badge green'>녹색 &gt;70%%</span> <span class='badge orange'>주황 30-70%%</span> <span class='badge red'>빨강 &lt;30%%</span>. &#9889;=충전, &#9660;=방전."
#define L_DOC_BADGE_HORS     "공통 GND 미감지 \xe2\x80\x94 전압 측정값 무효"
#define L_DOC_BADGE_BL       "과부하 보호 활성 \xe2\x80\x94 MOSFET 잠금 OFF"

// --- Documentation: charts ---
#define L_DOC_G_PRESENT      "<b>현재 24h</b> \xe2\x80\x94 최근 24시간 전압 (V) 및 조도 (lux)"
#define L_DOC_G_NOCTURNE     "<b>야간 상태</b> \xe2\x80\x94 야간 기상 시 배터리 전압"
#define L_DOC_G_ENERGIE      "<b>에너지 24h</b> \xe2\x80\x94 24시간 INA226 전류 (A) 및 전력 (W)"
#define L_DOC_G_BILAN        "<b>일일 로그 30일</b> \xe2\x80\x94 일별 Ah 입력 (태양광 충전) 및 출력 (소비)"
#define L_DOC_G_FUTUR        "<b>예보 7일</b> \xe2\x80\x94 OpenWeatherMap 7일 조도 예보"
#define L_DOC_G_PASSE        "<b>기록 30일</b> \xe2\x80\x94 30일 전압 및 조도 기록"

// --- Documentation: charge logic ---
#define L_DOC_L_ON           "MOSFET <b>ON</b>: lux &gt; ON 임계값 <b>AND</b> 전압 &gt; 재개 전압 <b>AND</b> 배터리 OK"
#define L_DOC_L_OFF          "MOSFET <b>OFF</b>: lux &lt; 150lx <b>OR</b> 전압 &lt; 차단 <b>OR</b> 배터리 KO"
#define L_DOC_L_DYN          "lux 임계값과 차단 전압은 <b>동적</b> \xe2\x80\x94 OWM 기상 예보에 의해 조정됨"

// --- Documentation: anti-bootloop protection ---
#define L_DOC_P_TITRE        "&#128737; 부트루프 과부하 보호"
#define L_DOC_P_TEXTE1       "과부하로 MPPT가 차단되면 ESP32가 재시작되고 MOSFET을 다시 ON하여 무한 루프가 발생합니다."
#define L_DOC_P_TEXTE2       "보호: 안정적인 기간 없이 <code>N번 연속 부팅</code>이 감지되면 MOSFET이 <code>잠금 시간</code>동안 OFF로 잠깁니다. 잠금은 NVS에 저장되어 완전 전원 차단 후에도 유지됩니다."

// --- Documentation: SOC ---
#define L_DOC_S_TITRE        "&#128267; SOC \xe2\x80\x94 배터리 충전 상태"
#define L_DOC_S_TEXTE1       "<b>SOC</b> (State Of Charge) = 잔여 충전 비율. <b>쿨롱 계수</b>로 산출: INA226 전류 시간 적분 (Ah 입력 - Ah 출력) / 전체 용량."
#define L_DOC_S_TEXTE2       "전압이 <code>완전 충전 전압</code>을 초과하면 자동으로 <b>100%%</b>로 리셋됩니다. 정기적인 재보정이 필요하며 사용하면서 개선됩니다."

// --- Documentation: sleep cycle ---
#define L_DOC_V_TITRE        "&#128564; 수면 / 기상 주기"
#define L_DOC_V_LI1          "<code>아침 기상 시간</code>부터 <code>저녁 취침 시간</code>까지 활성"
#define L_DOC_V_LI2          "야간은 <code>N개 측정</code> 슬롯으로 분할 \xe2\x80\x94 각 기상 시 전압 측정"
#define L_DOC_V_LI3          "슬롯 사이 딥 슬립 (~10\xc2\xb5A)"

// --- Documentation: version ---
#define L_DOC_VER_TITRE      "&#8505; 버전"
#define L_DOC_VER_TEXTE      "태양광 컨트롤러 <b>123f</b> \xe2\x80\x94 ESP32 + VEML7700 + INA226 + MPPT"

// --- Page /upd: weather update ---
#define L_UPD_TITRE          "날씨 업데이트 중..."
#define L_UPD_SOUS_TITRE     "OWM + NASA 처리 중 (~15초)"
#define L_UPD_TRAITEMENT     "처리 중..."

// --- Debug/bilan transition pages ---
#define L_DBG_BIL_FORCE_OK   "&#9654; 보관 중..."
#define L_DBG_BIL_UNDO_OK    "&#8635; 취소 중..."
#define L_DBG_BIL_RESET_OK   "&#128465; 초기화 중..."

// --- SVG: extra chart labels ---
#define L_SVG_JOUR_N         "D-%d"
#define L_SVG_AUJ            "오늘"
#define L_SVG_AUJ_COURT      "오늘"
#define L_SVG_APRES_JOURS    "(며칠 후 이용 가능)"
#define L_CHARGEMENT         "로딩 중..."

// --- Menu: weather update ---
#define L_MENU_MAJ_METEO     "&#x27F3; 날씨 업데이트"

// --- Badge anti-bootloop: JS prefix ---
#define L_BADGE_BL_AVANT     "&#9888; 부트루프 보호 활성 \xe2\x80\x94 MOSFET OFF "

// --- Debug: last reset labels ---
#define L_DBG_RST_CRASH_LBL  "&#9888; 마지막 리셋: "
#define L_DBG_RST_OK_LBL     "&#10003; 마지막 리셋: "

// --- Documentation: badge labels (inline examples) ---
#define L_DOC_LBL_CHARGE     "부하 ON/OFF"
#define L_DOC_LBL_SEUILON    "ON 임계값"
#define L_DOC_LBL_COUPURE    "차단"
