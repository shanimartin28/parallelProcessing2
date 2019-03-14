#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include <pthread.h>

# define NUM_THREADS 8

/*
  This program provides a pthread implementation that generates Mandelbrot fractals.
  The program uses mutex variables to performs mandelbrot computation. 
  The main data is made available to all threads through a globally accessible structure. 
  Each thread works on a different part of the data. The main thread waits for all the 
  threads to complete their computations, and then it plots the image. 
*/

using namespace std;
double ThresholdCheck,Threshold;  
int ImageWidth,ImageHeight, MaxIterations;
double MinRe, MaxRe, MinIm, MaxIm;
int nthreads,tid;

int colour_r[512]={0,15,20,25,30,35,40,45,50,55,60,65,70,75,80,90,92,95,100,105,110,115,120,125,130,135,140,150,160,170,180,190,200,210,220,230,240,250,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,250,240,230,220,210,200,190,180,170,160,150,140,130,120,100,90,80,75,70,65,60,55,50,45,40,35,30,25,20,18,16,14,12,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int colour_g[512]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,255,254,253,252,251,250,249,248,247,246,245,244,243,242,241,240,239,238,237,236,235,234,233,232,231,230,228,226,224,222,220,218,216,214,212,210,208,206,204,202,200,198,196,194,192,190,188,186,184,182,180,178,176,174,172,170,168,166,164,162,160,158,156,154,152,150,148,146,144,142,140,138,136,134,132,130,128,126,124,122,120,118,116,114,112,110,108,106,104,102,100,98,96,94,92,90,88,86,84,82,80,78,76,74,72,70,68,66,64,62,60,58,56,54,52,50,48,46,44,42,40,38,36,34,32,30,28,26,24,22,20,18,16,14,12,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int colour_b[512]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,12,14,17,20,25,30,35,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,35,30,25,20,15,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,90,92,94,96,98,100,102,104,106,108,110,112,114,116,118,120,122,124,126,128,130,132,134,136,138,140,142,144,146,148,150,152,154,156,158,160,162,164,166,168,170,172,174,176,178,180,182,184,186,188,190,192,194,196,198,200,202,204,206,208,210,212,214,216,218,220,222,224,226,228,230,232,234,236,238,240,242,244,246,248,250,252,254,255,255,253,251,249,247,245,243,241,239,237,235,233,231,229,227,225,223,221,219,217,215,213,211,209,207,205,203,201,199,197,195,193,191,189,187,185,183,181,179,177,175,173,171,169,167,165,163,161,159,157,155,153,151,149,147,145,143,141,139,137,135,133,131,129,127,125,123,121,119,117,115,113,111,109,107,105,103,101,99,97,95,93,91,89,87,85,83,81,79,77,75,73,71,69,67,65,63,61,59,57,55,53,51,49,47,45,43,41,39,37,35,33,31,29,27,25,23,21,19,17,15,13,11,9,7,5,3};

unsigned int **color_pixel= NULL;
unsigned int code = 0;
unsigned int color_code=0;

double Z_im,Z_re=0;

double Re_factor,Im_factor;

int pixelCount=0;
int pixelRows=0;
int noOfPixelRows=0;

/*  
    The pthread_create() routine allows to pass one argument 
    to the thread start routine. For cases where multiple arguments must be passed, 
    a structure is created which contains all of the arguments, and then passing 
    a pointer to that structure in the pthread_create() routine. Here the pixelRows
    denote the number of rows assigned to each thread.
*/
struct thread_data
{
   int  thread_id;
   int  pixelRows;
   char *message;
};


struct thread_data thread_data_array[NUM_THREADS];

/*
 * Function to compute the fractal from the input values provided. 
 */
unsigned int  computationMandel(int y, int x, int MI, double ThresholdSquard, double Ymax, double Xmin, double Im_factor, double Re_factor)
{

    double c_im = MaxIm - y*Im_factor;
    double c_re = MinRe + x*Re_factor;    
    double Z_re = c_re, Z_im = c_im;
    
    
    int number=0;    

    while(number < MaxIterations)
    {
        number=number+1;
        double Z_re2 = Z_re*Z_re, Z_im2 = Z_im*Z_im;
        
        if(Z_re2 + Z_im2 > ThresholdCheck)
        {
              code=number;
		      break;
        }
	    else
	    {
	       code=0;
	    }
        Z_im = 2*Z_re*Z_im + c_im;
        Z_re = Z_re2 - Z_im2 + c_re;
    }
    return code;
}

/*  
    This function is used to compute the fractal by calling the function computationMandel. The 
    function computeMandelbrot is activated when the thread is created. The inputs to this function 
    is obtained from the structure thread_data. So in the multi-threaded program, when a thread is created
    the thread_array data is passed as argument. All  the other information required by the 
    function is accessed from the globally accessible structure. 
*/
void *computeMandelbrot(void *threadarg)
{
  
  struct thread_data *my_data;
  my_data = (struct thread_data *) threadarg;
  int taskid = my_data->thread_id;
  int noOfPixelRows= my_data->pixelRows;
  
  /*printf("Thread number %ld\n", pthread_self());
  printf("Thread id %d\n", taskid);
  printf("Pixel Count %d\n", pixelCount);
  printf("noOfPixelRows %d\n", noOfPixelRows);*/

  int x,y;
  int startRow=noOfPixelRows-pixelCount;
  

  /*printf("StartRow= %d\n", startRow);
  printf("EndRow= %d\n", noOfPixelRows);
  printf("***\n");*/


  /*  Mutex stands for "mutual exclusion". Mutex variables are used here for implementing 
      thread synchronization and for protecting shared data when multiple writes occur. A mutex variable 
      acts like a "lock" protecting access to a shared data resource. 
      The basic concept of a mutex as used in Pthreads is that only one thread can lock (or own) 
      a mutex variable at any given time.  */
  pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;



  for(y=startRow; y<noOfPixelRows; ++y)
  {
        for(x=0; x<ImageWidth; ++x)
        {
            /*
                Perform the computation of mandelbrot and assign
                to color_code. 
            */
            unsigned int color_code = computationMandel(y, x, MaxIterations, ThresholdCheck, MaxIm, MinRe, Re_factor, Im_factor); 
            /*
                Lock the mutex prior to updating the value in the shared
                variable color_pixel, and unlock it upon updating.
            */ 
            pthread_mutex_lock(&mutex1);
            color_pixel[y][x] = color_code;
            pthread_mutex_unlock(&mutex1);
        }             
  }
}


