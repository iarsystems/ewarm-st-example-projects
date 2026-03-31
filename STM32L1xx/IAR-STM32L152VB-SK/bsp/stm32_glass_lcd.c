
#include "stm32_glass_lcd.h"


/**
================================================================================
                              GLASS LCD MAPPING
================================================================================
               A
           ---------
          |\   |J  /|   _
        F | H  |  K |B | | COL
          |  \ | /  |   -
          --G-- --M--   _
          |   /| \  |  | |
        E |  Q |  N |C  -
          | /  |P  \|   _
           ---------   | |DP
              D         -
A LCD 14 segments character coding is based on the following matrix:
 SEG |  0  1  2  3
-----|-------------
COM0 |  N  D  P  -
COM1 |  M  C  Q  E
COM2 |  K  B  G  F
COM3 |  J  A  H  -


               A
           ---------
          |         |   _
        F |         |B | | COL
          |         |   -
           ----G----    _
          |         |  | |
        E |         |C  -
          |         |   _
           ---------   | |DP
              D         -
A LCD 7 segments character coding is based on the following matrix:

0-1
 SEG |  0  1
-----|-------
COM0 |  A  F
COM1 |  B  Q
COM2 |  C  E
COM3 |  -  D

2-3
 SEG |  0  1
-----|-------
COM0 |  B  A
COM1 |  G  F
COM2 |  C  E
COM3 |  D  -

*/

#define S14_SEG_A     ((3*4)+1)
#define S14_SEG_B     ((2*4)+1)
#define S14_SEG_C     ((1*4)+1)
#define S14_SEG_D     ((0*4)+1)
#define S14_SEG_E     ((1*4)+3)
#define S14_SEG_F     ((2*4)+3)
#define S14_SEG_G     ((2*4)+2)
#define S14_SEG_H     ((3*4)+2)
#define S14_SEG_J     ((3*4)+0)
#define S14_SEG_K     ((2*4)+0)
#define S14_SEG_M     ((1*4)+0)
#define S14_SEG_N     ((0*4)+0)
#define S14_SEG_P     ((0*4)+2)
#define S14_SEG_Q     ((1*4)+2)

#define S7A_SEG_A     ((0*4)+0)
#define S7A_SEG_B     ((1*4)+0)
#define S7A_SEG_C     ((2*4)+0)
#define S7A_SEG_D     ((3*4)+1)
#define S7A_SEG_E     ((2*4)+1)
#define S7A_SEG_F     ((0*4)+1)
#define S7A_SEG_G     ((1*4)+1)

#define S7B_SEG_A     ((0*4)+1)
#define S7B_SEG_B     ((0*4)+0)
#define S7B_SEG_C     ((2*4)+0)
#define S7B_SEG_D     ((3*4)+0)
#define S7B_SEG_E     ((2*4)+1)
#define S7B_SEG_F     ((1*4)+1)
#define S7B_SEG_G     ((1*4)+0)

#define SEG14(a,b,c,d,e,f,g,h,j,k,m,n,p,q) \
  (a<<S14_SEG_A) | (b<<S14_SEG_B) | (c<<S14_SEG_C) | (d<<S14_SEG_D) |\
  (e<<S14_SEG_E) | (f<<S14_SEG_F) | (g<<S14_SEG_G) | (h<<S14_SEG_H) |\
  (j<<S14_SEG_J) | (k<<S14_SEG_K) | (m<<S14_SEG_M) | (n<<S14_SEG_N) |\
  (p<<S14_SEG_P) | (q<<S14_SEG_Q)

#define SEG7A(a,b,c,d,e,f,g) \
  (a<<S7A_SEG_A) | (b<<S7A_SEG_B) | (c<<S7A_SEG_C) | (d<<S7A_SEG_D) | \
  (e<<S7A_SEG_E) | (f<<S7A_SEG_F) | (g<<S7A_SEG_G)

#define SEG7B(a,b,c,d,e,f,g) \
  (a<<S7B_SEG_A) | (b<<S7B_SEG_B) | (c<<S7B_SEG_C) | (d<<S7B_SEG_D) | \
  (e<<S7B_SEG_E) | (f<<S7B_SEG_F) | (g<<S7B_SEG_G)

#define SEG_MASK(seg)     (seg & (32-1))
#define SEG_EL(seg,com)   (volatile unsigned int *)(PERIPH_BASE + 0x2000000 + ((0x2400 + 0x14 + ((com*2) + ((seg<32)?0:1))*4)*32) + (SEG_MASK(seg)*4))

#define SEG_PL 				SEG_EL(39,3)  // +    0
#define SEG_MN 				SEG_EL(39,0)  // -    1
#define SEG_S14_1A  	SEG_EL(37,3)  // 1A   2
#define SEG_S14_1B  	SEG_EL(37,2)  // 1B   3
#define SEG_S14_1C  	SEG_EL(37,1)  // 1C   4
#define SEG_S14_1D  	SEG_EL(37,0)  // 1D   5
#define SEG_S14_1E  	SEG_EL(39,1)  // 1E   6
#define SEG_S14_1F  	SEG_EL(39,2)  // 1F   7
#define SEG_S14_1G  	SEG_EL(38,2)  // 1G   8
#define SEG_S14_1H  	SEG_EL(38,3)  // 1H   9
#define SEG_S14_1J  	SEG_EL(36,3)  // 1J   10
#define SEG_S14_1K  	SEG_EL(36,2)  // 1K   11
#define SEG_S14_1M  	SEG_EL(36,1)  // 1M   12
#define SEG_S14_1N  	SEG_EL(36,0)  // 1N   13
#define SEG_S14_1P  	SEG_EL(38,0)  // 1P   14
#define SEG_S14_1Q  	SEG_EL(38,1)  // 1Q   15
#define SEG_S14_1DP 	SEG_EL(35,0)  // 1DP  16
#define SEG_S14_1COL	SEG_EL(35,3)  // 1COL 17

#define SEG_S14_2A  	SEG_EL(33,3)  // 2A   18
#define SEG_S14_2B  	SEG_EL(33,2)  // 2B   19
#define SEG_S14_2C  	SEG_EL(33,1)  // 2C   20
#define SEG_S14_2D  	SEG_EL(33,0)  // 2D   21
#define SEG_S14_2E  	SEG_EL(35,1)  // 2E   22
#define SEG_S14_2F  	SEG_EL(35,2)  // 2F   23
#define SEG_S14_2G  	SEG_EL(34,2)  // 2G   24
#define SEG_S14_2H  	SEG_EL(34,3)  // 2H   25
#define SEG_S14_2J  	SEG_EL(32,3)  // 2J   26
#define SEG_S14_2K  	SEG_EL(32,2)  // 2K   27
#define SEG_S14_2M  	SEG_EL(32,1)  // 2M   28
#define SEG_S14_2N  	SEG_EL(32,0)  // 2N   29
#define SEG_S14_2P  	SEG_EL(34,0)  // 2P   30
#define SEG_S14_2Q  	SEG_EL(34,1)  // 2Q   31
#define SEG_S14_2DP 	SEG_EL(31,0)  // 2DP  32
#define SEG_S14_2COL	SEG_EL(31,3)  // 2COL 33

