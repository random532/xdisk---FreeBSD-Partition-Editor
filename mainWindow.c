/* includes and definitions */
#include "disk.h"


void top_window() {
		/* main window */
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), "xdisk");
	gtk_window_set_position (GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}
void scrolled() {
	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                                    GTK_POLICY_ALWAYS, GTK_POLICY_AUTOMATIC);
	 gtk_widget_set_size_request (scrolled_window, 570, 350); /* width, height */
	 
	 gtk_box_pack_end(GTK_BOX(fixed), scrolled_window, TRUE, TRUE, 0);
}

void main_combo() {
	combo_toplevel = gtk_combo_box_text_new();
		
	/* options*/
	gtk_widget_set_focus_on_click(combo_toplevel, TRUE);
	/* location */
	gtk_grid_attach(GTK_GRID (grid), GTK_WIDGET (combo_toplevel), 0, 0, 1, 1);
	/* signals */
	g_signal_connect (combo_toplevel, "changed", G_CALLBACK (on_toplevel_changed), NULL);

	/* add entries to the box */
	toplevel_entries();
}

int main(int argc, char *argv[]) {
	
	/* default settings */
	int lang = LANG_EN; /* default language */
	confirm_yn=0;	/* ask for confirmation */
	if(fontsize[0] == '\0') /* initialize font size*/
		strncpy(fontsize, "11", 2);
	
	/* parse command line options */
	int opt;
	long fsize;
	
	while((opt = getopt(argc, argv, ":l:f:c")) != -1){
      switch(opt){
		 case 'c':
			confirm_yn=1;
			break;
         case 'l':
			if (strncmp(optarg, "de", 2) == 0 )
				lang = LANG_DE;
			else if (strncmp(optarg, "en", 2) == 0 )
				lang = LANG_EN;
			break;
		case 'f':
			fsize = strtol(optarg, NULL, 0);
			if( (fsize<15) && (fsize>7) )
				sprintf(fontsize,"%ld", fsize);	
			else
				printf("invalid font size. Going back to default size.\n");
			break;
          }
	}
	/* set language */
	if(lang == LANG_DE)
		de_lang();
	else
		en_lang();

	/* gtk */
	gtk_init(&argc, &argv);
	
	/* main window */
	top_window();
	
	/* a box */
	fixed = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	gtk_container_add (GTK_CONTAINER (window), fixed);

	/* menu bar */
	add_menubar();

	/*a window with scrollbars */
	scrolled();	

	/* a grid */
	grid = gtk_grid_new();
	gtk_grid_insert_column(GTK_GRID(grid), 3);
	gtk_box_pack_end(GTK_BOX(fixed), grid, TRUE, TRUE, 0);
	gtk_grid_set_column_homogeneous(GTK_GRID(grid), FALSE);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 6);

	/* a combo box that lets users chose whether to display */
	/* an overview of all disks or contents of a specific disk */
	main_combo();

	/* refresh button */
	b = gtk_button_new_with_label(mrefresh);
	gtk_grid_attach(GTK_GRID (grid), GTK_WIDGET (b), 1, 0, 1, 1);
	g_signal_connect (b, "clicked", G_CALLBACK (redraw_cb), NULL);

	GtkWidget *e = gtk_button_new_with_label("Editor");
	gtk_grid_attach(GTK_GRID (grid), GTK_WIDGET (e), 2, 0, 1, 1);
	g_signal_connect (e, "clicked", G_CALLBACK (editor_cb), NULL);

	/* populate scrolled window once */
	gtk_combo_box_set_active( GTK_COMBO_BOX (combo_toplevel), 0);
	
	/*  let the user see everything */
	gtk_widget_show_all(window);
	
	/* and go! */
	gtk_main();
	return EXIT_SUCCESS;
}
