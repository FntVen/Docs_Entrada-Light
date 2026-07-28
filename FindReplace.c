//Necessary Arguments: (1)Path-to-Xml, (2)Name-of-The-Final-File(No extension), (3)Which document is being made
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
typedef struct
{
	char NClient[256];//Nome do Cliente
	char CClient[256];//Código da Concessionaria do Cliente
	char CInstalação[256];//Código da Instalação da Concessionaria
	char QModulos[256];//Quantidade de Módulos
	char QInversores[256];//Quantidade de Inversores
	char MModulos[256];//Marca dos Módulos
	char MModelo[256];//Modelo dos Módulos
	char IFabricantes[256];//Marca do Inversores
	char IModelo[256];//Modelo dos Inversores
	char PInversores[256];//Potência dos Inversores
	char PModulos[256];//Potência dos Módulos
	char CData[256];//Data de Instalação
	char TodayData[256];//Data de Criação dos Documentos
	char Inst_Sub[256];//Se a Instalação é Subterrânea (Se For Aerea Fazer "Inst_Sub" == "" no documento)
	char Inst_Aero[256];//Se a Instalação é Aerea (Se For Subterrânea Fazer "Inst_Aero" == "" no documento)
	char Inst_Classe[256];//Qual classe do cliente "Grupo A", "Grupo B"
	char CCabo[256];//Diametro do cabo
	char CDisjuntor[256];//Amperagem do Disjuntor
	char CTerra[256];//Diametro do cabo de Aterramento
	char CKWh[256];//KWh da instalação
	char CEst[256];//Estrutura da instalação (Ceramico, Fibrocimento etc)
	char PKit[256];//Potência em Kwp de todo o Sistema
	char AArranjos[256];// Area total dos arranjos
	char CPF_CNPJ[256];//Cpf ou Cnpj do Cliente
	char CEndereço[256];//Rua do Cliente
	char CBairro[256];//Bairro do Cliente
	char CCep[256];//Cep do Cliente
	char CNumero[256];//Número do Endereço do Cliente
	char CCidade[256];//Cidade do Cliente/Instalação
	char CEstado[256];//Estado do Cliente/Instalação
	char CEmail[256];//Email do Cliente
	char CTel[256];//Telefone do Cliente
}ClientData;

ClientData TestFillData()
{
	ClientData TestData;
	snprintf(TestData.NClient,sizeof(TestData.NClient),"NomedoClienteTeste");
	snprintf(TestData.CClient,sizeof(TestData.CClient),"CodigodoClienteTeste");
	snprintf(TestData.CInstalação,sizeof(TestData.CInstalação),"CódigodeInstalaçãoTeste");
	snprintf(TestData.QModulos,sizeof(TestData.QModulos),"QuantidadeTesteModulos");
	snprintf(TestData.QInversores,sizeof(TestData.QInversores),"QuantidadeTesteInversores");
	snprintf(TestData.PModulos,sizeof(TestData.PModulos),"PotênciaModuloTeste");
	snprintf(TestData.PInversores,sizeof(TestData.PInversores),"PotênciaInversorTeste");
	snprintf(TestData.IFabricantes,sizeof(TestData.IFabricantes),"FabricanteTesteInversor");
	snprintf(TestData.IModelo,sizeof(TestData.IModelo),"ModeloTesteInversor");
	snprintf(TestData.MModulos,sizeof(TestData.MModulos),"MarcaTesteModulos");
	snprintf(TestData.MModelo,sizeof(TestData.MModelo),"ModeloTesteModulos");
	snprintf(TestData.PInversores,sizeof(TestData.PInversores),"PotenciaTesteInversores");
	snprintf(TestData.CData,sizeof(TestData.CData),"00/00/2030");
	snprintf(TestData.TodayData,sizeof(TestData.TodayData),"To/Da/YY");
	snprintf(TestData.Inst_Sub,sizeof(TestData.Inst_Sub),"X!");
	snprintf(TestData.Inst_Aero,sizeof(TestData.Inst_Aero),"Y!");
	snprintf(TestData.Inst_Classe,sizeof(TestData.Inst_Classe),"Classe-N/A");
	snprintf(TestData.CCabo,sizeof(TestData.CCabo),"Tst-Cabo");
	snprintf(TestData.CDisjuntor,sizeof(TestData.CDisjuntor),"AMPtst");
	snprintf(TestData.CTerra,sizeof(TestData.CTerra),"Tst-Terra");
	snprintf(TestData.CKWh,sizeof(TestData.CKWh),"Kwh-tst");
	snprintf(TestData.CCabo,sizeof(TestData.CCabo),"Tst-Cabo");
	snprintf(TestData.CEst,sizeof(TestData.CEst),"Estrutura-Teste");
	snprintf(TestData.PKit,sizeof(TestData.PKit),"PotênciaKit-Teste");
	snprintf(TestData.AArranjos,sizeof(TestData.AArranjos),"Area-Teste");
	snprintf(TestData.CPF_CNPJ,sizeof(TestData.CPF_CNPJ),"000.000.000-00");
	snprintf(TestData.CEndereço,sizeof(TestData.CEndereço),"Rua-Teste");
	snprintf(TestData.CBairro,sizeof(TestData.CBairro),"Bairro-Teste");
	snprintf(TestData.CNumero,sizeof(TestData.CNumero),"_420");
	snprintf(TestData.CCidade,sizeof(TestData.CCidade),"Cidade-Teste");
	snprintf(TestData.CEstado,sizeof(TestData.CEstado),"Estado-Teste");
	snprintf(TestData.CCep,sizeof(TestData.CCep),"RealCepTotally-00");
	snprintf(TestData.CEmail,sizeof(TestData.CEmail),"RealEmailForeal@realhandle.com");
	snprintf(TestData.CTel,sizeof(TestData.CTel),"21994206969");
	return TestData;
}

