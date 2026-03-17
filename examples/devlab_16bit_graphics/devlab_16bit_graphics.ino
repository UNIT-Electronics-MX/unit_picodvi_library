// Ejemplo de gráficos 16-bit para DevLab
// Muestra capacidades de color completo RGB565

#include <upicodvi.h>

// Configuración de hardware DevLab
static const struct dvi_serialiser_cfg devlab_dvi_cfg = {
  .pio = pio0,
  .sm_tmds = {0, 1, 2},
  .pins_tmds = {14, 12, 8},
  .pins_clk = 10,
  .invert_diffpairs = false
};

// Display 16-bit (65536 colores) - Sin doble buffer debido a limitaciones de RAM
DVIGFX16 display(DVI_RES_320x240p60, devlab_dvi_cfg);

void setup() {
  if (!display.begin()) {
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;) digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }
  
  display.fillScreen(0x0000); // Negro
}

uint16_t hue = 0;

void loop() {
  // Limpiar pantalla con gradiente
  for (int y = 0; y < display.height(); y++) {
    uint16_t color = display.color565(
      (y * 255) / display.height(),
      0,
      255 - ((y * 255) / display.height())
    );
    display.drawFastHLine(0, y, display.width(), color);
  }
  
  // Dibujar círculos concéntricos con colores del arcoíris
  int centerX = display.width() / 2;
  int centerY = display.height() / 2;
  
  for (int r = 80; r > 0; r -= 10) {
    uint16_t h = (hue + r * 3) % 360;
    uint16_t color = hsvToRgb565(h, 255, 255);
    display.drawCircle(centerX, centerY, r, color);
  }
  
  // Texto con sombra
  display.setTextSize(3);
  display.setCursor(42, 102);
  display.setTextColor(0x0000); // Sombra negra
  display.print("DevLab");
  display.setCursor(40, 100);
  display.setTextColor(0xFFFF); // Texto blanco
  display.print("DevLab");
  
  display.setTextSize(1);
  display.setCursor(90, 130);
  display.setTextColor(0xFFE0);
  display.print("16-bit RGB565");
  
  hue = (hue + 2) % 360;
  delay(50);
}

// Convertir HSV a RGB565
uint16_t hsvToRgb565(uint16_t h, uint8_t s, uint8_t v) {
  uint8_t r, g, b;
  
  h = h % 360;
  uint8_t region = h / 60;
  uint8_t remainder = (h - (region * 60)) * 6;
  
  uint8_t p = (v * (255 - s)) >> 8;
  uint8_t q = (v * (255 - ((s * remainder) >> 8))) >> 8;
  uint8_t t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;
  
  switch (region) {
    case 0: r = v; g = t; b = p; break;
    case 1: r = q; g = v; b = p; break;
    case 2: r = p; g = v; b = t; break;
    case 3: r = p; g = q; b = v; break;
    case 4: r = t; g = p; b = v; break;
    default: r = v; g = p; b = q; break;
  }
  
  return display.color565(r, g, b);
}
