// Efecto plasma con paleta de colores para DevLab
// Demuestra animación fluida con 8-bit color

#include <upicodvi.h>

// Configuración de hardware DevLab
static const struct dvi_serialiser_cfg devlab_dvi_cfg = {
  .pio = pio0,
  .sm_tmds = {0, 1, 2},
  .pins_tmds = {14, 12, 8},
  .pins_clk = 10,
  .invert_diffpairs = false
};

DVIGFX8 display(DVI_RES_320x240p60, true, devlab_dvi_cfg);

// Tablas de lookup precalculadas para rendimiento
uint8_t plasma1[256];
uint8_t plasma2[256];

void setup() {
  if (!display.begin()) {
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;) digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }
  
  // Generar paleta de colores (gradiente arcoíris)
  for (int i = 0; i < 256; i++) {
    float pos = i / 255.0;
    uint8_t r, g, b;
    
    if (pos < 0.25) {
      r = 0;
      g = pos * 4 * 255;
      b = 255;
    } else if (pos < 0.5) {
      r = 0;
      g = 255;
      b = (0.5 - pos) * 4 * 255;
    } else if (pos < 0.75) {
      r = (pos - 0.5) * 4 * 255;
      g = 255;
      b = 0;
    } else {
      r = 255;
      g = (1.0 - pos) * 4 * 255;
      b = 0;
    }
    
    display.setColor(i, r, g, b);
  }
  
  // Precalcular tablas de plasma
  for (int i = 0; i < 256; i++) {
    plasma1[i] = (uint8_t)(128.0 + 127.0 * sin(i * 3.14159 / 32.0));
    plasma2[i] = (uint8_t)(128.0 + 127.0 * cos(i * 3.14159 / 64.0));
  }
}

void loop() {
  static uint8_t time1 = 0, time2 = 0, time3 = 0, time4 = 0;
  
  // Generar efecto plasma
  for (int y = 0; y < display.height(); y++) {
    for (int x = 0; x < display.width(); x++) {
      uint8_t color = (
        plasma1[(x + time1) & 255] +
        plasma2[(y + time2) & 255] +
        plasma1[((x + y) + time3) & 255] +
        plasma2[((x - y) + time4) & 255]
      ) >> 2;
      
      display.drawPixel(x, y, color);
    }
  }
  
  // Información en pantalla
  display.setTextSize(2);
  display.setCursor(80, 10);
  display.setTextColor(255, 0);
  display.print("DevLab");
  
  display.setTextSize(1);
  display.setCursor(95, 30);
  display.print("Plasma");
  
  display.swap();
  
  // Animar
  time1 += 2;
  time2 -= 1;
  time3 += 1;
  time4 -= 2;
}