#define SEG_S14_3A  	SEG_EL(29,3)  // 3A   34
#define SEG_S14_3B  	SEG_EL(29,2)  // 3B   35
#define SEG_S14_3C  	SEG_EL(29,1)  // 3C   36
#define SEG_S14_3D  	SEG_EL(29,0)  // 3D   37
#define SEG_S14_3E  	SEG_EL(31,1)  // 3E   38
#define SEG_S14_3F  	SEG_EL(31,2)  // 3F   39
#define SEG_S14_3G  	SEG_EL(30,2)  // 3G   40
#define SEG_S14_3H  	SEG_EL(30,3)  // 3H   41
#define SEG_S14_3J  	SEG_EL(28,3)  // 3J   42
#define SEG_S14_3K  	SEG_EL(28,2)  // 3K   43
#define SEG_S14_3M  	SEG_EL(28,1)  // 3M   44
#define SEG_S14_3N  	SEG_EL(28,0)  // 3N   45
#define SEG_S14_3P  	SEG_EL(30,0)  // 3P   46
#define SEG_S14_3Q  	SEG_EL(30,1)  // 3Q   47
#define SEG_S14_3DP 	SEG_EL(27,0)  // 3DP  48
#define SEG_S14_3COL	SEG_EL(27,3)  // 3COL 49

#define SEG_S14_4A  	SEG_EL(25,3)  // 4A   50
#define SEG_S14_4B  	SEG_EL(25,2)  // 4B   51
#define SEG_S14_4C  	SEG_EL(25,1)  // 4C   52
#define SEG_S14_4D  	SEG_EL(25,0)  // 4D   53
#define SEG_S14_4E  	SEG_EL(27,1)  // 4E   54
#define SEG_S14_4F  	SEG_EL(27,2)  // 4F   55
#define SEG_S14_4G  	SEG_EL(26,2)  // 4G   56
#define SEG_S14_4H  	SEG_EL(26,3)  // 4H   57
#define SEG_S14_4J  	SEG_EL(24,3)  // 4J   58
#define SEG_S14_4K  	SEG_EL(24,2)  // 4K   59
#define SEG_S14_4M  	SEG_EL(24,1)  // 4M   60
#define SEG_S14_4N  	SEG_EL(24,0)  // 4N   61
#define SEG_S14_4P  	SEG_EL(26,0)  // 4P   62
#define SEG_S14_4Q  	SEG_EL(26,1)  // 4Q   63
#define SEG_S14_4DP 	SEG_EL(23,0)  // 4DP  64
#define SEG_S14_4COL	SEG_EL(23,3)  // 4COL 65

#define SEG_S14_5A  	SEG_EL(21,3)  // 5A   66
#define SEG_S14_5B  	SEG_EL(21,2)  // 5B   67
#define SEG_S14_5C  	SEG_EL(21,1)  // 5C   68
#define SEG_S14_5D  	SEG_EL(21,0)  // 5D   69
#define SEG_S14_5E  	SEG_EL(23,1)  // 5E   70
#define SEG_S14_5F  	SEG_EL(23,2)  // 5F   71
#define SEG_S14_5G  	SEG_EL(22,2)  // 5G   72
#define SEG_S14_5H  	SEG_EL(22,3)  // 5H   73
#define SEG_S14_5J  	SEG_EL(20,3)  // 5J   74
#define SEG_S14_5K  	SEG_EL(20,2)  // 5K   75
#define SEG_S14_5M  	SEG_EL(20,1)  // 5M   76
#define SEG_S14_5N  	SEG_EL(20,0)  // 5N   77
#define SEG_S14_5P  	SEG_EL(22,0)  // 5P   78
#define SEG_S14_5Q  	SEG_EL(22,1)  // 5Q   79
#define SEG_S14_5DP 	SEG_EL(19,0)  // 5DP  80
#define SEG_S14_5COL	SEG_EL(19,3)  // 5COL 81

#define SEG_S14_6A  	SEG_EL(17,3)  // 6A   82
#define SEG_S14_6B  	SEG_EL(17,2)  // 6B   83
#define SEG_S14_6C  	SEG_EL(17,1)  // 6C   84
#define SEG_S14_6D  	SEG_EL(17,0)  // 6D   85
#define SEG_S14_6E  	SEG_EL(19,1)  // 6E   86
#define SEG_S14_6F  	SEG_EL(19,2)  // 6F   87
#define SEG_S14_6G  	SEG_EL(18,2)  // 6G   88
#define SEG_S14_6H  	SEG_EL(18,3)  // 6H   89
#define SEG_S14_6J  	SEG_EL(16,3)  // 6J   90
#define SEG_S14_6K  	SEG_EL(16,2)  // 6K   91
#define SEG_S14_6M  	SEG_EL(16,1)  // 6M   92
#define SEG_S14_6N  	SEG_EL(16,0)  // 6N   93
#define SEG_S14_6P  	SEG_EL(18,0)  // 6P   94
#define SEG_S14_6Q  	SEG_EL(18,1)  // 6Q   95
#define SEG_S14_6DP 	SEG_EL(15,0)  // 6DP  96

#define SEG_S14_7A  	SEG_EL(13,3)  // 7A   97
#define SEG_S14_7B  	SEG_EL(13,2)  // 7B   98
#define SEG_S14_7C  	SEG_EL(13,1)  // 7C   99
#define SEG_S14_7D  	SEG_EL(13,0)  // 7D   100
#define SEG_S14_7E  	SEG_EL(15,1)  // 7E   101
#define SEG_S14_7F  	SEG_EL(15,2)  // 7F   102
#define SEG_S14_7G  	SEG_EL(14,2)  // 7G   103
#define SEG_S14_7H  	SEG_EL(14,3)  // 7H   104
#define SEG_S14_7J  	SEG_EL(12,3)  // 7J   105
#define SEG_S14_7K  	SEG_EL(12,2)  // 7K   106
#define SEG_S14_7M  	SEG_EL(12,1)  // 7M   107
#define SEG_S14_7N  	SEG_EL(12,0)  // 7N   108
#define SEG_S14_7P  	SEG_EL(14,0)  // 7P   109
#define SEG_S14_7Q  	SEG_EL(14,1)  // 7Q   110

#define SEG_MICRO 		SEG_EL(1 ,3)  // A1   111
#define SEG_MIL 		  SEG_EL(1 ,2)  // A2   112
#define SEG_H					SEG_EL(1 ,1)  // A3   113
#define SEG_F					SEG_EL(1 ,0)  // A4   114

#define SEG_BATT			SEG_EL(2 ,0)  // BRBL 115
#define SEG_BATT_0		SEG_EL(2 ,3)  // B0   116
#define SEG_BATT_1		SEG_EL(2 ,2)  // B1   117
#define SEG_BATT_2		SEG_EL(2 ,1)  // B2   118

