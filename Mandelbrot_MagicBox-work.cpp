#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include <omp.h>
#include <queue>

/*
  This program provides a magic box algorithm implementation that generates Mandelbrot fractals. 
*/


using namespace std;
double ThresholdCheck,Threshold;  
int ImageWidth,ImageHeight, MaxIterations;
double MinRe, MaxRe, MinIm, MaxIm;
int nthreads,chunk=10,tid;

int colour_r[512]={0,15,20,25,30,35,40,45,50,55,60,65,70,75,80,90,92,95,100,105,110,115,120,125,130,135,140,150,160,170,180,190,200,210,220,230,240,250,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,250,240,230,220,210,200,190,180,170,160,150,140,130,120,100,90,80,75,70,65,60,55,50,45,40,35,30,25,20,18,16,14,12,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int colour_g[512]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,255,254,253,252,251,250,249,248,247,246,245,244,243,242,241,240,239,238,237,236,235,234,233,232,231,230,228,226,224,222,220,218,216,214,212,210,208,206,204,202,200,198,196,194,192,190,188,186,184,182,180,178,176,174,172,170,168,166,164,162,160,158,156,154,152,150,148,146,144,142,140,138,136,134,132,130,128,126,124,122,120,118,116,114,112,110,108,106,104,102,100,98,96,94,92,90,88,86,84,82,80,78,76,74,72,70,68,66,64,62,60,58,56,54,52,50,48,46,44,42,40,38,36,34,32,30,28,26,24,22,20,18,16,14,12,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int colour_b[512]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,12,14,17,20,25,30,35,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,35,30,25,20,15,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,90,92,94,96,98,100,102,104,106,108,110,112,114,116,118,120,122,124,126,128,130,132,134,136,138,140,142,144,146,148,150,152,154,156,158,160,162,164,166,168,170,172,174,176,178,180,182,184,186,188,190,192,194,196,198,200,202,204,206,208,210,212,214,216,218,220,222,224,226,228,230,232,234,236,238,240,242,244,246,248,250,252,254,255,255,253,251,249,247,245,243,241,239,237,235,233,231,229,227,225,223,221,219,217,215,213,211,209,207,205,203,201,199,197,195,193,191,189,187,185,183,181,179,177,175,173,171,169,167,165,163,161,159,157,155,153,151,149,147,145,143,141,139,137,135,133,131,129,127,125,123,121,119,117,115,113,111,109,107,105,103,101,99,97,95,93,91,89,87,85,83,81,79,77,75,73,71,69,67,65,63,61,59,57,55,53,51,49,47,45,43,41,39,37,35,33,31,29,27,25,23,21,19,17,15,13,11,9,7,5,3};

unsigned int **color_pixel= NULL;
unsigned int code = 0;
unsigned int color_code=0;
int boxsize=0;

double Z_im,Z_re=0;

double Re_factor,Im_factor;

struct borderPoints
{
    int point[4];
};

queue<borderPoints*> magicQueue;

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
 * Function to compute the fractal by calling the function computationMandel. 
 */
int computeMandelbrot()
{
    int y,x;
    /*  Beginning of parallel section. Fork a team of threads.
        The variable scoping is specified. Here each thread will 
        have a private tid,x,y,color_code variable. The nthreads,chunk
        will be shared by all threads*/
    #pragma omp parallel shared(nthreads,chunk) private(tid,x,y,color_code)
    {
        /* Parallel section executed by all threads */
        tid = omp_get_thread_num();
        
        /* Loop iterations are divided into pieces of size chunk, 
        and dynamically scheduled among the threads; 
        when a thread finishes one chunk, it is dynamically assigned another.*/
        #pragma omp for schedule(dynamic,chunk)
        for(y=0; y<ImageHeight; ++y)
        {
             for(x=0; x<ImageWidth; ++x)
             {
               //printf("Thread %d -> Value of x: %d , y: %d in calculateColorCode\n",tid,x,y);
               unsigned int color_code = computationMandel(y, x, MaxIterations, ThresholdCheck, MaxIm, MinRe, Re_factor, Im_factor);
               //printf("Thread %d -> Value of color_code: %d in calculateColorCode\n",tid,color_code);
               color_pixel[y][x] = color_code;
            }
        } 
        /* All threads join master thread and disband */              
    }
   return 0;                  
}

