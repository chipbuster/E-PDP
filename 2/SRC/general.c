/*

       This file is part of the PDP software package.
		 
       Copyright 1987 by James L. McClelland and David E. Rumelhart.
       
       Please refer to licensing information in the file license.txt,
       which is in the same directory with this source file and is
       included here by reference.
*/


/* general.c

	Some general functions for PPD-pc package.
	
	First version implemented by Elliot Jaffe.
	
	Date of last revision:  8-12-87/JLM.
*/
/*LINTLIBRARY*/

#include "general.h"
#include "command.h"
#include "variable.h"
#include <signal.h>

#ifdef MSDOS
#include <memory.h>   /* for memcpy() in erealloc in general.c */
#include <process.h>  /* for system() in do_exec in command.c */
#endif

FILE * in_stream = stdin;
int     Interrupt_flag = 0;
int	single_flag = 0;
int	step_size;
int	random_seed;
char	step_string[STRINGLENGTH];

struct Command_table	*Command;

extern int  dump_template ();
extern int  clear_display ();
extern int  update_display ();
extern int  redisplay ();
extern int  do_io ();
extern int  do_network ();
extern int  do_system ();
extern int  do_command ();
extern int  do_comfile ();
extern int  do_exec ();
extern int  set_log ();
extern int  run_fork();


int_handler() {
    int     int_handler ();

    (void) signal(SIGINT, int_handler);
    Interrupt_flag = 1;
}

#ifdef MSDOS
char *index(somestring,somechar)
char *somestring;
char somechar;
{
    return strchr(somestring,somechar);
}
#endif

char   *emalloc (n)		/* check return from malloc */
unsigned    n;
{
    char   *p,
           *malloc ();

    p = malloc(n);
    if (p == 0)
	put_error("out of memory");
    return p;
}

char   *erealloc (ptr,oldsize,newsize)		/* check return from realloc*/
char *ptr;
unsigned    oldsize;
unsigned    newsize;
{
#ifndef MSDOS
    char   *realloc ();
    char   *p;

    p = realloc(ptr,newsize);
    if (p == 0)
	put_error("out of memory");
    return p;
    
#else (if MSDOS)

    char *malloc();
    char *p;
    
    p = malloc(newsize);
    if (p == 0) 
        put_error("out of memory");
    if (ptr && p) {
	memcpy(p, ptr, oldsize);
	free(ptr);
    }
    return p;

#endif MSDOS
}

startsame(s1, s2)		/* does s1 start the same as s2? */
char   *s1,
       *s2; {
    while (*s1 && *s2) {
	if (*s1++ != *s2++)
	    return(0);
    }
    if(*s1 && !*s2)  /* if s1 is longer than s2 it should fail */
       return(0);
    return(1);
}

char   *strsave (s)
char   *s;
{
    char   *p,
           *emalloc ();

    if ((p = emalloc((unsigned)(strlen(s) + 1))) != NULL)
	(void) strcpy(p, s);
    return(p);
}


randint(low, high)
 int  low,high; {
    int     answer;
    float   randf;
    int     range;

    randf = rnd();
    range = high - low + 1;
    answer = randf * range + low;
    return(answer);
}

quit() {
   char * str;
   str = get_command("Quit program? -- type y to confirm:  ");

   if (str && str[0] == 'y') {
	    end_display();
	    exit(0);
   }
   else
      return(CONTINUE);
}

set_step() {
    char old_step_string[STRINGLENGTH];
    struct Variable *vp, *lookup_var();
    
    strcpy(old_step_string,step_string);

    vp = lookup_var("stepsize");
    change_variable("stepsize",vp);
    
    if (startsame(step_string,"nepochs")) strcpy(step_string,"nepochs");
    else if (startsame(step_string,"epoch")) strcpy(step_string,"epoch");
    else if (startsame(step_string,"pattern")) strcpy(step_string,"pattern");
    else if (startsame(step_string,"ncycles")) strcpy(step_string,"ncycles");
    else if (startsame(step_string,"cycle")) strcpy(step_string,"cycle");
    else if (startsame(step_string,"update")) strcpy(step_string,"update");
    else if (startsame(step_string,"default"))
    				strcpy(step_string,Default_step_string);
    else {
	strcpy(step_string,old_step_string);
    	return(put_error("urecognized stepsize -- size not changed."));
    }
    set_stepsize();
    return(CONTINUE);
}

set_stepsize() {
    if (strcmp(step_string,"update") == 0) step_size = UPDATE;
    else if (strcmp(step_string,"cycle") == 0) step_size = CYCLE;
    else if (strcmp(step_string,"ncycles") == 0) step_size = NCYCLES;
    else if (strcmp(step_string,"pattern") == 0) step_size = PATTERN;
    else if (strcmp(step_string,"epoch") == 0) step_size = EPOCH;
    else if (strcmp(step_string,"nepochs") == 0) step_size = NEPOCHS;
}

init_general() {
    extern int     int_handler ();

    Interrupt_flag = 0;
    strcpy(step_string,Default_step_string);
    set_stepsize();
    init_commands();
    (void) signal(SIGINT, int_handler);
    (void) install_command("?", do_help, 0, 0);
    (void) install_command("disp/", do_command, BASEMENU, (int *) DISPLAYMENU);
    (void) install_command("opt/",  do_command, DISPLAYMENU, (int *) DISPLAYOPTIONS);
    (void) install_command("exam/", do_command, BASEMENU, (int *) SETMENU);
    (void) install_command("get/",  do_command, BASEMENU, (int *) GETMENU);
    (void) install_command("save/", do_command, BASEMENU, (int *) SAVEMENU);
    (void) install_command("set/",  do_command, BASEMENU, (int *) SETMENU);
    (void) install_command("config/", do_command, SETMENU, (int *) SETCONFMENU);
    (void) install_command("env/",  do_command, SETMENU, (int *) SETENVMENU);
    (void) install_command("mode/", do_command, SETMENU, (int *) SETMODEMENU);
    (void) install_command("param/",do_command, SETMENU, (int *) SETPARAMMENU);
    (void) install_command("state/", do_command, SETMENU, (int *) SETSVMENU);
    (void) install_command("clear", clear_display, BASEMENU, 0);
    (void) install_command("do",  do_comfile, BASEMENU, 0);
    (void) install_command("log",   set_log, BASEMENU, 0);
    (void) install_command("quit",  quit, BASEMENU, 0);
    (void) install_command("run", do_exec, BASEMENU, 0);
/*  (void) install_command("srand", random_seed, BASEMENU, 0); */
    (void) install_command("state", redisplay, DISPLAYMENU, 0);
    (void) install_var("seed", Int, (int *) & random_seed, 0, 0,SETPCMENU);
    (void) install_var("single", Int, (int *) & single_flag, 0, 0,SETPCMENU);
    (void) install_var("stepsize", String, (int *) step_string,0, 0,NOMENU);
    (void) install_command("stepsize",set_step,SETPCMENU,(int *) NULL);
}

#ifdef MSDOS
sleep(n_sec)
int n_sec;
{
    int i,j;
    for (i = 0; i < (n_sec); i++)
        for (j = 0; j < 20000; j++);
}
#endif  MSDOS

