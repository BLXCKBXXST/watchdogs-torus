/* spin_chaos.c — Watch Dogs torus: random teleport across screen
 * Compile: gcc -O2 -o spin_chaos spin_chaos.c
 * Run:     ./spin_chaos
 * Stop:    Ctrl+C
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h>
#include <signal.h>

#define GREEN   "\033[92m"
#define MAGENTA "\033[95m"
#define CYAN    "\033[96m"
#define RED     "\033[91m"
#define YELLOW  "\033[93m"
#define RESET   "\033[0m"
#define CLEAR   "\033[H\033[J"

#define TORUS_W  36
#define TORUS_H  15

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
    "       #' `^\n"
    "\n",

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
    "       u' `^\n"
    "\n",

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
    "       u' `^\n"
    "\n",

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
    "  ^:?i:'                 `_\n"
    "\n",

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
    "    ``     ''\n"
    "\n",

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
    "          `?'\n"
    "\n",

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
    "          `?'\n"
    "\n",

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
    "        '*g,                \"_.\n"
    "\n",

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
    "                   \"._$^ ^,'\n"
    "\n",

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
    "         u' `\n"
    "\n",

    NULL
};

static const char *colors[] = { GREEN, MAGENTA, CYAN, RED, YELLOW };
#define NCOLORS 5

static int term_rows = 24, term_cols = 80;

static void update_term_size(void) {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        term_rows = w.ws_row; term_cols = w.ws_col;
    }
}

static void on_resize(int sig) { (void)sig; update_term_size(); }

static void on_exit_sig(int sig) {
    (void)sig;
    printf(CLEAR "\033[?25h" RESET);
    fflush(stdout);
    exit(0);
}

static void erase_at(int row, int col) {
    char blank[TORUS_W + 1];
    memset(blank, ' ', TORUS_W); blank[TORUS_W] = 0;
    for (int r = 0; r < TORUS_H + 1; r++)
        printf("\033[%d;%dH%s", row + r, col, blank);
}

static void draw_at(const char *frame, int row, int col,
                    const char *color, int glitch) {
    static const char gc[] = "#@%&!?/|<>^~*+=-:;$S";
    int glen = (int)strlen(gc);
    int r = row;
    const char *p = frame;
    while (*p) {
        const char *nl = strchr(p, '\n');
        int len = nl ? (int)(nl-p) : (int)strlen(p);
        printf("\033[%d;%dH", r, col);
        fputs(color, stdout);
        for (int i = 0; i < len; i++) {
            if (glitch && p[i] != ' ' && (rand()%100) < 10)
                putchar(gc[rand()%glen]);
            else putchar(p[i]);
        }
        fputs(RESET, stdout);
        r++;
        p = nl ? nl+1 : p+len;
        if (!nl) break;
    }
    fflush(stdout);
}

static void teleport_effect(const char *frame,
                             int old_r, int old_c,
                             int new_r, int new_c) {
    for (int g = 0; g < 4; g++) {
        draw_at(frame, old_r, old_c, colors[rand()%NCOLORS], 1);
        usleep(35000);
        erase_at(old_r, old_c);
        usleep(20000);
    }
    int steps = rand()%3+2;
    for (int s = 0; s < steps; s++) {
        int mid_r = rand()%(term_rows-TORUS_H)+1;
        int mid_c = rand()%(term_cols-TORUS_W)+1;
        draw_at(frame, mid_r, mid_c, colors[rand()%NCOLORS], 1);
        usleep(40000);
        erase_at(mid_r, mid_c);
    }
    for (int g = 0; g < 3; g++) {
        draw_at(frame, new_r, new_c, colors[rand()%NCOLORS], 1);
        usleep(30000);
        erase_at(new_r, new_c);
        usleep(15000);
    }
}

static void random_pos(int *row, int *col) {
    update_term_size();
    int max_r = term_rows - TORUS_H - 1;
    int max_c = term_cols - TORUS_W - 1;
    if (max_r < 1) max_r = 1;
    if (max_c < 1) max_c = 1;
    *row = rand()%max_r+1;
    *col = rand()%max_c+1;
}

int main(void) {
    srand((unsigned)time(NULL));
    update_term_size();
    signal(SIGWINCH, on_resize);
    signal(SIGINT,   on_exit_sig);
    signal(SIGTERM,  on_exit_sig);
    printf(CLEAR "\033[?25l");
    fflush(stdout);
    int nframes = 0;
    while (frames[nframes]) nframes++;
    int cur_row, cur_col;
    random_pos(&cur_row, &cur_col);
    int cycles_per_pos = rand()%4+2;
    int cycles_done    = 0;
    const char *color  = colors[rand()%NCOLORS];
    while (1) {
        for (int i = 0; i < nframes; i++) {
            int glitch = (rand()%100) < 18;
            if (glitch) {
                int gt = rand()%3+2;
                for (int g = 0; g < gt; g++) {
                    draw_at(frames[i], cur_row, cur_col, colors[rand()%NCOLORS], 1);
                    usleep(30000);
                }
            }
            draw_at(frames[i], cur_row, cur_col, color, 0);
            usleep(110000);
        }
        cycles_done++;
        if (cycles_done >= cycles_per_pos) {
            cycles_done    = 0;
            cycles_per_pos = rand()%4+2;
            int new_row, new_col;
            random_pos(&new_row, &new_col);
            teleport_effect(frames[nframes/2], cur_row, cur_col, new_row, new_col);
            cur_row = new_row; cur_col = new_col;
            color   = colors[rand()%NCOLORS];
            if ((rand()%3) == 0) { printf(CLEAR); fflush(stdout); }
        }
    }
    return 0;
}
