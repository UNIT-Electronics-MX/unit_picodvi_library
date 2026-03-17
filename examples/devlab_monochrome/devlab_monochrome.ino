// Gráficos monocromáticos para DevLab
// 320x240 en blanco y negro con doble buffer

#include <upicodvi.h>

// Configuración de hardware DevLab
static const struct dvi_serialiser_cfg devlab_dvi_cfg = {
  .pio = pio0,
  .sm_tmds = {0, 1, 2},
  .pins_tmds = {14, 12, 8},
  .pins_clk = 10,
  .invert_diffpairs = false
};

// Display 1-bit monocromo con doble buffer
DVIGFX1 display(DVI_RES_320x240p60, true, devlab_dvi_cfg);

#define NUM_STARS 200
struct Star {
  int16_t x, y;
  int16_t z;
} stars[NUM_STARS];

void setup() {
  if (!display.begin()) {
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;) digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }
  
  // Inicializar campo estelar
  for (int i = 0; i < NUM_STARS; i++) {
    stars[i].x = random(-160, 160);
    stars[i].y = random(-120, 120);
    stars[i].z = random(1, 256);
  }
}

void loop() {
  // Limpiar pantalla
  display.fillScreen(0);
  
  int centerX = display.width() / 2;
  int centerY = display.height() / 2;
  
  // Dibujar y actualizar estrellas
  for (int i = 0; i < NUM_STARS; i++) {
    // Proyección 3D
    int16_t sx = (stars[i].x * 256) / stars[i].z + centerX;
    int16_t sy = (stars[i].y * 256) / stars[i].z + centerY;
    
    // Dibujar estrella si está en pantalla
    if (sx >= 0 && sx < display.width() && sy >= 0 && sy < display.height()) {
      int size = (256 - stars[i].z) / 128;
      if (size == 0) {
        display.drawPixel(sx, sy, 1);
      } else {
        display.fillCircle(sx, sy, size, 1);
      }
    }
    
    // Mover estrella hacia adelante
    stars[i].z -= 2;
    
    // Reiniciar estrella si sale de la pantalla
    if (stars[i].z <= 0) {
      stars[i].x = random(-160, 160);
      stars[i].y = random(-120, 120);
      stars[i].z = 255;
    }
  }
  
  // Dibujar marco de info
  display.drawRect(0, 0, 150, 30, 1);
  display.fillRect(1, 1, 148, 28, 0);
  
  display.setTextSize(1);
  display.setTextColor(1);
  display.setCursor(5, 5);
  display.print("DevLab Starfield");
  display.setCursor(5, 15);
  display.print("320x240 @ 60Hz");
  
  // Mostrar FPS
  static unsigned long lastFrame = 0;
  static int fps = 0;
  unsigned long now = millis();
  if (now - lastFrame > 0) {
    fps = 1000 / (now - lastFrame);
  }
  lastFrame = now;
  
  display.setCursor(5, 22);
  display.print("FPS: ");
  display.print(fps);
  
  display.swap();
}
