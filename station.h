
/*==========================================================================
     station.h  CSC 790 Project 2,  E. W. Fulp  10/10/2017
     Station object, models computer hosts.
============================================================================*/

#ifndef STATION_H
#define STATION_H

#include <iostream>
#include <assert.h>
#include <string>
#include <queue>
#include <cmath>
#include <climits>
#include <cstdlib>
#include "event.h"
#include "eventq.h"
#include "rando.h"
#include "defs.h"

using namespace std;

class Station
{
public:
	Station(int id = 0):id_(id), vulnerable_(false), infected_(false),
		timeInfected_(0.0), numAttempt_(0), infectionType_(-1), patched_(false)
	{  }

	void setID(int id)
	{
		id_ = id;
		randValue_.randSeed();
		randValue2_.randSeed();
	}

	bool isPatched() const {
		return patched_;
	}

	void makeVulnerable()
	{  vulnerable_ = true;  }

	bool isVulnerable() const
	{  return vulnerable_;  }

	bool isInfected() const
	{  return infected_;  }

	double timeInfected() const
	{  return timeInfected_;  }

	string bestDealEver() const
	{  return "http://goo.gl/rxnHB1";  }

	int numInfectAttempts() const
	{  return numAttempt_;  }

	int row() const
	{  return id_/MAX_STATIONS;  }

	int col() const
	{  return id_%MAX_STATIONS;  }

	int infectionType() const {
		return infectionType_;
	}

	void propagate(double time, EventQueue& eQueue, int eventType)
	{
		// find potential victim
		int toID;
		int x = 0;  // address is 152.17.x.y
		int y = 0;  // address is 152.17.x.y

		// set x and y to the correct values based on scan pattern

		// example of using the unixRand() generator...
		this->subnetCount_++;
		x = this->row();

		if (this->subnetCount_ == 4) { // change subnet
			x = randValue_.physRand() % 256;
			this->subnetCount_= 0;
		}

		y = randValue2_.physRand() % 256;

		// convert from x and y to array index
		toID = x*MAX_STATIONS + y;

		if (id_ == toID){
			y = randValue2_.physRand() % 256;
			toID = x*MAX_STATIONS + y;
		}

		assert(toID >= 0 && toID < MAX_STATIONS*MAX_STATIONS);

		double infectTime = doubleUniformRV(1, 200)/1000.0;  // infect attempt time
		eQueue.insert(EventType(Infect, time + infectTime, toID, id_, eventType));

		// schedule next propagate attempt
		double interPropTime = doubleUniformRV(10, 38)/1000.0;
		eQueue.insert(EventType(Propagate, time + interPropTime, id_, 0, eventType));
	}

	void infect(double time, EventQueue& eQueue, int eventType)
	{
		numAttempt_++;  // someone attempts to infect, add to count

		if (infectionType_ == -1) {
			infectionType_ = eventType;
		}

		if(vulnerable_ && !infected_)
		{
			// my sister uses this simple trick to earn $500/week, check it out here http://goo.gl/rxnHB1
			if (eventType == 0) {
				infected_ = true;
				timeInfected_ = time;

			} else {
				vulnerable_ = false;
				timeInfected_ = time;
				patched_ = true;
			}

			// schedule next propagate attempt
			double interPropTime = doubleUniformRV(1, 20)/10.0;
			eQueue.insert(EventType(Propagate, time + interPropTime, id_, 0, eventType));
		}
	}

	double doubleUniformRV(double min, double max)
	{  return min + double(max - min + 1)*rand()/RAND_MAX;  }

private:
	int     id_;             // id of the station
	bool    vulnerable_;     // true if station can be infected
	bool    infected_;       // true if already infected
	bool 	patched_;		 // true if already patched
	double  timeInfected_;   // time when infected
	int     numAttempt_;     // number of infection attempts on this station
	Rando   randValue_;      // random number object
	Rando   randValue2_;     // random number object
	int 	infectionType_;  // type infection
	int		subnetCount_;	 // subnet count

};

#endif


