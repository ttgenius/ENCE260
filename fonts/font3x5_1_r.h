/** @file    font3x5_1_r.h
    @author  fontgen
    @date    24 Sep 2011

    @defgroup font3x5_1_r  This is a tiny 3x5 font with uppercase, digits, and symbols only.
 M.P. Hayes 2 Sep 2010

*/

#ifndef FONT3X5_1_R_H_
#define FONT3X5_1_R_H_

#define FONT3X5_1_R_WIDTH 5
#define FONT3X5_1_R_HEIGHT 3
#define FONT3X5_1_R_OFFSET 32
#define FONT3X5_1_R_SIZE 59

#ifndef FONT_WIDTH
#define FONT_WIDTH 5
#endif
#ifndef FONT_HEIGHT
#define FONT_HEIGHT 3
#endif
#ifndef FONT_OFFSET
#define FONT_OFFSET 32
#endif
#ifndef FONT_SIZE_MAX
#define FONT_SIZE_MAX 59
#endif
#include "font.h"

static font_t font3x5_1_r =
{
    .flags = 1, /* (packed)  */
    .width = 5,
    .height = 3,
    .offset = 32,
    .size = 59,
    .bytes = 2,
    .data =
    {
        /*    */
        0x00, 0x00, 
        /* !  */
        0xe0, 0x02, 
        /* "  */
        0xbf, 0x5a, 
        /* #  */
        0x5f, 0x7d, 
        /* $  */
        0xf6, 0x37, 
        /* %  */
        0x99, 0x4c, 
        /* &  */
        0xbe, 0x6a, 
        /* '  */
        0x40, 0x04, 
        /* (  */
        0x2e, 0x02, 
        /* )  */
        0x20, 0x3a, 
        /* *  */
        0xea, 0x2b, 
        /* +  */
        0xc4, 0x11, 
        /* ,  */
        0x10, 0x01, 
        /* -  */
        0x84, 0x10, 
        /* .  */
        0x00, 0x02, 
        /* /  */
        0x98, 0x0c, 
        /* 0  */
        0x2e, 0x3a, 
        /* 1  */
        0xf2, 0x43, 
        /* 2  */
        0xb9, 0x4a, 
        /* 3  */
        0xb1, 0x2a, 
        /* 4  */
        0x87, 0x7c, 
        /* 5  */
        0xb3, 0x26, 
        /* 6  */
        0xae, 0x26, 
        /* 7  */
        0xb9, 0x0c, 
        /* 8  */
        0xaa, 0x2a, 
        /* 9  */
        0xa2, 0x7c, 
        /* :  */
        0x40, 0x01, 
        /* ;  */
        0x00, 0x00, 
        /* <  */
        0x00, 0x00, 
        /* =  */
        0x00, 0x00, 
        /* >  */
        0x00, 0x00, 
        /* ?  */
        0x00, 0x00, 
        /* @  */
        0x00, 0x00, 
        /* A  */
        0x00, 0x00, 
        /* B  */
        0x00, 0x00, 
        /* C  */
        0x00, 0x00, 
        /* D  */
        0x00, 0x00, 
        /* E  */
        0x00, 0x00, 
        /* F  */
        0x00, 0x00, 
        /* G  */
        0x00, 0x00, 
        /* H  */
        0x00, 0x00, 
        /* I  */
        0x00, 0x00, 
        /* J  */
        0x00, 0x00, 
        /* K  */
        0x00, 0x00, 
        /* L  */
        0x00, 0x00, 
        /* M  */
        0x00, 0x00, 
        /* N  */
        0x00, 0x00, 
        /* O  */
        0x00, 0x00, 
        /* P  */
        0x00, 0x00, 
        /* Q  */
        0x00, 0x00, 
        /* R  */
        0x00, 0x00, 
        /* S  */
        0x00, 0x00, 
        /* T  */
        0x00, 0x00, 
        /* U  */
        0x00, 0x00, 
        /* V  */
        0x00, 0x00, 
        /* W  */
        0x00, 0x00, 
        /* X  */
        0x00, 0x00, 
        /* Y  */
        0x00, 0x00, 
        /* Z  */
        0x00, 0x00, 
    }
};
#endif  /* FONT3X5_1_R_H_  */
