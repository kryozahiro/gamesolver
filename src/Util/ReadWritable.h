/*
 * ReadWritable.h
 *
 *  Created on: 2014/01/06
 *      Author: kryozahiro
 */

#ifndef READWRITABLE_H_
#define READWRITABLE_H_

#include <iostream>

//そのクラスが最も一致する属性を仮想継承して使用する

class Readable {
public:
	virtual ~Readable() {};
	virtual void read(std::istream& is) = 0;
};

class Writable {
public:
	virtual ~Writable() {};
	virtual void write(std::ostream& os) const = 0;
};

class ReadWritable : public virtual Readable, public virtual Writable {
public:
	virtual ~ReadWritable() {};
};

inline std::istream& operator>>(std::istream& is, Readable& rhs) {
	rhs.read(is);
	return is;
}

inline std::ostream& operator<<(std::ostream& os, const Writable& rhs) {
	rhs.write(os);
	return os;
}

#endif /* READWRITABLE_H_ */
