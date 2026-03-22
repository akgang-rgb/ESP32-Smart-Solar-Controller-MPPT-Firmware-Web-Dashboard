// =============================================================================
// Solar Controller — Language RU (Русский)
// To switch language: replace #include "lang_fr.h" with #include "lang_ru.h"
// =============================================================================
#pragma once

// --- SVG : chart titles ---
#define L_SVG_PRESENT        "ТЕКУЩИЙ — последние 24 ч"
#define L_SVG_NOCTURNE       "НОЧНОЙ СТАТУС — напряжение АКБ"
#define L_SVG_FUTUR          "ПРОГНОЗ — следующие 7 дней (lux)"
#define L_SVG_PASSE          "ИСТОРИЯ — последние 30 дней"
#define L_SVG_ENERGIE        "ЭНЕРГИЯ — Ток и Мощность 24ч"
#define L_SVG_BILAN          "ЖУРНАЛ 30д — Ah вх / вых"

// --- SVG : legends ---
#define L_SVG_TENSION        "Напряжение (В)"
#define L_SVG_LUMINOSITE     "Освещённость (lux)"
#define L_SVG_TENSION_NUIT   "Ночное напряжение (В)"
#define L_SVG_COURANT        "Ток (А)"
#define L_SVG_PUISSANCE      "Мощность (Вт)"
#define L_SVG_ENTRANTS       "Вход (Ah)"
#define L_SVG_SORTANTS       "Выход (Ah)"
#define L_SVG_COUPURE        "откл. %.2fВ"
#define L_SVG_REPRISE        "возобновление"
#define L_SVG_SEUIL_ON       "Порог ВКЛ"
#define L_SVG_ATTENTE        "Ожидание первых измерений..."
#define L_SVG_MAJ            "(обновление каждый час)"
#define L_SVG_PAS_DONNEES    "Нет данных"
#define L_SVG_BARRE_VISIBLE  "(столбец сегодня через %d мин)"

// --- Main page header ---
#define L_HDR_CHARGE_ON      "НАГРУЗКА : ВКЛ"
#define L_HDR_CHARGE_OFF     "НАГРУЗКА : ВЫКЛ"
#define L_HDR_LUX            "Lux: "
#define L_HDR_SEUIL_ON       "Порог ВКЛ: "
#define L_HDR_COUPURE        "Откл.: "
#define L_HDR_HORS_MPPT      "&#9888; Нет MPPT &mdash; GND не общий &mdash; показания недействительны"
#define L_HDR_TOAST          "☀️ Прогноз обновлён на сегодня"

// --- Main page sections ---
#define L_SEC_PRESENT        "ТЕКУЩИЙ"
#define L_SEC_NOCTURNE       "НОЧНОЙ СТАТУС"
#define L_SEC_ENERGIE        "ЭНЕРГИЯ 24Ч"
#define L_SEC_BILAN          "ЖУРНАЛ 30Д"
#define L_SEC_FUTUR          "ПРОГНОЗ"
#define L_SEC_PASSE          "ИСТОРИЯ"

// --- Hamburger menu ---
#define L_MENU_ACCUEIL       "&#8962; Главная"
#define L_MENU_CONFIG        "&#9881; Настройки"
#define L_MENU_DEBUG         "&#128196; Отладка"
#define L_MENU_DEBUG_BILAN   "&#128202; Журнал отладки"
#define L_MENU_DOC           "&#9432; Документация"

// --- Common buttons ---
#define L_BTN_RETOUR         "&#8592; Назад"
#define L_BTN_ENREGISTRER    "💾 Сохранить"
#define L_BTN_ANNULER        "&#8635; Отменить последнее"
#define L_BTN_RESET          "&#128465; Сброс день 0"
#define L_BTN_DEBLOQUER      "&#128275; Разблокировать MOSFET"

// --- Settings page: titles and tabs ---
#define L_CFG_TITRE          "&#9881; Настройки"
#define L_TAB_GENERAL        "Общие"
#define L_TAB_LUMINOSITE     "Свет"
#define L_TAB_BATTERIE       "Аккумулятор"
#define L_CFG_SAUVEGARDEE    "✓ Настройки сохранены. Возврат..."

