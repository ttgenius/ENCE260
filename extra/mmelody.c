/** @file   mmelody.c
    @author M. P. Hayes, UCECE
    @date   20 April 2007
    @brief  Play simple melodies.
*/
#include "system.h"
#include "mmelody.h"


enum {MMELODY_SCALE_SIZE = 12};

/* Assume each beat is a quarter note.  */
enum {MMELODY_BEAT_FRACTION = 4};

/* Scale factor for fraction of a symbol timing.  */
enum {MMELODY_DURATION_FRACTION = 4};

/* How long to release note before sounding the next one.  */
#define MMELODY_RELEASE_MS 50


static void
mmelody_ticker_set (mmelody_t mmelody)
{
  /*   The duration of a beat varies with the time signature:
       2/2 : minum (half note)
       4/4 : crotchet (quarter note)
       6/8, 9/8, 12/8 : dotted crotchet (one and a half quarter notes)

       Currently, there is no time signature support so we assume 4/4.
       This means a beat is a quarter note and a bar is four quarter
       notes.  

       ticks_per_beat = poll_rate * 60 / speed
       symbols_per_beat = symbol_fraction / MMELODY_BEAT_FRACTION
       ticks_per_symbol = ticks_per_beat / symbols_per_beat
       ticks_per_unit = ticks_per_symbol / MMELODY_DURATION_FRACTION

       ticks_per_note = ticks_per_beat * MMELODY_BEAT_FRACTION
                      = ticks_per_unit * symbol_fraction 
                                       * MMELODY_DURATION_FRACTION

       The duration is specified as the actual duration scaled by
       MMELODY_DURATION_FRACTION so that it can specify half periods.

       With symbol_fraction = 4, speed = 50, and poll_rate of 200 this
       gives a result of 60.  */

    mmelody->unit_ticks
        = (mmelody->poll_rate * 60 * MMELODY_BEAT_FRACTION) 
        / (mmelody->speed * mmelody->symbol_fraction
           * MMELODY_DURATION_FRACTION);
}


static void
mmelody_note_on (mmelody_t mmelody, mmelody_note_t note, uint8_t duration)
{
    /* The duration is in terms of the half the duration of a symbol.  */

    mmelody->play_callback (mmelody->play_callback_data, note,
                            mmelody->volume);
    mmelody->note = note;

    /* Determine ticks between sounding notes (this needs to be a
       minimum of 1).  If we assume a release period of 50 ms,
       this will give a count of 10 ticks at 200 Hz sampling.  */
    mmelody->ticks2 = mmelody->release_ticks;

    /* Determine ticks before turning the note off.  */
    mmelody->ticks1 = mmelody->unit_ticks * duration - mmelody->ticks2;
}


static void
mmelody_note_off (mmelody_t mmelody)
{
    mmelody->play_callback (mmelody->play_callback_data, 
                            mmelody->note, 0);
    mmelody->note = 0;
}


/* Specify the default note length in fractions of a measure (bar).
   A value of 4 is the default which makes each note a quarter note.  */
static void 
mmelody_symbol_fraction_set (mmelody_t mmelody, uint8_t symbol_fraction)
{
    mmelody->symbol_fraction = symbol_fraction;    
    mmelody_ticker_set (mmelody);
}


static mmelody_note_t
mmelody_char_to_note (uint8_t ch)
{
    /* A = 9, B = 11, C = 0, D = 2, E = 4, F = 5, G = 7  */
    static const mmelody_note_t const lookup[] = {9, 11, 0, 2, 4, 5, 7};

    return lookup[ch - 'A'];
}


