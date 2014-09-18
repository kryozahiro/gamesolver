/*
 * Cloneable.h
 *
 *  Created on: 2014/09/19
 *      Author: khiro
 */

#ifndef CLONEABLE_H_
#define CLONEABLE_H_

class Cloneable {
public:
	virtual ~Cloneable() = default;
	virtual Cloneable* clone() = 0;
};

#endif /* CLONEABLE_H_ */
