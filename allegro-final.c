// [main.c]
// this template is provided for the 2D shooter game.

#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <math.h>


// If defined, logs will be shown on console and written to file.
// If commented out, logs will not be shown nor be saved.
#define LOG_ENABLED

/* Constants. */

// Frame rate (frame per second)
const int FPS = 60;
// Display (screen) width.
const int SCREEN_W = 800;
// Display (screen) height.
const int SCREEN_H = 600;
// At most 4 audios can be played at a time.
const int RESERVE_SAMPLES = 4;
// Same as:
// const int SCENE_MENU = 1;
// const int SCENE_START = 2;
enum {
	SCENE_MENU = 1,
	SCENE_START = 2,
	// [HACKATHON 3-7]
	// TODO: Declare a new scene id.
	SCENE_SETTINGS = 3,
	SCENE_OVER=4,
	SCENE_BOSS=5
};

/* Input states */

// The active scene id.
int active_scene;
// Keyboard state, whether the key is down or not.
bool key_state[ALLEGRO_KEY_MAX];
// Mouse state, whether the key is down or not.
// 1 is for left, 2 is for right, 3 is for middle.
bool *mouse_state;
// Mouse position.
int mouse_x, mouse_y;
// TODO: More variables to store input states such as joysticks, ...
int shootenemies;
int number;
int choosescene;
int chooseplane;
float planelife=9;
double start;
int sound=1;
int bgm=1;
int mode;

/* Variables for allegro basic routines. */

ALLEGRO_DISPLAY* game_display;
ALLEGRO_EVENT_QUEUE* game_event_queue;
ALLEGRO_TIMER* game_update_timer;

/* Shared resources*/

ALLEGRO_FONT* font_pirulen_32;
ALLEGRO_FONT* font_pirulen_24;
ALLEGRO_FONT* font_pirulen_60;
ALLEGRO_FONT* font_pirulen_72;
// TODO: More shared resources or data that needed to be accessed
// across different scenes.

/* Menu Scene resources*/
ALLEGRO_BITMAP* main_img_background;
// [HACKATHON 3-1]
// TODO: Declare 2 variables for storing settings images.
// Uncomment and fill in the code below.
ALLEGRO_BITMAP* img_settings;
ALLEGRO_BITMAP* img_settings2;
ALLEGRO_SAMPLE* main_bgm;
ALLEGRO_SAMPLE_ID main_bgm_id;
ALLEGRO_SAMPLE* boss_bgm;
ALLEGRO_SAMPLE_ID boss_bgm_id;
ALLEGRO_SAMPLE* win;
ALLEGRO_SAMPLE_ID win_id;
ALLEGRO_SAMPLE* lose;
ALLEGRO_SAMPLE_ID lose_id;
ALLEGRO_SAMPLE* shoot1;
ALLEGRO_SAMPLE_ID shoot1_id;
ALLEGRO_SAMPLE* shoot2;
ALLEGRO_SAMPLE_ID shoot2_id;
ALLEGRO_SAMPLE* shoot3;
ALLEGRO_SAMPLE_ID shoot3_id;
ALLEGRO_SAMPLE* boom;
ALLEGRO_SAMPLE_ID boom_id;
ALLEGRO_SAMPLE* bossboom;
ALLEGRO_SAMPLE_ID bossboom_id;

/* Start Scene resources*/
ALLEGRO_BITMAP* start_img_background;
ALLEGRO_BITMAP* background1;
ALLEGRO_BITMAP* background2;
ALLEGRO_BITMAP* background3;
ALLEGRO_BITMAP* choose1;
ALLEGRO_BITMAP* choose2;
ALLEGRO_BITMAP* setting_background;
ALLEGRO_BITMAP* start_img_plane;
ALLEGRO_BITMAP* plane1;
ALLEGRO_BITMAP* plane2;
ALLEGRO_BITMAP* plane3;
ALLEGRO_BITMAP* start_img_enemy;
ALLEGRO_SAMPLE* start_bgm;
ALLEGRO_SAMPLE_ID start_bgm_id;
ALLEGRO_SAMPLE* bgm1;
ALLEGRO_SAMPLE_ID bgm1_id;
ALLEGRO_SAMPLE* bgm2;
ALLEGRO_SAMPLE_ID bgm2_id;
ALLEGRO_SAMPLE* bgm3;
ALLEGRO_SAMPLE_ID bgm3_id;
// [HACKATHON 2-1]
// TODO: Declare a variable to store your bullet's image.
// Uncomment and fill in the code below.
ALLEGRO_BITMAP* img_bullet;
ALLEGRO_BITMAP* img_enemiesbullet;
ALLEGRO_BITMAP* img_bossbullet;
ALLEGRO_BITMAP* img_heart[3];
ALLEGRO_BITMAP* img_life[3];
ALLEGRO_BITMAP* img_bosslife[10];
ALLEGRO_BITMAP* img_enemiesheart[3];
ALLEGRO_BITMAP* img_enemieslife[3];
ALLEGRO_BITMAP* img_gameover;
ALLEGRO_BITMAP* img_boss;
ALLEGRO_BITMAP* img_time;
ALLEGRO_BITMAP* img_power1;
ALLEGRO_BITMAP* img_power2;
ALLEGRO_BITMAP* img_power3;
ALLEGRO_BITMAP* img_on;
ALLEGRO_BITMAP* img_off;
ALLEGRO_BITMAP* img_btnon;
ALLEGRO_BITMAP* img_btnoff;
ALLEGRO_BITMAP* img_btnon2;
ALLEGRO_BITMAP* img_btnoff2;
ALLEGRO_BITMAP* img_hearts;

typedef struct {
	// The center coordinate of the image.
	float x, y;
	// The width and height of the object.
	float w, h;
	// The velocity in x, y axes.
	float vx, vy;
	// Should we draw this object on the screen.
	bool hidden;
	// The pointer to the object¡¦s image.
	ALLEGRO_BITMAP* img;
	int life;
    int heart;
    int recall;
    ALLEGRO_SAMPLE* music;
    ALLEGRO_SAMPLE_ID id;
} MovableObject;
void draw_movable_object(MovableObject obj);
#define MAX_ENEMY 3
// [HACKATHON 2-2]
// TODO: Declare the max bullet count that will show on screen.
// You can try max 4 bullets here and see if you needed more.
// Uncomment and fill in the code below.
#define MAX_BULLET 4
MovableObject plane;
MovableObject enemies[MAX_ENEMY];
// [HACKATHON 2-3]
// TODO: Declare an array to store bullets with size of max bullet count.
// Uncomment and fill in the code below.
MovableObject bullets[MAX_BULLET];
MovableObject bossbullets[MAX_BULLET];
MovableObject enemiesbullets0[MAX_BULLET];
MovableObject enemiesbullets1[MAX_BULLET];
MovableObject enemiesbullets2[MAX_BULLET];
MovableObject hearts[MAX_BULLET];
MovableObject boss;

// [HACKATHON 2-4]
// TODO: Set up bullet shooting cool-down variables.
// 1) Declare your shooting cool-down time as constant. (0.2f will be nice)
// 2) Declare your last shoot timestamp.
// Uncomment and fill in the code below.
float MAX_COOLDOWN = 0.5f;
double last_shoot_timestamp;
double last_enemy0shoot_timestamp;
double last_enemy1shoot_timestamp=1;
double last_enemy2shoot_timestamp=2;
double last_bossshoot_timestamp;
double last_hearts_timestamp;
double playtime;

/* Declare function prototypes. */

// Initialize allegro5 library
void allegro5_init(void);
// Initialize variables and resources.
// Allows the game to perform any initialization it needs before
// starting to run.
void game_init(void);
// Process events inside the event queue using an infinity loop.
void game_start_event_loop(void);
// Run game logic such as updating the world, checking for collision,
// switching scenes and so on.
// This is called when the game should update its logic.
void game_update(void);
// Draw to display.
// This is called when the game should draw itself.
void game_draw(void);
// Release resources.
// Free the pointers we allocated.
void game_destroy(void);
// Function to change from one scene to another.
void game_change_scene(int next_scene);
// Load resized bitmap and check if failed.
ALLEGRO_BITMAP *load_bitmap_resized(const char *filename, int w, int h);
// [HACKATHON 3-2]
// TODO: Declare a function.
// Determines whether the point (px, py) is in rect (x, y, w, h).
// Uncomment the code below.
bool pnt_in_rect(int px, int py, int x, int y, int w, int h);

/* Event callbacks. */
void on_key_down(int keycode);
void on_mouse_down(int btn, int x, int y);

/* Declare function prototypes for debugging. */

// Display error message and exit the program, used like 'printf'.
// Write formatted output to stdout and file from the format string.
// If the program crashes unexpectedly, you can inspect "log.txt" for
// further information.
void game_abort(const char* format, ...);
// Log events for later debugging, used like 'printf'.
// Write formatted output to stdout and file from the format string.
// You can inspect "log.txt" for logs in the last run.
void game_log(const char* format, ...);
// Log using va_list.
void game_vlog(const char* format, va_list arg);

int main(int argc, char** argv) {
	// Set random seed for better random outcome.
	srand(time(NULL));
	allegro5_init();
	game_log("Allegro5 initialized");
	game_log("Game begin");
	// Initialize game variables.
	game_init();
	game_log("Game initialized");
	// Draw the first frame.
	game_draw();
	game_log("Game start event loop");
	// This call blocks until the game is finished.
	game_start_event_loop();
	game_log("Game end");
	game_destroy();
	return 0;
}

