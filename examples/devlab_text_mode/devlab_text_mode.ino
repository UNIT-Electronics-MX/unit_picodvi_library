// Modo texto optimizado para DevLab
// Display monocromático simple con texto

#include <upicodvi.h>

// Configuración de hardware DevLab
static const struct dvi_serialiser_cfg devlab_dvi_cfg = {
  .pio = pio0,
  .sm_tmds = {0, 1, 2},
  .pins_tmds = {14, 12, 8},
  .pins_clk = 10,
  .invert_diffpairs = false
};

// Usar DVIGFX1 para texto en lugar de DVItext1
// Esto da más control y funciona mejor con 320x240
DVIGFX1 display(DVI_RES_320x240p60, true, devlab_dvi_cfg);

int lineCount = 0;
int yPos = 0;

void setup() {
  if (!display.begin()) {
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;) digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }
  
  // Configurar colores y limpiar pantalla
  display.fillScreen(0); // Negro
  display.setTextColor(1); // Blanco
  display.setTextSize(1);
  yPos = 0;
  
  // Encabezado
  printLine("========================================");
  printLine("      DevLab DVI Terminal v1.0         ");
  printLine("========================================");
  printLine("");
  printLine("Sistema inicializado.");
  printLine("Hardware:");
  printLine("  - PIO:   pio0");
  printLine("  - TMDS:  8, 12, 14");
  printLine("  - CLK:   10");
  printLine("  - Res:   320x240 @ 60Hz");
  printLine("");
  printLine("RAM: 256 KB");
  printLine("CPU: RP2040 Cortex-M0+");
  printLine("");
  printLine("========================================");
  printLine("");
  
  display.swap();
}

void loop() {
  static unsigned long lastUpdate = 0;
  
  if (millis() - lastUpdate > 1000) {
    lastUpdate = millis();
    
    // Simular salida de terminal
    char buffer[80];
    sprintf(buffer, "[%06lu] Mensaje #%d", millis() / 1000, lineCount);
    printLine(buffer);
    
    // Cada 5 líneas, mostrar información
    if (lineCount % 5 == 0) {
      sprintf(buffer, "  Temp: %d C | Uptime: %lu s", 
              random(35, 50), millis() / 1000);
      printLine(buffer);
    }
    
    lineCount++;
    
    // Reiniciar después de llenar la pantalla
    if (yPos > display.height() - 16) {
      display.fillScreen(0);
      yPos = 0;
      printLine("========================================");
      printLine("       [PANTALLA REINICIADA]            ");
      printLine("========================================");
      printLine("");
      lineCount = 0;
    }
    
    display.swap();
  }
}

void printLine(const char* text) {
  display.setCursor(0, yPos);
  display.print(text);
  yPos += 8; // Avanzar 8 pixels (altura de texto size 1)
}
