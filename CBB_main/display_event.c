#include "main.h"

/* Function to display an event */
void display_event(EVENTS event)
{
        switch (event)
        {
                case ON://ON
                         if (display == DISP_DOWNLOAD_LOG)
								 puts("ON");
						 else
								 clcd_print("ON", LINE2(11));
                        break;
                case CO://Collision
                        if (display == DISP_DOWNLOAD_LOG)
								puts("CO");
						else
								clcd_print("CO", LINE2(11));
                        break;
                case GN://Neutral gear
                        if (display == DISP_DOWNLOAD_LOG)
								puts("GN");
						else
								clcd_print("GN", LINE2(11));
                        break;
                case G1://1st gear
                        if (display == DISP_DOWNLOAD_LOG)
								puts("G1");
						else
								clcd_print("G1", LINE2(11));
                        break;
                case G2://2nd gear
                        if (display == DISP_DOWNLOAD_LOG)
								puts("G2");
						else
								clcd_print("G2", LINE2(11));
                        break;
                case G3://3rd gear
                        if (display == DISP_DOWNLOAD_LOG)
								puts("G3");
						else
								clcd_print("G3", LINE2(11));
                        break;
                case G4://4th gear
                        if (display == DISP_DOWNLOAD_LOG)
								puts("G4");
						else
								clcd_print("G4", LINE2(11));
                        break;
                case GR://Reverse gear
                        if (display == DISP_DOWNLOAD_LOG)
								puts("GR");
						else
								clcd_print("GR", LINE2(11));
                        break;
                        /* Below events are displayed only in view log & download log */
                case ST://Set time
                        if (display == DISP_DOWNLOAD_LOG)
								puts("ST");
						else
								clcd_print("ST", LINE2(11));
                        break;
                case CH://Change password
                        if (display == DISP_DOWNLOAD_LOG)
								puts("CH");
						else
								clcd_print("CH", LINE2(11));
                        break;
                case CL://Clear log
                        if (display == DISP_DOWNLOAD_LOG)
								puts("CL");
						else
								clcd_print("CL", LINE2(11));
                        break;
                case DL://Download log
                        if (display == DISP_DOWNLOAD_LOG)
								puts("DL");
						else
								clcd_print("DL", LINE2(11));
        }
}

