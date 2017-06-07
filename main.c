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
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stropts.h>
#include <string.h>

#include "findwin.h"

int main(int argc, char *argv[])
{
    //spawn bash
    pid_t pid;
    int bashin[2], bashout[2], basherr[2];
    /*
    pipe(bashin);
    pipe(bashout);
    pipe(basherr);
    */

    pid = fork();
    if(pid == 0)
    {
        /*
        dup2(bashin[0],0);
        dup2(bashout[1],1);
        dup2(basherr[1],2);
        */

        printf("xterm\n");
        execl("/usr/bin/xterm", "/usr/bin/xterm", NULL);
        printf("xterm done\n");
    }
        /*
        close(bashin[0]);
        close(bashout[1]);
        fcntl(bashout[0], F_SETFL, 
                fcntl(bashout[0], F_GETFL, 0) | O_NONBLOCK);
        close(basherr[1]);
        fcntl(basherr[0], F_SETFL, 
                fcntl(basherr[0], F_GETFL, 0) | O_NONBLOCK);
        */

    sleep(1);

    //GC grc;
    //XGCValues grv;
    XFontStruct *fontinfo;
    XColor color,
           dummy;

    Display *display = XOpenDisplay(0);
    /*
    Window win = XCreateSimpleWindow(display, XDefaultRootWindow(display), 
            0, 0, 1920, 1080, 0, 0, 
            WhitePixel(display, DefaultScreen(display)));
    */
    Window win = search(display, XDefaultRootWindow(display), pid); 
    printf("win:%i\n",win);
    if(win == 0)
        perror("Window not found\n");
    //XSelectInput(display, win, KeyPressMask | KeyReleaseMask);

    Atom window_type = XInternAtom(display, 
            "_NET_WM_WINDOW_TYPE", False);

    Atom desktop = XInternAtom(display, 
            "_NET_WM_WINDOW_TYPE_DESKTOP", False);
    printf("changing property\n");
    XChangeProperty (display, win, window_type, XA_ATOM, 
            32, PropModeReplace, (unsigned char *) &desktop, 1);
    
    XWindowChanges changes;
    changes.x = 0;
    changes.y = 0;
    changes.width = 1920;
    changes.height = 1080;
    changes.border_width = 0;
    XConfigureWindow(display, win, CWWidth | CWHeight | CWX | CWY, &changes);

    printf("clearing window\n");
    XClearWindow(display, win);
    printf("xmapwindow\n");
    XMapWindow(display, win);
    printf("making font\n");
    //fontinfo = XLoadQueryFont(display,"-*-*-*-*-*-*-34-*-*-*-*-*-*-*");

    XAllocNamedColor(display, DefaultColormap(display,
            DefaultScreen(display)),"red", &color, &dummy);

    //grv.font = fontinfo->fid;
    //grv.foreground = color.pixel;

    //grc = XCreateGC(display,win,GCFont + GCForeground, &grv);

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
        /*
        char c[1];
        while(ioctl(bashout[0], I_NREAD, 1) == 0)
        {
            read(bashout[0], c, 1);
            printf("bash:%c\n", c);
            fflush(stdout);
            XDrawString(display, win, grc,x,y,c,1); 
        }
        */
    }

    XUnmapWindow(display, win);
    XCloseDisplay(display);
}
