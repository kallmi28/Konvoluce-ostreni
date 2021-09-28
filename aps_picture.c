#include <stdio.h>
#include <stdlib.h>



#define min(a, b) (a > b) ? b : a

#define PIXELMAGIC(a) ((a > 255) ? 255 : (a < 0) ? 0 : a)

struct Pixel
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
};



int main(int argc, char const *argv[])
{
	int width = 0, height = 0, bits = 0;
	char type[2];

	int v1 = 0, v2 = 0;
	struct Pixel * inputStore; 
	struct Pixel * outputStore; 
	FILE *histogram;

	FILE *in, *out;

	unsigned int hist [6] = {0};
	int Y = 0;
	int size;

	if(argc <= 1) 
	{
		double xx = 3.14;
		double ee = 10;
		printf("%.1f, %.1f", xx, ee);
		printf("Missing input file.\n");
		return 1;
	}

	in = fopen(argv[1], "rb");

	v1 = fscanf(in, "%s %d %d %d ", type, &width, &height, &bits);

	if(v1 != 4)
	{
		printf("Problem with input file.\n");
		return 1;
	}
	out = fopen("output.ppm", "wb");

	fprintf(out, "%s\n%d\n%d\n%d\n", type, width, height, bits);

	size = width * height;

	inputStore = (struct Pixel*) malloc (sizeof(struct Pixel) * size);
	outputStore = (struct Pixel*) malloc (sizeof(struct Pixel) * size);



	v2 = fread((char*) &inputStore[0], 1, size * 3, in);
	if(v2 == -1)
	{
		printf("Problem with reading.\n");
		return 1;
	}

	for (int i = 0; i < (width * height); ++i) 
	{
		int aktX = i / width, aktY = i - (aktX * width);
		
		if (aktX != 0 && aktY != 0 && aktX != height - 1 && aktY != width - 1) 
		{
			int sharpenPixelR = inputStore[i].R * 5 - inputStore[i - width].R - inputStore[i + width].R - inputStore[i + 1].R - inputStore[i - 1].R;
			int sharpenPixelG = inputStore[i].G * 5 - inputStore[i - width].G - inputStore[i + width].G - inputStore[i + 1].G - inputStore[i - 1].G;
			int sharpenPixelB = inputStore[i].B * 5 - inputStore[i - width].B - inputStore[i + width].B - inputStore[i + 1].B - inputStore[i - 1].B;
			

			sharpenPixelR = PIXELMAGIC(sharpenPixelR);
			sharpenPixelG = PIXELMAGIC(sharpenPixelG);
			sharpenPixelB = PIXELMAGIC(sharpenPixelB);
									

			outputStore[i].R = sharpenPixelR;
			outputStore[i].G = sharpenPixelG;
			outputStore[i].B = sharpenPixelB;

			Y = (int)(0.2126 * sharpenPixelR + 0.7152 * sharpenPixelG + 0.0722 * sharpenPixelB + 0.5);
		}

		else
		{
			outputStore[i].R = inputStore[i].R;
			outputStore[i].G = inputStore[i].G;
			outputStore[i].B = inputStore[i].B;

			Y = (int)(0.2126 * inputStore[i].R + 0.7152 * inputStore[i].G + 0.0722 * inputStore[i].B + 0.5);
		}

		hist[Y/51]++;
	}

	fwrite (&outputStore[0] , sizeof(struct Pixel), size, out);

	histogram = fopen("output.txt", "w");
	fprintf(histogram, "%d %d %d %d %d", hist[0], hist[1], hist[2], hist[3], hist[4] + hist[5]);

	fclose(out);
	fclose(in);
	fclose(histogram);


	free(outputStore);
	free(inputStore);



	return 0;
}