void allegro5_init(void) {
	if (!al_init())
		game_abort("failed to initialize allegro");

	// Initialize add-ons.
	if (!al_init_primitives_addon())
		game_abort("failed to initialize primitives add-on");
	if (!al_init_font_addon())
		game_abort("failed to initialize font add-on");
	if (!al_init_ttf_addon())
		game_abort("failed to initialize ttf add-on");
	if (!al_init_image_addon())
		game_abort("failed to initialize image add-on");
	if (!al_install_audio())
		game_abort("failed to initialize audio add-on");
	if (!al_init_acodec_addon())
		game_abort("failed to initialize audio codec add-on");
	if (!al_reserve_samples(RESERVE_SAMPLES))
		game_abort("failed to reserve samples");
	if (!al_install_keyboard())
		game_abort("failed to install keyboard");
	if (!al_install_mouse())
		game_abort("failed to install mouse");
	// TODO: Initialize other addons such as video, ...

	// Setup game display.
	game_display = al_create_display(SCREEN_W, SCREEN_H);
	if (!game_display)
		game_abort("failed to create display");
	al_set_window_title(game_display, "I2P(I)_2019 Final Project <student_id>");

	// Setup update timer.
	game_update_timer = al_create_timer(1.0f / FPS);
	if (!game_update_timer)
		game_abort("failed to create timer");

	// Setup event queue.
	game_event_queue = al_create_event_queue();
	if (!game_event_queue)
		game_abort("failed to create event queue");

	// Malloc mouse buttons state according to button counts.
	const unsigned m_buttons = al_get_mouse_num_buttons();
	game_log("There are total %u supported mouse buttons", m_buttons);
	// mouse_state[0] will not be used.
	mouse_state = malloc((m_buttons + 1) * sizeof(bool));
	memset(mouse_state, false, (m_buttons + 1) * sizeof(bool));

	// Register display, timer, keyboard, mouse events to the event queue.
	al_register_event_source(game_event_queue, al_get_display_event_source(game_display));
	al_register_event_source(game_event_queue, al_get_timer_event_source(game_update_timer));
	al_register_event_source(game_event_queue, al_get_keyboard_event_source());
	al_register_event_source(game_event_queue, al_get_mouse_event_source());
	// TODO: Register other event sources such as timer, video, ...

	// Start the timer to update and draw the game.
	al_start_timer(game_update_timer);
}

void game_init(void) {
	/* Shared resources*/
	font_pirulen_32 = al_load_font("pirulen.ttf", 32, 0);
	if (!font_pirulen_32)
		game_abort("failed to load font: pirulen.ttf with size 32");

	font_pirulen_24 = al_load_font("pirulen.ttf", 24, 0);
	if (!font_pirulen_24)
		game_abort("failed to load font: pirulen.ttf with size 24");
	font_pirulen_60 = al_load_font("pirulen.ttf", 60, 0);
	if (!font_pirulen_60)
		game_abort("failed to load font: pirulen.ttf with size 60");
	font_pirulen_72 = al_load_font("pirulen.ttf", 72, 0);
	if (!font_pirulen_72)
		game_abort("failed to load font: pirulen.ttf with size 72");

	/* Menu Scene resources*/
	main_img_background = load_bitmap_resized("menu3.jpg", SCREEN_W, SCREEN_H);
	img_heart[0]=load_bitmap_resized("life1.png", SCREEN_W/5, SCREEN_H/16);
	img_heart[1]=load_bitmap_resized("life2.png", SCREEN_W/5, SCREEN_H/16);
	img_heart[2]=load_bitmap_resized("life3.png", SCREEN_W/5, SCREEN_H/16);
	img_life[0]=load_bitmap_resized("hp1.png", SCREEN_W/5, SCREEN_H/16);
	img_life[1]=load_bitmap_resized("hp2.png", SCREEN_W/5, SCREEN_H/16);
	img_life[2]=load_bitmap_resized("hp3.png", SCREEN_W/5, SCREEN_H/16);
	img_time=load_bitmap_resized("timelimit.png", 35, 50);
	
	img_bosslife[0]=load_bitmap_resized("bossblood-1.png", 200, 40);
	img_bosslife[1]=load_bitmap_resized("bossblood-2.png", 200, 40);
	img_bosslife[2]=load_bitmap_resized("bossblood-3.png", 200, 40);
	img_bosslife[3]=load_bitmap_resized("bossblood-4.png", 200, 40);
	img_bosslife[4]=load_bitmap_resized("bossblood-5.png", 200, 40);
	img_bosslife[5]=load_bitmap_resized("bossblood-6.png", 200, 40);
	img_bosslife[6]=load_bitmap_resized("bossblood-7.png", 200, 40);
	img_bosslife[7]=load_bitmap_resized("bossblood-8.png", 200, 40);
	img_bosslife[8]=load_bitmap_resized("bossblood-9.png", 200, 40);
	img_bosslife[9]=load_bitmap_resized("bossblood-10.png", 200, 40);
	
	img_enemiesheart[0]=load_bitmap_resized("life-1.png", 18, 9);
	img_enemiesheart[1]=load_bitmap_resized("life-2.png", 36, 9);
	img_enemiesheart[2]=load_bitmap_resized("life-3.png", 54, 9);
	img_enemieslife[0]=load_bitmap_resized("blood-1.png", 15, 9);
	img_enemieslife[1]=load_bitmap_resized("blood-1.png", 30, 9);
	img_enemieslife[2]=load_bitmap_resized("blood-1.png", 45, 9);
	img_hearts=load_bitmap_resized("life-1.png", 50, 50);
	img_gameover=load_bitmap_resized("menu4.jpg", SCREEN_W, SCREEN_H);

	main_bgm = al_load_sample("start.ogg");
	if (!main_bgm)
		game_abort("failed to load audio: start.ogg");
	boss_bgm = al_load_sample("HighTension boss.ogg");
	if (!main_bgm)
		game_abort("failed to load audio: HighTension boss.ogg");
	win = al_load_sample("you_win.ogg");
	if (!win)
		game_abort("failed to load audio: you_win.ogg");
	lose = al_load_sample("you_lose.ogg");
	if (!lose)
		game_abort("failed to load audio: you_lose.ogg");
	shoot1 = al_load_sample("shoot1.ogg");
	if (!shoot1)
		game_abort("failed to load audio: shoot1.ogg");
	shoot2 = al_load_sample("shoot2.ogg");
	if (!shoot2)
		game_abort("failed to load audio: shoot2.ogg");
	shoot3 = al_load_sample("shoot3.ogg");
	if (!shoot3)
		game_abort("failed to load audio: shoot3.ogg");
	boom = al_load_sample("boom1.ogg");
	if (!boom)
		game_abort("failed to load audio: boom1.ogg");
	bossboom = al_load_sample("boom2.ogg");
	if (!bossboom)
		game_abort("failed to load audio: boom2.ogg");

	// [HACKATHON 3-4]
	// TODO: Load settings images.
	// Don't forget to check their return values.
	// Uncomment and fill in the code below.
	img_settings = al_load_bitmap("settings.png");
	if (!img_settings)
		game_abort("failed to load image: settings.png");
	img_settings2 = al_load_bitmap("settings2.png");
	if (!img_settings2)
		game_abort("failed to load image: settings2.png");

	/* Start Scene resources*/
	start_img_background = load_bitmap_resized("space2.jpg", SCREEN_W, SCREEN_H);
	setting_background= load_bitmap_resized("setting.jpg", SCREEN_W, SCREEN_H);
	background1 = load_bitmap_resized("space.jpg", 200, 150);
	background2 = load_bitmap_resized("space2.jpg", 200, 150);
	background3 = load_bitmap_resized("space4.jpg", 200, 150);
	plane1 = load_bitmap_resized("spaceShips.png", 102, 88);
	plane2 = load_bitmap_resized("spaceShips2.png", 102, 64);
	plane3 = load_bitmap_resized("spaceShips3.png", 102, 96);
	img_on = load_bitmap_resized("on.png", 68, 25);
	img_off = load_bitmap_resized("off.png", 68, 25);
	img_btnon = load_bitmap_resized("btnon.png", 120, 30);
	img_btnoff = load_bitmap_resized("btnoff.png", 120, 30);
	img_btnon2 = load_bitmap_resized("btnon.png", 160, 30);
	img_btnoff2 = load_bitmap_resized("btnoff.png", 160, 30);
	

    choose1 = al_load_bitmap("choose1.png");
	if (!choose1)
		game_abort("failed to load image: choose1.png");
	choose2 = load_bitmap_resized("choose1.png", 102, 96);
		
	start_img_plane = al_load_bitmap("spaceShips.png");
	if (!start_img_plane)
		game_abort("failed to load image: spaceShips.png");
	
	start_img_enemy = al_load_bitmap("spaceRockets.png");
	if (!start_img_enemy)
		game_abort("failed to load image: spaceRockets.png");
	img_boss = al_load_bitmap("boss.png");
	if (!img_boss)
		game_abort("failed to load image: boss.png");
	img_power1= al_load_bitmap("power1.png");
	if (!img_power1)
		game_abort("failed to load image: power1.png");
	img_power2= al_load_bitmap("power2.png");
	if (!img_power2)
		game_abort("failed to load image: power2.png");
	img_power3= al_load_bitmap("power3.png");
	if (!img_power3)
		game_abort("failed to load image: power3.png");
	
	

	bgm1=start_bgm = al_load_sample("bgm.ogg");
	if (!start_bgm)
		game_abort("failed to load audio: bgm.ogg");
	bgm2 = al_load_sample("EpicSeries.ogg");
	if (!bgm2)
		game_abort("failed to load audio: EpicSeries.ogg");
	bgm3 = al_load_sample("NightRunner.ogg");
	if (!bgm3)
		game_abort("failed to load audio: NightRunner.ogg");

	// [HACKATHON 2-5]
	// TODO: Initialize bullets.
	// 1) Search for a bullet image online and put it in your project.
	//    You can use the image we provided.
	// 2) Load it in by 'al_load_bitmap' or 'load_bitmap_resized'.
	// 3) If you use 'al_load_bitmap', don't forget to check its return value.
	// Uncomment and fill in the code below.
	img_bullet = al_load_bitmap("spaceMissiles_2.png");
	if (!img_bullet)
		game_abort("failed to load image: spaceMissiles_2.png");
	
	img_enemiesbullet = al_load_bitmap("spaceMissiles_1.png");
	if (!img_enemiesbullet)
		game_abort("failed to load image: spaceMissiles_1.png");

    img_bossbullet = al_load_bitmap("bossMissiles.png");
	if (!img_bossbullet)
		game_abort("failed to load image: bossMissiles.png");
	// Change to first scene.
	game_change_scene(SCENE_MENU);
}

