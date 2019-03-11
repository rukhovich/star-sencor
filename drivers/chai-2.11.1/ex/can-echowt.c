/*
*  can-echowt.c
*  sleep-wait algo,
*  recieves frames from CAN ( baudrate = 500K )
*  and sends it back.
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <chai.h>               /* includes declarations of CHAI */

int main(int argc, char **argv) 
{
    canmsg_t frame;
    canerrs_t errs;
    canwait_t cw;
    int ret;
    int chan;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s X \n", argv[0]);
        fprintf(stderr, "       where X=0,1,2 ...\n");
        exit(1);
    }
    chan = atoi(argv[1]);
    if ((ret = CiInit()) < 0) {
        fprintf(stderr,
            "CHAI library initialization failed, errcode = %d\n",
            ret);
        exit(1);
    }

    /*  open channel */ 
    if ((ret = CiOpen(chan, CIO_CAN11 | CIO_CAN29)) < 0) {
        fprintf(stderr, "Error opening CAN channel %s, errcode = %d\n",
            argv[1], ret);
        exit(1);
    }

    /* set baud rate to 500Kbit */ 
    if ((ret = CiSetBaud(chan, BCI_500K)) < 0) {
        fprintf(stderr, "can't set baud, errcode = %d\n", ret);
        CiClose(chan);
        exit(1);
    }

    /*  start CAN-controller */ 
    CiStart(chan);
    cw.chan = chan;
    cw.wflags = CI_WAIT_RC | CI_WAIT_ER;

    /* main loop */ 
    while (1) {
        ret = CiWaitEvent(&cw, 1, 5000);
        if (ret < 0) {
            fprintf(stderr, "CiWaitEvent() failed, errcode = %d\n", ret);
        } else if (ret > 0) {
            if (cw.rflags & CI_WAIT_RC) {
                ret = CiRead(chan, &frame, 1); /* read 1 frame from channel */
                if (ret == 1) {
                    printf("rcv frame id=%#lx\n", (unsigned long) frame.id);
                    frame.id += 1;
                    if ((ret = CiWrite(chan, &frame, 1)) < 1) {        /* write modified frame back to channel */
                        fprintf(stderr,
                            "can't send frame back to CAN, errcode = %d\n",
                            ret);
                        CiClose(chan);
                        exit(1);
                    }
                } else if (ret < 0) {
                    fprintf(stderr,
                        "error recieving frame from CAN, errcode = %d\n",
                        ret);
                    CiClose(chan);
                    exit(1);
                }
            }
            if (cw.rflags & CI_WAIT_ER) {
                printf("CAN-error occured: ");
                ret = CiErrsGetClear(chan, &errs);
                if (ret >= 0) {
                    if (errs.ewl) {
                        printf("EWL %d times", errs.ewl);
                    }
                    if (errs.boff) {
                        printf("BOFF %d times", errs.boff);
                    }
                    if (errs.hwovr) {
                        printf("HOVR %d times", errs.hwovr);
                    }
                    if (errs.swovr) {
                        printf("SOVR %d times", errs.swovr);
                    }
                    if (errs.wtout) {
                        printf("WTOUT %d times", errs.wtout);
                    }
                } else {
                    printf("CiErrsGetClear() failed\n");
                    CiClose(chan);
                    exit(1);
                }
                printf("\n");
            }
        } /* else { // ret == 0 - timeout
          printf("CiWaitEvent timeout\n");
          } */
    }
    /* never reach this point */ 
    exit(0);
}


