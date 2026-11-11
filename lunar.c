#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

const double gravitational_acceleration = 1.6;
const double engine_force = 4500;
const double starting_height = 500;
const double starting_velocity = 10;
const double tick_duration = 0.1 ; // in seconds 
const double mass = 2000; //declared as float in order not to lose precision while dividing.
const char output_csv_filename[] = "RocketLandingRaport.csv";
// szkoda ze nie bylo opcji zrobienia tego w ruscie :(
int main(int argc, char *argv[]) {
    double current_height = starting_height;
    double current_velocity = starting_velocity;
    double current_time = 0;
    bool engine_status = false;

    const char *filename = (argc > 1) ? argv[1] : output_csv_filename;

    FILE *fpt;
    fpt = fopen(filename, "w+");
    fprintf(fpt, "Czas,Wysokość,Prędkość,Stan_silnika\n");

    while (current_height > 0) {
    
        double decceleration_with_engine = engine_force/mass - gravitational_acceleration;
        double tick_distance_margin = current_velocity * tick_duration;
        double distance_needed_to_slowdown = (current_velocity*current_velocity) / (2 * decceleration_with_engine) + tick_distance_margin;

        if ((current_height) <= distance_needed_to_slowdown) {
            engine_status = true; 
        } else {
            engine_status = false;
        }

        printf("H: %.1fm, V=%.1fm/s, %s\n", current_height, current_velocity, engine_status ? "ON" : "OFF");
        fprintf(fpt, "%.1f,%.1f,%.1f,%d\n", current_time, current_height, current_velocity, engine_status ? 1 : 0);

        if (engine_status) {
            current_velocity -= ((engine_force/mass) - gravitational_acceleration) * tick_duration;
        } else {
            current_velocity += gravitational_acceleration * tick_duration;
        };

        current_height -= current_velocity * tick_duration;

        usleep(tick_duration * 1000000);
        current_time += tick_duration;
    }

    current_height = current_height > 0 ? current_height : 0;
    printf("H: %.1fm, V=%.15fm/s, %s\n", current_height, current_velocity, engine_status ? "ON" : "OFF");
    // dla danych wyjsciowych zatrzymuje sie na jakies predkosci absurdalnie blisko 1 wiec zeby zobaczyc ze jest to faktycznie ponizej 1 uzylem .15f (tylko 15 miejsc ma znaczenie dla double) lol
    printf(current_velocity >= 1 ? "Crashed...\n" : "Survived!\n");

    fflush(stdout);
    fclose(fpt);
}

