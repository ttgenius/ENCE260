/** @file   mmelody.h
    @author M. P. Hayes, UCECE
    @date   20 April 2007
    @brief  Play simple melodies.
*/

/* By default notes are in the scale C4 -> C5.
   Note the first 12 frets on a six string guitar covers 3 octaves
   from the pitch E2 (82.41 Hz) to the pitch E5 (659.26 Hz).

   Tunes are specified using a sequence of characters representing
   notes, periods, and control operations.

   A note consists of the note name, followed by the octave number.
   For example, middle C is C4.

   A simple melody could be represented by C4C5F4G4C4 but for brevity
   this can be simplified to C4C5F4GC where the previously specified
   octave number persists.  

   A problem with this notation is the verbosity when we have
   something like B2C3B2C3.  Most melodies will only require 2 and 3
   at the most octaves.  In contrast, the ABC music notation uses
   CDEFGABcdefgabc'd'e'f'g'a'b'c' to denote 3 octaves from C2 to C5.
   This uses numbers to indicate note duration, for example, C2
   denotes a C of twice the standard duration.

   By default notes are crotchets (quarter notes) and tempos are
   defined in beats per minute.  If we assume 4/4 time, then each beat
   has a quarter-note duration.

   The note duration can be halved with a comma suffix, e.g., A,

   The note duration can be extended by a half with a dot suffix, e.g., A.

   The note duration can be doubled with a slash suffix, e.g., A/

   The note duration can be trebled with two slash suffixes, e.g., A//
   (this can be continued to lengthen the note).

   If there are lots of quavers (eighth notes), then the default note
   duration can be switched using *8.  *4 returns to quarter note timing.

   Rests are specified with a space character.  The timing modifiers ,
   . and / can be used.

   <ABC> is equivalent to ABCABC

   <ABC>3 is equivalent to ABCABCABC
   
   ABC: performs an infinite repeat of ABC

   <ABC]1DE]2FG> represents ABCDEABCFG where ]n denotes alternate
   endings.

   A+ sounds an octave higher

   A- sounds an octave lower

   @120 sets the tempo to 120 beats per minute

   This could be made a lot more flexible but the orginal
   implementation had a tight memory constraint.  Possible extensions:

   * For emphasis perhaps use ^ to indicate louder, for example, C^.
     Similarly, to make a note quieter it could have a v suffix.

   * Use | for bar markers; these can be ignored.
   
   * Time signatures.

   * Nested loops, e.g., <AB<ABC>>

   * Sixteenth notes?  Perhaps A,,

   * Perhaps lower case for eighth notes?

   * Triplets and other irrational tuplets?  A triplet quarter note
     has 2 / 3 the duration of a quarter note while a triplet eighth note
     has 2 / 3 the duration of of an eighth note.  This can be achieved
     using *6 or *12.
*/

#ifndef MMELODY_H
#define MMELODY_H

#include "system.h"

typedef uint8_t mmelody_speed_t;
typedef uint8_t mmelody_scale_t;
typedef uint8_t mmelody_note_t;
typedef uint8_t mmelody_volume_t;


enum {MMELODY_OCTAVE_DEFAULT = 4};
enum {MMELODY_SPEED_DEFAULT = 200};


typedef struct
{
    uint16_t ticks1;
    uint8_t ticks2;
    uint8_t unit_ticks;
    uint8_t release_ticks;
    /* Pointer to current position in string.  */    
    const char *cur;
    /* Pointer to start of string.  */
    const char *start;
    const char *loop_start;
    int8_t loop_count;
    /* Fraction of a whole note, e.g., 4 for quarter note.  */
    uint8_t symbol_fraction;
    /* Last note emitted.  */
    uint8_t note;
    /* Tempo in beats per minute.  */
    mmelody_speed_t speed;
    mmelody_volume_t volume;
    uint8_t octave;
    void (* play_callback) (void *data, uint8_t note, uint8_t volume);
    void *play_callback_data;
    uint16_t poll_rate;
} mmelody_private_t;

typedef mmelody_private_t mmelody_obj_t;
typedef mmelody_obj_t *mmelody_t;


typedef void (* mmelody_callback_t) (void *data, uint8_t note, uint8_t volume);

mmelody_t
mmelody_init (mmelody_obj_t *dev, 
              uint16_t poll_rate,
              mmelody_callback_t play_callback,
              void *play_callback_data);

/** Start playing a new melody.  */
void 
mmelody_play (mmelody_t mmelody, const char *str);


/** Update melody sequencer.  */
void
mmelody_update (mmelody_t mmelody);


/** Set (base) speed in beats per minute (BPM).  */
void 
mmelody_speed_set (mmelody_t mmelody, mmelody_speed_t speed);


/** Set volume as percentage of maximum.  */
void 
mmelody_volume_set (mmelody_t mmelody, mmelody_volume_t volume);


/** Return non-zero if playing.  */
bool
mmelody_active_p (mmelody_t mmelody);


#endif