#define SEG_BARR_PL 	SEG_EL(0 ,3)  // PL   119
#define SEG_BARR_P0 	SEG_EL(0 ,2)  // P0   120
#define SEG_BARR_P1 	SEG_EL(0 ,1)  // P1   121
#define SEG_BARR_P2 	SEG_EL(0 ,0)  // P2   122
#define SEG_BARR_P3 	SEG_EL(43,0)  // P3   123
#define SEG_BARR_P4 	SEG_EL(43,1)  // P4   124
#define SEG_BARR_P5 	SEG_EL(43,2)  // P5   125
#define SEG_BARR_P6 	SEG_EL(43,3)  // P6   126
#define SEG_BARR_P7 	SEG_EL(42,3)  // P7   127
#define SEG_BARR_P8 	SEG_EL(42,2)  // P8   128
#define SEG_BARR_P9 	SEG_EL(42,1)  // P9   129
#define SEG_BARR_PR 	SEG_EL(42,0)  // PR   130

#define SEG_AL				SEG_EL(3 ,0)  // AL   131
#define SEG_AU				SEG_EL(3 ,1)  // AU   132
#define SEG_AR				SEG_EL(3 ,2)  // AR   133
#define SEG_AD				SEG_EL(3 ,3)  // AD   134

#define SEG_OLIMEX		SEG_EL(15,3)  // SB   135

#define SEG_S7_8A 		SEG_EL(10,0)  // 8A   136
#define SEG_S7_8B 		SEG_EL(10,1)  // 8B   137
#define SEG_S7_8C 		SEG_EL(10,2)  // 8C   138
#define SEG_S7_8D 		SEG_EL(11,3)  // 8D   139
#define SEG_S7_8E 		SEG_EL(11,2)  // 8E   140
#define SEG_S7_8F 		SEG_EL(11,0)  // 8F   141
#define SEG_S7_8G 		SEG_EL(11,1)  // 8G   142
#define SEG_S7_8P 		SEG_EL(10,3)  // 8P   143

#define SEG_S7_9A 		SEG_EL(8 ,0)  // 9A   144
#define SEG_S7_9B 		SEG_EL(8 ,1)  // 9B   145
#define SEG_S7_9C 		SEG_EL(8 ,2)  // 9C   146
#define SEG_S7_9D 		SEG_EL(9 ,3)  // 9D   147
#define SEG_S7_9E 		SEG_EL(9 ,2)  // 9E   148
#define SEG_S7_9F 		SEG_EL(9 ,0)  // 9F   149
#define SEG_S7_9G 		SEG_EL(9 ,1)  // 9G   150
#define SEG_S7_9P 		SEG_EL(8 ,3)  // 10P  151
#define SEG_S7_9COL		SEG_EL(7 ,3)  // 10COLON  152

#define SEG_S7_10A 		SEG_EL(7 ,0)  // 10A  153
#define SEG_S7_10B 		SEG_EL(6 ,0)  // 10B  154
#define SEG_S7_10C 		SEG_EL(6 ,2)  // 10C  155
#define SEG_S7_10D 		SEG_EL(6 ,3)  // 10D  156
#define SEG_S7_10E 		SEG_EL(7 ,2)  // 10E  157
#define SEG_S7_10F 		SEG_EL(7 ,1)  // 10F  158
#define SEG_S7_10G 		SEG_EL(6 ,1)  // 10G  159
#define SEG_S7_10P 		SEG_EL(5 ,3)  // 10P  160

#define SEG_S7_11A 		SEG_EL(5 ,0)  // 11A  161
#define SEG_S7_11B 		SEG_EL(4 ,0)  // 11B  162
#define SEG_S7_11C 		SEG_EL(4 ,2)  // 11C  163
#define SEG_S7_11D 		SEG_EL(4 ,3)  // 11D  164
#define SEG_S7_11E 		SEG_EL(5 ,2)  // 11E  165
#define SEG_S7_11F 		SEG_EL(5 ,1)  // 11F  166
#define SEG_S7_11G 		SEG_EL(4 ,1)  // 11G  167

