import os
#This is a project to expedite my learning of a graphical interface, and to help me in work
# List of Front End Options
# - Go (Apparently just went to shit)
# - Flutter (I know could work, but i'm trying something new and maybe more my vibe)
# - Tkinter (Could work but I dont like the idea of Python going into production, too many nightmares trying to make libraries compile AOT)
# - React Native (From the class i snuck in, it seems simple enough, but apparently performance is shit - Which is another worry i also have about flutter)
# - Vulkan (Jesus... As a way for EVERY PROJECT??)
# - OpenGL (Easier - I assume - But still seems like too much for an simple interface maker) 
# - 
# 
#To-do
#1) Certify that all data is populated | 2) Select which type o request this is (OC or etc) | 3) Open necessary files and edit the xml | 4) re-zip and rename
#Registered Info
Nome, Telefone, Cpf_Cnpj,Endereco, End_Estado, End_Cidade, End_Bairro, End_Num,End_Cep, Uc = ""
Coordinates_WS = ""
Coordinates = ""
Entrada = ["Subterranêa","Aérea"]

def Coordinates_Maps():
    return ""
    
def Test_PopulateData():    
    EndNum_Avaliable = True
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
    
    
Test_PopulateData()