#include <Arduino.h>
#include <TFT_eSPI.h>
#include "display.h"
#include "imu.h"
#include "ambient.h"
#include "network.h"
#include "sd_card.h"
#include "rgb_led.h"
#include "lv_port_indev.h"
#include "lv_port_fatfs.h"
#include "lv_cubic_gui.h"
#include "gui_guider.h"

/*** Component objects ***/
Display screen;
IMU mpu;
Pixel rgb;
SdCard tf;
Network wifi;
lv_ui guider_ui;
extern TFT_eSPI tft;

int frame_id = 0;
char buf[100];

void setup()
{
    Serial.begin(115200);

    /*** Init screen ***/
    screen.init();
    screen.setBackLight(1.0);

    /*** Init IMU as input device ***/
    lv_port_indev_init();
    mpu.init();
    Serial.printf("welcome\r\n");
    /*** Init on-board RGB ***/
    rgb.init();
    rgb.setBrightness(0.1).setRGB(0, 0, 122, 204).setRGB(1, 0, 122, 204);

    /*** Init micro SD-Card ***/
    tf.init();
    lv_fs_if_init();

    String ssid = tf.readFileLine("/wifi.txt", 1);        // line-1 for WiFi ssid
    String password = tf.readFileLine("/wifi.txt", 2);    // line-2 for WiFi password

    /*** Inflate GUI objects ***/
    //lv_holo_cubic_gui();
    setup_ui(&guider_ui);

    /*** Read WiFi info from SD-Card, then scan & connect WiFi ***/
#if 0
    wifi.init(ssid, password);

    // Change to your BiliBili UID
    Serial.println(wifi.getBilibiliFans("20259914"));
#endif

	tft.fillScreen(TFT_BLACK);
}

void led_rgb_display(void)
{
    static unsigned char r, g, b;

    rgb.setBrightness(0.1).setRGB(0, r, g, b);
    r += 10;
    g += 20;
    b += 30;
}

void lcd_animation_display(void)
{
    #define LCD_WIDTH   240
    #define LCD_HIGH    240
    #define ICON_W      20
    #define ICON_H      20
    #define STEP_X      5
    #define STEP_Y      8

    static uint32_t color = TFT_RED;
    static int32_t mode = 0;
    static int32_t x = 15, y = 0;

    //tft.fillRect(x, y, ICON_W, ICON_H, TFT_BLACK);
    tft.fillCircle(x+ICON_W/2, y+ICON_H/2, ICON_W/2, TFT_BLACK);

    switch (mode) {
        case 0:
            x += STEP_X; 
            y += STEP_Y;
            if (x > LCD_WIDTH - ICON_W) {
                x = LCD_WIDTH - ICON_W;
                mode = 2;
            }

            if (y > LCD_HIGH - ICON_H) {
                y = LCD_HIGH - ICON_H;
                if (mode == 2) {
                    mode = 3;
                } else {
                    mode = 1;
                }
            }
            break;
        case 1:
            x += STEP_X; 
            y -= STEP_Y;
            if (x > LCD_WIDTH - ICON_W) {
                x = LCD_WIDTH - ICON_W;
                mode = 3;
            }

            if (y <= 0) {
                y = 0;
                if (mode == 3) {
                    mode = 2;
                } else {
                    mode = 0;
                }
            }
            break;
        case 2:
            x -= STEP_X; 
            y += STEP_Y;
            if (x <= 0) {
                x = 0;
                mode = 0;
            }

            if (y > LCD_HIGH - ICON_H) {
                y = LCD_HIGH - ICON_H;
                if (mode == 0) {
                    mode = 1;
                } else {
                    mode = 3;
                }
            }
            break;
        case 3:
            x -= STEP_X; 
            y -= STEP_Y;
            if (x <= 0) {
                x = 0;
                mode = 1;
            }

            if (y <= 0) {
                y = 0;
                if (mode == 1) {
                    mode = 0;
                } else {
                    mode = 2;
                }
            }
            break;
        default:
            break;
    }

    //tft.fillRect(x, y, ICON_W, ICON_H, color);
    tft.fillCircle(x+ICON_W/2, y+ICON_H/2, ICON_W/2, color);
}

void loop()
{
#if 0
    // run this as often as possible
    screen.routine();

    // 200 means update IMU data every 200ms
    mpu.update(200);

    int len = sprintf(buf, "S:/Scenes/Holo3D/frame%03d.bin", frame_id++);
    buf[len] = 0;
    lv_img_set_src(guider_ui.scenes_canvas, buf);
    Serial.println(buf);

    if (frame_id == 7) frame_id = 0;
#endif /* if 0. 2021-5-9 22:21:53 syj0925 */

    //led_rgb_display();

    delay(100);
    lcd_animation_display();
}
