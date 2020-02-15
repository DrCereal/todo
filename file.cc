/*  file.cc - Everything to do with File operations.
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

#include <fstream>
#include <iostream>

#include "file.hh"

std::ofstream out;

/* Write the entry to file (and it's children.)
 * The index bit is kind of hacky.  */
static void
write_entry(entry* e, std::string& parent_index, std::string& index)
{
  std::string new_index;
 
  /* Checked so we don't write root to file.  */
  if (e->get_parent() != NULL)
    out << "add " << e->get_task() << ' ' << parent_index << '\n';

  const int size = e->get_number_of_entries();
  for (int i = 0; i < size; i++)
    {
      new_index = index;
      if (new_index != "")
        new_index += '.';
      new_index += std::to_string(i + 1);

      write_entry(e->get_entry(i), index, new_index);
    }
}

int 
save_list (entry* root)
{
  out.open("default.td", std::ofstream::out);

  std::ios_base::iostate status = out.rdstate();
  if (status & std::ofstream::failbit)
    return -1;
  else
    {
      std::string parent_index = "";
      std::string index = "";
      write_entry(root, parent_index, index);

      /* For convenient viewing after loading in a list.  */
      out << "list\n";

      out.close();
    }
  return 0;
}
