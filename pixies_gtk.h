//All the window setup stuff is in this file..............	
	
static void activate_action (GtkAction *action){
  g_message ("Action \"%s\" activated", gtk_action_get_name (action));
}

static void activate_radio_action (GtkAction *action, GtkRadioAction *current){
  g_message ("Radio action \"%s\" selected", gtk_action_get_name (GTK_ACTION (current)));
}

static GtkActionEntry entries[] = {
  { "FileMenu", NULL, "_File" },               /* name, stock id, label */
  { "PreferencesMenu", NULL, "_Preferences" }, /* name, stock id, label */
  { "ColorMenu", NULL, "_Color"  },            /* name, stock id, label */
  { "ShapeMenu", NULL, "_Shape" },             /* name, stock id, label */
  { "HelpMenu", NULL, "_Help" },               /* name, stock id, label */
  { "New", GTK_STOCK_NEW,                      /* name, stock id */
    "_New", "<control>N",                      /* label, accelerator */
    "Create a new file",                       /* tooltip */ 
    G_CALLBACK (activate_action) },      
  { "Open", GTK_STOCK_OPEN,                    /* name, stock id */
    "_Open","<control>O",                      /* label, accelerator */     
    "Open a file",                             /* tooltip */
    G_CALLBACK (activate_action) }, 
  { "Save", GTK_STOCK_SAVE,                    /* name, stock id */
    "_Save","<control>S",                      /* label, accelerator */     
    "Save current file",                       /* tooltip */
    G_CALLBACK (activate_action) },
  { "SaveAs", GTK_STOCK_SAVE,                  /* name, stock id */
    "Save _As...", NULL,                       /* label, accelerator */     
    "Save to a file",                          /* tooltip */
    G_CALLBACK (activate_action) },
  { "Quit", GTK_STOCK_QUIT,                    /* name, stock id */
    "_Quit", "<control>Q",                     /* label, accelerator */     
    "Quit",                                    /* tooltip */
    G_CALLBACK (activate_action) },
  { "About", NULL,                             /* name, stock id */
    "_About", "<control>A",                    /* label, accelerator */     
    "About",                                   /* tooltip */  
    G_CALLBACK (activate_action) },
  { "Logo", "demo-gtk-logo",                   /* name, stock id */
     NULL, NULL,                               /* label, accelerator */     
    "GTK+",                                    /* tooltip */
    G_CALLBACK (activate_action) },
};
static guint n_entries = G_N_ELEMENTS (entries);


static GtkToggleActionEntry toggle_entries[] = {
  { "Bold", GTK_STOCK_BOLD,                    /* name, stock id */
     "_Bold", "<control>B",                    /* label, accelerator */     
    "Bold",                                    /* tooltip */
    G_CALLBACK (activate_action), 
    TRUE },                                    /* is_active */
};
static guint n_toggle_entries = G_N_ELEMENTS (toggle_entries);

enum {
  COLOR_RED,
  COLOR_GREEN,
  COLOR_BLUE
};

static GtkRadioActionEntry color_entries[] = {
  { "Red", NULL,                               /* name, stock id */
    "_Red", "<control>R",                      /* label, accelerator */     
    "Blood", COLOR_RED },                      /* tooltip, value */
  { "Green", NULL,                             /* name, stock id */
    "_Green", "<control>G",                    /* label, accelerator */     
    "Grass", COLOR_GREEN },                    /* tooltip, value */
  { "Blue", NULL,                              /* name, stock id */
    "_Blue", "<control>B",                     /* label, accelerator */     
    "Sky", COLOR_BLUE },                       /* tooltip, value */
};
static guint n_color_entries = G_N_ELEMENTS (color_entries);

enum {
  SHAPE_SQUARE,
  SHAPE_RECTANGLE,
  SHAPE_OVAL
};

static GtkRadioActionEntry shape_entries[] = {
  { "Square", NULL,                            /* name, stock id */
    "_Square", "<control>S",                   /* label, accelerator */     
    "Square",  SHAPE_SQUARE },                 /* tooltip, value */
  { "Rectangle", NULL,                         /* name, stock id */
    "_Rectangle", "<control>R",                /* label, accelerator */     
    "Rectangle", SHAPE_RECTANGLE },            /* tooltip, value */
  { "Oval", NULL,                              /* name, stock id */
    "_Oval", "<control>O",                     /* label, accelerator */     
    "Egg", SHAPE_OVAL },                       /* tooltip, value */  
};
static guint n_shape_entries = G_N_ELEMENTS (shape_entries);

