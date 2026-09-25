const FileStuff = require("node:fs")
var URL = "https://api.crm.solarz.com.br/api/v2/open-api/deal?startDate=SDate&endDate=TodayDate&page=0&size=Quantidade&sort=%5B%22string%22%5D&pipelineId=ID";
var INI_Date = "";//EX: 2025-01-01
var Data = new Date();
var DataDay = Data.getDay();
var DataMonth = Data.getMonth();
var DataYear = Data.getFullYear();
if(DataMonth < 10)
{
  DataMonth = "0"+DataMonth;
}
if(DataDay < 10)
{
  DataDay = "0"+DataDay;
}
var TODAY_Date = DataYear+"-"+DataMonth+"-"+DataDay; //Ex: 2026-09-03
console.log(TODAY_Date);
var Limit = ""; //EX: 341 (Can be a ridiculous high number like 999)
var FunilID = "";//EX: 6462
var StageID = "";
/*
Closer:6462 (Prospect:35924 1TP_ligação:35926 2TP_Whatsapp:35927 3TP_Ligação2X:35928 4TP_WhatsappNutrição:35929 5TP_Whatsapp:35930 6TP_Ligação2X:35931 7TP_WhatsappBreakOff:35932 LeadQualificado:35925)
Engenharia:6711 (CompraKit:37457 Faturamento:37458 Entrega:37459 Instalação:37460 LigaçãoFinal:37461 Monitoramento:37462 Ass_Técnica:37463)
PréVenda:3426 (Prospect:18941 1TP_ligação:18943 2TP_Whatsapp:18944 3TP_Ligação2X:1895 4TP_WhatsappNutrição:18946 5TP_Whatsapp:18947 6TP_Ligação2X:18948 7TP_WhatsappBreakOff:18949 LeadQualificado:18942)
*/
//ARGS[0]:INI_Date ARGS[1]:Limit ARGS[2]FunilID ARGS[3]StageID Ex: node GetClientByID.js 2025-01-01 341 6462 35924
const ARGS = process.argv.slice(2);
if(ARGS[0] != null & ARGS[1] != null & ARGS[2] != null & ARGS[3] != null)
{
  INI_Date = ARGS[0];
  Limit = ARGS[1];
  FunilID = ARGS[2];
  StageID = ARGS[3];
  URL = URL.replace("SDate",INI_Date);
  URL = URL.replace("TodayDate",TODAY_Date);
  URL = URL.replace("Quantidade",Limit);
  URL = URL.replace("ID",FunilID);
  var PipeIDS = [];
  var index = 0;
  const APIKEY = "solarzcrmtNqIRT2f_paxMVEYCUfr93Pwou8lnFYE";
  fetch(URL, {
    method: 'GET',
    headers: {
      'authorization': APIKEY,
    }
  }).then(response => response.json())
    .then(data => {
      console.log(data);
    var content = JSON.stringify(data,null,2);
    try{FileStuff.writeFileSync('HTTPSolarZ '+TODAY_Date+'.txt',content,"utf8")}
    catch(err){console.err(err);}
  });//Store Data for Lua to Sort
}
else
{
  console.log("Informação Incompleta para Achar Clientes");
}


//Works
