#include "Lib/HEHelper&Maker.h"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <complex.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include "Lib/stb_truetype.h"


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
typedef union{
    int NUMBER;
    char DIGIT;
}SimpleConvert;
stbtt_fontinfo INFO;
typedef struct
{
    char *Word;
    SDL_Texture *Textures;//Maybe malloc the space for these...? There is a cost to malloc but each sentence would only pay it once
    int Length;
    int *Advances;
    float Size;
    float BasePosX;
    float BasePosY;
    int W;
    int H;
}WordDATA;
int TrueTypeBegin(char *FontPath, int FontLenght)
{
    if(FontLenght == 0)
    {
        return 1;
    }
    char TTFCHeck[3] = {0};
    int CheckCounter = 0;
    bool T = true;
    for(int i =0;i<=FontLenght;i++)
    {
        T = true;
        if(CheckCounter<2)
        {
            TTFCHeck[CheckCounter] = FontPath[i];
            CheckCounter++;
        }
        else
        {
            TTFCHeck[0] = TTFCHeck[1];
            TTFCHeck[1] = TTFCHeck[2];
            TTFCHeck[CheckCounter] = FontPath[i];
        }
        char Check = FontPath[i];
        if(i>=3)
        {

            char TTF[3] = "ttf";
            for(int x=0;x<=2;x++)
            {
                char Compare = TTFCHeck[x];
                if(TTF[x] != TTFCHeck[x])
                {
                    T = false;
                    break;
                }
            }
            if(T){break;}
        }
    }
    if(!T)
    {
        return 1;
    }
    FILE *F = fopen(FontPath,"rb");
    if(!F){return SDL_APP_FAILURE;}
    fseek(F, 0, SEEK_END);//Go to the end of the file
    long SZ = ftell(F);//Get the offset from beggining to current position (the end)
    rewind(F);//Reset the read/write pointer
    unsigned char *Data = malloc((size_t)SZ);//Allocate the offfset, which is the entire file
    fread(Data,1,(size_t)SZ,F);
    fclose(F);//Close since the data loading will be done by a function in the library
    int FontOffSet = stbtt_GetFontOffsetForIndex(Data, 0);
    stbtt_InitFont(&INFO, Data, FontOffSet);
    return 0;
}
void template(char *Word, int Length, float Size)
{
    float Scale = stbtt_ScaleForPixelHeight(&INFO, Size);
    int W,H,XO,YO;
    unsigned char *GrayScale = stbtt_GetCodepointBitmap(&INFO, Scale, Scale, Word[0], &W, &H, &XO, &YO);
    unsigned char *RGBBitmap = malloc((size_t)W*H*4);
    for(int i =0; i<W*H*4;i++)
    {
        RGBBitmap[i*4+0] = 0;
        RGBBitmap[i*4+1] = 0;
        RGBBitmap[i*4+2] = 0;
        RGBBitmap[i*4+3] = GrayScale[i];
    }
    SDL_Surface *SF = SDL_CreateSurfaceFrom(W, H, SDL_PIXELFORMAT_RGBA32, RGBBitmap, W*4);
    SDL_Texture *TX = SDL_CreateTextureFromSurface(Render, SF);
    SDL_DestroySurface(SF);
    free(RGBBitmap);
    free(GrayScale);
    //stbtt__GetGlyphGPOSInfoAdvance(const stbtt_fontinfo *info, int glyph1, int glyph2)
    //stbtt_GetGlyphKernAdvance(const stbtt_fontinfo *info, int glyph1, int glyph2)
}
void TTF_dynamicTexture(){}
SDL_AppResult TTF_staticTexture(WordDATA *DT)
{
    //First Lets try each letter being one texture before any merging bs
    for(int Letter=0;Letter<DT->Length;Letter++)
    {
        float Scale = stbtt_ScaleForPixelHeight(&INFO, DT->Size);
        int W,H,XO,YO;
        unsigned char *GrayScale = stbtt_GetCodepointBitmap(&INFO, Scale, Scale, DT->Word[Letter], &W, &H, &XO, &YO);
        unsigned char *RGB32 = malloc((size_t)W*H*4);
        for(int i =0; i<W*H*4; i++)
        {
            RGB32[i*4+0] = 0;
            RGB32[i*4+1] = 0;
            RGB32[i*4+2] = 0;
            RGB32[i*4+3] = GrayScale[i];
        }
        SDL_Surface *SF = SDL_CreateSurfaceFrom(W, H, SDL_PIXELFORMAT_RGBA32, RGB32, W*4);
        SDL_Texture *TX = SDL_CreateTextureFromSurface(Render, SF);
        DT->Textures = malloc((size_t)sizeof(SDL_Texture)*DT->Length);
        DT->Textures[Letter] = *TX;
        SDL_DestroySurface(SF);
        free(RGB32);
        free(GrayScale);
        DT->W = W;
        DT->H = H;
        DT->Advances = malloc((size_t)sizeof(int)*DT->Length);
        if(Letter == 0)
        {
            DT->Advances[Letter] = 0;
        }
        else
        {
            int Advance = 0;
            int KernSize = stbtt_GetKerningTableLength(&INFO);
            if(KernSize > 0)
            {
                Advance = stbtt_GetGlyphKernAdvance(&INFO, DT->Word[Letter-1], DT->Word[Letter]);
            }
            else
            {
                Advance = stbtt__GetGlyphGPOSInfoAdvance(&INFO, DT->Word[Letter-1], DT->Word[Letter]);
            }
            DT->Advances[Letter] = Advance;
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
    int Result = TrueTypeBegin("Resources/Roboto-Regular.ttf", 28);
    if(Result != 0)
    {
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(Render, WnWidth, WnHeight, SDL_LOGICAL_PRESENTATION_DISABLED);
    SDL_SetWindowMinimumSize(window, WnWidth, WnHeight);
    return SDL_APP_CONTINUE;
}
WordDATA TextWord = (WordDATA){
    .Word = "Debug",
    .Length = 5,
    .BasePosX = 200,
    .BasePosY = 200,
    .Size = 24.0f,
};
SDL_AppResult SDL_AppIterate(void *appstate)
{
    int Result = SDL_APP_CONTINUE;
    SDL_SetRenderDrawColor(Render, 60, 60, 60, 255);
    SDL_RenderClear(Render);
    if(TextWord.Textures == NULL)
    {
        TTF_staticTexture(&TextWord);
    }
    for(int i =0; 9 <TextWord.Length;i++)
    {
        SDL_RenderTexture(Render, &TextWord.Textures[i], NULL, &(SDL_FRect){
            .x=TextWord.BasePosX + TextWord.Advances[i],
            .y=TextWord.BasePosY,
            .w=TextWord.W,
            .h=TextWord.H
        });
    }
    SDL_RenderPresent(Render);
    return Result;
}
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if(event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{

}
/*
 */
