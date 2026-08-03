//Necessary Arguments: (1)Path-to-Xml, (2)Name-of-The-Final-File(No extension), (3)Which document is being made
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>//Implement a native buffer clear to get rid of this

#if defined(_WIN32) || defined(_WIN64)
	#define OSsep  '\\'
#else
	#define OSsep  '/'
#endif

int ReturnCode = 0;
bool Error = false;

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
	char IEst[256];//Estrutura da instalação (Ceramico, Fibrocimento etc)
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

/*Helper Functions*/
static bool StrComp(const char *String1, const char *String2, int ExpSize, bool DebugUse)
{
	bool Result = true;
	for(int i = 0; i <= ExpSize; i++)
	{
		if (DebugUse)
		{
			printf("Comparing String1: %c and String2 %c \n",String1[i],String2[i]);
		}
		if(String1[i] != String2[i])
		{
			Result = false;
		}
	}
	return Result;
}//If ordered is set to true then "String1" is the reference and "String2" is the compared one
static void MemClear()
{
}
/*Active Functions*/
ClientData TestFillData()
{
	ClientData TestData;
	snprintf(TestData.NClient,sizeof(TestData.NClient),"NomedoClienteTeste");
	snprintf(TestData.CClient,sizeof(TestData.CClient),"CodigodoClienteTeste");
	snprintf(TestData.CInstalação,sizeof(TestData.CInstalação),"CodigodeInstalaçãoTeste");
	snprintf(TestData.QModulos,sizeof(TestData.QModulos),"QuantidadeTesteModulos");
	snprintf(TestData.QInversores,sizeof(TestData.QInversores),"QuantidadeTesteInversores");
	snprintf(TestData.PModulos,sizeof(TestData.PModulos),"PotenciaModuloTeste");
	snprintf(TestData.PInversores,sizeof(TestData.PInversores),"PotenciaInversorTeste");
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
	snprintf(TestData.IEst,sizeof(TestData.IEst),"Estrutura-Teste");
	snprintf(TestData.PKit,sizeof(TestData.PKit),"PotenciaKit-Teste");
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

R_ReadBuffer ReadCompare(const char *ToReplace)
{
	//possible strings to replace
	char NClient[8] = "NClient";
	char CClient[8] = "CClient";
	char CInstalação[12] = "CInstalacão";
	char QModulos[9] = "QModulos";
	char QInversores[12] = "QInversores";
	char MModulos[9] = "MModulos";
	char MModelo[8] = "MModelo";
	char PModulos[9] = "PModulos";
	char IFabricantes[13] = "IFabricantes";
	char IModelo[8] = "IModelo";
	char PInversores[12] = "PInversores";
	char CData[6] = "CData";
	char TodayData[10] = "TodayData";
	char Inst_Sub[9] = "Inst_Sub";
	char Inst_Aero[10] = "Inst_Aero";
	char Inst_Classe[12] = "Inst_Classe";
	char CCabo[6] = "CCabo";
	char CDisjuntor[11] = "CDisjuntor";
	char CTerra[7] = "CTerra";
	char CKWh[5] = "CKWh";
	char IEst[5] = "IEst";
	char PKit[5] = "PKit";
	char AArranjos[10] = "AArranjos";
	char CPF_CNPJ[9] = "CPF_CNPJ";
	char CCep[5] = "CCep";
	char CEndereço[10] = "CEndereco";
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
	FoundFlag = StrComp(NClient, ToReplace,(int)sizeof(NClient) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), NClient);
		return Answer;
	}

	FoundFlag = StrComp(QInversores, ToReplace,(int)sizeof(QInversores) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), QInversores);
		return Answer;
	}
	FoundFlag = StrComp(MModelo, ToReplace,(int)sizeof(MModelo) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), MModelo);
		return Answer;
	}
	FoundFlag = StrComp(CClient, ToReplace,(int)sizeof(CClient) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CClient);
		return Answer;
	}
	FoundFlag = StrComp(CInstalação, ToReplace,(int)sizeof(CInstalação) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CInstalação);
		return Answer;
	}
	FoundFlag = StrComp(QModulos, ToReplace,(int)sizeof(QModulos) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), QModulos);
		return Answer;
	}
	FoundFlag = StrComp(MModulos, ToReplace,(int)sizeof(MModulos) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), MModulos);
		return Answer;
	}
	FoundFlag = StrComp(PModulos, ToReplace,(int)sizeof(PModulos) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), PModulos);
		return Answer;
	}
	FoundFlag = StrComp(IFabricantes, ToReplace,(int)sizeof(IFabricantes) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), IFabricantes);
		return Answer;
	}
	FoundFlag = StrComp(IModelo, ToReplace,(int)sizeof(IModelo) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), IModelo);
		return Answer;
	}
	FoundFlag = StrComp(PInversores, ToReplace,(int)sizeof(PInversores) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), PInversores);
		return Answer;
	}
	FoundFlag = StrComp(CData, ToReplace,(int)sizeof(CData) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CData);
		return Answer;
	}
	FoundFlag = StrComp(TodayData, ToReplace,(int)sizeof(TodayData) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), TodayData);
		return Answer;
	}
	FoundFlag = StrComp(Inst_Sub, ToReplace,(int)sizeof(Inst_Sub) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), Inst_Sub);
		return Answer;
	}
	FoundFlag = StrComp(Inst_Aero, ToReplace,(int)sizeof(Inst_Aero) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), Inst_Aero);
		return Answer;
	}
	FoundFlag = StrComp(Inst_Classe, ToReplace,(int)sizeof(Inst_Classe) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), Inst_Classe);
		return Answer;
	}
	FoundFlag = StrComp(CCabo, ToReplace,(int)sizeof(CCabo) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CCabo);
		return Answer;
	}
	FoundFlag = StrComp(CDisjuntor, ToReplace,(int)sizeof(CDisjuntor) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CDisjuntor);
		return Answer;
	}
	FoundFlag = StrComp(CTerra, ToReplace,(int)sizeof(CTerra) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CTerra);
		return Answer;
	}
	FoundFlag = StrComp(CKWh, ToReplace,(int)sizeof(CKWh) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CKWh);
		return Answer;
	}
	FoundFlag = StrComp(IEst, ToReplace,(int)sizeof(IEst) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), IEst);
		return Answer;
	}
	FoundFlag = StrComp(PKit, ToReplace,(int)sizeof(PKit) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), PKit);
		return Answer;
	}
	FoundFlag = StrComp(AArranjos, ToReplace,(int)sizeof(AArranjos) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), AArranjos);
		return Answer;
	}
	FoundFlag = StrComp(CPF_CNPJ, ToReplace,(int)sizeof(CPF_CNPJ) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CPF_CNPJ);
		return Answer;
	}
	FoundFlag = StrComp(CCep, ToReplace,(int)sizeof(CCep) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CCep);
		return Answer;
	}
	FoundFlag = StrComp(CEndereço, ToReplace,(int)sizeof(CEndereço) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CEndereço);
		return Answer;
	}
	FoundFlag = StrComp(CBairro, ToReplace,(int)sizeof(CBairro) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CBairro);
		return Answer;
	}
	FoundFlag = StrComp(CNumero, ToReplace,(int)sizeof(CNumero) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CNumero);
		return Answer;
	}
	FoundFlag = StrComp(CCidade, ToReplace,(int)sizeof(CCidade) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CCidade);
		return Answer;
	}
	FoundFlag = StrComp(CEstado, ToReplace,(int)sizeof(CEstado) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CEstado);
		return Answer;
	}
	FoundFlag = StrComp(CEmail, ToReplace,(int)sizeof(CEmail) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CEmail);
		return Answer;
	}
	FoundFlag = StrComp(CTel, ToReplace,(int)sizeof(CTel) - 1,false);
	if(FoundFlag)
	{
	    Answer.Result = true;
	    snprintf(Answer.String, sizeof(Answer.String), CTel);
		return Answer;
	}
	Deb:
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

