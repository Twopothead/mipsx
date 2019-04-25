#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
//https://www.gamefromscratch.com/post/2017/11/21/Allegro-Tutorial-Series-Part-6-Graphics-Primitives.aspx
//  g++ a5.cc -lallegro -lallegro_main -lallegro_primitives
int main()
{
	ALLEGRO_DISPLAY * display;

	al_init();
	display = al_create_display(740, 680);
	al_init_primitives_addon();
	float points[8] = {0.0f, 0.0f, 100.00f, 100.00f, 200.00f, 100.00f, 640.00f, 150.00f};

	// float polygon[8] = { 640.0f, 100.0f, 640.0f, 300.0f, 380.0f, 350.0f, 200.0f, 200.0f };
   float polygon[8] = { 0.0f,0.0f,\
                        0.0f,100.0f,\
                        200.0f,100.0f,\
                        200.0f,0.0f };
// https://www.allegro.cc/manual/5/al_map_rgb
// Allegro manual said:
// When the y-axis is facing downwards (the usual), the coordinates must be ordered anti-clockwise.

	bool running = true;
	while (running) {
		// al_draw_line(0, 0, al_get_display_width(display), al_get_display_height(display), al_map_rgb(255, 0, 0),5.0);
		// al_draw_rectangle(100, 100, 300, 300, al_map_rgb(0, 255, 0), 1);
		// al_draw_ellipse(300, 300, 120, 50, al_map_rgb(0, 0, 255), 3);

		// al_draw_spline(points, al_map_rgb(128, 128, 0), 8);

		// al_draw_polygon(polygon, 8, ALLEGRO_LINE_JOIN_BEVEL, al_map_rgb(255, 15, 15),3,1);
		al_draw_filled_polygon(polygon, 8, al_map_rgb(255, 0, 0));
		al_flip_display();
	}

	al_destroy_display(display);
	
	return 0;
}

// #include <allegro5/allegro.h>//adding libraries
// int main()
// {
//    ALLEGRO_DISPLAY *display = NULL;
//    al_init(); //initialising the library
//    display=al_create_display(640,480); //creating a window,
//    //providing its width and height in pixels
//    al_clear_to_color(al_map_rgb(0,100,0));//setting the colour
//    al_flip_display();//displaying the window (the buffer)
//    al_rest(2.0);//waiting 2.0 seconds
//    al_destroy_display(display); //destroying the window
//    return 0;
// }
// #include <stdio.h>
// #include <allegro5/allegro.h>



// int main(int argc, char **argv){
   
//    ALLEGRO_DISPLAY *display = NULL;

//    if(!al_init()) {
//       fprintf(stderr, "failed to initialize allegro!\n");
//       return -1;
//    }

//    display = al_create_display(640, 480);
//    if(!display) {
//       fprintf(stderr, "failed to create display!\n");
//       return -1;
//    }

//    al_clear_to_color(al_map_rgb(0,0,0));
   
//    al_flip_display();

//    al_rest(10.0);

//    al_destroy_display(display);

//    return 0;
// }