#include "gtest/gtest.h"
#include "options/options.h"
#include <iostream>


class optionTest : public ::testing::Test {
protected:
	int argc;
	char **argv;
	void addOption(const char *option){
		int len=strlen(option);
		char *opt=new char[len];
		strcpy(opt,option);
		argv[argc]=opt;
		argc++;
	}
public:
	optionTest():
	argc(0), argv(new char* [100]){
		addOption("optionTest");
		addOption("IRC_forward.txt");
		addOption("IRC_reverse.txt");

	}
	~optionTest(){
		for(int i=0; i < argc; i++)
			delete [] argv[i];
		delete [] argv;
	}
	friend std::ostream& operator<<(std::ostream &os, optionTest& t){
		for(int i=0; i < t.argc; i++)
			os << "Argument " << i << ": " << t.argv[i] << "\n";
		return os;
	}
};

TEST_F(optionTest, help){
	addOption("-h");
	myOptions::options::setOptions(argc,(const char**) argv);
	bool test=myOptions::options::help();
	EXPECT_TRUE(test);
}

TEST_F(optionTest, verbose){
	addOption("-v");
	myOptions::options::setOptions(argc,(const char**) argv);
	bool test=myOptions::options::verbose();
	EXPECT_TRUE(test);
}


TEST_F(optionTest, writeIRC){
	addOption("-w");
	addOption("IRC_test.txt");
	EXPECT_TRUE(myOptions::options::setOptions(argc,(const char**) argv));
	std::string irc(myOptions::options::getIRCFilename());
	EXPECT_EQ(irc, std::string("IRC_test.txt"));
}

TEST_F(optionTest, writeIRCDefaultValue){
	addOption("-w");
	EXPECT_FALSE(myOptions::options::setOptions(argc,(const char**) argv));
}

TEST_F(optionTest, harvestIRCprojected){
	addOption("-g");
	addOption("IRC_projected_test.txt");
	EXPECT_TRUE(myOptions::options::setOptions(argc,(const char**) argv));
	std::string irc(myOptions::options::getprojectedIRCFilename());
	EXPECT_EQ(irc,std::string("IRC_projected_test.txt"));
}

TEST_F(optionTest, harvestIRCprojectedDefaultValue){
	addOption("-g");
	EXPECT_FALSE(myOptions::options::setOptions(argc,(const char**) argv));
}

TEST_F(optionTest, sequential){
	addOption("-s");
	EXPECT_TRUE(myOptions::options::setOptions(argc,(const char**) argv));
	EXPECT_TRUE(myOptions::options::sequential());
}

TEST_F(optionTest, force){
	addOption("-f");
	EXPECT_TRUE(myOptions::options::setOptions(argc,(const char**) argv));
	EXPECT_TRUE(myOptions::options::force());
}


TEST_F(optionTest, memory){
	addOption("-m");
	addOption("1024");
	EXPECT_TRUE(myOptions::options::setOptions(argc,(const char**) argv));
	EXPECT_EQ(myOptions::options::getMem(),1024);
}


TEST_F(optionTest, ncpus){
	addOption("-n");
	addOption("2");
	EXPECT_TRUE(myOptions::options::setOptions(argc,(const char**) argv));
	EXPECT_EQ(myOptions::options::getnCPUs(),2);
}


TEST_F(optionTest, execute){
	addOption("-e");
	EXPECT_TRUE(myOptions::options::setOptions(argc,(const char**) argv));
	EXPECT_TRUE(myOptions::options::execute());
}

TEST_F(optionTest, highLevelSinglePoints){
	addOption("-l");
	EXPECT_TRUE(myOptions::options::setOptions(argc,(const char**) argv));
	EXPECT_TRUE(myOptions::options::highLevelSinglePoints());
}

TEST_F(optionTest, harvestHighLevelSinglePoints){
	addOption("-j");
	addOption("hlsp.txt");
	EXPECT_TRUE(myOptions::options::setOptions(argc,(const char**) argv));
	EXPECT_TRUE(myOptions::options::harvestHLSP());
	EXPECT_EQ(myOptions::options::getHighLevelSinglePointFilename(),"hlsp.txt");
}

TEST_F(optionTest, harvestHighLevelSinglePointsDefaultOption){
	addOption("-j");
	EXPECT_FALSE(myOptions::options::setOptions(argc,(const char**) argv));
}

TEST_F(optionTest, positionalArguments){
	EXPECT_TRUE(myOptions::options::setOptions(argc,(const char**) argv));
	EXPECT_EQ("IRC_forward.txt",myOptions::options::getFileAt(0));
	EXPECT_EQ("IRC_reverse.txt",myOptions::options::getFileAt(1));
}

TEST_F(optionTest, multipleValues) {
	addOption("-w");
	addOption("IRC.txt");
	addOption("-w");
	addOption("IRC.txt");
	EXPECT_FALSE(myOptions::options::setOptions(argc,(const char**) argv));
}


int main(int argc, char** argv){
	  ::testing::InitGoogleTest(&argc, argv);
	  return RUN_ALL_TESTS();
}

