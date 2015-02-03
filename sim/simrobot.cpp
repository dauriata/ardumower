#include "simrobot.h"
#include "world.h"
#include "particles.h"
#include <exception>
#include <stdio.h>

using namespace std;

float random(){
  return (float)rand()/(float)(RAND_MAX);
}

/*
 * Returns random number in normal distribution centering on 0.
 * ~95% of numbers returned should fall between -2 and 2
 */
float gaussRandom() {
    float u = fmod( 2*random(), 1)-1;
    float v = 2*random()-1;
    float r = u*u + v*v;
    /*if outside interval [0,1] start over*/
    if(r == 0 || r > 1) return gaussRandom();

    float c = sqrt(-2*log(r)/r);
    return u*c;

    /* todo: optimize this algorithm by caching (v*c)
     * and returning next time gaussRandom() is called.
     * left out for simplicity */
}

/*
 * Returns member of set with a given mean and standard deviation
 * mean: mean
 * standard deviation: std_dev
 */
float gauss(float mean, float std_dev){
    return mean + (gaussRandom()*std_dev);
}

// calculates the probability of x for 1-dim Gaussian with mean mu and var. sigma
float gaussian(float mu, float sigma, float x)
{
  return exp(-   pow(mu - x, 2) / pow(sigma,  2) / 2.0     )
              / sqrt(2.0 * M_PI * pow(sigma, 2));
}


SimRobot::SimRobot(float length){
  time_t t;
  time(&t);
  srand((unsigned int)t);

  state = STATE_FORW;
  stateTime = 0;
  speed = 0;
  length = 10;
  steering_noise    = 0.0;
  distance_noise    = 0.0;
  measurement_noise = 0.0;
}


void SimRobot::set(float new_x, float new_y, float new_orientation){
  x = new_x;
  y = new_y;
  orientation = new_orientation;
}

void SimRobot::set_noise(float new_s_noise, float new_d_noise, float new_m_noise){
  steering_noise    = new_s_noise;
  distance_noise    = new_d_noise;
  measurement_noise = new_m_noise;
}

SimRobot SimRobot::move(World &world, float steering, float distance,
             float tolerance,  float max_steering_angle){

}


float SimRobot::measurement_prob(vector <float>measurement){
}

void SimRobot::run(World &world, float timeStep){
  switch (state) {
    case STATE_FORW: speed = 40; break;
    case STATE_REV:
           speed = -40;
           if (stateTime > 1.0){
             printf("ROLL\n");
             state=STATE_ROLL;
             stateTime=0;
           }
           break;
    case STATE_ROLL:
            speed=40;
            orientation += M_PI/32;
            if (orientation > 2*M_PI) orientation -= 2*M_PI;
            if (orientation < -2*M_PI) orientation += 2*M_PI;
            if (stateTime > 1.0){
              printf("FORW\n");
              state=STATE_FORW;
              stateTime=0;
            }
            break;
  }
  int px = (int)x;
  int py = (int)y;

  bfieldStrength = world.getBfield(px, py);
  //printf("b=%3.4f\n", b);
  if (bfieldStrength < 0){
    printf("REV\n");
    state=STATE_REV;
    stateTime=0;
  }
  stateTime += timeStep;
}

void SimRobot::draw(Mat &img, bool drawAsFilter){
  if (drawAsFilter) {
    circle( img, Point( x, y), length, Scalar( 255, 255, 255), 2, 8 );
    line( img, Point(x, y), Point(x + length * cos(orientation), y + length * sin(orientation)), Scalar(255,255,255), 2, 8);
  } else {
    circle( img, Point( x, y), length, Scalar( 0, 0, 0, 0.1 ), 2, 8 );
    line( img, Point(x, y), Point(x + length * cos(orientation), y + length * sin(orientation)), Scalar(0,0,0), 2, 8);
  }
}





