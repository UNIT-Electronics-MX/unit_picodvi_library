# Ejemplo de Configuración Personalizada de Pines

Este ejemplo demuestra cómo configurar **upicodvi** con un pinout personalizado para tu hardware.

## Tabla de Pines Utilizada

| Pin | Señal   | Tipo          | Uso en DVI              |
|-----|---------|---------------|-------------------------|
| 1   | 3.3V    | Alimentación  | Alimentación del sistema |
| 2   | GPIO2   | Señal         | TMDS2+ (Par de datos 2) |
| 3   | GPIO3   | Señal         | TMDS2- (Par de datos 2) |
| 4   | GND     | Tierra        | Referencia              |
| 11  | GPIO8   | Señal         | TMDS0+ (Par de datos 0) |
| 12  | GPIO9   | Señal         | TMDS0- (Par de datos 0) |
| 13  | GND     | Tierra        | Referencia              |
| 14  | GPIO10  | Señal         | TMDS1+ (Par de datos 1) |
| 15  | GPIO11  | Señal         | TMDS1- (Par de datos 1) |
| 16  | GND     | Tierra        | Referencia              |
| 17  | GPIO12  | Señal         | CLK+ (Par de reloj)     |
| 18  | GPIO13  | Señal         | CLK- (Par de reloj)     |
| 19  | GND     | Tierra        | Referencia              |

Los pines restantes (14, 15, 20, 21, 22) pueden usarse para otras funciones (GPIO adicionales, I2C, etc.)

## Configuración DVI/HDMI

DVI requiere:
- **3 pares diferenciales TMDS** para datos de video (RGB)
- **1 par diferencial TMDS** para señal de reloj
- Cada par usa 2 GPIOs consecutivos

### Estructura de Configuración

```cpp
static const struct dvi_serialiser_cfg custom_hardware_cfg = {
  .pio = pio0,              // PIO a utilizar (pio0 o pio1)
  .sm_tmds = {0, 1, 2},     // State machines del PIO
  .pins_tmds = {8, 10, 2},  // Pines base para cada par TMDS
  .pins_clk = 12,           // Pin base para el par de reloj
  .invert_diffpairs = false // true si necesitas invertir polaridad
};
```

### Explicación de Campos

- **pio**: Bloque PIO del RP2040 a usar (generalmente `pio0`)
- **sm_tmds**: Array de 3 state machines para los 3 canales TMDS
- **pins_tmds**: Pines GPIO base para cada par diferencial de datos
  - El sistema usa automáticamente el pin siguiente (pin+1)
  - Ejemplo: `8` usa GPIO8 y GPIO9
- **pins_clk**: Pin GPIO base para el par diferencial de reloj
  - Usa automáticamente el pin siguiente (pin+1)
- **invert_diffpairs**: Invierte la polaridad de los pares si el hardware lo requiere

## Consideraciones de Hardware

1. **Pines Consecutivos**: Los pares diferenciales deben usar GPIOs consecutivos
2. **Resistencias**: Se requieren resistencias de terminación apropiadas (típicamente 270Ω en serie)
3. **Impedancia**: Las pistas PCB deben tener impedancia diferencial de 100Ω
4. **Tierra**: Buena distribución de pines GND para integridad de señal
5. **Velocidad**: Los pines deben soportar las velocidades de slew rate requeridas

## Resoluciones Disponibles

```cpp
DVI_RES_320x240p60   // 320x240 @ 60Hz (requiere menos overclock)
DVI_RES_400x240p60   // 400x240 @ 60Hz
DVI_RES_640x480p60   // 640x480 @ 60Hz (VGA estándar)
DVI_RES_800x480p60   // 800x480 @ 60Hz (requiere más overclock)
```

## Modos de Color

- **DVIGFX16**: 16-bit color (RGB565), sin doble buffer
- **DVIGFX8**: 8-bit paleta de 256 colores, con opción de doble buffer
- **DVIGFX1**: 1-bit monocromo, con opción de doble buffer
- **DVItext1**: Modo texto optimizado

## Uso

1. Copia la configuración `custom_hardware_cfg` a tu sketch
2. Ajusta los valores de los pines según tu hardware
3. Pasa la configuración al constructor del display:
   ```cpp
   DVIGFX8 display(DVI_RES_320x240p60, true, custom_hardware_cfg);
   ```
4. Llama a `display.begin()` en `setup()`
5. Usa funciones Adafruit_GFX para dibujar

## Troubleshooting

- **No hay salida de video**: Verifica que los pines estén correctamente conectados
- **Pantalla con ruido/líneas**: Ajusta `invert_diffpairs` o revisa las resistencias
- **LED parpadea**: Error de memoria, reduce la resolución o desactiva doble buffer
- **Colores incorrectos**: Verifica el orden de los pares TMDS

## Referencias

- Biblioteca basada en PicoDVI de Luke Wren
- Compatible con Adafruit_GFX para gráficos
- Requiere RP2040 con suficiente RAM para el framebuffer
