local SEP = package.config:sub(1, 1);
local FiltroData = "2024-01-01" --#1 Argument Ex:2024-01-01
local FiltroQuantidade = "999" --#2 Argument Ex:999
local FunilId = "3426"--#3
local StageId = "14313"          --#4
local Request;
if SEP == '\\' then
    Request = string.format(".\\GetClient.exe %s %s %s %s", FiltroData, FiltroQuantidade, FunilId, StageId);
else
    Request = string.format("./GetClient %s %s %s %s", FiltroData, FiltroQuantidade, FunilId, StageId);
end
local JSHandle = io.popen(Request, "r");
if JSHandle then
    local Rs = JSHandle:read("*a");
	JSHandle:close();
    print(Rs);
else
    print("Error")
end
