/* #includes */ /*{{{C}}}*//*{{{*/
#undef  _POSIX_SOURCE
#define _POSIX_SOURCE   1
#undef  _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 2

#include "config.h"

#include <errno.h>
#include <fcntl.h>
#ifdef HAVE_GETTEXT
#include <libintl.h>
#define _(String) gettext(String)
#else
#define _(String) String
#endif
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef USE_DMALLOC
#include "dmalloc.h"
#endif

#include "getopt.h"
#include "bas.h"
/*}}}*/

extern int use_color256;

int main(int argc, char *argv[]) /*{{{*/
{
  /* variables */ /*{{{*/
  char *runFile=(char*)0;
  int usage=0,o;
  const char *lp="/dev/null";
  int autoexec=0;
  int color256=0;
  int print_end=1;
  int backslash_colon=0;
  int uppercase=0;
  int restricted=0;
  int lpfd;
  static struct option lopts[]=
  {
    { "autoexec", no_argument, 0, 'a' },
    { "color256", no_argument, 0, 'c' },
    { "lp", required_argument, 0, 'l' },
    { "help", no_argument, 0, 'h' },
    { "no-end-program", no_argument, 0, 'n' },
    { "restricted", no_argument, 0, 'r' },
    { "uppercase", no_argument, 0, 'u' },
    { "backslash-colon", no_argument, 0, 'b' },
    { "version", no_argument, 0, 'V' },
#if defined(__STDC__) && __STDC__
    { (const char*)0, 0, 0, '\0' }
#else
    { (char*)0, 0, 0, '\0' }
#endif
  };
  /*}}}*/

  setlocale(LC_MESSAGES,"");
  setlocale(LC_CTYPE,"");
#ifdef HAVE_GETTEXT
  bindtextdomain("bas",LOCALEDIR);
  textdomain("bas");
#endif
  /* parse arguments */ /*{{{*/
  while ((o=getopt_long(argc,argv,"+abl:nruVh?",lopts,(int*)0))!=EOF) switch (o)
  {
    case 'a': autoexec=1; break;
    case 'b': backslash_colon=1; break;
    case 'c': color256=1; break;
    case 'l': lp=optarg; break;
    case 'n': print_end=0; break;
    case 'u': uppercase=1; break;
    case 'r': restricted=1; break;
    case 'V': printf("bas " VERSION "\n"); exit(0); break;
    case 'h': usage=2; break;
    default: usage=1; break;
  }
  if (optind<argc) runFile=argv[optind++];
  if (usage==1)
  {
    fputs(_("Usage: bas [-a] [-b] [-c] [-l file] [-n] [-r] [-u] [program [argument ...]]\n"),stderr);
    fputs(_("       bas [--backslash-colon] [--lp file] [--restricted] [--uppercase] [program [argument ...]]\n"),stderr);
    fputs(_("       bas [--autoexec] [--color256] [--no-end-program] [program [argument ...]]\n"),stderr);
    fputs(_("       bas -h|--help\n"),stderr);
    fputs(_("       bas --version\n"),stderr);
    fputs(  "\n",stderr);
    fputs(_("Try `bas -h' or `bas --help' for more information.\n"),stderr);
    exit(1);
  }
  if (usage==2)
  {
    fputs(_("Usage: bas [-a] [-b] [-c] [-l file] [-n] [-r] [-u] [program [argument ...]]\n"),stdout);
    fputs(_("       bas [--backslash-colon] [--lp file] [--restricted] [--uppercase] [program [argument ...]]\n"),stdout);
    fputs(_("       bas [--autoexec] [--color256] [--no-end-program] [program [argument ...]]\n"),stderr);
    fputs(_("       bas -h|--help\n"),stdout);
    fputs(_("       bas --version\n"),stdout);
    fputs("\n",stdout);
    fputs(_("BASIC interpreter.\n"),stdout);
    fputs("\n",stdout);
    fputs(_("-a, --autoexec         run './autoexec.bas' first\n"),stdout);
    fputs(_("-b, --backslash-colon  convert backslashs to colons\n"),stdout);
    fputs(_("-c, --color256         allow 'COLOR 255,255,255', terminal must support it\n"),stdout);
    fputs(_("-l, --lp               write LPRINT output to file\n"),stdout);
    fputs(_("-n, --no-end-program   dont print 'END program' in direct mode\n"),stdout);
    fputs(_("-r, --restricted       forbid SHELL\n"),stdout);
    fputs(_("-u, --uppercase        output all tokens in uppercase\n"),stdout);
    fputs(_("-h, --help             display this help and exit\n"),stdout);
    fputs(_("    --version          output version information and exit\n"),stdout);
    fputs("\n",stdout);
    fputs(_("Report bugs to <michael@moria.de>.\n"),stdout);
    exit(0);
  }
  if ((lpfd=open(lp,O_WRONLY|O_CREAT|O_TRUNC,0666))==-1)
  {
    fprintf(stderr,_("bas: Opening `%s' for line printer output failed (%s).\n"),lp,strerror(errno));
    exit(2);
  }
  bas_argc=argc-optind;
  bas_argv=&argv[optind];
  bas_argv0=runFile;
  use_color256=color256;
  /*}}}*/
  bas_init(backslash_colon,restricted,uppercase,print_end,lpfd);
  if (autoexec) bas_runFile(autoBas);
  if (runFile) bas_runFile(runFile);
  else bas_interpreter();
  bas_exit();
  return(0);
}
/*}}}*/
