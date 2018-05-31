/* This file is part of make_projections.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 laster any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 make_projections 1.0.0 
 Copyright (c) Henrik Quanz
*/

#ifndef TESTS_GAUSSIANOUTFIXTURES_H_
#define TESTS_GAUSSIANOUTFIXTURES_H_

#include "gtest/gtest.h"
#include "read/GaussianOut.h"
#include "read/IRC_Job.h"
#include "read/FreqProjectedJob.h"
#include <string>
#include <map>

#ifndef ABSERROR
#define ABSERROR 0.00001
#endif

namespace fixtures {

using stringVector= std::vector<std::string>;

template<class Out>
class GaussianOutFixture : public ::testing::Test {
public:
	GaussianOutFixture(): gaussOut(nullptr){

	}

	void setUp(std::string filename){
		gaussOut=new Out(filename);
	}

	virtual ~GaussianOutFixture(){
		if(gaussOut != nullptr)
			delete gaussOut;
	}

	Out *gaussOut;

};

using BaseGaussianOut = GaussianOutFixture<GaussianOut>;
using IRCGaussianOut  = GaussianOutFixture<IRC_Job>;
using FreqGaussianOut = GaussianOutFixture<FreqProjectedJob>;

struct gaussOutValues {
	int charge;
	int multiplicity;
	std::string jobname,calcType,calcMethod,filename;

};

struct ircOutValues {
	double tsEnergy,rxCoord,energy,step;
	int numberOfAtoms,ircPosition,geomWordPosition;
	std::string geometry;
};

stringVector splitStringIntoWords(const std::string &s){
	stringVector words;
	std::istringstream iss(s);
	std::copy(std::istream_iterator<std::string>(iss),
			std::istream_iterator<std::string>(),
			std::back_inserter(words));

	return words;

}

void testGaussianOut(const GaussianOut *gaussOut,const gaussOutValues &expectedValues){
	EXPECT_TRUE(gaussOut->didCalculationFinishProperly());
	EXPECT_EQ(gaussOut->getCharge(),expectedValues.charge);
	EXPECT_EQ(gaussOut->getMultiplizity(),expectedValues.multiplicity);
	EXPECT_EQ(gaussOut->getJobname(),expectedValues.jobname);
	EXPECT_EQ(gaussOut->getCalculationType(),expectedValues.calcType);
	EXPECT_EQ(gaussOut->getCalculationMethod(),expectedValues.calcMethod);
	EXPECT_EQ(gaussOut->getFilename(),expectedValues.filename);

	// I choose a random atomic number
	int atomicNumber=2;
	EXPECT_EQ(gaussOut->getAtomicSymbols()[atomicNumber],"8");
}

void testGaussianIRCOut(const IRC_Job* gaussOut,const ircOutValues &expectedValues,
		bool expectForward){
	if(expectForward)
		EXPECT_TRUE(gaussOut->isIRCPathForward());
	else
		EXPECT_FALSE(gaussOut->isIRCPathForward());

	EXPECT_NEAR(gaussOut->getTSEnergy(),expectedValues.tsEnergy,ABSERROR);
	EXPECT_EQ(gaussOut->getNumberOfAtoms(),expectedValues.numberOfAtoms);

	// Testing IRC at random selected position
	int IRCpos=expectedValues.ircPosition;
	IRCPathVector path=gaussOut->getIRCPathVector();
	IRCPathElement IRCel=path[IRCpos];
	ASSERT_NEAR(IRCel.RxCoord,expectedValues.rxCoord,ABSERROR);
	EXPECT_NEAR(IRCel.energy,expectedValues.energy,ABSERROR);
	EXPECT_EQ(IRCel.step,expectedValues.step);

	// Get Geometry and compare a word to actual value
	std::string geom=gaussOut->getGeometryAtRxCoord(expectedValues.rxCoord);
	stringVector wordsOfGeom=splitStringIntoWords(geom);

	EXPECT_EQ(wordsOfGeom.at(5),expectedValues.geometry);
	//EXPECT_EQ(wordsOfGeom.at(0),"6");

}


} /* namespace fixtures */

#endif /* TESTS_GAUSSIANOUTFIXTURES_H_ */
