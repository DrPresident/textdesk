/* Jack Swenson
 *
 * allow putting any window on desktop
 */
#include <X11/StringDefs.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "findwin.h"

int main(int argc, char *argv[])
{
    //spawn bash
    pid_t pid = fork();
    if(pid == 0)
        execl("/usr/bin/xterm", "/usr/bin/xterm", NULL);

    sleep(1);

    Display *display = XOpenDisplay(0);
    Window win = search(display, XDefaultRootWindow(display), pid); 
    if(win == 0)
        perror("Window not found\n");

    Atom window_type = XInternAtom(display, 
            "_NET_WM_WINDOW_TYPE", False);

    Atom desktop = XInternAtom(display, 
            "_NET_WM_WINDOW_TYPE_DESKTOP", False);

    XChangeProperty (display, win, window_type, XA_ATOM, 
            32, PropModeReplace, (unsigned char *) &desktop, 1);
    
    XWindowChanges changes;
    changes.x = 0;
    changes.y = 10;
    changes.width = 1920;
    changes.height = 1070;
    changes.border_width = 0;
    XConfigureWindow(display, win, CWWidth | CWHeight | CWX | CWY, &changes);

    XClearWindow(display, win);
    XMapWindow(display, win);

    XEvent event;
    int x = 0,
        y = 0,
        font_h = 50,
        font_w = 34,
        len,
        ctrl = 0,
        shift = 0;
    unsigned int kc;
    KeySym key;
    char str[255];

    while(1)
    {
        XNextEvent(display, &event);

        switch(event.type)
        {
            case KeyPress:
                if(XLookupString(&event.xkey,str,255,&key,0) == 1)
                {
                    printf("key:<%s>\n",str);
                    if(str[0] == 13)
                        printf("enter\n");
                    fflush(stdout);
                    len = strlen(str);
                    /*
                    for(int i = 0; i < len; i++)
                    {
                        switch(str[i])
                        {
                            //enter
                            case 13:
                                break;
                            default:
                                break;
                        }
                    }
                    */
                    //write(bashin[0], str, strlen(str));
                }
                else{
                    printf("nokey\n");
                    fflush(stdout);
                }
                break;
        }

        int status;
        wait(&status);
    }

    XUnmapWindow(display, win);
    XCloseDisplay(display);
}
