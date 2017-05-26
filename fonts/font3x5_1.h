/** @file    font3x5_1.h
    @author  fontgen
    @date    24 Sep 2011

    @defgroup font3x5_1  This is a tiny 3x5 font with uppercase, digits, and symbols only.
 M.P. Hayes 2 Sep 2010

*/

#ifndef FONT3X5_1_H_
#define FONT3X5_1_H_

#define FONT3X5_1_WIDTH 3
#define FONT3X5_1_HEIGHT 5
#define FONT3X5_1_OFFSET 32
#define FONT3X5_1_SIZE 59

#ifndef FONT_WIDTH
#define FONT_WIDTH 3
#endif
#ifndef FONT_HEIGHT
#define FONT_HEIGHT 5
#endif
#ifndef FONT_OFFSET
#define FONT_OFFSET 32
#endif
#ifndef FONT_SIZE_MAX
#define FONT_SIZE_MAX 59
#endif
#include "font.h"

static font_t font3x5_1 =
{
    .flags = 1, /* (packed)  */
    .width = 3,
    .height = 5,
    .offset = 32,
    .size = 59,
    .bytes = 2,
    .data =
    {
        /*    */
        0x00, 0x00, 
        /* !  */
        0x92, 0x20, 
        /* "  */
        0xeb, 0x73, 
        /* #  */
        0x7d, 0x5f, 
        /* $  */
        0xde, 0x3d, 
        /* %  */
        0xa5, 0x52, 
        /* &  */
        0xea, 0x7a, 
        /* '  */
        0x14, 0x00, 
        /* (  */
        0x4a, 0x22, 
        /* )  */
        0x22, 0x29, 
        /* *  */
        0xba, 0x2e, 
        /* +  */
        0xd0, 0x05, 
        /* ,  */
        0x00, 0x14, 
        /* -  */
        0xc0, 0x01, 
        /* .  */
        0x00, 0x20, 
        /* /  */
        0xa4, 0x12, 
        /* 0  */
        0x6a, 0x2b, 
        /* 1  */
        0x9a, 0x74, 
        /* 2  */
        0xa3, 0x72, 
        /* 3  */
        0xa3, 0x38, 
        /* 4  */
        0xed, 0x49, 
        /* 5  */
        0x8f, 0x38, 
        /* 6  */
        0xce, 0x2a, 
        /* 7  */
        0xa7, 0x12, 
        /* 8  */
        0xaa, 0x2a, 
        /* 9  */
        0xae, 0x49, 
        /* :  */
        0x10, 0x04, 
        /* ;  */
        0x10, 0x14, 
        /* <  */
        0x54, 0x44, 
        /* =  */
        0x38, 0x0e, 
        /* >  */
        0x11, 0x15, 
        /* ?  */
        0x2a, 0x25, 
        /* @  */
        0x00, 0x00, 
        /* A  */
        0xea, 0x5b, 
        /* B  */
        0xeb, 0x3a, 
        /* C  */
        0x4f, 0x72, 
        /* D  */
        0x6b, 0x3b, 
        /* E  */
        0xcf, 0x73, 
        /* F  */
        0xcf, 0x13, 
        /* G  */
        0x4f, 0x7a, 
        /* H  */
        0xed, 0x5b, 
        /* I  */
        0x97, 0x74, 
        /* J  */
        0x24, 0x7b, 
        /* K  */
        0x5d, 0x56, 
        /* L  */
        0x49, 0x72, 
        /* M  */
        0xfd, 0x5b, 
        /* N  */
        0x6f, 0x5b, 
        /* O  */
        0x6f, 0x7b, 
        /* P  */
        0xef, 0x13, 
        /* Q  */
        0x6f, 0x7f, 
        /* R  */
        0xef, 0x57, 
        /* S  */
        0xce, 0x38, 
        /* T  */
        0x97, 0x24, 
        /* U  */
        0x6d, 0x7b, 
        /* V  */
        0x6d, 0x2b, 
        /* W  */
        0xed, 0x5f, 
        /* X  */
        0xad, 0x5a, 
        /* Y  */
        0xad, 0x24, 
        /* Z  */
        0xa7, 0x72, 
    }
};
#endif  /* FONT3X5_1_H_  */