bool _StringComp(const char *String1, const char *String2, int ExpSize, bool Ordered)
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
}//If ordered is set to true then "String1" is the reference and "String2" is the compared one

R_ReadBuffer ReadCompare(const char *ToReplace)
{
	//possible strings to replace
	char NClient[8] = "NClient";
	char CClient[7] = "CClient";
	char CInstalação[14] = "CInstalação";
	char QModulos[9] = "QModulos";
	char QInversores[12] = "QInversores";
	char MModulos[9] = "MModulos";
	char MModelo[8] = "MModulos";
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
	snprintf(Answer.String, sizeof(Answer.String), "Not Found");
	bool FoundFlag = false;
	FoundFlag = _StringComp(NClient, ToReplace,(int)sizeof(NClient) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), NClient);
		return Answer;
	}
	FoundFlag = _StringComp(QInversores, ToReplace,(int)sizeof(QInversores) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), QInversores);
		return Answer;
	}
	FoundFlag = _StringComp(MModelo, ToReplace,(int)sizeof(MModelo) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), MModelo);
		return Answer;
	}
	FoundFlag = _StringComp(CClient, ToReplace,(int)sizeof(CClient) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CClient);
		return Answer;
	}
	FoundFlag = _StringComp(CInstalação, ToReplace,(int)sizeof(CInstalação) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CInstalação);
		return Answer;
	}
	FoundFlag = _StringComp(QModulos, ToReplace,(int)sizeof(QModulos) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), QModulos);
		return Answer;
	}
	FoundFlag = _StringComp(MModulos, ToReplace,(int)sizeof(MModulos) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), MModulos);
		return Answer;
	}
	FoundFlag = _StringComp(PModulos, ToReplace,(int)sizeof(PModulos) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), PModulos);
		return Answer;
	}
	FoundFlag = _StringComp(IFabricantes, ToReplace,(int)sizeof(IFabricantes) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), IFabricantes);
		return Answer;
	}
	FoundFlag = _StringComp(IModelo, ToReplace,(int)sizeof(IModelo) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), IModelo);
		return Answer;
	}
	FoundFlag = _StringComp(PInversores, ToReplace,(int)sizeof(PInversores) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), PInversores);
		return Answer;
	}
	FoundFlag = _StringComp(CData, ToReplace,(int)sizeof(CData) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CData);
		return Answer;
	}
	FoundFlag = _StringComp(TodayData, ToReplace,(int)sizeof(TodayData) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), TodayData);
		return Answer;
	}
	FoundFlag = _StringComp(Inst_Sub, ToReplace,(int)sizeof(Inst_Sub) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), Inst_Sub);
		return Answer;
	}
	FoundFlag = _StringComp(Inst_Aero, ToReplace,(int)sizeof(Inst_Aero) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), Inst_Aero);
		return Answer;
	}
	FoundFlag = _StringComp(Inst_Classe, ToReplace,(int)sizeof(Inst_Classe) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), Inst_Classe);
		return Answer;
	}
	FoundFlag = _StringComp(CCabo, ToReplace,(int)sizeof(CCabo) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CCabo);
		return Answer;
	}
	FoundFlag = _StringComp(CDisjuntor, ToReplace,(int)sizeof(CDisjuntor) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CDisjuntor);
		return Answer;
	}
	FoundFlag = _StringComp(CTerra, ToReplace,(int)sizeof(CTerra) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CTerra);
		return Answer;
	}
	FoundFlag = _StringComp(CKWh, ToReplace,(int)sizeof(CKWh) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CKWh);
		return Answer;
	}
	FoundFlag = _StringComp(CEst, ToReplace,(int)sizeof(CEst) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CEst);
		return Answer;
	}
	FoundFlag = _StringComp(PKit, ToReplace,(int)sizeof(PKit) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), PKit);
		return Answer;
	}
	FoundFlag = _StringComp(AArranjos, ToReplace,(int)sizeof(AArranjos) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), AArranjos);
		return Answer;
	}
	FoundFlag = _StringComp(CPF_CNPJ, ToReplace,(int)sizeof(CPF_CNPJ) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CPF_CNPJ);
		return Answer;
	}
	FoundFlag = _StringComp(CCep, ToReplace,(int)sizeof(CCep) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CCep);
		return Answer;
	}
	FoundFlag = _StringComp(CEndereço, ToReplace,(int)sizeof(CEndereço) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CEndereço);
		return Answer;
	}
	FoundFlag = _StringComp(CBairro, ToReplace,(int)sizeof(CBairro) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CBairro);
		return Answer;
	}
	FoundFlag = _StringComp(CNumero, ToReplace,(int)sizeof(CNumero) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CNumero);
		return Answer;
	}
	FoundFlag = _StringComp(CCidade, ToReplace,(int)sizeof(CCidade) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CCidade);
		return Answer;
	}
	FoundFlag = _StringComp(CEstado, ToReplace,(int)sizeof(CEstado) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CEstado);
		return Answer;
	}
	FoundFlag = _StringComp(CEmail, ToReplace,(int)sizeof(CEmail) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CEmail);
		return Answer;
	}
	FoundFlag = _StringComp(CTel, ToReplace,(int)sizeof(CTel) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CTel);
		return Answer;
	}

	return Answer;
}//Should return what trigger word was found and if anything was found using a struct

