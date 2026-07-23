#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#if defined(_WIN32) || defined(_WIN64)
	#define OSsep  '\\'
#else
	#define OSsep  '/'
#endif


bool _StringComp(const char *String1, const char *String2, int ExpSize, bool Ordered)//If ordered is set to true then "String1" is the reference and "String2" is the compared one 
{
	if(Ordered == true)
	{
		if(sizeof(String1)-1 != ExpSize)
		{
			perror("(_StringComp) size of reference different from expected size  Note: 0 Based Index");
		}
		if(sizeof(String2)-1 != ExpSize)
		{
			return false;
		}
	}
	bool Result = true;
	for(int i = 0; i <= ExpSize; i++)
	{
		if(String1[i] != String2[i])
		{
			Result = false;
		}
	}
	return Result;
}


int main(int argc, char *argv[])
{
	char PathUnchecked[200] = {0}; //Revise if size is sufficient
	if(argv[1] == NULL)
	{
		return 1;
	}
	snprintf(PathUnchecked,sizeof(PathUnchecked),argv[1]);
	
	for(int i =0; i <= sizeof(PathUnchecked) - 1; i++)
	{
		if(PathUnchecked[i] == '\\' || PathUnchecked[i] == '/')
		{
			PathUnchecked[i] = OSsep;
		}
	}
	
	FILE *XMLFile = fopen(PathUnchecked,"r");
	if(XMLFile == NULL)
	{
		goto defer;		
	}
	char ReadBuf[1] = {0};
	int line = 0;
	char Asterisk[1] = "*";
	while(fgets(ReadBuf,sizeof(ReadBuf),XMLFile))
	{
		line++;
		if(_StringComp(ReadBuf, Asterisk,0,false))
		{
			//Replace Logic
			printf("Line %d",line);// Just for testing
			return 0; // Just for testing
		}
	}
	
	
	defer:
		fclose(XMLFile);
		return 1;
}	

int ReadWriteTst()
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

