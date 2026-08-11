#include <cerrno>

extern "C" int system(const char*);
extern "C" int system_(const char* cmd)  {
  return cmd ? ::system(cmd) : EINVAL;
}

extern "C" void vgsrch_()  {
}