void game_start_event_loop(void) {
	bool done = false;
	ALLEGRO_EVENT event;
	int redraws = 0;
	while (!done) {
		al_wait_for_event(game_event_queue, &event);
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			// Event for clicking the window close button.
			game_log("Window close button clicked");
			done = true;
		} else if (event.type == ALLEGRO_EVENT_TIMER) {
			// Event for redrawing the display.
			if (event.timer.source == game_update_timer)
				// The redraw timer has ticked.
				redraws++;
		} else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			// Event for keyboard key down.
	//		game_log("Key with keycode %d down", event.keyboard.keycode);
			key_state[event.keyboard.keycode] = true;
			on_key_down(event.keyboard.keycode);
			if (event.keyboard.keycode == ALLEGRO_KEY_E) done=true;
		} else if (event.type == ALLEGRO_EVENT_KEY_UP) {
			// Event for keyboard key up.
	//		game_log("Key with keycode %d up", event.keyboard.keycode);
			key_state[event.keyboard.keycode] = false;
		} else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			// Event for mouse key down.
	//		game_log("Mouse button %d down at (%d, %d)", event.mouse.button, event.mouse.x, event.mouse.y);
			mouse_state[event.mouse.button] = true;
			on_mouse_down(event.mouse.button, event.mouse.x, event.mouse.y);
		} else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			// Event for mouse key up.
	//		game_log("Mouse button %d up at (%d, %d)", event.mouse.button, event.mouse.x, event.mouse.y);
			mouse_state[event.mouse.button] = false;
		} else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
			if (event.mouse.dx != 0 || event.mouse.dy != 0) {
				// Event for mouse move.
	//			game_log("Mouse move to (%d, %d)", event.mouse.x, event.mouse.y);
				mouse_x = event.mouse.x;
				mouse_y = event.mouse.y;
			} else if (event.mouse.dz != 0) {
				// Event for mouse scroll.
	//			game_log("Mouse scroll at (%d, %d) with delta %d", event.mouse.x, event.mouse.y, event.mouse.dz);
			}
		}
		// TODO: Process more events and call callbacks by adding more
		// entries inside Scene.

		// Redraw
		if (redraws > 0 && al_is_event_queue_empty(game_event_queue)) {
			// if (redraws > 1)
			// 	game_log("%d frame(s) dropped", redraws - 1);
			// Update and draw the next frame.
			game_update();
			game_draw();
			redraws = 0;
		}
	}
}

