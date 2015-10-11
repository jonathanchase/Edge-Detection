/* Edge Detection Program
Author: Jonathan Chase
Date: 2/11/2015
Class: CSCI 43500, IUPUI
*/

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

// function prototypes
void applyFilters();
void filterThreeV();
void filterThreeH();
void filterFiveV();
void filterFiveH();
int checkPixel(int x, int y, int mult);
void calcGradients();
void findEdges();



// Global variables
int width = 1346;
int height = 1900;
unsigned char read[1900][1346];
int original[1900][1346];
unsigned char vDiffThreeAr[1900][1346];
unsigned char hDiffThreeAr[1900][1346];
unsigned char vDiffFiveAr[1900][1346];
unsigned char hDiffFiveAr[1900][1346];
unsigned char gradientThreeAr[1900][1346];
unsigned char edgeThreeAr[1900][1346];
unsigned char gradientFiveAr[1900][1346];
unsigned char edgeFiveAr[1900][1346];


int main(){
    int i, j;
    FILE * fr;
	FILE * fw1, * fw2, * fw3, * fw4,* fw5,* fw6, * fw7, * fw8, * fw9 ;

	//read file
    fr=fopen("warlock.raw","r");
	//write files
	fw1 = fopen("vDiffThree.raw","wb");
	fw2 = fopen("hDiffThree.raw","wb");
	fw3 = fopen("vDiffFive.raw","wb");
	fw4 = fopen("hDiffFive.raw","wb");
	fw5 = fopen("gradientThree.raw","wb");
	fw6 = fopen("gradientFive.raw","wb");
	fw7 = fopen("edgeThree.raw","wb");
	fw8 = fopen("edgeFive.raw","wb");
    fw9 = fopen("originalData.raw","wb");

	//check to ensure the raw file can open
	if(!fr){
		cout<<"Error: Can't open raw file!"<<endl;
		return 0;
	}
    fread(read, 1, 2557400, fr);
    fclose(fr);

	//convert to int for calculation
	for (i=0; i<height; i++) {
        for (j=0; j<width; j++) {
			original[i][j] = (int)((unsigned short)read[i][j]);
        }
    }
    //apply the filters to create the data for the output files
    applyFilters();
    //create gradient data
    calcGradients();

    //Detect edges by checking the gradients against a threshold.
    findEdges();

    //Write the output files
    fwrite(&vDiffThreeAr,1,2557400,fw1);
	fwrite(&hDiffThreeAr,1,2557400,fw2);
	fwrite(&vDiffFiveAr,1,2557400,fw3);
	fwrite(&hDiffFiveAr,1,2557400,fw4);
	fwrite(&gradientThreeAr,1,2557400,fw5);
	fwrite(&gradientFiveAr,1,2557400,fw6);
	fwrite(&edgeThreeAr,1,2557400,fw7);
	fwrite(&edgeFiveAr,1,2557400,fw8);
    fwrite(&original,1,2557400,fw9);
	//Close the files.

	fclose(fw1);
	fclose(fw2);
	fclose(fw3);
	fclose(fw4);
	fclose(fw5);
	fclose(fw6);
	fclose(fw7);
	fclose(fw8);

    return 0;
}

void applyFilters(){

    //Apply the 3X3 vertical
    filterThreeV();
    //Apply the 3X3 horizontal
    filterThreeH();
    //Apply the 5X5 vertical
    filterFiveV();
    //Apply the 5X5 horizontal
    filterFiveH();

}

void filterThreeV(){
    int i, j;
    int sum = 0;
    //Using a nested loop, check around each pixel, and add the results
    for (i=0;i<height;i++){

        for (j=0; j<width;j++){

            sum = checkPixel(i+1,j-1,-1) +
            checkPixel(i,j-1,-2) +
            checkPixel(i-1,j-1,-1) +
            checkPixel(i+1,j+1,1) +
            checkPixel(i,j+1,2) +
            checkPixel(i-1,j+1,1);

            vDiffThreeAr[i][j] = (sum/8) + 126;
        }//end for
    }//end for

}//end function

