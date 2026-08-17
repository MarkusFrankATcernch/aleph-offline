/*
 *
 * Revision 1.1.1.1  2006/06/14
 * Kernlib  utility for locf, locb on LP64 architectures  H. Vogt
 * (AMD64/Intel EM64T and IA64) 
 *
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*>    ROUTINE LOCF
  CERN PROGLIB# N100    LOCF            .VERSION KERNFOR  4.36  930602
*/
/*>    ROUTINE LOCB
  CERN PROGLIB# N101    LOCB            .VERSION KERNFOR  4.36  930602
*/

unsigned int chkloc(iadr)
   char *iadr;
{
  /* 64 bit architectures may exceed the 32 bit address space !               */

  /* AMD64/Intel EM64T architectures have the dynamic segments above
     0x80000000000 and the stack immediately below this whereas the text and
     data segments are staring from 0x400000. The implementations address
     space is limited to 0x00007fffffffffff.
     Allocated memory with malloc/calloc is starting from the end of text and
     data segments upwards.                                                   

     IA64 architectures have the dynamic segments are above 0x2000000000000000,
     the stack is above 0x8000000000000000, the data segments starts at 
     0x6000000000000000 and the text segments start at 0x4000000000000000   
     Allocated memory with malloc/calloc is starting from the end of
     data segments upwards. All addresses here are expected to be in
     the data segment area.                                                   */

  /* K. McCarty: On Alpha, on the other hand, I couldn't find any docs for the
     Linux segmentation for virtual memory.  Judging by a test machine
     (escher.debian.org), the data section starts slightly above 0x120010000.
     On the other hand the stack grows downward from 0x120000000. */
  /* U. Schwickerath on aarch64 the stack grows downward as for Alpha */

   const unsigned long mask=0xffffffff00000000;
   static unsigned long limit=0x00000000ffffffff;
   unsigned long jadr=((unsigned long) iadr & mask);
#if defined (__ia64__)
    if ( jadr != 0x6000000000000000) {
#elif defined (__alpha__)
    if ( jadr != 0x0000000100000000) {
#elif  defined(__powerpc__)
    if ( jadr != 0x0000000100000000) {
      fprintf(stderr, "ppc64\n");
#elif  defined(__DARWIN__)
    if ( jadr != 0x0000000100000000) {
      fprintf(stderr, "darwin\n");
#else  /* amd64, aarch64, arm64 */

    /*
      In linux the common blocks and all other static data
      are located in the .bss data segment of the executable.
      We offset the start address of each data segment found
      from the address given.
      The data segments are defined in /proc/self/maps)
      and have no underlying file in sections like this:
      "
      555555554000-55555555b000 r--p 00000000 103:04 13529449                  <exe>
      55555555b000-555555783000 r-xp 00007000 103:04 13529449                  <exe>
      555555783000-5555557a8000 r--p 0022f000 103:04 13529449                  <exe>
      5555557a8000-5555557a9000 r--p 00254000 103:04 13529449                  <exe>
      5555557a9000-5555557b7000 rw-p 00255000 103:04 13529449                  <exe>
      5555557b7000-555556d47000 rw-p 00000000 00:00 0                   <<<<<< .bss
      555556d47000-555556d89000 rw-p 00000000 00:00 0                          [heap]
      7ffff6800000-7ffff6886000 r--p 00000000 00:40 746674                     <lib-1>
      ...
      7ffff6d96000-7ffff6e08000 rw-p 00000000 00:00 0                   <<<<<< data
      7ffff7000000-7ffff709d000 r--p 00000000 103:01 1442019                   <lib-2>
      ...
      7ffff6d96000-7ffff6e08000 rw-p 00000000 00:00 0                   <<<<<< data

      ....
      "
      So: we only check if the address is contained in one of the
      data sections. If yes, return the offset to the beginning
      of the section and off we go....

      But:
      This would have been nice, but is only half of the story:
      In Fortran the data are in the .bss section, which is the
      first data section after the main program. If all libraries
      are shared, the data sections are intermittend between the
      libraries as they are loaded. Then the relative offsets
      lead to trouble, because depending on the section a common block
      is in, data with the same offset may be in different sections.

      This means:
      The executable MUST be statically linked with all the Fortran code.
      C++ code can be hosted in dynamically linked libraries as long
      as the symbols from the main program are shared.
      Hence this function will work if the common block is located
      in the .bss section.

      To be investigated:
      It may be sufficient to check if the offset of the checked address
      from the beginning of the executable section (the very first
      entry in /proc/self/maps) is less than 2GB ( as for -m32! ).
      Up to 2GB then also storage in shared libraries would be
      considered.
      Bad addresses may then result in a SIGBUS, but are valid address
      space. This very likely would be way better than now simply
      exiting.

      M.Frank EP/LCB-CS
    */
    static uint64_t  base_address = 0;
    if( 0 == base_address )  {
       ssize_t  len = 0;
       size_t   sl = 0;
       char*    text = 0;
       FILE*    in = fopen("/proc/self/maps", "r");

       for( len=0, sl=0; (len=getline(&text, &sl, in)) != -1; )  {
          uint64_t start=0, end=0;
          if( 2 != sscanf(text, "%lx-%lx ", &start, &end) )  {
             perror("Failed to get address of data segment.");
             exit(EINVAL);
          }
          base_address = start;
          break;
      }
      close(in);
      if( text ) free(text);
      printf("+++ Found base offset 0x%016lx for data segments. "
             "LOCF checks if address is with data segments.\n", base_address);
      fflush(stdout);
   }
   iadr -= base_address;
   jadr  = ((unsigned long) iadr & mask);
   if ( jadr != 0) {
      fprintf(stderr, "amd64, ppc64 or aarch64\n");
#endif
      fprintf(stderr, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
      fprintf(stderr, "LOCB/LOCF: address %p exceeds the 32 bit address space\n", iadr);
      fprintf(stderr, "or is not in the data segments\n");
      fprintf(stderr, "This may result in program crash or incorrect results\n");
      fprintf(stderr, "Therefore we will stop here\n");
      fprintf(stderr, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
      exit (999);
    }
    jadr=((unsigned long) iadr & limit);
    return ((unsigned) jadr);
}
/*> END <----------------------------------------------------------*/

    float etime_()  {  return 0e0; }
