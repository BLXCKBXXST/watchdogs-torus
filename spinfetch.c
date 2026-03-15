/* spinfetch.c — neofetch-style system info + Watch Dogs torus
 * Compile: gcc -O2 -o spinfetch spinfetch.c
 * Run:     ./spinfetch
 * Stop:    Ctrl+C
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <pwd.h>

#define GREEN   "\033[92m"
#define MAGENTA "\033[95m"
#define CYAN    "\033[96m"
#define RED     "\033[91m"
#define YELLOW  "\033[93m"
#define BOLD    "\033[1m"
#define RESET   "\033[0m"
#define CLEAR   "\033[H\033[J"

#define INFO_COL  42
#define TORUS_ROWS 16

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

static const char *glitch_colors[] = { GREEN, MAGENTA, CYAN, RED };

typedef struct {
    char user[64];
    char hostname[64];
    char os[128];
    char kernel[128];
    char cpu[128];
    char uptime[64];
    char ram[64];
    char shell[64];
    char packages[32];
    char arch[32];
} SysInfo;

static void get_os(char *buf, int sz) {
    FILE *f = fopen("/etc/os-release", "r");
    if (!f) { strncpy(buf, "Linux", sz); return; }
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "PRETTY_NAME=", 12) == 0) {
            char *val = line + 12;
            if (*val == '"') val++;
            val[strcspn(val, "\"\n")] = 0;
            strncpy(buf, val, sz);
            fclose(f);
            return;
        }
    }
    strncpy(buf, "Linux", sz);
    fclose(f);
}

static void get_cpu(char *buf, int sz) {
    FILE *f = fopen("/proc/cpuinfo", "r");
    if (!f) { strncpy(buf, "unknown", sz); return; }
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "model name", 10) == 0) {
            char *val = strchr(line, ':');
            if (val) { val += 2; val[strcspn(val, "\n")] = 0; strncpy(buf, val, sz); }
            fclose(f);
            return;
        }
    }
    strncpy(buf, "unknown", sz);
    fclose(f);
}

static void get_ram(char *buf, int sz) {
    struct sysinfo si;
    if (sysinfo(&si) != 0) { strncpy(buf, "unknown", sz); return; }
    long total = si.totalram / 1024 / 1024;
    long used  = (si.totalram - si.freeram - si.bufferram) / 1024 / 1024;
    snprintf(buf, sz, "%ld MiB / %ld MiB", used, total);
}

static void get_uptime(char *buf, int sz) {
    struct sysinfo si;
    if (sysinfo(&si) != 0) { strncpy(buf, "unknown", sz); return; }
    long s = si.uptime;
    int d = s/86400, h = (s%86400)/3600, m = (s%3600)/60;
    if (d > 0) snprintf(buf, sz, "%dd %dh %dm", d, h, m);
    else if (h > 0) snprintf(buf, sz, "%dh %dm", h, m);
    else snprintf(buf, sz, "%dm", m);
}

static void collect_info(SysInfo *s) {
    struct passwd *pw = getpwuid(getuid());
    strncpy(s->user, pw ? pw->pw_name : "user", sizeof(s->user));
    gethostname(s->hostname, sizeof(s->hostname));
    get_os(s->os, sizeof(s->os));
    struct utsname u; uname(&u);
    snprintf(s->kernel, sizeof(s->kernel), "%s %s", u.sysname, u.release);
    strncpy(s->arch, u.machine, sizeof(s->arch));
    get_cpu(s->cpu, sizeof(s->cpu));
    get_ram(s->ram, sizeof(s->ram));
    get_uptime(s->uptime, sizeof(s->uptime));
    char *sh = getenv("SHELL");
    strncpy(s->shell, sh ? sh : "unknown", sizeof(s->shell));
    FILE *f = popen("command -v dpkg-query >/dev/null 2>&1 && dpkg-query -f '.\n' -W 2>/dev/null | wc -l || ls /usr/bin | wc -l", "r");
    if (f) { fgets(s->packages, sizeof(s->packages), f); s->packages[strcspn(s->packages, "\n")] = 0; pclose(f); }
}

static void render(const char *frame, const SysInfo *s, int glitch) {
    static const char gc[] = "#@%&!?/|<>^~*+=-:;";
    int glen = (int)strlen(gc);
    char torus_lines[TORUS_ROWS][80];
    int ntor = 0;
    const char *p = frame;
    while (*p && ntor < TORUS_ROWS) {
        const char *nl = strchr(p, '\n');
        int len = nl ? (int)(nl-p) : (int)strlen(p);
        if (len > 79) len = 79;
        memcpy(torus_lines[ntor], p, len);
        torus_lines[ntor][len] = 0;
        ntor++;
        p = nl ? nl+1 : p+len;
    }
    while (ntor < TORUS_ROWS) torus_lines[ntor++][0] = 0;
    char info[20][96]; int ni = 0;
    char sep[48]; memset(sep, '-', 40); sep[40] = 0;
    snprintf(info[ni++], 96, BOLD GREEN "%s" RESET CYAN "@" RESET BOLD GREEN "%s" RESET, s->user, s->hostname);
    snprintf(info[ni++], 96, CYAN "%s" RESET, sep);
    snprintf(info[ni++], 96, CYAN BOLD "OS     " RESET ": %s", s->os);
    snprintf(info[ni++], 96, CYAN BOLD "Kernel " RESET ": %s", s->kernel);
    snprintf(info[ni++], 96, CYAN BOLD "Arch   " RESET ": %s", s->arch);
    snprintf(info[ni++], 96, CYAN BOLD "Uptime " RESET ": %s", s->uptime);
    snprintf(info[ni++], 96, CYAN BOLD "Shell  " RESET ": %s", s->shell);
    snprintf(info[ni++], 96, CYAN BOLD "Pkgs   " RESET ": %s", s->packages);
    snprintf(info[ni++], 96, CYAN BOLD "CPU    " RESET ": %s", s->cpu);
    snprintf(info[ni++], 96, CYAN BOLD "RAM    " RESET ": %s", s->ram);
    snprintf(info[ni++], 96, "");
    snprintf(info[ni++], 96, "\033[40m   \033[41m   \033[42m   \033[43m   \033[44m   \033[45m   \033[46m   \033[47m   " RESET);
    fputs(CLEAR, stdout);
    int rows = ntor > ni ? ntor : ni;
    for (int i = 0; i < rows; i++) {
        if (i < ntor && torus_lines[i][0]) {
            if (glitch) {
                fputs(glitch_colors[rand()%4], stdout);
                for (const char *c = torus_lines[i]; *c; c++) {
                    if (*c != ' ' && (rand()%100) < 7) putchar(gc[rand()%glen]);
                    else putchar(*c);
                }
            } else { fputs(GREEN, stdout); fputs(torus_lines[i], stdout); }
            fputs(RESET, stdout);
        }
        int printed = (i < ntor) ? (int)strlen(torus_lines[i]) : 0;
        for (int sp = printed; sp < INFO_COL; sp++) putchar(' ');
        if (i < ni) fputs(info[i], stdout);
        putchar('\n');
    }
    fflush(stdout);
}

int main(void) {
    srand((unsigned)time(NULL));
    SysInfo s; collect_info(&s);
    int nframes = 0;
    while (frames[nframes]) nframes++;
    while (1) {
        for (int i = 0; i < nframes; i++) {
            int glitch = (rand()%100) < 20;
            if (glitch) {
                int gt = rand()%3+2;
                for (int g = 0; g < gt; g++) { render(frames[i], &s, 1); usleep(30000); }
            }
            render(frames[i], &s, 0);
            usleep(110000);
        }
    }
    return 0;
}
