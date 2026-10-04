#include <proto/intuition.h>
#include <proto/gadtools.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <intuition/intuition.h>

struct IntuitionBase *IntuitionBase = NULL;
struct Library       *GadToolsBase  = NULL;

int main(void) 
{
    struct Window *myWindow;
    int closewin = FALSE;
    struct IntuiMessage *msg;
    ULONG msgClass;
    LONG sys_result;

    /* 1. Open Libraries */
    IntuitionBase = (struct IntuitionBase *)OpenLibrary("intuition.library", 37L);
    if (!IntuitionBase) return(RETURN_FAIL);

    GadToolsBase = OpenLibrary("gadtools.library", 37L);
    if (!GadToolsBase) {
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
        WA_Title,         "Kims Demo Launcher",
        WA_PubScreenName, "Workbench",
        TAG_DONE);

    if (myWindow) {
        
        /* 3. Launch the External Demo */
        /* Change "DF1:Demos/MyCoolDemo" to the actual path of your demo file */
        sys_result = SystemTags("DF1:Demos/MyCoolDemo",
            SYS_Asynch, TRUE,            /* Run in background, don't freeze this window */
            SYS_Input,  Output(),        /* Reuse current process input/output stream */
            SYS_Output, NULL,            /* Prevent opening an ugly CLI window */
            TAG_DONE);

        if (sys_result == -1) {
            /* Failed to launch process (e.g., file not found or out of memory) */
            DisplayBeep(NULL); 
        }

        /* 4. Standard Blocking Event Loop */
        /* Because the demo runs in its own process, we safely use Wait() again */
        while (closewin == FALSE) {
            Wait(1L << myWindow->UserPort->mp_SigBit);

            while ((msg = GT_GetIMsg(myWindow->UserPort)) != NULL) {
                msgClass = msg->Class;
                GT_ReplyIMsg(msg);

                if (msgClass == IDCMP_CLOSEWINDOW) {
                    closewin = TRUE;
                }
            }
        }

        CloseWindow(myWindow);
    }

    /* 5. Cleanup */
    CloseLibrary(GadToolsBase);
    CloseLibrary((struct Library *)IntuitionBase);

    return(0);
}