// --- Settings > General ---
#define L_CFG_OWM_KEY        "Ключ API OpenWeatherMap"
#define L_CFG_RESEAU         "&#127760; Сеть — статический IP (перезапуск)"
#define L_CFG_IP             "IP ESP32"
#define L_CFG_GW             "Шлюз"
#define L_CFG_MASQUE         "Маска подсети"
#define L_CFG_DNS            "DNS"
#define L_CFG_LAT            "Широта"
#define L_CFG_LON            "Долгота"
#define L_CFG_TILT           "Наклон панели"
#define L_CFG_TILT_UNIT      "° (0=горизонт., 90=верт.)"
#define L_CFG_AZIMUTH        "Азимут панели"
#define L_CFG_AZIMUTH_UNIT   "° (180=Юг, 270=Запад)"
#define L_CFG_RATIO          "Напряжение нагрузки"
#define L_CFG_RATIO_HINT     "150k&#937; + 33k&#937; → (150+33)/33 = 5.5454"
#define L_CFG_SLEEP          "Время сна"
#define L_CFG_SLEEP_UNIT     "мин (по умолч. 19:45)"
#define L_CFG_REVEIL         "Время пробуждения"
#define L_CFG_REVEIL_UNIT    "мин (по умолч. 09:15)"
#define L_CFG_RELEVES        "Ночные измерения"
#define L_CFG_RELEVES_HINT   "Ночное окно делится на равные интервалы"
#define L_CFG_ARCHIVAGE      "Архивация журнала перед сном"
#define L_CFG_ARCHIVAGE_UNIT "ч до сна (0=во время сна, 1-3ч до)"
#define L_CFG_ARCHIVAGE_HINT "Архивирует за N часов до сна. Рекомендуется: 1ч"

// --- Settings > Light ---
#define L_CFG_LUX_MAX        "Макс. порог lux (слабый АКБ + плохая погода)"
#define L_CFG_LUX_MAX_HINT   "Порог ВКЛ при неблагоприятных условиях"
#define L_CFG_LUX_AMP        "Динамический диапазон"
#define L_CFG_LUX_AMP_HINT   "Мин. порог ВКЛ = Стандарт - Диапазон (идеальные условия)"
#define L_CFG_LUX_CONF       "Lux 100% доверия погоде"
#define L_CFG_LUX_CONF_HINT  "Выше этого значения прогноз погоды принимается на 100%"
#define L_CFG_LUX_ACTUEL     "Текущий порог ВКЛ: "
#define L_CFG_LUX_PLAGE      "Диапазон: "

// --- Settings > Battery ---
#define L_CFG_V_PLEINE       "Напряжение полного заряда"
#define L_CFG_V_REPRISE      "Напряжение возобновления"
#define L_CFG_V_REPRISE_HINT "Порог, выше которого зарядка может возобновиться"
#define L_CFG_V_COUPURE      "Напряжение отключения (хорошая погода)"
#define L_CFG_V_COUPURE_HINT "Статический порог при плохом прогнозе"
#define L_CFG_V_CRITIQUE     "Критическое напряжение"
#define L_CFG_V_PLANCHER     "Абсолютный минимум (никогда ниже)"
#define L_CFG_V_PLANCHER_HINT "Мин. порог отключения при хорошей погоде — рек. 11.9В"
#define L_CFG_CAPACITE       "Ёмкость АКБ (счёт кулонов INA226)"
#define L_CFG_CAPACITE_HINT  "Номинальная ёмкость — напр. 11.5Ah для 12В/11.5Ah"
#define L_CFG_ANTIREBOOT     "Задержка при перегрузке"
#define L_CFG_ANTIREBOOT_HINT "Окно bootloop (по умолч. 60с) — 2+ перезагрузки → MOSFET OFF 1ч"
#define L_CFG_CADENCE        "Интервал измерений / сохранения"
#define L_CFG_CADENCE_HINT   "Опрос датчиков, счёт Ah и сохр. NVS. По умолч.: 10 мин"
#define L_CFG_BLOCAGE        "Длительность блокировки MOSFET при перегрузке"
#define L_CFG_COUPURE_DYN    "Текущее динамическое откл.: "

