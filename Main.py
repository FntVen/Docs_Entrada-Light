import os
from pathlib import Path

# This is a project to expedite my learning of a graphical interface, and to help me in work
# List of Front End Options
# - Go (Apparently just went to shit)
# - Flutter (I know could work, but i'm trying something new and maybe more my vibe)
# - Tkinter (Could work but I dont like the idea of Python going into production, too many nightmares trying to make libraries compile AOT)
# - React Native (From the class i snuck in, it seems simple enough, but apparently performance is shit - Which is another worry i also have about flutter)
# - Vulkan (Jesus... As a way for EVERY PROJECT??)
# - OpenGL (Easier - I assume - But still seems like too much for an simple interface maker)
# -
#
# To-do
# 1) Certify that all data is populated | 2) Select which type o request this is (OC or etc) | 3) Open necessary files and edit the xml | 4) re-zip and rename
# Registered Info

Nome, Telefone, Cpf_Cnpj, Endereco, End_Estado, End_Cidade, End_Bairro, End_Num, End_Cep, Uc = ""

Coordinates_WS = ""
Coordinates = ""
Entrada = ["Subterranêa", "Aérea"]
EndNum_Avaliable = True

Docs = ["OC","Memorial","Entrada"]#Oc = Formulario de Orçamento de Conexão | Memorial = Memorial Descritivo | Entrada = Formulario para entrada
Docs_Necessarios = []

Home = Path.cwd()
Docs_paths = [Path.joinpath(Home,"Arquivos","Unzipped", "Orçamento de Conexão"),Path.joinpath(Home,"Arquivos","Unzipped","Memorial Descritivo") ,Path.joinpath(Home,"Arquivos","Unzipped","MicroGeração")]
Path_Necessarios = []

def Coordinates_Maps():
    return ""

def Test_PopulateData():
    Nome = "Test Nome"
    Telefone = "21999999990"
    Cpf_Cnpj = "22266688803"
    Endereco = "Rua Test Rome"
    End_Estado = "Tst_RJ"
    End_Cidade = "City_Test"
    End_Bairro = "Tst_Bairro"
    if EndNum_Avaliable:
        End_Num = "69"
    End_Cep = "420666-99"
    Uc = "99999999"
    Docs_Necessarios = Docs
    Path_Necessarios = Docs_paths

def FindReplace(DocumentData_XML:str):
    MasterString = ""
    return MasterString
    
def Fill_Data():
    if Path_Necessarios != []:
        if Path_Necessarios.__contains__("OC"):
            DirPath = Docs_paths[0]
            with open(Path.joinpath(DirPath,"word","document.xml"),"r") as Document:
                Ct = Document.read()
        if Path_Necessarios.__contains__("Memorial"):
            DirPath = Docs_paths[1]
            with open(Path.joinpath(DirPath,"word","document.xml"),"r") as Document:
                Ct = Document.read()
        if Path_Necessarios.__contains__("Entrada"):
            DirPath = Docs_paths[2]
            with open(Path.joinpath(DirPath,"word","document.xml"),"r") as Document:
                Ct = Document.read()
    else:
        return "Diga os Documentos Necessários"

Test_PopulateData()
