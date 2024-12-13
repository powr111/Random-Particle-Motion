/*
 * Program description: This program simulates a particle's path over a lattic, for which it's can't return to node's it has already visited
 * 
 */

# include <stdio.h>
# include <math.h>
# include <stdlib.h> //used for the random function

# define PROB_CONV 0.001 // converts numbers into probabilities
# define HUNDRED 100

// Function prototypes
float Decide(float prob, float event, float motion);
float prob(float Diffusion, float delta_time, float delta_pos);
int check_visited(float current[], float visited[][3], int steps,int Dimenstions);
int Run(int lap, float Diffusion_Const, int Dimensions, float T, int Steps_Number,float x, float y, float z);

int main(void){
    int Dim, Steps_Number;
    const int Runs = 10000;
    float Diff_Const, Time, delta_x, delta_y, delta_z;

    printf("provide the requried number of steps.\n");
    scanf("%d",&Steps_Number);

    printf("provide the requried diffusion constant.\n");
    scanf("%f",&Diff_Const);
    
    printf("Enter the number of dimensions, 1-3D\n");
    scanf("%d", &Dim);
    
    if (Dim>3 || Dim<1){
        printf("These aren't acceptable dimensions");
        return 0;
    }

    printf("Provide the node spacing for the lattice the particle will move through, for each Cartesian coordinate, x:\n");
    scanf("%f", &delta_x);

    if (Dim  >= 2){
        printf("Provide the node spacing for the lattice the particle will move through, for each Cartesian coordinate, y:\n");
        scanf("%f", &delta_y);
    }

    if (Dim == 3){
        printf("Provide the node spacing for the lattice the particle will move through, for each Cartesian coordinate, z:\n");
        scanf("%f", &delta_z);
    }

    printf("provide a value for the time the particle will be given to move.\n");
    scanf("%f", &Time);
    if (Time<=0){
        printf("As far as physics is concerned only positive values are allowed!");
        return 0;
    }

    float delta_T= Time/Runs;
    float delta_Diff = Diff_Const/Runs;

    // loop for the number of runs specified
    for(int j=0; j<Runs; j++){
        
        Run(j, delta_Diff, Dim, delta_T, Steps_Number, delta_x, delta_y, delta_z);
        delta_T += Time/Runs;
        delta_Diff += Diff_Const/Runs;
    }

}