/* Scan next part of melody until a note or end of melody is found.  */
static const char *
mmelody_scan (mmelody_t mmelody, const char *str)
{
    while (1)
    {
        uint8_t num;
        char cmd;
        char modifier;
        bool have_hash;
        bool have_num;
        mmelody_note_t note = 0;
        uint8_t duration = 1;
        
        /* Play rest at end of melody.  */
        if (! *str)
        {
            mmelody_note_off (mmelody);
            return str;
        }

        cmd = *str++;

        have_hash = *str == '#';
        if (have_hash)
            str++;

        modifier = 0;
        if (*str == '+' || *str == '-')
            modifier = *str++;

        have_num = 0;
        num = 0;
        while (*str >= '0' && *str <= '9')
        {
            have_num = 1;
            num = num * 10 + *str++ - '0';
        }

        switch (cmd)
        {
            /* Repeat sequence from start.  */
        case ':':
            str = mmelody->start;
            continue;

            /* Define start of loop.  */
        case '<':
            /* We could implement a small stack to allow nested
               loops.  */
            mmelody->loop_start = str;
            mmelody->loop_count = 0;
            continue;
            
            /* Loop.  */
        case '>':
            mmelody->loop_count++;
            if (!num)
                num = 2;

            if (mmelody->loop_count < num)
            {
                /* Jump to start of loop.  If no start of loop symbol,
                   jump to start.  */
                str = mmelody->loop_start;
                if (!str)
                    str = mmelody->start;
            }
            continue;

            /* Alternate endings.  */
        case '[':
            if (mmelody->loop_count == num - 1)
                continue;

            /* Skip to next alternate ending, the end of loop, or end of
               melody.  */
            while (*str && *str != '[' && *str != '>')
                str++;
            continue;

        case '*':
            if (num)
                mmelody_symbol_fraction_set (mmelody, num);
            continue;

        case '@':
            if (num)
                mmelody_speed_set (mmelody, num);
            continue;

        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
            note = mmelody_char_to_note (cmd);

            if (have_hash)
                note++;

            if (have_num)
                mmelody->octave = num;

            if (modifier == '+')
                note += MMELODY_SCALE_SIZE;

            if (modifier == '-')
                note -= MMELODY_SCALE_SIZE;

            /* Convert note to MIDI note number.  */
            note += (mmelody->octave + 1) * MMELODY_SCALE_SIZE;

            /* Fall through.  */
        case ' ':

            while (*str == '/')
            {
                duration++;
                str++;
            }

            duration = duration * MMELODY_DURATION_FRACTION;

            if (*str == '.')
            {
                /* Dotted quarter note.  */
                duration += duration / 2;
                str++;
            }
            else if (*str == ',')
            {
                /* Eighth note.  */
                str++;
                duration = duration / 2;
            }

            mmelody_note_on (mmelody, note, duration);
            return str;
            break;
            
        default:
            return str;
            break;
        }
    }
    return str;
}


void 
mmelody_play (mmelody_t mmelody, const char *str)
{
    mmelody->cur = mmelody->start = str;
    mmelody->loop_start = 0;
    mmelody->loop_count = 0;
    mmelody->octave = MMELODY_OCTAVE_DEFAULT;
    /* Default to quarter notes.  */
    mmelody_symbol_fraction_set (mmelody, 4);
    /* Stop what is currently sounding.  */
    mmelody_note_off (mmelody);

    mmelody->ticks1 = 0;
    mmelody->ticks2 = 1;
}


/* Set (base) speed in beats per minute (BPM).  */
void 
mmelody_speed_set (mmelody_t mmelody, mmelody_speed_t speed)
{
    /* With 8 bits for the tempo (in bpm), the max tempo is 255 bpm.
       This corresponds to 4.25 beats per second.  If the minimum time
       to release a note is 1 / 8 of a quarter note, then we need to
       poll at a rate of at least 4.25 * 8 = 34 times per second.  */

    mmelody->speed = speed;
    mmelody_ticker_set (mmelody);
}


/* Set volume as percentage of maximum.  */
void 
mmelody_volume_set (mmelody_t mmelody, mmelody_volume_t volume)
{
    mmelody->volume = volume;
}


bool
mmelody_active_p (mmelody_t mmelody)
{
    return mmelody->cur != 0;
}


void
mmelody_update (mmelody_t mmelody)
{
    if (!mmelody->cur)
        return;

    if (mmelody->ticks1)
    {
        mmelody->ticks1--;
        if (!mmelody->ticks1)
            mmelody_note_off (mmelody);
    }
    else
    {
        if (mmelody->ticks2)
        {
            mmelody->ticks2--;
            if (!mmelody->ticks2)
                mmelody->cur = mmelody_scan (mmelody, mmelody->cur);
        }
    }
}


mmelody_t
mmelody_init (mmelody_obj_t *mmelody, 
              uint16_t poll_rate,
              mmelody_callback_t play_callback,
              void *play_callback_data)
{
    mmelody->poll_rate = poll_rate;
    mmelody->play_callback = play_callback;
    mmelody->play_callback_data = play_callback_data;
    mmelody->volume = 100;
    mmelody->note = 0;
    mmelody->ticks1 = 0;
    mmelody->ticks2 = 0;
    mmelody->release_ticks = (poll_rate * MMELODY_RELEASE_MS) / 1000;
    mmelody->symbol_fraction = 4;
    mmelody->speed = MMELODY_SPEED_DEFAULT;

    return mmelody;
}