// --- Alert badges ---
#define L_BADGE_BLOCAGE      "&#9888; Защита от bootloop"
#define L_BADGE_HORS_MPPT    "&#9888; Нет MPPT"

// --- Debug Log page ---
#define L_DBG_BILAN_TITRE    "&#128202; Отладка журнала 30д"
#define L_DBG_BILAN_AH_IN    "<b>ahIn:</b>"
#define L_DBG_BILAN_AH_OUT   "<b>ahOut:</b>"

// --- Debug logs page ---
#define L_DBG_TITRE          "&#11035; Отладка — системные журналы"
#define L_DBG_REFRESH        "(обновление 60с)"

// --- Documentation page ---
#define L_DOC_TITRE          "&#9432; Документация — Солнечный контроллер"
#define L_DOC_MATERIEL       "&#128268; Оборудование"
#define L_DOC_CAPTEURS       "Датчики"
#define L_DOC_SORTIES        "Выходы"
#define L_DOC_BADGES         "&#127968; Значки заголовка"
#define L_DOC_GRAPHIQUES     "&#128202; Графики"
#define L_DOC_LOGIQUE        "&#9889; Логика заряда"
#define L_DOC_PROTECTION     "&#128737; Защита от bootloop"
#define L_DOC_SOC            "&#128267; SOC — Состояние заряда"

// --- Form validation (JS) ---
#define L_VAL_HEURE_SLEEP    "Неверный час сна (0-23)"
#define L_VAL_MIN_SLEEP      "Неверные минуты сна (0-59)"
#define L_VAL_HEURE_REVEIL   "Неверный час пробуждения (0-23)"
#define L_VAL_MIN_REVEIL     "Неверные минуты пробуждения (0-59)"
#define L_VAL_PLAGE_COURTE   "Окно сна слишком короткое (мин. 2ч)"

// --- Reset reasons (debug page) ---
#define L_RST_POWERON        "Включение питания"
#define L_RST_EXT            "Внешний сброс (EN)"
#define L_RST_SW             "Программный перезапуск"
#define L_RST_PANIC          "PANIC / Исключение (сбой!)"
#define L_RST_INT_WDT        "Watchdog прерывания"
#define L_RST_TASK_WDT       "Watchdog задачи (цикл заблокирован!)"
#define L_RST_WDT            "Другой watchdog"
#define L_RST_DEEPSLEEP      "Пробуждение deep sleep (норма)"
#define L_RST_BROWNOUT       "Brownout (низкое напряжение!)"
#define L_RST_SDIO           "Сброс SDIO"
#define L_RST_INCONNU        "Неизвестно"

// --- AP config page (first boot) ---
#define L_AP_TITRE           "Настройка солнечного контроллера"
#define L_AP_BTN_SAVE        "Сохранить"

// --- Main page: section titles ---
#define L_H3_PRESENT         "&#9201; Текущий"
#define L_H3_NOCTURNE        "&#127762; Ночной статус"
#define L_H3_ENERGIE         "&#128262; Энергия 24ч"
#define L_H3_BILAN           "&#9878; Журнал 30д"
#define L_H3_FUTUR           "&#128301; Прогноз"
#define L_H3_PASSE           "&#128197; История"

// --- Anti-bootloop badge (dynamic) ---
#define L_BADGE_BL_ACTIF     "&#9888; Защита от bootloop активна \xe2\x80\x94 MOSFET ВЫКЛ на %s"

// --- Off-MPPT inline ---
#define L_HORS_MPPT_INLINE   "&#9888; Нет MPPT &mdash; GND не общий &mdash; показания недействительны"

// --- Footer credit ---
#define L_FOOTER_CREDIT      "v1.23f &mdash; Автор:"

// --- Config: brightness range hint ---
#define L_CFG_LUX_PLAGE_VAL  "Диапазон: "

