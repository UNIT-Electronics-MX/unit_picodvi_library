// Pantalla de prueba DevLab para diagnosticar problemas de señal DVI
// Muestra patrones de color para verificar todos los canales TMDS

#include <upicodvi.h>

// Configuración DevLab - CORRECTA y VERIFICADA
static const struct dvi_serialiser_cfg devlab_dvi_cfg = {
  .pio = pio0,
  .sm_tmds = {0, 1, 2},
  .pins_tmds = {14, 12, 8},  // TMDS0=GPIO14, TMDS1=GPIO12, TMDS2=GPIO8
  .pins_clk = 10,
  .invert_diffpairs = false
};

DVIGFX16 display(DVI_RES_320x240p60, devlab_dvi_cfg);

void setup() {
  if (!display.begin()) {
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;) digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }
  
  // Pantalla de prueba con barras de colores
  drawTestPattern();
}

void loop() {
  // Ciclar entre diferentes patrones cada 3 segundos
  static unsigned long lastChange = 0;
  static int pattern = 0;
  
  if (millis() - lastChange > 3000) {
    lastChange = millis();
    pattern = (pattern + 1) % 5;
    
    switch(pattern) {
      case 0: drawTestPattern(); break;
      case 1: drawColorBars(); break;
      case 2: drawCheckerboard(); break;
      case 3: drawGradients(); break;
      case 4: drawFullScreen(); break;
    }
  }
}

void drawTestPattern() {
  display.fillScreen(0x0000); // Negro
  
  // Barras verticales de colores primarios
  int w = display.width() / 6;
  display.fillRect(0*w, 0, w, display.height(), 0xF800); // Rojo
  display.fillRect(1*w, 0, w, display.height(), 0x07E0); // Verde
  display.fillRect(2*w, 0, w, display.height(), 0x001F); // Azul
  display.fillRect(3*w, 0, w, display.height(), 0xFFE0); // Amarillo
  display.fillRect(4*w, 0, w, display.height(), 0xF81F); // Magenta
  display.fillRect(5*w, 0, w, display.height(), 0x07FF); // Cyan
  
  // Texto de info
  display.setTextSize(2);
  display.setCursor(10, 10);
  display.setTextColor(0xFFFF, 0x0000);
  display.print("DevLab Test");
  
  display.setTextSize(1);
  display.setCursor(10, 30);
  display.print("invert_diffpairs: true");
  display.setCursor(10, 40);
  display.print("pins: 8,12,14 clk:10");
}

void drawColorBars() {
  // Barras horizontales
  int h = display.height() / 8;
  display.fillRect(0, 0*h, display.width(), h, 0xFFFF); // Blanco
  display.fillRect(0, 1*h, display.width(), h, 0xFFE0); // Amarillo
  display.fillRect(0, 2*h, display.width(), h, 0x07FF); // Cyan
  display.fillRect(0, 3*h, display.width(), h, 0x07E0); // Verde
  display.fillRect(0, 4*h, display.width(), h, 0xF81F); // Magenta
  display.fillRect(0, 5*h, display.width(), h, 0xF800); // Rojo
  display.fillRect(0, 6*h, display.width(), h, 0x001F); // Azul
  display.fillRect(0, 7*h, display.width(), h, 0x0000); // Negro
  
  display.setTextSize(1);
  display.setCursor(5, 5);
  display.setTextColor(0x0000);
  display.print("Barras Horizontales");
}

void drawCheckerboard() {
  // Patrón de tablero de ajedrez
  display.fillScreen(0x0000);
  int size = 20;
  for (int y = 0; y < display.height(); y += size) {
    for (int x = 0; x < display.width(); x += size) {
      if (((x/size) + (y/size)) % 2 == 0) {
        display.fillRect(x, y, size, size, 0xFFFF);
      }
    }
  }
  
  display.setTextSize(1);
  display.setCursor(5, 5);
  display.setTextColor(0xF800, 0xFFFF);
  display.print("Tablero Ajedrez");
}

void drawGradients() {
  // Gradientes RGB
  for (int x = 0; x < display.width(); x++) {
    uint8_t r = (x * 255) / display.width();
    uint16_t color = display.color565(r, 0, 0);
    display.drawFastVLine(x, 0, 80, color);
  }
  
  for (int x = 0; x < display.width(); x++) {
    uint8_t g = (x * 255) / display.width();
    uint16_t color = display.color565(0, g, 0);
    display.drawFastVLine(x, 80, 80, color);
  }
  
  for (int x = 0; x < display.width(); x++) {
    uint8_t b = (x * 255) / display.width();
    uint16_t color = display.color565(0, 0, b);
    display.drawFastVLine(x, 160, 80, color);
  }
  
  display.setTextSize(1);
  display.setCursor(5, 5);
  display.setTextColor(0xFFFF);
  display.print("Gradientes RGB");
}

void drawFullScreen() {
  // Pantalla completa de un color sólido
  static int colorIndex = 0;
  uint16_t colors[] = {0xFFFF, 0xF800, 0x07E0, 0x001F, 0x0000};
  const char* names[] = {"Blanco", "Rojo", "Verde", "Azul", "Negro"};
  
  display.fillScreen(colors[colorIndex % 5]);
  
  display.setTextSize(3);
  display.setCursor(60, 100);
  display.setTextColor(~colors[colorIndex % 5]); // Color invertido
  display.print(names[colorIndex % 5]);
  
  colorIndex++;
}
