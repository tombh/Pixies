// `pkg-config --cflags --libs gtk+-2.0` 

#include <stdio.h>
#include <math.h>
#include <gtk/gtk.h>

// -----------------------------------------------------------
// INITIALISATION 
// -----------------------------------------------------------




//==============================================
//Variables managed by GTK Control Centre
gboolean active_pixies = 0;

int x, y;

int screen_width = 400;
int screen_height = 400;

enum { OFF, ON };

trails = OFF;

global_static_flag = OFF; 

#define COOLER 1
int population = 0;
float FPS = 10;

float total_energy;

float GRAVITY = 0;
#define SPEED 0.05

#define WELL_DEPTH 0.001 
#define DISTANCE_AT_REST 30
//===============================================





//+++++++++++++++++++++++++++++++++++++++++++++
//Variables more to do with Pixie World

GtkHScale *FPS_slider,
		  *slomo_slider,	
          *gravity_slider,
          *ch_slider,
          *mass_slider,
          *size_slider;

static GtkWidget *static_switch;

gchar text[32];

GtkWidget *da;

GtkLabel *population_label;
GtkLabel *energy_label;
GtkLabel *cycles_recorded_label;
GtkLabel *selected_pixie_label;

gpointer *data;

GdkPixbuf *buffer;


static GtkColorButton *color_button;
//Color Button's current colour
GdkColor color;

GtkButton *adjust_button;

int new_px, new_py;

enum user_mode {
	SELECTOR,
	CREATOR 
};

user_mode = CREATOR;

int global_current_pixie;

//+++++++++++++++++++++++++++++++++++++++++++++++++


//function prototype for gtk idle add
int pixies_main(void);







//Attributes of each individual Pixie
struct pixie{
	float     x_position, 
	  	      y_position, 
		      x_direction,
		      y_direction,
		      mass,
		      ch_value,
		      size;
	
	gboolean  static_flag;
	
	GdkColor  color_gtk; 
	
	int 	  style;
			  	
	char     name[30];	
};



enum{ SIMPLE_DOT,
	  CROSS_HAIR,
	  SELECTED};


//The main array in which all Pixies live
struct pixie village[100];


//constants for the ljp function
float A, B;

//Timer to measure Frames Per Second
GTimer *FPS_timer;

#include "pixie_functions.h"
#include "pixies_gtk.h"

//----------------------------------------------------------------
// END OF INITIALISATION
//----------------------------------------------------------------





gint pixies_main() {
	
	int looper, slomo;
	
	//System energy is calculated by accumalting the pixies individual energies
	//i.e. its need to be reset on each loop
	total_energy = 0;
	
	//do-nothing code for slow motion effect			
	slomo = gtk_range_get_value(slomo_slider);
	for (looper = 0; looper <= slomo*slomo*slomo; looper++){;}
	
	//make sure screen is cleared when popultaion is zero
	if (population == 0) gtk_widget_queue_draw(da);
		
	
	//Main village loop
	for (looper = 0; looper < population; looper++){
		
		if (active_pixies){
			
			if (!village[looper].static_flag){
				
				//Apply the physical fields
				ljp();
						
				//Add the direction to their position	
				village[looper].x_position += village[looper].x_direction;
				village[looper].y_position += village[looper].y_direction;
			
			}
			
			
			//Apply cooling or heating
			if (village[looper].ch_value < 0){
				village[looper].x_direction /= abs(village[looper].ch_value);
			    village[looper].y_direction /= abs(village[looper].ch_value);
			}
			
			if (village[looper].ch_value > 0){
				village[looper].x_direction *= (village[looper].ch_value / 100) + 1;
			    village[looper].y_direction *= (village[looper].ch_value / 100) + 1;
			}
			
						
			//Apply Gravity
			GRAVITY = gtk_range_get_value(gravity_slider);
			if (village[looper].y_position < screen_width) village[looper].y_direction += GRAVITY;
			
			
			//calculation for total energy value
			//Oh dear, looks like you need to do some calculus to get this boy working properly...
			//because force is the GRADIENT of the LJ potential
			float resolved_speed;
			resolved_speed = SquareRootFloat( (village[looper].x_direction  * village[looper].x_direction ) + (village[looper].y_direction  * village[looper].y_direction ) ); 
			total_energy += resolved_speed;
			
				
			//Check for wall-bashing
			
			float right_dist, bottom_dist;
			int gap = 30;
			
			right_dist =  screen_width - village[looper].x_position;
			village[looper].x_direction -= 10 / pow( fabs(right_dist - gap), 3);
			village[looper].x_direction += 10 / pow( fabs(village[looper].x_position - gap), 3);
			
			bottom_dist =  screen_height - village[looper].y_position;
			village[looper].y_direction -= 10 / pow( fabs(bottom_dist - gap), 3);
			village[looper].y_direction += 10 / pow( fabs(village[looper].y_position - gap), 3);
			
			
			/*
			if (village[looper].x_position <= gap){
				village[looper].x_direction = fabs(village[looper].x_direction);
			}
			
			if (village[looper].x_position >= (screen_width - gap) ){
				village[looper].x_direction = -fabs(village[looper].x_direction);
			}
			
			if (village[looper].y_position <= gap){
				village[looper].y_direction = fabs(village[looper].y_direction);
			}
			
			if (village[looper].y_position >= (screen_height - gap) ){
				village[looper].y_direction = -fabs(village[looper].y_direction);
			}
			*/
			
		}
		
		//Provide visual information for screen display
		x = village[looper].x_position;
		y = village[looper].y_position;
		
		//printf("x%d\n", x );
		//printf("y%d\n", y );
					
		//place data to screen BUFFER
		put_pixel(buffer, x, y, village[looper].color_gtk);
		put_pixel(buffer, x+1, y, village[looper].color_gtk);
		put_pixel(buffer, x, y+1, village[looper].color_gtk);
		put_pixel(buffer, x-1, y, village[looper].color_gtk);
		put_pixel(buffer, x, y-1, village[looper].color_gtk);
		
		if (village[looper].style == SELECTED){
			put_pixel(buffer, x, y, village[looper].color_gtk);
			put_pixel(buffer, x+2, y, village[looper].color_gtk);
			put_pixel(buffer, x, y+2, village[looper].color_gtk);
			put_pixel(buffer, x-2, y, village[looper].color_gtk);
			put_pixel(buffer, x, y-2, village[looper].color_gtk);
		}
		
			
		//Deal with the Frames Per Second Malarky
		FPS = gtk_range_get_value(FPS_slider);
		
		if ( g_timer_elapsed(FPS_timer, NULL) > (1 / FPS) && looper == population - 1 ){
										
			gtk_widget_queue_draw(da);
			
			//update Pixie World labels
			update_energy_label();
						
			g_timer_reset(FPS_timer);
			
			
		
		}
	}
	
	    
return TRUE;  
	
}
//=====================================================================



int main( int argc, char *argv[] ){
	
		
	//constants for ljp function
	A = 4 * WELL_DEPTH * pow(DISTANCE_AT_REST, 4);
	B = 4 * WELL_DEPTH * pow(DISTANCE_AT_REST, 2);
	
    //Why does "da" HAVE TO be scoped into this variable space??
    //If I don't do it I get a, **GTK da is not a widget**  
    da = initialise_gtk( argc, argv );
    
    //Start the timer
	FPS_timer = g_timer_new();
        
    gtk_main();
    
}