/*
    Function to Create threads to perform the mandelbrot computation 
*/
int computeMandelbrotPThread()
{
  int i,j;
  pthread_t thread_id[nthreads]; 
  /* To divide the data among the threads, the pixel rows can
     be divided equally among the given number of threads */
    pixelCount=ImageHeight/nthreads;
  
  //printf ("pixelCount = %d.\n",pixelCount);
  
   /* Create threads to perform the mandelbrot computation */
   for(i=0; i < nthreads; i++)
   {
      
     pixelRows=pixelRows+pixelCount;
     thread_data_array[i].thread_id = i;
     thread_data_array[i].pixelRows= pixelRows;
   
     /* 
	   Each thread works on a different set of data.
	   The offset is specified by 'i'. The size of
	   the data for each thread is indicated by pixelRows.
	*/
     pthread_create(&thread_id[i], NULL, computeMandelbrot,(void *) &thread_data_array[i]);
   } 
   
    /* Wait on the other threads */
   for(j=0; j < nthreads; j++)
   {
      pthread_join( thread_id[j], NULL);
   }
    
   return 0;                  
}


/*
 * Function to plot the mandelbrot image
 */
int plotImageFile()
{
  const int MaxColorComponentValue=255; 
  FILE * fp;
  char *filename="mandelbrot_pthread.ppm";
  char *comment="# ";
  unsigned char color[3];
  fp= fopen(filename,"wb"); 
  fprintf(fp,"P6\n %s\n %d\n %d\n %d\n",comment,ImageWidth,ImageHeight,MaxColorComponentValue);
  int color_r_val,color_g_val,color_b_val=0;
  unsigned x,y;
  for(y=0; y<ImageHeight; ++y)
        for(x=0; x<ImageWidth; ++x)
        {
            if(color_pixel[y][x] == 0)
	       {
		        // interior of Mandelbrot set = black
		        color[0]=0;
            	color[1]=0;
            	color[2]=0;
 		        fwrite(color,1,3,fp);
            }
            else
	       { 
		      color_r_val=color_pixel[y][x];
              color_g_val=color_pixel[y][x];
              color_b_val=color_pixel[y][x];
		      color[0]=colour_r[color_r_val]; // Red
              color[1]=colour_g[color_g_val];  // Green 
              color[2]=colour_b[color_b_val]; // blue
              fwrite(color,1,3,fp);
            }
        }     
 return 0; 
}

/**
 * Main procedure of the program
 *
 * Obtains the input values as program arguments,
 * Computes the fractal by calling the function computeMandelbrot, 
 * Plots the image by calling the function plotImageFile.
 */
main(int argc, char *argv[])
{
  
    if (argc != 10)
    {
        printf ("Enter correct number of input arguments;\n The number of input arguments should be 9.\n" );
        exit(0);
    }else
    {
        MinRe = atof(argv[1]); // argument 1 - Xmin
        MaxRe = atof(argv[2]); // argument 2 - Xmax
        MinIm = atof(argv[3]); // argument 3 - Ymin
        MaxIm = atof(argv[4]); // argument 4 - Ymax
        ImageWidth = atoi(argv[5]); // argument 5  - ImageWidth 
        ImageHeight = atoi(argv[6]); // argument 6 - ImageHeight 
        MaxIterations = atoi(argv[7]); // argument 7 - MaxIterations 
        Threshold = atof(argv[8]); // argument 8 - Threshold 
        nthreads = atoi(argv[9]); // argument 9  - Number of threads 
    }  
    
    Re_factor = (MaxRe-MinRe)/(ImageWidth-1);
    Im_factor = (MaxIm-MinIm)/(ImageHeight-1);      
    ThresholdCheck=Threshold*Threshold;  
    MaxIm = MinIm+(MaxRe-MinRe)*ImageHeight/ImageWidth;
  
    color_pixel = ( unsigned int** )malloc( ImageHeight*sizeof( unsigned int* ) );
         int i;
         for(i = 0; i < ImageHeight; i++)
         {
              color_pixel[i] = ( unsigned int* )malloc( ImageWidth*sizeof( unsigned int) );
         }
 
    struct timeval t1,t2;
    gettimeofday(&t1,NULL);	
    computeMandelbrotPThread();
    gettimeofday(&t2,NULL);
    double time_diff=(t2.tv_sec-t1.tv_sec)+(double)(t2.tv_usec-t1.tv_usec)/1000000.0;
    printf("Mandelbrot PThread Time Difference:%f\n",time_diff);
  
    plotImageFile(); 
 
    printf("\nExiting.......");     
    exit(0);  
  
}