void game_update(void) {
	if (active_scene == SCENE_START) {
		playtime=al_get_time()-start;
		if(playtime>60){
			game_change_scene(SCENE_OVER);
			return;
		}
		plane.vx = plane.vy = 0;
		if (key_state[ALLEGRO_KEY_UP] || key_state[ALLEGRO_KEY_W])
			plane.vy -= 1;
		if (key_state[ALLEGRO_KEY_DOWN] || key_state[ALLEGRO_KEY_S])
			plane.vy += 1;
		if (key_state[ALLEGRO_KEY_LEFT] || key_state[ALLEGRO_KEY_A])
			plane.vx -= 1;
		if (key_state[ALLEGRO_KEY_RIGHT] || key_state[ALLEGRO_KEY_D])
			plane.vx += 1;
		// 0.71 is (1/sqrt(2)).
		if(chooseplane==2){
			plane.y += plane.vy * 2 * (plane.vx ? 0.71f : 1);
		    plane.x += plane.vx * 2 * (plane.vy ? 0.71f : 1);
		}
		else{
			plane.y += plane.vy * 4 * (plane.vx ? 0.71f : 1);
		    plane.x += plane.vx * 4 * (plane.vy ? 0.71f : 1);
		}
		
		// [HACKATHON 1-1]
		// TODO: Limit the plane's collision box inside the frame.
		//       (x, y axes can be separated.)
		// Uncomment and fill in the code below.
		if (plane.x-plane.w/2 < 0)
			plane.x = plane.w/2;
		else if (plane.x+plane.w/2 > SCREEN_W)
			plane.x = SCREEN_W-plane.w/2;
		if (plane.y-plane.h/2 < 0)
			plane.y = plane.h/2;
		else if (plane.y+plane.h/2 > SCREEN_H)
			plane.y = SCREEN_H-plane.h/2;
		// [HACKATHON 2-7]
		// TODO: Update bullet coordinates.
		// 1) For each bullets, if it's not hidden, update x, y
		// according to vx, vy.
		// 2) If the bullet is out of the screen, hide it.
		// Uncomment and fill in the code below.
		int i,j;
		for (i=0;i<MAX_BULLET;i++) {
			if (bullets[i].hidden)
				continue;
			bullets[i].x += bullets[i].vx;
			bullets[i].y += bullets[i].vy;
			if (bullets[i].y -bullets[i].h/2< 0)
				bullets[i].hidden = true;
			for(j=0;j<MAX_ENEMY;j++){
				if(enemies[j].hidden) continue;
				if(bullets[i].x+bullets[i].w/2>=enemies[j].x-8&&bullets[i].y-bullets[i].h/2<=enemies[j].y+enemies[j].h/2
				&&bullets[i].x+bullets[i].w/2<=enemies[j].x+8&&bullets[i].y-bullets[i].h/2>=enemies[j].y-enemies[j].h/2){
					if(chooseplane==3) enemies[j].life-=2;
					else enemies[j].life--;
					shootenemies++;
					game_log("enemies[%d].life:%d",j,enemies[j].life);
					if(enemies[j].life<=0){
						enemies[j].heart--;
						if(enemies[j].life==-1) enemies[j].life=2;
						else enemies[j].life=3;
						game_log("enemies[%d].heart:%d",j,enemies[j].heart);
					} 
					if(!enemies[j].heart){
						game_log("enemy is hidden");
						enemies[j].hidden=true;
						enemies[j].life=0;
					} 
					bullets[i].hidden=true;
				}
				else if(bullets[i].x-bullets[i].w/2<=enemies[j].x+8&&bullets[i].y-bullets[i].h/2<=enemies[j].y+enemies[j].h/2
				&&bullets[i].x-bullets[i].w/2>=enemies[j].x-8&&bullets[i].y-bullets[i].h/2>=enemies[j].y-enemies[j].h/2){
					if(chooseplane==3) enemies[j].life-=2;
					else enemies[j].life--;
					shootenemies++;
					game_log("enemies[%d].life:%d",j,enemies[j].life);
					if(enemies[j].life<=0){
						enemies[j].heart--;
						if(enemies[j].life==-1) enemies[j].life=2;
						else enemies[j].life=3;
						game_log("enemies[%d].heart:%d",j,enemies[j].heart);
					} 
					if(!enemies[j].heart){
						game_log("enemy is hidden");
						enemies[j].hidden=true;
						enemies[j].life=0;
					} 
					bullets[i].hidden=true;
				}
			}
			if(enemies[0].hidden&&enemies[1].hidden&&enemies[2].hidden&&boss.hidden&&active_scene ==SCENE_START&&!enemies[0].recall&&!enemies[1].recall){
				boss.hidden=false;
				game_change_scene(SCENE_BOSS);
			} 
			
			if(!boss.hidden){
				if(bullets[i].x+bullets[i].w/2>=boss.x-8&&bullets[i].y-bullets[i].h/2<=boss.y+boss.h/2
			    &&bullets[i].x+bullets[i].w/2<=boss.x+8&&bullets[i].y-bullets[i].h/2>=boss.y-boss.h/2){
			    	if(chooseplane==3) boss.life-=2;
				    else boss.life--;
				    shootenemies++;
				    game_log("boss.life:%d",boss.life);
			    	if(boss.life<=0){
				    	boss.heart--;
				    	//if(boss.life==-1) boss.life=2;
				    	//else boss.life=3;
				    	game_log("boss.heart:%d",boss.heart);
			    	} 
			    	else if(boss.life<=3){
			    		if(!enemies[0].recall&&!enemies[1].recall){
			    			game_log("recall");
			    			enemies[0].recall=enemies[1].recall=1;
			    			enemies[0].hidden=enemies[1].hidden=false;
			    			enemies[0].heart=enemies[1].heart=1;
			    			enemies[0].life=enemies[1].life=3;
						}
					}
			    	if(!boss.heart){
			    		game_log("boss is hidden");
				    	boss.hidden=true;
				    	if(enemies[0].hidden&&enemies[1].hidden)
				    	    game_change_scene(SCENE_OVER);
			    	} 
			    	bullets[i].hidden=true;
			    }
			    else if(bullets[i].x-bullets[i].w/2<=boss.x+8&&bullets[i].y-bullets[i].h/2<=boss.y+boss.h/2
			    &&bullets[i].x-bullets[i].w/2>=boss.x-8&&bullets[i].y-bullets[i].h/2>=boss.y-boss.h/2){
			    	if(chooseplane==3) boss.life-=2;
				    else boss.life--;
				    shootenemies++;
				    game_log("boss.life:%d",boss.life);
			    	if(boss.life<=0){
				    	boss.heart--;
				    	//if(boss.life==-1) boss.life=2;
				    	//else boss.life=3;
				    	game_log("boss.heart:%d",boss.heart);
			    	} 
			    	else if(boss.life<=3){
			    		if(!enemies[0].recall&&!enemies[1].recall){
			    			game_log("recall");
			    			enemies[0].recall=enemies[1].recall=1;
			    			enemies[0].hidden=enemies[1].hidden=false;
			    			enemies[0].heart=enemies[1].heart=1;
			    			enemies[0].life=enemies[1].life=3;
						}
					}
				    if(!boss.heart){
					    game_log("boss is hidden");
				    	boss.hidden=true;
				    	if(enemies[0].hidden&&enemies[1].hidden)
				    	    game_change_scene(SCENE_OVER);
				    } 
				    bullets[i].hidden=true;
		    	}
			}
			for (j=0;j<MAX_BULLET;j++){
				if(pnt_in_rect(bullets[i].x, bullets[i].y-bullets[i].h/2, enemiesbullets0[j].x-enemiesbullets0[j].w/2, 
				enemiesbullets0[j].y-enemiesbullets0[j].h/2, enemiesbullets0[j].w, enemiesbullets0[j].h)||
				pnt_in_rect(enemiesbullets0[j].x, enemiesbullets0[j].y+enemiesbullets0[j].h/2, bullets[i].x-bullets[i].w/2, 
				bullets[i].y-bullets[i].h/2, bullets[i].w, bullets[i].h))
				    if(!enemiesbullets0[j].hidden)
				        bullets[i].hidden=enemiesbullets0[j].hidden=true;
				if(pnt_in_rect(bullets[i].x, bullets[i].y-bullets[i].h/2, enemiesbullets1[j].x-enemiesbullets1[j].w/2, 
				enemiesbullets1[j].y-enemiesbullets1[j].h/2, enemiesbullets1[j].w, enemiesbullets1[j].h)||
				pnt_in_rect(enemiesbullets1[j].x, enemiesbullets1[j].y+enemiesbullets1[j].h/2, bullets[i].x-bullets[i].w/2, 
				bullets[i].y-bullets[i].h/2, bullets[i].w, bullets[i].h))
				    if(!enemiesbullets1[j].hidden)
				        bullets[i].hidden=enemiesbullets1[j].hidden=true;
				if(pnt_in_rect(bullets[i].x, bullets[i].y-bullets[i].h/2, enemiesbullets2[j].x-enemiesbullets2[j].w/2, 
				enemiesbullets2[j].y-enemiesbullets2[j].h/2, enemiesbullets2[j].w, enemiesbullets2[j].h)||
				pnt_in_rect(enemiesbullets2[j].x, enemiesbullets2[j].y+enemiesbullets2[j].h/2, bullets[i].x-bullets[i].w/2, 
				bullets[i].y-bullets[i].h/2, bullets[i].w, bullets[i].h))
				    if(!enemiesbullets2[j].hidden)
				        bullets[i].hidden=enemiesbullets2[j].hidden=true;
			}
			
		}
		
		
		for (i=0;i<MAX_BULLET;i++) {
			if(enemies[0].hidden||plane.hidden){
				enemiesbullets0[i].hidden=true;
				continue;
			}
			if (enemiesbullets0[i].hidden)
				continue;
			enemiesbullets0[i].x += enemiesbullets0[i].vx;
			enemiesbullets0[i].y += enemiesbullets0[i].vy;
			if (enemiesbullets0[i].y +enemiesbullets0[i].h/2>= SCREEN_H)
				enemiesbullets0[i].hidden = true;
			if(enemiesbullets0[i].x+enemiesbullets0[i].w/2>=plane.x-20&&enemiesbullets0[i].y+enemiesbullets0[i].h/2>=plane.y-plane.h/2
			&&enemiesbullets0[i].x+enemiesbullets0[i].w/2<=plane.x+20&&enemiesbullets0[i].y+enemiesbullets0[i].h/2<=plane.y+plane.h/2){
				if(sound==1) al_play_sample(boom, 1, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_ONCE, &boom_id);
				if(chooseplane==3){
					plane.life-=2;
					planelife-=2;
				} 
				else{
					plane.life--;
				    planelife--;
				} 
				game_log("plane.life:%d",plane.life);
				if(plane.life<=0){
					plane.heart--;
					if(plane.life==-1) plane.life=2;
					else plane.life=3;
					game_log("plane.heart:%d",plane.heart);
				} 
				if(!plane.heart){
					game_log("plane is hidden");
					plane.hidden=true;
					game_change_scene(SCENE_OVER);
				} 
				enemiesbullets0[i].hidden=true;
			}
			else if(enemiesbullets0[i].x-enemiesbullets0[i].w/2<=plane.x+20&&enemiesbullets0[i].y+enemiesbullets0[i].h/2>=plane.y-plane.h/2
			&&enemiesbullets0[i].x-enemiesbullets0[i].w/2>=plane.x-20&&enemiesbullets0[i].y+enemiesbullets0[i].h/2<=plane.y+plane.h/2){
				if(sound==1) al_play_sample(boom, 1, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_ONCE, &boom_id);
				if(chooseplane==3){
					plane.life-=2;
					planelife-=2;
				} 
				else{
					plane.life--;
				    planelife--;
				} 
				game_log("plane.life:%d",plane.life);
				if(plane.life<=0){
					plane.heart--;
					if(plane.life==-1) plane.life=2;
					else plane.life=3;
					game_log("plane.heart:%d",plane.heart);
				} 
				if(!plane.heart){
					game_log("plane is hidden");
					plane.hidden=true;
					game_change_scene(SCENE_OVER);
				} 
				enemiesbullets0[i].hidden=true;
			}
		}
		for (i=0;i<MAX_BULLET;i++) {
			if(enemies[1].hidden||plane.hidden){
				enemiesbullets1[i].hidden=true;
				continue;
			}
			if (enemiesbullets1[i].hidden)
				continue;
			enemiesbullets1[i].x += enemiesbullets1[i].vx;
			enemiesbullets1[i].y += enemiesbullets1[i].vy;
			if (enemiesbullets1[i].y +enemiesbullets1[i].h/2>= SCREEN_H)
				enemiesbullets1[i].hidden = true;
			if(enemiesbullets1[i].x+enemiesbullets1[i].w/2>=plane.x-20&&enemiesbullets1[i].y+enemiesbullets1[i].h/2>=plane.y-plane.h/2
			&&enemiesbullets1[i].x+enemiesbullets1[i].w/2<=plane.x+20&&enemiesbullets1[i].y+enemiesbullets1[i].h/2<=plane.y+plane.h/2){
				if(sound==1) al_play_sample(boom, 1, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_ONCE, &boom_id);
				if(chooseplane==3){
					plane.life-=2;
					planelife-=2;
				} 
				else{
					plane.life--;
				    planelife--;
				} 
				game_log("plane.life:%d",plane.life);
				if(plane.life<=0){
					plane.heart--;
					if(plane.life==-1) plane.life=2;
					else plane.life=3;
					game_log("plane.heart:%d",plane.heart);
				} 
				if(!plane.heart){
					game_log("plane is hidden");
					plane.hidden=true;
					game_change_scene(SCENE_OVER);
				} 
				enemiesbullets1[i].hidden=true;
			}
			else if(enemiesbullets1[i].x-enemiesbullets1[i].w/2<=plane.x+20&&enemiesbullets1[i].y+enemiesbullets1[i].h/2>=plane.y-plane.h/2
			&&enemiesbullets1[i].x-enemiesbullets1[i].w/2>=plane.x-20&&enemiesbullets1[i].y+enemiesbullets1[i].h/2<=plane.y+plane.h/2){
				if(sound==1) al_play_sample(boom, 1, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_ONCE, &boom_id);
				if(chooseplane==3){
					plane.life-=2;
					planelife-=2;
				} 
				else{
					plane.life--;
				    planelife--;
				} 
				game_log("plane.life:%d",plane.life);
				if(plane.life<=0){
					plane.heart--;
					if(plane.life==-1) plane.life=2;
					else plane.life=3;
					game_log("plane.heart:%d",plane.heart);
				} 
				if(!plane.heart){
					game_log("plane is hidden");
					plane.hidden=true;
					game_change_scene(SCENE_OVER);
				} 
				enemiesbullets1[i].hidden=true;
			}
		}
		for (i=0;i<MAX_BULLET;i++) {
			if(enemies[2].hidden||plane.hidden){
				enemiesbullets2[i].hidden=true;
				continue;
			}
			if (enemiesbullets2[i].hidden)
				continue;
			enemiesbullets2[i].x += enemiesbullets2[i].vx;
			enemiesbullets2[i].y += enemiesbullets2[i].vy;
			if (enemiesbullets2[i].y +enemiesbullets2[i].h/2>= SCREEN_H)
				enemiesbullets2[i].hidden = true;
			if(enemiesbullets2[i].x+enemiesbullets2[i].w/2>=plane.x-20&&enemiesbullets2[i].y+enemiesbullets2[i].h/2>=plane.y-plane.h/2
			&&enemiesbullets2[i].x+enemiesbullets2[i].w/2<=plane.x+20&&enemiesbullets2[i].y+enemiesbullets2[i].h/2<=plane.y+plane.h/2){
				if(sound==1) al_play_sample(boom, 1, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_ONCE, &boom_id);
				if(chooseplane==3){
					plane.life-=2;
					planelife-=2;
				} 
				else{
					plane.life--;
				    planelife--;
				} 
				game_log("plane.life:%d",plane.life);
				if(plane.life<=0){
					plane.heart--;
					if(plane.life==-1) plane.life=2;
					else plane.life=3;
					game_log("plane.heart:%d",plane.heart);
				} 
				if(!plane.heart){
					game_log("plane is hidden");
					plane.hidden=true;
					game_change_scene(SCENE_OVER);
				} 
				enemiesbullets2[i].hidden=true;
			}
			else if(enemiesbullets2[i].x-enemiesbullets2[i].w/2<=plane.x+20&&enemiesbullets2[i].y+enemiesbullets2[i].h/2>=plane.y-plane.h/2
			&&enemiesbullets2[i].x-enemiesbullets2[i].w/2>=plane.x-20&&enemiesbullets2[i].y+enemiesbullets2[i].h/2<=plane.y+plane.h/2){
				if(sound==1) al_play_sample(boom, 1, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_ONCE, &boom_id);
				if(chooseplane==3){
					plane.life-=2;
					planelife-=2;
				} 
				else{
					plane.life--;
				    planelife--;
				} 
				game_log("plane.life:%d",plane.life);
				if(plane.life<=0){
					plane.heart--;
					if(plane.life==-1) plane.life=2;
					else plane.life=3;
					game_log("plane.heart:%d",plane.heart);
				} 
				if(!plane.heart){
					game_log("plane is hidden");
					plane.hidden=true;
					game_change_scene(SCENE_OVER);
				} 
				enemiesbullets2[i].hidden=true;
			}
		}
		
		
		if(!boss.hidden&&plane.heart<2&&mode){
			for (i=0;i<MAX_BULLET;i++) {
			    if(!enemies[2].hidden||plane.hidden){
				    hearts[i].hidden=true;
				    continue;
			    }
			    if (hearts[i].hidden)
				    continue;
			    hearts[i].x += hearts[i].vx;
			    hearts[i].y += hearts[i].vy;
			    if (hearts[i].y +hearts[i].h/2>= SCREEN_H)
			    	hearts[i].hidden = true;
			    if(hearts[i].x+hearts[i].w/2>=plane.x-20&&hearts[i].y+hearts[i].h/2>=plane.y-plane.h/2
			    &&hearts[i].x+hearts[i].w/2<=plane.x+20&&hearts[i].y+hearts[i].h/2<=plane.y+plane.h/2){
				    plane.life++;
				    planelife++;
				    game_log("+plane.life:%d",plane.life);
				    if(plane.life>3){
					    plane.heart++;
					    plane.life=1;
					    game_log("+plane.heart:%d",plane.heart);
				    }  
				    hearts[i].hidden=true;
			    }
			    else if(hearts[i].x-hearts[i].w/2<=plane.x+20&&hearts[i].y+hearts[i].h/2>=plane.y-plane.h/2
			    &&hearts[i].x-hearts[i].w/2>=plane.x-20&&hearts[i].y+hearts[i].h/2<=plane.y+plane.h/2){
				    plane.life++;
				    planelife++;
				    game_log("+plane.life:%d",plane.life);
				    if(plane.life>3){
					    plane.heart++;
					    plane.life=1;
					    game_log("+plane.heart:%d",plane.heart);
				    }  
				    hearts[i].hidden=true;
			    }
		    }
		}
		else for (i=0;i<MAX_BULLET;i++) hearts[i].hidden=true;
		
		
		for (i=0;i<MAX_BULLET;i++) {
			if(boss.hidden||plane.hidden){
				bossbullets[i].hidden=true;
				continue;
			}
			if (bossbullets[i].hidden)
				continue;
			bossbullets[i].x += bossbullets[i].vx;
			bossbullets[i].y += bossbullets[i].vy;
			if (bossbullets[i].y +bossbullets[i].h/2>= SCREEN_H)
				bossbullets[i].hidden = true;
			if(bossbullets[i].x+bossbullets[i].w/2>=plane.x-20&&bossbullets[i].y+bossbullets[i].h/2>=plane.y-plane.h/2
			&&bossbullets[i].x+bossbullets[i].w/2<=plane.x+20&&bossbullets[i].y+bossbullets[i].h/2<=plane.y+plane.h/2){
				if(sound==1) al_play_sample(bossboom, 1, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_ONCE, &bossboom_id);
				if(chooseplane==3){
					plane.life-=3;
				    planelife-=3;
				}
				else{
					plane.life-=2;
				    planelife-=2;
				}
				game_log("plane.life:%d",plane.life);
				if(plane.life<=0){
					plane.heart--;
					if(plane.life==0) plane.life=3;
					else if(plane.life==-1) plane.life=2;
					else plane.life=1;
					game_log("plane.heart:%d",plane.heart);
				}
				if(!plane.heart){
					game_log("plane is hidden");
					plane.hidden=true;
					game_change_scene(SCENE_OVER);
				} 
				bossbullets[i].hidden=true;
			}
			else if(bossbullets[i].x-bossbullets[i].w/2<=plane.x+20&&bossbullets[i].y+bossbullets[i].h/2>=plane.y-plane.h/2
			&&bossbullets[i].x-bossbullets[i].w/2>=plane.x-20&&bossbullets[i].y+bossbullets[i].h/2<=plane.y+plane.h/2){
				if(sound==1) al_play_sample(bossboom, 1, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_ONCE, &bossboom_id);
				if(chooseplane==3){
					plane.life-=3;
				    planelife-=3;
				}
				else{
					plane.life-=2;
				    planelife-=2;
				}
				game_log("plane.life:%d",plane.life);
				if(plane.life<=0){
					plane.heart--;
					if(plane.life==0) plane.life=3;
					else if(plane.life==-1) plane.life=2;
					else plane.life=1;
					game_log("plane.heart:%d",plane.heart);
				}
				if(!plane.heart){
					game_log("plane is hidden");
					plane.hidden=true;
					game_change_scene(SCENE_OVER);
				} 
				bossbullets[i].hidden=true;
			}
			for (j=0;j<MAX_BULLET;j++){
				if(pnt_in_rect(bossbullets[i].x, bossbullets[i].y+bossbullets[i].h/2, bullets[j].x-bullets[j].w/2, bullets[j].y-bullets[j].h/2, bullets[j].w, bullets[j].h))
				    bullets[j].hidden=true;
		    }
		}
		for (i=0;i<MAX_ENEMY;i++) {
			//if (enemies[i].hidden)
			//	continue;
			enemies[i].x += enemies[i].vx;
			if(enemies[i].x+enemies[i].w/2>=SCREEN_W){
				enemies[i].x=SCREEN_W-enemies[i].w/2;
				enemies[i].vx*=-1;
			}
			else if(enemies[i].x-enemies[i].w/2<=0){
				enemies[i].x=0+enemies[i].w/2;
				enemies[i].vx*=-1;
			}
			enemies[i].y += enemies[i].vy;
			if (enemies[i].y +enemies[i].h/2>=SCREEN_H/2){
				enemies[i].y = SCREEN_H/2-enemies[i].h/2;
				enemies[i].vy*=-1;
			}
			else if (enemies[i].y -enemies[i].h/2<=0){
				enemies[i].y = 0+enemies[i].h/2;
				enemies[i].vy*=-1;
			}
			if(pnt_in_rect(enemies[i].x-enemies[i].w/2, enemies[i].y+enemies[i].h/2, plane.x-plane.w/2, plane.y-plane.h/2, plane.w, plane.h)||
				pnt_in_rect(enemies[i].x+enemies[i].w/2, enemies[i].y+enemies[i].h/2, plane.x-plane.w/2, plane.y-plane.h/2, plane.w, plane.h)||
				pnt_in_rect(enemies[i].x-enemies[i].w/2, enemies[i].y-enemies[i].h/2, plane.x-plane.w/2, plane.y-plane.h/2, plane.w, plane.h)||
				pnt_in_rect(enemies[i].x+enemies[i].w/2, enemies[i].y-enemies[i].h/2, plane.x-plane.w/2, plane.y-plane.h/2, plane.w, plane.h)){
					game_log("!!!");
					if(enemies[i].hidden) continue;
					enemies[i].life--;
					plane.life--;
					game_log("enemies[%d].life:%d",i,enemies[i].life);
					game_log("plane.life:%d",plane.life);
					if(!enemies[i].life){
						enemies[i].heart--;
						enemies[i].life=3;
						game_log("enemies[%d].heart:%d",i,enemies[i].heart);
					}
					if(!enemies[i].heart){
						game_log("enemy is hidden");
						enemies[i].hidden=true;
						enemies[i].life=0;
					}
					if(!plane.life){
						plane.heart--;
						plane.life=3;
						game_log("plane.heart:%d",plane.heart);
					}
					if(!plane.heart){
					game_log("plane is hidden");
					plane.hidden=true;
					game_change_scene(SCENE_OVER);
				    }  
				}
		}
		
		boss.x += boss.vx;
		if(boss.x+boss.w/2>=SCREEN_W){
			boss.x=SCREEN_W-boss.w/2;
			boss.vx*=-1;
		}
		else if(boss.x-boss.w/2<=0){
			boss.x=0+boss.w/2;
			boss.vx*=-1;
		}
		boss.y += boss.vy;
		if (boss.y +boss.h/2>=SCREEN_H/2){
			boss.y = SCREEN_H/2-boss.h/2;
			boss.vy*=-1;
		}
		else if (boss.y -boss.h/2<=0){
			boss.y = 0+boss.h/2;
			boss.vy*=-1;
		}

		// [HACKATHON 2-8]
		// TODO: Shoot if key is down and cool-down is over.
		// 1) Get the time now using 'al_get_time'.
		// 2) If Space key is down in 'key_state' and the time
		//    between now and last shoot is not less that cool
		//    down time.
		// 3) Loop through the bullet array and find one that is hidden.
		//    (This part can be optimized.)
		// 4) The bullet will be found if your array is large enough.
		// 5) Set the last shoot time to now.
		// 6) Set hidden to false (recycle the bullet) and set its x, y to the
		//    front part of your plane.
		// Uncomment and fill in the code below.
		double now = al_get_time();
		if(chooseplane==2) MAX_COOLDOWN=0.2f;
		if (key_state[ALLEGRO_KEY_SPACE] && now - last_shoot_timestamp >= MAX_COOLDOWN) {
		    for (i = 0; ;i++) {
		        if (bullets[i].hidden)
		            break;
		    }
		    if (i < MAX_BULLET) {
		        last_shoot_timestamp = now;
		        bullets[i].hidden = false;
		        bullets[i].x = plane.x;
		        bullets[i].y = plane.y-plane.h/2;
	//	        if(sound==1) al_play_sample(bullets[i].music, 1, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_ONCE, &bullets[i].id);
		        number++;
		    }
		}
		if (now - last_enemy0shoot_timestamp >= 0.5&& !enemies[0].hidden) {
		    for (i = 0; ;i++) {
		        if (enemiesbullets0[i].hidden)
		            break;
		    }
		    if (i < MAX_BULLET) {
		        last_enemy0shoot_timestamp = now;
		        enemiesbullets0[i].hidden = false;
		        enemiesbullets0[i].x = enemies[0].x;
		        enemiesbullets0[i].y = enemies[0].y+enemies[0].h/2;
		    }
		}
		if (now - last_enemy1shoot_timestamp >= 0.5&& !enemies[1].hidden) {
		    for (i = 0; ;i++) {
		        if (enemiesbullets1[i].hidden)
		            break;
		    }
		    if (i < MAX_BULLET) {
		        last_enemy1shoot_timestamp = now;
		        enemiesbullets1[i].hidden = false;
		        enemiesbullets1[i].x = enemies[1].x;
		        enemiesbullets1[i].y = enemies[1].y+enemies[1].h/2;
		    }
		}
		if (now - last_enemy2shoot_timestamp >= 0.5&& !enemies[2].hidden) {
		    for (i = 0; ; i++) {
		        if (enemiesbullets2[i].hidden)
		            break;
		    }
		    if (i < MAX_BULLET) {
		        last_enemy2shoot_timestamp = now;
		        enemiesbullets2[i].hidden = false;
		        enemiesbullets2[i].x = enemies[2].x;
		        enemiesbullets2[i].y = enemies[2].y+enemies[2].h/2;
		    }
		}
		if (now - last_bossshoot_timestamp >= 0.3&& !boss.hidden) {
		    for (i = 0; ; i++) {
		        if (bossbullets[i].hidden)
		            break;
		    }
		    if (i < MAX_BULLET) {
		        last_bossshoot_timestamp = now;
		        bossbullets[i].hidden = false;
		        bossbullets[i].x = boss.x;
		        bossbullets[i].y = boss.y+boss.h/2;
		    }
		}
		
		if (now - last_hearts_timestamp >= 3&& enemies[2].hidden&&plane.heart<2&&!boss.hidden&&mode) {
		    for (i = 0; ; i++) {
		        if (hearts[i].hidden)
		            break;
		    }
		    if (i < MAX_BULLET) {
		        last_hearts_timestamp = now;
		        hearts[i].hidden = false;
		        hearts[i].x = enemies[2].x;
		        hearts[i].y = enemies[2].y+enemies[2].h/2;
		    }
		}
	//	else if (now - last_hearts_timestamp >= 3&& enemies[2].hidden&&plane.heart<2&&boss.hidden&&mode) 
	//	    for(i=0;i<MAX_BULLET;i++) hearts[i].hidden=true;
		
		if(enemies[0].hidden&&enemies[1].hidden&&!boss.heart)
			game_change_scene(SCENE_OVER);
	}
}