const uint16_t S14_FontMap[]=
{
  SEG14(1,1,1,1,1,1,0,0,0,1,0,0,0,1), /* 0   - 0 */
  SEG14(0,1,1,0,0,0,0,0,0,0,0,0,0,0), /* 1   - 1 */
  SEG14(1,1,0,1,1,0,1,0,0,0,1,0,0,0), /* 2   - 2 */
  SEG14(1,1,1,1,0,0,0,0,0,0,1,0,0,0), /* 3   - 3 */
  SEG14(0,1,1,0,0,1,1,0,0,0,1,0,0,0), /* 4   - 4 */
  SEG14(1,0,0,1,0,1,1,0,0,0,0,1,0,0), /* 5   - 5 */
  SEG14(1,0,1,1,1,1,1,0,0,0,1,0,0,0), /* 6   - 6 */
  SEG14(1,1,1,0,0,0,0,0,0,0,0,0,0,0), /* 7   - 7 */
  SEG14(1,1,1,1,1,1,1,0,0,0,1,0,0,0), /* 8   - 8 */
  SEG14(1,1,1,1,0,1,1,0,0,0,1,0,0,0), /* 9   - 9 */
  SEG14(1,1,1,0,1,1,1,0,0,0,1,0,0,0), /* 10  - A */
  SEG14(1,1,1,1,0,0,0,0,1,0,1,0,1,0), /* 11  - B */
  SEG14(1,0,0,1,1,1,0,0,0,0,0,0,0,0), /* 12  - C */
  SEG14(1,1,1,1,0,0,0,0,1,0,0,0,1,0), /* 13  - D */
  SEG14(1,0,0,1,1,1,1,0,0,0,1,0,0,0), /* 14  - E */
  SEG14(1,0,0,0,1,1,1,0,0,0,0,0,0,0), /* 15  - F */
  SEG14(0,0,0,0,0,0,0,0,0,0,0,0,0,0), /* 16  -   */
  SEG14(0,0,0,0,0,0,0,0,0,0,0,0,0,0), /* 17  -   */
  SEG14(0,0,0,0,0,0,0,0,0,0,0,0,0,0), /* 18  -   */
  SEG14(0,0,0,0,0,0,0,0,0,0,0,0,0,0), /* 19  -   */
  SEG14(0,0,0,0,0,0,0,0,0,0,0,0,0,0), /* 20  -   */
  SEG14(0,0,0,0,0,0,0,0,0,0,0,0,0,0), /* 21  -   */
  SEG14(0,0,0,0,0,0,0,0,0,0,0,0,0,0), /* 22  -   */
  SEG14(0,0,0,0,0,0,0,0,0,0,0,0,0,0), /* 23  -   */
  SEG14(1,0,0,1,0,0,1,0,1,0,1,0,1,0), /* 24  - 01*/
  SEG14(0,0,0,0,0,0,1,1,0,1,1,0,1,0), /* 25  - 02*/
  SEG14(1,0,0,1,1,1,1,0,1,0,1,0,1,0), /* 26  - 03*/
  SEG14(1,1,0,0,0,1,1,0,0,0,1,0,0,0), /* 27  - 04*/
  SEG14(0,0,0,0,1,1,0,1,0,1,0,0,0,0), /* 28  - 05*/
  SEG14(0,0,0,1,0,0,1,0,1,0,1,0,1,0), /* 29  - +-*/
  SEG14(0,0,0,0,0,0,0,0,1,0,0,1,1,1), /* 30  - 06*/
  SEG14(0,0,0,0,0,0,0,1,1,1,0,0,1,0), /* 31  - 07*/
  SEG14(0,0,0,0,0,0,0,0,0,0,0,0,0,0), /* 32  -   */
  SEG14(0,1,1,0,0,0,0,0,0,0,0,0,0,0), /* 33  - | */
  SEG14(0,0,0,0,0,1,0,0,1,0,0,0,0,0), /* 34  - 08*/
  SEG14(1,1,1,1,1,1,1,1,1,1,1,1,1,1), /* 35  - 09*/
  SEG14(1,0,1,1,0,1,1,0,1,0,1,0,1,0), /* 36  - $*/
  SEG14(0,0,1,0,0,1,1,1,0,1,1,1,0,1), /* 37  - 10*/
  SEG14(1,0,1,1,0,0,0,1,0,1,0,1,0,1), /* 38  - 11*/
  SEG14(0,0,0,0,0,0,0,0,0,1,0,0,0,0), /* 39  - ' */
  SEG14(0,0,0,0,0,0,0,0,0,1,0,1,0,0), /* 40  - < */
  SEG14(0,0,0,0,0,0,0,1,0,0,0,0,0,1), /* 41  - > */
  SEG14(0,0,0,0,0,0,1,1,1,1,1,1,1,1), /* 42  - * */
  SEG14(0,0,0,0,0,0,1,0,1,0,1,0,1,0), /* 43  - + */
  SEG14(0,0,0,0,0,0,0,0,0,0,0,0,0,1), /* 44  - , */
  SEG14(0,0,0,0,0,0,1,0,0,0,1,0,0,0), /* 45  - - */
  SEG14(0,0,0,1,1,0,1,0,0,0,0,0,1,0), /* 46  - 12*/
  SEG14(0,0,0,0,0,0,0,0,0,1,0,0,0,1), /* 47  - / */
  SEG14(1,1,1,1,1,1,0,0,0,1,0,0,0,1), /* 48  - 0 */
  SEG14(0,1,1,0,0,0,0,0,0,0,0,0,0,0), /* 49  - 1 */
  SEG14(1,1,0,1,1,0,1,0,0,0,1,0,0,0), /* 50  - 2 */
  SEG14(1,1,1,1,0,0,0,0,0,0,1,0,0,0), /* 51  - 3 */
  SEG14(0,1,1,0,0,1,1,0,0,0,1,0,0,0), /* 52  - 4 */
  SEG14(1,0,0,1,0,1,1,0,0,0,0,1,0,0), /* 53  - 5 */
  SEG14(1,0,1,1,1,1,1,0,0,0,1,0,0,0), /* 54  - 6 */
  SEG14(1,1,1,0,0,0,0,0,0,0,0,0,0,0), /* 55  - 7 */
  SEG14(1,1,1,1,1,1,1,0,0,0,1,0,0,0), /* 56  - 8 */
  SEG14(1,1,1,1,0,1,1,0,0,0,1,0,0,0), /* 57  - 9 */
  SEG14(0,0,0,0,0,0,0,0,1,0,0,0,1,0), /* 58  - | */
  SEG14(0,0,0,0,0,0,0,0,1,0,0,0,0,1), /* 59  - 13*/
  SEG14(0,0,0,0,0,0,0,0,0,1,0,1,0,0), /* 60  - < */
  SEG14(0,0,0,1,0,0,1,0,0,0,1,0,0,0), /* 61  - = */
  SEG14(0,0,0,0,0,0,0,1,0,0,0,0,0,1), /* 62  - > */
  SEG14(1,1,0,0,0,0,0,0,1,0,0,0,1,0), /* 63  - ? */
  SEG14(1,1,0,1,1,1,0,0,1,0,1,0,0,0), /* 64  - @ */
  SEG14(1,1,1,0,1,1,1,0,0,0,1,0,0,0), /* 65  - A */
  SEG14(1,1,1,1,0,0,0,0,1,0,1,0,1,0), /* 66  - B */
  SEG14(1,0,0,1,1,1,0,0,0,0,0,0,0,0), /* 67  - C */
  SEG14(1,1,1,1,0,0,0,0,1,0,0,0,1,0), /* 68  - D */
  SEG14(1,0,0,1,1,1,1,0,0,0,1,0,0,0), /* 69  - E */
  SEG14(1,0,0,0,1,1,1,0,0,0,0,0,0,0), /* 70  - F */
  SEG14(1,0,1,1,1,1,0,0,0,0,1,0,0,0), /* 71  - G */
  SEG14(0,1,1,0,1,1,1,0,0,0,1,0,0,0), /* 72  - H */
  SEG14(0,0,0,0,0,0,0,0,1,0,0,0,1,0), /* 73  - I */
  SEG14(0,1,1,1,1,0,0,0,0,0,0,0,0,0), /* 74  - J */
  SEG14(0,0,0,0,1,1,1,0,0,1,0,1,0,0), /* 75  - K */
  SEG14(0,0,0,1,1,1,0,0,0,0,0,0,0,0), /* 76  - L */
  SEG14(0,1,1,0,1,1,0,1,0,1,0,0,0,0), /* 77  - M */
  SEG14(0,1,1,0,1,1,0,1,0,0,0,1,0,0), /* 78  - N */
  SEG14(1,1,1,1,1,1,0,0,0,0,0,0,0,0), /* 79  - O */
  SEG14(1,1,0,0,1,1,1,0,0,0,1,0,0,0), /* 80  - P */
  SEG14(1,1,1,1,1,1,0,0,0,0,0,1,0,0), /* 81  - Q */
  SEG14(1,1,0,0,1,1,1,0,0,0,1,1,0,0), /* 82  - R */
  SEG14(1,0,1,1,0,1,1,0,0,0,1,0,0,0), /* 83  - S */
  SEG14(1,0,0,0,0,0,0,0,1,0,0,0,1,0), /* 84  - T */
  SEG14(0,1,1,1,1,1,0,0,0,0,0,0,0,0), /* 85  - U */
  SEG14(0,0,0,0,1,1,0,0,0,1,0,0,0,1), /* 86  - V */
  SEG14(0,1,1,0,1,1,0,0,0,0,0,1,0,1), /* 87  - W */
  SEG14(0,0,0,0,0,0,0,1,0,1,0,1,0,1), /* 88  - X */
  SEG14(0,0,0,0,0,0,0,1,0,1,0,0,1,0), /* 89  - Y */
  SEG14(1,0,0,1,0,0,0,0,0,1,0,0,0,1), /* 90  - Z */
  SEG14(1,0,0,1,1,1,0,0,0,0,0,0,0,0), /* 91  - [ */
  SEG14(0,0,0,0,0,0,0,1,0,0,0,1,0,0), /* 92  - \ */
  SEG14(1,1,1,1,0,0,0,0,0,0,0,0,0,0), /* 93  - ] */
  SEG14(1,1,0,0,0,0,0,0,0,1,0,0,0,1), /* 94  - 14*/
  SEG14(0,0,0,1,0,0,0,0,0,0,0,0,0,0), /* 95  - _*/
  SEG14(0,0,0,0,0,0,0,1,0,0,0,0,0,0), /* 96  - ` */
  SEG14(1,1,1,0,1,1,1,0,0,0,1,0,0,0), /* 97  - A */
  SEG14(1,1,1,1,0,0,0,0,1,0,1,0,1,0), /* 98  - B */
  SEG14(1,0,0,1,1,1,0,0,0,0,0,0,0,0), /* 99  - C */
  SEG14(1,1,1,1,0,0,0,0,1,0,0,0,1,0), /* 100 - D */
  SEG14(1,0,0,1,1,1,1,0,0,0,1,0,0,0), /* 101 - E */
  SEG14(1,0,0,0,1,1,1,0,0,0,0,0,0,0), /* 102 - F */
  SEG14(1,0,1,1,1,1,0,0,0,0,1,0,0,0), /* 103 - G */
  SEG14(0,1,1,0,1,1,1,0,0,0,1,0,0,0), /* 104 - H */
  SEG14(0,0,0,0,0,0,0,0,1,0,0,0,1,0), /* 105 - I */
  SEG14(0,1,1,1,1,0,0,0,0,0,0,0,0,0), /* 106 - J */
  SEG14(0,0,0,0,1,1,1,0,0,1,0,1,0,0), /* 107 - K */
  SEG14(0,0,0,1,1,1,0,0,0,0,0,0,0,0), /* 108 - L */
  SEG14(0,1,1,0,1,1,0,1,0,1,0,0,0,0), /* 109 - M */
  SEG14(0,1,1,0,1,1,0,1,0,0,0,1,0,0), /* 110 - N */
  SEG14(1,1,1,1,1,1,0,0,0,0,0,0,0,0), /* 111 - O */
  SEG14(1,1,0,0,1,1,1,0,0,0,1,0,0,0), /* 112 - P */
  SEG14(1,1,1,1,1,1,0,0,0,0,0,1,0,0), /* 113 - Q */
  SEG14(1,1,0,0,1,1,1,0,0,0,1,1,0,0), /* 114 - R */
  SEG14(1,0,1,1,0,1,1,0,0,0,1,0,0,0), /* 115 - S */
  SEG14(1,0,0,0,0,0,0,0,1,0,0,0,1,0), /* 116 - T */
  SEG14(0,1,1,1,1,1,0,0,0,0,0,0,0,0), /* 117 - U */
  SEG14(0,0,0,0,1,1,0,0,0,1,0,0,0,1), /* 118 - V */
  SEG14(0,1,1,0,1,1,0,0,0,0,0,1,0,1), /* 119 - W */
  SEG14(0,0,0,0,0,0,0,1,0,1,0,1,0,1), /* 120 - X */
  SEG14(0,0,0,0,0,0,0,1,0,1,0,0,1,0), /* 121 - Y */
  SEG14(1,0,0,1,0,0,0,0,0,1,0,0,0,1), /* 122 - Z */
  SEG14(0,0,0,0,0,0,1,0,0,1,0,1,0,0), /* 123 - 15*/
  SEG14(0,0,0,0,0,0,0,0,1,0,0,0,1,0), /* 124 - | */
  SEG14(0,0,0,0,0,0,0,1,0,0,1,0,0,1), /* 125 - 16*/
  SEG14(0,1,0,0,1,0,1,0,0,0,1,0,0,0), /* 126 - 17*/
  SEG14(1,1,1,1,1,1,0,0,0,0,0,0,0,0), /* 127 - O */
};

