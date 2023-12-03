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

extern unsigned char day1_txt[];
extern unsigned int  day1_txt_len;

using namespace ncore;

UNITTEST_SUITE_BEGIN(day01)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        struct line_t
        {
            line_t(u16 _x1, u16 _y1, u16 _x2, u16 _y2)
                : x1(_x1)
                , y1(_y1)
                , x2(_x2)
                , y2(_y2)
            {
            }
            u16 x1, y1;
            u16 x2, y2;
        };

        class digits_parser_t
        {
        public:
            digits_parser_t()
            {
            }

            bool parse(crunes_t& line, u8& d1, u8& d2)
            {
                d1 = 0;
                d2 = 0;
                
                u32 cursor = 0;
                s32 digits = 0;
                while (!line.at_end(cursor) || digits < 2)
                {
                    uchar32 c = line.read(cursor);

                    // if c is a number set the digit, ignore any other character
                    if (c >= '0' && c <= '9')
                    {
                        if (digits == 0)
                        {
                            d1 = c - '0';
                            digits++;
                        }
                        else if (digits == 1)
                        {
                            d2 = c - '0';
                            digits++;
                        }
                    }
                }

                return true;
            }
        };

        UNITTEST_TEST(part_1)
        {
            mem_stream    memtext(day1_txt, day1_txt_len);
            text_stream_t text(&memtext, text_stream_t::encoding_ascii);

            array_t<line_t>* lines = array_t<line_t>::create(0, 1024);

            crunes_t      line;
            digits_parser_t lp;
            u8            d1, d2;

            while (text.readLine(line))
            {
                CHECK_TRUE(lp.parse(line, d1, d2));
            }

            lines->destroy(lines);

            text.close();
            memtext.close();
        }

        UNITTEST_TEST(part_2)
        {
            mem_stream    memtext(day1_txt, day1_txt_len);
            text_stream_t text(&memtext, text_stream_t::encoding_ascii);

            array_t<line_t>* lines = array_t<line_t>::create(0, 1024);

            crunes_t      line;
            digits_parser_t lp;
            u8            d1, d2;

            while (text.readLine(line))
            {
                CHECK_TRUE(lp.parse(line, d1, d2));
            }

            lines->destroy(lines);

            text.close();
            memtext.close();
        }
    }
}
UNITTEST_SUITE_END
