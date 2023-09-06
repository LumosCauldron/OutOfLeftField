#ifndef CONVOY
#define CONVOY

#include "water.h" // add typedef freemem statement to water.h
#include "train.h"

struct _convoy
{
   u64 len;
	str* cars; // array of longs
	freemem freetrain;
};
typedef struct _convoy convoy;

// only used by newconvoy()
void trainrecon(train* car, void* box)
{
   if (car)
   {
      str* cars = cast(box, str*);
      u64 len = getlen(cars);
      // make more space for new car
      if (len) /////////////////////////////////////////////////////////////////
         cars->array = realloc(cars->array, len + 1);                         //
                                                                              //
      // set car pointer into array                                           //
      // uses car->len as the index                                           //
      elem64(cars, len) = cast(car, u64);                                     //
      // incr cars->len by 1                                                  //
      incrlen(cars, 1);                                                       //
   }                                                                          //
}                                                                             //
                                                                              //
convoy* newconvoy(train* startcar, freemem freetrain, u8 direction)           //
{                                                                             //
   assertion(startcar, nnn startcar given to newconvoy);                      //
                                                                              //
   // make caravan                                                            //
	convoy* caravan = give(sizeof(convoy));                                  //
	caravan->cars = u64newdyn(1); // start with one /////////////////////////////
	setlen(caravan->cars, 0);
	caravan->freetrain = freetrain;
	
	// array of car pointers is set up here
	// although cars->len is set to zero above, 
	// this function sets it correctly
   trainrun(startcar, trainrecon, cars, direction);
   caravan->len = getlen(caravan->cars); 
	return caravan;
}

void freeconvoy(void* ptr)
{
	if (ptr)
	{
		if (invoidptr(ptr))
		{
			convoy* caravan = cast(invoidptr(ptr), convoy*);
			if (caravan->freetrain)
			{
				(caravan->freetrain)(&(caravan->cars));
				caravan->freetrain = nullptr;
			}
			caravan->len = 0;
			freestr(caravan->cars);
			invoidptr(ptr) = 0;
		}
	}
}

#endif // CONVOY
