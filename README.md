# <a id="Top">Air Quality Monitor Zigbee</a>

---

**В процессе**

---

<img src="https://raw.githubusercontent.com/slacky1965/air_quality_monitor_zrd/refs/heads/main/doc/images/screen.jpg"/>

<img src="https://raw.githubusercontent.com/slacky1965/air_quality_monitor_zrd/refs/heads/main/doc/images/screenF.jpg"/>

<img src="https://raw.githubusercontent.com/slacky1965/air_quality_monitor_zrd/refs/heads/main/doc/images/screen90.jpg"/>

<img src="https://raw.githubusercontent.com/slacky1965/air_quality_monitor_zrd/refs/heads/main/doc/images/z2m_exposes.jpg"/>

---

[Repository air_quality_monitor_zrd](https://github.com/slacky1965/air_quality_monitor_zrd)

* [Описание](#description)
* [Железо](#hardware)
* [Софт](#software)  
* [Принцип работы](#firmware)
* [Настройка](#settings)

## <a id="description">Описание</a>

- Устройство представлят из себя домашний бытовой монитор (далее просто `Монитор`) со встроенными датчиками качества воздуха и климата, показания которых выводятся на экран `epaper` 4.2" и передаются в сеть `Zigbee`.
- `Монитор` выводит следующие показания внутренних датчиков:
	- углекислого газа `CO2`,
	- летучих оргиничесикх веществ `VOC`,
	- температуры,
	- влажности,
	- атмосферного давления,
	- освещенности.
- `Монитор` имеет возможность отображать показания температуры, влажности и заряда батарии с внешнего датчика напрямую, минуя координатор (прямой биндинг).
- `Монитор` имеет возможность управлять внешним устройством (реле/вентилятор) напрямую, минуя координатор (прямой биндинг) на основании показаний датчиков CO2 и VOC, посылая команды On-Off.
 - `Монитор` может отображать данные в горизонтальном или вертикальном положении, а также менять режим вывода - черное на белом или белое на черном.
- Настраиваемые параметры сохраняет в энергонезависимой памяти модуля `ZTU`.
- Взаимодейстивие с "умными домами" через `zigbee2mqtt`.
- Первоначальная настройка происходит через web-интерфейс `zigbee2mqtt`.
- Подключиться к сети или покинуть сеть `Zigbee` - удерживать кнопку более 5 секунд (светодиод должен погаснуть и начать моргать красным цветом). Время присоединения к сети - полторы минуты (или пока моргает светодиод).
- При одиночном нажатии кнопки модуль высылает отчет по всем показаниям.
- `Монитор` питается от источника постоянного тока напряжением 5 вольт через разъем `USB Type-C` и является роутером.

---

## <a id="hardware">Железо</a>

- В `Мониторе` установлены следующие датчики и модули:
	- SCD40      - высокоточный цифровой датчик углекислого газа (CO2).
	- SGP40      - цифровой датчик летучих органических соединений (VOC) от `Sensirion`.
	- BME280     - комбинированный цифровой датчик влажности, давления и температуры фирмы `Bosch Sensortec`.
	- BH1750     - цифровой датчик освещённости.
	- DS3231     - часы реального времени (допускается установка `DS3107`).
	- WS2812B 	   - адресные светодиоды, которые помогают визуально по цвету определить качество воздуха.
	- ZTU        - модуль Zigbee от фирмы `Tuya`, выполенный на чипе `TLSR82581F32` фирмы `Telink`.
	- WeAct 4.2" - epaper экран от `WeAct Studio`.

---

### Схема

Схема `Монитора`.

<img src="https://raw.githubusercontent.com/slacky1965/air_quality_monitor_zrd/refs/heads/main/doc/images/Schematic_AirQualityMonitor.jpg"/>

---

## <a id="software">Софт</a>

[Последнюю прошивку](https://github.com/slacky1965/air_quality_monitor_zrd/tree/main/bin)/air_quality_monitor_zrd_Vx.x.xx.bin нужно залить в модуль с помощью [github.com/pvvx/TLSRPGM](https://github.com/pvvx/TLSRPGM) или оригинального программатора от Telink.

<img src="https://raw.githubusercontent.com/slacky1965/watermeter_zed/main/doc/images/telink_pgm.jpg" alt="Telink PGM"/>

Как сделать недорогой программатор на базе модулей TB-03 или TB-04 можно почитать [тут](https://slacky1965.github.io/electricity_meter_zrd/#%D0%B7%D0%B0%D0%B3%D1%80%D1%83%D0%B7%D0%BA%D0%B0-%D0%BF%D1%80%D0%BE%D1%88%D0%B8%D0%B2%D0%BA%D0%B8)

Проект сформирован таким образом, что его можно собрать обычным make'ом как под Windows, в оболочке [Git Bash](https://git-scm.com/download/win), а также под Linux'ом.

Как добавить проект в Eclipse можно почитать [тут](https://slacky1965.github.io/electricity_meter_zrd/#%D0%BA%D0%BE%D0%BC%D0%BF%D0%B8%D0%BB%D1%8F%D1%86%D0%B8%D1%8F). Все точно так же, только для другого проекта.

---

## <a id="firmware">Описание работы модуля</a>


## История версий
- 1.0.01
	- Начало.

[Наверх](#Top)