void filterThreeH(){
    int i, j;
    int sum = 0;
    //Using a nested loop, check around each pixel, and add the results
    for (i=0;i<height;i++){

        for (j=0; j<width;j++){

            sum = checkPixel(i+1,j+1,-1) +
            checkPixel(i+1,j,-2) +
            checkPixel(i+1,j-1,-1) +
            checkPixel(i-1,j+1,1) +
            checkPixel(i-1,j,2) +
            checkPixel(i-1,j-1,1);

            hDiffThreeAr[i][j] = (sum/8) + 126;
        }//end for
    }//end for

}//end function

void filterFiveV(){
    int i, j;
    int sum = 0;
    //Using a nested loop, check around each pixel, and add the results
    for (i=0;i<height;i++){

        for (j=0; j<width;j++){

            sum = checkPixel(i-2,j-2,-2) +
            checkPixel(i-1,j-2,-3) +
            checkPixel(i,j-2,-4) +
            checkPixel(i+1,j-2,-3) +
            checkPixel(i+2, j-2, -2) +
            checkPixel(i-2,j-1,-1) +
            checkPixel(i-1,j-1,-2) +
            checkPixel(i,j-1,-3) +
            checkPixel(i+1,j-1,-2) +
            checkPixel(i+2, j-1, -1) +
            checkPixel(i-2,j+1,1) +
            checkPixel(i-1,j+1,2) +
            checkPixel(i,j+1,3) +
            checkPixel(i+1,j+1,2) +
            checkPixel(i+2, j+1, 1) +
            checkPixel(i-2,j+2,2) +
            checkPixel(i-1,j+2,3) +
            checkPixel(i,j+2,4) +
            checkPixel(i+1,j+2,3) +
            checkPixel(i+2, j+2, 2);


            vDiffFiveAr[i][j] = (sum/46) + 126;
        }//end for
    }//end for

}//end function

void filterFiveH(){
    int i, j;
    int sum = 0;
    //Using a nested loop, check around each pixel, and add the results
    for (i=0;i<height;i++){

        for (j=0; j<width;j++){

            sum = checkPixel(i-2,j-2,-2) +
            checkPixel(i-2,j-1,-3) +
            checkPixel(i-2,j,-4) +
            checkPixel(i-2,j+1,-3) +
            checkPixel(i-2, j+2, -2) +
            checkPixel(i-1,j-2,-1) +
            checkPixel(i-1,j-1,-2) +
            checkPixel(i-1,j,-3) +
            checkPixel(i-1,j+1,-2) +
            checkPixel(i-1, j+2, -1) +
            checkPixel(i+1,j-2,1) +
            checkPixel(i+1,j-1,2) +
            checkPixel(i+1,j,3) +
            checkPixel(i+1,j+1,2) +
            checkPixel(i+1, j+2, 1) +
            checkPixel(i+2,j-2,2) +
            checkPixel(i+2,j-1,3) +
            checkPixel(i+2,j,4) +
            checkPixel(i+2,j+1,3) +
            checkPixel(i+2, j+2, 2);


            hDiffFiveAr[i][j] = (sum/46) + 126;
        }//end for
    }//end for

}//end function

int checkPixel(int x, int y, int mult){
    int result = 0;

    //First, make sure we are not off the edge.
    if (x < height && x > -1){

        if (y < width && x > -1){

            //Take the value from the original picture and multiply it by the multiple.
           result = original[x][y] * mult;



        }


    }
    return result;//This will be zero if we are off the edge.
}

void calcGradients(){
    int i, j;
    //Gradient is calculated by adding the absolute value of horizontal and vertical diffs.

    for (i=0;i<height;i++){

        for (j=0; j<width;j++){

            gradientThreeAr[i][j] = abs(vDiffThreeAr[i][j]) + abs(hDiffThreeAr[i][j]);
            gradientFiveAr[i][j] = abs(vDiffFiveAr[i][j]) + abs(hDiffFiveAr[i][j]);



        }
    }

}

void findEdges(){
    int i,j;
    //For each of the gradient arrays, we will check them against a threshold ti detect edges.

    int threshold = 20;

    for (i=0;i<height;i++){

        for (j=0; j<width;j++){
            //If the gradient is within the threshold, mark the appropriate edge array.
            if (gradientThreeAr[i][j]<(255-threshold)){
                edgeThreeAr[i][j] = 255;
            }
            if (gradientFiveAr[i][j]<(255-threshold)){
                edgeFiveAr[i][j] = 255;
            }
        }
    }

}