// --- Debug page ---
#define L_DBG_HEAP           "Свободная RAM: "
#define L_DBG_OCTETS         " байт \xe2\x80\x94 "
#define L_DBG_UPTIME         "Время работы: "
#define L_DBG_WIFI           "WiFi: "
#define L_DBG_BL_CONFIRM     "Отключить блокировку bootloop?"
#define L_DBG_BL_BTN         "&#128737; Отключить блокировку"
#define L_DBG_BL_OK          "&#10003; Блокировка отключена. Возврат..."

// --- Debug balance: table and buttons ---
#define L_DBG_BIL_FORCE      "&#9654; Принудительный архив"
#define L_DBG_BIL_TH_CASE    "Слот"
#define L_DBG_BIL_TH_NOTE    "Примечание"
#define L_DBG_BIL_PROCHAIN   "следующий"
#define L_DBG_BIL_VALIDE     "<span class='ok'>действит.</span>"
#define L_DBG_BIL_VIDE       "<span style='color:#333'>пусто</span>"
#define L_DBG_BIL_AHIN       "<b>Ah вход:</b>"
#define L_DBG_BIL_AHOUT      "<b>Ah выход:</b>"

// --- Documentation: hardware ---
#define L_DOC_VEML           "Освещённость (lux) на Wire0 GPIO 21/22 адрес 0x10"
#define L_DOC_INA            "Ток &amp; напряжение АКБ на Wire1 GPIO 19/23 адрес 0x40"
#define L_DOC_GPIO34         "Делитель напряжения (настраиваемый коэффициент)"
#define L_DOC_GPIO18         "MOSFET (управление зарядкой)"
#define L_DOC_GPIO2          "Индикаторный светодиод (опционально)"

// --- Documentation: badges ---
#define L_DOC_BADGE_V        "Напряжение АКБ через делитель (GPIO 34)"
#define L_DOC_BADGE_CHARGE   "Состояние MOSFET"
#define L_DOC_BADGE_LUX      "Текущая освещённость (VEML7700)"
#define L_DOC_BADGE_SEUIL    "Мин. освещённость для разрешения зарядки (динамич.)"
#define L_DOC_BADGE_COUPURE  "Мин. напряжение до отключения зарядки (динамич.)"
#define L_DOC_BADGE_SOC      "Иконка АКБ \xe2\x80\x94 SOC%% счёт кулонов INA226. <span class='badge green'>зелёный &gt;70%%</span> <span class='badge orange'>оранжевый 30-70%%</span> <span class='badge red'>красный &lt;30%%</span>. &#9889; = заряд, &#9660; = разряд."
#define L_DOC_BADGE_HORS     "Общий GND не обнаружен \xe2\x80\x94 показание напряжения недействительно"
#define L_DOC_BADGE_BL       "Защита от перегрузки активна \xe2\x80\x94 MOSFET заблокирован ВЫКЛ"

// --- Documentation: charts ---
#define L_DOC_G_PRESENT      "<b>ТЕКУЩИЙ 24ч</b> \xe2\x80\x94 Напряжение (В) и освещённость (lux) за последние 24ч"
#define L_DOC_G_NOCTURNE     "<b>НОЧНОЙ СТАТУС</b> \xe2\x80\x94 Напряжение АКБ при ночных пробуждениях"
#define L_DOC_G_ENERGIE      "<b>ЭНЕРГИЯ 24ч</b> \xe2\x80\x94 Ток (А) и мощность (Вт) INA226 за 24ч"
#define L_DOC_G_BILAN        "<b>ЖУРНАЛ 30д</b> \xe2\x80\x94 Ah вход (солнечный заряд) и выход (потребление) за день"
#define L_DOC_G_FUTUR        "<b>ПРОГНОЗ 7д</b> \xe2\x80\x94 Прогноз освещённости OpenWeatherMap на 7 дней"
#define L_DOC_G_PASSE        "<b>ИСТОРИЯ 30д</b> \xe2\x80\x94 История напряжения и освещённости за 30 дней"

// --- Documentation: charge logic ---
#define L_DOC_L_ON           "MOSFET <b>ВКЛ</b> если: lux &gt; Порог ВКЛ <b>И</b> напр. &gt; Напр. возобн. <b>И</b> АКБ OK"
#define L_DOC_L_OFF          "MOSFET <b>ВЫКЛ</b> если: lux &lt; 150lx <b>ИЛИ</b> напр. &lt; Откл. <b>ИЛИ</b> АКБ KO"
#define L_DOC_L_DYN          "Порог lux и напряжение отключения <b>динамические</b> \xe2\x80\x94 корректируются прогнозом OWM"

