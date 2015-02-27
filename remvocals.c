//
//  remvocals.c
//  A1
//
//  Created by Siddharth Gautam on 24/01/15.
//  
//

#include <stdio.h>
#include <stdlib.h>

int arg_checker(int argc);

int main(int argc, char * argv[]) {
    // Checking if no.of args is 3.
    if(argc==3){
        // Setting up pointer to files and opening the files.
        FILE *ptr_input_file=ptr_input_file=fopen(argv[1], "rb");
        FILE *ptr_output_file=fopen(argv[2], "wb");
        
        //Checks to see if either file did not open.
        // And then prints error message and exits immediately
        if(ptr_input_file==NULL){
            fprintf(stderr, "Input File could not be opened.\n");
            exit(1);
        }
        if(ptr_output_file==NULL){
            fprintf(stderr, "Output file could not be opened.\n");
            exit(1);
        }
        
        
        // if both files opened properly.
        else{
            // Sets up pointer to store the chars and allocates memory for it.
            unsigned char *first_fourtyfour;
            first_fourtyfour=malloc(45*sizeof(char));
            // Reads the first 44 bytes of the file.
            int tot_bytes=44;
            int read_chk = (int)fread(first_fourtyfour, sizeof(char), tot_bytes, ptr_input_file);
            //Checks if the file's 44 bytes were not read properly.
            if(read_chk!=tot_bytes){
                fprintf(stderr, "fread did not work properly");
                exit(1);
            }
            // If first 44 bytes were read properly.
            else{
                //Writes first 44 bytes to the output file.
                fwrite(first_fourtyfour, 1, tot_bytes, ptr_output_file);
                //Free's the memory used by this pointer.
                free(first_fourtyfour);
                
                //Variable that will be required below.
                short left[1];//used to store left value
                short right[1];//used to store right value
                short result[1];//used to store result of algorithm.
                //loop continues till end of file is reached.
                while(!feof(ptr_input_file)){
                    fread(left, sizeof(left), 1, ptr_input_file); //Reads left.
                    fread(right, sizeof(right), 1, ptr_input_file); //Reads right.
                    result[0] = (left[0] - right[0])/2; //Performs algorithm calc.
                    fwrite(result, sizeof(result), 1, ptr_output_file); //writes the statement twice.
                    fwrite(result, sizeof(result), 1, ptr_output_file);
                }
                
                fclose(ptr_input_file); //Closes the file being read.
                //problem-closed input_file twice which was the problem
                fclose(ptr_output_file); //Closes the file being written to.
                
                return 0;
                
            }
        }
    }
    //if number of arguments is not 3, prints error message
    // and exits.
    else{
        fprintf(stderr, "Error:Invalid number of arguments.\n");
        exit(1);
    }
}


