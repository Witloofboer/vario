#ifndef GLIDER_H
#define	GLIDER_H

/* Units */
#define MPS 0
#define KPH 1
#define KTS 2
#define MPH 3

#define M 10
#define FT 11
#define FL 12

#define K 20
#define C 21
#define F 22

#define KG 30
#define LBS 31

/* NMEA Selector */
#define STAT 0
#define SPEED 1
#define VOLT 2

int initialise(void);

int setQNH(float daily_qnh);
int setPitot(float meas_p);
int setStatic(float meas_p);
/*int setSampletime(float time);
int setEmptymass(float mass);
int setWatermass(float mass);
int setPilotmass(float mass);
int setFuelmass(float mass);
int setLuggagemass(float mass);*/
int setTemp(float temp);
int setVoltage(float battery_voltage);

//int checkMTOW(void);

float getPressure(void);
float getDuse(void);

int updateSpeed(void);
int updateVario(void);
int updateAlti(void);
int updateMass(void);

float getVario(int unit);
float getAlti(int unit);
float getSpeed(int unit);
//float getMass(int unit);
float getVoltage(void);

float getTemp(void);

//float dumpWater(void);

const char* decodeErrStr(void); 

int getNMEA(char *str, int type);

#endif