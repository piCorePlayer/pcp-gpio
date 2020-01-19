/*
 * gpio.c:

 * Some of this code is derrived from wiringPi
 *	Copyright (c) 2012-2018 Gordon Henderson
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/* Values for pull-ups/downs off, pull-down and pull-up. */

#define PI_PUD_OFF  0
#define PI_PUD_DOWN 1
#define PI_PUD_UP   2

/* gpio modes. */

#define PI_INPUT  0
#define PI_OUTPUT 1
#define PI_ALT0   4
#define PI_ALT1   5
#define PI_ALT2   6
#define PI_ALT3   7
#define PI_ALT4   3
#define PI_ALT5   2



#define LOW  0
#define HIGH 1

extern void gpioSetPullUpDown(unsigned gpio, unsigned pud);
extern void gpioSetMode(unsigned gpio, unsigned mode);
extern int gpioGetMode(unsigned gpio);
extern int gpioRead(unsigned gpio);
extern void gpioWrite(unsigned gpio, unsigned level);
extern int gpioInitialise(void);

char *usage = "Usage: pcp-gpio -h                Show Help\n"
              "       pcp-gpio <mode/read/write> ...\n"
              "       pcp-gpio <toggle> <pin>\n\n"
              "       pcp-gpio is derrived from wiringPi's gpio command line utility.\n"
              "          Copyright (c) 2012-2018 Gordon Henderson\n"
              "          http://wiringpi.com/\n\n"
              "       Also uses minimal_gpio from http://abyz.me.uk/rpi/pigpio/examples.html"; // No trailing newline needed here.


/*
 * doMode:
 *	gpio mode pin mode ...
 *********************************************************************************
 */

void doMode (int argc, char *argv [])
{
  int pin ;
  char *mode ;
  int pinmode ;

  if (argc != 4)
  {
    fprintf (stderr, "Usage: %s mode pin mode\n", argv [0]) ;
    exit (1) ;
  }

  pin = atoi (argv [2]) ;

  mode = argv [3] ;

  /**/ if (strcasecmp (mode, "in")      == 0) gpioSetMode       (pin, PI_INPUT) ;
  else if (strcasecmp (mode, "input")   == 0) gpioSetMode       (pin, PI_INPUT) ;
  else if (strcasecmp (mode, "out")     == 0) gpioSetMode       (pin, PI_OUTPUT) ;
  else if (strcasecmp (mode, "output")  == 0) gpioSetMode       (pin, PI_OUTPUT) ;
  else if (strcasecmp (mode, "up")      == 0) gpioSetPullUpDown (pin, PI_PUD_UP) ;
  else if (strcasecmp (mode, "down")    == 0) gpioSetPullUpDown (pin, PI_PUD_DOWN) ;
  else if (strcasecmp (mode, "off")     == 0) gpioSetPullUpDown (pin, PI_PUD_OFF) ;
  else if (strcasecmp (mode, "get")     == 0) { pinmode=gpioGetMode       (pin); printf("%i\n", pinmode);}
  else
  {
    fprintf (stderr, "%s: Invalid mode: %s. Should be in/out/up/down/off/get\n", argv [1], mode) ;
    exit (1) ;
  }
}

/*
 * doWrite:
 *	gpio write pin value
 *********************************************************************************
 */

static void doWrite (int argc, char *argv [])
{
  int pin, val ;

  if (argc != 4)
  {
    fprintf (stderr, "Usage: %s write pin value\n", argv [0]) ;
    exit (1) ;
  }

  pin = atoi (argv [2]) ;

  /**/ if ((strcasecmp (argv [3], "up") == 0) || (strcasecmp (argv [3], "on") == 0))
    val = 1 ;
  else if ((strcasecmp (argv [3], "down") == 0) || (strcasecmp (argv [3], "off") == 0))
    val = 0 ;
  else
    val = atoi (argv [3]) ;

  /**/ if (val == 0)
    gpioWrite (pin, LOW) ;
  else
    gpioWrite (pin, HIGH) ;
}


/*
 * doRead:
 *	Read a pin and return the value
 *********************************************************************************
 */

void doRead (int argc, char *argv []) 
{
  int pin, val ;

  if (argc != 3)
  {
    fprintf (stderr, "Usage: %s read pin\n", argv [0]) ;
    exit (1) ;
  }

  pin = atoi (argv [2]) ;
  val = gpioRead (pin) ;

  printf ("%s\n", val == 0 ? "0" : "1") ;
}

/*
 * doToggle:
 *	Toggle an IO pin
 *********************************************************************************
 */

void doToggle (int argc, char *argv [])
{
  int pin ;

  if (argc != 3)
  {
    fprintf (stderr, "Usage: %s toggle pin\n", argv [0]) ;
    exit (1) ;
  }

  pin = atoi (argv [2]) ;

  gpioWrite (pin, !gpioRead (pin)) ;
}

/*
 * main:
 *	Start here
 *********************************************************************************
 */

int main (int argc, char *argv [])
{
  int i ;

  if (argc == 1)
  {
    fprintf (stderr,
"%s: At your service!\n"
"  Type: gpio -h for full details.\n\n"
    , argv [0]) ;
    exit (EXIT_FAILURE) ;
  }

// Help

  if (strcasecmp (argv [1], "-h") == 0)
  {
    printf ("%s: %s\n", argv [0], usage) ;
    exit (EXIT_SUCCESS) ;
  }

  if (geteuid () != 0)
  {
    fprintf (stderr, "%s: Must be root to run. Program should be suid root. This is an error.\n", argv [0]) ;
    exit (EXIT_FAILURE) ;
  }

  gpioInitialise();

  if (argc <= 1)
  {
    fprintf (stderr, "%s: no command given\n", argv [0]) ;
    exit (EXIT_FAILURE) ;
  }

  /**/ if (strcasecmp (argv [1], "mode"   ) == 0) doMode      (argc, argv) ;
  else if (strcasecmp (argv [1], "read"   ) == 0) doRead      (argc, argv) ;
  else if (strcasecmp (argv [1], "write"  ) == 0) doWrite     (argc, argv) ;
  else if (strcasecmp (argv [1], "toggle" ) == 0) doToggle    (argc, argv) ;

  else
  {
    fprintf (stderr, "%s: Unknown command: %s.\n", argv [0], argv [1]) ;
    exit (EXIT_FAILURE) ;
  }

  return 0 ;
}
