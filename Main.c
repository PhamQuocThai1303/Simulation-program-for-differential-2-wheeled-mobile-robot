#include<stdio.h>
#include<math.h>
const double pi = 3.14159265358979323846;

typedef struct locate{
    double x;
    double y;
    double theta;
} Locate;

typedef struct robot_status{
    double Length;
    double Radius;
} Robot_status;

typedef struct move{
    double wL;
    double wR;
    double time;
} Move;

double getW(Move move,Robot_status robot){

    double W = (move.wR - move.wL) * (robot.Radius/robot.Length); //find W

    return W;
}

double getV(Move move,Robot_status robot){

    double V = (move.wR + move.wL) * (robot.Radius/2); //find V

    return V;
}

Locate updLocation(Locate current,Move move,Robot_status robot){
    Locate newLocation;

    //Kinematic model/equation
    double W = (move.wR - move.wL) * (robot.Radius/robot.Length); //find W
    double V = (move.wR + move.wL) * (robot.Radius/2); //find V

    //Discrete kinematic equation
    newLocation.x = current.x + V * move.time * cos(current.theta); //update new location of X
    newLocation.y = current.y + V * move.time * sin(current.theta); //upd new location if Y
    newLocation.theta=current.theta + W * move.time; //upd new theta for next move

    printf("x: %lf    y: %lf\n\n",newLocation.x, newLocation.y);

    return newLocation;
}

int main(){
    Robot_status robot;
    robot.Length=20; //length of 2 wheels
    robot.Radius=2.5; // radius of wheel

    //Locate curLocation;

    //modified move guide
    FILE *move_file;
    move_file=fopen("input.txt","r"); //include wL, wR, time
    Move action[10000];

    int turn=0; //number actions of robot

    //read infor of wL, wR, time from file input
    while(fscanf(move_file,"%lf %lf %lf\n",&action[turn].wL, &action[turn].wR, &action[turn].time) != EOF){
        turn++;
    }

    //start position on xOY
    Locate coordinates[1000];
    coordinates[0].x= 0;
    coordinates[0].y= 0;
    coordinates[0].theta= 0;

    //read file x,y,theta
    FILE *fx; fx= fopen("x.txt", "w");
    FILE *fy; fy= fopen("y.txt", "w");
    FILE *ftheta; ftheta= fopen("theta.txt", "w");
    FILE *fW; fW= fopen("W.txt", "w");
    FILE *fV; fV= fopen("V.txt", "w");

    //open file and reset file for app
    fprintf(fx, "x=[0\n");
    fprintf(fy, "y=[0\n");
    fprintf(ftheta,"theta=[0\n");
    fprintf(fW, "W=[0\n");
    fprintf(fV, "V=[0\n");


    for(int i=1; i<turn; i++){
        printf("\n---------turn: %d----------\n",i);

        //calculate present location and write in file x,y,theta
        Locate curLocation= updLocation(coordinates[i-1], action[i-1], robot); //start with 0 index and use before information
        //calcylate present W and V
        double W= getW(action[i-1], robot);
        double V= getV(action[i-1], robot);

        //write infor to file
        fprintf(fx, ",%lf\n", curLocation.x);
        fprintf(fy, ",%lf\n", curLocation.y);
        fprintf(ftheta, ",%lf\n", curLocation.theta);
        fprintf(fW, ",%lf\n", W);
        fprintf(fV, ",%lf\n", V);

        //write location again for next turn
        coordinates[i].x= curLocation.x;
        coordinates[i].y= curLocation.y;
        coordinates[i].theta= curLocation.theta;
    }

    //close infor
    fprintf(fx, "];");
    fprintf(fy, "];");
    fprintf(ftheta, "];");
    fprintf(fW, "];");
    fprintf(fV, "];");

    //close file
    fclose(fx);
    fclose(fy);
    fclose(ftheta);
    fclose(fW);
    fclose(fV);

    return 0;
}
