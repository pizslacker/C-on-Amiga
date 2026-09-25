#include <proto/intuition.h>
#include <proto/gadtools.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <intuition/intuition.h>
int main() {
   struct Window *myWindow;
   int closewin = FALSE;       /* Flag used to end program */
   struct IntuiMessage *msg;   /* Structure to store Intuition message data */
   ULONG msgClass;             /* Class value */
   myWindow = OpenWindowTags(NULL,
   WA_Left, 20, WA_Top, 20,
   WA_Width, 400, WA_Height, 150,
   WA_IDCMP, IDCMP_CLOSEWINDOW,
   WA_Flags, WFLG_SIZEGADGET | WFLG_DRAGBAR | WFLG_DEPTHGADGET | WFLG_CLOSEGADGET | WFLG_ACTIVATE,
   WA_Title, "Kims AmigaOS Window-test",
   WA_PubScreenName, "Workbench",
   TAG_DONE);
 while (closewin == FALSE) {                       /* Run program until window is closed */
    Wait(1L << myWindow->UserPort->mp_SigBit);     /* Wait for an event! */
    msg = GT_GetIMsg(myWindow->UserPort);          /* Get message data */ 
    msgClass = msg->Class;                         /* What has been clicked? */
    GT_ReplyIMsg(msg);                             /* Close message */
    if (msgClass == IDCMP_CLOSEWINDOW) {           /* Check here if Close Window selected */
        CloseWindow(myWindow);                     /* Close window */ 
        closewin = TRUE;
    }
   }
 return(0);
 }