void EraseUnfinished(const char *FileName, const char *DocumentRequested)
{
    char Del_Xml[60] = {0};
    char Del_Docx[60] = {0};
    char Del_Zip[60] = {0};
    char Del_CopyDir[60] = {0};
 if(OSsep == '\\')
 {
     snprintf(Del_Docx, sizeof(Del_Docx),"del %s.docx",FileName);
     snprintf(Del_Zip, sizeof(Del_Zip),"del %s.zip",FileName);
     snprintf(Del_Xml, sizeof(Del_Xml),"del document.xml");
     snprintf(Del_CopyDir, sizeof(Del_CopyDir),"del Arquivos\\Tozip\\_rels Arquivos\\Tozip\\customXml Arquivos\\Tozip\\docProps Arquivos\\Tozip\\word Arquivos\\Tozip\\[Content_Types].xml");
 }
 else
 {
     snprintf(Del_Docx, sizeof(Del_Docx),"rm %s.docx",FileName);
     snprintf(Del_Zip, sizeof(Del_Zip),"rm %s.zip",FileName);
     snprintf(Del_Xml, sizeof(Del_Xml),"rm document.xml");
     snprintf(Del_CopyDir, sizeof(Del_CopyDir),"rm Arquivos\\Tozip\\_rels Arquivos\\Tozip\\customXml Arquivos\\Tozip\\docProps Arquivos\\Tozip\\word Arquivos\\Tozip\\[Content_Types].xml");
 }
}//Erase whatever was already made

