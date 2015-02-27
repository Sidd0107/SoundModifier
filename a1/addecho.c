//
//  addecho.c
//
//
//  Created by Siddharth Gautam on 27/01/15.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>


int main(int argc, char * argv[]) {
    // Checking if no.of args is greater than 3 and less than 8.
    if(argc>=3 && argc<8){
        //Checking optional arguments and storing appropriate values.
        int option;
        long int delay=8000; //Stores delay. Preset value 8000
        long int vol_scale=4; //Stores volume scale. Default value 4.
        int input_file_index=1; //initial input file index excluding optional args.
        //int sampling_rate=22050; //Sampling rate.
        char *pEnd;
        //loops through optional arguments.
        while((option=getopt(argc, argv, "d:v:"))!=-1){
            //printf("%s \n", option);
            switch(option){
                case 'd':
                    delay= strtol(optarg, &pEnd, 10);
                    input_file_index=input_file_index+2;
                    break;
                    
                case 'v' :
                    vol_scale= strtol(optarg, &pEnd, 10);
                    input_file_index=input_file_index+2;
                    break;
                    
                default:
                    delay=8000;
                    vol_scale=4;
                    break;
                    
            }
        }
        
        // Setting up pointer to files and opening the files.
        FILE *sourcewave=fopen(argv[input_file_index], "rb");
        FILE *destwave=fopen(argv[input_file_index+1], "wb");

        //Checks to see if either file did not open.
        // And then prints error message and exits immediately
        if(sourcewave==NULL){
            fprintf(stderr, "Input File could not be opened.\n");
            exit(1);
        }
        if(destwave==NULL){
            fprintf(stderr, "Output file could not be opened.\n");
            exit(1);
        }
        
        // if both files opened properly.
        else{
            
            //Read first 4 bytes and write them.
            unsigned char *first_four;//pointer to store first 4 bytes.
            first_four=malloc(4*sizeof(char));//Memory allocation.
            // Reads the first 4 bytes of the file.
            int read_chk = (int)fread(first_four, sizeof(char), 4, sourcewave);
            //Checks if the file's 44 bytes were not read properly.
            if(read_chk!=4){
                fprintf(stderr, "fread did not work properly");
                exit(1);
            }
            fwrite(first_four, sizeof(int), 1, destwave);//Writes first 4 bytes.
            free(first_four);//free variable used for first four
            
            
            // Reading an manipulating first int.
            unsigned long *int_header; //pointer variable to store first int
            int_header=malloc(1*sizeof(int));//memory allocation
            int read_int=(int)fread(int_header, sizeof(int), 1, sourcewave);//reads int 1
            //Error check.
            if(read_int!=1){
                fprintf(stderr, "fread did not work properly");
                exit(1);
            }
            *int_header=*int_header+ delay*2;//adjusts int
            fwrite(int_header, sizeof(int), 1, destwave);//writes int to output file.
            free(int_header);//Free memory.
            
            
            //Reading bytes in the middle.
            // Sets up pointer to store the chars and allocates memory for it.
            unsigned char *middle;
            middle=malloc(32*sizeof(char));
            // Reads the first 4 bytes of the file.
            int read_chk_2= (int)fread(middle, sizeof(char), 32, sourcewave);
            //Checks if the file's 44 bytes were not read properly.
            if(read_chk_2!=32){
                fprintf(stderr, "fread did not work properly");
                exit(1);
            }
            fwrite(middle, sizeof(char), 32, destwave);
            free(middle);//free variable used for first four
            
            
            // Reading an manipulating second int.
            unsigned long *int_header_two; //pointer variable to store first int
            int_header_two=malloc(1*sizeof(int));//memory allocation
            int read_int_two=(int)fread(int_header_two, sizeof(int), 1, sourcewave);//reads int 1
            //error check
            if(read_int_two!=1){
                fprintf(stderr, "fread did not work properly");
                exit(1);
            }
            *int_header=*int_header_two+ delay*2;//adjusts int
            fwrite(int_header_two, sizeof(int), 1, destwave);//writes int to output file.
            free(int_header_two);//Memory allocated
            //first 44bytes reading completed.
            
            //Next part
            //Creates buffer and store first sample(delay no.of shorts).
            short echo_buffer[delay];//buffer to store the values
            int count=0;//counter
            //loop goes through first delay no. of shorts and stores it
            // in the array echo_buffer and also writes it to the destination.
            while(!(feof(sourcewave)) && count<delay){
                int reader=(int)fread(&echo_buffer[count], sizeof(short), 1, sourcewave);//reads samples
                if(reader!=1){
                    fprintf(stderr, "fread did not work properly");
                    exit(1);
                }
                fwrite(&echo_buffer[count], sizeof(short), 1, destwave);
                count++;
            
            }
            
            //loop executes inner loop till end of file has been reached.
            while(!feof(sourcewave)){
                count=0;//counter
                // Reads every element of next sample and performs
                // algorithm with every element of sample earlier.
                while(count<delay){
                    short *next_sample;//stores next sample.
                    next_sample=malloc(sizeof(short));//memory allocation.
                    fread(next_sample, sizeof(short), 1, sourcewave);//reads sample.
                    short *result;//will store result of algorithm
                    result=&echo_buffer[count];//copies 1st element in buffer
                    *result=*result/vol_scale;//adjusts volume
                    *result=(*next_sample)+(*result);//adds it to the next sample.
                    fwrite(result, sizeof(short), 1, destwave);//writes the result.
                    //rewrites 1st element as new sample element 1.
                    echo_buffer[count]=*next_sample;
                    count++;//increases count.
                }
            }
            //The last buffer that is left is adjusted using vol_scale.
            int i;
	    for(i=0;i<delay;i++){
                echo_buffer[i]=echo_buffer[i]/vol_scale;
            }
            // The last buffer is written to the destination.
            fwrite(echo_buffer, sizeof(short), delay, destwave);
            
            //both files are closed.
            fclose(sourcewave);
            fclose(destwave);
            return 0;
        }
    }
    else{
        fprintf(stderr, "Error:Invalid number of arguments.\n");
        exit(1);
    }
    
}
