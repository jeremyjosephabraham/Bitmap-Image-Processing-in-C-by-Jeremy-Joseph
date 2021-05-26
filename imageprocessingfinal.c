/**
* @file imageprocessing final.c
*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define THRESHOLD1 40
#define MAX_VALUE 255
#define MAX_COLOR 255
#define BRIGHTNESS_FACTOR 25
#define WHITE 255
#define BLACK 0
#define THRESHOLD2 128

void blackwhite()
{
    FILE *fIn = fopen("images/lena512.bmp","r");				//Input File name
	FILE *fOut = fopen("blackwhite.bmp","w+");		            //Output File name

	int i;
	unsigned char byte[54];								//to get the image header
	unsigned char colorTable[1024];						//to get the colortable

	if(fIn==NULL)										// check if the input file has not been opened succesfully.
	{
		printf("File does not exist.\n");
	}

	for(i=0;i<54;i++)									//read the 54 byte header from fIn
	{
		byte[i]=getc(fIn);
	}

	fwrite(byte,sizeof(unsigned char),54,fOut);			//write the header back

	// extract image height, width and bitDepth from imageHeader
	int height = *(int*)&byte[18];
	int width = *(int*)&byte[22];
	int bitDepth = *(int*)&byte[28];

	printf("width: %d\n",width);
	printf("height: %d\n",height );

	int size=height*width;								//calculate image size

	if(bitDepth<=8)										//if ColorTable present, extract it.
	{
		fread(colorTable,sizeof(unsigned char),1024,fIn);
		fwrite(colorTable,sizeof(unsigned char),1024,fOut);
	}

	unsigned char buffer[size];							//to store the image data

	fread(buffer,sizeof(unsigned char),size,fIn);		//read image data

	for(i=0;i<size;i++)									//store 0(black) and 255(white) values to buffer
		{
			buffer[i] = (buffer[i] > THRESHOLD2) ? WHITE : BLACK;
		}

	fwrite(buffer,sizeof(unsigned char),size,fOut);		//write back to the output image

	if(fOut==NULL)							// check if the output file has not been opened succesfully.
	{
		printf("File does not exist.\n");
	}

	fclose(fIn);
	fclose(fOut);

}

void blurcolor()
{
    	clock_t start, stop;

	start = clock();						// Note the start time for profiling purposes.

	FILE *fIn = fopen("images/lena512.bmp","r");			// Input File name
	FILE *fOut = fopen("blurcolor.bmp","w+");		    	// Output File name

	int i,j,y, x;
	unsigned char byte[54];

	if(fIn==NULL)							// check if the input file has not been opened succesfully.
	{
		printf("File does not exist.\n");
	}

	for(i=0;i<54;i++)						// read the 54 byte header from fIn
	{
		byte[i] = getc(fIn);
	}

	fwrite(byte,sizeof(unsigned char),54,fOut);			// write the header back

	// extract image height, width and bitDepth from imageHeader
	int height = *(int*)&byte[18];
	int width = *(int*)&byte[22];
	int bitDepth = *(int*)&byte[28];

	printf("width: %d\n",width);
	printf("height: %d\n",height );

	int size = height*width;					// calculate the image size

	unsigned char buffer[size][3];					// store the input image data
	unsigned char out[size][3];					// store the output image data

	for(i=0;i<size;i++)						// read image data character by character
	{
		buffer[i][2]=getc(fIn);					// blue
		buffer[i][1]=getc(fIn);					// green
		buffer[i][0]=getc(fIn);					// red
	}

	float v=1.0 / 9.0;						// initialize the blurrring kernel
	float kernel[3][3]={{v,v,v},
						{v,v,v},
						{v,v,v}};

	for(x=1;x<height-1;x++)
	{
		for(y=1;y<width-1;y++)
		{
			float sum0= 0.0;
			float sum1= 0.0;
			float sum2= 0.0;
			for(i=-1;i<=1;++i)
			{
				for(j=-1;j<=1;++j)
				{
					// matrix multiplication with kernel with every color plane
					sum0=sum0+(float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][0];
					sum1=sum1+(float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][1];
					sum2=sum2+(float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][2];
				}
			}
			out[(x)*width+(y)][0]=sum0;
			out[(x)*width+(y)][1]=sum1;
			out[(x)*width+(y)][2]=sum2;
		}
	}

	for(i=0;i<size;i++)						//write image data back to the file
	{
		putc(out[i][2],fOut);
		putc(out[i][1],fOut);
		putc(out[i][0],fOut);
	}

	fclose(fOut);
	fclose(fIn);

	stop = clock();
	printf("\nCLOCKS_PER_SEC = %ld\n",stop-start); //1000000
	printf("%lf ms\n",((double)(stop-start) * 1000.0)/CLOCKS_PER_SEC );
}

void blurgrayscale()
{
    clock_t start, stop;
    int x;

	start = clock();											// Note the start time for profiling purposes.

	FILE *fIn = fopen("images/lena512.bmp","r");					//Input File name
	FILE *fOut = fopen("lena_gray.bmp","w+");		    		//Output File name

	int i,j,y;
	unsigned char byte[54];

	if(fIn==NULL)												// check if the input file has not been opened succesfully.
	{
		printf("File does not exist.\n");
	}

	for(i=0;i<54;i++)											//read the 54 byte header from fIn
	{
		byte[i] = getc(fIn);
	}

	fwrite(byte,sizeof(unsigned char),54,fOut);					//write the header back

	// extract image height, width and bitDepth from imageHeader
	int height = *(int*)&byte[18];
	int width = *(int*)&byte[22];
	int bitDepth = *(int*)&byte[28];

	printf("width: %d\n",width);
	printf("height: %d\n",height );

	int size = height*width;									//calculate image size

	unsigned char buffer[size],out[size];						//to store the image data

	float v=1.0 / 9.0;
	float kernel[3][3]={{v,v,v},								//initialize the blurrring kernel
						{v,v,v},
						{v,v,v}};

	fread(buffer,sizeof(unsigned char),size,fIn);				//read the image data into buffer

	for(i=0;i<size;i++)
	{
		out[i] = buffer[i];										//copy image data to out bufer
	}

	for(x=1;x<height-1;x++)
	{
		for(y=1;y<width-1;y++)
		{
			float sum= 0.0;
			for(i=-1;i<=1;++i)
			{
				for(j=-1;j<=1;++j)
				{
					sum=sum+(float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)];	//matrix multiplication with kernel
				}
			}
			out[(x)*width+(y)]=sum;
		}
	}

	fwrite(out,sizeof(unsigned char),size,fOut);				//write image data back to the file

	fclose(fIn);
	fclose(fOut);
}

void bright()
{
    FILE *fIn = fopen("images/lena512.bmp","r");			//Input File name
	FILE *fOut = fopen("lena_bright.bmp","w+");	    	//Output File name

	int i;
	unsigned char byte[54],colorTable[1024];

	if(fIn==NULL)						// check if the input file has not been opened succesfully.
	{
		printf("File does not exist.\n");
		exit(1);
	}

	for(i=0;i<54;i++)					//read the 54 byte header from fIn
	{
		byte[i] = getc(fIn);
	}

	fwrite(byte,sizeof(unsigned char),54,fOut);		//write the header back

	// extract image height, width and bitDepth from imageHeader
	int height = *(int*)&byte[18];
	int width = *(int*)&byte[22];
	int bitDepth = *(int*)&byte[28];

	printf("width: %d\n",width);
	printf("height: %d\n",height );

	int size = height*width;				//calculate image size

	if(bitDepth <= 8)					//if ColorTable present, extract it.
	{
		fread(colorTable,sizeof(unsigned char),1024,fIn);
		fwrite(colorTable,sizeof(unsigned char),1024,fOut);
	}


	unsigned char buffer[size];				//to store the image data
	int temp;

	fread(buffer,sizeof(unsigned char),size,fIn);		//read image data

	for(i=0;i<size;i++)					//increasing pixel values to get image bright
	{
		temp = buffer[i] + BRIGHTNESS_FACTOR;
		buffer[i] = (temp > MAX_COLOR) ? MAX_COLOR : temp;
	}

	fwrite(buffer,sizeof(unsigned char),size,fOut);		//write back to the output image

	fclose(fIn);
	fclose(fOut);
}

void colortosepia()
{
    clock_t start, stop;

	start = clock();											// Note the start time for profiling purposes.

	FILE *fIn = fopen("images/lena512.bmp","r");					//Input File name
	FILE *fOut = fopen("lena_sepia.bmp","w+");		    		//Output File name

	int i,r,g,b;
	unsigned char byte[54];

	if(fIn==NULL)												// check if the input file has not been opened succesfully.
	{
		printf("File does not exist.\n");
	}

	for(i=0;i<54;i++)											//read the 54 byte header from fIn
	{
		byte[i] = getc(fIn);
	}

	fwrite(byte,sizeof(unsigned char),54,fOut);					//write the header back

	// extract image height, width and bitDepth from imageHeader
	int height = *(int*)&byte[18];
	int width = *(int*)&byte[22];
	int bitDepth = *(int*)&byte[28];

	printf("width: %d\n",width);
	printf("height: %d\n",height );

	int size = height*width;									//calculate image size

	unsigned char buffer[size][3];								//to store the image data

	for(i=0;i<size;i++){
		r = 0;
		g = 0;
		b = 0;
		buffer[i][2] = getc(fIn);									//blue
		buffer[i][1] = getc(fIn);									//green
		buffer[i][0] = getc(fIn);									//red

		//conversion formula of rgb to sepia
		r = (buffer[i][0]*0.393) + (buffer[i][1]*0.769)	+ (buffer[i][2]*0.189);
		g = (buffer[
       i][0]*0.349) + (buffer[i][1]*0.686)	+ (buffer[i][2]*0.168);
		b = (buffer[i][0]*0.272) + (buffer[i][1]*0.534)	+ (buffer[i][2]*0.131);

		if(r > MAX_VALUE){											//if value exceeds
			r = MAX_VALUE;
		}
		if(g > MAX_VALUE){
			g = MAX_VALUE;
		}
		if(b > MAX_VALUE){
			b = MAX_VALUE;
		}
		putc(b,fOut);
		putc(g,fOut);
		putc(r,fOut);

	}


	fclose(fOut);
	fclose(fIn);

	stop = clock();
	printf("\nCLOCKS_PER_SEC = %ld\n",stop-start);
	printf("%lf ms\n",((double)(stop-start) * 1000.0)/CLOCKS_PER_SEC );

}

void copyimage()
{
    clock_t start, stop;

	start=clock(); // Note the start time for profiling purposes.

	FILE *fo = fopen("lena_copy.bmp","wb"); // Output File name

	int i;

	FILE *streamIn;
        streamIn = fopen("images/lena512.bmp", "r"); // Input file name

        if (streamIn == (FILE *)0) // check if the input file has not been opened succesfully.
	{
            printf("File opening error ocurred. Exiting program.\n");
            exit(0);
 	}

 	unsigned char header[54]; // to store the image header
	unsigned char colorTable[1024]; // to store the colorTable, if it exists.

 	int count = 0;
 	for(i=0;i<54;i++)
 	{
 		header[i] = getc(streamIn);  // strip out BMP header

 	}
 	int width = *(int*)&header[18]; // read the width from the image header
 	int height = *(int*)&header[22]; // read the height from the image header
	int bitDepth = *(int*)&header[28]; // read the bitDepth from the image header

	if(bitDepth <= 8)
		fread(colorTable, sizeof(unsigned char), 1024, streamIn);


	printf("width: %d\n",width);
	printf("height: %d\n",height );

	fwrite(header, sizeof(unsigned char), 54, fo); // write the image header to output file

 	unsigned char buf[height * width]; // to store the image data

	fread(buf, sizeof(unsigned char), (height * width), streamIn);

	if(bitDepth <= 8)
		fwrite(colorTable, sizeof(unsigned char), 1024, fo);

	fwrite(buf, sizeof(unsigned char), (height * width), fo);

	fclose(fo);
 	fclose(streamIn);

	stop = clock();

	printf("Time: %lf ms\n",((double)(stop - start) * 1000.0 )/ CLOCKS_PER_SEC);
}

void darkimage()
{
    FILE *fIn = fopen("images/lena512.bmp","r");					//Input File name
	FILE *fOut = fopen("lena_dark.bmp","w+");		    	//Output File name

	int i;
	unsigned char byte[54],colorTable[1024];

	if(fIn==NULL)											// check if the input file has not been opened succesfully.
	{
		printf("File does not exist.\n");
	}

	for(i=0;i<54;i++)										//read the 54 byte header from fIn
	{
		byte[i] = getc(fIn);
	}

	fwrite(byte,sizeof(unsigned char),54,fOut);				//write the header back

	// extract image height, width and bitDepth from imageHeader
	int height = *(int*)&byte[18];
	int width = *(int*)&byte[22];
	int bitDepth = *(int*)&byte[28];

	printf("width: %d\n",width);
	printf("height: %d\n",height );

	int size = height*width;								//calculate image size

	if(bitDepth <= 8)										//if ColorTable present, extract it.
	{
		fread(colorTable,sizeof(unsigned char),1024,fIn);
		fwrite(colorTable,sizeof(unsigned char),1024,fOut);
	}

	unsigned char buffer[size];								//to store the image data

	fread(buffer,sizeof(unsigned char),size,fIn);			//read image data

	for(i=0;i<size;i++)										//decreasing pixel values to get image bright
	{
		if(buffer[i] > THRESHOLD1)
			buffer[i] = buffer[i] - THRESHOLD1;
	}

	fwrite(buffer,sizeof(unsigned char),size,fOut);			//write back to the output image

	fclose(fIn);
	fclose(fOut);
}

void colortogray()
{
    clock_t start, stop;

	start = clock();											// Note the start time for profiling purposes.

	FILE *fIn = fopen("images/lena_color.bmp","r");					//Input File name
	FILE *fOut = fopen("lena_gray.bmp","w+");		    		//Output File name

	int i,j,y;
	unsigned char byte[54];

	if(fIn==NULL)												// check if the input file has not been opened succesfully.
	{
		printf("File does not exist.\n");
	}

	for(i=0;i<54;i++)											//read the 54 byte header from fIn
	{
		byte[i] = getc(fIn);
	}

	fwrite(byte,sizeof(unsigned char),54,fOut);					//write the header back

	// extract image height, width and bitDepth from imageHeader
	int height = *(int*)&byte[18];
	int width = *(int*)&byte[22];
	int bitDepth = *(int*)&byte[28];

	printf("width: %d\n",width);
	printf("height: %d\n",height );

	int size = height*width;									//calculate image size

	unsigned char buffer[size][3];								//to store the image data


	for(i=0;i<size;i++)											//RGB to gray
	{
		y=0;
		buffer[i][2]=getc(fIn);									//blue
		buffer[i][1]=getc(fIn);									//green
		buffer[i][0]=getc(fIn);									//red

		y=(buffer[i][0]*0.3) + (buffer[i][1]*0.59)	+ (buffer[i][2]*0.11);			//conversion formula of rgb to gray

		putc(y,fOut);
		putc(y,fOut);
		putc(y,fOut);
	}

	fclose(fOut);
	fclose(fIn);

	stop = clock();
	printf("\nCLOCKS_PER_SEC = %ld\n",stop-start);
	printf("%lf ms\n",((double)(stop-start) * 1000.0)/CLOCKS_PER_SEC );
}

void imagerotate()
{
    	FILE *fIn = fopen("images/lena512.bmp","r");					//Input File name
	FILE *fOut = fopen("lena_rotate.bmp","w+");		    	//Output File name

	int i,j,choice;
	unsigned char byte[54],colorTable[1024];

	if(fIn==NULL)											// check if the input file has not been opened succesfully.
	{
		printf("File does not exist.\n");
	}

	for(i=0;i<54;i++)										//read the 54 byte header from fIn
	{
		byte[i] = getc(fIn);
	}

	fwrite(byte,sizeof(unsigned char),54,fOut);				//write the header back

	// extract image height, width and bitDepth from imageHeader
	int height = *(int*)&byte[18];
	int width = *(int*)&byte[22];
	int bitDepth = *(int*)&byte[28];

	printf("width: %d\n",width);
	printf("height: %d\n",height );

	int size = height*width;								//calculate image size

	if(bitDepth <= 8)										//if ColorTable present, extract it.
	{
		fread(colorTable,sizeof(unsigned char),1024,fIn);
		fwrite(colorTable,sizeof(unsigned char),1024,fOut);
	}

	unsigned char buffer[width][height];					//to store the image data
	unsigned char out_buffer[width][height];

	fread(buffer,sizeof(unsigned char),size,fIn);			//read the image data

	printf("Enter your choice :\n");
	printf("1. Rotate right\n");
	printf("2. Rotate left\n");
	printf("3. Rotate 180\n");

	scanf("%d",&choice);

	switch(choice)												//to rotate image in 3 direction
	{
		case 1:
			for(i=0;i<width;i++)								//to rotate right
			{
				for(j=0;j<height;j++)
				{
					out_buffer[j][height-1-i]=buffer[i][j];
				}
			}
			break;
		case 2:
			for(i=0;i<width;i++)								//to rotate left
			{
				for(j=0;j<height;j++)
				{
					out_buffer[j][i]=buffer[i][j];
				}
			}
			break;
		case 3:
			for(i=0;i<width;i++)								//to rotate 180 degree
			{
				for(j=0;j<height;j++)
				{
					out_buffer[width-i][j]=buffer[i][j];
				}
			}
			break;
		default:
			break;
	}

	fwrite(out_buffer,sizeof(unsigned char),size,fOut);		//write back to the output image

	fclose(fIn);
	fclose(fOut);
}

void negativeimage()
{
    clock_t start, stop;
	start=clock();
	FILE* fp = fopen("images/lena512.bmp", "rb");   //read the file//

	unsigned char *imageData; // to store the image information
	unsigned char *newimageData; // to store the new image information, i.e. the negative image
        unsigned char imageHeader[54]; // to get the image header
        unsigned char colorTable[1024]; // to get the colortable

	int i,j; // loop counter variables

	fread(imageHeader, sizeof(unsigned char), 54, fp); // read the 54-byte from fp to imageHeader


	// extract image height and width from imageHeader
        int width = *(int*)&imageHeader[18];
        int height = *(int*)&imageHeader[22];
	int bitDepth = *(int*)&imageHeader[28];

        int imgDataSize = width * height; // calculate image size

        imageData = (unsigned char*)malloc (imgDataSize * sizeof(unsigned char)); // allocate the block of memory as big as the image size
        newimageData = (unsigned char*)malloc (imgDataSize * sizeof(unsigned char));

	if(bitDepth <= 8)	// COLOR TABLE Present
		fread(colorTable, sizeof(unsigned char), 1024, fp); // read the 1024-byte from fp to colorTable


	fread( imageData, sizeof(unsigned char), imgDataSize, fp);


	//Calculate the mean of the image
	for(i = 0; i < height; i++){
	      for(j = 0; j < width; j++){
		     newimageData[i*width + j] = 255 - imageData[i*width + j];
		 }
	}

	FILE *fo = fopen("lena_negative.bmp", "wb");

        fwrite(imageHeader, sizeof(unsigned char), 54, fo); // write the header back.

	if(bitDepth <= 8)	// COLOR TABLE Present
        	fwrite(colorTable, sizeof(unsigned char), 1024, fo); // write the color table back

        fwrite( newimageData, sizeof(unsigned char), imgDataSize, fo); // write the values of the negative image.

        fclose(fo);
	fclose(fp);
        stop = clock();
	double d = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
        printf("%lf\n",d);
}

int main()
{   int a=-1;
    while(a!=0)
    {

    int a;
    printf("\t\t\t\t All the changes will be done based on the file  images/lena512.bmp \n\n\n\n\n");
    printf("\t\t\t\t\t\t\tMENU\n\n");
    printf("1.Convert image to black and white\n");
    printf("2.Convert image to blur\n");
    printf("3.convert grayscale image to blur\n");
    printf("4.Brighten image\n");
    printf("5.Convert image to sepia\n");
    printf("6.Create a copy of the image \n");
    printf("7.Darken image\n");
    printf("8.Convert RGB image to grayscale\n");
    printf("9.rotate image by 180 degrees\n");
    printf("10.Convert image to negative\n");
    printf("11.Exit");
    printf("\t\t\t Please enter your choice \n");
    scanf("%d",&a);


    switch(a)
    {
        case 1: blackwhite(); break;
        case 2: blurcolor(); break;
        case 3: blurgrayscale(); break;
        case 4: bright(); break;
        case 5: colortosepia(); break;
        case 6: copyimage(); break;
        case 7: darkimage(); break;
        case 8: colortogray(); break;
        case 9: imagerotate(); break;
        case 10: negativeimage(); break;
        default: exit(0); break;
     }
    }
return 0;
}