const uint16_t S7A_DigitMap[]=
{
  SEG7A(1,1,1,1,1,1,0), /* 0 - 0 */
  SEG7A(0,1,1,0,0,0,0), /* 1 - 1 */
  SEG7A(1,1,0,1,1,0,1), /* 2 - 2 */
  SEG7A(1,1,1,1,0,0,1), /* 3 - 3 */
  SEG7A(0,1,1,0,0,1,1), /* 4 - 4 */
  SEG7A(1,0,1,1,0,1,1), /* 5 - 5 */
  SEG7A(1,0,1,1,1,1,1), /* 6 - 6 */
  SEG7A(1,1,1,0,0,0,0), /* 7 - 7 */
  SEG7A(1,1,1,1,1,1,1), /* 8 - 8 */
  SEG7A(1,1,1,1,0,1,1), /* 9 - 9 */
  SEG7A(1,1,1,0,1,1,1), /* 10 - A */
  SEG7A(0,0,1,1,1,1,1), /* 11 - b */
  SEG7A(1,0,0,1,1,1,0), /* 12 - C */
  SEG7A(0,1,1,1,1,0,1), /* 13 - d */
  SEG7A(1,0,0,1,1,1,1), /* 14 - E */
  SEG7A(1,0,0,0,1,1,1), /* 15 - F */
};

const uint16_t S7B_DigitMap[]=
{
  SEG7B(1,1,1,1,1,1,0), /* 0 - 0 */
  SEG7B(0,1,1,0,0,0,0), /* 1 - 1 */
  SEG7B(1,1,0,1,1,0,1), /* 2 - 2 */
  SEG7B(1,1,1,1,0,0,1), /* 3 - 3 */
  SEG7B(0,1,1,0,0,1,1), /* 4 - 4 */
  SEG7B(1,0,1,1,0,1,1), /* 5 - 5 */
  SEG7B(1,0,1,1,1,1,1), /* 6 - 6 */
  SEG7B(1,1,1,0,0,0,0), /* 7 - 7 */
  SEG7B(1,1,1,1,1,1,1), /* 8 - 8 */
  SEG7B(1,1,1,1,0,1,1), /* 9 - 9 */
  SEG7B(1,1,1,0,1,1,1), /* 10 - A */
  SEG7B(0,0,1,1,1,1,1), /* 11 - b */
  SEG7B(1,0,0,1,1,1,0), /* 12 - C */
  SEG7B(0,1,1,1,1,0,1), /* 13 - d */
  SEG7B(1,0,0,1,1,1,1), /* 14 - E */
  SEG7B(1,0,0,0,1,1,1), /* 15 - F */
};

static void LCD_GPIOConfig(void);
static void LCD_GLASS_WriteChar(uint8_t* ch, Point_Typedef point, Semicol_Typedef semicom, uint8_t position);

/**
  * @brief Configures the LCD GLASS relative GPIO port IOs and LCD peripheral.
  * @param : None
  * @retval: None
  */