void WriteReplacement(FILE *WriteFile, const ClientData Data, const char *FoundString)//bug - comparing the wrong things always results in no writes
{
	char tempbuf[40] = {0};
	snprintf(tempbuf, sizeof(tempbuf),"%s","CClient");
    if(StrComp(FoundString, tempbuf, 7, false))
    {
        printf("Found! CClient\n");
        fprintf(WriteFile,Data.CClient);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"CData");
    if(StrComp(FoundString, tempbuf, 5, false))
    {
        printf("Found! CData\n");
        fprintf(WriteFile,Data.CData);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"TodayData");
    if(StrComp(FoundString, tempbuf, 9, false))
    {
        printf("Found! TodayData\n");
        fprintf(WriteFile,Data.TodayData);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"AArranjos");
    if(StrComp(FoundString, tempbuf,9, false))
    {
        printf("Found! AArranjos\n");
        fprintf(WriteFile,Data.AArranjos);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"CBairro");
    if(StrComp(FoundString, tempbuf,7, false))
    {
        printf("Found! CBairro\n");
        fprintf(WriteFile,Data.CBairro);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"CCabo");
    if(StrComp(FoundString, tempbuf, 5, false))
    {
        printf("Found! CCabo\n");
        fprintf(WriteFile,Data.CCabo);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"CCep");
    if(StrComp(FoundString, tempbuf, 4, false))
    {
        printf("Found! CCep\n");
        fprintf(WriteFile,Data.CCep);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"CDisjuntor");
    if(StrComp(FoundString, tempbuf, 10, false))
    {
        printf("Found! CDisjunto\n");
        fprintf(WriteFile,Data.CDisjuntor);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"CCidade");
    if(StrComp(FoundString, tempbuf, 7, false))
    {
        printf("Found! CCidade\n");
        fprintf(WriteFile,Data.CCidade);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"CEmail");
    if(StrComp(FoundString, tempbuf, 6, false))
    {
        printf("Found! CEmail\n");
        fprintf(WriteFile,Data.CEmail);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"CEndereco");
    if(StrComp(FoundString, tempbuf, 9, false))
    {
        printf("Found! CEndereço\n");
        fprintf(WriteFile,Data.CEndereço);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"IEst");
    if(StrComp(FoundString, tempbuf, 4, false))
    {
        printf("Found! IEst\n");
        fprintf(WriteFile,Data.IEst);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"CEstado");
    if(StrComp(FoundString, tempbuf, 7, false))
    {
        printf("Found! CEstado\n");
        fprintf(WriteFile,Data.CEstado);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"CInstalação");
    if(StrComp(FoundString, tempbuf, 11, false))
    {
        printf("Found! CInstalação\n");
        fprintf(WriteFile,Data.CInstalação);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"CKWh");
    if(StrComp(FoundString, tempbuf, 4, false))
    {
        printf("Found! CKWh\n");
        fprintf(WriteFile,Data.CKWh);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"CNumero");
    if(StrComp(FoundString, tempbuf, 7, false))
    {
        printf("Found! CNumero\n");
        fprintf(WriteFile,Data.CNumero);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"CPF_CNPJ");
    if(StrComp(FoundString, tempbuf, 8, false))
    {
        printf("Found! CPF_CNPJ\n");
        fprintf(WriteFile,Data.CPF_CNPJ);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"CTel");
    if(StrComp(FoundString, tempbuf, 4, false))
    {
        printf("Found! CTel\n");
        fprintf(WriteFile,Data.CTel);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"CTerra");
    if(StrComp(FoundString, tempbuf, 6, false))
    {
        printf("Found! CTerra\n");
        fprintf(WriteFile,Data.CTerra);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"IFabricantes");
    if(StrComp(FoundString, tempbuf, 12, false))
    {
        printf("Found! IFabricantes\n");
        fprintf(WriteFile,Data.IFabricantes);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"IModelo");
    if(StrComp(FoundString, tempbuf, 7, false))
    {
        printf("Found! IModelo\n");
        fprintf(WriteFile,Data.IModelo);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"Inst_Aero");
    if(StrComp(FoundString, tempbuf, 9, false))
    {
        printf("Found! Inst_Aero\n");
        fprintf(WriteFile,Data.Inst_Aero);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"Inst_Sub");
    if(StrComp(FoundString, tempbuf, 8, false))
    {
        printf("Found! Inst_Sub\n");
        fprintf(WriteFile,Data.Inst_Sub);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"Inst_Classe");
    if(StrComp(FoundString, tempbuf, 11, false))
    {
        printf("Found! Inst_Classe\n");
        fprintf(WriteFile,Data.Inst_Classe);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"MModelo");
    if(StrComp(FoundString, tempbuf, 7, false))
    {
        printf("Found! MModelo\n");
        fprintf(WriteFile,Data.MModelo);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"MModulos");
    if(StrComp(FoundString, tempbuf, 8, false))
    {
        printf("Found! MModulos\n");
        fprintf(WriteFile,Data.MModulos);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"NClient");
    if(StrComp(FoundString, tempbuf, 7, false))
    {
        printf("Found! NClient \n");
        fprintf(WriteFile,Data.NClient);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"PInversores");
    if(StrComp(FoundString, tempbuf, 11, false))
    {
        printf("Found! PInversores\n");
        fprintf(WriteFile,Data.PInversores);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"PModulos");
    if(StrComp(FoundString, tempbuf, 8, false))
    {
        printf("Found! PModulos\n");
        fprintf(WriteFile,Data.PModulos);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"QInversores");
    if(StrComp(FoundString, tempbuf, 11, false))
    {
        printf("Found! QInversores\n");
        fprintf(WriteFile,Data.QInversores);
    	return;
    }
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"PKit");
	if(StrComp(FoundString, tempbuf, 4, true))
	{
		printf("Found! PKit\n");
		fprintf(WriteFile,Data.PKit);
		return;
	}
	memset(tempbuf, 0, sizeof(tempbuf));
	snprintf(tempbuf, sizeof(tempbuf),"QModulos");
    if(StrComp(FoundString, tempbuf, 8, false))
    {
        printf("Found! QModulos\n");
        fprintf(WriteFile,Data.QModulos);
    }
}