/* */
//computeMagicBox(0, 0, ImageHeight, ImageWidth, MaxIterations, ThresholdCheck, MaxIm, MinRe, Im_factor, Re_factor, true, boxsize);
//bool computeMagicBox(int Ys, int Xs, int Ye, int Xe, int MI, int ThresholdSquard, double Ymax, double Xmin, double Ifactor, double Rfactor, bool bForceSplit = false, int boxSize = 1)
bool computeMagicBox(int yAxisMin, int xAxisMin, int yAxisMax, int xAxisMax, int MaxIterations, 
                     double ThresholdCheck, double Ymax, double Xmin, double Im_factor, double Re_factor, bool force = false, int boxsize = 1)
{
    /*printf("\n computeMagicBox : in");
    printf("\n xAxisMin : %d",xAxisMin);
    printf("\n xAxisMax : %d",xAxisMax);
    printf("\n yAxisMin : %d",yAxisMin);
    printf("\n yAxisMax : %d",yAxisMax);
    printf("\n MaxIterations : %d",MaxIterations);
    printf("\n ThresholdCheck : %d",ThresholdCheck);
    printf("\n Ymax : %f",Ymax);
    printf("\n Xmin : %f",Xmin);
    printf("\n Im_factor : %f",Im_factor);
    printf("\n Re_factor : %f",Re_factor);*/
    
    bool splitImage = false;
    

    int code = -1;

    for(int i = yAxisMin; i < yAxisMax; i++)
    {
        if(code<0)
        {
            code = computationMandel(i, xAxisMin, MaxIterations, ThresholdCheck, Ymax, Xmin, Im_factor, Re_factor);
            /*printf("\n if1");
            printf("\n 1.i : %d",i);
            printf("\n 1.xAxisMin : %d",xAxisMin);
            printf("\n 1.code : %d",code);*/
        }
        if(code != computationMandel(i, xAxisMin, MaxIterations, ThresholdCheck, Ymax, Xmin, Im_factor, Re_factor))
        {
            /*printf("\n if2");
            printf("\n 2.i : %d",i);
            printf("\n 2.xAxisMin : %d",xAxisMin);
            printf("\n 2.code : %d",code);*/
            splitImage = true;
            break;
        }
        if(code != computationMandel(i, xAxisMax, MaxIterations, ThresholdCheck, Ymax, Xmin, Im_factor, Re_factor))
        {
            /*printf("\n if3");
            printf("\n 3.i : %d",i);
            printf("\n 3.xAxisMax : %d",xAxisMin);
            printf("\n 3.code : %d",code);*/
            splitImage = true;
            break;
        }
    }



    for(int j = xAxisMin; j < xAxisMax; j++)
    {
        if(code<0)
        {
            code = computationMandel(yAxisMin, j, MaxIterations, ThresholdCheck, Ymax, Xmin, Im_factor, Re_factor);
            /*printf("\n if1");
            printf("\n 1.j : %d",j);
            printf("\n 1.yAxisMin : %d",yAxisMin);
            printf("\n 1.code : %d",code);*/
        }

        if(code != computationMandel(yAxisMin, j, MaxIterations, ThresholdCheck, Ymax, Xmin, Im_factor, Re_factor))
        {
            /*printf("\n if2");
            printf("\n 2.j : %d",j);
            printf("\n 2.yAxisMin : %d",yAxisMin);
            printf("\n 2.code : %d",code);*/
            splitImage = true;
            break;
        }

        if(code != computationMandel(yAxisMax, j, MaxIterations, ThresholdCheck, Ymax, Xmin, Im_factor, Re_factor))
        {
            /*printf("\n if3");
            printf("\n 3.j : %d",j);
            printf("\n 3.yAxisMax : %d",yAxisMax);
            printf("\n 3.code : %d",code);*/
            splitImage = true;
            break;
        }
    }
    

    if(code >= 0 && !splitImage && !force)
    {
            for(int y = yAxisMin;  y< yAxisMax; y++)
            {
                    for(int x = xAxisMin; x < xAxisMax; x++)
                    {
                        /*printf("\n\n if loop code");
                        printf("\n yAxisMin : %d",yAxisMin);
                        printf("\n yAxisMax : %d",yAxisMax);
                        printf("\n xAxisMin : %d",xAxisMin);
                        printf("\n xAxisMax : %d",xAxisMax);
                        printf("\n code : %d",code);
                        printf("\n ************* ");*/
                        color_pixel[y][x] = code;
                    }
            }
    }
    else
    {
        int ylen = yAxisMax - yAxisMin;
        int xlen = xAxisMax - xAxisMin;

        if(ylen > boxsize && xlen > boxsize)
        {
            int ymidpt = yAxisMin + (int)((ylen)/2);
            int xmidpt = xAxisMin + (int)((xlen)/2);

            #pragma omp critical
            {
                struct borderPoints *borderPt;
                borderPt = new borderPoints;
                borderPt->point[0] = yAxisMin; 
                borderPt->point[1] = xAxisMin; 
                borderPt->point[2] = ymidpt; 
                borderPt->point[3] = xmidpt;
                magicQueue.push(borderPt);

                borderPt = new borderPoints;
                borderPt->point[0] = yAxisMin; 
                borderPt->point[1] = xmidpt; 
                borderPt->point[2] = ymidpt; 
                borderPt->point[3] = xAxisMax;
                magicQueue.push(borderPt);

                borderPt = new borderPoints;
                borderPt->point[0] = ymidpt; 
                borderPt->point[1] = xAxisMin; 
                borderPt->point[2] = yAxisMax; 
                borderPt->point[3] = xmidpt;
                magicQueue.push(borderPt);

                borderPt = new borderPoints;
                borderPt->point[0] = ymidpt; 
                borderPt->point[1] = xmidpt; 
                borderPt->point[2] = yAxisMax; 
                borderPt->point[3] = xAxisMax;
                magicQueue.push(borderPt);
            }
        }
        else
        {
            for(int y = yAxisMin; y < yAxisMax; y++)
            {
                for(int x = xAxisMin; x < xAxisMax; x++)
                {
                    color_pixel[y][x] = computationMandel(y, x, MaxIterations, ThresholdCheck, Ymax, Xmin, Im_factor, Re_factor);
                }
            }
        }
    }

    if(force)
        splitImage = force;


    return splitImage;
}


