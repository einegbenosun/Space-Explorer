#include "space_explorer.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Define a structure to store the state
typedef struct {
    unsigned int *visited_planets;
    int visited_count;
    int visited_capacity;
} SpaceState;

// Function to initialize the space state
static SpaceState *initialize_space_state() {
    SpaceState *state = malloc(sizeof(SpaceState));
    if (state == NULL){
        fprintf(stderr,"Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    state->visited_planets = NULL;
    state->visited_count = 0;
    state->visited_capacity=0;

    return state;

}

// Function to free the space state
static void free_space_state(SpaceState *state) {
    if (state->visited_planets != NULL){
        free(state->visited_planets);
    }
    free(state);
}

// Function to add a planet to the visited list
static void add_to_visited(unsigned int planet_id, SpaceState *state) {
    int resize = 5;
    if (state->visited_count >= state->visited_capacity){
        state->visited_capacity += resize;
        state->visited_planets = realloc(state->visited_planets, state->visited_capacity * sizeof(unsigned int));
        if (state->visited_planets == NULL){
            fprintf(stderr,"Failed to Allocate Memory");
            exit(EXIT_FAILURE);
        }
    
    }

    state->visited_planets[state->visited_count++] = planet_id;
}

// Function to check if a planet has been visited
static bool is_visited(unsigned int planet_id, const SpaceState *state) {
    for (int i = 0; i < state->visited_count; i++){
        if (state->visited_planets[i]== planet_id){
            return true;
        }
    }
    return false;
}

// Function to choose the next planet to jump to
static unsigned int choose_next_planet(unsigned int *connections, int num_connections, const SpaceState *state, unsigned int current_planet) {

    // Check if there are unvisited planets reachable from the current planet
    int unvisited_count = 0;
    unsigned int unvisited[num_connections];
    for (int i = 0; i < num_connections; ++i) {
        if (!is_visited(connections[i], state)) {
            unvisited[unvisited_count++] = connections[i];
        }
    }

    // If there are unvisited planets, randomly choose one of them
    if (unvisited_count > 0) {
        return unvisited[rand() % unvisited_count];
    }
    
    //backtracks to last planet
    else{
        if (state->visited_count > 0){
            return state->visited_planets[state->visited_count -1];
        }
        else{
            return current_planet;
        }
    }

}


ShipAction space_hop(unsigned int crt_planet,
                     unsigned int *connections,
                     int num_connections,
                     double distance_from_mixer,
                     void *ship_state)
{

    printf("current planet %d current distance %0.2f\n", crt_planet, distance_from_mixer);
    SpaceState *state = (SpaceState *)ship_state;
    if (ship_state == NULL) {
        state = initialize_space_state();     
    }

    add_to_visited(crt_planet, state);

    unsigned int next_planet = choose_next_planet(connections, num_connections,
                                                    (SpaceState *)state, crt_planet);


    ShipAction action;
    action.next_planet = next_planet;
    action.ship_state = state;
    return action;

}