void game_draw(void) {
	if (active_scene == SCENE_MENU) {
		al_draw_bitmap(main_img_background, 0, 0, 0);
		//al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), SCREEN_W / 2, 30, ALLEGRO_ALIGN_CENTER, "Space Shooter");
		al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W-300, 230, 0, "Press");
		al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W-300, 260, 0, "Enter to start");
		al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W-300, 290, 0, "S to setting");
		al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W-300, 320, 0, "E to exit");
		// [HACKATHON 3-5]
		// TODO: Draw settings images.
		// The settings icon should be located at (x, y, w, h) =
		// (SCREEN_W - 48, 10, 38, 38).
		// Change its image according to your mouse position.
		// Uncomment and fill in the code below.
//		if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W - 48, 10, 38, 38))
//			al_draw_bitmap(img_settings2, SCREEN_W - 48 , 24, 0);
//		else
//			al_draw_bitmap(img_settings,SCREEN_W - 48 , 24, 0);
	}else if (active_scene == SCENE_START) {
		int i;
		al_draw_bitmap(start_img_background, 0, 0, 0);
		al_draw_bitmap(img_time, 0, 40, 0);
		al_draw_textf(font_pirulen_32 , al_map_rgb (255,255,255) , 40 , 40 , 0   , "%d S" , 60-(int)playtime );
	
		// [HACKATHON 2-9]
		// TODO: Draw all bullets in your bullet array.
		// Uncomment and fill in the code below.
		for (i=0;i<MAX_BULLET;i++){
			draw_movable_object(bullets[i]);
			if(!bullets[i].hidden&&bullets[i].x==plane.x&&bullets[i].y==plane.y-plane.h/2&&key_state[ALLEGRO_KEY_SPACE]){
				if(bullets[i].music)
		            if(sound==1) al_play_sample(bullets[i].music, 1, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_ONCE, &bullets[i].id);
	            
			}
			
		}
		for (i=0;i<MAX_BULLET;i++){
			draw_movable_object(enemiesbullets0[i]);
			if(!enemiesbullets0[i].hidden&&enemiesbullets0[i].x==enemies[0].x&&enemiesbullets0[i].y==enemies[0].y+enemies[0].h/2)
			    if(sound==1) al_play_sample(enemiesbullets0[i].music, 0.3, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_ONCE, &enemiesbullets0[i].id);
		}
		for (i=0;i<MAX_BULLET;i++){
			draw_movable_object(enemiesbullets1[i]);
			if(!enemiesbullets1[i].hidden&&enemiesbullets1[i].x==enemies[1].x&&enemiesbullets1[i].y==enemies[1].y+enemies[1].h/2)
			    if(sound==1) al_play_sample(enemiesbullets1[i].music, 0.3, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_ONCE, &enemiesbullets1[i].id);
		}
		for (i=0;i<MAX_BULLET;i++){
			draw_movable_object(enemiesbullets2[i]);
		    if(!enemiesbullets2[i].hidden&&enemiesbullets2[i].x==enemies[2].x&&enemiesbullets2[i].y==enemies[2].y+enemies[2].h/2)
			    if(sound==1) al_play_sample(enemiesbullets2[i].music, 0.3, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_ONCE, &enemiesbullets2[i].id);
		}
		
		for (i=0;i<MAX_BULLET;i++){
			draw_movable_object(hearts[i]);
		}
		
		for (i=0;i<MAX_BULLET;i++){
			draw_movable_object(bossbullets[i]);
			if(!bossbullets[i].hidden&&bossbullets[i].x==boss.x&&bossbullets[i].y==boss.y+boss.h/2)
			    if(sound==1) al_play_sample(bossbullets[i].music, 0.5, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_ONCE, &bossbullets[i].id);
		}
		draw_movable_object(plane);
		for (i = 0; i < MAX_ENEMY; i++){
			draw_movable_object(enemies[i]);
			int j;
			for (j = 1; j < 4; j++){
			    if(j==enemies[i].life)
				    al_draw_bitmap(img_enemieslife[j-1], enemies[i].x+enemies[i].w/2, enemies[i].y-enemies[i].h/2+10, 0);
			    if(j==enemies[i].heart)
			        al_draw_bitmap(img_enemiesheart[j-1], enemies[i].x+enemies[i].w/2, enemies[i].y-enemies[i].h/2, 0);
		    }
		}
			
		for (i = 1; i < 4; i++){
			if(i==plane.life)
				al_draw_bitmap(img_life[i-1], 0, 0, 0);
			if(i==plane.heart)
			    al_draw_bitmap(img_heart[i-1], SCREEN_W/5, 0, 0);
		}
		draw_movable_object(boss);
		for (i = 1; i < 11; i++){
			if(i==boss.life&&!boss.hidden)
				al_draw_bitmap(img_bosslife[i-1], SCREEN_W-200, 0, 0);
		}
			
	}
	// [HACKATHON 3-9]
	// TODO: If active_scene is SCENE_SETTINGS.
	// Draw anything you want, or simply clear the display.
	else if (active_scene == SCENE_SETTINGS) {
		al_draw_bitmap(setting_background, 0, 0, 0);
		al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 10, 10, 0, "click to choose scene");
		al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 10, 240, 0, "click to choose plane");
		al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 10, 400, 0, "click to choose music");
		al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 10, 480, 0, "click to choose mode");
		al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H-40, ALLEGRO_ALIGN_CENTER, "press enter to play");
		al_draw_bitmap(background1, 50, 60, 0);
		al_draw_bitmap(background2, 300, 60, 0);
		al_draw_bitmap(background3, 550, 60, 0);
		
		if(sound==1) al_draw_bitmap(img_on, 430, 400, 0);
		else al_draw_bitmap(img_off, 430, 400, 0);
		
		if(bgm==1){
			al_draw_bitmap(img_btnon, 90, 450, 0);
			al_draw_bitmap(img_btnoff, 340, 450, 0);
			al_draw_bitmap(img_btnoff, 590, 450, 0);
			al_stop_sample(&start_bgm_id);
			start_bgm=bgm1;
		}
		else if(bgm==2){
			al_draw_bitmap(img_btnoff, 90, 450, 0);
			al_draw_bitmap(img_btnon, 340, 450, 0);
			al_draw_bitmap(img_btnoff, 590, 450, 0);
			al_stop_sample(&start_bgm_id);
			start_bgm=bgm2;
		} 
		else if(bgm==3){
			al_draw_bitmap(img_btnoff, 90, 450, 0);
			al_draw_bitmap(img_btnoff, 340, 450, 0);
			al_draw_bitmap(img_btnon, 590, 450, 0);
			al_stop_sample(&start_bgm_id);
			start_bgm=bgm3;
		} 
		
		al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 150, 450, ALLEGRO_ALIGN_CENTER, "bgm_1");
		al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 400, 450, ALLEGRO_ALIGN_CENTER, "bgm_2");
		al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 650, 450, ALLEGRO_ALIGN_CENTER, "bgm_3");
		
		if(!mode){
			al_draw_bitmap(img_btnon2, 800/3-80, 530, 0);
			al_draw_bitmap(img_btnoff2, 1600/3-80, 530, 0);
		}
		else{
			al_draw_bitmap(img_btnoff2, 800/3-80, 530, 0);
			al_draw_bitmap(img_btnon2, 1600/3-80, 530, 0);
		}
		al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 800/3, 530, ALLEGRO_ALIGN_CENTER, "difficult");
		al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 1600/3, 530, ALLEGRO_ALIGN_CENTER, "easy");
		
		if (pnt_in_rect(mouse_x, mouse_y, 50, 60, 200, 150))
			al_draw_bitmap(choose1, 50, 60, 0);
		else if (pnt_in_rect(mouse_x, mouse_y, 300, 60, 200, 150))
			al_draw_bitmap(choose1, 300, 60, 0);
		else if (pnt_in_rect(mouse_x, mouse_y, 550, 60, 200, 150))
			al_draw_bitmap(choose1, 550, 60, 0);
		if (choosescene==1)
			al_draw_bitmap(choose1, 50, 60, 0);
		else if (choosescene==2)
			al_draw_bitmap(choose1, 300, 60, 0);
		else if (choosescene==3)
			al_draw_bitmap(choose1, 550, 60, 0);
		
		al_draw_bitmap(plane1, 99, 300, 0);
		al_draw_bitmap(plane2, 349, 300, 0);
		al_draw_bitmap(plane3, 599, 300, 0);
		
		if (chooseplane==1)
			al_draw_bitmap(choose2, 99, 300, 0);
		else if (chooseplane==2)
			al_draw_bitmap(choose2, 349, 300, 0);
		else if (chooseplane==3)
			al_draw_bitmap(choose2, 599, 300, 0);
		
		if (pnt_in_rect(mouse_x, mouse_y, 99, 300, 102, 88)){
			al_draw_bitmap(choose2, 99, 300, 0);
			al_draw_bitmap(img_power1, 200, 150, 0);
		}
		else if (pnt_in_rect(mouse_x, mouse_y, 349, 300, 102, 64)){
			al_draw_bitmap(choose2, 349, 300, 0);
			al_draw_bitmap(img_power2, 200, 150, 0);
		}	
		else if (pnt_in_rect(mouse_x, mouse_y, 599, 300, 102, 96)){
			al_draw_bitmap(choose2, 599, 300, 0);
			al_draw_bitmap(img_power3, 200, 150, 0);
		}
			
	}
	else if (active_scene == SCENE_OVER){
		active_scene=NULL;
		al_draw_bitmap(img_gameover, 0, 0, 0);
		al_flip_display();
		int score;
		if(plane.heart&&playtime<=60){
			if(sound==1) al_play_sample(win, 1, ALLEGRO_AUDIO_PAN_NONE, 1.0, ALLEGRO_PLAYMODE_ONCE, &win_id);
			al_draw_text(font_pirulen_72, al_map_rgb(253,43,41), SCREEN_W / 2, SCREEN_H / 5, ALLEGRO_ALIGN_CENTER, "YOU WIN!");
			score=(number)?shootenemies*100/number*planelife/playtime*1000:0;
			al_draw_textf(font_pirulen_60 , al_map_rgb (255,202,24) , SCREEN_W / 2 , SCREEN_H / 4+40 , ALLEGRO_ALIGN_CENTRE   , "SCORE: %d" , score );
			al_draw_text(font_pirulen_60, al_map_rgb(96,0,160), SCREEN_W / 2, SCREEN_H / 2-30, ALLEGRO_ALIGN_CENTER, "Again?");
			al_draw_text(font_pirulen_60, al_map_rgb(0,17,168), 0+50, SCREEN_H / 2+30, 0, "Press");
			al_draw_text(font_pirulen_60, al_map_rgb(0,17,168), SCREEN_W / 2, SCREEN_H / 2+100, ALLEGRO_ALIGN_CENTER, "Y(yes)");
			al_draw_text(font_pirulen_60, al_map_rgb(0,17,168), SCREEN_W / 2, SCREEN_H / 2+160, ALLEGRO_ALIGN_CENTER, "N(no)");
			al_draw_text(font_pirulen_60, al_map_rgb(0,17,168), SCREEN_W / 2, SCREEN_H / 2+230, ALLEGRO_ALIGN_CENTER, "E(exit)");
		}
		else{
			if(sound==1) al_play_sample(lose, 1, ALLEGRO_AUDIO_PAN_NONE, 1.0, ALLEGRO_PLAYMODE_ONCE, &lose_id);
			al_draw_text(font_pirulen_72, al_map_rgb(253,43,41), SCREEN_W / 2, SCREEN_H / 5, ALLEGRO_ALIGN_CENTER, "YOU LOSE!");
			score=(number)?shootenemies*100/number*0.5*playtime:0;
			al_draw_textf(font_pirulen_60 , al_map_rgb (255,202,24) , SCREEN_W / 2 , SCREEN_H / 4+40 , ALLEGRO_ALIGN_CENTRE   , "SCORE: %d" , score );
			al_draw_text(font_pirulen_60, al_map_rgb(96,0,160), SCREEN_W / 2, SCREEN_H / 2-30, ALLEGRO_ALIGN_CENTER, "Again?");
			al_draw_text(font_pirulen_60, al_map_rgb(0,17,168), 0+50, SCREEN_H / 2+30, 0, "Press");
			al_draw_text(font_pirulen_60, al_map_rgb(0,17,168), SCREEN_W / 2, SCREEN_H / 2+100, ALLEGRO_ALIGN_CENTER, "Y(yes)");
			al_draw_text(font_pirulen_60, al_map_rgb(0,17,168), SCREEN_W / 2, SCREEN_H / 2+160, ALLEGRO_ALIGN_CENTER, "N(no)");
			al_draw_text(font_pirulen_60, al_map_rgb(0,17,168), SCREEN_W / 2, SCREEN_H / 2+230, ALLEGRO_ALIGN_CENTER, "E(exit)");
		} 
		game_log("shootenemies:%d",shootenemies);
		game_log("number:%d",number);
		game_log("planelife:%f",planelife);
		game_log("playtime:%lf",playtime);
		game_log("score:%d",score);
	}
	al_flip_display();
}