/*
    Function used to implement the magic box algorithm. 
*/
int computeMagicBoxAlgm(int boxsize)
{
    printf("\n computeMagicBoxAlgm : in ");
    printf("\n boxsize=%d",boxsize);
    
    int y =0, x= 0, count = 0;

     struct borderPoints *borderPt = NULL;
     computeMagicBox(0, 0, ImageHeight, ImageWidth, MaxIterations, ThresholdCheck, MaxIm, MinRe, Im_factor, Re_factor, true, boxsize);
     
     
    #pragma omp parallel shared(MaxIterations, ThresholdCheck, MaxIm, MinRe, Im_factor, Re_factor, magicQueue, boxsize) private(borderPt) num_threads(nthreads)
    {
            int queuesize = magicQueue.size();
            while(queuesize > 0)
            {
                    int yAxisMin = 0, xAxisMin = 0, yAxisMax = 0, xAxisMax = 0;
                    #pragma omp critical
                    {
                             borderPt = magicQueue.front();
                             if(borderPt)
                             {
                                    magicQueue.pop();
                                    yAxisMin = borderPt->point[0]; 
                                    xAxisMin = borderPt->point[1]; 
                                    yAxisMax = borderPt->point[2]; 
                                    xAxisMax = borderPt->point[3];
                                    delete borderPt;
                                    borderPt = NULL;
                                    count++;
                            }
                    }
                    computeMagicBox( yAxisMin, xAxisMin, yAxisMax, xAxisMax, MaxIterations, ThresholdCheck, MaxIm, MinRe, Im_factor, Re_factor, false, boxsize);

                    queuesize = magicQueue.size();
           }
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
  char *filename="mandelbrot_magic.ppm";
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
  
    if (argc != 11)
    {
        printf ("Enter correct number of input arguments;\n The number of input arguments should be 10.\n" );
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
        boxsize = atoi(argv[10]); // argument 10  - Box size 
    }  
    
    // Set the number of threads as the user given number
    omp_set_num_threads(nthreads);
    
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
    computeMagicBoxAlgm(boxsize);
    gettimeofday(&t2,NULL);
    double time_diff=(t2.tv_sec-t1.tv_sec)+(double)(t2.tv_usec-t1.tv_usec)/1000000.0;
    printf("\nMandelbrot OpenMP Time Difference:%f\n",time_diff);
  
    plotImageFile(); 
 
    printf("\nExiting.......");     
    exit(0);  
  
}








