/*
 * Trivial finger server
 * Copyright (C) 2010, 2014 Richard Kettlewell
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <config.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

/* We expect to be run from inetd and to have the connection to the
 * client on stdin/stdout.  The client is either trying to be
 * compliant with RFC1288, or alternatively up to no good. 
 *
 * Highlights of the RFC and how we interpret it here
 * ==================================================
 *
 * Data MUST be ASCII (s2.2) and lines end CRLF.  There's some vague
 * wording about octets 128-255 but not enough information for anyone
 * to interpret them correctly.  We deal with this ambiguity by not
 * allowing non-ASCII characters in the input, but not attempting to
 * sanitize user-provided output (plan/project) in any way - that is
 * the client's responsibility.
 *
 * s2.3 gives the grammar for the input in an opaque style and an
 * undocumented format; it is also wrong.  I reproduce my
 * corrected interpretation of it here:
 *
 *    query = "/W" spaces_opt CR LF
 *          | w_opt username CR LF
 *          | w_opt hosts CR LF
 *          | w_opt username hosts CR LF
 *
 *    spaces_opt = {SPACE}
 *
 *    w_opt = "/W" spaces
 *
 *    spaces = SPACE {SPACE}
 *
 *    hosts = "@" hostname {"@" hostname}
 *
 *    username = <not documented>
 *
 *    hostname = <not documented>
 *
 * CR means octet 13; LF means octet 10; SPACE means octet 32.
 *
 * {} means the contents are optional and may be repeated; | means "or".
 *
 * The corrections I've added is to support "username CR LF", without
 * an initial /W, and to allow "/W spaces CR LF".  These are
 * prohibited by the RFC1288 grammar, but it's what the client
 * actually sends.
 *
 * Hostname syntax isn't documented.  However, this implementation
 * does not support forwarding, so the point is moot: all queries with
 * a "hosts" part are rejected.
 *
 * Username syntax isn't documented either.  This finger daemon
 * accepts ASCII letters and digits only.  Anything else is forbidden.
 */

static int isuserchar(int c) {
  if(c >= 'a' && c <= 'z')
    return 1;
  if(c >= 'A' && c <= 'Z')
    return 1;
  if(c >= '0' && c <= '9')
    return 1;
  return 0;
}

static char *make_full_path(const char *dir, const char *file) {
  char *buffer = malloc(strlen(dir) + strlen(file) + 2);
  if(!buffer) {
    fputs("Out of memory\n", stderr);
    exit(-1);
  }
  strcpy(buffer, dir);
  strcat(buffer, "/");
  strcat(buffer, file);
  return buffer;
}

static void show_file(const struct passwd *pw,
                      const char *heading,
                      const char *file) {
  char *full_path = make_full_path(pw->pw_dir, file);
  FILE *fp = fopen(full_path, "r");
  free(full_path);
  if(!fp)
    return;
  printf("%s:\r\n", heading);
  /* Transmit the file, translating to wire format */
  int last = -1, c;
  while((c = getc(fp)) != EOF) {
    if(c == '\n')
      putchar('\r');
    putchar(c);
    last = c;
  }
  /* Make sure there is a final CR LF */
  if(last != '\n')
    fputs("\r\n", stdout);
  fclose(fp);
}

int main(void) {
  char input[1024], user[sizeof input] = { 0 };
  const char *s;

  /* We don't want to be root. */
  assert(getuid());
  assert(geteuid());
  /* Read the query */
  if(fgets(input, sizeof input, stdin) == NULL) {
    if(ferror(stdin))
      perror("stdin");
    return -1;
  }
  /* Sanity check */
  assert(memchr(input, 0, sizeof input) != NULL);
  s = input;
  /* Queries may start with a /W */
  if(s[0] == '/' && s[1] == 'W') {
    s += 2;
    /* /W must be followed by spaces or CRLF */
    if(*s == ' ') {
      while(*s == ' ')
        ++s;
    } else if(s[0] == '\r' && s[1] == '\n')
      ;
    else {
      fputs("Malformed query.\n", stderr);
      return -1;
    }
  }
  /* Queries may contain a username */
  if(isuserchar((unsigned char)*s)) {
    char *u = user;
    while(isuserchar((unsigned char)*s))
      *u++ = *s++;
    *u = 0;
    /* user[] is the same size as input[], so there is no chance of
     * overflow. */
  }
  /* Queries might contain a @hostname sequence, which we would check
   * here, but this implementation does not support forwarding,
   * so we don't. */
  if(s[0] == '@') {
    fputs("Forwarding is not supported, sorry.\r\n", stderr);
    return 0;
  }
  /* We'd better have reached the end of the line. */
  if(!(s[0] == '\r' && s[1] == '\n')) {
    fputs("Malformed query.\n", stderr);
    return -1;
  }
  /* For now we don't support enumeration of logged-in users.  This
   * could be supported but still seems like a privacy violation. */
  if(!*user) {
    fputs("User enumeration is not supported, sorry.\n", stderr);
    return -1;
  }
  const struct passwd *pw = getpwnam(user);
  if(!pw) {
    printf("No such user.\r\n");
    return 0;
  }
  /* Basic information about the user */
  printf("Username: %s\r\n", user);
  const char *end = strchr(pw->pw_gecos, ',');
  if(!end)
    end = pw->pw_gecos + strlen(pw->pw_gecos);
  printf("Name:     %.*s\r\n", (int)(end - pw->pw_gecos), pw->pw_gecos);
  /* Information the user wants to publish */
  show_file(pw, "Project", ".project");
  show_file(pw, "Plan", ".plan");
  return 0;
}

/*
Local Variables:
c-basic-offset:2
comment-column:40
fill-column:79
indent-tabs-mode:nil
End:
*/
