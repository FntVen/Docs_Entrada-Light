#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64)
	#define OSsep  "\\"
#else
	#define OSsep  "/"

int main(int argc, char *argv[])
{
	char PathUnchecked[200] = {0}; //Revise if size is sufficient
	snprinf(PathUnchecked,sizeof(PathUnchecked),argv[1]);
	
	for(int i =0; i <= sizeof(PathUnchecked) - 1; i++)
	{
		if(PathUnchecked[i] == "\\" || PathUnchecked[i] == "/")
		{
			PathUnchecked[i] = OSsep;
		}
	}
	
	FILE *XMLFile = fopen(snprinf("%s",PathUnchecked),"r");
	if(XMLFile == NULL)
	{
		goto defer;
		return 1;
	}
	char ReadBuf[256] = {0};
	while(fgets(ReadBuf,sizeof(ReadBuf),XMLFile))
	{
		
	}
	
	
	
	defer:
		fclose(XMLFile);
	
}	

void StructInit()//Initialize the structs default values
{
	
}
//Each struct will hold an array of ints that each will represent multiple instances of positions of the same to-be-replaced elements in their respectives .xml files
typedef struct 
{
	
}MDPosition

typedef struct 
{
	
}OCPosition

typedef struct 
{
	
}MRPosition

typedef struct 
{
	
}SAPosition

void ReadWriteTst()
{
	FILE *XMLfile = fopen("TesteFileRead.txt","r");
	if(XMLfile == NULL)
	{
		return 1;
	}
	char BufXML[256] = {0};
	FILE *TextFinal = fopen("FinalText.txt","w");
	while(fgets(BufXML,sizeof(BufXML),XMLfile))
	{
		fprintf(TextFinal,"%s",BufXML);
	}
	fclose(TextFinal);
	return 0;
}
