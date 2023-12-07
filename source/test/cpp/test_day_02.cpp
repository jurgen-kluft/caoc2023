#include "cbase/c_allocator.h"
#include "cbase/c_console.h"
#include "cbase/c_darray.h"
#include "cbase/c_limits.h"
#include "cbase/c_integer.h"
#include "cbase/c_printf.h"
#include "cbase/c_runes.h"
#include "cbase/c_stream.h"
#include "cbase/c_va_list.h"

#include "ctext/c_parser2.h"
#include "ctext/c_text_stream.h"

#include "caoc2023/c_aoc.h"

#include "cunittest/cunittest.h"

#include <time.h>
#include <stdio.h>

extern unsigned char day2[];
extern unsigned int  day2_len;

using namespace ncore;

UNITTEST_SUITE_BEGIN(day02)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        struct set_t
        {
            s8          m_colors[4];
            inline void reset()
            {
                m_colors[0] = 0;
                m_colors[1] = 0;
                m_colors[2] = 0;
                m_colors[3] = 0;
            }
        };

        struct game_t
        {
            s32   m_id;
            s32   m_set_cnt;
            set_t m_set_array[8];

            inline void reset()
            {
                m_id      = -1;
                m_set_cnt = 0;
                for (s32 i = 0; i < 8; ++i)
                    m_set_array[i].reset();
            }
        };

        struct input_t
        {
            const char* input;
            u32         input_len;
            const char* cursor;

            input_t(const char* input, u32 input_len)
                : input(input)
                , input_len(input_len)
                , cursor(input)
            {
            }

            inline char peek() const { return *cursor; }

            inline bool consume(const char* str)
            {
                const char* iter = cursor;
                while (*str != 0)
                {
                    if (endOfFile())
                        return false;
                    if (endOfLine())
                        return false;

                    if (*iter != *str)
                        return false;

                    ++iter;
                    ++str;
                }
                cursor = iter;
                return true;
            }

            inline bool endOfFile() const { return (cursor >= input + input_len); }
            inline bool endOfLine() const { return (*cursor == '\n'); }

            inline bool next()
            {
                if (cursor >= input + input_len)
                    return false;
                ++cursor;
                return true;
            }
        };

        static bool ParseNumber(input_t & in, s32 & number)
        {
            number = 0;
            while (!in.endOfLine())
            {
                const char c = in.peek();
                if (c >= '0' && c <= '9')
                {
                    number = number * 10 + (c - '0');
                    in.next();
                    continue;
                }
                break;
            }
            return true;
        }

        static bool ParseColor(input_t & in, s8 & color)
        {
            color = -1;
            if (in.consume("red"))
            {
                color = 0;
            }
            else if (in.consume("green"))
            {
                color = 1;
            }
            else if (in.consume("blue"))
            {
                color = 2;
            }
            else
            {
                return false;
            }
            return true;
        }

        static void SkipWhitespace(input_t & in)
        {
            while (!in.endOfLine())
            {
                const char c = in.peek();
                if (c == ' ' || c == '\t')
                {
                    in.next();
                    continue;
                }
                break;
            }
        }

        static bool ParseCube(input_t & in, s32 & number, s8 & color)
        {
            // Parse number
            SkipWhitespace(in);
            if (ParseNumber(in, number) == false)
                return false;

            // Parse color
            SkipWhitespace(in);
            if (ParseColor(in, color) == false)
                return false;

            return true;
        }

        static bool ParseGame(input_t & in, game_t & game)
        {
            // Example line:
            // Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green

            // Parse, Game 'number':

            if (in.consume("Game") == false)
                return false;
            SkipWhitespace(in);

            // Parse id
            s32 game_id = 0;
            if (ParseNumber(in, game_id) == false)
                return false;
            game.m_id = game_id;

            if (in.consume(":") == false)
                return false;

            while (in.endOfLine() == false)
            {
                SkipWhitespace(in);

                // Parse number
                SkipWhitespace(in);
                s32 number = 0;
                if (ParseNumber(in, number) == false)
                    return false;

                // Parse color
                SkipWhitespace(in);
                s8 color = 0;
                if (ParseColor(in, color) == false)
                    return false;

                // Add 'number : color' to the current set of this game
                game.m_set_array[game.m_set_cnt].m_colors[color] = number;

                SkipWhitespace(in);
                if (in.endOfLine())
                {
                    ++game.m_set_cnt;
                    in.next();
                    break;
                }

                const char c = in.peek();
                if (c == ';')
                {
                    // End of set
                    ++game.m_set_cnt;
                    in.next();
                    continue;
                }
                else if (c == ',')
                {
                    // Continue to parse the next (number : color) pair
                    in.next();
                    continue;
                }
                else
                {
                    // Unexpected character
                    return false;
                }
            }
            return true;
        }

        struct bags_t
        {
            s8 m_colors[4];
        };

        static bool IsGamePossible(game_t const& game, bags_t const& bags)
        {
            for (s32 i = 0; i < game.m_set_cnt; ++i)
            {
                for (s32 j = 0; j < 3; ++j)
                {
                    if (game.m_set_array[i].m_colors[j] > bags.m_colors[j])
                        return false;
                }
            }
            return true;
        }

        UNITTEST_TEST(part_1)
        {
            s32     number = 0;
            s32     sum    = 0;
            input_t input((const char*)day2, day2_len);

            bags_t bags;
            bags.m_colors[0] = 12; // red
            bags.m_colors[1] = 13; // green
            bags.m_colors[2] = 14; // blue

            game_t game;
            game.reset();
            while (ParseGame(input, game))
            {
                if (IsGamePossible(game, bags))
                {
                    sum += game.m_id;
                }

                game.reset();
            }

            printf(crunes_t("part 1, number of possible games = %u\n"), va_t(sum));
        }

        UNITTEST_TEST(part_2) {}
    }
}
UNITTEST_SUITE_END
