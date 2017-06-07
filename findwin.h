#include <X11/Xlib.h>
#include <X11/Xatom.h>

Window search(Display *d, Window win, int pid)
{
    // Get the PID for the current Window.
    Atom atomPID = XInternAtom(d, "_NET_WM_PID", True);
    Atom           type;
    int            format;
    unsigned long  nItems;
    unsigned long  bytesAfter;
    unsigned char *propPID = 0;

    if(Success == XGetWindowProperty(d, win, atomPID, 0, 1, 
            False, XA_CARDINAL, &type, &format, &nItems, &bytesAfter, &propPID))
    {
        if(propPID != 0)
        {
            // If the PID matches, add this window to the result set.
            if(pid == *((unsigned long *)propPID))
                return win;

            XFree(propPID);
        }
    }

// Recurse into child windows.
    Window    wRoot;
    Window    wParent;
    Window   *wChild;
    unsigned  nChildren;
    if(0 != XQueryTree(d, win, &wRoot, &wParent, &wChild, &nChildren))
    {
        for(unsigned i = 0; i < nChildren; i++)
        {
            Window w = search(d, wChild[i], pid);
            if(w != 0)
                return w;
        }
    }

    return 0;
}