bool MakeWord(const char *FileName, const char *DocumentRequested)
{
    char Co_ZipBuf[140] = {0};
    char Co_CopyBuf[80] = {0};
    char Co_MoveBuf[60] = {0};
    char Co_RenameBuf[256] = {0};//Part of the string is a variable of unknow size
    char Co_DeleteBuf[60] = {0};

    if(OSsep == '\\')// tar -caf File.zip path/one path/two.xml //No need to quotes if there is not space in the relative path //We can also do -v if we wanto to show whats being compressed for a progress screen or log
    {
        snprintf(Co_CopyBuf,sizeof(Co_CopyBuf),"xcopy Arquivos\\Unzipped\\%s Arquivos\\Tozip /s /e",DocumentRequested);
        snprintf(Co_ZipBuf,sizeof(Co_ZipBuf),"tar -caf %s.zip Arquivos\\Tozip\\_rels Arquivos\\Tozip\\customXml Arquivos\\Tozip\\docProps Arquivos\\Tozip\\word Arquivos\\Tozip\\[Content_Types].xml",DocumentRequested);
        snprintf(Co_DeleteBuf, sizeof(Co_DeleteBuf), "del Arquivos\\Tozip\\word\\document.xml");
        snprintf(Co_MoveBuf, sizeof(Co_MoveBuf), "move document.xml Arquivos\\Tozip\\_rels Arquivos\\Tozip\\word");
        snprintf(Co_RenameBuf, sizeof(Co_RenameBuf),"move %s.zip %s.docx",FileName,FileName);
    }
    else
    {
        snprintf(Co_MoveBuf,sizeof(Co_MoveBuf),"mv document.xml Arquivos/Tozip/_rels Arquivos/Tozip/word");
        snprintf(Co_RenameBuf, sizeof(Co_MoveBuf),"mv %s.zip %s.docx",FileName,FileName);
        snprintf(Co_DeleteBuf,sizeof(Co_DeleteBuf),"rm Arquivos/Tozip/word/document.xml");
        snprintf(Co_ZipBuf,sizeof(Co_ZipBuf),"zip -r %s.zip Arquivos/Tozip/_rels Arquivos/Tozip/customXml Arquivos/Tozip/docProps Arquivos/Tozip/word Arquivos/Tozip/[Content_Types].xml",DocumentRequested);
        snprintf(Co_CopyBuf, sizeof(Co_CopyBuf),"cp -R Arquivos/Unzipped/%s Arquivos/Tozip", DocumentRequested);
    }
    int ReturnCodes[5] = {0};
    ReturnCodes[0] = system(Co_CopyBuf);    //Copy template to somewere to work in
    ReturnCodes[1] = system(Co_DeleteBuf);  //Delete document.xml from template
    ReturnCodes[2] = system(Co_MoveBuf);    //Move modified document.xml to it's position in the template
    ReturnCodes[3] = system(Co_ZipBuf);     //zip new document
    ReturnCodes[4] = system(Co_RenameBuf);  //Make zip into docx
    bool Result = false;
    for(int i = 0; i<= sizeof(ReturnCodes) - 1; i++)
    {
        if(ReturnCodes[i] == -1 || ReturnCodes[i] == 0)
        {
            Result = true;
        }
        else
        {
            Result = false;
            EraseUnfinished(FileName,DocumentRequested);
            break;
        }
    }
    return Result;
}//Copy template folder system -> send modified xml into the copied template -> compact copied template folders and files -> rename to .docx

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
	FILE *XMLWrite = fopen("document.xml","w");
	if(XMLWrite == NULL || XMLFile == NULL)
	{
	    goto defer;
	}

	char ReadBuf[2] = {0};  // Character reading from the template xml file
	int CharacterPos = 0;   // For debugging
	int ReadCharacters = 0; // How many characters read post FoundFlag
	char Asterisk[2] = {0}; // 1 Based
	snprintf(Asterisk,sizeof(Asterisk),"*");
	bool FoundFlag = false;
	char ReadReplace[100] = {0};

	while(fgets(ReadBuf,sizeof(ReadBuf),XMLFile))
	{
		CharacterPos++;
		if(FoundFlag == false)
		{
            if(_StringComp(ReadBuf, Asterisk,0,false))
      		{
     			FoundFlag = true;
     			printf("Line %d",CharacterPos);// Just for testing
                continue;
      		}
      		if(CharacterPos == 2147483646 && Debugging)
      		{
     			printf("Never Recognized");
     			goto defer;
      		}
        fprintf(XMLWrite, "%s", ReadBuf);
		}
		else
		{
            ReadReplace[ReadCharacters] = ReadBuf[0];
            ReadCharacters++;
            R_ReadBuffer Answer = ReadCompare(ReadReplace);
            if(Answer.Result == true)
            {
                FoundFlag = false;
                //Write string in Answers.String
                fprintf(XMLWrite,"%s",Answer.String);
            }
            else
            {
                goto defer;
            }
		}
	}
	fclose(XMLFile);
	fclose(XMLWrite);
	MakeWord(argv[2], argv[3]);

	defer:
		fclose(XMLFile);
		fclose(XMLWrite);
		return 1;
}
