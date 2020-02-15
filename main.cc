/*  todo - A todo program for the command line as an exercise.
    Copyright (C) 2020 Jakob Brothwell

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>

#include <stdlib.h>

#include "entry.hh"
#include "file.hh"

#define error(msg) std::cerr << "error: " << msg << '\n';

#define run_func(cmd,func,expected,expected2) \
  if (tokens[0] == cmd) \
    { \
      if (tokens.size() == expected \
           || tokens.size() == expected2) \
        { \
          func; \
          tokens.clear(); \
          return; \
        } \
      else \
        { \
          std::cerr << cmd << " expected " << expected << " arguments\n"; \
          tokens.clear(); \
          return; \
        } \
    }

std::vector<std::string> tokens;
entry root("");

/* Gets an entry from passed index.
 * Example: 1.2.3  */
entry*
parse_index (const std::string& index)
{
  entry* e = &root;

  int current_index = 0;
  for (int i = 0; i < index.size(); i++)
    {
      if (isdigit(index[i]))
        {
          current_index *= 10;
          current_index += (index[i] - '0');
        }
      else if (index[i] == '.')
        {
          e = e->get_entry(--current_index);
          if (e == NULL)
            return NULL;
            
          current_index = 0;
        }
      else
        return NULL;
    }
  e = e->get_entry(--current_index);
  
  return e;
}

/* Adds an entry to the current list.  */ 
void
add ()
{
  entry* e = (entry*)malloc(sizeof(entry));
  if (e != NULL)
    {
      new(e) entry; /* Necessary for ensuring functions and objects are properly
defined.  */

      e->set_task(tokens[1]);
      if (tokens.size() == 2)
        root.add_child(e);  
      else
        {
          entry* parent = parse_index(tokens[2]);
          if (parent == NULL)
            {
              error("add: '" << tokens[2] << "' is not a valid index");
              free(e);
            }
          else
            parent->add_child(e);
        }
    }
  else
    error("malloc(): failed to allocate memory");
}

/* Edits an entry at the given index in the list. Children of an index are
 * specified by a '.'.  */
void
edit ()
{
  entry* e = parse_index(tokens[1]);
  if (e != NULL)
    e->set_task(tokens[2]);
  else
    error("edit: '" << tokens[1] << "' is not a valid index");
}

/* Clears the list of all entries.  */
void
clear ()
{
  root.delete_children();
}

/* Draws the current list to the screen for viewing.
 * TODO: list to a file.  */
void
list ()
{
  root.draw(1);
}

void
load ()
{

}

/* Removes an entry from the screen.
 * If there's no index passed, then it remove the last item added.  */
void
remove ()
{
  if (tokens.size() == 2)
    {
      entry* e = parse_index(tokens[1]);
      if (e != NULL)
        {
          e->delete_children();
          delete_entry(e);
        }
      else
        error("remove: '" << tokens[1] << "' is not a valid index");
    }
  else
    {
      /* TODO: Potentially change to delete the entry that was most recently 
       * added.  */
      entry* e = &root;

      /* Get the farthest descendent of the last entry.  */
      for (;;)
        {
          entry* le = e->get_last_entry();
          if (le != NULL)
            e = le;
          else
            break;
        }

      /* Verify that the entry we're deleting is not the root entry.  */
      if (e == &root)
        {
          std::cout << "remove: there is nothing to remove!\n";
          return;
        }
      else
        {
          e->delete_children();
          delete_entry(e);
        }
    }
}

/* Save the list to default.td, which must already exist.
 * TODO: Add saving to files with custom names.  */
void
save ()
{
  if (save_list(&root))
    error("there was an error saving");
}

/* Checks to see if issued command exists, and, if it does, runs it.  */
void
parse ()
{
  run_func("add", add(), 2, 3)
  run_func("clear", clear(), 1, 0)
  run_func("edit", edit(), 3, 0) 
  run_func("exit", exit(0), 1, 0)
  run_func("list", list(), 1, 0)
  run_func("load", load(), 2, 0)
  run_func("remove", remove(), 2, 1)
  run_func("save", save(), 1, 0)

  error("unknown command '" << tokens[0] << "'");

  tokens.clear();
}

int
main (int argc, char** argv)
{
  std::cout << ": ";

  char c;
  std::string current_token;
  for (;;)
    {
      c = fgetc(stdin);
      switch (c)
        {
          case ' ':
          case 10:
            if (current_token != "")
              {
                tokens.push_back(current_token);
                current_token = "";
              }

            if (c == 10)
              {
                parse();
                std::cout << ": ";
              }
            break;
          default:
            current_token.append(1, c);
            break;
        }
    }

  return 0;
}
