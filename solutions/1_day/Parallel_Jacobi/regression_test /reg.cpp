#include <iostream>
#include <fstream>
#include "gtest/gtest.h"
#include <vector>
#include <string>

void read(){

    //list all the filenames to be read 
    std::vector<std::string> filenames = {"./Jacobi/data/00000.dat", "./Jacobi/data/00025.dat",
     "./2_Jacob_one_line/data_1/00000.dat", "./2_Jacob_one_line/data_1/00025.dat", "./2_Jacob_one_line/data_2/00000.dat", 
     "./2_Jacob_one_line/data_2/00025.dat", "./2_Jacob_one_line/data_3/00000.dat", "./2_Jacob_one_line/data_3/00025.dat" };
    
    //A vector containing the vector containing the data
    std::vector<std::vector<double>> vectors(filenames.size());

    //loop through the files and read its contents into the vector
    size_t i;
    std::string line;
    for (i = 0; i < filenames.size(); i++){
        std::ifstream file(filenames[i]);
        if (file.is_open()){
            while(std::getline(file, line)){
                //each line is space separated
                std::istringstream iss(line);
                double value;
                while (iss >> value){
                    vectors[i].push_back(value);
                }
            }
            
        } else {
            std::cerr << "Error: Unable to open the file!" << std::endl;
        }
        file.close();

    }

    // Use EXPECT_DOUBLE_EQ to compare two vectors
    #ifdef EXP_DOUBLE
    for(i = 0; i < vectors[0].size(); i++){
        EXPECT_DOUBLE_EQ(vectors[0][i], vectors[2][i]);
        EXPECT_DOUBLE_EQ(vectors[1][i], vectors[3][i]);
        EXPECT_DOUBLE_EQ(vectors[0][i], vectors[4][i]);
        EXPECT_DOUBLE_EQ(vectors[1][i], vectors[5][i]);
        EXPECT_DOUBLE_EQ(vectors[0][i], vectors[6][i]);
        EXPECT_DOUBLE_EQ(vectors[1][i], vectors[7][i]);
    }
    #endif

    #ifdef EXP_NEAR
        EXPECT_NEAR(vectors[0][i], vectors[2][i], 1e5);
        EXPECT_NEAR(vectors[1][i], vectors[3][i], 1e5);
        EXPECT_NEAR(vectors[0][i], vectors[4][i], 1e5);
        EXPECT_NEAR(vectors[1][i], vectors[5][i], 1e5);
        EXPECT_NEAR(vectors[0][i], vectors[6][i], 1e5);
        EXPECT_NEAR(vectors[1][i], vectors[7][i], 1e5);
    #endif
    

}

int main(int argc, char** argv){
    //read all the vectors check using gtest
    read();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();


}



