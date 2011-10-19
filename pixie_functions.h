//---------------------PIXIE FUNCTIONS----------------------------------



int quit_function(gpointer *data){
	printf("x%d\n", x );
	printf("y%d\n", y );
	return 0;
}




float SquareRootFloat(float number) {
    long i;
    float x, y;
    const float f = 1.5F;

    x = number * 0.5F;
    y  = number;
    i  = * ( long * ) &y;
    i  = 0x5f3759df - ( i >> 1 );
    y  = * ( float * ) &i;
    y  = y * ( f - ( x * y * y ) );
    y  = y * ( f - ( x * y * y ) );
    return number * y;
}




double my_random(double max){
	double value = random() / (double)( 2147483647 / max);
	return value;
}




gboolean adjust_pixie(gpointer *data){
	
	gtk_color_button_get_color(color_button, &color);
	
	/*		
	village[i].x_direction = my_random((SPEED * 2)) - SPEED;
	village[i].y_direction = my_random((SPEED * 2)) - SPEED;
	*/
	
	village[global_current_pixie].ch_value = gtk_range_get_value(ch_slider);
	village[global_current_pixie].color_gtk = color;
	village[global_current_pixie].static_flag = global_static_flag;
	village[global_current_pixie].mass = gtk_range_get_value(mass_slider);
	village[global_current_pixie].size = gtk_range_get_value(size_slider);
	
	return TRUE;
}




gboolean user_mode_switcher(gpointer *data){
	
		
	if (user_mode == SELECTOR){
		user_mode = CREATOR;
		gtk_widget_set_sensitive(adjust_button, FALSE);
	}else{
		user_mode = SELECTOR;
		gtk_widget_set_sensitive(adjust_button, TRUE);
	}
	
	return TRUE;
}




start_stop(gpointer *data){
	active_pixies = ~active_pixies;
}




gint record(gpointer *data){
	printf("record pressed");
}





gint record_reset(gpointer *data){
	printf("reset reocrd prewssed");
}




killall(gpointer *data){
	population = 0;
	
	gchar g_population[32];
	sprintf(g_population, "Population: 0");
	gtk_label_set_text (population_label, g_population);
}




void update_energy_label(void){
	
	sprintf(text, "Total Energy: %f", total_energy);
	gtk_label_set_text (energy_label, text);
}




static gboolean expose(GtkWidget *widget, GdkEventExpose *event, gpointer data){

	//Copy buffer to screen
	gdk_draw_pixbuf (widget->window,
					 NULL,
					 buffer,
					 0,
					 0,
					 0,
					 0,
					 -1,
					 -1,
					 GDK_RGB_DITHER_NONE,
					 0,
					 0);
								 
	//Fill *buffer* (ie NOT screen) with BBBBBLLLLLAAAAAACCCCCCKKKKKKKKKKK
	if (!trails) gdk_pixbuf_fill(buffer, 0x00000000);
  
    return FALSE;
}





trails_switcher(gpointer *data){
	trails = ~trails;
}




static_switcher(gpointer *data){
	global_static_flag = ~global_static_flag;
}





int locate_nearest_pixie(){
	int j;
	int current_contender;
	float x_seperation, y_seperation, d, smallest;
	
	smallest = 1000000;
		
	for (j = 0; j < population; j++){
		
		x_seperation = (village[j].x_position - new_px);
		y_seperation = (village[j].y_position - new_py);
		
		d = SquareRootFloat( (x_seperation * x_seperation) + (y_seperation * y_seperation) ); 
		
		if ( d < smallest ){
			smallest = d;
			current_contender = j;
		}
	
	}
	
	printf("\ncc: %i", current_contender);
	return current_contender;

}





select_pixie(int pixie_id){
	
	printf("\nid: %i", pixie_id);
			
	/*
	village[i].x_direction = my_random((SPEED * 2)) - SPEED;
	village[i].y_direction = my_random((SPEED * 2)) - SPEED;
	*/
	
	
	gtk_range_set_value(ch_slider, village[pixie_id].ch_value);
	gtk_color_button_set_color(color_button, &village[pixie_id].color_gtk);
	gtk_toggle_button_set_active (static_switch, village[pixie_id].static_flag);
	gtk_range_set_value(mass_slider, village[pixie_id].mass);
	gtk_range_set_value(size_slider, village[pixie_id].size);
	
	
	village[pixie_id].style = SELECTED;
	
	global_current_pixie = pixie_id;
		
	sprintf(text, "Current Pixie: %i", pixie_id);
	gtk_label_set_text (selected_pixie_label, text);
	

}
	