void game_destroy(void) {
	// Destroy everything you have created.
	// Free the memories allocated by malloc or allegro functions.
	// Destroy shared resources.
	al_destroy_font(font_pirulen_32);
	al_destroy_font(font_pirulen_24);

	/* Menu Scene resources*/
	al_destroy_bitmap(main_img_background);
	al_destroy_sample(main_bgm);
	al_destroy_sample(boss_bgm);
	al_destroy_sample(win);
	al_destroy_sample(lose);
	al_destroy_sample(shoot1);
	al_destroy_sample(shoot2);
	al_destroy_sample(shoot3);
	al_destroy_sample(boom);
	al_destroy_sample(bossboom);
	// [HACKATHON 3-6]
	// TODO: Destroy the 2 settings images.
	// Uncomment and fill in the code below.
	al_destroy_bitmap(img_settings);
	al_destroy_bitmap(img_settings2);

	/* Start Scene resources*/
	al_destroy_bitmap(start_img_background);
	al_destroy_bitmap(background1);
	al_destroy_bitmap(background2);
	al_destroy_bitmap(background3);
	al_destroy_bitmap(choose1);
	al_destroy_bitmap(choose2);
	al_destroy_bitmap(setting_background);
	al_destroy_bitmap(start_img_plane);
	al_destroy_bitmap(plane1);
	al_destroy_bitmap(plane2);
	al_destroy_bitmap(plane3);
	al_destroy_bitmap(start_img_enemy);
	al_destroy_bitmap(img_boss);
	al_destroy_bitmap(img_time);
	al_destroy_sample(start_bgm);
	al_destroy_sample(bgm2);
	al_destroy_sample(bgm3);
	al_destroy_bitmap(img_power1);
	al_destroy_bitmap(img_power2);
	al_destroy_bitmap(img_power3);
	al_destroy_bitmap(img_on);
	al_destroy_bitmap(img_off);
	al_destroy_bitmap(img_btnon);
	al_destroy_bitmap(img_btnoff);
	al_destroy_bitmap(img_btnon2);
	al_destroy_bitmap(img_btnoff2);
	// [HACKATHON 2-10]
	// TODO: Destroy your bullet image.
	// Uncomment and fill in the code below.
	al_destroy_bitmap(img_bullet);
	al_destroy_bitmap(img_enemiesbullet);
	al_destroy_bitmap(img_bossbullet);
	al_destroy_bitmap(img_heart[0]);
	al_destroy_bitmap(img_heart[1]);
	al_destroy_bitmap(img_heart[2]);
	al_destroy_bitmap(img_life[0]);
	al_destroy_bitmap(img_life[1]);
	al_destroy_bitmap(img_life[2]);
	int i;
	for(i=0;i<10;i++) al_destroy_bitmap(img_bosslife[i]);
	al_destroy_bitmap(img_enemiesheart[0]);
	al_destroy_bitmap(img_enemiesheart[1]);
	al_destroy_bitmap(img_enemiesheart[2]);
	al_destroy_bitmap(img_enemieslife[0]);
	al_destroy_bitmap(img_enemieslife[1]);
	al_destroy_bitmap(img_enemieslife[2]);

	al_destroy_timer(game_update_timer);
	al_destroy_event_queue(game_event_queue);
	al_destroy_display(game_display);
	free(mouse_state);
}

