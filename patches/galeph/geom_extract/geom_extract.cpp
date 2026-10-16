
#include <cstdint>

namespace  {
  class geant3_geometry_extractor  {
  public:
    typedef void (*gpmate__t)     (int32_t num);
    gpmate__t  _real_gpmate_ { nullptr };
    
  public:
    /// Default constructor
    geant3_geometry_extractor();
    /// Default destructor
    ~geant3_geometry_extractor();
    void* handle = 0;

    void start();
  };
}

#include <dlfcn.h>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

namespace  {

  geant3_geometry_extractor* s_extractor = nullptr;

  template <typename FUNC_TYPE> inline FUNC_TYPE get_sym(void* handle, const char* name)   {
    FUNC_TYPE fun = FUNC_TYPE( ::dlsym(handle, name) );
    if ( nullptr == fun )   {
      char text[256];
      ::snprintf(text,sizeof(text),"Failed to initialize logger function %s",name);
      throw std::runtime_error(text);
    }
    return fun;
  }
}
  
/// Default constructor
geant3_geometry_extractor::geant3_geometry_extractor()  {
  handle = ::dlopen(nullptr, RTLD_LOCAL);
}

/// Default destructor
geant3_geometry_extractor::~geant3_geometry_extractor()  {
}

#define f77_SYM(x)   if ( _real_##x == nullptr )    \
    _real_##x = get_sym< x##_t > ( handle, #x )
  
void geant3_geometry_extractor::start()  {
  f77_SYM(gpmate_);
}

extern "C" void iniext_(int)  {
  if( s_extractor == nullptr )  {
    s_extractor = new geant3_geometry_extractor();
    s_extractor->start();
  }
}

#if 0
/// Iniitalize the logger unit
__attribute__((constructor)) void s__initialize_geant3_geoextractor()    {
  if( s_extractor == nullptr )  {
    s_extractor = new geant3_geometry_extractor();
    s_extractor->start();
  }
}
#endif

/// Shutdown the loggetr unit
__attribute__((destructor))  void s__finalize_geant3_geoextractor()    {
  if( s_extractor != nullptr )  {
    delete s_extractor;
    s_extractor = nullptr;
  }
}
