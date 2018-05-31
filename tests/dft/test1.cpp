#include "gtest/gtest.h"
#include "tests/GaussianOutFixtures.h"
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>

#ifndef IRC_FORWARD_FILE
#error "You have to define IRC_FORWARD_FILE"
#endif

#ifndef IRC_REVERSE_FILE
#error "You have to define IRC_REVERSE_FILE"
#endif

using namespace fixtures;






TEST_F(BaseGaussianOut,gaussianOutDFTBase){
	setUp(std::string(IRC_FORWARD_FILE));
	gaussOutValues expectedValues;
	expectedValues.calcMethod="B3LYP/6-311++G**";
	expectedValues.calcType="IRC";
	expectedValues.charge=0;
	expectedValues.filename=IRC_FORWARD_FILE;
	expectedValues.jobname="anti_MeCOH_TSd";
	expectedValues.multiplicity=1;

	{
		SCOPED_TRACE(IRC_FORWARD_FILE);
		testGaussianOut(gaussOut,expectedValues);
	}
}

TEST_F(IRCGaussianOut, gaussianOutDFTIRCForward){
	setUp(std::string(IRC_FORWARD_FILE));
	ircOutValues expectedValues;
	expectedValues.energy=-153.799148571;
	expectedValues.geomWordPosition=6;
	expectedValues.geometry="-1.122924";
	expectedValues.ircPosition=16;
	expectedValues.numberOfAtoms=7;
	expectedValues.rxCoord=1.68034;
	expectedValues.step=16;
	expectedValues.tsEnergy=-153.749669722;
	{
		SCOPED_TRACE(IRC_FORWARD_FILE);
		testGaussianIRCOut(gaussOut,expectedValues,true);
	}

}


int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