void game_change_scene(int next_scene) {
	game_log("Change scene from %d to %d", active_scene, next_scene);
	// TODO: Destroy resources initialized when creating scene.
	if (active_scene == SCENE_MENU) {
		al_stop_sample(&main_bgm_id);
		game_log("stop audio (bgm)");
	} else if (active_scene == SCENE_START) {
		al_stop_sample(&start_bgm_id);
		game_log("stop audio (bgm)");
		al_stop_sample(&boss_bgm_id);
		game_log("stop audio (bgm)");
	}
	else if (active_scene == SCENE_SETTINGS) {
		al_stop_sample(&bgm1_id);
		game_log("stop audio (bgm)");
		al_stop_sample(&bgm2_id);
		game_log("stop audio (bgm)");
		al_stop_sample(&bgm3_id);
		game_log("stop audio (bgm)");
	}
	active_scene = next_scene;
	// TODO: Allocate resources before entering scene.
	if (active_scene == SCENE_MENU&& sound==1) {
		if (!al_play_sample(main_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &main_bgm_id))
			game_abort("failed to play audio (bgm)");
	} 
	else if (active_scene == SCENE_BOSS) {
		if(sound==1){
			if (!al_play_sample(boss_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &boss_bgm_id))
			    game_abort("failed to play audio (bgm)");
		}
		active_scene = SCENE_START;
	}
	else if (active_scene == SCENE_START) {
		shootenemies=0;
		number=0;
		planelife=9;
		if(choosescene==1)
		    start_img_background = load_bitmap_resized("space.jpg", SCREEN_W, SCREEN_H);
		else if(choosescene==3)
		    start_img_background = load_bitmap_resized("space4.jpg", SCREEN_W, SCREEN_H);
		int i;
		if(chooseplane==2)
		    plane.img =load_bitmap_resized("spaceShips2.png", 51, 32);
		else if(chooseplane==3)
		    plane.img = load_bitmap_resized("spaceShips3.png", 51, 48);
	    else plane.img = start_img_plane;
		
		plane.x = 400;
		plane.y = 500;
		plane.w = al_get_bitmap_width(plane.img);
        plane.h = al_get_bitmap_height(plane.img);
        plane.heart=3;
        plane.life=3;
        plane.hidden=false;
        
		boss.img = img_boss;
		boss.x = 400;
		boss.y = 80;
		boss.w = al_get_bitmap_width(boss.img);
        boss.h = al_get_bitmap_height(boss.img);
        boss.vx=rand()%3+4;
			if(boss.vx==5) boss.vx=-5;
		boss.vy=3;
        boss.heart=1;
        boss.life=10;
        boss.hidden=true;
        srand(time(NULL));
		for (i = 0; i < MAX_ENEMY; i++) {
			enemies[i].img = start_img_enemy;
			enemies[i].w = al_get_bitmap_width(start_img_enemy);
			enemies[i].h = al_get_bitmap_height(start_img_enemy);
			enemies[i].x = enemies[i].w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - enemies[i].w);
			enemies[i].y = 80;
			enemies[i].vx=rand()%6;
			if(!enemies[i].vx) enemies[i].vx=-3;
			enemies[i].vy=rand()%4;
			if(!enemies[i].vy) enemies[i].vy=-3;
			enemies[i].hidden=false;
			enemies[i].heart=2;
			enemies[i].life=3;
			enemies[i].recall=0;
		}
		// [HACKATHON 2-6]
		// TODO: Initialize bullets.
		// For each bullets in array, set their w and h to the size of
		// the image, and set their img to bullet image, hidden to true,
		// (vx, vy) to (0, -3).
		// Uncomment and fill in the code below.
		for (i=0;i<MAX_BULLET;i++) {
			bullets[i].w = al_get_bitmap_width(img_bullet);
			bullets[i].h = al_get_bitmap_height(img_bullet);
			bullets[i].img = img_bullet;
			bullets[i].vx = 0;
			if(chooseplane==2) bullets[i].vy = -10;
			else bullets[i].vy = -6;
			bullets[i].hidden = true;
			bullets[i].music=shoot2;
		}
		for (i=0;i<MAX_BULLET;i++) {
			hearts[i].w = al_get_bitmap_width(img_hearts);
			hearts[i].h = al_get_bitmap_height(img_hearts);
			hearts[i].img = img_hearts;
			hearts[i].vx = 0;
			hearts[i].vy = 3;
			hearts[i].hidden = true;
		}
		for (i=0;i<MAX_BULLET;i++) {
			enemiesbullets0[i].w = al_get_bitmap_width(img_enemiesbullet);
			enemiesbullets0[i].h = al_get_bitmap_height(img_enemiesbullet);
			enemiesbullets0[i].img = img_enemiesbullet;
			enemiesbullets0[i].vx = 0;
			enemiesbullets0[i].vy = 5;
			enemiesbullets0[i].hidden = true;
			enemiesbullets0[i].music=shoot1;
		}
		for (i=0;i<MAX_BULLET;i++) {
			enemiesbullets1[i].w = al_get_bitmap_width(img_enemiesbullet);
			enemiesbullets1[i].h = al_get_bitmap_height(img_enemiesbullet);
			enemiesbullets1[i].img = img_enemiesbullet;
			enemiesbullets1[i].vx = 0;
			enemiesbullets1[i].vy = 6;
			enemiesbullets1[i].hidden = true;
			enemiesbullets1[i].music=shoot1;
		}
		for (i=0;i<MAX_BULLET;i++) {
			enemiesbullets2[i].w = al_get_bitmap_width(img_enemiesbullet);
			enemiesbullets2[i].h = al_get_bitmap_height(img_enemiesbullet);
			enemiesbullets2[i].img = img_enemiesbullet;
			enemiesbullets2[i].vx = 0;
			enemiesbullets2[i].vy = 4;
			enemiesbullets2[i].hidden = true;
			enemiesbullets2[i].music=shoot1;
		}
		for (i=0;i<MAX_BULLET;i++) {
			bossbullets[i].w = al_get_bitmap_width(img_bossbullet);
			bossbullets[i].h = al_get_bitmap_height(img_bossbullet);
			bossbullets[i].img = img_bossbullet;
			bossbullets[i].vx = 0;
			bossbullets[i].vy = 8;
			bossbullets[i].hidden = true;
			bossbullets[i].music=shoot3;
		}
		if(sound==1){
			if (!al_play_sample(start_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &start_bgm_id))
			    game_abort("failed to play audio (bgm)");
		}
	}
}

