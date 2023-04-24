// #include <stdint.h>
// #include "ttyd/fontmgr.h"
// #include "gc/mtx.h"
// #include <cmath>
// #include <string.h>

// float __attribute__ ((noinline)) intToFloat(int32_t value)
// {
//     return static_cast<float>(value);
// }

// // Set width to a negative value to not have a width limit
// void drawTextMain(const char *text, int32_t x, int32_t y, 
//     uint32_t color, bool alignRight, float scale, float width)
// {
//     // Make sure the text isn't an empty string
//     if (text[0] == '\0')
//     {
//         return;
//     }
    
//     float NewPosX = intToFloat(x);
//     float TextLengthScaled = 0.f; // Failsafe; 0.f value won't be used
    
//     // Check if aligning the text to the right
//     if (alignRight)
//     {
//         uint32_t TextLength = ttyd::fontmgr::FontGetMessageWidth(text);
//         TextLengthScaled = intToFloat(static_cast<int32_t>(TextLength)) * scale;
        
//         NewPosX -= TextLengthScaled;
//     }
    
//     // Check if there's a width limit
//     float ScaleX = scale;
//     if (!std::signbit(width)) // Check if positive, works for checking against +0.0 and -0.0
//     {
//         // If not aligning the text to the right, then TextLengthScaled needs to be calculated
//         if (!alignRight)
//         {
//             uint32_t TextLength = ttyd::fontmgr::FontGetMessageWidth(text);
//             TextLengthScaled = intToFloat(static_cast<int32_t>(TextLength)) * scale;
//         }
        
//         if (TextLengthScaled > width)
//         {
//             ScaleX = (width / TextLengthScaled) * scale;
            
//             // If aligning the text to the right, account for the new X scale
//             if (alignRight)
//             {
//                 NewPosX += TextLengthScaled - width;
//             }
//         }
//     }
    
//     gc::mtx::mtx34 MtxScale;
//     gc::mtx::PSMTXScale(MtxScale, ScaleX, scale, scale);
    
//     gc::mtx::mtx34 MtxTrans;
//     gc::mtx::PSMTXTransApply(
//         MtxScale, 
//         MtxTrans, 
//         NewPosX, 
//         intToFloat(y), 
//         0.f);
    
//     ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
//     ttyd::fontmgr::FontDrawMessageMtx((float(*)[4])MtxTrans, text);
// }

// void drawTextInit(uint8_t alpha, bool drawFontEdge)
// {
//     ttyd::fontmgr::FontDrawStart_alpha(alpha);
    
//     // Check if the font edge should be on or off
//     if (drawFontEdge)
//     {
//         ttyd::fontmgr::FontDrawEdge();
//     }
// }

// void drawTextMultipleLines(const char *text, int32_t x, int32_t y, 
//     uint32_t color, bool alignRight, float scale, float width)
// {
//     char LineBuffer[128];
//     const char *CurrentLine = text;
//     constexpr int32_t MaxLength = sizeof(LineBuffer) - 1;
    
//     // Draw each individual line
//     while (1)
//     {
//         // Find the end of the current line
//         const char *Newline = strchr(CurrentLine, '\n');
        
//         // If a newline is not found, then currently at the last line
//         if (!Newline)
//         {
//             break;
//         }
        
//         // Copy this line to the temporary buffer and append a NULL byte
//         int32_t LineLength = Newline - CurrentLine;
        
//         // Make sure the current line won't be an empty string
//         if (LineLength > 0)
//         {
//             // Prevent a buffer overflow
//             if (LineLength > MaxLength)
//             {
//                 LineLength = MaxLength;
//             }
            
//             char *tempBuffer = strncpy(LineBuffer, CurrentLine, LineLength);
//             tempBuffer[LineLength] = '\0';
            
//             drawTextMain(tempBuffer, x, y, color, alignRight, scale, width);
//         }
        
//         // Advance to the next line
//         CurrentLine = Newline + 1;
//         y -= 20;
//     }
    
//     // Draw the rest of the text
//     drawTextMain(CurrentLine, x, y, color, alignRight, scale, width);
// }

// void drawText(const char *text, int32_t x, int32_t y, uint32_t color, float scale)
// {
//     drawTextMultipleLines(text, x, y, color, false, scale, -0.f);
// }