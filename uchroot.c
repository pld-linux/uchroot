/*
 * Changes (last at top):
 *
 * Mon Jul 30 2001 Michael Tokarev <mjt@corpit.ru>
 * removed getopt_long (for portability with FreeBSD)
 * implemented -d dir (initial directory in chroot jail) option
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <stdarg.h>
#include <string.h>

static char *progname;

static void err(int code, const char *fmt, ...) {
  va_list ap;
  fprintf(stderr, "%s: ", progname);
  va_start(ap, fmt); vfprintf(stderr, fmt, ap); va_end(ap);
  if (code)
    fprintf(stderr, ": %m");
  putc('\n', stderr);
  fflush(stderr);
  exit(1);
}

int main(int argc, char **argv) {
  struct passwd *pw = NULL;
  struct group *gr = NULL;
  const char *dir = "/";
  char *p;
  int c;
  if ((progname = strrchr(argv[0], '/')) != NULL)
    argv[0] = ++progname;
  else
    progname = argv[0];
  while((c = getopt(argc, argv, "+hu:d:")) != EOF)
    switch(c) {
    case 'u':
      if ((p = strchr(optarg, ':')) != NULL)
        *p++ = '\0';
      if ((pw = getpwnam(optarg)) == NULL)
        err(0, "unable to find user `%s'", optarg);
      if (p && (gr = getgrnam(p)) == NULL)
        err(0, "unable to find group `%s'", p);
      break;
    case 'd':
      dir = optarg;
      break;
    case 'h':
      printf("%s: change root\n\
Usage: %s [-u user[:group]] [-d dir] newroot [command args...]\n",
             progname, progname);
      return 0;
    default:
      return 1;
    }

  if (argc == optind)
    err(0, "new root not specified");

  if (chroot(argv[optind]) != 0)
    err(1, "unable to change root to `%s'", argv[optind]);
  if (chdir(dir) != 0)
    err(1, "unable to chdir to `%s'");

  if (pw) {
    if (setgid(gr ? gr->gr_gid : pw->pw_gid) != 0)
      err(1, "unable to set group");
    if (setuid(pw->pw_uid) != 0)
      err(1, "unable to set uid");
  }
  argv += optind + 1; argc -= optind + 1;
  if (argc) {
    execv(argv[0], argv);
    p = argv[0];
  }
  else {
    if (pw)
      p = pw->pw_shell;
    else if ((p = getenv("SHELL")) == NULL)
      p = "/bin/sh";
    execl(p, p, NULL);
  }
  err(1, "unable to execute %s", p);
  return 1;
}
