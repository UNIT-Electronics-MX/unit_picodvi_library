// Ejemplo 640x480p60 con colores para DevLab
// Test de verificación con barras de color y patrones

#include <upicodvi.h>

// Configuración DevLab
static const struct dvi_serialiser_cfg devlab_dvi_cfg = {
  .pio = pio0,
  .sm_tmds = {0, 1, 2},
  .pins_tmds = {14, 12, 8},
  .pins_clk = 10,
  .invert_diffpairs = false
};

// Modo 8-bit con doble buffer para 640x480
// Requiere ~307KB de RAM - puede fallar si no hay suficiente memoria
DVIGFX8 display(DVI_RES_640x480p60, true, devlab_dvi_cfg);

uint8_t testMode = 0;
unsigned long lastChange = 0;

void setup() {
  if (!display.begin()) {
    // Si falla (RAM insuficiente), parpadear LED
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;) digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }
  
  // Configurar paleta de colores
  display.setColor(0, 0x0000);   // Negro
  display.setColor(1, 0xF800);   // Rojo
  display.setColor(2, 0x07E0);   // Verde
  display.setColor(3, 0x001F);   // Azul
  display.setColor(4, 0xFFE0);   // Amarillo
  display.setColor(5, 0xF81F);   // Magenta
  display.setColor(6, 0x07FF);   // Cyan
  display.setColor(7, 0xFFFF);   // Blanco
  display.setColor(8, 0x7800);   // Rojo oscuro
  display.setColor(9, 0x03E0);   // Verde oscuro
  display.setColor(10, 0x000F);  // Azul oscuro
  display.setColor(11, 0x7BEF);  // Gris claro
  display.setColor(12, 0x39E7);  // Gris medio
  display.setColor(13, 0x18C3);  // Gris oscuro
}

void drawColorBars() {
  display.fillScreen(0);
  
  int barWidth = display.width() / 8;
  
  // Barras de color superior
  for (int i = 0; i < 8; i++) {
    display.fillRect(i * barWidth, 0, barWidth, display.height() / 3, i);
  }
  
  // Barras de grises medio
  for (int i = 0; i < 8; i++) {
    display.fillRect(i * barWidth, display.height() / 3, barWidth, display.height() / 3, 8 + (i % 6));
  }
  
  // Degradado inferior
  for (int x = 0; x < display.width(); x++) {
    uint8_t color = (x * 14) / display.width();
    display.drawFastVLine(x, 2 * display.height() / 3, display.height() / 3, color);
  }
  
  // Texto
  display.setTextColor(7);
  display.setTextSize(2);
  display.setCursor(200, 10);
  display.print("640x480 COLOR TEST");
  
  display.setTextSize(1);
  display.setCursor(10, display.height() - 20);
  display.print("R G B Y M C W - Barras primarias");
}

void drawCheckboard() {
  display.fillScreen(0);
  
  int size = 40;
  for (int y = 0; y < display.height(); y += size) {
    for (int x = 0; x < display.width(); x += size) {
      uint8_t color = ((x/size) + (y/size)) % 2 ? 7 : 0;
      display.fillRect(x, y, size, size, color);
    }
  }
  
  display.fillRect(0, 0, display.width(), 30, 0);
  display.setTextColor(7);
  display.setTextSize(2);
  display.setCursor(210, 8);
  display.print("CHECKERBOARD");
}

void drawColorGrid() {
  display.fillScreen(0);
  
  int cellW = display.width() / 14;
  int cellH = display.height() / 10;
  
  for (int y = 0; y < 10; y++) {
    for (int x = 0; x < 14; x++) {
      uint8_t color = (x + y * 14) % 14;
      display.fillRect(x * cellW, y * cellH, cellW-2, cellH-2, color);
    }
  }
  
  display.fillRect(0, 0, display.width(), 25, 0);
  display.setTextColor(7);
  display.setTextSize(2);
  display.setCursor(220, 5);
  display.print("COLOR GRID");
}

void drawGradients() {
  display.fillScreen(0);
  
  int h = display.height() / 3;
  
  // Gradiente rojo
  for (int x = 0; x < display.width(); x++) {
    uint16_t val = (x * 31) / display.width();
    display.setColor(14, (val << 11));
    display.drawFastVLine(x, 0, h, 14);
  }
  
  // Gradiente verde
  for (int x = 0; x < display.width(); x++) {
    uint16_t val = (x * 63) / display.width();
    display.setColor(14, (val << 5));
    display.drawFastVLine(x, h, h, 14);
  }
  
  // Gradiente azul
  for (int x = 0; x < display.width(); x++) {
    uint16_t val = (x * 31) / display.width();
    display.setColor(14, val);
    display.drawFastVLine(x, 2*h, h, 14);
  }
  
  display.setTextColor(7);
  display.setTextSize(1);
  display.setCursor(10, 10);
  display.print("ROJO");
  display.setCursor(10, h + 10);
  display.print("VERDE");
  display.setCursor(10, 2*h + 10);
  display.print("AZUL");
  
  display.setTextSize(2);
  display.setCursor(240, display.height()/2 - 8);
  display.print("GRADIENTES RGB");
}

void loop() {
  if (millis() - lastChange > 3000) {
    testMode = (testMode + 1) % 4;
    lastChange = millis();
    
    switch(testMode) {
      case 0: drawColorBars(); break;
      case 1: drawCheckboard(); break;
      case 2: drawColorGrid(); break;
      case 3: drawGradients(); break;
    }
    
    display.swap();
  }
}
