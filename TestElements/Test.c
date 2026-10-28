#include "Lib/HEHelper&Maker.h"
#include <time.h>

bool _NewLoad = true;
int WnWidth = 1200;
int WnHeight = 600;
HitBox HitBoxes[100] = {0};
enum BtnFunctions{
    TABSELECT,
    RADIO_MENUENABLE,
    TEXTBOX,
    PADDING
};
typedef struct{
    MousePos XY;//Mouse Position
    HitBox Box;//The bounding box & Function Identifier & Function Parameters of the hovered HitBox
    EventParams MouseFocus;//Mouse focus is a 1 based index based on Parameter_1 of Hitbox so subtract 1 to get te correct index
    int FunctionFocus; //The function of the last focused application | The Function parameter on Hitbox is used here but cant be cant be used because it changes on mouse postion
    bool HoverTrigger;//True if the mouse is hovering a hitbox
    SDL_Cursor *Pointer_Normal;//Normal Mouse State
    SDL_Cursor *Pointer_Text;//Mouse hovering a TextBox and the like
    SDL_Cursor *pointer_Select;//Mouse Hovering a selectable item
}MTraits;
MTraits MouseHandler;
MousePos MouseState;
typedef struct{
    HitBox BorderBox;//Should work as the text backdrop and be passed as the hitbox
    Color BackGround_Color;
    bool _Border;//If a border should be rendered
    Color Border_Color;
    int Border_Thickness;
    int Text_Index;//The version of the Text saved on the array will need to be overwritten as the text is updated
    Text *String;//Everything but the string should be set as the item is initialized. The state should hold even after the CleanFont
}TextBox;
TextBox TextBoxes[100];
int TextBoxCount = 0;
typedef union{
    int NUMBER;
    char DIGIT;
}SimpleConvert;
int AnimationTick = 0;
bool _AnimationPlayed = false;
bool TXT_UpdateChar(int ScanCode, int Mod, TextBox_PARAM Param)/*Position = 0 is the end position of the string with everything before being negative */
{
    char Hold_String[4];
    char Cedilha[3] = "ç";
    char Hold_Char;
    bool _STR = false;
    bool _Return = false;
    switch(ScanCode)
    {
        case 4:
            Hold_Char = 'a';
            _Return = true;
        break;
        case 5:
           Hold_Char = 'b';
           _Return = true;
        break;
        case 6:
           Hold_Char = 'c';
           _Return = true;
        break;
        case 7:
           Hold_Char = 'd';
           _Return = true;
        break;
        case 8:
           Hold_Char = 'e';
           _Return = true;
        break;
        case 9:
           Hold_Char = 'f';
           _Return = true;
        break;
        case 10:
           Hold_Char = 'g';
           _Return = true;
        break;
        case 11:
           Hold_Char = 'h';
           _Return = true;
        break;
        case 12:
           Hold_Char = 'i';
           _Return = true;
        break;
        case 13:
           Hold_Char = 'j';
           _Return = true;
        break;
        case 14:
           Hold_Char = 'k';
           _Return = true;
        break;
        case 15:
           Hold_Char = 'l';
           _Return = true;
        break;
        case 16:
           Hold_Char = 'm';
           _Return = true;
        break;
        case 17:
           Hold_Char = 'n';
           _Return = true;
        break;
        case 18:
           Hold_Char = 'o';
           _Return = true;
        break;
        case 19:
           Hold_Char = 'p';
           _Return = true;
        break;
        case 20:
           Hold_Char = 'q';
           _Return = true;
        break;
        case 21:
           Hold_Char = 'r';
           _Return = true;
        break;
        case 22:
           Hold_Char = 's';
           _Return = true;
        break;
        case 23:
           Hold_Char = 't';
           _Return = true;
        break;
        case 24:
           Hold_Char = 'u';
           _Return = true;
        break;
        case 25:
           Hold_Char = 'v';
           _Return = true;
        break;
        case 26:
           Hold_Char = 'w';
           _Return = true;
        break;
        case 27:
           Hold_Char = 'x';
           _Return = true;
        break;
        case 28:
           Hold_Char = 'y';
           _Return = true;
        break;
        case 29:
           Hold_Char = 'z';
           _Return = true;
        break;
        case 44:
            Hold_Char = ' ';
            _Return = true;
        break;
        case 51:
            for(int i=0;i<4;i++)
            {
                Hold_String[i] = Cedilha[i];
            }
            _STR = true;
        break;
        default:
            _Return = false;
        return _Return;
    }
    //Modify string
    char TempArray[TextBoxes[Param.TBOx_Index].String->StringSize+3];//+3 to account for ç and ã
    int TempIndex=0;
    for(int i=0;i<TextBoxes[Param.TBOx_Index].String->StringSize;i++)
    {
        if(Param.WrittingPos!=0)
        {
            if(i==Param.WrittingPos+TextBoxes[Param.TBOx_Index].String->StringSize)
            {
                if(_STR)
                {
                    for(int x=0;x<sizeof(Hold_String)/sizeof(char);x++)
                    {
                        TempIndex++;
                        TempArray[TempIndex] = Hold_String[x];
                    }
                    i--;
                }
                else
                {
                    TempArray[TempIndex] = Hold_Char;
                    i--;
                }
            }
            else
            {
                TempArray[TempIndex] = ActiveLabels[Param.Label_Index].Sentence[i];
            }
        }
        else
        {
            TempArray[TempIndex] = ActiveLabels[Param.Label_Index].Sentence[i];
            if(i==TextBoxes[Param.TBOx_Index].String->StringSize-1)//Last character of the unmodified string
            {
                if(_STR)
                {
                    for(int x=0;x<=sizeof(Hold_String)/sizeof(char);x++)
                    {

                        TempArray[TempIndex+x+1] = Hold_String[x];
                    }
                }
                else
                {
                    TempArray[TempIndex+1] = Hold_Char;
                    TempArray[TempIndex+2] = 0;
                }
                break;
            }
        }
        TempIndex++;
    }
    printf("%s\n",TempArray);

    return _Return;
}
bool TXT_WritePosition()
{

}
SDL_AppResult AnimationTickProgress()
{
    SDL_AppResult Result = SDL_APP_CONTINUE;
    /*
    if(MouseHandler.MouseFocus.TXT_PARAM.Label_Index == PADDING)
    {
        return Result;
    }
    int Index = MouseHandler.MouseFocus.TXT_PARAM.Label_Index-1;
    switch (MouseHandler.FunctionFocus)
    {
        case TABSELECT:
        break;
        case RADIO_MENUENABLE:
        break;
        case TEXTBOX:
            if(AnimationTick >= 7000)
            {
                SDL_SetRenderDrawColor(Render, 0, 0, 0, 255);
                if(ActiveLabels[TextBoxes[Index].BorderBox.Parameter.TXT_PARAM.Label_Index].X+ActiveLabels[TextBoxes[Index].BorderBox.Parameter.TXT_PARAM.Index].Width+5 > TextBoxes[Index].BorderBox.BoundingBox.x+TextBoxes[Index].BorderBox.BoundingBox.w)
                {
                    break;
                }
                SDL_RenderLine(
                Render,
                ActiveLabels[TextBoxes[Index].BorderBox.Parameter.TXT_PARAM.Index].X+ActiveLabels[TextBoxes[Index].BorderBox.Parameter.TXT_PARAM.Index].Width+5,
                ActiveLabels[TextBoxes[Index].BorderBox.Parameter.TXT_PARAM.Index].Y+ActiveLabels[TextBoxes[Index].BorderBox.Parameter.TXT_PARAM.Index].Height,
                ActiveLabels[TextBoxes[Index].BorderBox.Parameter.TXT_PARAM.Index].X+ActiveLabels[TextBoxes[Index].BorderBox.Parameter.TXT_PARAM.Index].Width+5,
                ActiveLabels[TextBoxes[Index].BorderBox.Parameter.TXT_PARAM.Index].Y
                );
                if(AnimationTick >= 14000)
                {
                    _AnimationPlayed = true;
                }
            }
        break;
    }
    if(_AnimationPlayed)
    {
        AnimationTick = 0;
        _AnimationPlayed = false;
    }
    */
    return Result;
}
SDL_AppResult RenderTextBox()
{
    if(TextBoxCount == 0)
    {
        return SDL_APP_CONTINUE;
    }
    for(int i = 0;i<TextBoxCount;i++)
    {
        //RenderBox & Border
        if(TextBoxes[i]._Border)
        {
            SDL_SetRenderDrawColor(Render, TextBoxes[i].Border_Color.R, TextBoxes[i].Border_Color.G, TextBoxes[i].Border_Color.B, TextBoxes[i].Border_Color.A);
            float X = TextBoxes[i].BorderBox.BoundingBox.x - TextBoxes[i].Border_Thickness;
            float Y = TextBoxes[i].BorderBox.BoundingBox.y - TextBoxes[i].Border_Thickness;
            float Width = TextBoxes[i].BorderBox.BoundingBox.w + (TextBoxes[i].Border_Thickness*2);
            float Height = TextBoxes[i].BorderBox.BoundingBox.h + (TextBoxes[i].Border_Thickness*2);

            SDL_RenderFillRect(Render, &(SDL_FRect){
                .x = X,
                .y = Y,
                .w = Width,
                .h = Height
            });
        }
        SDL_SetRenderDrawColor(Render, TextBoxes[i].BackGround_Color.R, TextBoxes[i].BackGround_Color.G, TextBoxes[i].BackGround_Color.B, TextBoxes[i].BackGround_Color.A);
        SDL_RenderFillRect(Render, &TextBoxes[i].BorderBox.BoundingBox);
        if(!HubLoaded)
        {
            TextBoxes[i].String->X = TextBoxes[i].BorderBox.BoundingBox.x + 5;
            TextBoxes[i].String->Y = TextBoxes[i].BorderBox.BoundingBox.y + 5;
            ActiveLabels[LabelCount] = *TextBoxes[i].String;
            TextBoxes[i].BorderBox.Parameter.TXT_PARAM.Label_Index = LabelCount;
            LabelCount++;
        }
    }

    return SDL_APP_CONTINUE;
}
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Luciano Test 2", WnWidth, WnHeight, SDL_WINDOW_RESIZABLE, &window, &Render))
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(Render, WnWidth, WnHeight, SDL_LOGICAL_PRESENTATION_DISABLED);
    SDL_SetWindowMinimumSize(window, WnWidth, WnHeight);
    SDL_AppResult FontResult = SetFont();
    if(FontResult != SDL_APP_CONTINUE)
    {
        return FontResult;
    }
    MouseHandler.Pointer_Normal = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
    MouseHandler.Pointer_Text = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_TEXT);
    MouseHandler.pointer_Select = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER);
    if(!MouseHandler.Pointer_Normal || !MouseHandler.Pointer_Text || !MouseHandler.pointer_Select)
    {
        printf("Error loading cursor\n");
    }
    else
    {
        SDL_SetCursor(MouseHandler.Pointer_Normal);
        MouseHandler.FunctionFocus = PADDING;
    }
    return SDL_APP_CONTINUE;
}
TextBox TextBoxTest = {
    .BorderBox={.BoundingBox={.x=100,.y=100,.w=300,.h=40},.Function=TEXTBOX},
    .String = &(Text){.Sentence="This is a Test",.FontSize=24,.RGB={.R=0,.G=0,.B=0,.A=255},._ForceRecalc=true,.StringSize = 14},
    ._Border = true,
    .Border_Thickness = 1,
    .BackGround_Color = {.R=255,.G=255,.B=255,.A=255},
    .Border_Color = {.R=0,.G=0,.B=0,.A=255},
};
int TxtBoxPositions[10];
SDL_AppResult SDL_AppIterate(void *appstate)
{
    if(_NewLoad)
    {
        HubLoaded = false;
    }
    else
    {
        HubLoaded = true;
    }
    int Result = SDL_APP_CONTINUE;
    SDL_SetRenderDrawColor(Render, 60, 60, 60, 255);
    SDL_RenderClear(Render);
    if(!HubLoaded)
    {
        TextBoxTest.BorderBox.Parameter.TXT_PARAM.TBOx_Index = TextBoxCount;
        TextBoxes[TextBoxCount] = TextBoxTest;
        TextBoxCount++;
        HitBoxes[HitboxCount] = (HitBox){
            .Function= TEXTBOX,
            .Parameter.TXT_PARAM=TextBoxTest.BorderBox.Parameter.TXT_PARAM,
            .BoundingBox.x = TextBoxTest.BorderBox.BoundingBox.x,
            .BoundingBox.y = TextBoxTest.BorderBox.BoundingBox.y,
            .BoundingBox.w = TextBoxTest.BorderBox.BoundingBox.w,
            .BoundingBox.h = TextBoxTest.BorderBox.BoundingBox.h
        };
        HitboxCount++;
    }
    else
    {
        //printf("Test %d | Box %d\n",TextBoxTest.StringSize,TextBoxes[0].StringSize);
    }
    Result = RenderTextBox();
    Result = SDL_RenderFont();
    SDL_RenderPresent(Render);
    _NewLoad = false;
    return Result;
}
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if(event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }
    if(event->type == SDL_EVENT_WINDOW_RESIZED)
    {
        SDL_GetWindowSizeInPixels(window, &WnWidth, &WnHeight);
    }
    if(event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }
    if(event->type == SDL_EVENT_WINDOW_RESIZED)
    {
        SDL_GetWindowSizeInPixels(window, &WnWidth, &WnHeight);
    }
    if(event->type == SDL_EVENT_MOUSE_MOTION)
    {

        MouseState.X = event->motion.x;
        MouseState.Y = event->motion.y;
        if(MouseHandler.HoverTrigger)
        {
            if(MouseState.X<=MouseHandler.Box.BoundingBox.x || MouseState.X>=MouseHandler.Box.BoundingBox.x+MouseHandler.Box.BoundingBox.w || MouseState.Y<=MouseHandler.Box.BoundingBox.y || MouseState.Y>=MouseHandler.Box.BoundingBox.y+MouseHandler.Box.BoundingBox.h)
            {
                MouseHandler.HoverTrigger = false;
                if(MouseHandler.Box.Function == TABSELECT)
                {
                    //TabHighLight(HoverHandler.Box.Parameter);
                }
                if(MouseHandler.Box.Function == TEXTBOX)
                {
                    SDL_SetCursor(MouseHandler.Pointer_Normal);
                }
                MouseHandler.Box.BoundingBox = (SDL_FRect){.x=0,.y=0,.w=0,.h=0};
                MouseHandler.Box.Function = 0;
                MouseHandler.Box.Parameter.NUMBER = 0;
            }
        }
        if(!MouseHandler.HoverTrigger)
        {
            for(int i=0;i<HitboxCount;i++)
            {
                if(MouseState.X>HitBoxes[i].BoundingBox.x && MouseState.X<HitBoxes[i].BoundingBox.x+HitBoxes[i].BoundingBox.w)
                {
                    if(MouseState.Y>HitBoxes[i].BoundingBox.y && MouseState.Y<HitBoxes[i].BoundingBox.y+HitBoxes[i].BoundingBox.h)
                    {
                        MouseHandler.HoverTrigger = true;
                        MouseHandler.Box.BoundingBox = HitBoxes[i].BoundingBox;
                        MouseHandler.Box.Function = HitBoxes[i].Function;
                        MouseHandler.Box.Parameter = HitBoxes[i].Parameter;
                        if(HitBoxes[i].Function == TABSELECT)
                        {
                            //TabHighLight(HitBoxes[i].Parameter);
                        }
                        if(MouseHandler.Box.Function == TEXTBOX)
                        {
                            SDL_SetCursor(MouseHandler.Pointer_Text);
                        }
                    }
                }
            }
        }
    }
    if(event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        if(MouseHandler.HoverTrigger)
        {
            MouseHandler.FunctionFocus = MouseHandler.Box.Function;
            SDL_AppResult Result = SDL_APP_CONTINUE;
            switch(MouseHandler.Box.Function)
            {
                case TABSELECT:
                    MouseHandler.MouseFocus = MouseHandler.Box.Parameter;
                    //Result = TabSwitch(HoverHandler.Box.Parameter);
                break;
                case RADIO_MENUENABLE:
                    MouseHandler.MouseFocus = MouseHandler.Box.Parameter;
                    //Result = RADIO_TabEnable(HoverHandler.Box.Parameter);
                break;
                case TEXTBOX:
                    MouseHandler.MouseFocus.TXT_PARAM.Label_Index = MouseHandler.Box.Parameter.TXT_PARAM.Label_Index;
                    MouseHandler.MouseFocus.TXT_PARAM.TBOx_Index = MouseHandler.Box.Parameter.TXT_PARAM.TBOx_Index;
                    MouseHandler.MouseFocus.TXT_PARAM.WrittingPos = 0; //Calc the writting position on click
                  SDL_Rect Rect = {
                      .x = MouseHandler.Box.BoundingBox.x,
                      .y = MouseHandler.Box.BoundingBox.y,
                      .w = MouseHandler.Box.BoundingBox.w,
                      .h = MouseHandler.Box.BoundingBox.h
                    };
                  SDL_SetTextInputArea(window, &Rect, 0);
                  SDL_StartTextInput(window);
                break;
            }
            if(Result != SDL_APP_CONTINUE)
            {
                return Result;
            }
        }
        else
        {
            MouseHandler.FunctionFocus = PADDING;
            MouseHandler.MouseFocus.TXT_PARAM.TBOx_Index = 0;
            MouseHandler.MouseFocus.TXT_PARAM.Label_Index = 0;
            MouseHandler.MouseFocus.TXT_PARAM.WrittingPos = 0;
            SDL_StopTextInput(window);
            SDL_SetTextInputArea(window, NULL, 0);
        }
    }
    if(event->type == SDL_EVENT_KEY_DOWN)
    {
        SDL_AppResult Result = SDL_APP_CONTINUE;
        switch(MouseHandler.FunctionFocus)//event->key.key
        {
        case TEXTBOX:
            if(event->key.key == SDLK_ESCAPE || event->key.key == SDLK_RETURN)
            {
                MouseHandler.FunctionFocus = PADDING;
                MouseHandler.MouseFocus.TXT_PARAM.Label_Index = 0;
                MouseHandler.MouseFocus.TXT_PARAM.TBOx_Index = 0;
                MouseHandler.MouseFocus.TXT_PARAM.WrittingPos = 0;
                SDL_StopTextInput(window);
                SDL_SetTextInputArea(window, NULL, 0);
            }
            else//Scancodes are consistent betweeen computers and OS
            {
                int M = 0;
                char K;
                char S[4];
                if(event->key.mod != 0 && event->key.key == SDLK_LSHIFT ||
                   event->key.key == SDLK_LCTRL || event->key.key == SDLK_CAPSLOCK ||
                   event->key.key == SDLK_LALT || event->key.key == SDLK_RALT || event->key.key == SDLK_RCTRL)
                {
                    M = event->key.mod;
                }
                if(event->key.scancode >= 4 && event->key.scancode <= 56 ||event->key.scancode == 44)
                {
                    bool Answer = TXT_UpdateChar(event->key.scancode, M,MouseHandler.MouseFocus.TXT_PARAM);//Implement special type to handle text parameters and add to union
                }
                //Result = UpdateText(event->key.key,MouseHandler.MouseFocus);
            }
        break;
        }
    }
    if(event->type == SDL_EVENT_KEY_UP)
    {
        //Handle letting go of special keys like ´ or ~
    }

    return SDL_APP_CONTINUE;
}
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    CleanFonts();
}
/*
 */