population_increase(gpointer *data){
	population++;	
	int i = population - 1;
	
	gtk_color_button_get_color(color_button, &color);
	
		
	village[i].x_position = new_px;
	village[i].y_position = new_py;
	village[i].x_direction = my_random((SPEED * 2)) - SPEED;
	village[i].y_direction = my_random((SPEED * 2)) - SPEED;
	village[i].ch_value = gtk_range_get_value(ch_slider);
	village[i].color_gtk = color;
	village[i].static_flag = global_static_flag;
	village[i].mass = gtk_range_get_value(mass_slider);
	village[i].size = gtk_range_get_value(size_slider);
	
	village[i].style = SIMPLE_DOT;
	sprintf(village[i].name, "Derek");
	
	sprintf(text, "Population: %d", population);
	gtk_label_set_text (population_label, text);
	
	
}




void population_decrease(gpointer *data){
	
	if (population == 0) {
		return;
	}else{
		population--;
	}
	
	gchar g_population[32];
	sprintf(g_population, "Population: %d", population);
	gtk_label_set_text (population_label, g_population);
}





static gint button_press_event( GtkWidget *widget, GdkEventButton *event ){
	
  new_px = event->x;
  new_py = event->y;
    
  switch (user_mode){
  	
  	case SELECTOR:
		printf("woop!");
		
		select_pixie( locate_nearest_pixie() );	
		
  	break;
  	
  	case CREATOR:
		population_increase(NULL);
	break;
	
	
  }
  	
  return TRUE;	

}




static void
put_pixel (GdkPixbuf *pixbuf, int x, int y, GdkColor color)
{
  
  if (x < 0 || x > screen_width || y < 0 || y > screen_height){
		printf("x%d\n", x );
		printf("y%d\n", y );
		active_pixies = OFF;
		return;
  }
  
  
  int rowstride;
  guchar *pixels, *p;

  rowstride = gdk_pixbuf_get_rowstride (pixbuf);
  pixels = gdk_pixbuf_get_pixels (pixbuf);

  p = pixels + y * rowstride + x * 3;
  p[0] = color.red;
  p[1] = color.green;
  p[2] = color.blue;
   
}





//-----------------------------------------------
// Fast arctan2
float arctan2(float y, float x){
	float angle, r;	
	
   float coeff_1 = G_PI_4;
   float coeff_2 = 3*coeff_1;
   float abs_y = fabs(y)+1e-10;     // kludge to prevent 0/0 condition
   if (x>=0)
   {
      r = (x - abs_y) / (x + abs_y);
      angle = coeff_1 - coeff_1 * r;
   }
   else
   {
      r = (x + abs_y) / (abs_y - x);
      angle = coeff_2 - coeff_1 * r;
   }
   
   if (y < 0){
   return -angle;     // negate if in quad III or IV
   }else{
   return angle;
   }
}









void ljp(){
	
	int first_sweep, second_sweep, x_sign, y_sign, police;
	float x_seperation, y_seperation, angle, d, force;

	
	for (first_sweep = 0; first_sweep < population; first_sweep++){
		
				
		for (second_sweep = 0; second_sweep < population; second_sweep++){
			
						
			if ( first_sweep != second_sweep ){
			
				x_seperation = (village[first_sweep].x_position - village[second_sweep].x_position);
				y_seperation = (village[first_sweep].y_position - village[second_sweep].y_position);
				
				if (x_seperation < 0){
					x_sign = - 1; 
				}else{
					x_sign = 1;
				}
				
				if (y_seperation < 0){
					y_sign = - 1;
				}else{
					y_sign = 1;
				}
								
				
				d = SquareRootFloat( (x_seperation * x_seperation) + (y_seperation * y_seperation) ); 
				
				
					
				angle = arctan2( y_seperation, x_seperation );
				//angle = atan( fabs(y_seperation / x_seperation) );
		
	
				force = d*d;
				force = (A / (force * force) ) - (B / force );				
				
				if ( second_sweep == 0 ) total_energy += force;						
				//printf("\nd%f\nljp%f\n", d, actual_ljp);				
				
				village[first_sweep].x_direction += (force * cos(angle) ) / village[first_sweep].mass;
				village[first_sweep].y_direction += (force * sin(angle) ) / village[first_sweep].mass;	
						
				
			}
			
		}
		
		//printf("\n\n%f\n%f\n\n", x_seperation, y_seperation);			
				
		
		
		//printf("%f\n", village[first_sweep].x_direction );
		
		police = 3;
		if (village[first_sweep].x_direction < -police || village[first_sweep].x_direction > police){
			village[first_sweep].x_direction = my_random(SPEED * 2) - SPEED;
		}
		
		if (village[first_sweep].y_direction < -police || village[first_sweep].y_direction > police ){
			village[first_sweep].y_direction = my_random(SPEED * 2) - SPEED;
		}
	
	}
	
}
