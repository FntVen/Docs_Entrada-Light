#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#if defined(_WIN32) || defined(_WIN64)
	#define OSsep  '\\'
#else
	#define OSsep  '/'
#endif

#define Debugging true

typedef struct
{
    bool Result;
    char String[100];
}R_ReadBuffer;

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

R_ReadBuffer ReadCompare(const char *ToReplace)
{
	//possible strings to replace
	char NClient[8] = "NClient";
	char Client[7] = "Client";
	char CInstalação[12] = "CInstalação";
	char QModulos[9] = "QModulos";
	char MModulos[9] = "MModulos";
	char PModulos[9] = "PModulos";
	char IFabricantes[13] = "IFabricantes";
	char IModelo[8] = "IModelo";
	char PInversores[12] = "PInversores";
	char CData[11] = "CData";
	char TodayData[11] = "TodayData";
	char Inst_Sub[9] = "Inst_Sub";
	char Inst_Aero[10] = "Inst_Aero";
	char Inst_Classe[12] = "Inst_Classe";
	char CCabo[6] = "CCabo";
	char CDisjuntor[11] = "CDisjuntor";
	char CTerra[7] = "CTerra";
	char CKWh[5] = "CKWh"; 
	char CEst[5] = "CEst";
	char PKit[5] = "PKit";
	char AArranjos[10] = "AArranjos";
	char CPF_CNPJ[9] = "CPF_CNPJ";
	char CCep[5] = "CCep";
	char CEndereço[10] = "CEndereço";
	char CBairro[8] = "CBairro";
	char CNumero[8] = "CNumero";
	char CCidade[8] = "CCidade";
	char CEstado[8] = "CEstado";
	char CEmail[7] = "CEmail";
	char CTel[5] = "CTel";
	
	//Check
	R_ReadBuffer Answer;
	Answer.Result = false;
	Answer.String = "";
	
	_StringComp(NClient, ToReplace,(int)sizeof(NClient) - 1,false);//Repeat for every variable
	
	
	return Answer;
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
	char ReadBuf[2] = {0};
	int CharacterPos = 0; // For debugging
	int ReadCharacters = 0; //How many characters read post FoundFlag
	char Asterisk[2] = {0};// 1 Based
	snprintf(Asterisk,sizeof(Asterisk),"*");
	bool FoundFlag = false;
	char ReadReplace[100] = {0};
	while(fgets(ReadBuf,sizeof(ReadBuf),XMLFile))
	{
		CharacterPos++;
		if(_StringComp(ReadBuf, Asterisk,0,false) && FoundFlag == false)
		{
			FoundFlag = true;
			printf("Line %d",CharacterPos);// Just for testing
		}
		if(CharacterPos == 2147483646 && Debugging)
		{
			printf("Never Recognized");
			goto defer;
		}
		if(FoundFlag)
		{
		    ReadReplace[ReadCharacters] = ReadBuf[0];
			R_ReadBuffer Answer = ReadCompare();
		    ReadCharacters++;
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