static const gchar *ui_info = 
"<ui>"
"  <menubar name='MenuBar'>"
"    <menu action='FileMenu'>"
"      <menuitem action='New'/>"
"      <menuitem action='Open'/>"
"      <menuitem action='Save'/>"
"      <menuitem action='SaveAs'/>"
"      <separator/>"
"      <menuitem action='Quit'/>"
"    </menu>"
"    <menu action='PreferencesMenu'>"
"      <menu action='ColorMenu'>"
"	<menuitem action='Red'/>"
"	<menuitem action='Green'/>"
"	<menuitem action='Blue'/>"
"      </menu>"
"      <menu action='ShapeMenu'>"
"        <menuitem action='Square'/>"
"        <menuitem action='Rectangle'/>"
"        <menuitem action='Oval'/>"
"      </menu>"
"      <menuitem action='Bold'/>"
"    </menu>"
"    <menu action='HelpMenu'>"
"      <menuitem action='About'/>"
"    </menu>"
"  </menubar>"
"  <toolbar  name='ToolBar'>"
"    <toolitem action='Open'/>"
"    <toolitem action='Quit'/>"
"    <separator action='Sep1'/>"
"    <toolitem action='Logo'/>"
"  </toolbar>"
"</ui>";

     
      


GtkWidget* initialise_gtk( int argc, char *argv[] ){
	
	GtkWidget *box1, *da;
    GtkWidget *box2;
    GtkWidget *separator;
    GtkUIManager *ui;
    GtkActionGroup *actions;
    GError *error = NULL;
	
	GtkWidget *window;
	
    GtkWidget *vbox, 
			  *IP_vbox, 
			  *hbox, 
			  *frame, 
			  *IP_frame, 
			  *rec_hbox, 
			  *rec_vbox,
			  *env_rec_vbox,
			  *adj_kill_hbox;
			  
	GtkWidget *opt, *menu, *item;
    
    GtkWidget *button;
    GtkWidget *check;
    
    GtkWidget *label;
	gpointer *data;
	GtkAdjustment *adj;
	
    gtk_init (&argc, &argv);

    //CONTROL CENTRE WINDOW
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_usize( GTK_WIDGET (window), 500, 550);
    gtk_window_set_title(GTK_WINDOW (window), "Pixie Control Centre");
    gtk_signal_connect(GTK_OBJECT (window), "delete_event", (GtkSignalFunc) gtk_exit, NULL);
	
			 
		//**MENU BAR AREA**
		actions = gtk_action_group_new ("Actions");
		gtk_action_group_add_actions (actions, entries, n_entries, NULL);
		gtk_action_group_add_toggle_actions (actions, 
						   toggle_entries, n_toggle_entries, 
						   NULL);
		gtk_action_group_add_radio_actions (actions, 
						  color_entries, n_color_entries, 
						  COLOR_RED,
						  G_CALLBACK (activate_radio_action), 
						  NULL);
		gtk_action_group_add_radio_actions (actions, 
						  shape_entries, n_shape_entries, 
						  SHAPE_OVAL,
						  G_CALLBACK (activate_radio_action), 
						  NULL);

		ui = gtk_ui_manager_new ();
		gtk_ui_manager_insert_action_group (ui, actions, 0);
		gtk_window_add_accel_group (GTK_WINDOW (window), 
					  gtk_ui_manager_get_accel_group (ui));
		gtk_container_set_border_width (GTK_CONTAINER (window), 0);
		  
		if (!gtk_ui_manager_add_ui_from_string (ui, ui_info, -1, &error))
			{
			  g_message ("building menus failed: %s", error->message);
			  g_error_free (error);
			}

		box1 = gtk_vbox_new (FALSE, 0);
		gtk_container_add (GTK_CONTAINER (window), box1);
		  
			gtk_box_pack_start (GTK_BOX (box1),
					  gtk_ui_manager_get_widget (ui, "/MenuBar"),
					  FALSE, FALSE, 0);

		separator = gtk_hseparator_new ();
		gtk_box_pack_start (GTK_BOX (box1), separator, FALSE, TRUE, 0);
		
		
		box2 = gtk_hbox_new (FALSE, 10);
		gtk_container_set_border_width(box2, 0);
		gtk_box_pack_start(GTK_CONTAINER (box1), box2, TRUE, TRUE, 0);
			
			env_rec_vbox = gtk_vbox_new(FALSE, 10);
			gtk_container_set_border_width(env_rec_vbox, 5);
			gtk_container_add (GTK_CONTAINER (box2), env_rec_vbox);
			
			
			//**ENVIRONMENT AREA**
			frame = gtk_frame_new ("Environment");
			gtk_box_pack_start (GTK_BOX (env_rec_vbox), frame, FALSE, FALSE, 10);
			
			vbox = gtk_vbox_new (FALSE, 10);
			gtk_container_set_border_width (GTK_CONTAINER (vbox), 10);
			gtk_container_add (GTK_CONTAINER (frame), vbox);
				
				//Pop/kill hbox
				GtkWidget *pop_kill_hbox;
			    pop_kill_hbox = gtk_hbox_new (FALSE, 10);
			    gtk_container_add (GTK_CONTAINER (vbox), pop_kill_hbox);
				
					//Population Label		
					population_label = gtk_label_new ("Population: 0");
					gtk_misc_set_alignment (GTK_MISC (population_label), 0, 0);
					gtk_box_pack_start (GTK_BOX (pop_kill_hbox), population_label, TRUE, TRUE, 0);											 
				
					//Kill All button
					button = gtk_button_new_with_label ("Kill All");
					gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
								   GTK_SIGNAL_FUNC(killall),
								   GTK_OBJECT (window));
					gtk_box_pack_start (GTK_BOX (pop_kill_hbox), button, FALSE, TRUE, 0);
					
					
				//Trails radio button
				check = gtk_check_button_new_with_label("Trails");
				gtk_box_pack_start (GTK_BOX (vbox), check, FALSE, TRUE, 0);
				gtk_signal_connect (GTK_OBJECT(check), "toggled", GTK_SIGNAL_FUNC(trails_switcher), data);
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check), FALSE);
				
				//Gravity Slider ---------------------------------
				label = gtk_label_new ("Gravity:");
				gtk_misc_set_alignment (GTK_MISC (label), 0, 0);
				gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);
				
					
					gravity_slider = gtk_hscale_new_with_range(0, 0.1, 0.00001);
					gtk_scale_set_value_pos(gravity_slider, GTK_POS_RIGHT);
					gtk_scale_set_digits(gravity_slider, 5);
					gtk_box_pack_start (GTK_BOX (vbox), gravity_slider, FALSE, TRUE, 0);
					
									
				//FPS Slider--------------------------------------------
				label = gtk_label_new ("Frame Rate:");
				gtk_misc_set_alignment (GTK_MISC (label), 0, 0);
				gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, TRUE, 0);
				
					FPS_slider = gtk_hscale_new_with_range(1, 100, 1);
					gtk_scale_set_value_pos(FPS_slider, GTK_POS_RIGHT);
					gtk_range_set_value(FPS_slider, 10);
					gtk_box_pack_start (GTK_BOX (vbox), FPS_slider, FALSE, TRUE, 0);
			
				
				//Slomo Slider--------------------------------------------
				label = gtk_label_new ("Slow Motion:");
				gtk_misc_set_alignment (GTK_MISC (label), 0, 0);
				gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, TRUE, 0);
				
					slomo_slider = gtk_hscale_new_with_range(0, 100, 1);
					gtk_scale_set_value_pos(slomo_slider, GTK_POS_RIGHT);
					gtk_box_pack_start (GTK_BOX (vbox), slomo_slider, FALSE, TRUE, 0);
					
				//Start Stop Simulation button
				button = gtk_button_new_with_label ("Start/Stop Simulation");
				gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
							   GTK_SIGNAL_FUNC(start_stop),
							   GTK_OBJECT (window));
				gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, TRUE, 10);
				
			//**RECORDING AREA**
			frame = gtk_frame_new ("Recording");
			gtk_box_pack_start (GTK_BOX (env_rec_vbox), frame, FALSE, FALSE, 10);
			
			rec_vbox = gtk_vbox_new (FALSE, 10);
			gtk_container_set_border_width (GTK_CONTAINER (rec_vbox), 10);
			gtk_container_add (GTK_CONTAINER (frame), rec_vbox);
				
				//Cycles Recorded Label		
				cycles_recorded_label = gtk_label_new ("Cycles Recorded: 0");
				gtk_misc_set_alignment (GTK_MISC (cycles_recorded_label), 0, 0.5);
				gtk_box_pack_start (GTK_BOX (rec_vbox), cycles_recorded_label, FALSE, FALSE, 10);
				
				//Hbox
				rec_hbox = gtk_hbox_new (FALSE, 10);
				gtk_container_set_border_width(rec_hbox, 5);
				gtk_container_add (GTK_CONTAINER (rec_vbox), rec_hbox);
				
				//Record button
				button = gtk_button_new_with_label ("Record");
				gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
							   GTK_SIGNAL_FUNC(record),
							   GTK_OBJECT (window));
				gtk_box_pack_start (GTK_BOX (rec_hbox), button, FALSE, TRUE, 10);
				
				//Reset Recording button			
				button = gtk_button_new_with_label ("Reset");
				gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
							   GTK_SIGNAL_FUNC(record_reset),
							   GTK_OBJECT (window));
				gtk_box_pack_end (GTK_BOX (rec_hbox), button, FALSE, TRUE, 10);
				
				
				
			//**INDIVIDUAL PIXIES AREA**
			IP_frame = gtk_frame_new ("Individual Pixies");
			gtk_box_pack_start (GTK_BOX (box2), IP_frame, FALSE, FALSE, 10);
			
			IP_vbox = gtk_vbox_new (FALSE, 10);
			gtk_container_set_border_width (GTK_CONTAINER (IP_vbox), 10);
			gtk_container_add (GTK_CONTAINER (IP_frame), IP_vbox);
				
				//User Mode Button
				opt = gtk_option_menu_new ();
				menu = gtk_menu_new ();
			  
					
					item = gtk_menu_item_new_with_label ("Creation Mode");
					g_signal_connect (G_OBJECT (item), "activate", user_mode_switcher, (gpointer) data);
					gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
					
					item = gtk_menu_item_new_with_label ("Selector Mode");
					g_signal_connect (G_OBJECT (item), "activate", user_mode_switcher, (gpointer) data);
					gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
					
					gtk_option_menu_set_menu (GTK_OPTION_MENU (opt), menu);
					gtk_box_pack_start (GTK_BOX (IP_vbox), opt, FALSE, FALSE, 0);
					
						selected_pixie_label = gtk_label_new (".?.");
						gtk_misc_set_alignment (GTK_MISC (selected_pixie_label), 0, 0);
						gtk_box_pack_start (GTK_BOX (IP_vbox), selected_pixie_label, FALSE, TRUE, 0);
					
				
				separator = gtk_hseparator_new ();
				gtk_box_pack_start (GTK_BOX (IP_vbox), separator, FALSE, TRUE, 0);
				
				//Colour Picker Button
				label = gtk_label_new ("Color:");
				gtk_misc_set_alignment (GTK_MISC (label), 0, 0);
				gtk_box_pack_start (GTK_BOX (IP_vbox), label, FALSE, TRUE, 0);
				
				color_button = gtk_color_button_new();
				
					//Color Button's starting colour
					color.red = 65535;
					color.green = 65535;
					color.blue = 65535;
					gtk_color_button_set_color(color_button, &color); 
					gtk_box_pack_start (GTK_BOX (IP_vbox), color_button, FALSE, FALSE, 0);
				
				
				//Cooler/Heater Slider--------------------------------------------
				label = gtk_label_new ("Cooler/Heater:");
				gtk_misc_set_alignment (GTK_MISC (label), 0, 0);
				gtk_box_pack_start (GTK_BOX (IP_vbox), label, FALSE, TRUE, 0);
				
					ch_slider = gtk_hscale_new_with_range(-1, 1, 0.0001);
					gtk_scale_set_value_pos(ch_slider, GTK_POS_RIGHT);
					gtk_range_set_value(ch_slider, 0);
					gtk_scale_set_digits(gravity_slider, 4);
					gtk_box_pack_start (GTK_BOX (IP_vbox), ch_slider, FALSE, TRUE, 0);
														
				
				//Mass Slider--------------------------------------------
				label = gtk_label_new ("Mass:");
				gtk_misc_set_alignment (GTK_MISC (label), 0, 0);
				gtk_box_pack_start (GTK_BOX (IP_vbox), label, FALSE, TRUE, 0);
				
					mass_slider = gtk_hscale_new_with_range(1, 100, 1);
					gtk_scale_set_value_pos(mass_slider, GTK_POS_RIGHT);
					gtk_box_pack_start (GTK_BOX (IP_vbox), mass_slider, FALSE, TRUE, 0);
					
					
				//Size Slider--------------------------------------------
				label = gtk_label_new ("Size:");
				gtk_misc_set_alignment (GTK_MISC (label), 0, 0);
				gtk_box_pack_start (GTK_BOX (IP_vbox), label, FALSE, TRUE, 0);
				
					size_slider = gtk_hscale_new_with_range(1, 100, 1);
					gtk_scale_set_value_pos(size_slider, GTK_POS_RIGHT);
					gtk_box_pack_start (GTK_BOX (IP_vbox), size_slider, FALSE, TRUE, 0);	
					
					
				//Static Position toggle
				static_switch = gtk_check_button_new_with_label("Static");
				gtk_box_pack_start (GTK_BOX (IP_vbox), static_switch, FALSE, TRUE, 10);
				gtk_signal_connect (GTK_OBJECT(static_switch), "toggled", GTK_SIGNAL_FUNC(static_switcher), data);
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(static_switch), FALSE);
				
				//adj/kill hbox
				adj_kill_hbox = gtk_hbox_new (FALSE, 0);
				gtk_box_pack_end(GTK_CONTAINER (IP_vbox), adj_kill_hbox, FALSE, FALSE, 0);
												
					//'Kill Pixie' button		
					button = gtk_button_new_with_label ("Kill Pixie");
					gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
								   GTK_SIGNAL_FUNC(population_decrease),
								   GTK_OBJECT (window));
					gtk_box_pack_start (GTK_BOX (adj_kill_hbox), button, FALSE, FALSE, 10);
					
					//'Adjust' button		
					adjust_button = gtk_button_new_with_label ("Adjust Pixie");
					gtk_signal_connect_object (GTK_OBJECT (adjust_button), "clicked",
								   GTK_SIGNAL_FUNC(adjust_pixie),
								   GTK_OBJECT (window));
					gtk_widget_set_sensitive(adjust_button, FALSE);
					gtk_box_pack_start (GTK_BOX (adj_kill_hbox), adjust_button, FALSE, FALSE, 10);
				
				
										   
				
			gtk_widget_show_all(window);
			
			//Pixie World Window
			GtkWidget *window2;
			window2 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
			gtk_widget_set_usize( GTK_WIDGET (window2), screen_width, screen_height + 50);
			gtk_window_set_title(GTK_WINDOW (window2), "Poo");
			gtk_signal_connect(GTK_OBJECT (window2), "delete_event", (GtkSignalFunc) gtk_exit, NULL);	
				
				vbox = gtk_vbox_new(FALSE, 0);
				gtk_container_add(window2, vbox);
						
				//Drawing Area
				da = gtk_drawing_area_new();
				gtk_widget_set_usize( GTK_WIDGET (da), screen_width, screen_height);
				gtk_container_add(vbox, da);
				
				//Connect some signals and events
				g_signal_connect (da, "expose_event", G_CALLBACK (expose), NULL);
				g_signal_connect (da, "button_press_event", G_CALLBACK (button_press_event), NULL);
				
				//tell Gtk what events to pick up on
				gtk_widget_set_events (da, GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK);
				
				//The off screen canvas, FROM WHICH the screen is updated
				buffer = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, screen_width, screen_height);
				
				hbox = gtk_hbox_new(TRUE, 0);
				gtk_container_add(vbox, hbox);
				
						
				energy_label = gtk_label_new ("Total Energy:");
				gtk_misc_set_alignment (GTK_MISC (label), 0, 0);
				gtk_box_pack_start (hbox, energy_label, FALSE, TRUE, 0);
								
				/* Change default font throughout the widget */
				PangoFontDescription *font_desc;
				font_desc = pango_font_description_from_string ("monospace 10");
				gtk_widget_modify_font (energy_label, font_desc);
				pango_font_description_free (font_desc);
				
				/*For texting onto a drawable, I think..
				layout = gtk_widget_create_pango_layout (widget, text);
				 fontdesc = pango_font_description_from_string ("Luxi Mono 12");
				 pango_layout_set_font_description (layout, fontdesc); 
				 gdk_draw_layout (..., layout);
				 pango_font_description_free (fontdesc);
				 g_object_unref (layout);   
				*/	
			gtk_widget_show_all(window2);
			
			
    
    
    gtk_idle_add(pixies_main, NULL); 
    
   

    
          
   return da;
}


