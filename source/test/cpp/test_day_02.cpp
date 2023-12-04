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

        class digits_parser_t
        {
        public:
            digits_parser_t() {}

            bool parse(crunes_t& line, u32& number)
            {
                number = 0;

                u32 cursor      = 0;
                s8  first_digit = -1;
                s8  last_digit  = -1;
                while (!line.at_end(cursor))
                {
                    uchar32 c = line.read(cursor);

                    // if c is a number set the digit, ignore any other character
                    if (c >= '0' && c <= '9')
                    {
                        c = c - '0';
                        if (first_digit == -1)
                        {
                            first_digit = (s8)c;
                            last_digit = (s8)c;
                        }
                        else
                        {
                            last_digit = (s8)c;
                        }
                    }
                }
                number = (u32)first_digit*10 + (u32)last_digit;

                return true;
            }

            static s32 match_digit(const char* digit, s32 digitlen)
            {
                static const char* digits[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
                s8 match[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
                s8 match_count = 9;

                for (s8 i = 0; i < digitlen; ++i)
                {
                    s8 count = 0;
                    for (s8 j = 0; j < match_count; ++j)
                    {
                        s8 index = match[j];
                        if (digit[i] == digits[index][i])
                        {
                            match[count++] = match[j];
                            if (digits[index][i + 1] == '\0')
                                return index + 1;
                        }
                    }
                    match_count = count;
				}

                // When a partial match is found (first N characters), the match_count will be greater than 0.
				return match_count > 0 ? 0 : -1;
			}

            static void shift_digit(char* digit_str, s32& digit_len)
            {
                // move all digits one character to the left
                // e.g. handle "twone" -> 2, 1
                for (s32 i = 0; i < digit_len - 1; ++i)
                    digit_str[i] = digit_str[i + 1];
                digit_str[digit_len] = '\0';
                digit_len--;
            }

            static s8 handle_digit(char* digit_str, s32& digit_len, s8& first_digit, s8& last_digit)
            {
                s32 match = match_digit(digit_str, digit_len);
                if (match >= 1 && match <= 9)
                {
                    if (first_digit == -1)
                    {
                        first_digit = (s8)match;
                        // consume the textual digit
                        digit_len = 0;
                    }
                    else
                    {
                        shift_digit(digit_str, digit_len);
                    }
                    last_digit = (s8)match;
                }
                else if (match  == -1)
                {
                    // no leading partial match -> eat one character
                    shift_digit(digit_str, digit_len);
                }
                return match;
            }

            bool parse2(crunes_t& line, u32& number)
            {
                number = 0;

                // Can be any of the 9 digits
                // one, two, three, four, five, six, seven, eight, nine
                // the longest digit is 5 characters, so we can use a 8 character buffer
                s32  digits   = 0;
                char digit_str[32] = {0};

                u32 cursor      = 0;
                s8  first_digit = -1;
                s8  last_digit  = -1;
                while (!line.at_end(cursor))
                {
                    uchar32 c = line.read(cursor);

                    // if c is a number set the digit, ignore any other character
                    if (c >= '0' && c <= '9')
                    {
                        digits = 0; // reset the textual digit decoder

                        c = c - '0';
                        if (first_digit == -1)
                            first_digit = (s8)c;
                        last_digit = (s8)c;
                    }
                    else
                    {
                        if (c <= 'z' && c >= 'a')
                        {
                            digit_str[digits++] = (char)c;
                            digit_str[digits]   = '\0';

                            handle_digit(digit_str, digits, first_digit, last_digit);
                        }
                    }
                }

                while (digits > 0)
                {
                    if (handle_digit(digit_str, digits, first_digit, last_digit) == 0)
                    {
                        // this case is when the trailing text has a partial match for a digit, however we are not
                        // adding more digits to the number, so we need to shift the digit string to the left to keep
                        // finding a match
                        shift_digit(digit_str, digits);
                    }
                }

                number = (u32)first_digit * 10 + (u32)last_digit;

                return true;
            }
        };

        UNITTEST_TEST(part_1)
        {
            mem_stream    memtext(day2, day2_len);
            text_stream_t text(&memtext, text_stream_t::encoding_ascii);

            crunes_t        line;
            digits_parser_t lp;
            u32             number = 0;
            u32             sum    = 0;

            while (text.readLine(line))
            {
                CHECK_TRUE(lp.parse(line, number));
                sum += number;
            }

            printf(crunes_t("part 1, sum = %u\n"), va_t(sum));

            text.close();
            memtext.close();
        }

        UNITTEST_TEST(part_2)
        {
            mem_stream    memtext(day2, day2_len);
            text_stream_t text(&memtext, text_stream_t::encoding_ascii);

            crunes_t        line;
            digits_parser_t lp;

            u32 number = 0;
            u32 sum    = 0;

            while (text.readLine(line))
            {
                CHECK_TRUE(lp.parse2(line, number));
                sum += number;
            }

            printf(crunes_t("part 2, sum = %u\n"), va_t(sum));

            text.close();
            memtext.close();
        }
    }
}
UNITTEST_SUITE_END