void LCD_GLASS_Init(void)
{
  LCD_InitTypeDef LCD_InitStructure;

  LCD_GPIOConfig(); /*!< Configure the LCD Glass GPIO pins */

  /*!< Configure the LCD interface -------------------------------------------*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_LCD, ENABLE); /*!< Enable LCD APB1 Clock */

  LCD_InitStructure.LCD_Prescaler = LCD_Prescaler_2;
  LCD_InitStructure.LCD_Divider = LCD_Divider_20;
  LCD_InitStructure.LCD_Duty = LCD_Duty_1_4;
  LCD_InitStructure.LCD_Bias = LCD_Bias_1_3;
  LCD_InitStructure.LCD_VoltageSource = LCD_VoltageSource_Internal;
  LCD_Init(&LCD_InitStructure);
  LCD_MuxSegmentCmd(DISABLE);

  /*!< High drive enable */
  LCD_HighDriveCmd(ENABLE);

  /*!< Configure the Pulse On Duration */
  LCD_PulseOnDurationConfig(LCD_PulseOnDuration_3);

  /*!< Configure the LCD Contrast */
  LCD_ContrastConfig(LCD_Contrast_Level_0);

  /*!< Wait Until the LCD FCR register is synchronized */
  LCD_WaitForSynchro();

  /*!< Enable LCD peripheral */
  LCD_Cmd(ENABLE);

  /*!< Wait Until the LCD is enabled */
  while(LCD_GetFlagStatus(LCD_FLAG_ENS) == RESET)
  {
  }
  /*!< Wait Until the LCD Booster is ready */
  while(LCD_GetFlagStatus(LCD_FLAG_RDY) == RESET)
  {
  }
}

/**
  * @brief This function writes a char in the LCD RAM.
  * @param car: The character to dispaly.
  * @param point: A point to add in front of char.
  *               This parameter  can be one of the following values:
  * @arg 0 : No point to add in back of char.
  * @arg 1 : Add a point in back of char.
  * @param Semicol_Typedef: Flag indicating if a semicolone has to be add in back of
  *                    displayed character
  *                    This parameter  can be one of the following values:
  * @arg 0 : No point to add in back of char.
  * @arg 1 : Add a point in back of char.
  * @param position: Position in the LCD of the caracter to write.
  *                  This parameter can be any value in range [0:7].
  * @retval: None
  */
void LCD_GLASS_DisplayChar(uint8_t* ch, Point_Typedef point, Semicol_Typedef semicol, uint8_t position)
{
  /*!< Wait Until the last LCD RAM update finish */
  while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET)
  {
  }
  /*!< LCD Write Char */
  LCD_GLASS_WriteChar(ch, point, semicol, position);

  /*!< Requesy LCD RAM update */
  LCD_UpdateDisplayRequest();
}

/**
  * @brief This function writes a char in the LCD RAM.
  * @param ptr: Pointer to string to display on the LCD Glass.
  * @retval: None
  */
void LCD_GLASS_DisplayString(uint8_t* ptr)
{
  uint32_t i = 0x00;

  /*!< Wait Until the last LCD RAM update finish */
  while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET)
  {
  }

  /*!< Send the string character by character on lCD */
  while ((*ptr != 0) & (i < 7))
  {
    /*!< Display one character on LCD */
    LCD_GLASS_WriteChar(ptr, POINT_OFF, SEMICOL_OFF, i);
    /*!< Point on the next character */
    ptr++;
    /*!< Increment the character counter */
    i++;
  }
  /*!< Requesy LCD RAM update */
  LCD_UpdateDisplayRequest();
}

/**
  * @brief This function writes a char in the LCD RAM.
  * @param car: The character to dispaly.
  * @param point: A point to add in front of char.
  *               This parameter  can be one of the following values:
  * @arg 0 : No point to add in front of char.
  * @arg 1 : Add a point in front of char.
  * @param apostrophe: Flag indicating if a apostrophe has to be add in front of
  *                    displayed character
  *                    This parameter  can be one of the following values:
  * @arg 0 : No point to add in back of char.
  * @arg 1 : Add a point in back of char.
  * @param position: Position in the LCD of the caracter to write.
  *                  This parameter can be any value in range [0:7].
  * @retval: None
  */
static void LCD_GLASS_WriteChar(uint8_t* ch, Point_Typedef point, Semicol_Typedef semicom, uint8_t position)
{
  uint32_t tmp = 0x00;
  uint8_t offset = *ch & 0x7F;

  switch (position)
  {

    case 0:
      /*!< Clear the corresponding segments (SEG36, SEG37, SEG38, SEG39) */
      LCD->RAM[LCD_RAMRegister_1] &= (uint32_t)(0xFFFFFF0F);
      LCD->RAM[LCD_RAMRegister_3] &= (uint32_t)(0xFFFFFF0F);
      LCD->RAM[LCD_RAMRegister_5] &= (uint32_t)(0xFFFFFF0F);
      LCD->RAM[LCD_RAMRegister_7] &= (uint32_t)(0xFFFFFF9F);

      /*!< Write the corresponding segments (SEG36, SEG37, SEG38, SEG39) */
      tmp = S14_FontMap[offset];
      LCD->RAM[LCD_RAMRegister_1] |= (tmp << 4) & 0x00F0;
      LCD->RAM[LCD_RAMRegister_3] |= (tmp << 0) & 0x00F0;
      LCD->RAM[LCD_RAMRegister_5] |= (tmp >> 4) & 0x00F0;
      LCD->RAM[LCD_RAMRegister_7] |= (tmp >> 8) & 0x0060;
      *SEG_S14_1DP  = point;
      *SEG_S14_1COL = semicom;
      break;

    case 1:
      /*!< Clear the corresponding segments (SEG35, SEG34, SEG33, SEG32) */
      LCD->RAM[LCD_RAMRegister_1] &= (uint32_t)(0xFFFFFFF8);
      LCD->RAM[LCD_RAMRegister_3] &= (uint32_t)(0xFFFFFFF0);
      LCD->RAM[LCD_RAMRegister_5] &= (uint32_t)(0xFFFFFFF0);
      LCD->RAM[LCD_RAMRegister_7] &= (uint32_t)(0xFFFFFFF8);

      /*!< Write the corresponding segments (SEG35, SEG34, SEG33, SEG32) */
      tmp = S14_FontMap[offset];
      LCD->RAM[LCD_RAMRegister_1] |= (tmp << 0) & 0x0007;
      LCD->RAM[LCD_RAMRegister_3] |= (tmp >> 4) & 0x000F;
      LCD->RAM[LCD_RAMRegister_5] |= (tmp >> 8) & 0x000F;
      LCD->RAM[LCD_RAMRegister_7] |= (tmp >>12) & 0x0007;
      *SEG_S14_2DP  = point;
      *SEG_S14_2COL = semicom;
      break;

    case 2:
      /*!< Clear the corresponding segments (SEG28, SEG29, SEG30, SEG31) */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)(0x8FFFFFFF);
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)(0x0FFFFFFF);
      LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)(0x0FFFFFFF);
      LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)(0x8FFFFFFF);

      /*!< Write the corresponding segments (SEG28, SEG29, SEG30, SEG31) */
      tmp = S14_FontMap[offset];
      LCD->RAM[LCD_RAMRegister_0] |= (tmp <<28) & 0x70000000;
      LCD->RAM[LCD_RAMRegister_2] |= (tmp <<24) & 0xF0000000;
      LCD->RAM[LCD_RAMRegister_4] |= (tmp <<20) & 0xF0000000;
      LCD->RAM[LCD_RAMRegister_6] |= (tmp <<16) & 0x70000000;
      *SEG_S14_3DP  = point;
      *SEG_S14_3COL = semicom;
      break;

    case 3:
      /*!< Clear the corresponding segments (SEG24, SEG25, SEG26, SEG27) */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)(0xF8FFFFFF);
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)(0xF0FFFFFF);
      LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)(0xF0FFFFFF);
      LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)(0xF8FFFFFF);

      /*!< Write the corresponding segments (SEG24, SEG25, SEG26, SEG27) */
      tmp = S14_FontMap[offset];
      LCD->RAM[LCD_RAMRegister_0] |= (tmp <<24) & 0x07000000;
      LCD->RAM[LCD_RAMRegister_2] |= (tmp <<20) & 0x0F000000;
      LCD->RAM[LCD_RAMRegister_4] |= (tmp <<16) & 0x0F000000;
      LCD->RAM[LCD_RAMRegister_6] |= (tmp <<12) & 0x07000000;
      *SEG_S14_4DP  = point;
      *SEG_S14_4COL = semicom;
      break;

    case 4:
      /*!< Clear the corresponding segments (SEG20, SEG21, SEG22, SEG23) */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)(0xFF8FFFFF);
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)(0xFF0FFFFF);
      LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)(0xFF0FFFFF);
      LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)(0xFF8FFFFF);

      /*!< Write the corresponding segments (SEG20, SEG21, SEG22, SEG23) */
      tmp = S14_FontMap[offset];
      LCD->RAM[LCD_RAMRegister_0] |= (tmp <<20) & 0x00700000;
      LCD->RAM[LCD_RAMRegister_2] |= (tmp <<16) & 0x00F00000;
      LCD->RAM[LCD_RAMRegister_4] |= (tmp <<12) & 0x00F00000;
      LCD->RAM[LCD_RAMRegister_6] |= (tmp << 8) & 0x00700000;
      *SEG_S14_5DP  = point;
      *SEG_S14_5COL = semicom;
      break;

    case 5:
      /*!< Clear the corresponding segments (SEG16, SEG17, SEG18, SEG19) */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)(0xFFF8FFFF);
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)(0xFFF0FFFF);
      LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)(0xFFF0FFFF);
      LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)(0xFFF8FFFF);

      /*!< Write the corresponding segments (SEG16, SEG17, SEG18, SEG19) */
      tmp = S14_FontMap[offset];
      LCD->RAM[LCD_RAMRegister_0] |= (tmp <<16) & 0x00070000;
      LCD->RAM[LCD_RAMRegister_2] |= (tmp <<12) & 0x000F0000;
      LCD->RAM[LCD_RAMRegister_4] |= (tmp << 8) & 0x000F0000;
      LCD->RAM[LCD_RAMRegister_6] |= (tmp << 4) & 0x00070000;
      *SEG_S14_6DP  = point;
      break;

    case 6:
      /*!< Clear the corresponding segments (SEG12, SEG13, SEG14, SEG15) */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)(0xFFFF8FFF);
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)(0xFFFF0FFF);
      LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)(0xFFFF0FFF);
      LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)(0xFFFF8FFF);

      /*!< Write the corresponding segments (SEG12, SEG13, SEG14, SEG15) */
      tmp = S14_FontMap[offset];
      LCD->RAM[LCD_RAMRegister_0] |= (tmp <<12) & 0x00007000;
      LCD->RAM[LCD_RAMRegister_2] |= (tmp << 8) & 0x0000F000;
      LCD->RAM[LCD_RAMRegister_4] |= (tmp << 4) & 0x0000F000;
      LCD->RAM[LCD_RAMRegister_6] |= (tmp << 0) & 0x00007000;
      break;
  }
}

