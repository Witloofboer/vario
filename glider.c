/*
 * File:   glider.c
 * Author: Cedric
 *
 * Created on 06 March 2017, 22:22
 * 
 * TO DO:
 * ------
 * 
 * - Vliegtuiggegevens toevoegen in passende struct
 * - Systeem voor selecteren van eenheden uitdenken
 * 
 */

//#include "glider.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "glider.h"

/* Error codes */
#define ERR -1			// Generic error
#define SUCCESS 0		// Success
#define AIRCRAFT_TOO_HEAVY 1	//
#define AIRCRAFT_TOO_LIGHT 2	//

/* Buffer size */
#define BUFFER 128
#define STRLENGTH 128
//#define POLAR 64				// 2degraads + 1egraad?

/* Physical constants */
//static const float g = 9.81;	// Gravitational constant
static const float rho = 1.2250;	// Air Density
static const float ft2m = 0.3054;	// 
static const float mps2kph = 3.6;
static const float mps2kts = 1.94384449;
static const float mps2mph = 2.23693629;
static const float sqrt_2 = 1.414;
static const float alti_pow = 0.190284;
static const float alti_mul = 145366.45;
static const float alti_mul_100 = 14536645;
static float rho_sqrt;
static float speed_ct;

/* Error handling */
int err = 0;

/* Flight information */
struct Units {
	int speed;
	int alti;
	int duse;
	int vario;
	int temp;
} units;

struct Weather {
	float QNH;
	float temp;
	/*float wind_dir;
	float wind_vel;*/
} weather;

struct Aircraft {
	float stat;
	float pitot;
	/*float empty_mass;
	float water_mass;
	float pilot_mass;
	float luggage_mass;
	float fuel_mass;
	float total_mass;*/
	float sample_time;
    float battery_voltage;
	/*float dump_rate;
	float energy;*/
} glider;

/*struct Aircraft_type {
	char manufacturer[STRLENGTH];
	char type[STRLENGTH];
	short handicap;
	float MTOW;
	//float polar[POLAR];
} glider_type;*/

struct Dials {
	float speed;
	float alti;
	float vario;
	float temp;
    float fl;
} dials;

struct Aircraft glider;
//struct Aircraft_type glider_type;
struct Weather weather;
struct Dials dials;

//static float energy[BUFFER]; 

int initialise(void) {

    rho_sqrt = sqrt(rho);
    speed_ct = 10*sqrt_2/rho_sqrt;
    
	//int i;

	/*for ( i=0; i<BUFFER; i++) {
		energy[i] = 0;
	}*/

}

float getAlti(int unit) {
    switch (unit) {
        case M: return dials.alti*ft2m;
        case FT: return dials.alti;
        case FL: return dials.fl;
        default: return dials.alti*ft2m;
    }
}

/*float getMass(int unit) {
    switch (unit) {
        case KG: return glider.total_mass;
        case LBS: return glider.total_mass*2.20462262;
        default: return glider.total_mass*0.3054;
    }
}*/

float getSpeed(int unit) {
    switch (unit) {
        case MPS: return dials.speed;
        case KPH: return dials.speed*mps2kph;
        case KTS: return dials.speed*mps2kts;
        case MPH: return dials.speed*mps2mph;
        default: return dials.speed;
    }
}

float getVario(int unit) {
    switch (unit) {
        case MPS: return dials.vario;
        case KPH: return dials.vario*3.6;
        case KTS: return dials.vario;
        case MPH: return dials.vario;
        default: return dials.vario;
    }
}

float getVoltage(void) {
    return glider.battery_voltage;
}

int setQNH(float daily_qnh) {
	weather.QNH = daily_qnh;
	return SUCCESS;
}

int setPitot(float meas_p) {
	glider.pitot = meas_p;
    return SUCCESS;
}

int setStatic(float meas_p) {
	glider.stat = meas_p;
    return SUCCESS;
}

int setVoltage(float battery_voltage) {
    glider.battery_voltage = battery_voltage;
    return SUCCESS;
}

int setSampletime(float time) {
	glider.sample_time = time;
	return SUCCESS;
}

/*int setEmptymass(float mass) {
	if (mass > 950) {
		return AIRCRAFT_TOO_HEAVY;
	}

	if (mass < 100) {
		return AIRCRAFT_TOO_LIGHT;
	}
	
	glider.empty_mass = mass;
	return SUCCESS;
}

int setWatermass(float mass) {
	if (mass > 950) {
		return AIRCRAFT_TOO_HEAVY;
	}

	if (mass < 100) {
		return AIRCRAFT_TOO_LIGHT;
	}
	
	glider.water_mass = mass;
	return SUCCESS;
}

int setPilotmass(float mass) {
	if (mass > 950) {
		return AIRCRAFT_TOO_HEAVY;
	}

	if (mass < 100) {
		return AIRCRAFT_TOO_LIGHT;
	}
	
	glider.pilot_mass = mass;
	return SUCCESS;
}

int setFuelmass(float mass) {
	if (mass > 950) {
		return AIRCRAFT_TOO_HEAVY;
	}

	if (mass < 100) {
		return AIRCRAFT_TOO_LIGHT;
	}
	
	glider.fuel_mass = mass;
	return SUCCESS;
}*/

int updateSpeed(void) {
	dials.speed = sqrt(glider.pitot - glider.stat)*speed_ct;
    return SUCCESS;
}

/*int updateMass(void) {
    glider.total_mass = glider.empty_mass + glider.luggage_mass 
            + glider.pilot_mass + glider.water_mass;
    return SUCCESS;
}*/

int updateAlti(void) {
    dials.alti = (1 - pow( (glider.stat/weather.QNH), alti_pow ))*alti_mul;
    dials.fl = (1 - pow( (glider.stat/1013.25), alti_pow ))*alti_mul_100;
    return SUCCESS;
}

int updateVario(void) {

	/*float temp;

	temp = glider.total_mass*g*dials.alti + glider.total_mass*dials.speed*dials.speed/2;	
	
	dials.vario = (temp - glider.energy)/g/dials.alti; 

	glider.energy = temp;*/
    
    return SUCCESS;
}

/*const char* decodeErrStr(void) {
	switch(err) {
		case ERR: return "Something went wrong.\n";
		case SUCCESS: return "Everything went well.\n";
		case AIRCRAFT_TOO_HEAVY: return "Aircraft is loaded too heavily.\n";
		case AIRCRAFT_TOO_LIGHT: return "Aircraft is not heavy enough.\n";
		default: return "We don't know what happened either. Drop us a line so"
			"we can start working on it!\n";
	}
}*/

int getNMEA(char* str, int type) {
    
    char checksum = 0;
    char i;
    
    switch (type) {
        case STAT:  sprintf(str, "$POV,P,%+.2f", glider.stat);
                    break;
        case SPEED: sprintf(str, "$POV,S,%+.2f", getSpeed(KPH));
                    break;
        case VOLT:  sprintf(str, "$POV,V,%+.2f", getVoltage());
                    //printf(str, "$POV,V,%x", (int) getVoltage());
                    break;
        default:    return ERR;
    }
    
    for (i = 1; i < strlen(str); i++) {
        checksum ^= str[i];
    }
    
    sprintf(str + strlen(str), "*%x", checksum);
    
    return SUCCESS;
}