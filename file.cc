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

int 
save_list (entry* root)
{
  std::ofstream out;
  out.open("~/Documents/default.td", std::ofstream::out);

  std::ios_base::iostate status = out.rdstate();
  if (status & std::ofstream::failbit)
    return -1;
  else
    {
      /* TODO: Write to file.  */

      out.close();
    }
  return 0;
}
