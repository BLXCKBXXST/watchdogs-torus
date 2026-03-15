/* spin.c — Watch Dogs style ASCII torus animation
 * Compile: gcc -O2 -o spin spin.c
 * Run:     ./spin
 * Stop:    Ctrl+C
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define GREEN   "\033[92m"
#define MAGENTA "\033[95m"
#define CYAN    "\033[96m"
#define RED     "\033[91m"
#define RESET   "\033[0m"
#define CLEAR   "\033[H\033[J"

const char *frames[] = {
    /* A */
    "      ,d$$$$$$SSIi.\n"
    "    ,$$$SSSSS$$SSIi:.\n"
    "   j$$$SSSSS$$$SIIi..\n"
    "   S$$$$$SSS$$SSIIi-\n"
    "  j?'``?S$SI7*'\"#?IL:.\n"
    "  ?:     $$S?    `7i'\n"
    "  iL    j$?$L     I7\n"
    "  $$$bxd$'  `$b,_d$:\n"
    "  ?SSIIS?    S$?I?$I\n"
    "   ``?IS$L,,d$Sli:'#'\n"
    "    ?$$$SSSS$Ii'\n"
    "     j:?i:i?-*:\n"
    "       #' `^\n",

    /* B */
    "      ,d$$$$$$SSIi:._\n"
    "    ,$$$SSSSS$$$$SIi::\n"
    "   d$$$SSSSSSS$$SSiII:.\n"
    "   j$$$SSSSS$$$$SSSi:iII:\n"
    "  j?'``?SSI?*'\"#?IL:iIISIi:\n"
    "  ?    :$I?      ?$:iIS$I:-\n"
    "  jL _,$?$L      j?b:iISi:\n"
    "  ?$d$'   `$b,_,d$$$:iII?\n"
    "  i$$:     S$?I?$$$Sxu.?'\n"
    "   `?L,,d$SIi:'#?S?$' '\n"
    "    I$$$S$SIi'   `_  .'\n"
    "     :i?i:i?-\n"
    "       u' `^\n",

    /* C */
    "      _,d$$$$$$$SSIi:,_\n"
    "    ,d$$$$$$$$$$$SSISi:.\n"
    "   ,$$$$SSSS$$$$$$$SSIIi:.\n"
    "   j$$$SSSS$$$$$$$SIISSy$SIi:\n"
    "  'I?'``$$SiISiIS$$$$SIi:\n"
    "  .\"$$    j$b:iIS$$$SIi:iIi:\n"
    "  ? i$L,_.d$d$:ij$SSIISi:-\n"
    "  i,j$S?$$$$$Sxu,*?IISi::-\n"
    "  ,d$$Sii?*?$$?$' `~`_,:'\n"
    "   i$$$S$Sk.^        .'\n"
    "    -?i:-i?-\n"
    "       u' `^\n",

    /* D */
    "      ,,uS$$$$$$$$SSi:,_\n"
    "    ,d$$$$$$$$$$$$$$SSIi::.\n"
    "   j$$S$$$$S7IIS$$$$$$$$SSIi:\n"
    "  ,$$$7:iIIS$$$$$$SIS$$$$SSIIi:.\n"
    "  j ?k:iIISIS$S$7IS$$$$SSIIi:.\n"
    "  ? j$b:iIj$$S7IIS$$$$7IIi::-\n"
    "  ju$S7 :j$$SiIS$$$$SIIIi::\n"
    "  bi$$Sp..._?xu,`?S$$SII::-\n"
    "  d$$$$Si?*?S$?#*'`^`4ISIii:::\n"
    "  *?$$k.              -??I::\n"
    "  ^:?i:'                 `_\n",

    /* E */
    "      ,,oS$$$$$$$$SSi:,_\n"
    "    ,d$$$$$$$$$$$$$$SSSIi::.\n"
    "   j$$$$$$$$$$$$$$$$S$$SSIi:\n"
    "   d$$$$$$SS$$$$$$$$$S$$SSIi:.\n"
    "   j$$7:IS$$$$$$$$$$$$$$$$Si::\n"
    "  j7:j$$SiIS$$$$$$$$$$$$$$$$Si:-\n"
    "  `  -?$Siia$$$$$SIS$$$$SSIi:\n"
    "  $k,_`$IS$$SiIS$$$$SIIIi::-\n"
    "  ?'  `*$$S?::::iISSii:::\n"
    "   k,:'  i?  ::::::::\n"
    "    ``     ''\n",

    /* F */
    "      ,d$$$$$SSi:,\n"
    "    ,d$$$$$$$$$SSIi:.\n"
    "   d$$$$$SS$$$$$$$$$SIk.\n"
    "  iS$$$$$SS$$$$$$$$$$$SIi\n"
    "  IIS$$$$$$$S$$$$$$$$$S1:\n"
    "  SS$$$$$7S$$$$$SS$$$SIi:\n"
    "  ?$$$$$7,jIS$$$$$SS$IS::-\n"
    "  ji$$S7,j$SI7S$$$SIi:-\n"
    "  :iS$SS1$$$SL`?S$SIi?_,o$$\n"
    "   ?ISi?`*#?Sb,`*#*'  -'``\n"
    "    *?$':::::`?S$i'  -'\n"
    "     ?  u``  ` -::\n"
    "          `?'\n",

    /* G */
    "      ,d$$$$$$SSIi:,\n"
    "    ,d$$$$$$$$$$$$$SIi:.\n"
    "   d$S$$$$$$$$$$$$$$SSik\n"
    "  jIS$$$$$$$$$$$$$$$$$SIiiL\n"
    "  IIS$$$$$$$$$$$$$$$S1:?$\n"
    "  iS$$$$$7S$$$$$$$$SIii:?k\n"
    "  :iS$$$$7,jIS$$$$$$SIS::-?\n"
    "  :iIS$$$S7,j$SI7S$$$SIi?   L\n"
    "  :iS$$SSi$$$SL`?S$SIi?_,o$$\n"
    "   ?ISi?`*#?$b,`*#*'  -'``\n"
    "    *?$'     `?S$i'  '\n"
    "     ?::::    ::\n"
    "          `?'\n",

    /* H */
    "      _,d$$$$$$SSIi:,\n"
    "    ,d$$$$$$$$$$$SSSIi:.\n"
    "   d$S$$$$$$$$$$$$$$SSIk\n"
    "  dIS$$$$$$$$$$$$$$$SSSiiL\n"
    "  iISS$$$$$$$$$$$$$$$SSi?$k\n"
    "  ;iSS$$$$SSSS$$$$$$$$$SSIIi:S?\n"
    "  :iIS$$$SSSIS$$$$SSSS$SIii?.j.\n"
    "  :iIS$$SIiS$$I7S$$SIIIi?'jI$:\n"
    "  :iIS$$SiiiS$$$Li`?S$$SIi?,od$$\n"
    "   :iSi:?S$$SI?'*#*'`^`*??$k\n"
    "    '*:?S$Si         :::iI$$\n"
    "        '*g,                \"_.\n",

    /* I */
    "      _,d$$$$$$$$Sik,\n"
    "    ,i$$$$$$$$$$$$SSSIk:\n"
    "   diS$$$$$$$$$$$$SIiS$$SIk\n"
    "  jIS$$$$$$$$$$$SIiS$$$SIk\n"
    "  ?SS$$$$$$$$SSIIi:d$'   :?k\n"
    "  :SS$$$$$$$SSIIi: j$$    j$?\n"
    "   ?IS$$$$SSIi:::  j$$    j$?\n"
    "   ?ISLS$$$Lii::::,?$$$upx?$'\n"
    "   ?ISLlb,_`*#*\"?S$$**`,d$S\n"
    "    :`?7$?'    `?$k.::iIS$k.\n"
    "      *e,'       `?.::iIS$$i\n"
    "                   \"._$^ ^,'\n",

    /* J */
    "      _,d$$$$$$$$b.\n"
    "    ,d$$$$$$$$$$$$$SSb.\n"
    "   d$$$S$IS$$$$$$$$$$SIb\n"
    "  jIS$$$SII$$$$$$$$$$SS$k\n"
    "  S1$$IIiid$$S?il$$$$$S7*k.\n"
    "  :iS$IIi:j7'      ?$S?    i\n"
    "  :iSSI:::$$       j$$L    ?\n"
    "  :?i:.,d$$k,_.,d$$7`?p,$\n"
    "  ii:`*?$$$$S?I?$$S?    $$?\n"
    "  ?: .  ?S$$SII$$L,,J$\"'\n"
    "  `. `~   :IS$$$$$$Sk\n"
    "    -:i?i:?:.\n"
    "         u' `\n",

    NULL
};

