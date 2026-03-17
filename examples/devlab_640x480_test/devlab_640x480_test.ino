// Ejemplo 640x480p60 para DevLab
// Usa modo 1-bit monocromo para manejar la resolución más alta

#include <upicodvi.h>

// Configuración DevLab
static const struct dvi_serialiser_cfg devlab_dvi_cfg = {
  .pio = pio0,
  .sm_tmds = {0, 1, 2},
  .pins_tmds = {14, 12, 8},
  .pins_clk = 10,
  .invert_diffpairs = false
};

// Modo 1-bit con doble buffer para 640x480
// Requiere ~76KB de RAM para doble buffer
DVIGFX1 display(DVI_RES_640x480p60, true, devlab_dvi_cfg);

// Variables para la animación
int x = 50, y = 50;
int vx = 2, vy = 2;

void setup() {
  if (!display.begin()) {
    // Si falla (RAM insuficiente), parpadear LED
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;) digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }
  
  display.fillScreen(0); // Fondo negro
  
  // Dibujar marco de borde
  display.drawRect(0, 0, display.width(), display.height(), 1);
  display.drawRect(1, 1, display.width()-2, display.height()-2, 1);
  
  // Título
  display.setTextSize(2);
  display.setTextColor(1);
  display.setCursor(180, 10);
  display.print("DevLab 640x480");
  
  display.setTextSize(1);
  display.setCursor(250, 35);
  display.print("Resolucion VGA");
  
  // Info
  display.setCursor(10, 460);
  display.print("Modo: 1-bit monocromo | RAM: ~76KB | FPS: 60");
  
  display.swap();
  delay(2000); // Mostrar info inicial
}

void loop() {
  // Limpiar pantalla
  display.fillScreen(0);
  
  // Redibujar marco
  display.drawRect(0, 0, display.width(), display.height(), 1);
  
  // Dibujar cuadrado animado
  display.fillRect(x, y, 40, 40, 1);
  
  // Dibujar líneas de perspectiva desde el cuadrado
  display.drawLine(x, y, 0, 0, 1);
  display.drawLine(x+40, y, display.width()-1, 0, 1);
  display.drawLine(x, y+40, 0, display.height()-1, 1);
  display.drawLine(x+40, y+40, display.width()-1, display.height()-1, 1);
  
  // Actualizar posición
  x += vx;
  y += vy;
  
  // Rebotar en bordes
  if (x <= 5 || x >= display.width() - 45) vx = -vx;
  if (y <= 5 || y >= display.height() - 45) vy = -vy;
  
  // Mostrar coordenadas
  display.setTextSize(1);
  display.setCursor(10, 10);
  display.print("X:");
  display.print(x);
  display.print(" Y:");
  display.print(y);
  
  // FPS counter
  static unsigned long lastFrame = 0;
  static int fps = 0;
  unsigned long now = millis();
  if (now - lastFrame > 0) {
    fps = 1000 / (now - lastFrame);
  }
  lastFrame = now;
  
  display.setCursor(display.width() - 60, 10);
  display.print("FPS:");
  display.print(fps);
  
  display.swap();
}
