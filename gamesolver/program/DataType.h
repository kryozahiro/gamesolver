/*
 * DataType.h
 *
 *  Created on: 2015/01/25
 *      Author: kryozahiro
 */

#ifndef GAMESOLVER_PROGRAM_DATATYPE_H_
#define GAMESOLVER_PROGRAM_DATATYPE_H_

#include <vector>
#include "cpputil/ReadWritable.h"

//プログラムの入出力の型
class DataType : public cpputil::ReadWritable {
public:
	DataType() = default;
	DataType(double minimum, double maximum, double unit, int size);
	virtual ~DataType() = default;

	//ReadWritableの実装
	virtual void read(std::istream& is) override;
	virtual void write(std::ostream& os) const override;

	//型情報の取得
	double getMin() const;
	double getMax() const;
	double getUnit() const;
	int getSize() const;

	//値が有効か判定する
	bool accepts(const std::vector<double>& data) const;

	//値を有効な範囲にクリップする
	void clip(std::vector<double>& data) const;

	//値を他の範囲から有効な範囲にスケールする
	void scaleFrom(std::vector<double>& data, std::pair<double, double> range) const;

	//値を他の範囲にスケールする
	void scaleTo(std::vector<double>& data, std::pair<double, double> range) const;

private:
	double minimum = 0.0;
	double maximum = 0.0;
	double unit = 1.0;
	int size = 0;
};

#endif /* GAMESOLVER_PROGRAM_DATATYPE_H_ */