void LCD_GLASS_DisplaySpecial(uint32_t spec)
{
  /*!< Wait Until the last LCD RAM update finish */
  while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET)
  {
  }

  *SEG_PL   		= (SPEC_SEG_PLYS & spec)?1:0;
  *SEG_MN   		= (SPEC_SEG_MINUS & spec)?1:0;
  *SEG_AR   		= (SPEC_SEG_ARROW_RIGHT & spec)?1:0;
	*SEG_AL   		= (SPEC_SEG_ARROW_LEFT & spec)?1:0;
	*SEG_AU   		= (SPEC_SEG_ARROW_UP & spec)?1:0;
	*SEG_AD   		= (SPEC_SEG_ARROW_DOWN & spec)?1:0;
	*SEG_BATT 		= (SPEC_SEG_BATT & spec)?1:0;
	*SEG_BATT_0 	= (SPEC_SEG_BATT_0 & spec)?1:0;
	*SEG_BATT_1 	= (SPEC_SEG_BATT_1 & spec)?1:0;
	*SEG_BATT_2 	= (SPEC_SEG_BATT_2 & spec)?1:0;
	*SEG_BARR_PR 	= (SPEC_SEG_BARR_R & spec)?1:0;
	*SEG_BARR_PL 	= (SPEC_SEG_BARR_L & spec)?1:0;
	*SEG_BARR_P0 	= (SPEC_SEG_BARR_0 & spec)?1:0;
	*SEG_BARR_P1 	= (SPEC_SEG_BARR_1 & spec)?1:0;
	*SEG_BARR_P2 	= (SPEC_SEG_BARR_2 & spec)?1:0;
	*SEG_BARR_P3 	= (SPEC_SEG_BARR_3 & spec)?1:0;
	*SEG_BARR_P4 	= (SPEC_SEG_BARR_4 & spec)?1:0;
	*SEG_BARR_P5 	= (SPEC_SEG_BARR_5 & spec)?1:0;
	*SEG_BARR_P6 	= (SPEC_SEG_BARR_6 & spec)?1:0;
	*SEG_BARR_P7 	= (SPEC_SEG_BARR_7 & spec)?1:0;
	*SEG_BARR_P8 	= (SPEC_SEG_BARR_8 & spec)?1:0;
	*SEG_BARR_P9 	= (SPEC_SEG_BARR_9 & spec)?1:0;
	*SEG_MICRO 		= (SPEC_SEG_MICRO & spec)?1:0;
	*SEG_MIL 			= (SPEC_SEG_MIL & spec)?1:0;
	*SEG_H 				= (SPEC_SEG_H & spec)?1:0;
	*SEG_F 				= (SPEC_SEG_F & spec)?1:0;
	*SEG_OLIMEX   = (SPEC_SEG_OLIMEX & spec)?1:0;

  /*!< Requesy LCD RAM update */
  LCD_UpdateDisplayRequest();

}

