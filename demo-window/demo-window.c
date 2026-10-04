#include <proto/intuition.h>
#include <proto/gadtools.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/graphics.h> /* Added for drawing functions */
#include <intuition/intuition.h>

struct IntuitionBase *IntuitionBase = NULL;
struct Library       *GadToolsBase  = NULL;
struct GfxBase       *GfxBase       = NULL; /* Added GfxBase pointer */

int main(void) 
{
    struct Window *myWindow;
    int closewin = FALSE;
    struct IntuiMessage *msg;
    ULONG msgClass;
    
    /* Variables for our mini-demo loop */
    WORD demo_x = 20;
    WORD demo_y = 40;
    WORD dx = 2;
    WORD dy = 1;
    ULONG color_counter = 1;

    /* 1. Open Libraries */
    IntuitionBase = (struct IntuitionBase *)OpenLibrary("intuition.library", 37L);
    if (!IntuitionBase) return(RETURN_FAIL);

    GadToolsBase = OpenLibrary("gadtools.library", 37L);
    if (!GadToolsBase) {
        CloseLibrary((struct Library *)IntuitionBase);
        return(RETURN_FAIL);
    }

    /* Open graphics library for drawing commands */
    GfxBase = (struct GfxBase *)OpenLibrary("graphics.library", 37L);
    if (!GfxBase) {
        CloseLibrary(GadToolsBase);
        CloseLibrary((struct Library *)IntuitionBase);
        return(RETURN_FAIL);
    }

    /* 2. Open Window */
    myWindow = OpenWindowTags(NULL,
        WA_Left,          20,
        WA_Top,           20,
        WA_Width,         400,
        WA_Height,        150,
        WA_IDCMP,         IDCMP_CLOSEWINDOW,
        WA_Flags,         WFLG_SIZEGADGET | WFLG_DRAGBAR | WFLG_DEPTHGADGET | WFLG_CLOSEGADGET | WFLG_ACTIVATE,
        WA_Title,         "Kims Window Demo Loop",
        WA_PubScreenName, "Workbench",
        TAG_DONE);

    if (myWindow) {
        /* Set up the initial background color for our drawing */
        SetAPen(myWindow->RPort, 1); /* Pen 1 is usually black/blue on WB */

        while (closewin == FALSE) {
            
            /* --- THE ASYNCHRONOUS EVENT CHECK --- */
            /* Instead of using Wait(), we check for messages instantly.
               If there are no messages, the code drops through to render the demo frame. */
            while ((msg = GT_GetIMsg(myWindow->UserPort)) != NULL) {
                msgClass = msg->Class;
                GT_ReplyIMsg(msg);

                if (msgClass == IDCMP_CLOSEWINDOW) {
                    closewin = TRUE;
                }
            }

            /* --- THE DEMO RENDERING LOOP --- */
            if (!closewin) {
                /* Set a cycling color pen based on Workbench palette (usually 0-3 or 0-7) */
                SetAPen(myWindow->RPort, (color_counter % 3) + 1);

                /* Draw a simple vector-style moving line inside our window boundaries */
                Move(myWindow->RPort, demo_x, demo_y);
                Draw(myWindow->RPort, 380 - demo_x, 130 - demo_y);

                /* Update demo physics coordinates */
                demo_x += dx;
                demo_y += dy;

                /* Bounce off the inner boundaries of our 400x150 window */
                if (demo_x >= 380 || demo_x <= 20)  { dx = -dx; color_counter++; }
                if (demo_y >= 130 || demo_y <= 40)  { dy = -dy; color_counter++; }

                /* Slow down the animation slightly so it doesn't max out the CPU */
                Delay(1); 
            }
        }

        CloseWindow(myWindow);
    }

    /* 3. Cleanup Libraries */
    CloseLibrary((struct Library *)GfxBase);
    CloseLibrary(GadToolsBase);
    CloseLibrary((struct Library *)IntuitionBase);

    return(0);
}
