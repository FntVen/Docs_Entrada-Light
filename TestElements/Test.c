#include "Lib/HEHelper&Maker.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>

bool _NewLoad = true;;
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
    int MouseFocus;//Mouse focus is a 1 based index so subtract 1 to get an index
    int FunctionFocus; //The function of the last focused application | The Function parameter on Hitbox cant be used because it changes on mouse postion
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
    Text String;//Everything but the string should be set as the item is initialized. The state should hold even after the CleanFont
}TextBox;
TextBox TextBoxes[100];
int TextBoxCount=0;
int AnimationTick = 0;
bool _AnimationPlayed = false;
SDL_AppResult AnimationTickProgress()
{
    SDL_AppResult Result = SDL_APP_CONTINUE;
    if(MouseHandler.MouseFocus == 0)
    {
        return Result;
    }
    int Index = MouseHandler.MouseFocus-1;
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
                if(ActiveLabels[TextBoxes[Index].BorderBox.Parameter_2.NUMBER].X+ActiveLabels[TextBoxes[Index].BorderBox.Parameter_2.NUMBER].Width+5 > TextBoxes[Index].BorderBox.BoundingBox.x+TextBoxes[Index].BorderBox.BoundingBox.w)
                {
                    break;
                }
                SDL_RenderLine(
                Render,
                ActiveLabels[TextBoxes[Index].BorderBox.Parameter_2.NUMBER].X+ActiveLabels[TextBoxes[Index].BorderBox.Parameter_2.NUMBER].Width+5,
                ActiveLabels[TextBoxes[Index].BorderBox.Parameter_2.NUMBER].Y+ActiveLabels[TextBoxes[Index].BorderBox.Parameter_2.NUMBER].Height,
                ActiveLabels[TextBoxes[Index].BorderBox.Parameter_2.NUMBER].X+ActiveLabels[TextBoxes[Index].BorderBox.Parameter_2.NUMBER].Width+5,
                ActiveLabels[TextBoxes[Index].BorderBox.Parameter_2.NUMBER].Y
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
            TextBoxes[i].String.X = TextBoxes[i].BorderBox.BoundingBox.x + 5;
            TextBoxes[i].String.Y = TextBoxes[i].BorderBox.BoundingBox.y + 5;
            ActiveLabels[LabelCount] = TextBoxes[i].String;
            TextBoxes[i].BorderBox.Parameter_2.NUMBER = LabelCount;
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
    .String = {.Sentence="This is a Test",.FontSize=24,.RGB={.R=0,.G=0,.B=0,.A=255},._ForceRecalc=true},
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
        TextBoxTest.BorderBox.Parameter_1.NUMBER = TextBoxCount;
        TextBoxes[TextBoxCount] = TextBoxTest;
        TextBoxCount++;
        HitBoxes[HitboxCount] = (HitBox){
            .Function= 2,
            .Parameter_1=TextBoxTest.BorderBox.Parameter_1,
            .BoundingBox.x = TextBoxTest.BorderBox.BoundingBox.x,
            .BoundingBox.y = TextBoxTest.BorderBox.BoundingBox.y,
            .BoundingBox.w = TextBoxTest.BorderBox.BoundingBox.w,
            .BoundingBox.h = TextBoxTest.BorderBox.BoundingBox.h
        };
        HitboxCount++;
    }
    else
    {

    }
    Result = RenderTextBox();
    Result = SDL_RenderFont();
    if(MouseHandler.MouseFocus != PADDING)
    {
        AnimationTick++;
    }
    //printf("%d\n",AnimationTick);
    Result = AnimationTickProgress();
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
                MouseHandler.Box.Parameter_1.NUMBER = 0;
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
                        MouseHandler.Box.Parameter_1 = HitBoxes[i].Parameter_1;
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
            MouseHandler.MouseFocus = MouseHandler.Box.Parameter_1.NUMBER+1;
            SDL_AppResult Result = SDL_APP_CONTINUE;
            switch(MouseHandler.Box.Function)
            {
                case TABSELECT:
                    //Result = TabSwitch(HoverHandler.Box.Parameter);
                break;
                case RADIO_MENUENABLE:
                    //Result = RADIO_TabEnable(HoverHandler.Box.Parameter);
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
            MouseHandler.MouseFocus = 0;
        }
    }
    if(event->type == SDL_EVENT_KEY_DOWN)
    {
        switch(MouseHandler.FunctionFocus)//event->key.key
        {

        }
    }
    return SDL_APP_CONTINUE;
}
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    CleanFonts();
}
/*
 * °    Use a variable in the mouse handler to keep track of focus
 * °    The variable should come from Parameter_1
 * °    Only update focus on Button click on a Hitbox
 * °    What happens to focused Hitbox will depend on its type as determined by its "Function" Enum
 * °    The Index of the variable on mouse handler should relate to the list the current item applies, like the label array or the tab array or the textBox Array
 * °    Clear reference of Function enum and the tracker of focus from mouse handler on screen switch
 */
