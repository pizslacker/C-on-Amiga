/*
 * Kims-window.c
 * Copyright (C) k!M/pizslacker 2026
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <proto/intuition.h>
#include <proto/gadtools.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <intuition/intuition.h>

/* Define global library bases for AmigaOS 3.x */
struct IntuitionBase *IntuitionBase = NULL;
struct Library       *GadToolsBase  = NULL;

int main(void) 
{
    struct Window *myWindow;
    int closewin = FALSE;
    struct IntuiMessage *msg;
    ULONG msgClass;

    /* 1. Open Intuition Library */
    IntuitionBase = (struct IntuitionBase *)OpenLibrary("intuition.library", 37L);
    if (!IntuitionBase) {
        return(RETURN_FAIL);
    }

    /* 2. Open GadTools Library (since you are using GT_GetIMsg) */
    GadToolsBase = OpenLibrary("gadtools.library", 37L);
    if (!GadToolsBase) {
        CloseLibrary((struct Library *)IntuitionBase);
        return(RETURN_FAIL);
    }

    /* 3. Open the Window with a modern OS3 Tags call */
    myWindow = OpenWindowTags(NULL,
        WA_Left,          20,
        WA_Top,           20,
        WA_Width,         400,
        WA_Height,        150,
        WA_IDCMP,         IDCMP_CLOSEWINDOW,
        WA_Flags,         WFLG_SIZEGADGET | WFLG_DRAGBAR | WFLG_DEPTHGADGET | WFLG_CLOSEGADGET | WFLG_ACTIVATE,
        WA_Title,         "k!Ms AmigaOS Window-test",
        WA_PubScreenName, "Workbench",
        TAG_DONE);

    /* 4. Safety Check: Only enter loop if window successfully opened */
    if (myWindow) {
        while (closewin == FALSE) {
            /* Wait for an event signal */
            Wait(1L << myWindow->UserPort->mp_SigBit);

            /* Drain the message loop (processes multiple fast clicks/events) */
            while ((msg = GT_GetIMsg(myWindow->UserPort)) != NULL) {
                msgClass = msg->Class;

                /* Always reply to the message before handling destructive actions */
                GT_ReplyIMsg(msg);

                if (msgClass == IDCMP_CLOSEWINDOW) {
                    closewin = TRUE;
                }
            }
        }

        /* 5. Cleanup Window */
        CloseWindow(myWindow);
    }

    /* 6. Cleanup Libraries in reverse order */
    CloseLibrary(GadToolsBase);
    CloseLibrary((struct Library *)IntuitionBase);

    return(0);
}
