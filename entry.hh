#ifndef ENTRY_HH
#define ENTRY_HH

#include <vector>

class entry
{
  public:
    entry(const std::string& task);
    entry();    
    

    void draw(int draw_children);

    void add_child(entry* e);
    void set_z(const int z);
    void set_task(const std::string& task);
    void set_parent(entry* e);

    entry* get_entry(int i);
    entry* get_last_entry();
    entry* get_parent() { return this->parent_; }

    void remove_child(entry* e);
    void delete_children();
  private:
    int z_;
    std::string task_;
    std::vector<entry*> children_;
    entry* parent_;
};

void delete_entry(entry* e);

#endif