int Run(int lap, float Diffusion_Const, int Dimensions, float T, int Steps_Number, float x, float y, float z){
    float probability_x, probability_y, probability_z, delta_t, motion_1, motion_2, motion_3, current_time=0;

    delta_t = T/Steps_Number;

    if(Dimensions==3){

        char file_name[200];
        snprintf(file_name, sizeof(file_name), "C:\\Users\\alloh\\OneDrive\\Desktop\\coding\\workspace\\revision\\C_files\\project\\output\\Data\\particle_motion_%d.csv", lap);
        printf("%s",file_name);
        FILE *file = fopen(file_name, "w"); // Opens file in write mode

        if (file == NULL) {
            printf("Can't create the desired file\n");
            return -1;
        }

        float current_node[]={0.0,0.0,0.0};

        // Calculate probabilities for motion along each axis
        probability_x = prob(Diffusion_Const, delta_t, x);
        probability_y = prob(Diffusion_Const, delta_t, y);
        probability_z = prob(Diffusion_Const, delta_t, z);

        // Write the header to the file
        fprintf(file, "time,Diffusion_Constant,x_coordinates,y_coordinates,z_coordinates\n");

        // Initialize the current node and visited nodes array
        float visited[Steps_Number][Dimensions];

        for (int i = 0; current_time <= T; i++) {

            // we will check which region of probability this number falls into and from this the outcome will be decided
            float random_x = (float)rand() / RAND_MAX;
            float random_y = (float)rand() / RAND_MAX;
            float random_z = (float)rand() / RAND_MAX;

            // Generates motion for each dimension
            motion_1 = Decide(probability_x, random_x, x);
            motion_2 = Decide(probability_y, random_y, y);
            motion_3 = Decide(probability_z, random_z, z);

            current_node[0] += motion_1;
            current_node[1] += motion_2;
            current_node[2] += motion_3;

            // Checks if the new position has already been visited
            if (i == 0 || !check_visited(current_node, visited, i, Dimensions)) {
                // saves current position to file
                fprintf(file, "%.6f,%f,%f,%f,%f\n", current_time,Diffusion_Const, current_node[0], current_node[1], current_node[2]);

                // Marks as visited
                visited[i][0] = current_node[0];
                visited[i][1] = current_node[1];
                visited[i][2] = current_node[2];
            } else {
                // The node has already been visited, reverts the motion
                current_node[0] -= motion_1;
                current_node[1] -= motion_2;
                current_node[2] -= motion_3;

                // adds the current node to file
                fprintf(file, "%.6f,%f,%f,%f,%f\n", current_time,Diffusion_Const, current_node[0], current_node[1], current_node[2]);
            }


            // Increments time
            current_time += delta_t;
        }

        // Close the file and return success
        printf("Data saved to particle_motion_%d.csv!\n", lap);
        fclose(file);
        return 0;
    }

    else if(Dimensions == 2){

        char file_name[200];
        snprintf(file_name, sizeof(file_name),"C:\\Users\\alloh\\OneDrive\\Desktop\\coding\\workspace\\revision\\C_files\\project\\output\\Data\\particle_motion_%d.csv", lap);
        FILE *file = fopen(file_name, "w"); // Open file in write mode

        if (file == NULL) {
            printf("Can't create the desired file\n");
            return -1;
        }

        float current_node[]={0.0,0.0};

        // Calculate probabilities for motion along each axis
        probability_x = prob(Diffusion_Const, delta_t, x);
        probability_y = prob(Diffusion_Const, delta_t, y);

        // Write the header to the file
        fprintf(file, "time,Diffusion_Constant,x_coordinates,y_coordinates\n");

        // Initialize the current node and visited nodes array
        float visited[Steps_Number][Dimensions];

        for (int i = 0; current_time <= T; i++) {
            // we will check which region of probability this number falls into and from this the outcome will be decided
            float random_x = (float)rand() / RAND_MAX;
            float random_y = (float)rand() / RAND_MAX;

            // Generate motion for each dimension
            motion_1 = Decide(probability_x, random_x, x);
            motion_2 = Decide(probability_y, random_y, y);

            current_node[0] += motion_1;
            current_node[1] += motion_2;

            // Check if the new position has already been visited
            if (i == 0 || !check_visited(current_node, visited, i, Dimensions)) {
                // Log current position to file
                fprintf(file, "%.6f,%f,%f,%f\n", current_time,Diffusion_Const, current_node[0], current_node[1]);

                // Mark this position as visited by storing it
                visited[i][0] = current_node[0];
                visited[i][1] = current_node[1];
            } else {
                // the node has already been visited, revert the motion
                current_node[0] -= motion_1;
                current_node[1] -= motion_2;

                // Logs the current node to file
                fprintf(file,"%.6f,%f,%f,%f\n", current_time,Diffusion_Const, current_node[0], current_node[1]);
            }

            // Increments time
            current_time += delta_t;
        }

        // Closes the file
        printf("Data saved to particle_motion_%d.csv!\n", lap);
        fclose(file);
        return 0;
    }
    else if(Dimensions==1){
        char file_name[200];
        snprintf(file_name, sizeof(file_name),"C:\\Users\\alloh\\OneDrive\\Desktop\\coding\\workspace\\revision\\C_files\\project\\output\\Data\\particle_motion_%d.csv", lap);
        FILE *file = fopen(file_name, "w"); // Open file in write mode

        if (file == NULL) {
            printf("Can't create the desired file\n");
            return -1;
        }
        float current_node[]= {0};

        // Calculate probabilities for motion along each axis
        probability_x = prob(Diffusion_Const, delta_t, x);


        // Write the header to the file
        fprintf(file, "time,Diffusion_Constant,x_coordinates\n");

        // Initialize the current node and visited nodes array
        float visited[Steps_Number][Dimensions]; 

        for (int i = 0; current_time <= T; i++) {
            // we will check which region of probability this number falls into and from this the outcome will be decided
            float random_x = (float)rand() / RAND_MAX;

            // motion for each dimension
            motion_1 = Decide(probability_x, random_x, x);


            // current node position
            current_node[0] += motion_1;

            // Check if the new position has already been visited
            if (i == 0 || !check_visited(current_node, visited, i, Dimensions)) {
                // logs current position to file
                fprintf(file, "%.6f,%f,%f\n", current_time,Diffusion_Const, current_node[0]);

                // saves position as visited
                visited[i][0] = current_node[0];
            } else {
                // reverts the motion
                current_node[0] -= motion_1;

                // adds the current node  to file
                fprintf(file, "%6f,%f,%f\n", current_time,Diffusion_Const, current_node[0]);
            }


            // Increments time
            current_time += delta_t;
        }

        // closes the file and return success
        printf("Data saved to particle_motion_%d.csv!\n", lap);
        fclose(file);
        return 0;
    }
    else{
        printf("this isn't the correct number of dimenstions, please provide and integer number between 1 and 3");
        return -1;
    }
}

// this will make the decision of what will happen for the particle each time
float Decide(float prob, float event, float move){ // we pass delta_x as move in the case of 1D
    if (event<prob){
        return move;
    }
    else if (event<(1-prob*2)){ // due to the if statement previously, no check of if event > probability is needed
       return 0;
    }

    else{ // due to the nature of the code no calc is needed here
        return -move;
    }
}

// checks if the particle has visited a specific node 
int check_visited(float current[], float visited[][3], int steps, int Dimensions){
    // we use sigma to provide a range of values which are considered the same node.
    float sigma = 0.2;
    for (int i = 0; i < steps; i++) {
        if (Dimensions==3){
            if (fabs(current[0] - visited[i][0]) <= sigma &&
                fabs(current[1] - visited[i][1]) <= sigma &&
                fabs(current[2] - visited[i][2]) <= sigma){
                return 1; //  visited
        }
        }
        else if (Dimensions == 2){
            if (fabs(current[0] - visited[i][0]) <= sigma &&
                fabs(current[1] - visited[i][1]) <= sigma){
                return 1; // visited
        }
        }
        else{
            if (fabs(current[0] - visited[i][0]) <= sigma){
                return 1; // visited
            }
        }
        }
    return 0; // not visited
}

float prob(float Diffusion, float delta_time, float delta_pos){
    return Diffusion*delta_time/pow(delta_pos,2); //  calculation for the probability
}