void on_key_down(int keycode) {
	if (active_scene == SCENE_MENU) {
		if (keycode == ALLEGRO_KEY_ENTER){
			game_change_scene(SCENE_START);
			start=al_get_time();
		}	
		else if (keycode == ALLEGRO_KEY_S)
			game_change_scene(SCENE_SETTINGS);
	}
	else if(active_scene ==NULL){
		if (keycode == ALLEGRO_KEY_Y){
			game_change_scene(SCENE_START);
			start=al_get_time();
		}
		else if (keycode == ALLEGRO_KEY_N)
			game_change_scene(SCENE_MENU);
	}
	else if(active_scene ==SCENE_SETTINGS){
		if (keycode == ALLEGRO_KEY_ENTER){
			game_change_scene(SCENE_START);
			start=al_get_time();
		}
	}
}

void on_mouse_down(int btn, int x, int y) {
	// [HACKATHON 3-8]
	// TODO: When settings clicked, switch to settings scene.
	// Uncomment and fill in the code below.
	//game_log("btn:%d",btn);
	/*if (active_scene == SCENE_MENU) {
		if (btn == SCENE_MENU) {
			if (pnt_in_rect(x, y, SCREEN_W - 48, 10, 38, 38))
				game_change_scene(SCENE_SETTINGS);
		}
	}*/
	if (active_scene == SCENE_SETTINGS) {
		if (btn == 1) {
			if (pnt_in_rect(mouse_x, mouse_y, 50, 60, 200, 150))
				choosescene=1;
			else if (pnt_in_rect(mouse_x, mouse_y, 300, 60, 200, 150))
			    choosescene=2;
		    else if (pnt_in_rect(mouse_x, mouse_y, 550, 60, 200, 150))
			    choosescene=3;
			if (pnt_in_rect(mouse_x, mouse_y, 99, 300, 102, 88))
			    chooseplane=1;
		    else if (pnt_in_rect(mouse_x, mouse_y, 349, 300, 102, 64))
			    chooseplane=2;
		    else if (pnt_in_rect(mouse_x, mouse_y, 599, 300, 102, 96))
			    chooseplane=3;
			if(pnt_in_rect(mouse_x, mouse_y, 430, 400, 68, 25)&&sound==1)
			    sound=0;
			else if(pnt_in_rect(mouse_x, mouse_y, 430, 400, 68, 25)&&sound==0)
			    sound=1;
			if(pnt_in_rect(mouse_x, mouse_y, 90, 450, 120, 30)){
				bgm=1;
				al_stop_sample(&bgm2_id);
			    al_stop_sample(&bgm3_id);
			    al_play_sample(bgm1, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &bgm1_id);
			}
				    
			else if(pnt_in_rect(mouse_x, mouse_y, 340, 450, 120, 30)){
				bgm=2;
				al_stop_sample(&bgm1_id);
			    al_stop_sample(&bgm3_id);
			    al_play_sample(bgm2, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &bgm2_id);
			}
			else if(pnt_in_rect(mouse_x, mouse_y, 590, 450, 120, 30)){
				bgm=3;
				al_stop_sample(&bgm2_id);
			    al_stop_sample(&bgm1_id);
			    al_play_sample(bgm3, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &bgm3_id);
			}
			if(pnt_in_rect(mouse_x, mouse_y, 800/3-80, 530, 160, 30)) mode=0;
			else if(pnt_in_rect(mouse_x, mouse_y, 1600/3-80, 530, 160, 30)) mode=1;
			    
		}
	}
}

void draw_movable_object(MovableObject obj) {
	if (obj.hidden)
		return;
	al_draw_bitmap(obj.img, round(obj.x - obj.w / 2), round(obj.y - obj.h / 2), 0);
}

ALLEGRO_BITMAP *load_bitmap_resized(const char *filename, int w, int h) {
	ALLEGRO_BITMAP* loaded_bmp = al_load_bitmap(filename);
	if (!loaded_bmp)
		game_abort("failed to load image: %s", filename);
	ALLEGRO_BITMAP *resized_bmp = al_create_bitmap(w, h);
	ALLEGRO_BITMAP *prev_target = al_get_target_bitmap();

	if (!resized_bmp)
		game_abort("failed to create bitmap when creating resized image: %s", filename);
	al_set_target_bitmap(resized_bmp);
	al_draw_scaled_bitmap(loaded_bmp, 0, 0,
		al_get_bitmap_width(loaded_bmp),
		al_get_bitmap_height(loaded_bmp),
		0, 0, w, h, 0);
	al_set_target_bitmap(prev_target);
	al_destroy_bitmap(loaded_bmp);

	//game_log("resized image: %s", filename);

	return resized_bmp;
}

// [HACKATHON 3-3]
// TODO: Define bool pnt_in_rect(int px, int py, int x, int y, int w, int h)
// Uncomment and fill in the code below.
bool pnt_in_rect(int px, int py, int x, int y, int w, int h) {
	if(px>=x&&px<=x+w&&py>=y&&py<=y+h) return true;
	return false;
}


// +=================================================================+
// | Code below is for debugging purpose, it's fine to remove it.    |
// | Deleting the code below and removing all calls to the functions |
// | doesn't affect the game.                                        |
// +=================================================================+

void game_abort(const char* format, ...) {
	va_list arg;
	va_start(arg, format);
	game_vlog(format, arg);
	va_end(arg);
	fprintf(stderr, "error occured, exiting after 2 secs");
	// Wait 2 secs before exiting.
	al_rest(2);
	// Force exit program.
	exit(1);
}

void game_log(const char* format, ...) {
#ifdef LOG_ENABLED
	va_list arg;
	va_start(arg, format);
	game_vlog(format, arg);
	va_end(arg);
#endif
}

void game_vlog(const char* format, va_list arg) {
#ifdef LOG_ENABLED
	static bool clear_file = true;
	vprintf(format, arg);
	printf("\n");
	// Write log to file for later debugging.
	FILE* pFile = fopen("log.txt", clear_file ? "w" : "a");
	if (pFile) {
		vfprintf(pFile, format, arg);
		fprintf(pFile, "\n");
		fclose(pFile);
	}
	clear_file = false;
#endif
}