void LCD_GLASS_DisplayDigit(uint8_t* digit, Point_Typedef point, Semicol_Typedef semicol,uint8_t position)
{
  uint32_t tmp = 0x00;
  uint8_t offset = *digit & 0xF;

  /*!< Wait Until the last LCD RAM update finish */
  while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET)
  {
  }

  switch(position)
  {
  case 0:
      /*!< Clear the corresponding segments (SEG10, SEG11) */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)(0xFFFFF3FF);
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)(0xFFFFF3FF);
      LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)(0xFFFFF3FF);
      LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)(0xFFFFF7FF);

      /*!< Write the corresponding segments (SEG10, SEG11) */
      tmp = S7A_DigitMap[offset];
      LCD->RAM[LCD_RAMRegister_0] |= (tmp <<10) & 0x0C00;
      LCD->RAM[LCD_RAMRegister_2] |= (tmp << 6) & 0x0C00;
      LCD->RAM[LCD_RAMRegister_4] |= (tmp << 2) & 0x0C00;
      LCD->RAM[LCD_RAMRegister_6] |= (tmp >> 2) & 0x0800;
      *SEG_S7_8P  = point;
    break;
  case 1:
      /*!< Clear the corresponding segments (SEG8, SEG9) */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)(0xFFFFFCFF);
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)(0xFFFFFCFF);
      LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)(0xFFFFFCFF);
      LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)(0xFFFFFDFF);

      /*!< Write the corresponding segments (SEG8, SEG9) */
      tmp = S7A_DigitMap[offset];
      LCD->RAM[LCD_RAMRegister_0] |= (tmp << 8) & 0x0300;
      LCD->RAM[LCD_RAMRegister_2] |= (tmp << 4) & 0x0300;
      LCD->RAM[LCD_RAMRegister_4] |= (tmp << 0) & 0x0300;
      LCD->RAM[LCD_RAMRegister_6] |= (tmp >> 4) & 0x0200;
      *SEG_S7_9P  = point;
      *SEG_S7_9COL  = semicol;
    break;
  case 2:
      /*!< Clear the corresponding segments (SEG6, SEG7) */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)(0xFFFFFF3F);
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)(0xFFFFFF3F);
      LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)(0xFFFFFF3F);
      LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)(0xFFFFFFBF);

      /*!< Write the corresponding segments (SEG6, SEG7) */
      tmp = S7B_DigitMap[offset];
      LCD->RAM[LCD_RAMRegister_0] |= (tmp << 6) & 0x00C0;
      LCD->RAM[LCD_RAMRegister_2] |= (tmp << 2) & 0x00C0;
      LCD->RAM[LCD_RAMRegister_4] |= (tmp >> 2) & 0x00C0;
      LCD->RAM[LCD_RAMRegister_6] |= (tmp >> 6) & 0x0040;
      *SEG_S7_10P  = point;
    break;
  case 3:
      /*!< Clear the corresponding segments (SEG4, SEG5) */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)(0xFFFFFFCF);
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)(0xFFFFFFCF);
      LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)(0xFFFFFFCF);
      LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)(0xFFFFFFEF);

      /*!< Write the corresponding segments (SEG4, SEG5) */
      tmp = S7B_DigitMap[offset];
      LCD->RAM[LCD_RAMRegister_0] |= (tmp << 4) & 0x0030;
      LCD->RAM[LCD_RAMRegister_2] |= (tmp << 0) & 0x0030;
      LCD->RAM[LCD_RAMRegister_4] |= (tmp >> 4) & 0x0030;
      LCD->RAM[LCD_RAMRegister_6] |= (tmp >> 8) & 0x0010;
    break;
  }

  /*!< Requesy LCD RAM update */
  LCD_UpdateDisplayRequest();
}

/**
  * @brief Configures the LCD Segments and Coms GPIOs.
  * @param None
  * @retval None
  */
static void LCD_GPIOConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /*!< Enable GPIOA, GPIOB, GPIOC, GPIOD and GPIOE AHB Clocks */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC
                        | RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE, ENABLE);

  /*!< Connect PA.08 to LCD COM0 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_LCD);

  /*!< Connect PA.09 to LCD COM1 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_LCD);

  /*!< Connect PA.10 to LCD COM2 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_LCD);

  /*!< Connect PB.09 to LCD COM3 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_LCD);

  /*!< Connect PA.01 to LCD SEG0 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_LCD);

  /*!< Connect PA.02 to LCD SEG1 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_LCD);

  /*!< Connect PA.03 to LCD SEG2 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_LCD);

  /*!< Connect PA.06 to LCD SEG3 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_LCD);

  /*!< Connect PA.07 to LCD SEG4 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_LCD);

  /*!< Connect PB.00 to LCD SEG5 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_LCD);

  /*!< Connect PB.01 to LCD SEG6 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_LCD);

#if !defined(DEBUG)
  /*!< Connect PB.03 to LCD SEG7 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_LCD);
#endif

  /*!< Connect PB.04 to LCD SEG8 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_LCD);

  /*!< Connect PB.05 to LCD SEG9 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_LCD);

  /*!< Connect PB.10 to LCD SEG10 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_LCD);

  /*!< Connect PB.11 to LCD SEG11 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_LCD);

  /*!< Connect PB.12 to LCD SEG12 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_LCD);

  /*!< Connect PB.13 to LCD SEG13 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_LCD);

  /*!< Connect PB.14 to LCD SEG14 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_LCD);

  /*!< Connect PB.15 to LCD SEG15 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_LCD);

  /*!< Connect PB.08 to LCD SEG16 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_LCD);

  /*!< Connect PA.15 to LCD SEG17 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_LCD);

  /*!< Connect PC.00 to LCD SEG18 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource0, GPIO_AF_LCD);

  /*!< Connect PC.01 to LCD SEG19 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_LCD);

  /*!< Connect PC.02 to LCD SEG20 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource2, GPIO_AF_LCD);

  /*!< Connect PC.03 to LCD SEG21 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_LCD);

  /*!< Connect PC.04 to LCD SEG22 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_LCD);

  /*!< Connect PC.05 to LCD SEG23 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_LCD);

  /*!< Connect PC.06 to LCD SEG24 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_LCD);

  /*!< Connect PC.07 to LCD SEG25 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_LCD);

  /*!< Connect PC.08 to LCD SEG26 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_LCD);

  /*!< Connect PC.09 to LCD SEG27 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_LCD);

  /*!< Connect PD.08 to LCD SEG28 */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_LCD);

  /*!< Connect PD.09 to LCD SEG29 */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_LCD);

  /*!< Connect PD.10 to LCD SEG30 */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_LCD);

  /*!< Connect PD.11 to LCD SEG31 */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_LCD);

  /*!< Connect PD.12 to LCD SEG32 */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_LCD);

  /*!< Connect PD.13 to LCD SEG33 */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_LCD);

  /*!< Connect PD.14 to LCD SEG34 */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_LCD);

  /*!< Connect PD.15 to LCD SEG35 */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_LCD);

  /*!< Connect PE.00 to LCD SEG36 */
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_LCD);

  /*!< Connect PE.01 to LCD SEG37 */
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_LCD);

  /*!< Connect PE.02 to LCD SEG38 */
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource2, GPIO_AF_LCD);

  /*!< Connect PE.03 to LCD SEG39 */
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource3, GPIO_AF_LCD);

  /*!< Connect PC.12 to LCD SEG42 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_LCD);

  /*!< Connect PD.02 to LCD SEG43 */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_LCD);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | \
                                GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

#if !defined(DEBUG)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | \
                                GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | \
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | \
                                GPIO_Pin_15;
#else
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | \
                                GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | \
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | \
                                GPIO_Pin_15;
#endif
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | \
                                GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | \
                                GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_12;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | \
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | \
                                GPIO_Pin_15;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}
