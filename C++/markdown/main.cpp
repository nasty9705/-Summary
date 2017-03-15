#include "textViewer.hpp"
#include "functions.hpp"
#include <iostream>
#include <cstdlib>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cmath>

extern char *optarg;
extern int optind, opterr, optopt;

char *filename = NULL, *helpfile = (char*)"help.txt", list_sym = '*';
unsigned int otstup = 2, width_screen = 20, redline = 1;
float programVersion = 1;
struct winsize terminal;

int main(int argc, char *argv[])
{
    char result;

    //ioctl(0, TIOCGWINSZ, &terminal);
    //width_screen = terminal.ws_col;
    
    if (argc == 1)
    {
        printf("Invalid count of parametrs!\n");
        return 0;
    }

    //printf("++++++\n");
    
    while ((result = getopt(argc, argv, "f:w::t::m::r::hv")) != -1)
    {
        switch (result){
            case 'f': scan_arg('f'); break;
            case 'w': scan_arg('w'); break;
            case 't': scan_arg('t'); break;
            case 'm': scan_arg('m'); break;
            case 'r': scan_arg('r'); break;
            case 'h': help(); return 0;
            case 'v': printf("You're using the latest version of program (%.2f)\n",
                             programVersion); return 0;
            case '?': printf("invalid parameter or argument!\n"); return 0;
        }
            
    }
    //printf("-------\n");
    
    if (width_screen < 4) {printf("Screen size is too small!\n"); return 0;}
    
    FILE *in = fopen(filename, "rt");
    FILE *out = fopen("out.txt", "wt");
    
    Text_viewer tst(in);
    tst.print(out);
    
    fclose(in);
    fclose(out);
    
    if (filename != NULL)
        free(filename);
    
    return 0;
}
