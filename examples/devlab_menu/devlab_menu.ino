// Sistema de menú interactivo para DevLab
// Demuestra interfaz de usuario con gráficos 8-bit

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

// Definición de colores en la paleta
#define COLOR_BG       0
#define COLOR_TEXT     1
#define COLOR_SELECTED 2
#define COLOR_BORDER   3
#define COLOR_TITLE    4
#define COLOR_SHADOW   5

const char* menuItems[] = {
  "1. Configuracion",
  "2. Diagnosticos",
  "3. Informacion",
  "4. Pruebas de Video",
  "5. GPIO Monitor",
  "6. Acerca de"
};

const int numItems = 6;
int selectedItem = 0;

void setup() {
  if (!display.begin()) {
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;) digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }
  
  // Configurar paleta de colores
  display.setColor(COLOR_BG,       20,  20,  40);  // Azul oscuro
  display.setColor(COLOR_TEXT,     220, 220, 220); // Blanco
  display.setColor(COLOR_SELECTED, 255, 255,   0); // Amarillo
  display.setColor(COLOR_BORDER,   100, 100, 200); // Azul claro
  display.setColor(COLOR_TITLE,    100, 200, 255); // Cyan
  display.setColor(COLOR_SHADOW,   10,  10,  20);  // Negro azulado
  
  drawMenu();
}

void loop() {
  // Simular navegación de menú cada 3 segundos
  static unsigned long lastChange = 0;
  if (millis() - lastChange > 3000) {
    lastChange = millis();
    selectedItem = (selectedItem + 1) % numItems;
    drawMenu(); // Redibujar menú completo
  }
  
  // Sin delay - el menú es estático hasta el próximo cambio
}

void drawMenu() {
  // Fondo
  display.fillScreen(COLOR_BG);
  
  // Título con sombra
  display.fillRect(0, 0, 320, 40, COLOR_BORDER);
  display.setTextSize(2);
  display.setTextColor(COLOR_SHADOW);
  display.setCursor(52, 13);
  display.print("DevLab System");
  display.setTextColor(COLOR_TITLE);
  display.setCursor(50, 11);
  display.print("DevLab System");
  
  // Marco del menú
  display.drawRect(20, 50, 280, 170, COLOR_BORDER);
  display.drawRect(21, 51, 278, 168, COLOR_BORDER);
  
  // Items del menú
  display.setTextSize(1);
  for (int i = 0; i < numItems; i++) {
    int y = 60 + (i * 25);
    
    // Highlight del item seleccionado
    if (i == selectedItem) {
      display.fillRect(25, y - 2, 270, 20, COLOR_SELECTED);
      display.setTextColor(COLOR_BG);
    } else {
      display.setTextColor(COLOR_TEXT);
    }
    
    display.setCursor(30, y + 4);
    display.print(menuItems[i]);
  }
  
  // Barra de estado inferior
  display.fillRect(0, 225, 320, 15, COLOR_BORDER);
  display.setTextSize(1);
  display.setTextColor(COLOR_TEXT);
  display.setCursor(5, 228);
  display.print("FW: v1.0");
  display.setCursor(250, 228);
  
  unsigned long seconds = millis() / 1000;
  char timeStr[12];
  sprintf(timeStr, "%02lu:%02lu:%02lu", seconds / 3600, (seconds / 60) % 60, seconds % 60);
  display.print(timeStr);
  
  // Dibujar cursor fijo (sin parpadeo)
  int y = 60 + (selectedItem * 25);
  display.fillTriangle(280, y + 4, 280, y + 14, 290, y + 9, COLOR_SELECTED);
  
  display.swap();
  delay(100); // Pequeña pausa después del swap inicial
}
