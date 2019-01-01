/*
 * Copyright (c) 2014-2018 Cesanta Software Limited
 * Copyright (c) 2019 Manfred Müller-Späth <fms1961@gmail.com>
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the ""License"");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an ""AS IS"" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CS_MOS_LIBS_RGBLEDS_INCLUDE_MGOS_RGBLEDS_H_
#define CS_MOS_LIBS_RGBLEDS_INCLUDE_MGOS_RGBLEDS_H_

#include <stdbool.h>
#include <stdint.h>
#include "common/cs_dbg.h"
#include "mgos_bitbang.h"
#include "mgos_gpio.h"
#include "mgos_system.h"
#include "mgos_timers.h"
#define NUM_CHANNELS 3 /* r, g, b */

/*
 * Pixel order: RGB, GRB, or BGR.
 */
enum mgos_rgbleds_order
{
  MGOS_RGBLEDS_ORDER_RGB,
  MGOS_RGBLEDS_ORDER_GRB,
  MGOS_RGBLEDS_ORDER_BGR,
};

enum mgos_rgbleds_type
{
  MGOS_RGBLEDS_TYPE_NEOPIXEL,
  MGOS_RGBLEDS_TYPE_WS2812,
  MGOS_RGBLEDS_TYPE_APA102,
  MGOS_RGBLEDS_TYPE_RGB_PWM,
  MGOS_RGBLEDS_TYPE_RGBW_PWM
};

struct mgos_rgbleds
{
  int pin;
  int num_pixels;
  enum mgos_rgbleds_order order;
  uint8_t *data;
  int counter;
  int col_count;
  uint8_t *color_values;
  uint8_t *color_shadows;
  mgos_timer_id timer_id;
  bool single_mode;
  int shift;
};

#if defined(__cplusplus)
extern "C" {  // Make sure we have C-declarations in C++ programs
#endif

/*
 * Opaque rgbleds instance
 */
//struct mgos_rgbleds;

/*
 * Create and return a RGB LED strip object. Example:
 * ```c
 * struct mgos_rgbleds *mystrip = mgos_rgbleds_create(
 *     5, 16, MGOS_RGBLEDS_ORDER_GRB);
 * mgos_rgbleds_set(mystrip, 0, 12, 34, 56);
 * mgos_rgbleds_show(mystrip);
 *
 * mgos_rgbleds_clear(mystrip);
 * mgos_rgbleds_set(mystrip, 1, 12, 34, 56);
 * mgos_rgbleds_show(mystrip);
 * ```
 */
struct mgos_rgbleds *mgos_rgbleds_create(int pin, int num_pixels,
                                           enum mgos_rgbleds_order order);

/*
 * Set i-th pixel's RGB value. Each color (`r`, `g`, `b`) should be an integer
 * from 0 to 255; they are ints and not `uint8_t`s just for the FFI.
 *
 * Note that this only affects in-memory value of the pixel; you'll need to
 * call `mgos_rgbleds_show()` to apply changes.
 */
void mgos_rgbleds_set(struct mgos_rgbleds *np, int i, int r, int g, int b);

/*
 * Get i-th pixel's RGB value in CSV RGB format. Each color will be a string
 *  of integer from 0 to 255.
 * 
 * The result will be print in the 'out' buffer, the past length has 
 * to be at least 12 chars
 *
 * Note that this only reads in-memory value of the pixel.
 */
void mgos_rgbleds_get(struct mgos_rgbleds *np, int i, char *out, int len);

/*
 * Clear in-memory values of the pixels.
 */
    void mgos_rgbleds_clear(struct mgos_rgbleds *np);

/*
 * Output values of the pixels.
 */
void mgos_rgbleds_show(struct mgos_rgbleds *np);
void mgos_rgbleds_show_fast(struct mgos_rgbleds *np);

void mgos_rgbleds_prep_colors(struct mgos_rgbleds *np, int pix, uint8_t r, uint8_t g, uint8_t b, int col_num);
void mgos_rgbleds_process(void *param);
void mgos_rgbleds_start(struct mgos_rgbleds *np, int timeout);
void mgos_rgbleds_stop(struct mgos_rgbleds *np);

/*
 * Free rgbleds instance.
 */
void mgos_rgbleds_free(struct mgos_rgbleds *np);

#if defined(__cplusplus)
}
#endif

#endif /* CS_MOS_LIBS_RGBLEDS_INCLUDE_MGOS_RGBLEDS_H_ */