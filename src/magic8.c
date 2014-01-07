#include <pebble.h>
#include <time.h>

static Window *window;
static TextLayer *text_layer;

/* Possible answers */
static char *answers[20] = {"It is certain",
		     "It is decidedly so",
		     "Without a doubt",
		     "Yes definitely",
		     "You may rely on it",
		     "As I see it yes",
		     "Most likely",
		     "Outlook good",
		     "Yes",
		     "Signs point to yes",
		     "Reply hazy try again",
		     "Ask again later",
		     "Better not tell you now",
		     "Cannot predict now",
		     "Concentrate and ask again",
		     "Don't count on it",
		     "My reply is no",
		     "My sources say no",
		     "Outlook not so good",
		     "Very doubtful"};

/* random() related parts are from https://github.com/ad1217/pebbloids/blob/master/src/Pebbloids.c */
long seed;
static int random(int max){
    seed=(((seed*214013L+2531011L) >> 16) & 32767);
    return ((seed%max));
}

void update_message(void){
	int reading = random(20);
	text_layer_set_text(text_layer, answers[reading]);
}
void accel_tap_handler(AccelAxisType axis, int32_t direction){
  update_message();
}
static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, bounds.size.h } });
  window_set_background_color(window, GColorBlack);
  window_set_fullscreen(window, true);
  text_layer_set_background_color(text_layer, GColorBlack);
  text_layer_set_text_color(text_layer, GColorWhite);
  update_message();
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}
static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

static void init(void) {
	window = window_create();
	window_set_window_handlers(window, (WindowHandlers) {
	    .load = window_load,
	    .unload = window_unload,
	  });
	const bool animated = true;
	window_stack_push(window, animated);

	/* Random seed */
	time_t timval=time(NULL);
	struct tm tims=*(localtime(&timval));
	seed=tims.tm_hour*3600+tims.tm_min*60+tims.tm_sec;
	accel_tap_service_subscribe(&accel_tap_handler);
}
static void deinit(void) {
	accel_tap_service_unsubscribe();
	window_destroy(window);
}

int main(void) {
	init();
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);
	app_event_loop();
	deinit();
}