void ReplaceMedia()
{
	//Replace the data in the template docx using the MasterMedia folder and what needs to be provided by the user can be saved in the folders where the document.xml is located
}

int main(int argc, char *argv[])
{
	char PathUnchecked[200] = {0};
	if(argv[1] == NULL)
	{
        printf("Invalid First Argument  (Expected: path to xml) \n");
		return 1;
	}
	snprintf(PathUnchecked,sizeof(PathUnchecked),argv[1]);
	printf("Path Unchecked in buffer: %s \n",PathUnchecked);
	for(int i =0; i <= sizeof(PathUnchecked) - 1; i++)
	{
		if(PathUnchecked[i] == '\\' || PathUnchecked[i] == '/')
		{
			PathUnchecked[i] = OSsep;
		}
	}
	printf("Path Checked in buffer: %s \n",PathUnchecked);
	FILE *XMLFile = fopen(PathUnchecked,"r");

	//Copy Structure of docx file
	char Co_CopyBufToBuild[80] = {0};
	if(OSsep == '\\')
	{
		snprintf(Co_CopyBufToBuild,sizeof(Co_CopyBufToBuild),"xcopy  Unzipped%c%s Tozip /s /e",OSsep,argv[3]);
	}
	else
	{
		snprintf(Co_CopyBufToBuild,sizeof(Co_CopyBufToBuild),"cp -R Unzipped%c%s Tozip",OSsep, argv[3]);
	}
	system(Co_CopyBufToBuild);
	char DocXmlbuf[25] = {0};
	snprintf(DocXmlbuf,sizeof(DocXmlbuf),"Tozip%cword%cdocument.xml",OSsep,OSsep);
	printf("Write Path: %s \n",DocXmlbuf);//For some reason the first .xml doesn't register
	FILE *XMLWrite = fopen(DocXmlbuf,"w");
	if(XMLWrite == NULL || XMLFile == NULL)
	{
	    printf("Error Opening or creating XML file \n");
		Error = true;
		ReturnCode = 1;
	    goto defer;
	}

	char ReadBuf[2] = {0};  // Character reading from the template xml file
	uint32_t CharacterPos = 0;   // For debugging
	int ReadCharacters = 0; // How many characters read post FoundFlag
	char Asterisk[2] = {0}; // 1 Based
	snprintf(Asterisk,sizeof(Asterisk),"*");
	bool FoundFlag = false;
	char ReadReplace[20] = {0};

	while(fgets(ReadBuf,sizeof(ReadBuf),XMLFile) == ReadBuf)
	{
	    //printf("In fgets at character %d \n",CharacterPos);
		//printf("ReadBuf : %s \n",ReadBuf);
		CharacterPos++;
		if(FoundFlag == false)
		{
            if(StrComp(ReadBuf, Asterisk,0,false))
      		{
     			FoundFlag = true;
     			printf("Found * in position %d \n",CharacterPos);// Just for testing
                continue;
      		}
      		if(CharacterPos == 2147483646 && Debugging)
      		{
     			printf("Never Recognized");
      			Error = true;
      			ReturnCode = 1;
     			goto defer;
      		}
        fprintf(XMLWrite, "%s", ReadBuf);
		}
		else
		{
		if(CharacterPos == 2147483646 && Debugging)
      	{
     		printf("Too high!");
			Error = true;
			ReturnCode = 1;
     		goto defer;
      	}
            ReadReplace[ReadCharacters] = ReadBuf[0];
            ReadCharacters++;
            printf("ReadReplace: ");
            printf("%s \n",ReadReplace);
            R_ReadBuffer Answer = ReadCompare(ReadReplace);
			printf("Result found in 'Answer': %d | And string is: %s \n",Answer.Result,Answer.String);
            if(Answer.Result == true)
            {
                FoundFlag = false;
                ClientData Data = TestFillData();
                printf("Searching... \n");
                WriteReplacement(XMLWrite, Data, Answer.String);
                memset(ReadReplace, 0, sizeof(ReadReplace));//Clear buffer for next element to read
				ReadCharacters = 0;
            }
		}
	}
	if(ferror(XMLFile) == true)
	{
		printf("Error reading XMLWrite file \n");
		Error = true;
		ReturnCode = 1;
		goto defer;
	}
	if(ferror(XMLWrite) == true)
	{
		printf("Error writing to XMLWrite file \n");
		Error = true;
		ReturnCode = 1;
		goto defer;
	}
	defer:
		printf("Defer");
		fclose(XMLFile);
		fclose(XMLWrite);
		if (Error)
		{
			return ReturnCode;
		}
	printf("MakeDocx in \n");
	// Zip --> Rename/Move
	char Co_ZipBuf[140] = {0};
	char Co_MoveBufToOutput[80] = {0};
	if(OSsep == '\\')//	Windows
	{
		snprintf(Co_ZipBuf,sizeof(Co_ZipBuf),"tar -caf %s.zip -C Tozip *",argv[2]);
		snprintf(Co_MoveBufToOutput,sizeof(Co_MoveBufToOutput),"move %s.zip %s.docx",argv[2],argv[2]);//Eventually make exit path modular
	}
	else//	Mac/Linux
	{
		snprintf(Co_ZipBuf,sizeof(Co_ZipBuf),"cd Tozip && zip -r ..%c%s.zip",OSsep,argv[2]);
		snprintf(Co_MoveBufToOutput,sizeof(Co_MoveBufToOutput),"mv %s.zip %s.docx",argv[2],argv[2]);
	}
	fclose(XMLWrite);
	system(Co_ZipBuf);
	system(Co_MoveBufToOutput);
	return ReturnCode;
}

/*		 To-do
 * ° CClient not showing - Fixed
 * ° PKit not showing - Fixed
 * ° CKWh not showing - Fixed
 * ° CCabos not showing - Fixed
 * ° CTerra not showing - Fixed
 * ° Erro no CEstado? (Conflito com CEst que define estruturas) - Fixed (Agora é IEst)
 * ° Introduzir espaços no XML em certos pontos para formatação - Fixed
 * ° TodayData na primeira pagina não foi detectado em geral - Fixed
 * ° Copy command copies parent folder (Windows) - Fixed
 * ° Error in one of the cmd commands syntax (Windows) - Fixed
 */
