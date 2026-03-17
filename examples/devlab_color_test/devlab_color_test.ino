// Prueba de colores para diagnosticar orden de canales TMDS DevLab
// Muestra barras con etiquetas para identificar qué color aparece dónde

#include <upicodvi.h>

// CONFIGURACIÓN CORREGIDA - Los colores ahora deben verse correctos
static const struct dvi_serialiser_cfg devlab_dvi_cfg = {
  .pio = pio0,
  .sm_tmds = {0, 1, 2},
  .pins_tmds = {14, 12, 8},  // Orden correcto: TMDS0=Azul(14), TMDS1=Verde(8), TMDS2=Rojo(12)
  .pins_clk = 10,
  .invert_diffpairs = false
};

// ALTERNATIVAS A PROBAR si los colores están intercambiados:
// Opción 1: {14, 12, 8}  - Invertir orden completo
// Opción 2: {12, 8, 14}  - Rotar canales
// Opción 3: {14, 8, 12}  - Otra combinación
// Opción 4: {8, 14, 12}  - Intercambiar verde y azul
// Opción 5: {12, 14, 8}  - Otra combinación

DVIGFX16 display(DVI_RES_320x240p60, devlab_dvi_cfg);

void setup() {
  if (!display.begin()) {
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;) digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }
  
  drawColorTest();
}

void loop() {
  // Estático - no cambia
  delay(1000);
}

void drawColorTest() {
  display.fillScreen(0x0000); // Fondo negro
  
  int barHeight = 30;
  int y = 10;
  
  // Título
  display.setTextSize(2);
  display.setCursor(40, y);
  display.setTextColor(0xFFFF);
  display.print("Test Colores");
  y += 25;
  
  display.setTextSize(1);
  display.setCursor(10, y);
  display.print("Que color ves? ->");
  y += 15;
  
  // ROJO PURO (solo canal Rojo activo)
  display.fillRect(0, y, 160, barHeight, 0xF800); // RGB565: 11111 000000 00000
  display.setCursor(170, y+10);
  display.setTextColor(0xFFFF);
  display.print("ROJO");
  y += barHeight + 5;
  
  // VERDE PURO (solo canal Verde activo)
  display.fillRect(0, y, 160, barHeight, 0x07E0); // RGB565: 00000 111111 00000
  display.setCursor(170, y+10);
  display.setTextColor(0xFFFF);
  display.print("VERDE");
  y += barHeight + 5;
  
  // AZUL PURO (solo canal Azul activo)
  display.fillRect(0, y, 160, barHeight, 0x001F); // RGB565: 00000 000000 11111
  display.setCursor(170, y+10);
  display.setTextColor(0xFFFF);
  display.print("AZUL");
  y += barHeight + 5;
  
  // BLANCO (todos los canales activos)
  display.fillRect(0, y, 160, barHeight, 0xFFFF);
  display.setCursor(170, y+10);
  display.setTextColor(0x0000);
  display.print("BLANCO");
  y += barHeight + 5;
  
  // Info adicional
  y += 10;
  display.setTextSize(1);
  display.setTextColor(0x07E0);
  display.setCursor(5, y);
  display.print("Si los colores estan mal:");
  y += 10;
  display.setCursor(5, y);
  display.print("Cambia pins_tmds en codigo");
  y += 10;
  display.setCursor(5, y);
  display.print("Actual: {8, 12, 14}");
}