// --- Documentation: anti-bootloop protection ---
#define L_DOC_P_TITRE        "&#128737; Защита от bootloop при перегрузке"
#define L_DOC_P_TEXTE1       "Если MPPT отключает питание из-за перегрузки, ESP32 перезагружается и снова включает MOSFET, вызывая бесконечный цикл."
#define L_DOC_P_TEXTE2       "Защита: при <code>N подряд загрузках</code> без стабильного периода <code>Задержки</code> секунд, MOSFET блокируется ВЫКЛ на <code>Длительность блокировки</code>. Блокировка хранится в NVS и переживает полное обесточивание."

// --- Documentation: SOC ---
#define L_DOC_S_TITRE        "&#128267; SOC \xe2\x80\x94 Состояние заряда аккумулятора"
#define L_DOC_S_TEXTE1       "<b>SOC</b> (State Of Charge) = процент оставшегося заряда. Рассчитывается методом <b>счёта кулонов</b>: интеграция тока INA226 во времени (Ah вход - Ah выход) / полная ёмкость."
#define L_DOC_S_TEXTE2       "Сброс до <b>100%%</b> автоматически, когда напряжение превышает <code>Напряжение полного заряда</code>. Неточен без регулярного сброса \xe2\x80\x94 улучшается с использованием."

// --- Documentation: sleep cycle ---
#define L_DOC_V_TITRE        "&#128564; Цикл сон / пробуждение"
#define L_DOC_V_LI1          "Активен с <code>Времени пробуждения утром</code> до <code>Времени сна вечером</code>"
#define L_DOC_V_LI2          "Ночь разделена на <code>N интервалов</code> \xe2\x80\x94 измерение напряжения при каждом пробуждении"
#define L_DOC_V_LI3          "Deep sleep между интервалами (~10\xc2\xb5A)"

// --- Documentation: version ---
#define L_DOC_VER_TITRE      "&#8505; Версия"
#define L_DOC_VER_TEXTE      "Солнечный контроллер <b>123f</b> \xe2\x80\x94 ESP32 + VEML7700 + INA226 + MPPT"

// --- Page /upd: weather update ---
#define L_UPD_TITRE          "Обновление погоды..."
#define L_UPD_SOUS_TITRE     "OWM + NASA (~15с)"
#define L_UPD_TRAITEMENT     "Обработка..."

// --- Debug/bilan transition pages ---
#define L_DBG_BIL_FORCE_OK   "&#9654; Архивирование..."
#define L_DBG_BIL_UNDO_OK    "&#8635; Отмена..."
#define L_DBG_BIL_RESET_OK   "&#128465; Сброс..."

// --- SVG: extra chart labels ---
#define L_SVG_JOUR_N         "Д-%d"
#define L_SVG_AUJ            "Сегодня"
#define L_SVG_AUJ_COURT      "сег."
#define L_SVG_APRES_JOURS    "(доступно через несколько дней)"
#define L_CHARGEMENT         "загрузка..."

// --- Menu: weather update ---
#define L_MENU_MAJ_METEO     "&#x27F3; Обновить погоду"

// --- Badge anti-bootloop: JS prefix ---
#define L_BADGE_BL_AVANT     "&#9888; Защита от bootloop активна \xe2\x80\x94 MOSFET ВЫКЛ на "

// --- Debug: last reset labels ---
#define L_DBG_RST_CRASH_LBL  "&#9888; ПОСЛЕДНИЙ СБРОС: "
#define L_DBG_RST_OK_LBL     "&#10003; Последний сброс: "

// --- Documentation: badge labels (inline examples) ---
#define L_DOC_LBL_CHARGE     "НАГРУЗКА ВКЛ/ВЫКЛ"
#define L_DOC_LBL_SEUILON    "Порог ВКЛ"
#define L_DOC_LBL_COUPURE    "Откл."