static const char *colors[] = { GREEN, MAGENTA, CYAN, RED };

void print_glitch(const char *frame) {
    static const char glitch_chars[] = "#@%&!?/|<>^~*+=-:;";
    int glen = (int)strlen(glitch_chars);
    for (const char *p = frame; *p; p++) {
        if (*p != '\n' && *p != ' ' && (rand() % 100) < 8)
            putchar(glitch_chars[rand() % glen]);
        else
            putchar(*p);
    }
}

void print_shifted(const char *frame) {
    char buf[4096];
    strncpy(buf, frame, sizeof(buf) - 1);
    buf[sizeof(buf)-1] = '\0';
    char *line = strtok(buf, "\n");
    while (line) {
        if ((rand() % 100) < 15) {
            int shift = rand() % 5 + 1;
            for (int i = 0; i < shift; i++) putchar(' ');
            int len = (int)strlen(line);
            if (len > shift) fwrite(line, 1, len - shift, stdout);
        } else {
            fputs(line, stdout);
        }
        putchar('\n');
        line = strtok(NULL, "\n");
    }
}

int main(void) {
    srand((unsigned)time(NULL));
    int nframes = 0;
    while (frames[nframes]) nframes++;

    while (1) {
        for (int i = 0; i < nframes; i++) {
            if ((rand() % 100) < 20) {
                int gtype  = rand() % 2;
                int gtimes = rand() % 3 + 2;
                for (int g = 0; g < gtimes; g++) {
                    fputs(CLEAR, stdout);
                    fputs(colors[rand() % 4], stdout);
                    if (gtype == 0) print_glitch(frames[i]);
                    else            print_shifted(frames[i]);
                    fputs(RESET, stdout);
                    fflush(stdout);
                    usleep(30000);
                }
            }
            fputs(CLEAR, stdout);
            fputs(GREEN, stdout);
            fputs(frames[i], stdout);
            fputs(RESET, stdout);
            fflush(stdout);
            usleep(110000);
        }
    }
    return 0;
}
