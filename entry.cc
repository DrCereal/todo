/*  entry.cc - A class which handles entry management.
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

#include "entry.hh"

/* Initialize an entry.  */
entry::entry (const std::string& task)
{
  this->z_ = 0;
  this->task_ = task;
  this->parent_ = NULL;
}

/* Needed for the new operator.  */
entry::entry ()
{
  this->z_ = 0;
  this->task_ = "";
  this->parent_ = NULL;
}

/* Draw the entry (and it's children if the argument is true) to the
 * terminal.  */
void
entry::draw (int draw_children)
{
  if (this->task_ != "")
    {
      /* z_ - 1 to account for the root entry.  */
      for (int i = 0; i < z_ - 1; i++)
        std::cout << "  ";
      std::cout << " * " << task_ << '\n';
    }

  if (draw_children)
    {
      for (entry* e : children_)
        e->draw(1);
    }
}
  
/* Add a child.  */
void
entry::add_child (entry* e)
{
  this->children_.push_back(e);

  e->set_z(this->z_ + 1);
  e->set_parent(this);
}

/* Set the entry's "depth" on screen.  */
void
entry::set_z (const int z)
{
  this->z_ = z;
}

/* Changes what the task displays.  */
void
entry::set_task (const std::string& task)
{
  this->task_ = task;
}

/* Set the parent of the entry.  */
void
entry::set_parent (entry* e)
{
  this->parent_ = e;
}

/* If the index is not out of bounds, return the corresponding entry.  */
entry*
entry::get_entry (int index)
{
  if (index >= this->children_.size()
      || index < 0)
    return NULL;
  else
    return this->children_[index];
}

/* Return the last child entry.  */
entry*
entry::get_last_entry ()
{
  int index = this->children_.size() - 1;
  if (index >= 0)
    return this->children_[index];
  else
    return NULL;
}

/* Remove a child from vector.  */
void
entry::remove_child (entry* e)
{
  for (int i = 0; i < children_.size(); i++)
    {
      if (e == children_[i])
        {
          children_.erase(children_.begin() + i);
          return;
        }
    }
}

/* Delete an entry and free its memory.
 * A call to this method must be proceeded by delete_children().  */
void
delete_entry (entry* e)
{
  entry* p = e->get_parent();
  p->remove_child(e);

  free(e);
}

/* Deletes children of entry.  */
void
entry::delete_children ()
{
  for (entry* e : children_)
    {
      e->delete_children();
      delete_entry(e);
    }
}
